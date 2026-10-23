// SPDX-License-Identifier: MIT
// prova_gate.cpp — gate Tappa 1 (strada che scorre), headless, core-only (niente SDL).
// Build : cmake --build build --target prova_gate
// Uso   : ./build/prova_gate [cartella_output=out/debugger] [livello=lv_01_neon] [modo]
//         modo = smoke (500 frame, default) | soak (30000 frame) |
//                avvolgimento (playerZ oltre totalLen_) | inattivita (show dopo 10 s senza tasti) |
//                percorso (profilo del tracciato + un giro intero con guida automatica) |
//                espansione (verifica espansione tracciato su tutti i 6 livelli) |
//                gara (tappe con bivi: le 4 combinazioni di scelte arrivano alle 6 piste)
// Esegue la stessa simulazione 2 volte (seed e input fissi) e verifica:
//   1. partenza: playerSpeed > 0 al frame 60 (smoke/soak)
//   2. determinismo: frameHash identico ogni 60 frame fra run A e run B
//   3. prestazioni: ms/frame medio e massimo (obiettivo <= 20, meglio <= 12)
//   4. avvolgimento: playerZ torna a 0 al superare totalLen_ (nessuna discesa intermedia)
//   5. inattivita: dopo 500 frame senza input un show e' attivo e il fumetto e' nel screenshot
// Scrive: <out>/prova_gate*.log, screenshot PPM.
// Esegui dalla radice progetto (sglavinated_src): i dati sono letti con percorsi relativi.
// Exit code: 0 OK | 1 niente partenza | 2 replay non deterministico | 3 troppo lento |
//            4 cwd errata | 5 caso di prova non superato | 11 espansione tracciato non coerente |
//            13 percorso poco vario o non guidabile | 14 gara a tappe non completa
#include <cstdint>
using i32 = std::int32_t;
#include "game/game.h"
#include "render/framebuffer.h"
#include "render/sprite_raw.h"
#include "core/json_mini.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace sgl;

// Frequenza logica da dati/config.json: definizioni in src/game/game.cpp
// (spazio globale). Dichiarate qui per evitare ambiguità con la dichiarazione
// sgl::* visibile tramite using namespace sgl.
sgl::i32 leggiFrequenzaDaConfig();
sgl::i32 impostaFrequenzaDaConfig(sgl::i32 hz);

namespace {

constexpr i32 SMOKE_FRAMES = 500;   // 10 s @ 50 Hz (default)
constexpr i32 SOAK_FRAMES  = 30000; // 10 min @ 50 Hz (accettazione)
constexpr i32 INATTIVITA_FRAMES = 900; // inattivita: 60 frame di input, poi silenzio
constexpr i32 AVVOLGIMENTO_FRAMES = 60000; // abbastanza per diversi giri del circuito
constexpr i32 PIXEL_MIN = 100;      // soglia pixel per ritenere presente un rettangolo di show
constexpr double MAX_MS_FRAME = 20.0;
constexpr f32 MIN_SPEED_FIRST_FRAME = 8.0f;
constexpr f32 MIN_SPEED_FRAME_60 = 150.0f;
constexpr f32 MIN_ROAD_TRAVEL_FRAME_60 = 12000.0f;

struct Esito {
    std::vector<u32> hash;      // frameHash ogni frame (confronto fra run)
    f32 speedFirstFrame = 0.0f;
    f32 speed60 = 0.0f;
    i32 firstOffroad = -1;
    double msTot = 0.0, msMax = 0.0;
    f32 maxX = 0.0f, minX = 0.0f, zFinale = 0.0f;
};

bool accelerazionePronta(const Esito& esito) {
    return esito.speedFirstFrame >= MIN_SPEED_FIRST_FRAME &&
           esito.speed60 >= MIN_SPEED_FRAME_60;
}

// PPM P6; framebuffer = 0xAARRGGBB.
void salvaPPM(const std::string& path, const u32* px, int w, int h) {
    std::FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) { std::perror(path.c_str()); return; }
    std::fprintf(f, "P6\n%d %d\n255\n", w, h);
    for (int i = 0; i < w * h; ++i) {
        std::fputc((px[i] >> 16) & 0xFF, f);
        std::fputc((px[i] >> 8) & 0xFF, f);
        std::fputc(px[i] & 0xFF, f);
    }
    std::fclose(f);
}

// Cartelloni sponsor veri (raw SGLR generati da strumenti_py/png_a_raw.py): stesso
// ordine e stessi sponsor_id del runner (runner/sponsor_loader.cpp), cosi' il gate
// disegna gli stessi sprite del gioco. Da chiamare PRIMA di init().
void caricaSponsor(Game& game) {
    static const char* kSponsorId[7] = {
        "curve_pericolose", "brinis_udine", "piston_high", "canna_bis",
        "slow_down", "erba_medica", "green_box"};
    static const char* kStile[3] = { "neon", "atomico", "foglie" };
    std::vector<std::pair<std::string, Sprite>> sprites;
    for (const char* id : kSponsorId) {
        for (const char* stile : kStile) {
            Sprite s = caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/sponsor/") + id + "_" + stile + ".sglraw");
            if (s.width > 0) sprites.push_back({std::string(id) + "/" + stile, std::move(s)});
        }
    }
    game.setSponsorSprites(std::move(sprites));
}

u64 padPerFrame(i32 frameAssoluto) {
    const i32 frame = frameAssoluto % SMOKE_FRAMES; // il ciclo di 500 frame si ripete nel soak
    if (frame <= 100) return Pad::A;                 // dritto, accelera
    if (frame <= 170) return Pad::A | Pad::Left;     // sterzo sinistra (X negativo)
    return Pad::A | Pad::Right;                      // destra a oltranza: curva, poi fuoristrada
}

Esito esegui(const std::string& outDir, const std::string& livello, bool scrivi, i32 totalFrames) {
    Esito e;
    Game game(12345u);
    caricaSponsor(game); game.init(livello);
    std::ofstream log;
    if (scrivi) log.open(outDir + "/prova_gate.log");
    bool sc0 = false, sc1 = false, sc2 = false;
    for (i32 frame = 0; frame < totalFrames; ++frame) {
        Pad pad{};
        pad.held = padPerFrame(frame);
        game.setInput(pad);
        auto t0 = std::chrono::steady_clock::now();
        game.update();
        game.render();
        double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
        e.msTot += ms; if (ms > e.msMax) e.msMax = ms;

        const GameState& s = game.state();
        if (s.playerOffroad && e.firstOffroad < 0) e.firstOffroad = frame;
        if (frame == 0) e.speedFirstFrame = s.playerSpeed;
        if (frame == 60) e.speed60 = s.playerSpeed;
        if (s.playerX > e.maxX) e.maxX = s.playerX;
        if (s.playerX < e.minX) e.minX = s.playerX;
        e.zFinale = s.playerZ;
        u32 h = game.frameHash();
        e.hash.push_back(h);
        if (scrivi) {
            log << frame << ' ' << s.playerSpeed << ' ' << s.playerX << ' ' << s.playerZ
                << ' ' << (s.playerOffroad ? 1 : 0) << ' ' << h << '\n';
            const u32* d = game.fb().data();
            if (!sc0 && frame == 1)   { salvaPPM(outDir + "/prova_gate_partenza.ppm", d, Framebuffer::WIDTH, Framebuffer::HEIGHT); sc0 = true; }
            if (!sc1 && frame == 250) { salvaPPM(outDir + "/prova_gate_curva.ppm", d, Framebuffer::WIDTH, Framebuffer::HEIGHT); sc1 = true; }
            // NOTA: `frame` e' gia' frameAssoluto % SMOKE_FRAMES, quindi SMOKE_FRAMES - 1 e' CORRETTO anche nel soak.
            // NON sostituire con TOTAL_FRAMES - 1 (non verrebbe mai raggiunto). Falso positivo del reviewer, 17 ott.
            if (!sc2 && (frame == e.firstOffroad + 5 || frame == SMOKE_FRAMES - 1) && frame > 250) {
                salvaPPM(outDir + "/prova_gate_fuoristrada.ppm", d, Framebuffer::WIDTH, Framebuffer::HEIGHT); sc2 = true;
            }
        }
    }
    return e;
}

bool verificaRibaltamento(const std::string& livello) {
    Game game(12345u);
    caricaSponsor(game);
    game.init(livello);

    bool haRibaltato = false;
    for (i32 frame = 0; frame < 1500; ++frame) {
        Pad pad{};
        pad.held = Pad::A | Pad::Right;
        game.setInput(pad);
        game.update();
        game.render();
        if (game.state().playerFlipping) {
            haRibaltato = true;
            break;
        }
    }
    if (!haRibaltato) return false;

    const f32 posizioneIncidente = game.state().playerZ;
    // Aspetta che l'animazione di ribaltamento (FLIP_ANIMATION_S = 1.5 s in game.h)
    // sia finita: durante il ribaltamento l'auto resta ferma. Durata in secondi ->
    // frame alla frequenza corrente, cosi' il test e' identico a 50 e 60 Hz.
    i32 frameFlip = (i32)(1.5f * (f32)::leggiFrequenzaDaConfig());
    bool flipFinito = false;
    for (i32 frame = 0; frame < frameFlip; ++frame) {
        game.setInput(Pad{});
        game.update();
        game.render();
        if (!game.state().playerFlipping) { flipFinito = true; break; }
    }
    if (!flipFinito) return false;

    // L'auto deve restare ferma al punto dell'incidente.
    for (i32 frame = 0; frame < 75; ++frame) {
        game.setInput(Pad{});
        game.update();
        game.render();
        if (game.state().playerZ != posizioneIncidente) return false;
    }
    if (game.state().playerFlipping || game.state().playerX != 0.0f ||
        game.state().playerSpeed != 0.0f || game.state().playerZ != posizioneIncidente)
        return false;

    Pad gas{};
    gas.held = Pad::A;
    for (i32 frame = 0; frame < 60; ++frame) {
        game.setInput(gas);
        game.update();
        game.render();
    }
    return game.state().playerSpeed >= MIN_SPEED_FRAME_60;
}

bool verificaAccelerazione(const std::string& livello) {
    Game game(12345u);
    game.init(livello);
    Pad gas{};
    gas.held = Pad::A;

    f32 dt = 1.0f / (f32)::leggiFrequenzaDaConfig();
    i32 framesTotali = (i32)(10.0f / dt);   // 10 s alla frequenza corrente
    i32 frameDopo1s2 = (i32)(1.2f / dt);    // checkpoint a 1.2 s

    f32 speedPrecedente = 0.0f;
    f32 speedPrimoFrame = 0.0f;
    f32 speedDopo1s2 = 0.0f;
    f32 avanzamentoPrimi1s2 = 0.0f;
    f32 avanzamentoStrada = 0.0f;
    for (i32 frame = 0; frame < framesTotali; ++frame) {
        game.setInput(gas);
        game.update();
        game.render();
        const f32 speed = game.state().playerSpeed;
        if (frame == 0) speedPrimoFrame = speed;
        if (frame == frameDopo1s2 - 1) {
            speedDopo1s2 = speed;
            avanzamentoPrimi1s2 = game.state().playerZ;
        }
        if (speed + 0.001f < speedPrecedente) return false;
        speedPrecedente = speed;
        avanzamentoStrada = game.state().playerZ;
    }

    std::printf("accelerazione 0-10s: primo frame=%.2f, 1.2s=%.2f, 10s=%.2f; "
                "strada 1.2s=%.1f, posizione finale=%.1f (dt=%.4f)\n",
                speedPrimoFrame, speedDopo1s2, speedPrecedente, avanzamentoPrimi1s2, avanzamentoStrada, dt);
    return speedPrimoFrame >= MIN_SPEED_FIRST_FRAME &&
           speedDopo1s2 >= MIN_SPEED_FRAME_60 &&
           avanzamentoPrimi1s2 >= MIN_ROAD_TRAVEL_FRAME_60 &&
           speedPrecedente >= 240.0f && speedPrecedente <= 250.0f;
}

bool verificaContinuitaCurve() {
    Road road;
    road.build({
        {0.0f, 0.0f, 200},
        {1.0f, 0.0f, 200},
        {-1.0f, 0.0f, 200},
        {0.0f, 0.0f, 200}
    }, 1u);
    std::vector<Road::ProjectedSeg> projected;
    road.project(0.0f, 0.0f, projected);

    f32 pendenzaPrecedente = 0.0f;
    f32 saltoPendenzaMassimo = 0.0f;
    for (size_t i = 1; i < projected.size(); ++i) {
        const f32 dz = projected[i].worldZ - projected[i - 1].worldZ;
        if (dz <= 0.0f) return false;
        const f32 pendenza = (projected[i].worldX - projected[i - 1].worldX) / dz;
        if (i > 1)
            saltoPendenzaMassimo = std::max(saltoPendenzaMassimo,
                                            std::abs(pendenza - pendenzaPrecedente));
        pendenzaPrecedente = pendenza;
    }

    std::printf("continuita' curve: campioni=%zu, salto pendenza max=%.2f\n",
                projected.size(), saltoPendenzaMassimo);
    return projected.size() >= (size_t)Road::DRAW_DIST * 3 &&
           saltoPendenzaMassimo <= 0.51f;
}

bool verificaSterzo() {
    f32 dt = 1.0f / (f32)::leggiFrequenzaDaConfig();
    i32 framesSterzo = (i32)(0.6f / dt);   // 0.6 s alla frequenza corrente
    auto prova = [dt, framesSterzo](u64 direzione) {
        Player player;
        Pad gasSterzo{};
        gasSterzo.held = Pad::A | direzione;
        for (i32 frame = 0; frame < framesSterzo; ++frame) {
            player.input(gasSterzo, dt);
            player.update(dt);
        }
        return player.x;
    };

    const f32 sinistra = prova(Pad::Left);
    const f32 destra = prova(Pad::Right);
    std::printf("sterzo %.1f s: sinistra=%.1f destra=%.1f (dt=%.4f)\n", framesSterzo * dt, sinistra, destra, dt);
    return sinistra <= -200.0f && destra >= 200.0f &&
           std::abs(sinistra + destra) <= 0.1f;
}

// --- Caso nuovo: freno (Pad::B / Down) riduce la velocita' ---
bool verificaFrenata() {
    f32 dt = 1.0f / (f32)::leggiFrequenzaDaConfig();
    i32 framesFase = (i32)(2.0f / dt);   // 2 s per fase alla frequenza corrente
    Player player;
    Pad gas{}; gas.held = Pad::A;
    for (i32 frame = 0; frame < framesFase; ++frame) { player.input(gas, dt); player.update(dt); }
    const f32 speedPrima = player.speed;
    Pad freno{}; freno.held = Pad::B;
    for (i32 frame = 0; frame < framesFase; ++frame) { player.input(freno, dt); player.update(dt); }
    const f32 speedDopo = player.speed;
    std::printf("frenata: prima=%.2f dopo=%.2f (BRAKE=120, dt=%.4f, fase=%.1f s)\n", speedPrima, speedDopo, dt, framesFase * dt);
    return speedPrima > 60.0f && speedDopo <= speedPrima * 0.3f && speedDopo >= 0.0f;
}

// --- Caso nuovo: gas rilasciato -> coast (COAST=0.995), decadimento graduale e monotono ---
bool verificaCosta() {
    f32 dt = 1.0f / (f32)::leggiFrequenzaDaConfig();
    i32 framesAccel = (i32)(3.0f / dt);    // 3 s di accelerazione
    i32 framesCosta = (i32)(5.0f / dt);    // 5 s di inerzia
    Player player;
    Pad gas{}; gas.held = Pad::A;
    for (i32 frame = 0; frame < framesAccel; ++frame) { player.input(gas, dt); player.update(dt); }
    const f32 speedPrima = player.speed;
    Pad nullo{};
    f32 speedPre = speedPrima;
    bool decrescente = true;
    for (i32 frame = 0; frame < framesCosta; ++frame) {
        player.input(nullo, dt);
        player.update(dt);
        if (player.speed > speedPre + 0.001f) decrescente = false;
        speedPre = player.speed;
    }
    std::printf("costa: prima=%.2f dopo5s=%.2f decrescente=%d (dt=%.4f)\n", speedPrima, player.speed, decrescente, dt);
    return speedPrima > 100.0f && player.speed < speedPrima * 0.45f && decrescente;
}

// --- Caso nuovo: avvolgimento di playerZ oltre totalLen_ ---
// Guida tenuta vicino al centro: accelera sempre, sterza verso centro quando |x| e' alto.
Pad padAvvolgimento(const Game& game) {
    Pad pad{}; pad.held = Pad::A;
    const f32 x = game.state().playerX;
    if (x > 120.0f) pad.held |= Pad::Left;
    else if (x < -120.0f) pad.held |= Pad::Right;
    return pad;
}

struct EsitoAvvolgimento {
    std::vector<u32> hashA, hashB;
    i32 wrapCount = 0;
    f32 zMax = 0.0f, stimaTotalLen = 0.0f, zFinale = 0.0f;
    bool anomalia = false;
};

EsitoAvvolgimento eseguiAvvolgimento(const std::string& outDir, const std::string& livello, bool scrivi, i32 totalFrames) {
    EsitoAvvolgimento e;
    Game game(12345u);
    caricaSponsor(game); game.init(livello);
    std::ofstream log;
    if (scrivi) log.open(outDir + "/prova_gate_avvolgimento.log");
    f32 zMax = 0.0f;
    bool primaWrap = true;
    for (i32 frame = 0; frame < totalFrames; ++frame) {
        Pad pad = padAvvolgimento(game);
        game.setInput(pad);
        game.update();
        game.render();
        u32 h = game.frameHash();
        e.hashA.push_back(h);
        const GameState& s = game.state();
        const f32 z = s.playerZ;
        if (scrivi) log << frame << ' ' << s.playerSpeed << ' ' << s.playerX << ' ' << z << ' ' << (s.playerOffroad ? 1 : 0) << ' ' << h << '\n';
        if (scrivi && frame == totalFrames / 2)
            salvaPPM(outDir + "/prova_gate_avvolgimento.ppm", game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
        if (z < 0.0f) { e.anomalia = true; if (scrivi) log << "ANOMALIA z<0 frame " << frame << '\n'; }
        if (z < zMax - 5.0f) {
            if (z < zMax - 1000.0f) { // discesa grande = avvolgimento del circuito circolare
                e.wrapCount++;
                if (primaWrap) { e.stimaTotalLen = zMax; primaWrap = false; }
                zMax = z; // dopo l'avvolgimento si riparte dal fondo del nuovo giro
            } else { // discesa intermedia non giustificata da avvolgimento
                e.anomalia = true;
                if (scrivi) log << "ANOMALIA discesa z frame " << frame << " zMax " << zMax << " z " << z << '\n';
            }
        }
        if (z > zMax) zMax = z;
    }
    e.zMax = zMax; e.zFinale = game.state().playerZ;
    // Secondo run identico: determinismo.
    Game game2(12345u); caricaSponsor(game2); game2.init(livello);
    for (i32 frame = 0; frame < totalFrames; ++frame) {
        Pad pad = padAvvolgimento(game2);
        game2.setInput(pad);
        game2.update();
        game2.render();
        e.hashB.push_back(game2.frameHash());
    }
    return e;
}

// --- Caso nuovo: inattivita' (10 s senza tasti) -> show/fumetto visibile nello screenshot ---
Pad padInattivita(i32 frame) {
    Pad pad{};
    if (frame < 60) pad.held = Pad::A; // input iniziali (arma inattivita), poi silenzio assoluto
    return pad;
}

i32 contaPixel(const u32* px, int w, int h, u32 r, u32 g, u32 b) {
    i32 n = 0;
    for (int i = 0; i < w * h; ++i)
        if (((px[i] >> 16) & 0xFF) == r && ((px[i] >> 8) & 0xFF) == g && (px[i] & 0xFF) == b)
            ++n;
    return n;
}

struct EsitoInattivita {
    std::vector<u32> hashA, hashB;
    i32 maxBollaDopoIdle = 0;   // max pixel blu (bolla) dopo i 500 frame di inattivita
    i32 maxPersonaggioDopoIdle = 0;
};

EsitoInattivita eseguiInattivita(const std::string& outDir, const std::string& livello, bool scrivi, i32 totalFrames) {
    EsitoInattivita e;
    Game game(12345u);
    caricaSponsor(game); game.init(livello);
    std::ofstream log;
    if (scrivi) log.open(outDir + "/prova_gate_inattivita.log");
    for (i32 frame = 0; frame < totalFrames; ++frame) {
        Pad pad = padInattivita(frame);
        game.setInput(pad);
        game.update();
        game.render();
        u32 h = game.frameHash();
        e.hashA.push_back(h);
        const GameState& s = game.state();
        const u32* d = game.fb().data();
        i32 blu = contaPixel(d, Framebuffer::WIDTH, Framebuffer::HEIGHT, 0, 0, 255);     // fumetto: bolla blu
        i32 verde = contaPixel(d, Framebuffer::WIDTH, Framebuffer::HEIGHT, 0, 255, 0); // personaggio verde
        if (frame >= 560) { // oltre i 500 frame di inattivita (idle dal frame 60)
            if (blu > e.maxBollaDopoIdle) e.maxBollaDopoIdle = blu;
            if (verde > e.maxPersonaggioDopoIdle) e.maxPersonaggioDopoIdle = verde;
        }
        if (scrivi) log << frame << ' ' << s.playerSpeed << ' ' << s.playerX << ' ' << s.playerZ
                        << ' ' << (s.playerOffroad ? 1 : 0) << ' ' << blu << ' ' << verde << ' ' << h << '\n';
    }
    // Secondo run identico: determinismo.
    Game game2(12345u); caricaSponsor(game2); game2.init(livello);
    for (i32 frame = 0; frame < totalFrames; ++frame) {
        Pad pad = padInattivita(frame);
        game2.setInput(pad);
        game2.update();
        game2.render();
        e.hashB.push_back(game2.frameHash());
    }
    return e;
}

} // namespace


// Screenshot P1 (gate Tappa 1): tre immagini scelte in automatico guidando
// 3000 frame (60 s): la curva piu' marcata, il cartellone sponsor piu' grande,
// il primo frame con personaggio e fumetto a schermo. Servono agli agenti che
// non possono lanciare il runner a finestra: escono dallo stesso gate_t1.sh.
namespace {
bool coloreMotore(u32 c) {
    static const u32 kMotore[] = {
        0xFF000000u, 0xFFFFFFFFu, 0xFF7A7A7Au, 0xFF5A5A5Au, 0xFF3A3A3Au, 0xFFFF00FFu,
        0xFF202040u, 0xFF00AA00u, 0xFFFF8800u, 0xFF00FFFFu, 0xFFFFFF00u, 0xFFFF0000u,
        0xFF00FF00u, 0xFF444444u, 0xFFAAAAAAu, 0xFF008C00u, 0xFFFFF4B0u, 0xFF0000FFu};
    for (u32 m : kMotore) if (c == m) return true;
    return false;
}

// Centro della carreggiata (pixel grigio asfalto) sulla riga y, -1 se assente.
f32 centroStrada(const u32* d, i32 y) {
    i32 somma = 0, n = 0;
    for (i32 x = 0; x < Framebuffer::WIDTH; ++x) {
        const u32 c = d[y * Framebuffer::WIDTH + x];
        if (c == 0xFF7A7A7Au || c == 0xFF5A5A5Au) { somma += x; ++n; }
    }
    return n > 20 ? (f32)somma / (f32)n : -1.0f;
}
} // namespace

bool salvaScreenshotP1(const std::string& outDir, const std::string& livello) {
    Game game(12345u);
    caricaSponsor(game);
    game.init(livello);
    std::vector<u32> curva(Framebuffer::WIDTH * Framebuffer::HEIGHT), sponsor(curva.size());
    f32 curvaMax = 0.0f;
    i32 sponsorMax = 0, framePers = -1, frameCurva = -1, frameSponsor = -1;
    for (i32 frame = 1; frame <= 3000; ++frame) {
        Pad pad;
        // Gas fino al frame 600, poi a tratti: le pause fanno partire gli show di inattivita'.
        pad.held = (frame > 600 && (frame / 40) % 2) ? 0ull : (u64)Pad::A;
        game.setInput(pad);
        game.update();
        game.render();
        const u32* d = game.fb().data();
        const i32 nPx = Framebuffer::WIDTH * Framebuffer::HEIGHT;

        const f32 cAlto = centroStrada(d, 125), cBasso = centroStrada(d, 195);
        if (cAlto >= 0.0f && cBasso >= 0.0f && std::abs(cAlto - cBasso) > curvaMax) {
            curvaMax = std::abs(cAlto - cBasso);
            std::copy(d, d + nPx, curva.begin());
            frameCurva = frame;
        }
        i32 verdi = 0, blu = 0, esterni = 0;
        for (i32 i = 0; i < nPx; ++i) {
            if (d[i] == 0xFF00FF00u) ++verdi;
            else if (d[i] == 0xFF0000FFu) ++blu;
            else if (!coloreMotore(d[i])) ++esterni; // colori propri dei cartelloni sponsor
        }
        if (esterni > sponsorMax) {
            sponsorMax = esterni;
            std::copy(d, d + nPx, sponsor.begin());
            frameSponsor = frame;
        }
        if (framePers < 0 && verdi >= 400 && blu >= 200) {
            framePers = frame;
            salvaPPM(outDir + "/p1_personaggio_fumetto.ppm", d, Framebuffer::WIDTH, Framebuffer::HEIGHT);
        }
    }
    if (frameCurva > 0) salvaPPM(outDir + "/p1_curva.ppm", curva.data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
    if (frameSponsor > 0) salvaPPM(outDir + "/p1_cartelloni_sponsor.ppm", sponsor.data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
    std::printf("screenshot P1: curva frame %d (scarto %.0f px), sponsor frame %d (%d px), personaggio+fumetto frame %d\n",
                frameCurva, curvaMax, frameSponsor, sponsorMax, framePers);
    return frameCurva > 0 && frameSponsor > 0 && framePers > 0;
}

// --- Regole di guidabilita' per pista, lette da dati/tracciati/regole.json ---
// Soglie per livello (con default condiviso). Non sono hardcodate qui: provengono
// dal JSON, cosi' Andrea puo' ritoccarle editando solo il file dati.
struct RegolePercorso {
    f32 rettilineo_min = 0.25f;
    f32 rettilineo_max = 0.75f;
    i32 rettilineo_piu_lungo_min = 300;
    f32 curva_max = 1.0f;      // max |curva| ammessa nel giro
    f32 curva_dx_min = 0.3f;   // forza minima curve a destra (varieta')
    f32 curva_sx_min = 0.3f;   // forza minima curve a sinistra (varieta')
    f32 fuori_strada_max = 0.01f;
};

static bool leggiNumeroJson(const sgl::JsonObject& obj, const std::string& chiave, f32& out) {
    sgl::JsonValue v;
    if (!sgl::jsonGet(obj, chiave, v) || !sgl::jsonIsNumber(v)) return false;
    out = (f32)sgl::jsonAsNumber(v);
    return true;
}

static bool leggiIntJson(const sgl::JsonObject& obj, const std::string& chiave, i32& out) {
    sgl::JsonValue v;
    if (!sgl::jsonGet(obj, chiave, v) || !sgl::jsonIsNumber(v)) return false;
    out = (i32)sgl::jsonAsNumber(v);
    return true;
}

// Legge regole.json e restituisce le regole per `livello`, oppure le "default"
// se il livello non ha una voce propria (o il file manca / e' illeggibile).
RegolePercorso leggiRegolePercorso(const std::string& livello) {
    RegolePercorso r; // valori default già sopra
    std::ifstream f("motore_nuovo/dati/tracciati/regole.json");
    if (!f.is_open()) return r;
    std::stringstream ss;
    ss << f.rdbuf();
    f.close();
    sgl::JsonReader reader(ss.str());
    if (!reader.parse()) return r;
    const auto& val = reader.valore();
    if (!sgl::jsonIsObject(val)) return r;
    const auto& root = sgl::jsonAsObject(val);
    sgl::JsonValue regoleVal;
    if (!sgl::jsonGet(root, "regole", regoleVal) || !sgl::jsonIsObject(regoleVal)) return r;
    const auto& regole = sgl::jsonAsObject(regoleVal);
    const sgl::JsonObject* sorgente = nullptr;
    auto it = regole.find(livello);
    if (it != regole.end() && sgl::jsonIsObject(it->second)) {
        sorgente = &sgl::jsonAsObject(it->second);
    } else {
        sgl::JsonValue defVal;
        if (sgl::jsonGet(root, "default", defVal) && sgl::jsonIsObject(defVal))
            sorgente = &sgl::jsonAsObject(defVal);
    }
    if (!sorgente) return r;
    leggiNumeroJson(*sorgente, "rettilineo_min", r.rettilineo_min);
    leggiNumeroJson(*sorgente, "rettilineo_max", r.rettilineo_max);
    leggiIntJson(*sorgente, "rettilineo_piu_lungo_min", r.rettilineo_piu_lungo_min);
    leggiNumeroJson(*sorgente, "curva_max", r.curva_max);
    leggiNumeroJson(*sorgente, "curva_dx_min", r.curva_dx_min);
    leggiNumeroJson(*sorgente, "curva_sx_min", r.curva_sx_min);
    leggiNumeroJson(*sorgente, "fuori_strada_max", r.fuori_strada_max);
    return r;
}

// Legge i nomi dei tratti del campo "tracciato" di livello.json, in ordine.
static std::vector<std::string> leggiNomiTratti(const std::string& livello) {
    std::vector<std::string> out;
    std::string lvPath = "motore_nuovo/dati/livelli/" + livello + "/livello.json";
    std::ifstream f(lvPath);
    if (!f.is_open()) return out;
    std::stringstream ss;
    ss << f.rdbuf();
    f.close();
    sgl::JsonReader reader(ss.str());
    if (!reader.parse()) return out;
    const auto& val = reader.valore();
    if (!sgl::jsonIsObject(val)) return out;
    const auto& root = sgl::jsonAsObject(val);
    sgl::JsonValue trav;
    if (!sgl::jsonGet(root, "tracciato", trav) || !sgl::jsonIsArray(trav)) return out;
    const auto& arr = sgl::jsonAsArray(trav);
    for (const auto& t : arr) {
        if (sgl::jsonIsString(t)) out.push_back(sgl::jsonAsString(t));
    }
    return out;
}

// Lunghezze in segmenti di ogni tratto da tratti.json (chiave = nome tratto).
static std::map<std::string, i32> leggiLunghezzeTratti() {
    std::map<std::string, i32> out;
    std::ifstream f("motore_nuovo/dati/tracciati/tratti.json");
    if (!f.is_open()) return out;
    std::stringstream ss;
    ss << f.rdbuf();
    f.close();
    sgl::JsonReader reader(ss.str());
    if (!reader.parse()) return out;
    const auto& val = reader.valore();
    if (!sgl::jsonIsObject(val)) return out;
    const auto& root = sgl::jsonAsObject(val);
    sgl::JsonValue trattiVal;
    if (!sgl::jsonGet(root, "tratti", trattiVal) || !sgl::jsonIsObject(trattiVal)) return out;
    const auto& tratti = sgl::jsonAsObject(trattiVal);
    for (const auto& kv : tratti) {
        const auto& obj = sgl::jsonAsObject(kv.second);
        sgl::JsonValue lenVal;
        i32 len = 0;
        if (sgl::jsonGet(obj, "lunghezza", lenVal) && sgl::jsonIsNumber(lenVal))
            len = (i32)sgl::jsonAsNumber(lenVal);
        if (len <= 0) len = 1;
        out[kv.first] = len;
    }
    return out;
}

// Dato un indice di segmento del tracciato costruito ripetendo il pattern
// "tracciato" di livello.json, restituisce il nome del tratto che lo contiene
// e l'indice del primo segmento di quell'occorrenza del tratto.
// Il loader ripete il pattern per intero (n e' multiplo di sommaLunghezze:
// verificaEspansioneTracciato), quindi segmento i -> pattern position i % somma.
static bool trattoDiSegmento(i32 i, const std::vector<std::string>& nomi,
                             const std::map<std::string, i32>& lunghezze,
                             std::string& nomeOut, i32& inizioOut) {
    nomeOut.clear();
    inizioOut = -1;
    if (nomi.empty()) return false;
    std::vector<i32> lenPat;
    lenPat.reserve(nomi.size());
    i32 somma = 0;
    for (const auto& nome : nomi) {
        auto it = lunghezze.find(nome);
        i32 L = (it != lunghezze.end()) ? it->second : 1;
        lenPat.push_back(L);
        somma += L;
    }
    if (somma <= 0) return false;
    const i32 p = i % somma;   // posizione dentro il pattern
    const i32 rep = i / somma; // ripetizione del pattern
    i32 acc = 0;
    for (size_t t = 0; t < nomi.size(); ++t) {
        if (p < acc + lenPat[t]) {
            nomeOut = nomi[t];
            inizioOut = rep * somma + acc;
            return true;
        }
        acc += lenPat[t];
    }
    return false;
}

// --- Percorso: il tracciato del livello e' vario e si guida da capo a fondo ---
// Profilo: rettilinei fra rettilineo_min e rettilineo_max del giro, curve forti
// (|curva| >= curva_dx_min / curva_sx_min) a destra e a sinistra, almeno un
// rettilineo lungo rettilineo_piu_lungo_min segmenti. Guida: gas pieno e uno
// sterzo semplice che riporta l'auto verso il centro; il giro deve finire senza
// uscire di strada per piu' di fuori_strada_max dei frame.
// Soglie lette da dati/tracciati/regole.json (non hardcodate).
bool verificaPercorso(const std::string& livello) {
    Game game(12345u);
    game.init(livello);
    const auto& segs = game.road().segments();
    const i32 n = (i32)segs.size();
    i32 dritti = 0, rettilineoMax = 0, corsa = 0;
    i32 corsaStart = 0, idxRettMax = -1;
    f32 destraMax = 0.0f, sinistraMax = 0.0f;
    i32 idxDestra = -1, idxSinistra = -1;
    i32 idx = 0;
    for (const RoadSegment& seg : segs) {
        const f32 c = seg.curve;
        if (std::abs(c) < 0.05f) {
            if (corsa == 0) corsaStart = idx;
            ++corsa;
            ++dritti;
            if (corsa > rettilineoMax) { rettilineoMax = corsa; idxRettMax = corsaStart; }
        } else {
            corsa = 0;
        }
        if (c > destraMax) { destraMax = c; idxDestra = idx; }
        if (-c > sinistraMax) { sinistraMax = -c; idxSinistra = idx; }
        ++idx;
    }
    const f32 quotaDritti = n > 0 ? (f32)dritti / (f32)n : 0.0f;
    const RegolePercorso r = leggiRegolePercorso(livello);
    const std::vector<std::string> nomiTratti = leggiNomiTratti(livello);
    const std::map<std::string, i32> lunghezzeTratti = leggiLunghezzeTratti();
    auto trattoSuffisso = [&](i32 idxSeg) -> std::string {
        std::string nome;
        i32 inizio = -1;
        if (idxSeg < 0 || !trattoDiSegmento(idxSeg, nomiTratti, lunghezzeTratti, nome, inizio))
            return "";
        return " [tratto " + nome + " da segmento " + std::to_string(inizio) + "]";
    };

    const f32 giro = (f32)game.road().totalLength();
    f32 percorso = 0.0f, zPrima = 0.0f;
    i32 frame = 0, fuori = 0;
    const i32 FRAME_MAX = 20000; // 400 s: un giro normale ne richiede meno di 60
    while (percorso < giro && frame < FRAME_MAX) {
        Pad pad;
        pad.held = (u64)Pad::A;
        const f32 x = game.state().playerX;
        if (x > 120.0f) pad.held |= (u64)Pad::Left;
        else if (x < -120.0f) pad.held |= (u64)Pad::Right;
        game.setInput(pad);
        game.update();
        const f32 z = game.state().playerZ;
        f32 dz = z - zPrima;
        if (dz < 0.0f) dz += giro; // avvolgimento a fine giro
        percorso += dz;
        zPrima = z;
        if (game.state().playerOffroad) ++fuori;
        ++frame;
    }
    const bool giroFinito = percorso >= giro;
    const f32 quotaFuori = frame > 0 ? (f32)fuori / (f32)frame : 1.0f;
    std::printf("percorso %s: segmenti=%d rettilineo=%.0f%% (limiti %.0f%%-%.0f%%) (piu' lungo %d, min %d) curva max destra=%.2f (min %.2f) sinistra=%.2f (min %.2f, max %.2f)\n",
                livello.c_str(), n, quotaDritti * 100.0f, r.rettilineo_min * 100.0f, r.rettilineo_max * 100.0f,
                rettilineoMax, r.rettilineo_piu_lungo_min, destraMax, r.curva_dx_min, sinistraMax, r.curva_sx_min, r.curva_max);
    std::printf("giro guidato: %s in %d frame (%.1f s @ %d Hz), fuori strada %d frame (%.1f%%, max %.0f%%)\n",
                giroFinito ? "finito" : "NON finito", frame, frame / (f32)::leggiFrequenzaDaConfig(), ::leggiFrequenzaDaConfig(),
                fuori, quotaFuori * 100.0f, r.fuori_strada_max * 100.0f);

    bool ok = true;
    if (quotaDritti < r.rettilineo_min || quotaDritti > r.rettilineo_max) {
        std::printf("  VIOLAZIONE: %% rettilineo %.0f fuori da [%.0f, %.0f]\n",
                    quotaDritti * 100.0f, r.rettilineo_min * 100.0f, r.rettilineo_max * 100.0f);
        ok = false;
    }
    if (rettilineoMax < r.rettilineo_piu_lungo_min) {
        std::printf("  VIOLAZIONE: rettilineo piu' lungo %d < min %d%s\n",
                    rettilineoMax, r.rettilineo_piu_lungo_min, trattoSuffisso(idxRettMax).c_str());
        ok = false;
    }
    if (destraMax < r.curva_dx_min) {
        std::printf("  VIOLAZIONE: curva max destra %.2f < min %.2f%s\n",
                    destraMax, r.curva_dx_min, trattoSuffisso(idxDestra).c_str());
        ok = false;
    }
    if (sinistraMax < r.curva_sx_min) {
        std::printf("  VIOLAZIONE: curva max sinistra %.2f < min %.2f%s\n",
                    sinistraMax, r.curva_sx_min, trattoSuffisso(idxSinistra).c_str());
        ok = false;
    }
    if (destraMax > r.curva_max || sinistraMax > r.curva_max) {
        std::printf("  VIOLAZIONE: curva massima %.2f > limite %.2f\n",
                    std::max(destraMax, sinistraMax), r.curva_max);
        ok = false;
    }
    if (!giroFinito) {
        std::printf("  VIOLAZIONE: giro non completato\n");
        ok = false;
    }
    if (quotaFuori > r.fuori_strada_max) {
        std::printf("  VIOLAZIONE: fuori strada %.1f%% > max %.0f%%\n",
                    quotaFuori * 100.0f, r.fuori_strada_max * 100.0f);
        ok = false;
    }
    return ok;
}

// --- Soglie cambio tappa (FORK1 punto 4), lette da dati/tracciati/regole.json ("bivio") ---
struct BivioRegole {
    i32 finestra = 60;          // frame registrati prima e dopo il cambio tappa
    f32 delta_larghezza_max = 400.0f;   // carreggiata (fattore*ROAD_WIDTH) per frame
    f32 delta_centro_max = 400.0f;      // centro corsia del ramo per frame
    f32 delta_x_rel_max = 40.0f;        // playerX rispetto al centro corsia, per frame
    f32 delta_curva_max = 0.05f;        // curva del segmento sotto l'auto per frame
    f32 delta_collina_max = 0.05f;      // collina del segmento sotto l'auto per frame
};

static BivioRegole leggiRegoleBivio() {
    BivioRegole r;
    std::ifstream f("motore_nuovo/dati/tracciati/regole.json");
    if (!f.is_open()) return r;
    std::stringstream ss;
    ss << f.rdbuf();
    f.close();
    sgl::JsonReader reader(ss.str());
    if (!reader.parse()) return r;
    const auto& val = reader.valore();
    if (!sgl::jsonIsObject(val)) return r;
    const auto& root = sgl::jsonAsObject(val);
    sgl::JsonValue bivioVal;
    if (!sgl::jsonGet(root, "bivio", bivioVal) || !sgl::jsonIsObject(bivioVal)) return r;
    const auto& b = sgl::jsonAsObject(bivioVal);
    leggiIntJson(b, "finestra_frame", r.finestra);
    leggiNumeroJson(b, "delta_larghezza_max", r.delta_larghezza_max);
    leggiNumeroJson(b, "delta_centro_max", r.delta_centro_max);
    leggiNumeroJson(b, "delta_x_rel_max", r.delta_x_rel_max);
    leggiNumeroJson(b, "delta_curva_max", r.delta_curva_max);
    leggiNumeroJson(b, "delta_collina_max", r.delta_collina_max);
    return r;
}

struct CampioneBivio {
    i32 frame = 0;
    f32 larghezza = 0.0f; // fattore*ROAD_WIDTH della carreggiata in questo frame
    f32 centro = 0.0f;    // centro corsia del ramo del giocatore (0 in scelta aperta)
    f32 x = 0.0f;         // player.x assoluto
    f32 xRel = 0.0f;      // player.x rispetto al centro della corsia
    f32 curva = 0.0f;     // curva del segmento sotto l'auto
    f32 collina = 0.0f;   // collina del segmento sotto l'auto
};

static f32 collinaAt(const Road& road, f32 z) {
    if (road.segments().empty() || road.totalLength() <= 0) return 0.0f;
    z = std::fmod(z, (f32)road.totalLength());
    if (z < 0.0f) z += (f32)road.totalLength();
    f32 acc = 0.0f;
    for (const RoadSegment& seg : road.segments()) {
        if (acc + seg.length > z) return seg.hill;
        acc += (f32)seg.length;
    }
    return 0.0f;
}

static CampioneBivio campionaBivio(const Game& game) {
    const GameState& s = game.state();
    const Road& road = game.road();
    const f32 z = road.playerZ;
    const Road::Carreggiate c = road.carreggiate(z, s.latoBivio);
    f32 centro = 0.0f;
    if (s.latoBivio != 0 && c.doppia) {
        const i32 vicino =
            std::abs(s.playerX - c.centro[0]) <= std::abs(s.playerX - c.centro[1]) ? 0 : 1;
        centro = c.centro[vicino];
    }
    CampioneBivio cp;
    cp.frame = (i32)s.frame;
    cp.larghezza = c.fattore * Road::ROAD_WIDTH;
    cp.centro = centro;
    cp.x = s.playerX;
    cp.xRel = s.playerX - centro;
    cp.curva = road.curvaAt(z);
    cp.collina = collinaAt(road, z);
    return cp;
}

// Guida automatica verso il ramo scelto al bivio (come verificaGara), gas pieno.
static Pad guidaBivio(const Game& game, const char* scelte) {
    Pad pad{}; pad.held = Pad::A;
    const GameState& s = game.state();
    const Road& road = game.road();
    f32 obiettivo = 0.0f;
    if (road.bivioAttivo() && s.latoBivio == 0 && s.playerZ > road.bivioInizioZ()) {
        const i32 lato = (scelte[0] == 'S') ? -1 : 1;
        obiettivo = (f32)lato * Road::BIVIO_DIST;
    }
    for (const SpriteInstance& t : game.traffico()) {
        const f32 dz = t.worldZ - s.playerZ;
        if (dz > 0.0f && dz < 5000.0f && std::abs(t.worldX - s.playerX) < 380.0f)
            obiettivo = t.worldX + (t.worldX > obiettivo ? -650.0f : 650.0f);
    }
    if (s.playerX > obiettivo + 120.0f) pad.held |= (u64)Pad::Left;
    else if (s.playerX < obiettivo - 120.0f) pad.held |= (u64)Pad::Right;
    return pad;
}

// --- Gate FORK1 punto 4: cambio tappa / uscita dal bivio ---
// Per ciascuna delle 4 combinazioni di bivi guida la gara fino al primo cambio
// tappa (fineTappa carica la tappa successiva) e registra la finestra +-finestra
// attorno al cambio: larghezza strada, centro corsia, x dell'auto (relativa alla
// corsia), curva e collina. Fallisce se uno scarto tra due frame consecutivi
// supera le soglie di regole.json. Salva screenshot PPM (5 frame prima, cambio,
// 5 frame dopo).
bool verificaBivioUscita(const std::string& outDir) {
    const char* scelte[] = { "SS", "SD", "DS", "DD" };
    const BivioRegole r = leggiRegoleBivio();
    const i32 finestra = r.finestra > 0 ? r.finestra : 60;
    bool ok = true;
    for (const char* s : scelte) {
        // Passo 1: trova il blocco della scelta al bivio (teleport sul ramo) e
        // il primo cambio tappa (fineTappa carica la tappa successiva).
        i32 bloccoFrame = -1, cambioFrame = -1, tappaIniziale = 1, tappaDopo = 1;
        i32 latoPrecedente = 0;
        {
            Game game(12345u);
            caricaSponsor(game);
            if (!game.avviaGara()) { ok = false; continue; }
            tappaIniziale = game.state().tappa;
            for (i32 frame = 0; frame < 400000 && !game.state().arrivo; ++frame) {
                game.setInput(guidaBivio(game, s));
                game.update();
                game.render();
                if (bloccoFrame < 0 && game.state().latoBivio != 0 && latoPrecedente == 0) bloccoFrame = frame;
                latoPrecedente = game.state().latoBivio;
                if (game.state().tappa != tappaIniziale) {
                    cambioFrame = frame;
                    tappaDopo = game.state().tappa;
                    break;
                }
            }
        }
        if (cambioFrame < 0) {
            std::printf("bivio_uscita %s: FALLITO (cambio tappa non raggiunto)\n", s);
            ok = false;
            continue;
        }
        // Passo 2: rigioca deterministicamente e registra [blocco-finestra, cambio+finestra].
        const i32 inizio = bloccoFrame > finestra ? bloccoFrame - finestra : 0;
        const i32 fine = cambioFrame + finestra;
        std::vector<CampioneBivio> campioni;
        std::vector<std::vector<u32>> fotogrammi;
        {
            Game game(12345u);
            caricaSponsor(game);
            game.avviaGara();
            for (i32 frame = 0; frame <= fine && !game.state().arrivo; ++frame) {
                game.setInput(guidaBivio(game, s));
                game.update();
                game.render();
                if (frame >= inizio) {
                    campioni.push_back(campionaBivio(game));
                    const u32* d = game.fb().data();
                    fotogrammi.push_back(std::vector<u32>(d, d + (size_t)Framebuffer::WIDTH * Framebuffer::HEIGHT));
                }
            }
        }
        const i32 idxCambio = cambioFrame - inizio;
        // Scarti massimi tra frame consecutivi nella finestra.
        f32 maxLarghezza = 0.0f, maxCentro = 0.0f, maxXRel = 0.0f, maxCurva = 0.0f, maxCollina = 0.0f;
        for (size_t i = 1; i < campioni.size(); ++i) {
            const f32 dl = std::abs(campioni[i].larghezza - campioni[i - 1].larghezza);
            const f32 dc = std::abs(campioni[i].centro - campioni[i - 1].centro);
            const f32 dx = std::abs(campioni[i].xRel - campioni[i - 1].xRel);
            const f32 dcv = std::abs(campioni[i].curva - campioni[i - 1].curva);
            const f32 dh = std::abs(campioni[i].collina - campioni[i - 1].collina);
            if (dl > maxLarghezza) maxLarghezza = dl;
            if (dc > maxCentro) maxCentro = dc;
            if (dx > maxXRel) maxXRel = dx;
            if (dcv > maxCurva) maxCurva = dcv;
            if (dh > maxCollina) maxCollina = dh;
        }
        // Screenshot: blocco della scelta + cambio tappa, con 5 frame prima/dopo il cambio.
        const std::string base = outDir + "/p_bivio_uscita_" + std::string(s);
        auto salvaFrame = [&](i32 frameGlobale, const std::string& sufix) {
            const i32 idx = frameGlobale - inizio;
            if (idx >= 0 && idx < (i32)fotogrammi.size())
                salvaPPM(base + sufix, fotogrammi[(size_t)idx].data(),
                         Framebuffer::WIDTH, Framebuffer::HEIGHT);
        };
        if (bloccoFrame >= 0) salvaFrame(bloccoFrame, "_blocco.ppm");
        salvaFrame(cambioFrame, "_cambio.ppm");
        for (i32 k = 1; k <= 5; ++k) {
            salvaFrame(cambioFrame - k, "_prima" + std::to_string(k) + ".ppm");
            salvaFrame(cambioFrame + k, "_dopo" + std::to_string(k) + ".ppm");
        }
        bool comboOk = true;
        if (maxLarghezza > r.delta_larghezza_max) { std::printf("  VIOLAZIONE larghezza max=%.1f > soglia %.1f\n", maxLarghezza, r.delta_larghezza_max); comboOk = false; }
        if (maxCentro > r.delta_centro_max) { std::printf("  VIOLAZIONE centro max=%.1f > soglia %.1f\n", maxCentro, r.delta_centro_max); comboOk = false; }
        if (maxXRel > r.delta_x_rel_max) { std::printf("  VIOLAZIONE x_rel max=%.1f > soglia %.1f (blocco scelta al frame %d)\n", maxXRel, r.delta_x_rel_max, bloccoFrame); comboOk = false; }
        if (maxCurva > r.delta_curva_max) { std::printf("  VIOLAZIONE curva max=%.2f > soglia %.2f\n", maxCurva, r.delta_curva_max); comboOk = false; }
        if (maxCollina > r.delta_collina_max) { std::printf("  VIOLAZIONE collina max=%.2f > soglia %.2f\n", maxCollina, r.delta_collina_max); comboOk = false; }
        std::printf("bivio_uscita %s: blocco scelta frame %d, cambio tappa %d -> %d al frame %d | maxDelta larghezza=%.1f centro=%.1f x_rel=%.1f curva=%.2f collina=%.2f | %s\n",
                    s, bloccoFrame, tappaIniziale, tappaDopo, cambioFrame,
                    maxLarghezza, maxCentro, maxXRel, maxCurva, maxCollina,
                    comboOk ? "OK" : "FALLITO");
        ok = ok && comboOk;
    }
    std::printf("%s\n", ok ? "GATE BIVIO USCITA OK" : "FALLITO: cambio tappa/bivio non fluido");
    return ok;
}

// --- Gara: partenza, bivi e arrivo seguendo dati/percorso_gara.json ---
// Guida automatica: gas pieno, schiva gli avversari davanti, al bivio va sul
// ramo voluto. Per ogni combinazione di scelte controlla le tappe attraversate,
// l'arrivo e il determinismo (due corse identiche danno lo stesso risultato).
struct EsitoGara {
    std::vector<std::string> tappe;
    bool arrivo = false;
    i32 posizione = 0;
    u32 frame = 0;
    u32 incidenti = 0;
};

EsitoGara corriGara(const char* scelte) {
    EsitoGara e;
    Game game(12345u);
    caricaSponsor(game);
    if (!game.avviaGara()) return e;
    e.tappe.push_back(game.tappaCorrente());
    bool ribaltato = false;
    for (i32 frame = 0; frame < 15000 && !game.state().arrivo; ++frame) {
        const GameState& s = game.state();
        const Road& road = game.road();
        f32 obiettivo = 0.0f;
        const i32 indice = s.tappa - 1;
        const i32 lato = indice < (i32)std::strlen(scelte) && scelte[indice] == 'S' ? -1 : 1;
        if (road.bivioAttivo() && s.latoBivio == 0 && s.playerZ > road.bivioInizioZ())
            obiettivo = (f32)lato * Road::BIVIO_DIST;
        for (const SpriteInstance& t : game.traffico()) {
            const f32 dz = t.worldZ - s.playerZ;
            if (dz > 0.0f && dz < 5000.0f && std::abs(t.worldX - s.playerX) < 380.0f)
                obiettivo = t.worldX + (t.worldX > obiettivo ? -650.0f : 650.0f);
        }
        Pad pad;
        pad.held = (u64)Pad::A;
        if (s.playerX > obiettivo + 120.0f) pad.held |= (u64)Pad::Left;
        else if (s.playerX < obiettivo - 120.0f) pad.held |= (u64)Pad::Right;
        game.setInput(pad);
        game.update();
        if (game.state().playerFlipping && !ribaltato) ++e.incidenti;
        ribaltato = game.state().playerFlipping;
        if (game.tappaCorrente() != e.tappe.back()) e.tappe.push_back(game.tappaCorrente());
    }
    e.arrivo = game.state().arrivo;
    e.posizione = game.state().posizione;
    e.frame = game.state().frameGara;
    return e;
}

bool verificaGara() {
    // S = sinistra, D = destra a ogni bivio; atteso: la tappa di arrivo.
    struct Caso { const char* scelte; const char* atteso[3]; };
    const Caso casi[] = {
        {"SS", {"lv_01_neon", "lv_02_atomico", "lv_04_cromo"}},
        {"SD", {"lv_01_neon", "lv_02_atomico", "lv_05_spray"}},
        {"DS", {"lv_01_neon", "lv_03_foglie", "lv_05_spray"}},
        {"DD", {"lv_01_neon", "lv_03_foglie", "lv_06_desktop"}},
    };
    bool ok = true;
    for (const Caso& c : casi) {
        const EsitoGara a = corriGara(c.scelte);
        const EsitoGara b = corriGara(c.scelte);
        bool tappeOk = a.tappe.size() == 3;
        for (size_t i = 0; tappeOk && i < 3; ++i) tappeOk = a.tappe[i] == c.atteso[i];
        const bool uguali = a.frame == b.frame && a.posizione == b.posizione && a.tappe == b.tappe;
        std::printf("gara %s: %s -> %s -> %s, arrivo %s in %.1f s, posizione %d/8, incidenti %u, replay %s\n",
                    c.scelte, a.tappe.size() > 0 ? a.tappe[0].c_str() : "-", a.tappe.size() > 1 ? a.tappe[1].c_str() : "-",
                    a.tappe.size() > 2 ? a.tappe[2].c_str() : "-", a.arrivo ? "si" : "NO", a.frame / 50.0f,
                    a.posizione, a.incidenti, uguali ? "IDENTICO" : "DIVERSO");
        // Una tappa dura circa un minuto: la gara intera resta sotto i 5 minuti.
        ok = ok && tappeOk && a.arrivo && uguali && a.frame < 15000u && a.posizione >= 1 && a.posizione <= 8;
    }
    return ok;
}

// --- Test espansione tracciato: carica ogni livello, legge il campo "tracciato"
// --- di livello.json, somma le lunghezze dei tratti da tratti.json e verifica
// --- che l'espansione (road().segments()) sia un multiplo esatto di quella somma
// --- (pattern ripetuto per intero, nessun segmento perso).
bool verificaEspansioneTracciato() {
    const char* livelli[] = {"lv_01_neon", "lv_02_atomico", "lv_03_foglie",
                             "lv_04_cromo", "lv_05_spray", "lv_06_desktop"};
    // Leggi tratti.json: mappa nome tratto -> lunghezza (segmenti).
    std::ifstream f("motore_nuovo/dati/tracciati/tratti.json");
    if (!f.is_open()) { std::printf("FALLITO: tratti.json non trovato\n"); return false; }
    std::stringstream ss;
    ss << f.rdbuf();
    f.close();
    sgl::JsonReader reader(ss.str());
    if (!reader.parse()) { std::printf("FALLITO: tratti.json illeggibile\n"); return false; }
    const auto& val = reader.valore();
    if (!sgl::jsonIsObject(val)) { std::printf("FALLITO: tratti.json non e' un oggetto\n"); return false; }
    const auto& root = sgl::jsonAsObject(val);
    sgl::JsonValue trattiVal;
    if (!sgl::jsonGet(root, "tratti", trattiVal) || !sgl::jsonIsObject(trattiVal)) {
        std::printf("FALLITO: campo 'tratti' mancante in tratti.json\n"); return false;
    }
    const auto& tratti = sgl::jsonAsObject(trattiVal);
    std::map<std::string, i32> lunghezzaTratto;
    for (const auto& kv : tratti) {
        const auto& obj = sgl::jsonAsObject(kv.second);
        sgl::JsonValue lenVal;
        i32 len = 0;
        if (sgl::jsonGet(obj, "lunghezza", lenVal) && sgl::jsonIsNumber(lenVal))
            len = (i32)sgl::jsonAsNumber(lenVal);
        lunghezzaTratto[kv.first] = len;
    }

    bool ok = true;
    for (const char* lv : livelli) {
        Game game(12345u);
        game.init(lv);
        const auto& segs = game.road().segments();
        i32 n = (i32)segs.size();

        std::string lvPath = "motore_nuovo/dati/livelli/" + std::string(lv) + "/livello.json";
        std::ifstream lf(lvPath);
        if (!lf.is_open()) { std::printf("%s: FALLITO livello.json non trovato\n", lv); ok = false; continue; }
        std::stringstream lss;
        lss << lf.rdbuf();
        lf.close();
        sgl::JsonReader lreader(lss.str());
        if (!lreader.parse()) { std::printf("%s: FALLITO livello.json illeggibile\n", lv); ok = false; continue; }
        const auto& lval = lreader.valore();
        if (!sgl::jsonIsObject(lval)) { std::printf("%s: FALLITO livello.json non oggetto\n", lv); ok = false; continue; }
        const auto& lroot = sgl::jsonAsObject(lval);
        sgl::JsonValue trav;
        bool haTracciato = sgl::jsonGet(lroot, "tracciato", trav) && sgl::jsonIsArray(trav);
        if (!haTracciato) {
            std::printf("%s: FALLITO campo 'tracciato' mancante\n", lv);
            ok = false;
            continue;
        }
        const auto& arr = sgl::jsonAsArray(trav);
        i32 sommaLunghezze = 0;
        bool trattiOk = true;
        for (const auto& t : arr) {
            if (!sgl::jsonIsString(t)) { trattiOk = false; break; }
            const std::string& nome = sgl::jsonAsString(t);
            auto it = lunghezzaTratto.find(nome);
            if (it == lunghezzaTratto.end()) { trattiOk = false; break; }
            sommaLunghezze += it->second;
        }
        if (!trattiOk) {
            std::printf("%s: FALLITO tracciato contiene tratto sconosciuto\n", lv);
            ok = false;
            continue;
        }
        // Il loader ripete il pattern per intero finché segTotali >= minSeg:
        // segmenti deve essere >= somma e multiplo esatto di somma.
        bool espansioneOk = (sommaLunghezze > 0 && n >= sommaLunghezze && (n % sommaLunghezze) == 0);
        std::printf("%s: tracciato=%zu tratti somma=%d segmenti=%d espansione=%s\n",
                    lv, arr.size(), sommaLunghezze, n, espansioneOk ? "OK" : "FALLITO");
        if (!espansioneOk) ok = false;
    }
    std::printf("%s\n", ok ? "GATE ESPANSIONE OK" : "FALLITO: espansione tracciato non coerente");
    return ok;
}

// --- Frequenza: verifica determinismo e tempi di giro a 50/60 Hz ---
// Esegue un giro guidato alla frequenza indicata, due run identici,
// verifica replay IDENTICO, ms/frame <= 20 e riporta i secondi di giro.
// La frequenza viene impostata temporaneamente in config.json e ripristinata.
bool verificaFrequenza(const std::string& outDir, const std::string& livello, i32 frequenza) {
    i32 frequenzaPrecedente = ::impostaFrequenzaDaConfig(frequenza);
    f32 dt = 1.0f / (f32)frequenza;
    bool ok = true;
    f32 secondiA = 0.0f, secondiB = 0.0f;
    double msMaxA = 0.0, msMaxB = 0.0;
    std::vector<u32> hashA, hashB;

    for (int run = 0; run < 2 && ok; ++run) {
        Game game(12345u);
        game.init(livello);
        f32 giro = (f32)game.road().totalLength();
        f32 percorso = 0.0f, zPrima = 0.0f;
        i32 frame = 0;
        double msTot = 0.0, msMax = 0.0;
        const i32 FRAME_MAX = 60000;
        std::vector<u32>& hash = (run == 0) ? hashA : hashB;

        while (percorso < giro && frame < FRAME_MAX) {
            Pad pad;
            pad.held = (u64)Pad::A;
            const f32 x = game.state().playerX;
            if (x > 120.0f) pad.held |= (u64)Pad::Left;
            else if (x < -120.0f) pad.held |= (u64)Pad::Right;
            game.setInput(pad);
            auto t0 = std::chrono::steady_clock::now();
            game.update();
            game.render();
            double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
            msTot += ms;
            if (ms > msMax) msMax = ms;

            const f32 z = game.state().playerZ;
            f32 dz = z - zPrima;
            if (dz < 0.0f) dz += giro;
            percorso += dz;
            zPrima = z;
            hash.push_back(game.frameHash());
            ++frame;
        }

        f32 secondi = frame / (f32)frequenza;
        if (run == 0) { secondiA = secondi; msMaxA = msMax; }
        else { secondiB = secondi; msMaxB = msMax; }

        if (frame >= FRAME_MAX) {
            std::printf("%d Hz: run %d - giro NON completato in %d frame\n", frequenza, run + 1, frame);
            ok = false;
        }
    }

    bool deterministico = (hashA == hashB);
    double diffSecondi = (secondiA > 0.0f && secondiB > 0.0f)
        ? std::abs(secondiA - secondiB) / std::max(secondiA, secondiB) * 100.0
        : 0.0;

    // Ripristina configurazione originale prima di restituire.
    ::impostaFrequenzaDaConfig(frequenzaPrecedente);

    std::printf("frequenza %d Hz: runA=%.2f s (ms/frame max=%.2f) runB=%.2f s (ms/frame max=%.2f) "
                "replay %s diff=%.2f%%\n",
                frequenza, secondiA, msMaxA, secondiB, msMaxB,
                deterministico ? "IDENTICO" : "DIVERSO", diffSecondi);

    if (!ok) { std::printf("FALLITO: giro non completato\n"); return false; }
    if (!deterministico) { std::printf("FALLITO: non deterministico\n"); return false; }
    if (msMaxA > MAX_MS_FRAME || msMaxB > MAX_MS_FRAME) {
        std::printf("FALLITO: troppo lento\n");
        return false;
    }
    std::printf("GATE FREQUENZA %d Hz OK\n", frequenza);
    return true;
}

int main(int argc, char** argv) {
    const std::string outDir = argc > 1 ? argv[1] : "out/debugger";
    const std::string livello = argc > 2 ? argv[2] : "lv_01_neon";
    const std::string modo = argc > 3 ? argv[3] : "smoke";
    // I dati si leggono con percorsi relativi alla radice progetto (sglavinated_src):
    // da un'altra cartella il gioco ripiega in silenzio sul tracciato di emergenza.
    const std::string lvPath = "motore_nuovo/dati/livelli/" + livello + "/livello.json";
    if (!std::filesystem::exists(lvPath)) {
        std::printf("FALLITO: %s non trovato. Lanciare dalla radice progetto (sglavinated_src).\n", lvPath.c_str());
        return 4;
    }
    std::filesystem::create_directories(outDir);

    if (modo == "gara") {
        const bool ok = verificaGara();
        std::printf("%s\n", ok ? "GATE GARA OK" : "FALLITO: gara a tappe non completa");
        return ok ? 0 : 14;
    }

    if (modo == "percorso") {
        const bool ok = verificaPercorso(livello);
        std::printf("%s\n", ok ? "GATE PERCORSO OK" : "FALLITO: percorso poco vario o non guidabile");
        return ok ? 0 : 13;
    }

    if (modo == "screenshot") {
        const bool ok = salvaScreenshotP1(outDir, livello);
        std::printf("%s\n", ok ? "GATE SCREENSHOT P1 OK" : "FALLITO: screenshot P1 incompleti");
        return ok ? 0 : 12;
    }

    if (modo == "crash") {
        const bool ok = verificaRibaltamento(livello);
        std::printf("%s\n", ok ? "GATE RIBALTAMENTO OK"
                               : "FALLITO: ribaltamento o ripartenza sul tracciato");
        return ok ? 0 : 5;
    }

    if (modo == "accelerazione") {
        const bool ok = verificaAccelerazione(livello);
        std::printf("%s\n", ok ? "GATE ACCELERAZIONE OK"
                               : "FALLITO: accelerazione non progressiva o troppo lenta");
        return ok ? 0 : 6;
    }

    if (modo == "sterzo") {
        const bool ok = verificaSterzo();
        std::printf("%s\n", ok ? "GATE STERZO OK" : "FALLITO: sterzo poco reattivo o asimmetrico");
        return ok ? 0 : 7;
    }

    if (modo == "curve") {
        const bool ok = verificaContinuitaCurve();
        std::printf("%s\n", ok ? "GATE CURVE CONTINUE OK" : "FALLITO: curve a scatto o proiezione insufficiente");
        return ok ? 0 : 8;
    }

    if (modo == "soak") {
        const i32 TOTAL_FRAMES = SOAK_FRAMES;
        Esito a = esegui(outDir, livello, true, TOTAL_FRAMES);
        Esito b = esegui(outDir, livello, false, TOTAL_FRAMES);

        bool deterministico = a.hash == b.hash;
        i32 primaDivergenza = -1;
        for (size_t i = 0; i < a.hash.size() && i < b.hash.size(); ++i)
            if (a.hash[i] != b.hash[i]) { primaDivergenza = (i32)i; break; }

        for (i32 f = 0; f < TOTAL_FRAMES; f += 60) std::printf("frameHash %d %u\n", f, a.hash[f]);
        double msMed = a.msTot / TOTAL_FRAMES;
        std::printf("modo: soak, frame simulati: %d\n", TOTAL_FRAMES);
        std::printf("speed@60=%.2f speed@1=%.2f  X[min,max]=[%.1f,%.1f]  zFinale=%.1f\n",
                    a.speed60, a.speedFirstFrame, a.minX, a.maxX, a.zFinale);
        bool accelerazioneOK = accelerazionePronta(a);
        std::printf("accelerazione: %s (speed@1 >= %.1f, speed@60 >= %.1f)\n",
                    accelerazioneOK ? "reattiva" : "lenta", MIN_SPEED_FIRST_FRAME, MIN_SPEED_FRAME_60);
        std::printf("offroad: %s (frame %d)\n", a.firstOffroad >= 0 ? "raggiunto" : "non raggiunto", a.firstOffroad);
        std::printf("ms/frame medio=%.3f max=%.3f (limite %.0f)\n", msMed, a.msMax, MAX_MS_FRAME);
        std::printf("replay 2x: %s", deterministico ? "IDENTICO" : "DIVERSO");
        if (!deterministico) std::printf(" (prima divergenza al frame %d)\n", primaDivergenza);

        if (!accelerazioneOK) { std::printf("FALLITO: accelerazione poco reattiva\n"); return 6; }
        if (!deterministico)   { std::printf("FALLITO: non deterministico\n"); return 2; }
        if (msMed > MAX_MS_FRAME) { std::printf("FALLITO: troppo lento\n"); return 3; }
        std::printf("GATE OK\n");
        return 0;
    }

    if (modo == "avvolgimento") {
        const i32 TOTAL_FRAMES = AVVOLGIMENTO_FRAMES;
        EsitoAvvolgimento e = eseguiAvvolgimento(outDir, livello, true, TOTAL_FRAMES);
        bool deterministico = e.hashA == e.hashB;
        i32 primaDivergenza = -1;
        for (size_t i = 0; i < e.hashA.size() && i < e.hashB.size(); ++i)
            if (e.hashA[i] != e.hashB[i]) { primaDivergenza = (i32)i; break; }
        std::printf("modo: avvolgimento, frame simulati: %d\n", TOTAL_FRAMES);
        std::printf("wrapCount=%d stimaTotalLen=%.0f zMax=%.1f zFinale=%.1f\n",
                    e.wrapCount, e.stimaTotalLen, e.zMax, e.zFinale);
        std::printf("replay 2x: %s", deterministico ? "IDENTICO" : "DIVERSO");
        if (!deterministico) std::printf(" (prima divergenza al frame %d)\n", primaDivergenza);
        bool ok = !e.anomalia && e.wrapCount >= 1 && deterministico;
        std::printf("%s\n", ok ? "GATE AVVOLGIMENTO OK" : "FALLITO: avvolgimento non rilevato o anomalia");
        return ok ? 0 : 5;
    }

    if (modo == "inattivita") {
        const i32 TOTAL_FRAMES = INATTIVITA_FRAMES;
        EsitoInattivita e = eseguiInattivita(outDir, livello, true, TOTAL_FRAMES);
        bool deterministico = e.hashA == e.hashB;
        i32 primaDivergenza = -1;
        for (size_t i = 0; i < e.hashA.size() && i < e.hashB.size(); ++i)
            if (e.hashA[i] != e.hashB[i]) { primaDivergenza = (i32)i; break; }
        std::printf("modo: inattivita, frame simulati: %d\n", TOTAL_FRAMES);
        std::printf("maxBollaDopoIdle=%d maxPersonaggioDopoIdle=%d (soglia bolla %d)\n",
                    e.maxBollaDopoIdle, e.maxPersonaggioDopoIdle, PIXEL_MIN);
        std::printf("replay 2x: %s", deterministico ? "IDENTICO" : "DIVERSO");
        if (!deterministico) std::printf(" (prima divergenza al frame %d)\n", primaDivergenza);
        bool ok = (e.maxBollaDopoIdle >= PIXEL_MIN) && deterministico;
        std::printf("%s\n", ok ? "GATE INATTIVITA OK" : "FALLITO: show/fumetto inattivita non visibile");
        return ok ? 0 : 5;
    }

    if (modo == "frenata") {
        const bool ok = verificaFrenata();
        std::printf("%s\n", ok ? "GATE FRENATA OK" : "FALLITO: freno non riduce la velocita'");
        return ok ? 0 : 9;
    }

    if (modo == "costa") {
        const bool ok = verificaCosta();
        std::printf("%s\n", ok ? "GATE COSTA OK" : "FALLITO: gas rilasciato non decelera");
        return ok ? 0 : 10;
    }

    if (modo == "bivio_uscita") {
        const bool ok = verificaBivioUscita(outDir);
        std::printf("%s\n", ok ? "GATE BIVIO USCITA OK" : "FALLITO: cambio tappa/bivio non fluido");
        return ok ? 0 : 16;
    }

    if (modo == "espansione") {
        const bool ok = verificaEspansioneTracciato();
        return ok ? 0 : 11;
    }

    if (modo == "frequenza") {
        const i32 frequenza = argc > 4 ? std::atoi(argv[4]) : 60;
        const bool ok = verificaFrequenza(outDir, livello, frequenza);
        return ok ? 0 : 15;
    }

    // --- modo smoke (default) ---
    const i32 TOTAL_FRAMES = SMOKE_FRAMES;
    Esito a = esegui(outDir, livello, true, TOTAL_FRAMES);
    Esito b = esegui(outDir, livello, false, TOTAL_FRAMES);

    bool deterministico = a.hash == b.hash;
    i32 primaDivergenza = -1;
    for (size_t i = 0; i < a.hash.size() && i < b.hash.size(); ++i)
        if (a.hash[i] != b.hash[i]) { primaDivergenza = (i32)i; break; }

    for (i32 f = 0; f < TOTAL_FRAMES; f += 60) std::printf("frameHash %d %u\n", f, a.hash[f]);
    double msMed = a.msTot / TOTAL_FRAMES;
    std::printf("modo: smoke, frame simulati: %d\n", TOTAL_FRAMES);
    std::printf("speed@60=%.2f speed@1=%.2f  X[min,max]=[%.1f,%.1f]  zFinale=%.1f\n",
                a.speed60, a.speedFirstFrame, a.minX, a.maxX, a.zFinale);
    bool accelerazioneOK = accelerazionePronta(a);
    std::printf("accelerazione: %s (speed@1 >= %.1f, speed@60 >= %.1f)\n",
                accelerazioneOK ? "reattiva" : "lenta", MIN_SPEED_FIRST_FRAME, MIN_SPEED_FRAME_60);
    std::printf("offroad: %s (frame %d)\n", a.firstOffroad >= 0 ? "raggiunto" : "non raggiunto", a.firstOffroad);
    std::printf("ms/frame medio=%.3f max=%.3f (limite %.0f)\n", msMed, a.msMax, MAX_MS_FRAME);
    std::printf("replay 2x: %s", deterministico ? "IDENTICO" : "DIVERSO");
    if (!deterministico) std::printf(" (prima divergenza al frame %d)\n", primaDivergenza);

    i32 rc = 0;
    if (a.speed60 <= 0.0f) { std::printf("FALLITO: nessuna partenza\n"); rc = 1; }
    if (!accelerazioneOK) { std::printf("FALLITO: accelerazione poco reattiva\n"); rc = 6; }
    if (!deterministico)   { std::printf("FALLITO: non deterministico\n"); rc = 2; }
    if (msMed > MAX_MS_FRAME) { std::printf("FALLITO: troppo lento\n"); rc = 3; }

    // Casi estesi T1-Run punti 5-6: avvolgimento playerZ oltre totalLen_ e inattivita -> show/fumetto.
    if (rc == 0) {
        EsitoAvvolgimento av = eseguiAvvolgimento(outDir, livello, true, AVVOLGIMENTO_FRAMES);
        bool avOK = !av.anomalia && av.wrapCount >= 1 && (av.hashA == av.hashB);
        std::printf("modo: avvolgimento, frame simulati: %d\n", AVVOLGIMENTO_FRAMES);
        std::printf("wrapCount=%d stimaTotalLen=%.0f zMax=%.1f zFinale=%.1f replay 2x: %s\n",
                    av.wrapCount, av.stimaTotalLen, av.zMax, av.zFinale,
                    av.hashA == av.hashB ? "IDENTICO" : "DIVERSO");
        if (!avOK) { std::printf("FALLITO: avvolgimento\n"); rc = 5; }

        EsitoInattivita ia = eseguiInattivita(outDir, livello, true, INATTIVITA_FRAMES);
        bool iaOK = (ia.maxBollaDopoIdle >= PIXEL_MIN) && (ia.hashA == ia.hashB);
        std::printf("modo: inattivita, frame simulati: %d\n", INATTIVITA_FRAMES);
        std::printf("maxBollaDopoIdle=%d maxPersonaggioDopoIdle=%d (soglia bolla %d) replay 2x: %s\n",
                    ia.maxBollaDopoIdle, ia.maxPersonaggioDopoIdle, PIXEL_MIN,
                    ia.hashA == ia.hashB ? "IDENTICO" : "DIVERSO");
        if (!iaOK) { std::printf("FALLITO: inattivita\n"); rc = 5; }
    }

    std::printf("%s\n", rc == 0 ? "GATE OK" : "GATE SMOKE FALLITO");
    return rc;
}
