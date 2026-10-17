// SPDX-License-Identifier: MIT
// prova_gate.cpp — gate Tappa 1 (strada che scorre), headless, core-only (niente SDL).
// Build : cmake --build build --target prova_gate
// Uso   : ./build/prova_gate [cartella_output=out/debugger] [livello=lv_01_neon] [modo]
//         modo = smoke (500 frame, default) | soak (30000 frame) |
//                avvolgimento (playerZ oltre totalLen_) | inattivita (show dopo 10 s senza tasti)
// Esegue la stessa simulazione 2 volte (seed e input fissi) e verifica:
//   1. partenza: playerSpeed > 0 al frame 60 (smoke/soak)
//   2. determinismo: frameHash identico ogni 60 frame fra run A e run B
//   3. prestazioni: ms/frame medio e massimo (obiettivo <= 20, meglio <= 12)
//   4. avvolgimento: playerZ torna a 0 al superare totalLen_ (nessuna discesa intermedia)
//   5. inattivita: dopo 500 frame senza input un show e' attivo e il fumetto e' nel screenshot
// Scrive: <out>/prova_gate*.log, screenshot PPM.
// Esegui dalla radice progetto (sglavinated_src): i dati sono letti con percorsi relativi.
// Exit code: 0 OK | 1 niente partenza | 2 replay non deterministico | 3 troppo lento |
//            4 cwd errata | 5 caso di prova non superato
#include "game/game.h"
#include "render/framebuffer.h"
#include "render/sprite_raw.h"
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace sgl;

namespace {

constexpr i32 SMOKE_FRAMES = 500;   // 10 s @ 50 Hz (default)
constexpr i32 SOAK_FRAMES  = 30000; // 10 min @ 50 Hz (accettazione)
constexpr i32 INATTIVITA_FRAMES = 900; // inattivita: 60 frame di input, poi silenzio
constexpr i32 AVVOLGIMENTO_FRAMES = 60000; // abbastanza per diversi giri del circuito
constexpr i32 PIXEL_MIN = 100;      // soglia pixel per ritenere presente un rettangolo di show
constexpr double MAX_MS_FRAME = 20.0;
constexpr f32 MIN_SPEED_FIRST_FRAME = 2.0f;
constexpr f32 MIN_SPEED_FRAME_60 = 100.0f;

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
            if (s.width > 0) sprites.push_back({id, std::move(s)});
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

    f32 speedPrecedente = 0.0f;
    f32 speedPrimoFrame = 0.0f;
    f32 speedDopo60 = 0.0f;
    for (i32 frame = 0; frame < 500; ++frame) {
        game.setInput(gas);
        game.update();
        game.render();
        const f32 speed = game.state().playerSpeed;
        if (frame == 0) speedPrimoFrame = speed;
        if (frame == 59) speedDopo60 = speed;
        if (speed + 0.001f < speedPrecedente) return false;
        speedPrecedente = speed;
    }

    std::printf("accelerazione 0-10s: primo frame=%.2f, 1.2s=%.2f, 10s=%.2f\n",
                speedPrimoFrame, speedDopo60, speedPrecedente);
    return speedPrimoFrame >= MIN_SPEED_FIRST_FRAME &&
           speedDopo60 >= MIN_SPEED_FRAME_60 &&
           speedPrecedente >= 240.0f && speedPrecedente <= 250.0f;
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
