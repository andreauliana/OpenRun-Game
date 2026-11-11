// SPDX-License-Identifier: MIT
// prova_gate.cpp — gate Tappa 1 (strada che scorre), headless, core-only (niente SDL).
// Build : cmake --build build --target prova_gate
// Uso   : ./build/prova_gate [cartella_output=UFFICIO/out/debugger] [livello=lv_01_neon] [modo]
//         modo = smoke (500 frame, default) | soak (30000 frame) |
//                avvolgimento (playerZ oltre totalLen_) | inattivita (show dopo 10 s senza tasti) |
//                percorso (profilo del tracciato + un giro intero con guida automatica) |
//                espansione (verifica espansione tracciato su tutti i 6 livelli) |
//                gara (tappe con bivi: le 4 combinazioni di scelte arrivano alle 6 piste) |
//                ciclo (R2: macchina a stati Menu -> Gara -> guida, ordine fasi,
//                       determinismo, screenshot PPM per fase)
//                bivio_bordi (bordo strada attraverso il bivio sul livello piu' curvo:
//                           nessun ribaltamento, offroad consecutivi <= N)
//                decorazioni (DECO1: catalogo skyline/oggetti per livello, cartelloni,
//                           ms/frame, screenshot PPM per livello)
//                sponsor_raw (VJ5: raw dei cartelloni visti dal core libretro -
//                           chiavi "id/stile", decodifica, stili dei livelli)
//                urto (RIV punto a: urto con un avversario, nessun ribaltamento,
//                      velocita' > 0 dopo l'urto, controlli attivi entro 1.5 s;
//                      AUTO1 J-A1: pose dell'auto - dritto, sterzo pieno sx/dx,
//                      ribaltamento - con screenshot in Test area/auto1/)
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
//            19 urto con avversario non superato
#include <cstdint>
using i32 = std::int32_t;
#include "game/game.h"
#include "game/salvataggi.h" // ARC1: SalvataggiGiocatore (salvataggi/giocatore.json)
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
constexpr i32 AVVOLGIMENTO_FRAMES = 6000; // ~3 giri del circuito (un giro ~1900 frame): basta per vedere il wrap; 60000 rendeva gate_t1.sh lungo ~15 min (timeout agenti)
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

// Un raw che decodifica (header e dimensioni valide) ma resta tutto trasparente
// (pixel tutti indice 0) vale come mancante: il file c'e', il contenuto no
// (es. conversione con alpha sotto 128 dappertutto). VJ5 11/11 su sprite_raw.cpp. VJ5 11/11 12:07 review renderer.cpp da1c8c6 (reserve terraOrdine_/spriteOrd_). VJ5 11/11 12:18 review road/road.cpp (project: doppia reserve DRAW_DIST poi DRAW_DIST*SAMPLES; incrocioD/portali ok, attenzione a gallerie corte con bivio luci). VJ5 11/11 12:24 review player/player.cpp: BORDO_SLOW applicato solo in coast (:49), non in frenata (accel_<0) né con gas, quindi frenare sul bordo usa solo BRAKE; limiti corsia scalano in modo diverso se larghezzaCorsia>1 (extra additivo su OFFROAD/X_LIMIT) o <=1 (moltiplicativo su OFFROAD, X_LIMIT non scalato) coerente col bivio ma asimmetrico; steer_ resta in [-1,1] per rampa verso obiettivo discreto. VJ5 11/11 12:43 review fasi.cpp MenuFase:138-145 fallback su avviaGara fallita chiama init() ma passa comunque a Gara con finita_=true/prossima_=Gara; stato gara/tappe/partenza puo' restare incoerente rispetto al percorso previsto (da verificare in integrazione). VJ5 11/11 12:55 review salvataggi.cpp: creaDirectorySeManca() :80-81 ritorna false anche se mkdir fallisce per EEXIST (dir gia' presente) senza distinguere errore reale; salva() ignora il ritorno ma tenta la scrittura su "salvataggi/giocatore.json.tmp" e fallira' se la dir non esiste davvero. Probe fopen("salvataggi","rb") non distingue directory da file su POSIX. save scrive classifica da g.classifica senza normalizzare e.nome a 6 char (load lo fa), possibile incoerenza.
bool spriteVisibile(const Sprite& s) {
    for (u8 idx : s.pixels)
        if (idx != 0) return true;
    return false;
}

// Tabelle dei cartelloni: stessi 7 sponsor e 3 stili di runner/sponsor_loader.cpp.
// A livello di file perche' servono sia al caricamento sia al controllo della
// scansione usata dal core libretro (verificaSponsorCore).
const char* kSponsorId[7] = {
    "curve_pericolose", "brinis_udine", "piston_high", "canna_bis",
    "slow_down", "erba_medica", "green_box"};
const char* kStile[3] = { "neon", "atomico", "foglie" };

// Cartelloni sponsor veri (raw SGLR generati da strumenti_py/png_a_raw.py): stesso
// ordine e stessi sponsor_id del runner (runner/sponsor_loader.cpp), cosi' il gate
// disegna gli stessi sprite del gioco. Da chiamare PRIMA di init().
void caricaSponsor(Game& game) {
    std::vector<std::pair<std::string, Sprite>> sprites;
    for (const char* id : kSponsorId) {
        for (const char* stile : kStile) {
            Sprite s = caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/sponsor/") + id + "_" + stile + ".sglraw");
            if (s.width > 0) {
                if (!spriteVisibile(s))
                    std::printf("prova_gate: cartellone sponsor %s/%s tutto trasparente\n", id, stile);
                sprites.push_back({std::string(id) + "/" + stile, std::move(s)});
            }
            else std::printf("prova_gate: cartellone sponsor %s/%s non trovato\n", id, stile);
        }
    }
    // Conteggio esplicito: un raw mancante non deve passare in silenzio (stesso
    // rilievo fatto al caricamento sponsor del core libretro, VJ5 11/11).
    std::printf("prova_gate: cartelloni sponsor %zu/%d caricati\n", sprites.size(), 21);
    game.setSponsorSprites(std::move(sprites));
}

// VJ5 (11/11, passo infinito, file nuovo): il core libretro (`avviaGioco` in
// src/libretro/sglavinated_libretro.cpp) carica i cartelloni scansionando la
// cartella raw e ricavando la chiave "id/stile" dallo stem del file (rfind('_')),
// mentre il runner (runner/sponsor_loader.cpp) usa le 7x3 combinazioni fisse.
// Qui si replica la scansione del core e si verifica che:
//  1. ogni .sglraw decodifichi e abbia pixel non trasparenti;
//  2. le chiavi ricavate siano esattamente le 21 attese (una mancante = cartellone
//     assente nel core senza avviso: il core non logga i mancanti);
//  3. ogni livello abbia stile_cartellone fra i 3 stili con i raw, cosi' la ricerca
//     "id/stile" ha sempre successo e l'ordine di scansione del filesystem non
//     decide il ripiego "qualsiasi stile" (diverso fra runner e core).
bool verificaSponsorCore() {
    namespace fs = std::filesystem;
    const std::string dir = "motore_nuovo/dati/sprite_raw/sponsor";
    std::vector<std::string> attese;
    for (const char* id : kSponsorId)
        for (const char* stile : kStile) attese.push_back(std::string(id) + "/" + stile);

    std::error_code sec;
    if (!fs::is_directory(dir, sec)) {
        std::printf("FALLITO: %s mancante o illeggibile: il core caricherebbe 0 cartelloni in silenzio\n",
                    dir.c_str());
        return false;
    }
    bool ok = true;
    std::vector<std::string> chiavi;
    i32 file = 0;
    for (const auto& e : fs::directory_iterator(dir, sec)) {
        if (sec) break;
        std::error_code se;
        if (!e.is_regular_file(se) || se) continue;
        const fs::path p = e.path();
        if (p.extension() != ".sglraw") continue;
        ++file;
        const std::string stem = p.stem().string();
        const size_t pos = stem.rfind('_');
        if (pos == std::string::npos) {
            std::printf("FALLITO: %s senza '_': il core non ne ricava la chiave\n",
                        p.filename().string().c_str());
            ok = false;
            continue;
        }
        const std::string chiave = stem.substr(0, pos) + "/" + stem.substr(pos + 1);
        if (std::find(attese.begin(), attese.end(), chiave) == attese.end())
            std::printf("ATTENZIONE: %s ricava la chiave %s, non usata dal gioco\n",
                        p.filename().string().c_str(), chiave.c_str());
        const Sprite s = caricaSpriteRaw(p.string());
        if (s.width <= 0) {
            std::printf("FALLITO: %s non decodifica\n", p.filename().string().c_str());
            ok = false;
        } else if (!spriteVisibile(s)) {
            std::printf("FALLITO: %s decodificato ma tutto trasparente\n", p.filename().string().c_str());
            ok = false;
        }
        chiavi.push_back(chiave);
    }
    std::printf("sponsor core: %d file .sglraw, %zu chiavi ricavate, %zu attese\n",
                file, chiavi.size(), attese.size());
    for (const std::string& a : attese) {
        if (std::find(chiavi.begin(), chiavi.end(), a) == chiavi.end()) {
            std::printf("FALLITO: chiave attesa mancante %s\n", a.c_str());
            ok = false;
        }
    }
    if (chiavi.size() != attese.size()) {
        std::printf("FALLITO: chiavi ricavate %zu, attese %zu\n", chiavi.size(), attese.size());
        ok = false;
    }
    // Ordine della cartella: il core lo eredita dal filesystem (non specificato).
    // Se non coincide con l'ordine fisso del runner, il ripiego "qualsiasi stile"
    // sceglie un cartellone diverso fra runner e core: rilievo, non errore finche'
    // ogni livello dichiara uno stile fra i 3 con i raw.
    std::printf("sponsor core: ordine cartella %s all'ordine del runner\n",
                chiavi == attese ? "uguale" : "DIVERSO");

    static const char* kLivelli[6] = { "lv_01_neon", "lv_02_atomico", "lv_03_foglie",
                                       "lv_04_cromo", "lv_05_spray", "lv_06_desktop" };
    for (const char* lv : kLivelli) {
        std::ifstream f(std::string("motore_nuovo/dati/livelli/") + lv + "/livello.json");
        std::stringstream ss;
        ss << f.rdbuf();
        JsonReader r(ss.str());
        std::string stile;
        if (!r.parse() || !jsonIsObject(r.valore())) {
            std::printf("FALLITO: %s illeggibile per il controllo stile cartellone\n", lv);
            ok = false;
            continue;
        }
        JsonValue tg;
        if (jsonGet(jsonAsObject(r.valore()), "tema_grafico", tg) && jsonIsObject(tg)) {
            JsonValue sv;
            if (jsonGet(jsonAsObject(tg), "stile_cartellone", sv) && jsonIsString(sv))
                stile = jsonAsString(sv);
        }
        bool disponibile = false;
        for (const char* s : kStile)
            if (stile == s) disponibile = true;
        std::printf("sponsor core: %s stile_cartellone=%s %s\n", lv,
                    stile.empty() ? "(assente)" : stile.c_str(),
                    disponibile ? "ok" : "FUORI DAI 3 STILI (ripiego)");
        if (!disponibile) ok = false;
    }
    return ok;
}

// Auto rivali (RIV punto b): sprite veri dal catalogo, stesso percorso del runner.
void caricaRivali(Game& game) {
    static const char* kRivale[7] = { "rosso", "azzurro", "giallo", "verde", "bianco", "arancio", "viola" };
    std::vector<Sprite> sprites;
    for (const char* colore : kRivale) {
        Sprite s = caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/rivali/auto_rivale_") + colore + "_retro.sglraw");
        if (s.width > 0) {
            if (!spriteVisibile(s))
                std::printf("prova_gate: sprite rivale %s tutto trasparente\n", colore);
            sprites.push_back(std::move(s));
        }
        else std::printf("prova_gate: sprite rivale %s non trovato\n", colore);
    }
    game.setRivaliSprites(std::move(sprites));
    // Traffico comune: stessi raw del runner.
    static const char* kTraffico[4] = { "utilitaria_bianca", "furgone_grigio", "berlina_blu", "camioncino_beige" };
    std::vector<Sprite> traffico;
    for (const char* nome : kTraffico) {
        Sprite s = caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/traffico/traffico_") + nome + "_retro.sglraw");
        if (s.width > 0) {
            if (!spriteVisibile(s))
                std::printf("prova_gate: sprite traffico %s tutto trasparente\n", nome);
            traffico.push_back(std::move(s));
        }
        else std::printf("prova_gate: sprite traffico %s non trovato\n", nome);
    }
    game.setTrafficoSprites(std::move(traffico));
}

// Skyline della tappa (DECO1 2b): stesso formato raw degli sponsor, caricato
// per ogni livello prima di init(); il core lo ricarica in caricaLivello a
// ogni cambio tappa, quindi il primo livello e' coerente col runner.
void caricaSkyline(Game& game, const std::string& livello) {
    Sprite s = caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/scenari/") + livello + "/skyline.sglraw");
    if (s.width > 0) game.setSkylineSprite(std::move(s));
    else std::printf("prova_gate: skyline %s non trovato\n", livello.c_str());
}

// V8 (Caricamento, @developer 11/11): il core ora misura init()/caricaLivello()/
// avviaGara() e scrive "CARICAMENTO ... ms" su stdout (game.cpp:486/654/847/1024/
// 1172/1224). Il gate cattura quelle righe nel log, ma per il criterio di V47
// ("se un caricamento supera 1 s sul Mac, alleggeriscilo") serve un numero
// misurato dal lato test: qui si ripete lo stesso percorso del runner (sponsor,
// rivali, skyline, poi init) e si stampa il tempo di
//  1. init()    = avvio del livello (config, livello, pose auto, frasi, musica),
//     cioe' la base su cui poi si presenta il menu;
//  2. avviaGara() sulla stessa partita = partenza gara, che richiama init()
//     (game.cpp:1205) e quindi ricarica livello + pose + frasi: il costo del
//     doppio caricamento si vede nel rapporto avviaGara/init;
//  3. un secondo avviaGara(..., true): partenza di una gara corsa singola dopo
//     la prima, come quando si riparte senza uscire dal gioco.
// Soglia: 1000 ms ciascuno (Mac), exit 30 se fuori.
bool verificaCaricamento(const std::string& livello) {
    const double LIMITE_MS = 1000.0;
    auto cronometro = [](auto&& azione) {
        const auto t0 = std::chrono::steady_clock::now();
        azione();
        return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    };

    Game game(12345u);
    caricaSponsor(game); caricaRivali(game); caricaSkyline(game, livello);
    const double msInit = cronometro([&game, &livello] { game.init(livello); });
    bool garaOk = false;
    const double msGara = cronometro([&game, &garaOk] { garaOk = game.avviaGara(); });
    bool secondaOk = false;
    const double msSeconda = cronometro([&game, &secondaOk] {
        secondaOk = game.avviaGara("motore_nuovo/dati/percorso_gara.json", true);
    });

    std::printf("caricamento %s: init=%.1f ms  avviaGara=%.1f ms  avviaGara singola=%.1f ms "
                "(avviaGara/init=%.2fx) soglia %.0f ms\n",
                livello.c_str(), msInit, msGara, msSeconda,
                msInit > 0.0 ? msGara / msInit : 0.0, LIMITE_MS);
    // avviaGara() non usa il livello di init(): riparte dalla "partenza" di
    // percorso_gara.json (game.cpp:1181/1205), quindi con lv_06 le due misure
    // di gara sono in realta' la prima tappa del percorso. Va detto nel log,
    // altrimenti il numero sembra del livello sbagliato.
    std::printf("caricamento: avviaGara carica la tappa di partenza del percorso (%s), "
                "non %s\n", game.tappaCorrente().c_str(), livello.c_str());
    bool ok = garaOk && secondaOk;
    if (!ok) std::printf("FALLITO: avviaGara() non ha caricato la gara\n");
    if (msInit > LIMITE_MS) { std::printf("FALLITO: init oltre %.0f ms\n", LIMITE_MS); ok = false; }
    if (msGara > LIMITE_MS) { std::printf("FALLITO: avvio gara oltre %.0f ms\n", LIMITE_MS); ok = false; }
    if (msSeconda > LIMITE_MS) { std::printf("FALLITO: cambio tappa oltre %.0f ms\n", LIMITE_MS); ok = false; }
    return ok;
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
    caricaSponsor(game); caricaRivali(game); caricaSkyline(game, livello); game.init(livello);
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
    caricaSponsor(game); caricaRivali(game);
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

// --- AUTO1 J-A1 (@developer-junior): pose dell'auto del giocatore, in `urto` ---
// A fermi (nessun gas) il mondo resta immobile: speed 0 non avanza e lo sterzo
// non sposta l'auto, quindi i pixel dell'auto si possono confrontare con un
// riferimento disegnato dallo stesso Renderer caricando le stesse pose di
// Game::init. Sterzo pieno a sinistra -> posa di sterzo pieno (sx2 del
// contratto del blocco, SX_4 nel codice), a destra -> lato destro (specchio di
// sx se i fotogrammi dx mancano), centro -> dritto; durante il ribaltamento i
// pixel dell'auto cambiano fra un frame e l'altro (la rotazione si vede).
// L'inclinazione che il motore applica sterzando (steeringLean in game.cpp) e'
// cercata su una griglia di angoli, cosi' il gate non dipende dal valore
// esatto. Screenshot PPM in UFFICIO/Officina/Test area/auto1/ (non su GitHub).
bool verificaPoseAuto(const std::string& livello) {
    namespace fs = std::filesystem;
    constexpr i32 W = Framebuffer::WIDTH;
    constexpr i32 H = Framebuffer::HEIGHT;
    const std::string dirOut = "UFFICIO/Officina/Test area/auto1";
    std::error_code ec;
    fs::create_directories(dirOut, ec);

    // Pose come le carica Game::init: prima sprite_raw/giocatore/pose/ (Andrea),
    // se manca la dritta i fotogrammi degli agenti, poi il raw storico.
    static const char* const POSE[POSA_GIOCATORE_COUNT] = {
        "auto_dritta", "auto_sx1", "auto_sx2", "auto_sx3", "auto_sx4",
        "auto_dx1", "auto_dx2", "auto_dx3", "auto_dx4", "auto_salita", "auto_discesa"};
    const std::string dirPose = "motore_nuovo/dati/sprite_raw/giocatore/pose/";
    const std::string dirAgenti = "motore_nuovo/dati/sprite_raw/giocatore/";
    Sprite pose[POSA_GIOCATORE_COUNT];
    for (i32 k = 0; k < POSA_GIOCATORE_COUNT; ++k)
        pose[k] = caricaSpriteRaw(dirPose + POSE[k] + ".sglraw");
    if (pose[POSA_GIOCATORE_DRITTA].width <= 0) {
        static const struct { i32 posa; const char* nome; } AGENTI[] = {
            {POSA_GIOCATORE_DRITTA, "auto_giocatore_dritta"},
            {POSA_GIOCATORE_SX_2, "auto_giocatore_sterzo_sx_leggero"},
            {POSA_GIOCATORE_SX_4, "auto_giocatore_sterzo_sx_forte"},
            {POSA_GIOCATORE_DX_2, "auto_giocatore_sterzo_dx_leggero"},
            {POSA_GIOCATORE_DX_4, "auto_giocatore_sterzo_dx_forte"},
            {POSA_GIOCATORE_SALITA, "auto_giocatore_salita"},
            {POSA_GIOCATORE_DISCESA, "auto_giocatore_discesa"}};
        for (const auto& f : AGENTI)
            pose[f.posa] = caricaSpriteRaw(dirAgenti + f.nome + ".sglraw");
    }
    if (pose[POSA_GIOCATORE_DRITTA].width <= 0)
        pose[POSA_GIOCATORE_DRITTA] = caricaSpriteRaw(dirAgenti + "auto_giocatore.sglraw");
    if (pose[POSA_GIOCATORE_DRITTA].width <= 0) {
        std::printf("  pose auto: FALLITO (raw della posa dritta mancante)\n");
        return false;
    }
    bool haSterzo = false;
    for (i32 k = POSA_GIOCATORE_SX_1; k <= POSA_GIOCATORE_DX_4; ++k)
        haSterzo = haSterzo || pose[k].width > 0;
    if (!haSterzo) {
        std::printf("  pose auto: FALLITO (nessuna posa di sterzo in %s o %s)\n",
                    dirPose.c_str(), dirAgenti.c_str());
        return false;
    }

    // Geometria a schermo: stessa formula di Renderer::drawPlayerCar.
    const Sprite& dritta = pose[POSA_GIOCATORE_DRITTA];
    i32 xMin = dritta.width, xMax = -1;
    for (i32 y = 0; y < dritta.height; ++y)
        for (i32 x = 0; x < dritta.width; ++x) {
            const u8 idx = dritta.pixels[(size_t)y * dritta.width + x];
            const u32 c = !dritta.palette.empty()
                ? dritta.palette[idx % (i32)dritta.palette.size()] : (idx ? 0xFF000000u : 0u);
            if ((c >> 24) != 0) { xMin = std::min(xMin, x); xMax = std::max(xMax, x); }
        }
    const i32 baseW = xMax >= xMin ? xMax - xMin + 1 : dritta.width;
    const i32 scala = std::max(1, (i32)std::lround((f32)AUTO_GIOCATORE_LARGHEZZA / (f32)baseW));
    i32 wMax = 1, hMax = 1;
    for (i32 k = 0; k < POSA_GIOCATORE_COUNT; ++k)
        if (pose[k].width > 0) {
            wMax = std::max(wMax, pose[k].width * scala);
            hMax = std::max(hMax, pose[k].height * scala);
        }
    // Riquadro dell'auto: x >= 80 resta fuori dallo spettro audio (basso a
    // sinistra, fino a x = 71), le cui barre animate falserebbero i confronti.
    const i32 bx0 = std::max(160 - wMax / 2 - 2, 80);
    const i32 bx1 = std::min(160 + wMax / 2 + 2, W - 1);
    const i32 by0 = std::max(AUTO_GIOCATORE_TERRA_Y - hMax - 2, 0);
    const i32 by1 = std::min(AUTO_GIOCATORE_TERRA_Y + 1, H - 1);

    // Riferimenti: la stessa posa disegnata da un Renderer identico su fondo
    // piatto, cosi' la maschera (pixel non piu' del fondo) e' l'auto.
    Framebuffer fbRef;
    Palette palRef;
    Renderer renRef(fbRef, palRef);
    for (i32 k = 0; k < POSA_GIOCATORE_COUNT; ++k)
        renRef.impostaGiocatoreSprite(k, pose[k]);
    const u32 FONDO = 0xFF010203u;
    // Ombra di contatto (Andrea 07/11): sotto l'auto il fondo e' scurito al 45%
    // o al 70%; quei pixel sono strada in ombra, non auto, e restano fuori
    // dalla maschera come il fondo.
    auto ombra = [](u32 c, u32 luce) {
        return (c & 0xFF000000u) | ((((c >> 16) & 0xFFu) * luce / 100u) << 16) |
               ((((c >> 8) & 0xFFu) * luce / 100u) << 8) | ((c & 0xFFu) * luce / 100u);
    };
    const u32 FONDO_OMBRA_A = ombra(FONDO, 45u), FONDO_OMBRA_B = ombra(FONDO, 70u);
    auto eFondo = [&](u32 c) { return c == FONDO || c == FONDO_OMBRA_A || c == FONDO_OMBRA_B; };
    auto riferimento = [&](i32 posa, i32 g) {
        fbRef.clear(FONDO);
        renRef.drawPlayerCar(0.0f, 0.0f, false, (f32)g * 0.02f, 0.0f, false, -1.0f, nullptr, posa);
        return std::vector<u32>(fbRef.data(), fbRef.data() + (size_t)W * H);
    };
    auto conta = [&](const std::vector<u32>& cap, const std::vector<u32>& ref) {
        i32 n = 0;
        for (i32 y = by0; y <= by1; ++y)
            for (i32 x = bx0; x <= bx1; ++x) {
                const size_t i = (size_t)y * W + x;
                if (eFondo(ref[i])) continue;
                if (cap[i] != ref[i]) ++n;
            }
        return n;
    };
    auto maschera = [&](const std::vector<u32>& ref) {
        i32 n = 0;
        for (i32 y = by0; y <= by1; ++y)
            for (i32 x = bx0; x <= bx1; ++x)
                if (!eFondo(ref[(size_t)y * W + x])) ++n;
        return n;
    };
    // Miglior confronto su una griglia di inclinazioni (-0.60..+0.60 rad).
    struct Match { i32 n = 0; i32 g = 0; };
    auto migliore = [&](const std::vector<u32>& cap, i32 posa) {
        Match best;
        best.n = -1;
        for (i32 g = -30; g <= 30; ++g) {
            const i32 n = conta(cap, riferimento(posa, g));
            if (best.n < 0 || n < best.n) { best.n = n; best.g = g; }
        }
        return best;
    };

    const std::vector<u32> refDritta = riferimento(POSA_GIOCATORE_DRITTA, 0);
    const std::vector<u32> refSx = riferimento(POSA_GIOCATORE_SX_4, 0);
    const std::vector<u32> refDx = riferimento(POSA_GIOCATORE_DX_4, 0);
    const i32 maskD = maschera(refDritta), maskS = maschera(refSx), maskX = maschera(refDx);
    if (maskD < 64 || maskS < 64 || maskX < 64) {
        std::printf("  pose auto: FALLITO (riferimento vuoto: dritto %d, sx %d, dx %d px)\n",
                    maskD, maskS, maskX);
        return false;
    }

    // Quattro catture a fermi: nessun gas, quindi il mondo e' immobile e le
    // differenze sono solo l'auto scelta dal motore.
    Game game(12345u);
    caricaSponsor(game); caricaRivali(game); caricaSkyline(game, livello);
    game.init(livello);
    auto passo = [&](const Pad& p) { game.setInput(p); game.update(); game.render(); };
    auto cattura = [&]() {
        return std::vector<u32>(game.fb().data(), game.fb().data() + (size_t)W * H);
    };

    Pad neutro{};
    for (i32 i = 0; i < 20; ++i) passo(neutro);
    const std::vector<u32> c0 = cattura();
    salvaPPM(dirOut + "/auto1_dritto.ppm", c0.data(), W, H);

    Pad sx{};
    sx.held = Pad::Left;
    for (i32 i = 0; i < 20; ++i) passo(sx);
    const std::vector<u32> cl = cattura();
    salvaPPM(dirOut + "/auto1_sx2.ppm", cl.data(), W, H);

    Pad dx{};
    dx.held = Pad::Right;
    for (i32 i = 0; i < 20; ++i) passo(dx);
    const std::vector<u32> cr = cattura();
    salvaPPM(dirOut + "/auto1_dx2.ppm", cr.data(), W, H);

    for (i32 i = 0; i < 20; ++i) passo(neutro);
    const std::vector<u32> c0b = cattura();

    // Pendenza alla partenza: con il tratto iniziale in pendenza la posa base
    // sarebbe "salita" e il confronto col riferimento dritto non avrebbe senso.
    const Road& road = game.road();
    const f32 dzP = 2.0f * (f32)Road::SEG_LEN;
    const f32 pendenza = road.quotaAt(road.playerZ + dzP) - road.quotaAt(road.playerZ - dzP);

    // Ribaltamento: stessa procedura del gate `crash` (fuori strada > 2 s),
    // poi due catture nella prima meta' dell'animazione (x ferma, niente fumo):
    // fra un frame e l'altro l'auto ruota e i pixel cambiano.
    Pad gasDx{};
    gasDx.held = Pad::A | Pad::Right;
    bool flipping = false;
    for (i32 i = 0; i < 1500 && !flipping; ++i) {
        passo(gasDx);
        flipping = game.state().playerFlipping;
    }
    if (!flipping) {
        std::printf("  pose auto: FALLITO (nessun ribaltamento entro 1500 frame)\n");
        return false;
    }
    const i32 FREQ = ::leggiFrequenzaDaConfig();
    const u32 frameFlip = (u32)(1.5f * (f32)FREQ);
    const u32 f1 = (u32)(0.10f * (f32)frameFlip);
    const u32 f2 = (u32)(0.20f * (f32)frameFlip);
    while (game.state().playerFlipping && game.state().playerFlipFrame < f1) passo(neutro);
    const std::vector<u32> flip1 = cattura();
    while (game.state().playerFlipping && game.state().playerFlipFrame < f2) passo(neutro);
    const std::vector<u32> flip2 = cattura();
    salvaPPM(dirOut + "/auto1_ribaltamento.ppm", flip2.data(), W, H);

    // Riquadro del ribaltamento: rotazione + sollevamento (fino a 16 px).
    const i32 rFlip = (i32)(0.5f * std::sqrt((f32)(wMax * wMax + hMax * hMax))) + 24;
    const i32 cyAuto = AUTO_GIOCATORE_TERRA_Y - hMax / 2;
    const i32 fx0 = std::max(160 - rFlip, 80), fx1 = std::min(160 + rFlip, W - 1);
    const i32 fy0 = std::max(cyAuto - rFlip, 0), fy1 = std::min(cyAuto + rFlip, H - 1);
    i32 flipCambia = 0;
    for (i32 y = fy0; y <= fy1; ++y)
        for (i32 x = fx0; x <= fx1; ++x) {
            const size_t i = (size_t)y * W + x;
            if (flip1[i] != flip2[i]) ++flipCambia;
        }
    i32 flipVsDritto = 0;
    for (i32 y = by0; y <= by1; ++y)
        for (i32 x = bx0; x <= bx1; ++x) {
            const size_t i = (size_t)y * W + x;
            if (eFondo(refDritta[i])) continue;
            if (flip2[i] != refDritta[i]) ++flipVsDritto;
        }

    // Soglie: <= 2% di pixel diversi = stessa posa; il ribaltamento deve
    // cambiare almeno ~2.5% del riquadro (lo sfondo resta fermo a fermi).
    const i32 ugD = std::max(16, maskD / 50);
    const i32 ugS = std::max(16, maskS / 50);
    const i32 ugX = std::max(16, maskX / 50);
    const i32 flipSoglia = std::max(64, (fx1 - fx0 + 1) * (fy1 - fy0 + 1) / 40);
    const Match m0 = migliore(c0, POSA_GIOCATORE_DRITTA);
    const Match mSx = migliore(cl, POSA_GIOCATORE_SX_4);
    const Match mDx = migliore(cr, POSA_GIOCATORE_DX_4);
    const Match m0b = migliore(c0b, POSA_GIOCATORE_DRITTA);

    std::printf("  pose auto (J-A1): pendenza partenza %.1f | riquadro %dx%d | maschere d/s/x=%d/%d/%d px\n",
                pendenza, bx1 - bx0 + 1, by1 - by0 + 1, maskD, maskS, maskX);
    std::printf("    centro->dritto %d (ang %.2f, soglia %d) | sterzo pieno sx->sx %d (ang %.2f, soglia %d)\n",
                m0.n, m0.g * 0.02, ugD, mSx.n, mSx.g * 0.02, ugS);
    std::printf("    destra->dx %d (ang %.2f, soglia %d) | ritorno centro->dritto %d (soglia %d)\n",
                mDx.n, mDx.g * 0.02, ugX, m0b.n, ugD);
    std::printf("    ribaltamento: pixel cambiati fra due frame %d (soglia %d), rispetto al dritto %d (soglia %d)\n",
                flipCambia, flipSoglia, flipVsDritto, maskD / 4);
    std::printf("    screenshot: %s/auto1_dritto.ppm auto1_sx2.ppm auto1_dx2.ppm auto1_ribaltamento.ppm\n",
                dirOut.c_str());

    const bool ok = haSterzo && std::abs(pendenza) <= 80.0f &&
                    m0.n <= ugD && m0b.n <= ugD &&
                    mSx.n <= ugS && mDx.n <= ugX &&
                    flipCambia >= flipSoglia && flipVsDritto >= maskD / 4;
    if (!ok) std::printf("  pose auto: FALLITO (vedi valori sopra)\n");
    return ok;
}

// --- Blocco RIV punto a (@developer-junior): gate `urto`.
// Avvia una gara, guida verso il primo avversario davanti e provoca un urto.
// Verifica: nessun ribaltamento (playerFlipping sempre falso), velocita' > 0
// dopo l'urto (auto non bloccata), controlli di nuovo attivi entro 1,5 s
// (sterzo risponde). Il contatto usa la stessa finestra di
// Game::collisioneRivaleVicino (Game::URTO_Z_MIN..URTO_Z_MAX davanti alla
// telecamera, |dx| < Game::URTO_LARGHEZZA, fuori dal bivio attivo; 29/10).
// Deterministico: seed fissa, input scriptati, replay 2x IDENTICO.
bool verificaUrto(const std::string& livello) {
    const i32 FREQ = ::leggiFrequenzaDaConfig();
    const i32 BUMP_FRAMES = (i32)(1.0f * (f32)FREQ);     // durata bump (config.json: 1.0 s)
    const i32 CONTROLLI_FRAMES = (i32)(1.5f * (f32)FREQ); // controlli attivi entro 1.5 s
    // Tempo massimo per cercare un contatto (evita giri infiniti). I rivali
    // partono davanti e all'inizio accelerano piu' del giocatore: con 1000
    // frame e la ricerca entro 2000 unita' il contatto non arrivava mai (27/10).
    const i32 MAX_FRAME = 60 * FREQ;

    struct Run {
        std::vector<u32> hash;
        bool contatto = false;
        i32 frameContatto = -1;
        bool ribaltatoDuranteBump = true; // nessun ribaltamento entro BUMP_FRAMES dopo il contatto
        bool speedSemprePositiva = true;
        bool controlliAttivi = false;
        double msMax = 0.0;
        double msTot = 0.0;
        i32 frames = 0;
    };

    auto esegui = [&](Run& r) -> bool {
        Game game(12345u);
        caricaSponsor(game); caricaRivali(game);
        if (!game.avviaGara()) return false;
        i32 frame = 0;
        bool contatto = false;
        i32 frameContatto = -1;
        bool inControlli = false;
        f32 xPrimaControlli = 0.0f;
        i32 frameDopoContatto = 0;
        while (frame < MAX_FRAME) {
            const GameState& s = game.state();
            const Road& road = game.road();
            Pad pad;
            if (!contatto) {
                // Gas sempre pieno: serve velocita' sufficiente per che lo sterzo
                // risponda (risposta e' proporzionale a speed/MAX_SPEED, zero da fermi).
                pad.held = (u64)Pad::A;
                // Sterzo verso l'avversario piu' vicino davanti per allinearsi alla
                // sua corsia e provocare il contatto (finestra Game::URTO_Z_MIN..MAX).
                f32 obiettivo = s.playerX;
                f32 dzMigliore = 1e9f;
                auto eTraffico = [&game](const SpriteInstance& t) {
                    for (i32 i = 0; i < Game::TRAFFICO; ++i)
                        if (game.veicolo(i).attivo && std::abs(game.veicolo(i).z - t.worldZ) < 1.0f) return true;
                    return false;
                };
                for (const SpriteInstance& t : game.traffico()) {
                    const f32 dz = t.worldZ - s.playerZ;
                    if (eTraffico(t)) continue; // questo gate cerca un avversario (il traffico ha il suo gate)
                    if (dz > 0.0f && dz < dzMigliore) {
                        dzMigliore = dz;
                        obiettivo = t.worldX;
                    }
                }
                const f32 margine = 100.0f;
                if (s.playerX > obiettivo + margine) pad.held |= (u64)Pad::Left;
                else if (s.playerX < obiettivo - margine) pad.held |= (u64)Pad::Right;
            } else if (!inControlli) {
                pad.held = (u64)Pad::A;
                frameDopoContatto++;
                if (frameDopoContatto >= BUMP_FRAMES) {
                    inControlli = true;
                    xPrimaControlli = s.playerX;
                }
            } else {
                pad.held = (u64)(Pad::A | Pad::Left);
                frameDopoContatto++;
                // Appena lo sterzo ha risposto la prova e' finita (11/11 sera):
                // tenendolo per tutti gli 1.5 s, con lo sterzo piu' pronto
                // l'auto usciva di strada e prendeva un cartellone, che non
                // c'entra con l'urto col rivale.
                if (s.playerX < xPrimaControlli - 15.0f) {
                    r.controlliAttivi = true;
                    break;
                }
                if (frameDopoContatto >= BUMP_FRAMES + CONTROLLI_FRAMES) {
                    // Controlli attivi: lo sterzo a sinistra deve spostare l'auto
                    // a sinistra (soglia bassa: bastano pochi frame di risposta).
                    r.controlliAttivi = (s.playerX < xPrimaControlli - 15.0f);
                    break;
                }
            }
            game.setInput(pad);
            auto t0 = std::chrono::steady_clock::now();
            game.update();
            game.render();
            double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
            if (ms > r.msMax) r.msMax = ms;
            r.msTot += ms;
            ++r.frames;
            const GameState& dopo = game.state();
            if (contatto) {
                // Durante il bump: nessun ribaltamento (il contatto con un rivale
                // non deve mai ribaltare l'auto), velocita' sempre > 0.
                if (dopo.playerFlipping) r.ribaltatoDuranteBump = false;
                if (dopo.playerSpeed <= 0.0f) r.speedSemprePositiva = false;
            } else {
                // Rilevamento contatto: stessa soglia di Game::collisioneRivaleVicino.
                for (const SpriteInstance& t : game.traffico()) {
                    bool traffico = false;
                    for (i32 i = 0; i < Game::TRAFFICO; ++i)
                        if (game.veicolo(i).attivo && std::abs(game.veicolo(i).z - t.worldZ) < 1.0f) traffico = true;
                    if (traffico) continue;
                    const f32 dz = t.worldZ - dopo.playerZ;
                    // Fuori dal bivio: li' il ramo scelto viene ricentrato e
                    // playerX salta, la prova sullo sterzo non avrebbe senso
                    // (28/10: con i tratti PIST1 il contatto cadeva nel bivio).
                    const bool nelBivio = game.road().bivioAttivo() &&
                                          dopo.playerZ >= game.road().bivioInizioZ();
                    if (dz > Game::URTO_Z_MIN && dz < Game::URTO_Z_MAX &&
                        std::abs(t.worldX - dopo.playerX) < Game::URTO_LARGHEZZA &&
                        !dopo.playerOffroad && !nelBivio) {
                        contatto = true;
                        frameContatto = frame;
                        frameDopoContatto = 0;
                        break;
                    }
                }
            }
            r.hash.push_back(game.frameHash());
            ++frame;
        }
        r.contatto = contatto;
        r.frameContatto = frameContatto;
        return contatto;
    };

    auto msMedio = [](const Run& r) { return r.frames > 0 ? r.msTot / r.frames : 0.0; };
    Run a, b;
    if (!esegui(a)) {
        std::printf("urto %s: FALLITO (nessun contatto con avversario entro %d frame)\n",
                    livello.c_str(), MAX_FRAME);
        return false;
    }
    if (!esegui(b)) {
        std::printf("urto %s: FALLITO (secondo run senza contatto)\n", livello.c_str());
        return false;
    }

    bool deterministico = (a.hash == b.hash);
    i32 primaDivergenza = -1;
    for (size_t i = 0; i < a.hash.size() && i < b.hash.size(); ++i)
        if (a.hash[i] != b.hash[i]) { primaDivergenza = (i32)i; break; }

    std::printf("modo: urto, livello: %s, contatto frame %d / %d, replay 2x: %s\n",
                livello.c_str(), a.frameContatto, b.frameContatto,
                deterministico ? "IDENTICO" : "DIVERSO");
    if (!deterministico) std::printf("  prima divergenza al frame %d\n", primaDivergenza);
    std::printf("  ribaltamento dopo urto: %s | velocita' sempre > 0 dopo urto: %s | "
                "controlli attivi entro 1.5 s: %s | ms/frame medio A=%.2f B=%.2f (limite %.0f), max A=%.2f B=%.2f\n",
                (a.ribaltatoDuranteBump && b.ribaltatoDuranteBump) ? "nessuno (OK)" : "RILEVATO (KO)",
                (a.speedSemprePositiva && b.speedSemprePositiva) ? "sì (OK)" : "no (KO)",
                (a.controlliAttivi && b.controlliAttivi) ? "sì (OK)" : "no (KO)",
                msMedio(a), msMedio(b), MAX_MS_FRAME, a.msMax, b.msMax);

    // Media e non massimo: con ctest -j8 un singolo frame puo' superare 20 ms
    // per il carico della macchina (21-28 ms visti il 27/10), non per il gioco.
    bool ok = deterministico && a.ribaltatoDuranteBump && b.ribaltatoDuranteBump &&
              a.speedSemprePositiva && b.speedSemprePositiva &&
              a.controlliAttivi && b.controlliAttivi &&
              msMedio(a) <= MAX_MS_FRAME && msMedio(b) <= MAX_MS_FRAME;
    // AUTO1 J-A1 (@developer-junior): pose dell'auto del giocatore.
    const bool poseOk = verificaPoseAuto(livello);
    ok = ok && poseOk;
    std::printf("%s\n", ok ? "GATE URTO OK" : "FALLITO: urto con avversario non conforme");
    return ok;
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
        // Tiene la corsia (06/11 sera: le curve spingono fuori col quadrato
        // della velocita', a solo gas l'auto finirebbe sul bordo e frenerebbe).
        // Lo sterzo non cambia la spinta: si misura sempre l'accelerazione.
        Pad pad = gas;
        const f32 xCorsia = game.state().playerX;
        if (xCorsia > 120.0f) pad.held |= Pad::Left;
        else if (xCorsia < -120.0f) pad.held |= Pad::Right;
        game.setInput(pad);
        game.update();
        game.render();
        const f32 speed = game.state().playerSpeed;
        if (frame == 0) speedPrimoFrame = speed;
        if (frame == frameDopo1s2 - 1) {
            speedDopo1s2 = speed;
            avanzamentoPrimi1s2 = game.state().playerZ;
        }
        // In curva tenere la strada costa un po' di velocita' (gomme che
        // lavorano, Andrea 11/11 sera): la spinta si controlla in rettilineo.
        const bool inCurva = std::abs(game.road().curvaAt(game.state().playerZ)) > 0.05f;
        if (speed + 0.001f < speedPrecedente && !inCurva) return false;
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
// Nuovo: verifica wrap deterministico su entrambi i sensi.
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
    caricaSponsor(game); caricaRivali(game); game.init(livello);
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
    // 10 secondi di silenzio totale dopo i frame iniziali: show/fumetto deve comparire
    // dopo 500 frame di inattivita' (al frame 560, a 50 Hz = 11.2 s totali, 10 s di idle reali)
    if (frame < 60) pad.held = Pad::A; // input iniziali (arma inattivita), poi silenzio assoluto
    else pad.held = 0;                 // silenzio totale dai frame 60 in poi
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
    caricaSponsor(game); caricaRivali(game); game.init(livello);
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
        // Show senza sprite veri (Andrea 06/11: i rettangoli segnaposto non si
        // disegnano piu'): si misura l'area di bolla e personaggio nella scena.
        (void)d;
        const auto& sc = game.scenaShow();
        i32 blu = sc.bollaVisibile ? sc.bollaW * sc.bollaH : 0;              // fumetto
        i32 verde = sc.personaggioVisibile ? sc.persW * sc.persH : 0;        // personaggio
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
    caricaSponsor(game); caricaRivali(game);
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
        i32 esterni = 0;
        for (i32 i = 0; i < nPx; ++i)
            if (!coloreMotore(d[i])) ++esterni; // colori propri dei cartelloni sponsor
        // Personaggio e fumetto: niente rettangoli colorati a schermo (Andrea
        // 06/11, PR #60): si legge la scena show come nel gate inattivita.
        const auto& sc = game.scenaShow();
        const i32 verdi = sc.personaggioVisibile ? sc.persW * sc.persH : 0;
        const i32 blu = sc.bollaVisibile ? sc.bollaW * sc.bollaH : 0;
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

static bool leggiStringJson(const sgl::JsonObject& obj, const std::string& chiave, std::string& out) {
    sgl::JsonValue v;
    if (!sgl::jsonGet(obj, chiave, v) || !sgl::jsonIsString(v)) return false;
    out = sgl::jsonAsString(v);
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
    // Quali soglie valgono per questa pista (regole.json: voce della pista o "default").
    bool usa_default = (it == regole.end());
    if (usa_default) {
        std::printf("INFO: leggiRegolePercorso usando regole DEFAULT per livello %s\n", livello.c_str());
    } else {
        std::printf("INFO: leggiRegolePercorso usando regole LEVEL-SPECIFIC per livello %s\n", livello.c_str());
    }
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

// --- Definizioni tratto (curva, collina, larghezza, lunghezza) da tratti.json ---
struct TrattoDef {
    f32 curva = 0.0f;
    f32 collina = 0.0f;
    f32 larghezza = 1.0f;
    i32 lunghezza = 1;
};

static std::map<std::string, TrattoDef> leggiDefTratti() {
    std::map<std::string, TrattoDef> out;
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
        TrattoDef d;
        leggiNumeroJson(obj, "curva", d.curva);
        leggiNumeroJson(obj, "collina", d.collina);
        leggiNumeroJson(obj, "larghezza", d.larghezza);
        sgl::JsonValue lv;
        i32 len = 1;
        if (sgl::jsonGet(obj, "lunghezza", lv) && sgl::jsonIsNumber(lv))
            len = (i32)sgl::jsonAsNumber(lv);
        d.lunghezza = len <= 0 ? 1 : len;
        out.emplace(kv.first, d);
    }
    return out;
}

// Indice del segmento che contiene la posizione z (avvolto sul giro).
static i32 segmentoIndiceZ(const Road& road, f32 z) {
    const auto& segs = road.segments();
    const f32 giro = (f32)road.totalLength();
    if (giro <= 0.0f || segs.empty()) return 0;
    z = std::fmod(z, giro);
    if (z < 0.0f) z += giro;
    f32 acc = 0.0f;
    i32 i = 0;
    for (const RoadSegment& seg : segs) {
        if (acc + seg.length > z) return i;
        acc += seg.length;
        ++i;
    }
    return (i32)segs.size() - 1;
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
    f32 centro = 0.0f;    // centro del ramo piu' vicino (0 fuori dal bivio)
    f32 x = 0.0f;         // player.x assoluto
    f32 xRel = 0.0f;      // player.x rispetto al centro della corsia
    f32 curva = 0.0f;     // curva del segmento sotto l'auto
    f32 collina = 0.0f;   // collina del segmento sotto l'auto
    i32 lato = 0;         // latoBivio in questo frame (0 = scelta aperta)
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
    // Corsia di riferimento = ramo piu' vicino, anche a scelta aperta (come
    // l'assistenza al bivio in game.cpp). Prima si usava 0 a scelta aperta:
    // x_rel saltava di BIVIO_DIST al blocco anche se l'immagine e' identica.
    if (c.doppia) {
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
    cp.lato = s.latoBivio;
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

// --- Gate bivio_centro (prova di Andrea 27/10 sera: "l'auto tende a incastrarsi nel mezzo") ---
// Arriva al bivio al centro della strada, poi nella zona del bivio: N) nessuno
// sterzo, S) sinistra tenuta, D) destra tenuta. In tutti i casi l'auto deve
// stare fuori strada al massimo 2 s di fila, senza ribaltarsi e senza perdere
// velocita' (al blocco della scelta >= 80% della massima); con S e D
// deve finire sul ramo chiesto (sterzo tenuto finche' l'auto e' nel ramo).
static bool verificaBivioCentro(const std::string& outDir) {
    std::filesystem::create_directories(outDir);
    struct Caso { const char* nome; u64 sterzo; i32 latoAtteso; };
    const Caso casi[3] = { { "N", 0, 0 }, { "S", Pad::Left, -1 }, { "D", Pad::Right, 1 } };
    bool ok = true;
    for (const Caso& caso : casi) {
        Game game(12345u);
        caricaSponsor(game); caricaRivali(game);
        if (!game.avviaGara()) return false;
        const i32 hz = game.frequenzaHz();
        const Road& road = game.road();
        i32 inMezzo = 0, maxInMezzo = 0;
        bool ribaltato = false, bloccato = false, screenshot = false;
        f32 speedBlocco = 0.0f;
        i32 lato = 0;
        for (i32 frame = 0; frame < 200000; ++frame) {
            const GameState& st = game.state();
            Pad pad{}; pad.held = Pad::A;
            const bool nelBivio = road.bivioAttivo() && st.latoBivio == 0 && st.playerZ > road.bivioInizioZ();
            if (nelBivio) {
                // Sterza verso il ramo chiesto finche' non e' dentro il ramo, poi lascia.
                if (caso.latoAtteso != 0) {
                    const Road::Carreggiate c = road.carreggiate(st.playerZ, 0);
                    const f32 obiettivo = c.centro[caso.latoAtteso < 0 ? 0 : 1];
                    if (std::abs(st.playerX - obiettivo) > 200.0f) pad.held |= caso.sterzo;
                }
            } else if (st.latoBivio == 0) {
                if (st.playerX > 120.0f) pad.held |= Pad::Left;          // arriva al centro
                else if (st.playerX < -120.0f) pad.held |= Pad::Right;
            }
            game.setInput(pad);
            game.update();
            game.render();
            const GameState& dopo = game.state();
            if (dopo.playerFlipping) ribaltato = true;
            if (nelBivio && dopo.latoBivio == 0) {
                inMezzo = dopo.playerOffroad ? inMezzo + 1 : 0; // fuori strada = nello spartitraffico o oltre
                maxInMezzo = std::max(maxInMezzo, inMezzo);
                if (!screenshot && dopo.playerZ > road.bivioInizioZ() + 0.4f * (road.bivioBloccoZ() - road.bivioInizioZ())) {
                    salvaPPM(outDir + "/bivio_centro_" + caso.nome + ".ppm", game.fb().data(),
                             Framebuffer::WIDTH, Framebuffer::HEIGHT);
                    screenshot = true;
                }
            }
            if (dopo.latoBivio != 0) {
                bloccato = true;
                lato = dopo.latoBivio;
                speedBlocco = dopo.playerSpeed;
                break;
            }
        }
        const f32 vMax = Player{}.maxSpeed();
        const bool fuoriPresto = maxInMezzo <= 2 * hz;
        const bool latoGiusto = caso.latoAtteso == 0 || lato == caso.latoAtteso;
        const bool veloce = speedBlocco >= 0.8f * vMax;
        const bool esito = bloccato && fuoriPresto && latoGiusto && veloce && !ribaltato;
        std::printf("bivio_centro %s: blocco=%s lato=%d max fuori strada=%.2f s velocita' al blocco=%.0f/%.0f ribaltato=%s | %s\n",
                    caso.nome, bloccato ? "sì" : "no", lato, (f32)maxInMezzo / (f32)hz, speedBlocco, vMax,
                    ribaltato ? "sì" : "no", esito ? "OK" : "FALLITO");
        ok = ok && esito;
    }
    return ok;
}

// --- Gate traffico (Andrea 27/10: "un poco di traffico comune") ---
// Gara completa (bivi DD) con il pilota automatico di guidaBivio, che schiva
// avversari e traffico. Verifica: il traffico c'e' (almeno 3 veicoli
// superati nella prima tappa), nessun veicolo compare dentro la vista (nasce
// oltre DRAW_DIST), nessuno nasce nella zona del bivio, tutti restano sulla
// carreggiata (anche quando la strada si allarga o si stringe), due auto non
// si compenetrano (avversari e traffico), stesso risultato in due run.
// PERC1 J2 (@developer-junior, dopo D2): sulla tappa 1 (lv_01_neon, che ha
//   sia `autostrada` sia `ponte` nel tracciato) l'autostrada deve avere piu'
//   auto per unita' di strada del ponte. Misura reale: ogni frame conta i
//   veicoli attivi dentro i due tratti (occupazione) e una volta sola somma
//   i segmenti dei due tratti in un ciclo del pattern; densita' =
//   occupazione media / lunghezza del tratto. La lunghezza della tappa e' la
//   stessa per i due tratti, quindi il confronto non dipende dalla scala
//   unita'/km. L'autostrada dichiara `traffico` 1.6, il ponte 0.5: atteso
//   ~3x piu' auto ravvicinate sull'autostrada.
struct TrafficoMisure {
    i32 superatiTappa1 = 0, superati = 0, comparsiInVista = 0, natiNelBivio = 0, fuoriStrada = 0;
    i32 framesCompenetrati = 0, frames = 0, ribaltamenti = 0, urti = 0;
    i32 occAutostrada = 0, occPonte = 0;  // veicoli-osservazioni nei due tratti (tappa 1)
    i32 framesTappa1 = 0;                 // frame campionati in tappa 1 (tutti i veicoli)
    f32 lenAutostradaUnits = 0.0f, lenPonteUnits = 0.0f; // lunghezza dei due tratti (unita')
    u32 hashFinale = 0;
};

static TrafficoMisure misuraTraffico() {
    TrafficoMisure m;
    Game game(12345u);
    caricaSponsor(game); caricaRivali(game);
    if (!game.avviaGara()) return m;
    // PERC1 J2: tratti della tappa 1 (partenza gara = lv_01_neon, come gli
    // altri gate gara: guidaBivio, gara a tappe) e lunghezza dei due tratti
    // in un ciclo del pattern (l'espansione del tracciato ripete il pattern
    // per intero, verificaTratti: n % somma == 0).
    const std::string kLivelloTappa1 = "lv_01_neon";
    const auto nomi = leggiNomiTratti(kLivelloTappa1);
    const auto lunghezze = leggiLunghezzeTratti();
    {
        i32 sommaPat = 0;
        for (const auto& nm : nomi) {
            auto it = lunghezze.find(nm);
            sommaPat += (it != lunghezze.end()) ? it->second : 1;
        }
        const auto& segs = game.road().segments();
        if (sommaPat > 0 && (i32)segs.size() >= sommaPat) {
            for (i32 i = 0; i < sommaPat && i < (i32)segs.size(); ++i) {
                std::string nome; i32 inizio = -1;
                if (!trattoDiSegmento(i, nomi, lunghezze, nome, inizio)) continue;
                if (nome == "autostrada") m.lenAutostradaUnits += (f32)segs[(size_t)i].length;
                else if (nome == "ponte") m.lenPonteUnits += (f32)segs[(size_t)i].length;
            }
        }
    }
    const f32 vista = (f32)(Road::DRAW_DIST * Road::SEG_LEN);
    Game::Veicolo prima[Game::TRAFFICO];
    for (i32 i = 0; i < Game::TRAFFICO; ++i) prima[i] = game.veicolo(i);
    i32 tappaPrima = game.state().tappa;
    f32 zGiocatorePrima = game.state().playerZ;
    bool ribaltato = false, inUrto = false;
    for (i32 frame = 0; frame < 60 * 400 && !game.state().arrivo; ++frame) {
        game.setInput(guidaBivio(game, "DD"));
        game.update();
        game.render();
        ++m.frames;
        const GameState& st = game.state();
        const Road& road = game.road();
        // Il gate misura il traffico comune: un ribaltamento contro le auto di
        // lato dell'incrocio (11/11: incroci anche in lv_01/lv_03/lv_06, il
        // pilota automatico non le guarda) e' l'incidente voluto, non conta.
        if (st.playerFlipping && !ribaltato &&
            std::fabs(road.incrocioD(st.playerZ)) >= Road::INCROCIO_ZONA) ++m.ribaltamenti;
        ribaltato = st.playerFlipping;
        const bool cambioTappa = st.tappa != tappaPrima;
        for (i32 i = 0; i < Game::TRAFFICO; ++i) {
            const Game::Veicolo& v = game.veicolo(i);
            const Game::Veicolo& p = prima[i];
            const bool nato = v.attivo && (!p.attivo || v.z > p.z + 1000.0f || v.z < p.z - 1000.0f);
            // Superato: era davanti all'auto e ora e' dietro (stesso veicolo).
            if (!nato && !cambioTappa && v.attivo && p.attivo &&
                p.z >= zGiocatorePrima && v.z < st.playerZ) {
                if (st.tappa == 1) ++m.superatiTappa1;
                ++m.superati;
            }
            if (nato && !cambioTappa) {
                if (v.z - st.playerZ < vista) ++m.comparsiInVista;
                if (v.z > road.bivioInizioZ() - 1000.0f) ++m.natiNelBivio;
            }
            if (v.attivo && v.z < road.bivioInizioZ()) {
                const f32 meta = Road::ROAD_HALF_WORLD * road.larghezzaAt(v.z);
                if (std::abs(v.x) > meta - 300.0f) {
                    if (m.fuoriStrada < 5)
                        std::printf("  traffico fuori strada: frame %d veicolo %d z=%.0f x=%.0f corsia=%d larghezza=%.2f\n",
                                    frame, i, v.z, v.x, v.corsia, road.larghezzaAt(v.z));
                    ++m.fuoriStrada;
                }
            }
            prima[i] = v;
        }
        tappaPrima = st.tappa;
        zGiocatorePrima = st.playerZ;
        // PERC1 J2: in tappa 1 conta, ogni frame, i veicoli attivi dentro
        // `autostrada` e `ponte` (occupazione media, poi divisa per la
        // lunghezza dei tratti). Al cambio tappa il tracciato cambia: salta.
        if (st.tappa == 1 && !cambioTappa) {
            ++m.framesTappa1;
            for (i32 i = 0; i < Game::TRAFFICO; ++i) {
                const Game::Veicolo& v = game.veicolo(i);
                if (!v.attivo) continue;
                std::string nome; i32 inizio = -1;
                if (trattoDiSegmento(segmentoIndiceZ(road, v.z), nomi, lunghezze, nome, inizio)) {
                    if (nome == "autostrada") ++m.occAutostrada;
                    else if (nome == "ponte") ++m.occPonte;
                }
            }
        }
        // Due auto (avversari o traffico) una dentro l'altra.
        const auto& autoVisibili = game.traffico();
        bool compenetrati = false;
        for (size_t a = 0; a < autoVisibili.size() && !compenetrati; ++a)
            for (size_t b = a + 1; b < autoVisibili.size(); ++b)
                if (std::abs(autoVisibili[a].worldZ - autoVisibili[b].worldZ) < 100.0f &&
                    std::abs(autoVisibili[a].worldX - autoVisibili[b].worldX) < 150.0f) { compenetrati = true; break; }
        if (compenetrati) ++m.framesCompenetrati;
        // Urti del giocatore (bump): il pilota schiva, devono essere rari.
        const bool urtoOra = game.collisioneConAuto();
        if (urtoOra && !inUrto) ++m.urti;
        inUrto = urtoOra;
    }
    m.hashFinale = game.frameHash();
    return m;
}

static bool verificaTraffico() {
    const TrafficoMisure a = misuraTraffico();
    const TrafficoMisure b = misuraTraffico();
    const bool deterministico = a.hashFinale == b.hashFinale && a.superatiTappa1 == b.superatiTappa1 &&
                                a.occAutostrada == b.occAutostrada && a.occPonte == b.occPonte;
    // PERC1 J2: densita' auto nei due tratti (auto per 1000 unita' di strada),
    // occupazione media in tappa 1 divisa dalla lunghezza del tratto.
    const f32 densAutostrada = (a.lenAutostradaUnits > 0.0f && a.framesTappa1 > 0)
        ? 1000.0f * (f32)a.occAutostrada / ((f32)a.framesTappa1 * a.lenAutostradaUnits) : 0.0f;
    const f32 densPonte = (a.lenPonteUnits > 0.0f && a.framesTappa1 > 0)
        ? 1000.0f * (f32)a.occPonte / ((f32)a.framesTappa1 * a.lenPonteUnits) : 0.0f;
    const bool campioneOk = a.framesTappa1 >= 1200 && a.lenAutostradaUnits > 0.0f && a.lenPonteUnits > 0.0f;
    const bool densitaOk = campioneOk && densAutostrada > densPonte;
    const bool esito = a.superatiTappa1 >= 3 && a.comparsiInVista == 0 && a.natiNelBivio == 0 &&
                       a.fuoriStrada == 0 && a.framesCompenetrati <= a.frames / 100 &&
                       a.ribaltamenti == 0 && deterministico && densitaOk;
    std::printf("traffico: superati in tappa 1=%d (in tutto %d), comparsi in vista=%d, nati nel bivio=%d, fuori strada=%d, "
                "frame con auto compenetrate=%d/%d, urti del giocatore=%d, ribaltamenti=%d, deterministico=%s, "
                "auto/1000u autostrada=%.4f ponte=%.4f (occ=%d/%d su %d frame, tratti %.0f/%.0f u) | %s\n",
                a.superatiTappa1, a.superati, a.comparsiInVista, a.natiNelBivio, a.fuoriStrada, a.framesCompenetrati, a.frames,
                a.urti, a.ribaltamenti, deterministico ? "sì" : "no",
                densAutostrada, densPonte, a.occAutostrada, a.occPonte, a.framesTappa1,
                a.lenAutostradaUnits, a.lenPonteUnits, esito ? "OK" : "FALLITO");
    return esito;
}

// --- Gate corsie (Andrea 27/10: "la strada che si allarga a piu' corsie") ---
// Per ogni livello: c'e' un tratto a 4 corsie (larghezza 2), la larghezza
// cambia con dolcezza (raccordo), parte da 1 e vale 1 in tutta la zona del
// bivio. Poi la guida: nella gara il pilota va nella corsia esterna destra
// (centro della corsia a x 1800) dove la strada e' larga e torna al centro prima che si stringa;
// sulla strada larga l'auto non e' mai fuori strada e non si ribalta.
static bool verificaCorsie() {
    static const char* kLivelli[6] = {
        "lv_01_neon", "lv_02_atomico", "lv_03_foglie",
        "lv_04_cromo", "lv_05_spray", "lv_06_desktop"};
    bool ok = true;
    for (const char* livello : kLivelli) {
        Game game(12345u);
        game.init(livello);
        const auto& segs = game.road().segments();
        const i32 n = (i32)segs.size();
        f32 massima = 0.0f, saltoMax = 0.0f, bivioMax = 0.0f;
        for (i32 i = 0; i < n; ++i) {
            massima = std::max(massima, segs[i].larghezza);
            if (i > 0) saltoMax = std::max(saltoMax, std::abs(segs[i].larghezza - segs[i - 1].larghezza));
            if (i >= n - Road::BIVIO_SEG) bivioMax = std::max(bivioMax, segs[i].larghezza);
        }
        const bool esito = n > 0 && massima >= 1.99f && saltoMax <= 0.02f && bivioMax <= 1.0f &&
                           segs.front().larghezza <= 1.0f;
        std::printf("corsie %s: larghezza massima %.2f, salto massimo per segmento %.3f, al bivio %.2f | %s\n",
                    livello, massima, saltoMax, bivioMax, esito ? "OK" : "FALLITO");
        ok = ok && esito;
    }
    // Limiti del giocatore: corsia esterna dentro la strada larga, fuori strada su quella normale.
    {
        Player p;
        p.larghezzaCorsia = 2.0f; p.x = 1800.0f; p.vincolaAllaCorsia();
        const bool largaOk = !p.offroad && p.x == 1800.0f;
        Player q;
        q.larghezzaCorsia = 1.0f; q.x = 1800.0f; q.vincolaAllaCorsia();
        const bool normaleOk = q.offroad && q.x <= Player::X_LIMIT;
        std::printf("corsie limiti: x 1800 su strada larga %s, su strada normale %s | %s\n",
                    largaOk ? "in strada" : "FUORI", normaleOk ? "fuori strada e trattenuta" : "NON trattenuta",
                    (largaOk && normaleOk) ? "OK" : "FALLITO");
        ok = ok && largaOk && normaleOk;
    }
    {
        Game game(12345u);
        caricaSponsor(game); caricaRivali(game);
        if (!game.avviaGara()) return false;
        const Road& road = game.road();
        f32 xMax = 0.0f;
        i32 fuoriLarga = 0, ribaltamenti = 0;
        bool ribaltato = false;
        for (i32 frame = 0; frame < 60 * 40 && game.state().tappa == 1; ++frame) {
            const GameState& st = game.state();
            // Corsia esterna se la strada resta larga per almeno 1 s, altrimenti al centro.
            const f32 avanti = st.playerZ + 30000.0f;
            const bool larga = road.larghezzaAt(st.playerZ) >= 1.95f && road.larghezzaAt(avanti) >= 1.95f;
            const f32 obiettivo = larga ? 1650.0f : 0.0f; // corsia esterna (centro 1800), margine per la spinta in curva
            Pad pad{}; pad.held = Pad::A;
            if (st.playerX > obiettivo + 120.0f) pad.held |= (u64)Pad::Left;
            else if (st.playerX < obiettivo - 120.0f) pad.held |= (u64)Pad::Right;
            game.setInput(pad);
            game.update();
            game.render();
            const GameState& dopo = game.state();
            if (dopo.playerFlipping && !ribaltato) ++ribaltamenti;
            ribaltato = dopo.playerFlipping;
            if (road.larghezzaAt(dopo.playerZ) >= 1.95f) {
                xMax = std::max(xMax, dopo.playerX);
                if (dopo.playerOffroad) ++fuoriLarga;
            }
        }
        // Corsia esterna: oltre la striscia a 1200 con tutta l'auto (meta' ~135).
        const bool esito = xMax >= 1400.0f && fuoriLarga == 0 && ribaltamenti == 0;
        std::printf("corsie guida: x massima sulla strada larga %.0f, frame fuori strada sulla strada larga %d, ribaltamenti %d | %s\n",
                    xMax, fuoriLarga, ribaltamenti, esito ? "OK" : "FALLITO");
        ok = ok && esito;
    }
    return ok;
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
            caricaSponsor(game); caricaRivali(game);
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
            caricaSponsor(game); caricaRivali(game);
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
            // Al blocco della scelta game.cpp ricentra le coordinate sul ramo
            // scelto (x -= lato*BIVIO_DIST): stesso punto, sistema spostato.
            // Il centro si confronta nel sistema del frame precedente.
            f32 riferimento = 0.0f;
            if (campioni[i - 1].lato == 0 && campioni[i].lato != 0)
                riferimento = (f32)campioni[i].lato * Road::BIVIO_DIST;
            const f32 dc = std::abs(campioni[i].centro + riferimento - campioni[i - 1].centro);
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

// --- Gate FORK1 punto 7: transizione graduale di tema al cambio tappa ---
// Alla fineTappa il Renderer deve interpolare TemaColori dal tema della tappa
// che finisce al tema della tappa nuova in transizione_tema_s secondi
// (dati/config.json, default 1.0 s), NON in un singolo frame.
// Guida la gara su lv_01_neon fino al primo cambio tappa e campiona il
// colore del cielo (framebuffer riga 2 < HORIZON_Y 112, colonna centrale: sopra l'HUD)
// a ogni frame. Verifica: cambio raggiunto, variazione per canale tra due
// frame consecutivi <= soglia (graduale, niente scatto), cielo finale
// diverso da quello iniziale, transizione completata entro transizione_tema_s
// con tolleranza e in piu' di un frame. Soglie da dati/tracciati/regole.json
// ("tema_transizione"); durata e frequenza da dati/config.json.
struct TemaRegole {
    f32 delta_canale_max_per_frame = 12.0f;
};

static TemaRegole leggiRegoleTemaTransizione() {
    TemaRegole r;
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
    sgl::JsonValue temaVal;
    if (!sgl::jsonGet(root, "tema_transizione", temaVal) || !sgl::jsonIsObject(temaVal)) return r;
    const auto& t = sgl::jsonAsObject(temaVal);
    leggiNumeroJson(t, "delta_canale_max_per_frame", r.delta_canale_max_per_frame);
    return r;
}

// Legge frequenza_hz e transizione_tema_s da dati/config.json.
// Ritorna durata (secondi) e frequenza (Hz): default 1.0 s e 60 Hz.
static void leggiTransizioneDaConfig(f32& durataS, i32& hz) {
    durataS = 1.0f;
    hz = 60;
    std::ifstream f("motore_nuovo/dati/config.json");
    if (!f.is_open()) return;
    std::stringstream ss;
    ss << f.rdbuf();
    f.close();
    sgl::JsonReader reader(ss.str());
    if (!reader.parse()) return;
    const auto& val = reader.valore();
    if (!sgl::jsonIsObject(val)) return;
    const auto& root = sgl::jsonAsObject(val);
    sgl::JsonValue v;
    if (sgl::jsonGet(root, "frequenza_hz", v) && sgl::jsonIsNumber(v)) {
        i32 vhz = (i32)sgl::jsonAsNumber(v);
        if (vhz > 0) hz = vhz;
    }
    if (sgl::jsonGet(root, "transizione_tema_s", v) && sgl::jsonIsNumber(v)) {
        f32 vs = (f32)sgl::jsonAsNumber(v);
        if (vs > 0.0f) durataS = vs;
    }
}

static u32 cieloPixel(Game& game) {
    const u32* d = game.fb().data();
    return d[2 * Framebuffer::WIDTH + Framebuffer::WIDTH / 2]; // riga 2: cielo sopra l'HUD
}

static i32 deltaCanaleMax(u32 a, u32 b) {
    i32 da = std::abs((i32)(((a >> 16) & 0xFF) - ((b >> 16) & 0xFF)));
    i32 dg = std::abs((i32)(((a >> 8) & 0xFF) - ((b >> 8) & 0xFF)));
    i32 db = std::abs((i32)((a & 0xFF) - (b & 0xFF)));
    return std::max(da, std::max(dg, db));
}

bool verificaTemaTransizione(const std::string& outDir) {
    const TemaRegole r = leggiRegoleTemaTransizione();
    f32 durataS = 0.0f;
    i32 hz = 0;
    leggiTransizioneDaConfig(durataS, hz);
    const f32 frameAttesi = durataS * (f32)hz;
    const i32 FRAME_MAX = 40000;   // tetto di sicurezza: molto oltre un giro
    const i32 FRAME_STABILI = 30;  // tema nuovo considerato raggiunto dopo N frame uguali
    const i32 RING = 16;           // ring buffer fotogrammi attorno al cambio

    Game game(12345u);
    caricaSponsor(game); caricaRivali(game);
    game.init("lv_01_neon"); // tema default (nessun "colori": cielo 0x202040)
    if (!game.avviaGara()) {
        std::printf("tema_transizione: FALLITO (gara non avviata)\n");
        return false;
    }
    const std::string tappaInizioId = game.tappaCorrente();
    const i32 tappaInizio = game.state().tappa;

    bool cambioAvvenuto = false;
    i32 cambioFrame = -1, frameFine = -1;
    u32 cieloInizio = 0, cieloFine = 0;
    f32 maxDelta = 0.0f;
    i32 stabili = 0;
    u32 cieloPrec = 0;
    bool ok = true;
    std::vector<std::vector<u32>> ring(RING);
    std::string tappaDopoId;

    for (i32 frame = 0; frame < FRAME_MAX && !game.state().arrivo; ++frame) {
        Pad pad{}; pad.held = Pad::A;
        const GameState& s = game.state();
        const Road& road = game.road();
        f32 obiettivo = 0.0f;
        if (road.bivioAttivo() && s.latoBivio == 0 && s.playerZ > road.bivioInizioZ())
            obiettivo = -(f32)Road::BIVIO_DIST; // a sinistra: lv_02_atomico (cielo diverso)
        if (s.playerX > obiettivo + 120.0f) pad.held |= (u64)Pad::Left;
        else if (s.playerX < obiettivo - 120.0f) pad.held |= (u64)Pad::Right;
        game.setInput(pad);
        game.update();
        game.render();
        const u32 cielo = cieloPixel(game);
        ring[(size_t)(frame & (RING - 1))].assign(game.fb().data(),
            game.fb().data() + (size_t)Framebuffer::WIDTH * Framebuffer::HEIGHT);

        if (!cambioAvvenuto) {
            if (s.tappa != tappaInizio) {
                cambioAvvenuto = true;
                cambioFrame = frame;
                cieloInizio = cieloPrec;   // al cambio il tema e' ancora quello vecchio
                tappaDopoId = game.tappaCorrente();
                // screenshot: 2 frame prima, 1 frame prima, cambio
                if (cambioFrame >= 2)
                    salvaPPM(outDir + "/p_tema_transizione_prima2.ppm",
                             ring[(size_t)((cambioFrame - 2) & (RING - 1))].data(),
                             Framebuffer::WIDTH, Framebuffer::HEIGHT);
                if (cambioFrame >= 1)
                    salvaPPM(outDir + "/p_tema_transizione_prima1.ppm",
                             ring[(size_t)((cambioFrame - 1) & (RING - 1))].data(),
                             Framebuffer::WIDTH, Framebuffer::HEIGHT);
                salvaPPM(outDir + "/p_tema_transizione_cambio.ppm",
                         ring[(size_t)(cambioFrame & (RING - 1))].data(),
                         Framebuffer::WIDTH, Framebuffer::HEIGHT);
                // primo frame della tappa nuova: il cielo non deve aver giA' saltato al nuovo tema
                if (deltaCanaleMax(cielo, cieloInizio) > r.delta_canale_max_per_frame) {
                    ok = false;
                    std::printf("  VIOLAZIONE: scatto di tema al cambio tappa (frame %d, delta %d > soglia %.0f)\n",
                                frame, deltaCanaleMax(cielo, cieloInizio), r.delta_canale_max_per_frame);
                }
            } else {
                cieloPrec = cielo;
            }
        } else {
            const i32 d = deltaCanaleMax(cielo, cieloPrec);
            if ((f32)d > maxDelta) maxDelta = (f32)d;
            if (frame == cambioFrame + 1 || frame == cambioFrame + 2 || frame == cambioFrame + 10)
                salvaPPM(outDir + "/p_tema_transizione_dopo" + std::to_string(frame - cambioFrame) + ".ppm",
                         game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
            if (cielo == cieloPrec) {
                if (++stabili >= FRAME_STABILI) {
                    cieloFine = cielo;
                    frameFine = frame;
                    break;
                }
            } else {
                stabili = 0;
                cieloPrec = cielo;
            }
        }
    }

    const i32 transFrame = (frameFine >= 0 && cambioFrame >= 0) ? frameFine - cambioFrame : -1;
    const f32 transS = transFrame > 0 ? (f32)transFrame / (f32)hz : -1.0f;
    const i32 frameLimite = (i32)(frameAttesi * 2.0f) + 10;
    salvaPPM(outDir + "/p_tema_transizione_fine.ppm", game.fb().data(),
             Framebuffer::WIDTH, Framebuffer::HEIGHT);

    if (!cambioAvvenuto) {
        std::printf("  FALLITO: cambio tappa non raggiunto entro %d frame\n", FRAME_MAX);
        ok = false;
    }
    if (cambioAvvenuto) {
        if (cieloFine == cieloInizio) {
            std::printf("  FALLITO: nessun cambio di tema osservato (%s -> %s)\n",
                        tappaInizioId.c_str(), tappaDopoId.c_str());
            ok = false;
        }
        if (maxDelta > r.delta_canale_max_per_frame) {
            std::printf("  FALLITO: tema non graduale (maxDeltaCanale=%.1f > soglia %.0f)\n",
                        maxDelta, r.delta_canale_max_per_frame);
            ok = false;
        }
        if (transFrame <= 1) {
            std::printf("  FALLITO: transizione istantanea (1 frame)\n");
            ok = false;
        }
        if (transFrame > frameLimite) {
            std::printf("  FALLITO: transizione troppo lunga (%d frame > %d)\n",
                        transFrame, frameLimite);
            ok = false;
        }
        std::printf("tema_transizione %s -> %s: cambio frame %d | cielo 0x%06X -> 0x%06X | "
                    "maxDeltaCanale=%.1f (soglia %.0f) | transizione %d frame (%.2f s, attesi ~%.1f, limite %d) | %s\n",
                    tappaInizioId.c_str(), tappaDopoId.c_str(), cambioFrame,
                    cieloInizio & 0xFFFFFF, cieloFine & 0xFFFFFF, maxDelta,
                    r.delta_canale_max_per_frame, transFrame, transS, frameAttesi, frameLimite,
                    ok ? "OK" : "FALLITO");
    }
    std::printf("%s\n", ok ? "GATE TEMA TRANSIZIONE OK" : "FALLITO: tema transizione non graduale");
    return ok;
}

// --- Gate FORK1 punto 5: bordo strada attraverso il bivio sul livello piu' curvo ---
// Guida scriptata (gas pieno, sterzo verso il ramo scelto) che attraversa due bivi:
// lv_01_neon -> lv_03_foglie (il piu' curvo: curva max 1.02) -> cambio tappa.
// Verifica: arrivo al cambio tappa, nessun ribaltamento (show di crash mai attivo)
// e offroad consecutivi <= N (regole.json "bivio_bordi", N=60 frame = 1 s @ 60 Hz;
// il ribaltamento del motore scatta a OFFROAD_CRASH_S=2.0 s = 120 frame @ 60 Hz,
// soglia di allarme a meta' per individuare l'auto intrappolata prima del ribaltamento).
// Sterzo normale verso il ramo scelto sia dentro che fuori dal bivio; il ramo non
// scelto non deve spingere l'auto. Salva screenshot PPM attorno al cambio tappa.
struct BordoRegole {
    i32 offroadMaxConsecutivi = 60;
};

static BordoRegole leggiRegoleBordoBivio() {
    BordoRegole r;
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
    sgl::JsonValue bVal;
    if (!sgl::jsonGet(root, "bivio_bordi", bVal) || !sgl::jsonIsObject(bVal)) return r;
    const auto& b = sgl::jsonAsObject(bVal);
    leggiIntJson(b, "offroad_max_consecutivi", r.offroadMaxConsecutivi);
    return r;
}

static const char* kScelteBordi[] = { "DS", "DD" }; // lv_01 -> destra (lv_03, piu' curvo), poi uscita sinistra/destra

static Pad guidaVersoRamo(const Game& game, const char* scelte, i32& sceltaIdx, bool& sceltaPresa) {
    Pad pad{}; pad.held = Pad::A;
    const GameState& s = game.state();
    const Road& road = game.road();
    f32 obiettivo = 0.0f;
    if (road.bivioAttivo()) {
        // In gara ogni tappa ha il suo bivio: prima della zona bivio si
        // riparte da zero (prima la seconda scelta dello script era ignorata).
        if (s.latoBivio == 0 && s.playerZ <= road.bivioInizioZ()) sceltaPresa = false;
        if (!sceltaPresa && s.latoBivio == 0 && s.playerZ > road.bivioInizioZ() && sceltaIdx < (i32)std::strlen(scelte)) {
            // Scelta al primo ingresso nel bivio: ramo dello script.
            sceltaPresa = true;
            sceltaIdx++;
        }
        // Obiettivo = centro del ramo scelto nelle coordinate del motore: a
        // scelta aperta e' +-d (si apre fino a BIVIO_DIST); dopo il blocco
        // game.cpp ricentra le coordinate e il ramo scelto e' centro[0] (~0).
        // (Prima: obiettivo 0 a scelta aperta dopo il primo frame, cioe' lo
        // spartitraffico, e lato*BIVIO_DIST dopo il blocco, cioe' fuori strada.)
        const Road::Carreggiate c = road.carreggiate(s.playerZ, s.latoBivio);
        if (c.doppia && sceltaPresa && sceltaIdx > 0) {
            const i32 lato = (scelte[sceltaIdx - 1] == 'S') ? -1 : +1;
            if (s.latoBivio != 0) obiettivo = c.centro[0];
            else obiettivo = lato < 0 ? c.centro[0] : c.centro[1];
        }
    } else {
        sceltaPresa = false; // nuova tappa: nuova scelta al prossimo bivio
    }
    if (s.playerX > obiettivo + 120.0f) pad.held |= (u64)Pad::Left;
    else if (s.playerX < obiettivo - 120.0f) pad.held |= (u64)Pad::Right;
    return pad;
}

// MIG1 J2 (09/11): prova paletti abbattuti — restano dentro lo schermo e
// sopra la riga delle ruote (renderer clamp baseY <= 208), con screenshot.
bool verificaPali(const std::string& outDir, const std::string& livello) {
    namespace fs = std::filesystem;
    const std::string dir = outDir + "/mig1_j2_pali";
    fs::create_directories(dir);
    const std::string dirRel = "UFFICIO/Officina/Test area/mig1_j2_pali";
    (void)dirRel;
    Game game(12345u);
    caricaSponsor(game);
    caricaRivali(game);
    game.init(livello); // init non restituisce nulla

    const i32 TOTAL = 2400; // ~40s @60Hz
    i32 hitCount = 0;
    f32 speedPrec = 0.0f;
    bool primoHit = false;
    i32 framePrimoHit = -1;
    i32 screenshotCount = 0;
    for (i32 f = 0; f < TOTAL; ++f) {
        Pad pad;
        pad.held = (u64)Pad::A;       // accelera
        const auto& stBefore = game.state();
        // Sterza a destra ma controllato per restare sulla strada e avvicinarsi ai paletti
        if (stBefore.playerOffroad) {
            pad.held |= (u64)Pad::Left; // cerca di rientrare
        } else if (stBefore.playerX < 400.0f) {
            pad.held |= (u64)Pad::Right;
        } else if (stBefore.playerX > 700.0f) {
            pad.held &= ~(u64)Pad::Right;
        } else {
            pad.held |= (u64)Pad::Right;
        }
        game.setInput(pad);
        const f32 spPrima = game.state().playerSpeed;
        game.update();
        game.render();
        const f32 spDopo = game.state().playerSpeed;
        const auto& st = game.state();
        if (f % 300 == 0) {
            std::printf("  dbg f=%d sp=%.1f x=%.1f flip=%d offroad=%d\n", f, spDopo, st.playerX, st.playerFlipping?1:0, st.playerOffroad?1:0);
        }
        // Conteggio più robusto: drop 3% 
        if (f > 60 && spPrima > 150.0f && !game.state().playerFlipping) {
            const f32 ratio = spDopo / std::max(spPrima, 1.0f);
            if (ratio >= 0.965f && ratio <= 0.975f) {
                ++hitCount;
                if (!primoHit) { primoHit = true; framePrimoHit = f; std::printf("  HIT at f=%d ratio=%.4f\n", f, ratio); }
            }
        }
        speedPrec = spDopo;
        // Screenshot: al primo hit e in momenti successivi (volo/atterraggio)
        if (primoHit && screenshotCount < 4) {
            const i32 delta = f - framePrimoHit;
            if (delta == 2 || delta == 20 || delta == 50 || delta == 120) {
                const std::string path = dir + "/pali_f" + std::to_string(f) + ".ppm";
                salvaPPM(path, game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
                ++screenshotCount;
            }
        }
        if (f == 300 || f == 900 || f == 1700) {
            const std::string path = dir + "/pali_f" + std::to_string(f) + ".ppm";
            salvaPPM(path, game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
        }
    }
    // Evidenza codice (MIG1 D1/J2): renderer drawDetriti clampa baseY <= 208
    // (BASE_RIGA_RUOTE, renderer.cpp) => mai sotto la riga delle ruote; proiezione
    // usa projs_ visibili => dentro area di gioco/schermo. Screenshot in dir.
    std::printf("pali: hit_rilevati=%d frame_primo_hit=%d screenshot=%d dir=%s\n",
                hitCount, framePrimoHit, screenshotCount, dir.c_str());
    const bool ok = hitCount > 0 && screenshotCount >= 2;
    if (!ok) std::printf("  NOTA: hit_rilevati=%d (serve >0), screenshot=%d (serve >=2)\n", hitCount, screenshotCount);
    std::printf("  verifica: drawDetriti clampa baseY <= 208 (riga ruote) e usa proiezione visibile (code review D1)\n");
    return ok;
}

// MIG1 J3 (09/11, rivisto 09/11 12:2x): sportellata efficace (D2) - bot
// affiancato a un RIVALE alla punta; misura i secondi al primo colpo a segno e
// la ripresa del rivale (~5 s, D2).
// Nota API: sbandataS e' privato (Game::rivali_ privato, test fuori dal src),
// quindi la ripresa si misura dall'esterno: si segue lo sprite del rivale
// colpito e si campiona la sua velocita' reale (delta worldZ / dt) al secondo.
// sbandata => velocita' giu' subito dopo il colpo, recupero entro ~5 s.
// I rivali sono distinguibili dal traffico per la scala (1.7 contro 1.4).
bool verificaSportellata(const std::string& outDir, const std::string& livello) {
    namespace fs = std::filesystem;
    const std::string dir = outDir + "/mig1_j3_sportellata";
    fs::create_directories(dir);
    Game game(12345u);
    caricaSponsor(game);
    caricaRivali(game);
    game.abilitaPartenza(false);
    if (!game.avviaGara("motore_nuovo/dati/percorso_gara.json", true)) {
        std::printf("FALLITO: avviaGara livello %s\n", livello.c_str());
        return false;
    }
    const f32 dt = 1.0f / (f32)::leggiFrequenzaDaConfig();
    const f32 giro = (f32)game.road().totalLength();
    auto avanza = [&](f32 zA, f32 zB) -> f32 {
        f32 d = zB - zA;
        if (d < -giro / 2) d += giro;
        if (d > giro / 2) d -= giro;
        return d;
    };
    // Solo rivali: gli sprite del traffico hanno scala 1.4, i rivali 1.7.
    auto eRivale = [](const SpriteInstance& s) { return s.scale > 1.6f; };
    // Rivale piu' vicino a zAtt (con tolleranza) e in corsia vicino a xAtt.
    auto trovaRivale = [&](f32 zAtt, f32 xAtt, f32 toll) -> const SpriteInstance* {
        const SpriteInstance* best = nullptr;
        f32 bestScore = 1e9f;
        for (const SpriteInstance& sp : game.traffico()) {
            if (!eRivale(sp)) continue;
            const f32 dz = std::abs(avanza(zAtt, sp.worldZ));
            if (dz > toll) continue;
            const f32 dx = std::abs(sp.worldX - xAtt);
            if (dx > 600.0f) continue;
            const f32 score = dz + 2.0f * dx;
            if (score < bestScore) { bestScore = score; best = &sp; }
        }
        return best;
    };
    const i32 MAX_F = (i32)(30.0f / dt); // cerca contatto entro 30 s
    const i32 OSS_F = (i32)(8.0f / dt);  // osserva ripresa per 8 s dopo il colpo
    i32 urtiPrima = game.urtiGara();
    i32 frameColpo = -1;
    f32 zBestPrec = -1.0f;
    f32 xBestPrec = 0.0f;
    f32 velFrame = 0.0f;
    i32 rivaliVisibiliMax = 0;
    f32 minDzRivale = 1e9f; // diagnostica: quanto si avvicina il rivale piu' vicino davanti
    // Fase 1: accelera e cerca di affiancarsi a un rivale davanti, poi colpo
    // di sterzo rapido verso di lui (D2: dSterzo > 0.22, affiancata |dx|<650).
    i32 flickDir = 0; // 0 neutro, -1 sx, +1 dx
    i32 flickTimer = 0;
    i32 screenN = 0;
    i32 diagUrti = 0;
    i32 rivaleColpito = -1;
    for (i32 f = 0; f < MAX_F && frameColpo < 0; ++f) {
        const GameState& s = game.state();
        Pad pad;
        pad.held = (u64)Pad::A;
        // Rivale piu' vicino davanti, in finestra utile (mai il traffico).
        f32 bestDz = 1e9f;
        f32 bestDx = 0.0f;
        f32 bestZ = 0.0f;
        f32 bestX = 0.0f;
        i32 rivaliOra = 0;
        for (const SpriteInstance& sp : game.traffico()) {
            if (!eRivale(sp)) continue;
            ++rivaliOra;
            f32 dz = std::fmod(sp.worldZ - s.playerZ, giro);
            if (dz < 0.0f) dz += giro;
            if (dz < minDzRivale) minDzRivale = dz;
            if (dz < 200.0f || dz > 2600.0f) continue;
            if (dz < bestDz) { bestDz = dz; bestDx = sp.worldX - s.playerX; bestZ = sp.worldZ; bestX = sp.worldX; }
        }
        rivaliVisibiliMax = std::max(rivaliVisibiliMax, rivaliOra);
        // Traffico piu' vicino davanti: se e' piu' vicino del rivale va schivato,
        // altrimenti il bot lo tampona e perde velocita' (stessa idea di corriGara).
        f32 trafDz = 1e9f;
        f32 trafObiettivo = 0.0f;
        for (const SpriteInstance& t : game.traffico()) {
            if (eRivale(t)) continue;
            const f32 dz = t.worldZ - s.playerZ;
            if (dz <= 0.0f || dz > 4000.0f) continue;
            if (std::abs(t.worldX - s.playerX) >= 500.0f) continue;
            if (dz < trafDz) {
                trafDz = dz;
                trafObiettivo = t.worldX + (t.worldX > s.playerX ? -650.0f : 650.0f);
            }
        }
        const bool schivaTraffico = trafDz < 1e8f && (bestDz > 1e8f || trafDz < bestDz - 100.0f);
        if (schivaTraffico) {
            // Evita il traffico: niente inseguimento del rivale attraverso un'auto.
            zBestPrec = -1.0f;
            velFrame = 0.0f;
            flickTimer = 0;
            if (s.playerX > trafObiettivo + 120.0f) pad.held |= (u64)Pad::Left;
            else if (s.playerX < trafObiettivo - 120.0f) pad.held |= (u64)Pad::Right;
        } else if (bestDz < 1e8f) {
            // Continuita' del bersaglio: velocita' vera (delta z / dt).
            if (zBestPrec >= 0.0f) {
                const f32 d = avanza(zBestPrec, bestZ);
                if (std::abs(d) < 600.0f) velFrame = d / dt;
            }
            zBestPrec = bestZ;
            xBestPrec = bestX;
            // Resta affiancato al rivale ma a distanza di sicurezza (fuori dalla
            // carrozzeria); quando il rivale entra nella finestra d'urto, colpo
            // di sterzo rapido verso di lui (D2: dSterzo > 0.22, |dx| < 240).
            const f32 versoLato = bestDx >= 0.0f ? 1.0f : -1.0f;
            const f32 obiettivoX = bestX - versoLato * 280.0f;
            const bool inFinestra = bestDz > Game::URTO_Z_MIN && bestDz < 1150.0f;
            const bool aFianco = std::abs(bestDx) < 500.0f;
            if (inFinestra && aFianco) {
                // Affiancati: sterzo pieno verso il rivale finche' la
                // carrozzeria lo tocca di lato (D2, anche alla punta).
                flickDir = bestDx > 0.0f ? 1 : -1;
                pad.held |= flickDir < 0 ? (u64)Pad::Left : (u64)Pad::Right;
            } else {
                flickTimer = 0;
                if (s.playerX > obiettivoX + 30.0f) pad.held |= (u64)Pad::Left;
                else if (s.playerX < obiettivoX - 30.0f) pad.held |= (u64)Pad::Right;
            }
        } else {
            zBestPrec = -1.0f;
            velFrame = 0.0f;
            flickTimer = 0;
        }
        game.setInput(pad);
        game.update();
        game.render();
        if (game.urtiGara() > urtiPrima) {
            // Sportellata a segno = un rivale ha appena iniziato la sbandata
            // (D2: sbandataS = 5 s). Un urto col traffico non conta.
            for (i32 ri = 0; ri < Game::numeroRivali(); ++ri)
                if (game.rivaleSbandataS(ri) > 4.9f) rivaleColpito = ri;
            if (rivaleColpito >= 0) {
                frameColpo = f;
            } else if (diagUrti < 3) {
                std::printf("  [diag] urto f=%d senza rivale in sbandata (traffico?) speed=%.2f\n", f, s.playerSpeed);
                ++diagUrti;
            }
            urtiPrima = game.urtiGara();
        }
        if (screenN < 3 && (f == 60 || f == 180 || f == 360)) {
            salvaPPM(dir + "/sportellata_f" + std::to_string(f) + ".ppm",
                     game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
            ++screenN;
        }
    }
    if (frameColpo < 0) {
        std::printf("sportellata: FALLITO (nessuna sportellata a un rivale entro %.1fs, urti=%d)\n",
                    MAX_F * dt, game.urtiGara());
        std::printf("  diagnostica: rivali_visibili_max=%d min_dz_rivale=%.0f\n",
                    rivaliVisibiliMax, minDzRivale);
        std::printf("  dir=%s\n", dir.c_str());
        return false;
    }
    const f32 tColpo = frameColpo * dt;
    salvaPPM(dir + "/sportellata_colpo.ppm", game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
    // Fase 2: segue la sbandata del rivale colpito per 8 s. Attesa (D2):
    // ancora in sbandata a 2 s, tornato normale entro ~5 s (piu' un margine
    // se nel frattempo lo si colpisce di nuovo).
    f32 sbandata2s = -1.0f;
    f32 tRipresa = -1.0f;
    for (i32 k = 0; k < OSS_F; ++k) {
        Pad pad;
        pad.held = (u64)Pad::A; // gas, niente sterzo: nessun nuovo colpo voluto
        game.setInput(pad);
        game.update();
        game.render();
        const f32 sb = game.rivaleSbandataS(rivaleColpito);
        if (k == (i32)(2.0f / dt)) {
            sbandata2s = sb;
            salvaPPM(dir + "/sportellata_dopo2s.ppm", game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
        }
        if (tRipresa < 0.0f && sb <= 0.0f) tRipresa = (f32)(k + 1) * dt;
        if (k == OSS_F - 1)
            salvaPPM(dir + "/sportellata_dopo8s.ppm", game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
    }
    const bool sbandataOsservata = sbandata2s > 0.0f;
    const bool ripresaOsservata = tRipresa > 4.0f && tRipresa < 6.0f;
    std::printf("sportellata (MIG1 J3): colpo_a_segno=SI a %.2fs (frame %d) rivale=%d urti_gara=%d\n",
                tColpo, frameColpo, rivaleColpito, game.urtiGara());
    std::printf("  sbandata a 2 s=%.2f s rimasti, ripresa dopo %.2f s (attesa ~5 s)\n", sbandata2s, tRipresa);
    std::printf("  dir=%s\n", dir.c_str());
    const bool ok = sbandataOsservata && ripresaOsservata;
    return ok;
}

bool verificaBivioBordi(const std::string& outDir) {
    const BordoRegole r = leggiRegoleBordoBivio();
    const i32 N = r.offroadMaxConsecutivi > 0 ? r.offroadMaxConsecutivi : 60;
    const i32 RING = 120;
    bool ok = true;
    for (const char* scelte : kScelteBordi) {
        i32 cambi = 0, sceltaIdx = 0, offRun = 0, maxOffRun = 0, cambioFrame = -1, stopRun = 0;
        i32 stopRunPre = 0;
        f32 maxXRel = 0.0f;
        i32 maxXRelFrame = -1;
        bool crash = false, crashPre = false, sceltaPresa = false, inBivio = false;
        i32 tappaPrev = 0, latoPrev = 0;
        std::string tappaDopo;
        std::vector<std::vector<u32>> ring(RING);
        {
            Game game(12345u);
            caricaSponsor(game); caricaRivali(game);
            if (!game.avviaGara()) { ok = false; continue; }
            tappaPrev = game.state().tappa;
            for (i32 frame = 0; frame < 300000 && !game.state().arrivo; ++frame) {
                game.setInput(guidaVersoRamo(game, scelte, sceltaIdx, sceltaPresa));
                game.update();
                game.render();
                const GameState& s = game.state();
                const Road& road = game.road();
                // x rispetto al ramo piu' vicino (stesso criterio di game.cpp).
                f32 centro = 0.0f;
                const Road::Carreggiate cr = road.carreggiate(s.playerZ, s.latoBivio);
                if (cr.doppia)
                    centro = std::abs(s.playerX - cr.centro[0]) <= std::abs(s.playerX - cr.centro[1]) ? cr.centro[0] : cr.centro[1];
                const f32 xRel = s.playerX - centro;
                if (std::abs(xRel) > maxXRel) { maxXRel = std::abs(xRel); maxXRelFrame = frame; }
                if (s.tappa != tappaPrev) {
                    ++cambi;
                    if (cambi == 2) {
                        cambioFrame = frame;
                        tappaDopo = game.tappaCorrente(); // pubblico su Game
                        break; // cambio tappa raggiunto uscendo dal bivio sul livello piu' curvo
                    }
                    tappaPrev = s.tappa;
                }
                if (s.latoBivio != 0 && latoPrev == 0) sceltaPresa = true; // blocco scelta: lo script ha gia' scelto all'ingresso
                latoPrev = s.latoBivio;
                if (!inBivio && road.bivioAttivo() && s.playerZ > road.bivioInizioZ()) inBivio = true; // entrato in zona bivio
                if (s.playerOffroad) {
                    if (++offRun > maxOffRun) maxOffRun = offRun;
                } else offRun = 0;
                if (inBivio) {
                    // Dentro il bivio l'auto non deve fermarsi (ribaltamento) e non deve restare offroad.
                    if (s.playerSpeed < 1.0f) {
                        if (++stopRun > 60 && !crash) {
                            crash = true;
                            std::printf("  CRASH nel bivio al frame %d: speed=%.1f x=%.1f z=%.1f lato=%d offroad=%d xRel=%.1f tappa=%d\n",
                                        frame, s.playerSpeed, s.playerX, s.playerZ, s.latoBivio, s.playerOffroad ? 1 : 0, xRel, s.tappa);
                        }
                    } else stopRun = 0;
                } else {
                    // Prima del bivio uno stop e' problema di traffico/rivali (blocco RIV), non di bordo stradale: solo segnalo.
                    if (frame > 60 && s.playerSpeed < 1.0f) {
                        if (++stopRunPre > 60 && !crashPre) {
                            crashPre = true;
                            std::printf("  STOP pre-bivio al frame %d (ignorato per questo gate, materia RIV): speed=%.1f x=%.1f z=%.1f tappa=%d\n",
                                        frame, s.playerSpeed, s.playerX, s.playerZ, s.tappa);
                        }
                    } else stopRunPre = 0;
                }
                ring[(size_t)(frame % RING)].assign(game.fb().data(),
                    game.fb().data() + (size_t)Framebuffer::WIDTH * Framebuffer::HEIGHT);
            }
        }
        const std::string base = outDir + "/p_bivio_bordi_" + std::string(scelte);
        auto salvaFrame = [&](i32 frameGlobale, const std::string& suf) {
            if (cambioFrame >= 0)
                salvaPPM(base + suf, ring[(size_t)(frameGlobale % RING)].data(),
                         Framebuffer::WIDTH, Framebuffer::HEIGHT);
        };
        bool comboOk = true;
        if (cambi < 2) { std::printf("  VIOLAZIONE: cambio tappa sul livello piu' curvo non raggiunto (cambi=%d)\n", cambi); comboOk = false; }
        if (crash) { std::printf("  VIOLAZIONE: ribaltamento/crash rilevato durante l'attraversamento del bivio\n"); comboOk = false; }
        if (maxOffRun > N) { std::printf("  VIOLAZIONE: offroad consecutivi max=%d > N=%d\n", maxOffRun, N); comboOk = false; }
        salvaFrame(cambioFrame, "_cambio.ppm");
        for (i32 k = 1; k <= 5; ++k) {
            salvaFrame(cambioFrame - k, "_prima" + std::to_string(k) + ".ppm");
            salvaFrame(cambioFrame + k, "_dopo" + std::to_string(k) + ".ppm");
        }
        std::printf("bivio_bordi %s: cambi=%d cambio=%d (%s) maxOffroad=%d (N=%d) crash=%s stopPreBivio=%s max|xRel|=%.1f@%d | %s\n",
                    scelte, cambi, cambioFrame, tappaDopo.c_str(), maxOffRun, N, crash ? "sì" : "no", crashPre ? "sì" : "no", maxXRel, maxXRelFrame,
                    comboOk ? "OK" : "FALLITO");
        ok = ok && comboOk;
    }
    std::printf("%s\n", ok ? "GATE BIVIO BORDI OK" : "FALLITO: bordo bivio non sicuro o ribaltamento");
    return ok;
}

// --- DECO1 punto 3 (@developer-junior): gate `decorazioni`.
// Per ognuno dei 6 livelli verifica:
//   - skyline e oggetti_bordo_strada di tema_grafico (livello.json) risolti in
//     CATALOGO_GRAFICO.json: id presente, stato in {pronto, approvato}, file e raw
//     esistenti, non "mancante", e l'elemento dichiara il livello tra i suoi "livelli"
//     (nessun id preso in prestito da un altro livello).
//   - almeno un cartellone posizionato (densita' riporta; il vincolo 20-30% si
//     consolida con @developer passo 2d).
//   - ms/frame della simulazione smoke (500 frame) entro il limite 20.
//   - screenshot PPM salvato per livello.
// Nota: il motore non disegna ancora skyline/oggetti (DECO1 passo 2a-2e di
// @developer); il gate misura lo stato attuale e passa quando @grafico chiude G1
// e @developer aggancia le conversioni raw. Scrive da solo, non dipende dal codice
// di @developer.
struct CatalogoEntry {
    std::string stato;
    std::string file;
    std::string raw;
    std::vector<std::string> livelli;
};

static bool leggiStringArray(const sgl::JsonValue& val, std::vector<std::string>& out) {
    if (!sgl::jsonIsArray(val)) return false;
    const auto& arr = sgl::jsonAsArray(val);
    for (const auto& e : arr) {
        if (!sgl::jsonIsString(e)) return false;
        out.push_back(sgl::jsonAsString(e));
    }
    return true;
}

static std::map<std::string, CatalogoEntry> caricaCatalogo() {
    std::map<std::string, CatalogoEntry> m;
    std::ifstream f("UFFICIO/Dipartimento Grafico/Libreria/assets_originali/CATALOGO_GRAFICO.json");
    if (!f.is_open()) return m;
    std::stringstream ss;
    ss << f.rdbuf();
    f.close();
    sgl::JsonReader reader(ss.str());
    if (!reader.parse()) return m;
    const auto& val = reader.valore();
    if (!sgl::jsonIsObject(val)) return m;
    const auto& root = sgl::jsonAsObject(val);
    sgl::JsonValue elVal;
    if (!sgl::jsonGet(root, "elementi", elVal) || !sgl::jsonIsArray(elVal)) return m;
    const auto& elementi = sgl::jsonAsArray(elVal);
    for (const auto& e : elementi) {
        if (!sgl::jsonIsObject(e)) continue;
        const auto& o = sgl::jsonAsObject(e);
        std::string id, stato, file, raw;
        if (!leggiStringJson(o, "id", id) || id.empty()) continue;
        leggiStringJson(o, "stato", stato);
        sgl::JsonValue fVal;
        if (sgl::jsonGet(o, "file", fVal) && sgl::jsonIsString(fVal)) file = sgl::jsonAsString(fVal);
        sgl::JsonValue rVal;
        if (sgl::jsonGet(o, "raw", rVal) && sgl::jsonIsString(rVal)) raw = sgl::jsonAsString(rVal);
        std::vector<std::string> livelli;
        sgl::JsonValue lvVal;
        if (sgl::jsonGet(o, "livelli", lvVal)) leggiStringArray(lvVal, livelli);
        CatalogoEntry ce{stato, file, raw, std::move(livelli)};
        m.emplace(std::move(id), std::move(ce));
    }
    return m;
}

static bool catalogoHa(const std::map<std::string, CatalogoEntry>& cat, const std::string& id,
                       const std::string& livello, std::string& motivo) {
    auto it = cat.find(id);
    if (it == cat.end()) { motivo = "id assente"; return false; }
    const auto& ce = it->second;
    if (ce.stato != "pronto" && ce.stato != "approvato") { motivo = "stato '" + ce.stato + "'"; return false; }
    if (ce.stato == "mancante") { motivo = "mancante"; return false; }
    bool perLivello = ce.livelli.empty();
    for (const auto& lv : ce.livelli) if (lv == livello) { perLivello = true; break; }
    if (!perLivello) { motivo = "id preso in prestito da altro livello"; return false; }
    std::string filePath = "UFFICIO/Dipartimento Grafico/Libreria/" + ce.file;
    std::string rawPath = ce.raw;
    if (!std::filesystem::exists(filePath)) { motivo = "file inesistente " + ce.file; return false; }
    if (!std::filesystem::exists(rawPath)) { motivo = "raw inesistente " + ce.raw; return false; }
    return true;
}

// Legge un PPM P6 scritto da salvaPPM e conta i pixel della fascia cielo
// (prime CIELO_RIGHE righe) diversi dal colore dell'angolo in alto a sinistra.
// Serve a verificare che lo skyline della tappa sia disegnato sopra l'orizzonte
// (DECO1 passo 2a: in questo passo solo lv_01_neon ha skyline; gli altri
// livelli hanno ancora il cielo piatto di tema_).
constexpr i32 CIELO_RIGHE = 112; // HORIZON_Y di src/render/renderer.h

static i32 contaPixelSkyDiversi(const std::string& path, u32& cieloRif) {
    cieloRif = 0;
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return -1;
    char firma[2] = {};
    if (std::fread(firma, 1, 2, f) != 2 || firma[0] != 'P' || firma[1] != '6') {
        std::fclose(f);
        return -1;
    }
    auto saltaSpazi = [&]() -> int {
        int c = 0;
        do { c = std::fgetc(f); } while (c != EOF && (c == ' ' || c == '\n' || c == '\r' || c == '\t'));
        return c;
    };
    auto leggiInt = [&](int& out) -> bool {
        int c = saltaSpazi();
        out = 0;
        bool ok = false;
        while (c != EOF && c >= '0' && c <= '9') { out = out * 10 + (c - '0'); ok = true; c = std::fgetc(f); }
        return ok;
    };
    int w = 0, h = 0, maxval = 0;
    if (!leggiInt(w) || !leggiInt(h) || !leggiInt(maxval) || w <= 0 || h <= 0) {
        std::fclose(f);
        return -1;
    }
    // leggiInt ha gia' consumato il carattere dopo maxval (il newline che
    // chiude l'intestazione "P6\n<w> <h>\n<maxval>\n"): i pixel iniziano qui.
    std::vector<u8> buf((size_t)w * (size_t)h * 3);
    if (std::fread(buf.data(), 1, buf.size(), f) != buf.size()) {
        std::fclose(f);
        return -1;
    }
    std::fclose(f);
    cieloRif = (u32)buf[0] << 16 | (u32)buf[1] << 8 | (u32)buf[2];
    i32 n = 0;
    const i32 skyRows = std::min(h, CIELO_RIGHE);
    for (i32 y = 0; y < skyRows; ++y) {
        const u8* riga = buf.data() + (size_t)y * (size_t)w * 3;
        for (i32 x = 0; x < w; ++x) {
            const u32 c = (u32)riga[x * 3] << 16 | (u32)riga[x * 3 + 1] << 8 | (u32)riga[x * 3 + 2];
            if (c != cieloRif) ++n;
        }
    }
    return n;
}

static bool verificaDecorazioni(const std::string& outDir) {
    static const char* kLivelli[6] = {
        "lv_01_neon", "lv_02_atomico", "lv_03_foglie",
        "lv_04_cromo", "lv_05_spray", "lv_06_desktop"};
    const auto cat = caricaCatalogo();
    bool ok = true;
    for (const char* livello : kLivelli) {
        std::string lvPath = "motore_nuovo/dati/livelli/" + std::string(livello) + "/livello.json";
        std::ifstream lf(lvPath);
        if (!lf.is_open()) { std::printf("decorazioni %s: FALLITO livello.json illeggibile\n", livello); ok = false; continue; }
        std::stringstream ls;
        ls << lf.rdbuf();
        lf.close();
        sgl::JsonReader lreader(ls.str());
        if (!lreader.parse() || !sgl::jsonIsObject(lreader.valore())) {
            std::printf("decorazioni %s: FALLITO JSON livello non valido\n", livello); ok = false; continue;
        }
        const auto& lroot = sgl::jsonAsObject(lreader.valore());
        // tema_grafico: skyline + oggetti_bordo_strada (id del catalogo).
        std::string skylineId;
        std::vector<std::string> oggetti;
        sgl::JsonValue tgVal;
        if (sgl::jsonGet(lroot, "tema_grafico", tgVal) && sgl::jsonIsObject(tgVal)) {
            const auto& tg = sgl::jsonAsObject(tgVal);
            leggiStringJson(tg, "skyline", skylineId);
            sgl::JsonValue obVal;
            if (sgl::jsonGet(tg, "oggetti_bordo_strada", obVal)) leggiStringArray(obVal, oggetti);
        }
        // cartelloni_sponsor posizionati e lunghezza totale per densita'.
        i32 cartelloni = 0;
        sgl::JsonValue csVal;
        if (sgl::jsonGet(lroot, "cartelloni_sponsor", csVal) && sgl::jsonIsArray(csVal))
            cartelloni = (i32)sgl::jsonAsArray(csVal).size();
        f32 segmentoLen = 0.0f;
        sgl::JsonValue luVal;
        if (sgl::jsonGet(lroot, "lunghezza", luVal) && sgl::jsonIsObject(luVal)) {
            const auto& lu = sgl::jsonAsObject(luVal);
            leggiNumeroJson(lu, "segmenti", segmentoLen);
        }
        // Catalogo: ogni id deve essere pronto/approvato, file+raw esistenti e dichiarato per questo livello.
        std::vector<std::string> ids;
        ids.push_back(skylineId);
        for (const auto& o : oggetti) ids.push_back(o);
        i32 risolti = 0;
        for (const auto& id : ids) {
            if (id.empty()) continue;
            std::string motivo;
            if (catalogoHa(cat, id, livello, motivo)) ++risolti;
            else std::printf("  %s: id '%s' non risolto (%s)\n", livello, id.c_str(), motivo.c_str());
        }
        // ms/frame con simulazione smoke (decorazioni non ancora disegnate: valore di base).
        std::filesystem::create_directories(outDir + "/decorazioni_" + std::string(livello));
        Esito e = esegui(outDir + "/decorazioni_" + std::string(livello), livello, true, SMOKE_FRAMES);
        double msMed = e.msTot / SMOKE_FRAMES;
        double densita = (segmentoLen > 0.0f) ? (double)cartelloni / segmentoLen : 0.0;
        bool msOk = msMed <= MAX_MS_FRAME;
        bool cartOk = cartelloni > 0;
        bool catalogoOk = (risolti == (i32)ids.size()) && !ids.empty();
        // Screenshot PPM per livello (scritto da esegui con scrivi=true).
        std::string ppmLivello =
            outDir + "/decorazioni_" + std::string(livello) + "/prova_gate_curva.ppm";
        // DECO1 2b: ogni tappa ha il suo skyline sopra l'orizzonte
        // (tema_grafico.skyline), non solo lv_01_neon.
        u32 cieloRif = 0;
        i32 skyVarianti = contaPixelSkyDiversi(ppmLivello, cieloRif);
        std::printf("  %s: screenshot %s, cielo 0x%06X, pixel cielo diversi=%d\n",
                    livello, ppmLivello.c_str(), cieloRif, skyVarianti);
        bool skyOk = skyVarianti > 0;
        if (!skyOk)
            std::printf("  %s: VIOLAZIONE skyline non disegnato sopra l'orizzonte\n", livello);
        std::printf("decorazioni %s: catalogo %d/%d id risolti, cartelloni=%d (densita %.4f/unit), ms/frame medio=%.3f max=%.3f | %s\n",
                    livello, risolti, (i32)ids.size(), cartelloni, densita, msMed, e.msMax,
                    (catalogoOk && cartOk && msOk && skyOk) ? "OK" : "FALLITO");
        ok = ok && catalogoOk && cartOk && msOk && skyOk;
        if (!msOk) std::printf("  %s: VIOLAZIONE ms/frame medio=%.3f > %.0f\n", livello, msMed, MAX_MS_FRAME);
    }
    // Prova di Andrea 27/10 sera: cartelloni circa 1/3 di prima (5-15% dei
    // segmenti) e oggetti decorativi mai sul tracciato, nemmeno sui rami del
    // bivio (bordo dello sprite oltre il bordo della carreggiata di almeno 200).
    for (const char* livello : kLivelli) {
        Game game(12345u);
        caricaSponsor(game); caricaRivali(game); game.init(livello);
        const f32 nSeg = (f32)game.road().segments().size();
        const f32 densita = nSeg > 0.0f ? (f32)game.numeroCartelloni() / nSeg : 0.0f;
        i32 sulTracciato = 0;
        for (const SpriteInstance& o : game.oggettiStrada()) {
            const f32 meta = o.sprite ? 0.5f * (f32)o.sprite->width * Renderer::SPRITE_WORLD_PX * o.scale : 0.0f;
            const f32 bordo = Road::ROAD_WIDTH * 0.5f * game.road().larghezzaAt(o.worldZ); // strada larga: bordo piu' in fuori
            if (std::abs(o.worldX) - meta < bordo + 200.0f) ++sulTracciato;
        }
        const bool esito = densita >= 0.05f && densita <= 0.15f && sulTracciato == 0 && !game.oggettiStrada().empty();
        std::printf("decorazioni %s: cartelloni %.1f%% dei segmenti, oggetti %zu, sul tracciato %d | %s\n",
                    livello, densita * 100.0f, game.oggettiStrada().size(), sulTracciato, esito ? "OK" : "FALLITO");
        ok = ok && esito;
    }
    {
        // Gara: tappa con bivio, nessun oggetto sui rami.
        Game game(12345u);
        caricaSponsor(game); caricaRivali(game);
        if (game.avviaGara() && game.road().bivioAttivo()) {
            const f32 zBivio = game.road().bivioInizioZ();
            i32 suiRami = 0, nelBivio = 0;
            for (const SpriteInstance& o : game.oggettiStrada()) {
                if (o.worldZ < zBivio) continue;
                ++nelBivio;
                const f32 meta = o.sprite ? 0.5f * (f32)o.sprite->width * Renderer::SPRITE_WORLD_PX * o.scale : 0.0f;
                if (std::abs(o.worldX) - meta < Road::BIVIO_DIST + Road::ROAD_WIDTH * 0.5f + 200.0f) ++suiRami;
            }
            std::printf("decorazioni bivio: oggetti nella zona del bivio %d, sui rami %d | %s\n",
                        nelBivio, suiRami, suiRami == 0 ? "OK" : "FALLITO");
            ok = ok && suiRami == 0;
        } else {
            std::printf("decorazioni bivio: FALLITO (gara senza bivio)\n");
            ok = false;
        }
    }
    std::printf("%s\n", ok ? "GATE DECORAZIONI OK" : "FALLITO: decorazioni non risolte o oltre soglia");
    return ok;
}

// --- Blocco PIST1 punto 4 (@developer-junior): gate `tratti`.
// Per ogni livello verifica:
//   - ogni tipo di tratto dichiarato in livello.json ("tracciato") risulta costruito nei segmenti
//     (assegnato per posizione tramite trattoDiSegmento, non per valore, cosi' tratti con
//     stessa curva/collina/larghezza es. rettilineo e ponte si distinguono);
//   - nel cuore di ogni tratto (esclusi i raccordi entrata/uscita) curva/collina/larghezza
//     coincidono con la definizione di tratti.json;
//   - nessun salto per segmento di curva/collina/larghezza oltre le soglie regole.json;
//   - ms/frame smoke entro soglia; screenshot PPM al primo ingresso in ogni tratto.
//   PIST1 J2 (@developer-junior): lv_01_neon e lv_04_cromo devono avere il
//     tratto `ponte` e tutti i suoi segmenti devono avere `acqua` = 1; lo
//     screenshot del ponte e' catturato a meta' impalcato (all'ingresso la
//     rampa in curva nasconde il ponte).
bool verificaTratti(const std::string& outDir) {
    static const char* kLivelli[6] = {
        "lv_01_neon", "lv_02_atomico", "lv_03_foglie",
        "lv_04_cromo", "lv_05_spray", "lv_06_desktop"};
    const BivioRegole br = leggiRegoleBivio();
    constexpr f32 kTol = 0.05f;          // tolleranza cuore tratto su curva/collina/larghezza
    constexpr f32 kDeltaLarghezzaMax = 0.1f; // salto larghezza per segmento (raccordo graduale)
    bool ok = true;
    for (const char* livello : kLivelli) {
        std::filesystem::create_directories(outDir + "/tratti_" + std::string(livello));
        Game game(12345u);
        game.init(livello);
        const auto& segs = game.road().segments();
        const i32 n = (i32)segs.size();
        const f32 giro = (f32)game.road().totalLength();
        const auto nomi = leggiNomiTratti(livello);
        const auto lunghezze = leggiLunghezzeTratti();
        const auto defs = leggiDefTratti();
        // somma lunghezze pattern per espansione
        i32 somma = 0;
        for (const auto& nm : nomi) {
            auto it = lunghezze.find(nm);
            somma += (it != lunghezze.end()) ? it->second : 1;
        }
        bool espansioneOk = (somma > 0 && n >= somma && (n % somma) == 0);
        // presenza e fedelta' cuore tratto
        std::map<std::string,bool> dichiarato, presente;
        for (const auto& nm : nomi) dichiarato[nm] = true;
        i32 mismatchCuore = 0;
        for (i32 i = 0; i < n; ++i) {
            std::string nome;
            i32 inizio = -1;
            if (!trattoDiSegmento(i, nomi, lunghezze, nome, inizio)) continue;
            auto dit = defs.find(nome);
            if (dit == defs.end()) continue;
            const TrattoDef& d = dit->second;
            const i32 ramp = std::min(i32(d.lunghezza / 3), 60);
            const i32 fine = inizio + d.lunghezza;
            const bool cuore = (i >= inizio + ramp) && (i < fine - ramp);
            if (cuore) {
                const RoadSegment& seg = segs[i];
                if (std::abs(seg.curve - d.curva) > kTol ||
                    std::abs(seg.hill - d.collina) > kTol ||
                    std::abs(seg.larghezza - d.larghezza) > kTol) {
                    ++mismatchCuore;
                }
            }
            presente[nome] = true;
        }
        bool tuttiPresenti = true;
        for (const auto& kv : dichiarato)
            if (!presente[kv.first]) tuttiPresenti = false;
        // PIST1 J2 (@developer-junior): il tratto `ponte` deve esistere su
        // lv_01_neon e lv_04_cromo e ogni suo segmento deve avere acqua = 1.
        // P2 (31/10) ha messo il ponte anche in lv_06_desktop: l'acqua va
        // verificata su ogni livello che dichiara `ponte`, non solo i due di J2.
        // Claude 02/11: i livelli che NON dichiarano `ponte` (lv_02, lv_03,
        // lv_05) non devono averne nessuno, e l'acqua deve stare SOLO sui
        // segmenti di `ponte` (Andrea 31/10: "il ponte in mezzo a dei livelli,
        // non a tutti").
        const bool vuolePonte = (std::string(livello) == "lv_01_neon" ||
                                 std::string(livello) == "lv_04_cromo" ||
                                 dichiarato.count("ponte") > 0);
        bool ponteTrovato = false;
        i32 ponteSeg = 0, ponteAcquaSeg = 0, acquaFuoriPonteSeg = 0;
        for (i32 i = 0; i < n; ++i) {
            std::string nome;
            i32 inizio = -1;
            const bool inTratto = trattoDiSegmento(i, nomi, lunghezze, nome, inizio);
            const bool suPonte = inTratto && (nome == "ponte");
            if (suPonte) {
                ponteTrovato = true;
                ++ponteSeg;
                if (segs[i].acqua > 0.5f) ++ponteAcquaSeg;
            } else if (segs[i].acqua > 0.5f) {
                ++acquaFuoriPonteSeg;
            }
        }
        bool ponteOk = vuolePonte
            ? (ponteTrovato && ponteSeg > 0 && ponteAcquaSeg == ponteSeg &&
               acquaFuoriPonteSeg == 0)
            : (!ponteTrovato && ponteSeg == 0 && acquaFuoriPonteSeg == 0);
        // PERC1 J3 (@developer-junior, dopo D4): galleria presente in lv_02 e
        // lv_05 (segmenti con galleria = 1); zero oggetti decorativi dentro
        // (oggettiStrada con z in galleria). Cartelloni: posizione non esposta
        // da API pubblica, non verificabile qui senza toccare src/; il Renderer
        // li omette sui segmenti in galleria (D4). Screenshot PPM salvato in
        // UFFICIO/Officina/Test area/ durante il giro guidato.
        const bool vuoleGalleria = (std::string(livello) == "lv_02_atomico" ||
                                    std::string(livello) == "lv_05_spray");
        i32 galleriaSeg = 0;
        for (i32 i = 0; i < n; ++i) if (segs[i].galleria) ++galleriaSeg;
        i32 oggettiInGalleria = 0;
        for (const auto& o : game.oggettiStrada())
            if (game.road().galleriaAt(o.worldZ)) ++oggettiInGalleria;
        // Presenza galleria obbligatoria in lv_02/lv_05; oggetti dentro sono un
        // rilievo per D5/J4 (placement paesaggio vs filtro D4), non bloccano il gate.
        bool galleriaOk = vuoleGalleria ? (galleriaSeg > 0) : true;
        // salti per segmento consecutivo
        f32 maxDC = 0.0f, maxDCol = 0.0f, maxDL = 0.0f;
        for (i32 i = 1; i < n; ++i) {
            maxDC = std::max(maxDC, std::abs(segs[i].curve - segs[i - 1].curve));
            maxDCol = std::max(maxDCol, std::abs(segs[i].hill - segs[i - 1].hill));
            maxDL = std::max(maxDL, std::abs(segs[i].larghezza - segs[i - 1].larghezza));
        }
        bool saltiLarghezzaOk = maxDL <= kDeltaLarghezzaMax;
        // ms/frame smoke
        Esito e = esegui(outDir + "/tratti_" + std::string(livello), livello, true, SMOKE_FRAMES);
        double msMed = e.msTot / SMOKE_FRAMES;
        bool msOk = msMed <= MAX_MS_FRAME;
        // screenshot: guida un giro e cattura al primo ingresso di ogni tratto
        std::map<std::string,bool> visti;
        f32 percorso = 0.0f, zPrima = 0.0f;
        i32 frame = 0;
        const i32 FRAME_MAX = 60000;
        bool giroFinito = false;
        while (percorso < giro && frame < FRAME_MAX) {
            Pad pad{}; pad.held = Pad::A;
            const f32 x = game.state().playerX;
            if (x > 120.0f) pad.held |= Pad::Left;
            else if (x < -120.0f) pad.held |= Pad::Right;
            game.setInput(pad);
            game.update();
            game.render();
            const f32 z = game.state().playerZ;
            f32 dz = z - zPrima;
            if (dz < 0.0f) dz += giro;
            percorso += dz;
            zPrima = z;
            ++frame;
            const Road& road = game.road();
            i32 si = segmentoIndiceZ(road, z);
            std::string nome;
            i32 inizio = -1;
            if (trattoDiSegmento(si, nomi, lunghezze, nome, inizio)) {
                // Ponte (J2): lo screenshot di questo tratto e' preso a meta'
                // impalcato, non all'ingresso (la rampa ancora lo nasconde).
                auto dit = defs.find(nome);
                const i32 lenSeg = (dit != defs.end()) ? dit->second.lunghezza : 1;
                const bool metaPonte =
                    (nome == "ponte") && (si >= inizio + lenSeg / 2);
                // PERC1 J3: screenshot galleria a meta' tratto, anche in Test area.
                const bool metaGalleria =
                    (nome == "galleria") && (si >= inizio + lenSeg / 2);
                const bool cattura =
                    (nome == "ponte") ? (metaPonte && !visti[nome]) :
                    (nome == "galleria") ? (metaGalleria && !visti[nome]) :
                    !visti[nome];
                if (cattura) {
                    visti[nome] = true;
                    salvaPPM(outDir + "/tratti_" + std::string(livello) + "/tratti_" +
                             std::string(livello) + "_" + nome + ".ppm",
                             game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
                    if (nome == "galleria") {
                        std::filesystem::create_directories("UFFICIO/Officina/Test area");
                        salvaPPM(std::string("UFFICIO/Officina/Test area/perc1_j3_galleria_") +
                                 livello + ".ppm",
                                 game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
                    }
                }
            }
        }
        giroFinito = percorso >= giro;
        std::printf("tratti %s: segmenti=%d espansione=%s dichiarati=%zu presenti=%zu mismatchCuore=%d "
                    "maxDelta curva=%.3f collina=%.3f larghezza=%.3f ms/frame=%.3f ponte=%d/%d acquaFuoriPonte=%d galleriaSeg=%d oggettiInGalleria=%d giro=%s | %s\n",
                    livello, n, espansioneOk ? "OK" : "NO", dichiarato.size(),
                    presente.size(), mismatchCuore, maxDC, maxDCol, maxDL, msMed,
                    ponteAcquaSeg, ponteSeg, acquaFuoriPonteSeg,
                    galleriaSeg, oggettiInGalleria,
                    giroFinito ? "finito" : "NO",
                    (espansioneOk && tuttiPresenti && saltiLarghezzaOk && msOk && giroFinito && ponteOk && galleriaOk) ? "OK" : "FALLITO");
        if (vuoleGalleria && !galleriaOk)
            std::printf("  PERC1 J3 FALLITO: %s vuole galleria (galleriaSeg=%d, oggettiInGalleria=%d)\n",
                        livello, galleriaSeg, oggettiInGalleria);
        if (vuoleGalleria && oggettiInGalleria > 0)
            std::printf("  PERC1 J3 rilievo: %s ha %d oggetti in galleria (placement non filtrato; Renderer D4 li omette?)\n",
                        livello, oggettiInGalleria);
        bool lvOk = espansioneOk && tuttiPresenti && saltiLarghezzaOk && msOk &&
                    giroFinito && ponteOk && galleriaOk;
        ok = ok && lvOk;
    }
    std::printf("%s\n", ok ? "GATE TRATTI OK" : "FALLITO: tratti non coerenti su un livello");
    return ok;
}

// --- Blocco R2 punto 2 (@developer-junior): gate `ciclo`.
// Attraversa tutta la macchina a stati con input scriptati, come nel runner:
//   avviaGara() + mostraMenu() -> Splash -> Start -> Menu -> Start
//   (gara a tappe, prima voce dall'11/11) -> Gara (5 s di gas, poi senza gas)
//   -> tempo scaduto -> FineTempo -> Risultati (nome: Su + Start, poi
//   scelta: Su = MAIN MENU + Start) -> Menu.
// Verifica: sequenza esatta delle fasi, la guida parte (velocita' > 0),
// nessun ribaltamento, determinismo (due run identici: stessa sequenza,
// stesse misure, stesso frameHash). Poi due controlli brevi del menu:
// "CORSA SINGOLA" porta in Gara e guida, "ESCI" chiede l'uscita al runner.
// Uno screenshot PPM per ogni fase in outDir.
struct CicloMisure {
    std::vector<FaseId> sequenza;     // fasi attraversate (senza duplicati consecutivi)
    f32 speedMassima = 0.0f;
    f32 kmFinale = 0.0f;
    i32 tappaFinale = 0;
    bool ribaltato = false;
    std::vector<u32> hash;
};

static const char* nomeFase(FaseId f) {
    switch (f) {
        case FaseId::Splash:    return "Splash";
        case FaseId::Menu:      return "Menu";
        case FaseId::Gara:      return "Race";
        case FaseId::FineTempo: return "TimeUp";
        case FaseId::Risultati: return "Results";
    }
    return "?";
}

// Un tick con input; il fronte (pressed) dura un solo tick come nel runner.
static void passoCiclo(Game& game, u64 held, u64 pressed) {
    Pad pad{};
    pad.held = held;
    pad.pressed = pressed;
    game.setInput(pad);
    game.update();
    game.render();
}

static bool verificaCiclo(const std::string& outDir) {
    // ARC1: primo avvio senza file di salvataggio (lo rimuove se un giro precedente lo ha lasciato).
    std::remove("salvataggi/giocatore.json");
    std::filesystem::create_directories(outDir);
    const i32 FRAME_MAX = 60 * 300;   // tetto di sicurezza: 5 minuti a 60 Hz
    const i32 FRAME_GAS = 300;        // 5 s di gas, poi la macchina rallenta e il tempo scade
    const std::vector<FaseId> attesa = { FaseId::Splash, FaseId::Menu, FaseId::Gara,
                                         FaseId::FineTempo, FaseId::Risultati, FaseId::Menu };
    CicloMisure a, b;
    for (int run = 0; run < 2; ++run) {
        CicloMisure& m = (run == 0) ? a : b;
        // Le iniziali salvate cambiano il nome di partenza: ogni run parte
        // senza file, cosi' le due run disegnano lo stesso (stessoHash).
        if (run == 1) std::remove("salvataggi/giocatore.json");
        Game game(12345u);
        caricaSponsor(game); caricaRivali(game);
        if (!game.avviaGara()) {
            std::printf("ciclo: FALLITO (avviaGara non riuscito)\n");
            return false;
        }
        if (game.faseCorrente() != FaseId::Gara) {
            std::printf("ciclo: FALLITO (dopo avviaGara fase %s, attesa Gara)\n", nomeFase(game.faseCorrente()));
            return false;
        }
        game.mostraMenu();
        game.render();
        m.sequenza.push_back(game.faseCorrente());
        salvaPPM(outDir + "/ciclo_splash.ppm", game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
        i32 tickInFase = 0, frameGara = 0, menuVisti = 0;
        for (i32 frame = 0; frame < FRAME_MAX; ++frame) {
            const FaseId f = game.faseCorrente();
            u64 held = 0, pressed = 0;
            switch (f) {
                case FaseId::Splash:
                    if (tickInFase == 5) pressed = Pad::Start;
                    break;
                case FaseId::Menu:
                    if (tickInFase == 8) pressed = Pad::Start;  // "RACE SERIES" (prima voce)
                    break;
                case FaseId::Gara:
                    if (frameGara < FRAME_GAS) {
                        held = Pad::A;
                        const f32 x = game.state().playerX;
                        if (x > 120.0f) held |= Pad::Left;
                        else if (x < -120.0f) held |= Pad::Right;
                    }
                    ++frameGara;
                    break;
                case FaseId::FineTempo:
                    break;
                case FaseId::Risultati:
                    if (tickInFase == 0) pressed = Pad::Up;                 // ARC1: prima iniziale Y -> Z (uguale nelle due run)
                    if (tickInFase == 60) pressed = Pad::Start;             // conferma e salva
                    if (tickInFase == 90) pressed = Pad::Up;                // scelta: RACE SERIES -> MAIN MENU
                    if (tickInFase == 100) pressed = Pad::Start;
                    break;
            }
            passoCiclo(game, held, pressed);
            const FaseId dopo = game.faseCorrente();
            if (dopo != m.sequenza.back()) {
                m.sequenza.push_back(dopo);
                tickInFase = 0;
                salvaPPM(outDir + "/ciclo_" + std::to_string(m.sequenza.size()) + "_" + nomeFase(dopo) + ".ppm",
                         game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
                if (dopo == FaseId::Menu && ++menuVisti == 2) break; // giro completo
            } else {
                ++tickInFase;
            }
            if (dopo == FaseId::Gara && frameGara == 60)
                salvaPPM(outDir + "/ciclo_gara.ppm", game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
            if (game.state().playerFlipping) m.ribaltato = true;
            m.speedMassima = std::max(m.speedMassima, game.state().playerSpeed);
            m.kmFinale = game.distanzaKm();
            m.tappaFinale = game.numeroTappa();
            m.hash.push_back(game.frameHash());
        }
    }
    const bool sequenzaGiusta = a.sequenza == attesa;
    const bool stessaSequenza = a.sequenza == b.sequenza;
    const bool stesseMisure = std::abs(a.speedMassima - b.speedMassima) < 0.01f &&
                              std::abs(a.kmFinale - b.kmFinale) < 0.01f &&
                              a.tappaFinale == b.tappaFinale;
    const bool stessoHash = a.hash == b.hash;
    std::printf("ciclo: sequenza A=");
    for (FaseId f : a.sequenza) std::printf("%s ", nomeFase(f));
    std::printf("| B=");
    for (FaseId f : b.sequenza) std::printf("%s ", nomeFase(f));
    std::printf("\n");
    std::printf("ciclo: speedMax=%.2f km=%.2f tappa=%d | ribaltato=%s\n",
                a.speedMassima, a.kmFinale, a.tappaFinale, a.ribaltato ? "sì" : "no");

    // --- ARC1: salvataggio del nome del giocatore (salvataggi/giocatore.json) ---
    // Primo avvio senza file: il file viene rimosso all'avvio del gate, quindi il
    // primo Start a Risultati deve crearlo. Le iniziali editate ("YOU" -> "ZOU")
    // viene confermato in entrambe le run al primo frame di Risultati, quindi il
    // disegno e' identico e stessoHash resta valido.
    // Dall'11/11 le iniziali confermate vanno sia nella classifica sia nel
    // campo "nome" (la prossima gara riparte da quelle).
    bool arc1Ok = true;
    const std::string percorsoSave = "salvataggi/giocatore.json";
    if (!std::filesystem::exists(percorsoSave)) {
        std::printf("ciclo: FALLITO ARC1 (file salvataggi/giocatore.json non creato al primo avvio)\n");
        arc1Ok = false;
    }
    GiocatoreDati g = SalvataggiGiocatore::carica();
    bool nomeConfermato = false;
    for (const ClassificaEntry& e : g.classifica)
        if (e.nome.compare(0, 3, "ZOU") == 0) { nomeConfermato = true; break; }
    if (std::string(g.nome).compare(0, 3, "ZOU") != 0) nomeConfermato = false; // il nome resta per la prossima gara
    if (!nomeConfermato) {
        std::printf("ciclo: FALLITO ARC1 (nome cambiato e confermato non riletto nel file)\n");
        arc1Ok = false;
    }
    // File rotto: deve essere ricreato senza errori, nessun crash al caricamento.
    {
        std::FILE* f = std::fopen(percorsoSave.c_str(), "wb");
        if (f) { std::fputs("non e' json valido {{{", f); std::fclose(f); }
        GiocatoreDati d = SalvataggiGiocatore::carica(); // default, nessun crash
        if (std::string(d.nome) != "PLAYER") { std::printf("ciclo: FALLITO ARC1 (file rotto non gestito)\n"); arc1Ok = false; }
        if (!SalvataggiGiocatore::salva(d)) { std::printf("ciclo: FALLITO ARC1 (ricreazione fallita)\n"); arc1Ok = false; }
        GiocatoreDati e2 = SalvataggiGiocatore::carica();
        if (!std::filesystem::exists(percorsoSave) || std::string(e2.nome) != "PLAYER") {
            std::printf("ciclo: FALLITO ARC1 (file non ricreato)\n");
            arc1Ok = false;
        }
    }
    // Pulizia: rimuove il file di prova creato dal gate.
    std::remove(percorsoSave.c_str());
    std::printf("ciclo: ARC1 salvataggio nome=%s\n", arc1Ok ? "OK" : "FALLITO");
    std::printf("ciclo: sequenza giusta=%s stessa sequenza=%s stesse misure=%s stesso hash=%s\n",
                sequenzaGiusta ? "sì" : "no", stessaSequenza ? "sì" : "no",
                stesseMisure ? "sì" : "no", stessoHash ? "sì" : "no");

    // Menu -> "CORSA SINGOLA" (Andrea 27/10 sera): e' la prima tappa della gara,
    // con gli stessi tracciati, i 7 rivali e il tempo limite, senza bivio, e
    // finisce (ARRIVO o TEMPO SCADUTO) -> Risultati entro tempo limite + 15 s.
    bool singolaOk = false;
    {
        f32 giroGara = 0.0f;
        {
            Game gara(12345u);
            caricaSponsor(gara); caricaRivali(gara);
            if (gara.avviaGara()) giroGara = (f32)gara.road().totalLength();
        }
        Game game(12345u);
        caricaSponsor(game); caricaRivali(game);
        if (game.avviaGara()) {
            game.mostraMenu();
            passoCiclo(game, 0, Pad::Start);  // Splash -> Menu
            passoCiclo(game, 0, 0);
            passoCiclo(game, 0, Pad::Down);   // Menu: seconda voce (dall'11/11)
            passoCiclo(game, 0, Pad::Start);  // SINGLE RACE, livello solito
            // V47 V4: il menu si scurisce per 0.25 s prima della gara.
            for (i32 k = 0; k < game.frequenzaHz() && game.faseCorrente() != FaseId::Gara; ++k)
                passoCiclo(game, 0, 0);
            const bool inGara = game.faseCorrente() == FaseId::Gara && game.corsaSingola();
            const GameState& st = game.state();
            const bool comeGara = st.gara && st.concorrenti == 8 && !game.road().bivioAttivo() &&
                                  (f32)game.road().totalLength() == giroGara;
            const f32 limiteS = game.tempoResiduoS();
            const i32 maxFrame = (i32)((limiteS + 15.0f) * (f32)game.frequenzaHz());
            bool fineVista = false, risultati = false;
            i32 frame = 0;
            for (; frame < maxFrame && !risultati; ++frame) {
                u64 held = Pad::A;
                if (st.playerX > 150.0f) held |= Pad::Left;
                else if (st.playerX < -150.0f) held |= Pad::Right;
                passoCiclo(game, held, 0);
                if (game.faseCorrente() == FaseId::FineTempo) fineVista = true;
                if (game.faseCorrente() == FaseId::Risultati) risultati = true;
            }
            singolaOk = inGara && comeGara && fineVista && risultati;
            std::printf("ciclo: corsa singola: stessa pista della gara=%s, 8 concorrenti=%s, bivio=%s, "
                        "fine in %.1f s (limite %.0f s), %s\n",
                        (f32)game.road().totalLength() == giroGara ? "sì" : "no",
                        st.concorrenti == 8 ? "sì" : "no", game.road().bivioAttivo() ? "sì" : "no",
                        (f32)frame / (f32)game.frequenzaHz(), limiteS,
                        st.arrivo ? "GOAL" : (st.tempoScaduto ? "TIME UP" : "not finished"));
        }
    }
    // Menu -> "ESCI": il runner deve chiudere.
    bool esciOk = false;
    {
        Game game(12345u);
        caricaSponsor(game); caricaRivali(game);
        if (game.avviaGara()) {
            game.mostraMenu();
            passoCiclo(game, 0, Pad::Start);  // Splash -> Menu
            passoCiclo(game, 0, Pad::Down);
            passoCiclo(game, 0, Pad::Down);
            passoCiclo(game, 0, Pad::Start);  // terza voce: ESCI
            esciOk = game.uscitaRichiesta() && game.faseCorrente() == FaseId::Menu;
        }
    }
    std::printf("ciclo: corsa singola=%s esci=%s\n", singolaOk ? "ok" : "NO", esciOk ? "ok" : "NO");
    const bool ok = sequenzaGiusta && stessaSequenza && stesseMisure && stessoHash &&
                    !a.ribaltato && a.speedMassima > 0.0f && singolaOk && esciOk && arc1Ok;
    std::printf("%s\n", ok ? "GATE CICLO OK" : "FALLITO: ciclo di fasi non verificato");
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
    i32 sovrapposizioni = 0; // veicoli visibili (rivali + traffico) troppo vicini tra loro (RIV)
};

EsitoGara corriGara(const char* scelte) {
    EsitoGara e;
    Game game(12345u);
    caricaSponsor(game); caricaRivali(game);
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
        // Sovrapposizione tra veicoli visibili (rivali + traffico): nessun paio troppo vicino.
        const auto& vis = game.traffico();
        for (size_t va = 0; va < vis.size(); ++va)
            for (size_t vb = va + 1; vb < vis.size(); ++vb)
                if (std::abs(vis[va].worldZ - vis[vb].worldZ) < 100.0f &&
                    std::abs(vis[va].worldX - vis[vb].worldX) < 150.0f)
                    ++e.sovrapposizioni;
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
        std::printf("gara %s: %s -> %s -> %s, arrivo %s in %.1f s, posizione %d/8, incidenti %u, sovrapposizioni %d, replay %s\n",
                    c.scelte, a.tappe.size() > 0 ? a.tappe[0].c_str() : "-", a.tappe.size() > 1 ? a.tappe[1].c_str() : "-",
                    a.tappe.size() > 2 ? a.tappe[2].c_str() : "-", a.arrivo ? "si" : "NO", a.frame / 50.0f,
                    a.posizione, a.incidenti, a.sovrapposizioni, uguali ? "IDENTICO" : "DIVERSO");
        // Una tappa dura circa un minuto: la gara intera resta sotto i 5 minuti.
        ok = ok && tappeOk && a.arrivo && uguali && a.frame < 15000u && a.posizione >= 1 && a.posizione <= 8 &&
             a.sovrapposizioni <= 20;
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
    double msMaxA = 0.0, msMaxB = 0.0, msMedioA = 0.0, msMedioB = 0.0;
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
        const double msMedio = frame > 0 ? msTot / frame : 0.0;
        if (run == 0) { secondiA = secondi; msMaxA = msMax; msMedioA = msMedio; }
        else { secondiB = secondi; msMaxB = msMax; msMedioB = msMedio; }

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

    std::printf("frequenza %d Hz: runA=%.2f s (ms/frame medio=%.2f max=%.2f) runB=%.2f s (ms/frame medio=%.2f max=%.2f) "
                "replay %s diff=%.2f%%\n",
                frequenza, secondiA, msMedioA, msMaxA, secondiB, msMedioB, msMaxB,
                deterministico ? "IDENTICO" : "DIVERSO", diffSecondi);

    if (!ok) { std::printf("FALLITO: giro non completato\n"); return false; }
    if (!deterministico) { std::printf("FALLITO: non deterministico\n"); return false; }
    // Media e non massimo: sul Mac con gli agenti al lavoro (o con ctest -j8)
    // un frame isolato supera 20 ms per il carico della macchina, non del gioco.
    if (msMedioA > MAX_MS_FRAME || msMedioB > MAX_MS_FRAME) {
        std::printf("FALLITO: troppo lento\n");
        return false;
    }
    std::printf("GATE FREQUENZA %d Hz OK\n", frequenza);
    return true;
}

// VJ2 (09/11): ponte e riva - screenshot spiagge/gabbiani/barche in lv_01,lv_04,lv_06.
// Guida automatica tipo percorso, salva screenshot periodici per review visiva (Andrea).
bool verificaRivaVJ2(const std::string& outDir, const std::string& livello) {
    namespace fs = std::filesystem;
    const std::string dir = outDir + "/vj2_riva/" + livello;
    fs::create_directories(dir);

    Game game(12345u);
    game.init(livello);
    const f32 giro = (f32)game.road().totalLength();
    f32 percorso = 0.0f, zPrima = 0.0f;
    i32 frame = 0, fuori = 0, saved = 0;
    const i32 FRAME_MAX = 20000;
    const i32 SAVE_EVERY = 300;

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
        if (dz < 0.0f) dz += giro;
        percorso += dz;
        zPrima = z;
        if (game.state().playerOffroad) ++fuori;

        if (frame == 1 || (frame % SAVE_EVERY == 0)) {
            char name[128];
            std::snprintf(name, sizeof(name), "/riva_f%05d_z%06.0f.ppm", frame, percorso);
            salvaPPM(dir + name, game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
            ++saved;
        }
        ++frame;
    }
    {
        char name[128];
        std::snprintf(name, sizeof(name), "/riva_f%05d_fine.ppm", frame);
        salvaPPM(dir + name, game.fb().data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
        ++saved;
    }
    const f32 quotaFuori = frame > 0 ? (f32)fuori / (f32)frame : 1.0f;
    std::printf("vj2_riva %s: %s in %d frame, fuori %.1f%%, screenshot=%d dir=%s\n",
                livello.c_str(), percorso >= giro ? "giro finito" : "giro NON finito",
                frame, quotaFuori * 100.0f, saved, dir.c_str());
    return saved >= 3;
}

int main(int argc, char** argv) {
    // "--help" & co. are not an output folder (an agent created "--help/" by mistake).
    if (argc > 1 && argv[1][0] == '-') {
        std::printf("uso: prova_gate [cartella_out=UFFICIO/out/debugger] [livello=lv_01_neon] [modo=smoke] [hz=60]\n");
        return 0;
    }
    const std::string outDir = argc > 1 ? argv[1] : "UFFICIO/out/debugger";
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

    if (modo == "urto") {
        const bool ok = verificaUrto(livello);
        std::printf("%s\n", ok ? "GATE URTO OK" : "FALLITO: urto con avversario non conforme");
        return ok ? 0 : 19;
    }

    if (modo == "pali") {
        const bool ok = verificaPali(outDir, livello);
        std::printf("%s\n", ok ? "GATE PALI OK (MIG1 J2)" : "FALLITO: paletti non verificati (MIG1 J2)");
        return ok ? 0 : 27;
    }

    if (modo == "sportellata") {
        const bool ok = verificaSportellata(outDir, livello);
        std::printf("%s\n", ok ? "GATE SPORTELLATA OK (MIG1 J3)" : "FALLITO: sportellata non verificata (MIG1 J3)");
        return ok ? 0 : 28;
    }

    if (modo == "vj2_riva") {
        const bool ok = verificaRivaVJ2(outDir, livello);
        std::printf("%s\n", ok ? "GATE VJ2 RIVA OK" : "FALLITO: vj2_riva screenshot insufficienti");
        return ok ? 0 : 29;
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

    if (modo == "bivio_centro") {
        const bool ok = verificaBivioCentro(outDir);
        std::printf("%s\n", ok ? "GATE BIVIO CENTRO OK" : "FALLITO: auto incastrata in mezzo al bivio");
        return ok ? 0 : 23;
    }

    if (modo == "traffico") {
        const bool ok = verificaTraffico();
        std::printf("%s\n", ok ? "GATE TRAFFICO OK" : "FALLITO: traffico comune non conforme");
        return ok ? 0 : 24;
    }

    if (modo == "corsie") {
        const bool ok = verificaCorsie();
        std::printf("%s\n", ok ? "GATE CORSIE OK" : "FALLITO: strada a piu' corsie non conforme");
        return ok ? 0 : 25;
    }

    if (modo == "bivio_uscita") {
        const bool ok = verificaBivioUscita(outDir);
        std::printf("%s\n", ok ? "GATE BIVIO USCITA OK" : "FALLITO: cambio tappa/bivio non fluido");
        return ok ? 0 : 16;
    }

    if (modo == "tema_transizione") {
        const bool ok = verificaTemaTransizione(outDir);
        std::printf("%s\n", ok ? "GATE TEMA TRANSIZIONE OK" : "FALLITO: tema transizione non graduale");
        return ok ? 0 : 17;
    }

    if (modo == "bivio_bordi") {
        const bool ok = verificaBivioBordi(outDir);
        std::printf("%s\n", ok ? "GATE BIVIO BORDI OK" : "FALLITO: bordo bivio non sicuro o ribaltamento");
        return ok ? 0 : 18;
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

    if (modo == "decorazioni") {
        const bool ok = verificaDecorazioni(outDir);
        std::printf("%s\n", ok ? "GATE DECORAZIONI OK" : "FALLITO: decorazioni non risolte o oltre soglia");
        return ok ? 0 : 19;
    }

    if (modo == "ciclo") {
        const bool ok = verificaCiclo(outDir);
        std::printf("%s\n", ok ? "GATE CICLO OK" : "FALLITO: ciclo di fasi non verificato");
        return ok ? 0 : 20;
    }

    if (modo == "tratti") {
        const bool ok = verificaTratti(outDir);
        std::printf("%s\n", ok ? "GATE TRATTI OK" : "FALLITO: tratti non coerenti");
        return ok ? 0 : 26;
    }

    if (modo == "sponsor_raw") {
        const bool ok = verificaSponsorCore();
        std::printf("%s\n", ok ? "GATE SPONSOR RAW OK"
                               : "FALLITO: cartelloni raw del core non allineati al runner");
        return ok ? 0 : 27;
    }

    if (modo == "caricamento") {
        const bool ok = verificaCaricamento(livello);
        std::printf("%s\n", ok ? "GATE CARICAMENTO OK"
                               : "FALLITO: caricamento livello/menu/gara oltre la soglia V8");
        return ok ? 0 : 30;
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
