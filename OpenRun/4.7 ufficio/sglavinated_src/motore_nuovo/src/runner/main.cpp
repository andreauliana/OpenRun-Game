// SPDX-License-Identifier: MIT
#include "game/game.h"
#include "runner/sponsor_loader.h" // decodifica cartelloni sponsor (solo runner)
#include "render/sprite_raw.h"    // caricaSpriteRaw per i raw di png_a_raw.py
#include "render/crt.h"           // filtro CRT opzionale (solo resa a schermo)
#include "core/json_mini.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace sgl; // runner: puo' usare i tipi del nucleo senza prefisso

namespace {

constexpr i32 WINDOW_SCALE = 2; // finestra 640x448 per comodita' sul desktop
constexpr i32 MAX_TICK_PER_FRAME = 5; // recupero massimo dopo un intoppo (evita la "spirale")
// Coda audio. La musica avanza solo quando il gioco fa un tick, quindi un tick
// il cui audio non entra in coda e' musica saltata (brano piu' veloce). Per
// questo non si scarta mai: con la coda piena il tick aspetta il suo turno
// (l'audio comanda il tempo), e il passo si regola di poco (+-0.5%, non si
// sente) per tenere la coda vicino all'obiettivo anche se l'orologio della
// scheda audio e quello del sistema non vanno proprio uguali.
constexpr u32 AUDIO_FRAME_OBIETTIVO = 2;  // frame di gioco in coda a regime (~33 ms)
constexpr u32 AUDIO_MAX_FRAME_IN_CODA = MAX_TICK_PER_FRAME + 2; // oltre: il tick aspetta
constexpr f64 AUDIO_CORREZIONE_PASSO = 0.005;
constexpr const char* PROJECT_DATA_MARKER =
    "motore_nuovo/dati/livelli/lv_01_neon/livello.json";

struct Opzioni {
    bool fullscreen = false;
    bool vsync = true;      // --no-vsync: per driver che bloccano la presentazione
    std::string radiceDati; // --data: cartella sglavinated_src (vuota = ricerca automatica)
    std::string livello;    // --livello: solo quella pista, in tondo (vuoto = gara a tappe)
    bool subito = false;    // --subito: salta splash e menu, si parte gia' in gara
    int crt = -1;           // --crt / --no-crt: forza il filtro CRT (-1 = config.json)
    bool misura = false;    // --misura: tempi per frame nel log ogni 5 s e riassunto all'uscita (Pi 400)
    int misuraSecondi = 0;  // --misura-secondi N: esce da solo dopo N s (implica --misura)
    bool pilotaAuto = false;// --pilota-auto: gas e sterzo verso il centro da soli (misure senza mani)
};

Opzioni leggiOpzioni(int argc, char** argv) {
    Opzioni o;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--fullscreen") == 0) o.fullscreen = true;
        else if (std::strcmp(argv[i], "--window") == 0) o.fullscreen = false;
        else if (std::strcmp(argv[i], "--no-vsync") == 0) o.vsync = false;
        else if (std::strcmp(argv[i], "--data") == 0 && i + 1 < argc) o.radiceDati = argv[++i];
        else if (std::strcmp(argv[i], "--livello") == 0 && i + 1 < argc) o.livello = argv[++i];
        else if (std::strcmp(argv[i], "--subito") == 0) o.subito = true;
        else if (std::strcmp(argv[i], "--crt") == 0) o.crt = 1;
        else if (std::strcmp(argv[i], "--no-crt") == 0) o.crt = 0;
        else if (std::strcmp(argv[i], "--misura") == 0) o.misura = true;
        else if (std::strcmp(argv[i], "--misura-secondi") == 0 && i + 1 < argc) {
            o.misura = true;
            o.misuraSecondi = std::max(1, std::atoi(argv[++i]));
        }
        else if (std::strcmp(argv[i], "--pilota-auto") == 0) o.pilotaAuto = true;
    }
    return o;
}

// --misura (Pi 400): cronometri del ciclo del runner. Non tocca il gioco:
// misura quanto costano tick (update+render), copia nella texture (con CRT) e
// presentazione a schermo, e quanti giri del ciclo arrivano in ritardo.
// Stampa una riga ogni 5 s e un riassunto all'uscita.
struct MisuraRunner {
    bool attiva = false;
    f64 budgetMs = 16.667;        // 1/frequenza logica
    u64 freq = 1;
    // finestra corrente (5 s) e totale
    struct Somme {
        u64 giri = 0, tick = 0, lenti = 0, copie = 0;
        f64 tickMs = 0, tickMax = 0, copiaMs = 0, presentaMs = 0, presentaMax = 0, giroMax = 0;
    } fin, tot;
    std::vector<f32> giriMs; // tutti i giri, per il p99 finale
    u64 inizio = 0, inizioFin = 0, prevGiro = 0;

    static f64 ms(u64 a, u64 b, u64 f) { return (f64)(b - a) * 1000.0 / (f64)f; }
    void avvia(u64 ora, u64 frequenza, i32 hz) {
        freq = frequenza;
        budgetMs = 1000.0 / (f64)hz;
        inizio = inizioFin = prevGiro = ora;
        giriMs.reserve(60 * 600);
    }
    void tick(f64 m) {
        for (Somme* s : { &fin, &tot }) { ++s->tick; s->tickMs += m; s->tickMax = std::max(s->tickMax, m); }
    }
    void giro(u64 ora, bool copiato, f64 copia, f64 presenta) {
        const f64 g = ms(prevGiro, ora, freq);
        prevGiro = ora;
        giriMs.push_back((f32)g);
        // "lento": il giro ha superato 1.5 volte il passo logico (un fotogramma saltato)
        const bool lento = g > budgetMs * 1.5;
        for (Somme* s : { &fin, &tot }) {
            ++s->giri; s->presentaMs += presenta;
            if (copiato) { ++s->copie; s->copiaMs += copia; }
            s->presentaMax = std::max(s->presentaMax, presenta);
            s->giroMax = std::max(s->giroMax, g);
            if (lento) ++s->lenti;
        }
    }
    void stampaFinestra(u64 ora, u64 attese) {
        const f64 sec = ms(inizioFin, ora, freq) / 1000.0;
        if (sec < 5.0) return;
        const Somme& s = fin;
        SDL_Log("misura: %.0f fps video, %.1f tick/s | tick %.2f ms (max %.2f) | copia %.2f ms | "
                "presenta %.2f ms (max %.2f) | giro max %.1f ms | lenti %llu | audio attese %llu",
                (f64)s.giri / sec, (f64)s.tick / sec,
                s.tick ? s.tickMs / (f64)s.tick : 0.0, s.tickMax,
                s.copie ? s.copiaMs / (f64)s.copie : 0.0,
                s.giri ? s.presentaMs / (f64)s.giri : 0.0, s.presentaMax, s.giroMax,
                (unsigned long long)s.lenti, (unsigned long long)attese);
        fin = Somme{};
        inizioFin = ora;
    }
    void riassunto(u64 ora, u64 attese) {
        const f64 sec = ms(inizio, ora, freq) / 1000.0;
        const Somme& s = tot;
        f64 p99 = 0;
        if (!giriMs.empty()) {
            std::vector<f32> c = giriMs;
            const size_t k = (size_t)((f64)(c.size() - 1) * 0.99);
            std::nth_element(c.begin(), c.begin() + (std::ptrdiff_t)k, c.end());
            p99 = c[k];
        }
        std::printf("\n=== MISURA RUNNER (%.0f s) ===\n"
                    "fps video medi      : %.1f\n"
                    "tick logici al s    : %.1f (atteso %.0f)\n"
                    "tick update+render  : media %.2f ms, max %.2f ms (budget %.2f)\n"
                    "copia texture (+CRT): media %.2f ms\n"
                    "presentazione       : media %.2f ms, max %.2f ms\n"
                    "giro del ciclo      : p99 %.1f ms, max %.1f ms\n"
                    "giri lenti (>1.5 tick): %llu su %llu (%.1f%%)\n"
                    "audio attese        : %llu\n",
                    sec, sec > 0 ? (f64)s.giri / sec : 0.0, sec > 0 ? (f64)s.tick / sec : 0.0,
                    1000.0 / budgetMs,
                    s.tick ? s.tickMs / (f64)s.tick : 0.0, s.tickMax, budgetMs,
                    s.copie ? s.copiaMs / (f64)s.copie : 0.0,
                    s.giri ? s.presentaMs / (f64)s.giri : 0.0, s.presentaMax,
                    p99, s.giroMax,
                    (unsigned long long)s.lenti, (unsigned long long)s.giri,
                    s.giri ? 100.0 * (f64)s.lenti / (f64)s.giri : 0.0,
                    (unsigned long long)attese);
        std::fflush(stdout);
    }
};

bool impostaRadice(const std::filesystem::path& root) {
    std::error_code ec;
    if (!std::filesystem::exists(root / PROJECT_DATA_MARKER, ec) || ec) return false;
    std::filesystem::current_path(root, ec);
    if (!ec) return true;
    std::fprintf(stderr, "Impossibile impostare la cartella dati %s: %s\n",
                 root.string().c_str(), ec.message().c_str());
    return false;
}

// Cerca la cartella dati (sglavinated_src) e la rende cartella corrente.
// Ordine: --data, cartella corrente, cartella dell'eseguibile (SDL_GetBasePath,
// affidabile anche dentro un .app su Mac), percorso di argv[0]; da ognuna si risale.
bool trovaRadiceProgetto(const char* executablePath, const std::string& radiceDati) {
    if (!radiceDati.empty()) {
        if (impostaRadice(radiceDati)) return true;
        std::fprintf(stderr, "--data %s: dati del gioco non trovati (%s)\n",
                     radiceDati.c_str(), PROJECT_DATA_MARKER);
        return false;
    }

    std::error_code ec;
    std::vector<std::filesystem::path> starts;
    auto cwd = std::filesystem::current_path(ec);
    if (!ec) starts.push_back(cwd);
    if (char* base = SDL_GetBasePath()) {
        starts.push_back(base);
        SDL_free(base);
    }
    ec.clear();
    auto executable = std::filesystem::absolute(executablePath, ec);
    if (!ec) starts.push_back(executable.parent_path());

    for (const auto& start : starts) {
        auto candidate = start;
        while (!candidate.empty()) {
            for (const auto& root : {candidate, candidate / "sglavinated_src"})
                if (impostaRadice(root)) return true;
            const auto parent = candidate.parent_path();
            if (parent == candidate) break;
            candidate = parent;
        }
    }

    std::fprintf(stderr,
                 "Dati del gioco non trovati (%s). Avviare l'eseguibile dal progetto, dalla sua cartella build "
                 "oppure indicare la cartella con --data <percorso di sglavinated_src>.\n",
                 PROJECT_DATA_MARKER);
    return false;
}

// "filtro_crt": true in dati/config.json accende il filtro CRT all'avvio.
bool leggiFiltroCrtDaConfig() {
    std::FILE* f = std::fopen("motore_nuovo/dati/config.json", "rb");
    if (!f) return false;
    char buf[4096];
    std::string testo;
    size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0) testo.append(buf, n);
    std::fclose(f);
    JsonReader cfg(testo);
    JsonValue v;
    if (cfg.parse() && jsonIsObject(cfg.valore()) && jsonGet(jsonAsObject(cfg.valore()), "filtro_crt", v)
        && std::holds_alternative<bool>(v.v))
        return std::get<bool>(v.v);
    return false;
}

// Filtro CRT nel runner: sbavatura del composito sulla CPU (320x224) e
// maschera fissa righe + fosfori moltiplicata sopra dalla GPU, alla scala
// intera dello schermo. La maschera si rifa' solo se la scala cambia.
struct FiltroCrt {
    bool acceso = false;
    SDL_Texture* maschera = nullptr;
    i32 scala = 0;
    std::vector<u32> sbavato = std::vector<u32>((size_t)Framebuffer::WIDTH * Framebuffer::HEIGHT);

    void preparaMaschera(SDL_Renderer* ren) {
        int ow = 0, oh = 0;
        if (SDL_GetRendererOutputSize(ren, &ow, &oh) != 0) return;
        i32 s = std::min(ow / Framebuffer::WIDTH, oh / Framebuffer::HEIGHT);
        if (s < 1) s = 1;
        if (maschera && s == scala) return;
        if (maschera) SDL_DestroyTexture(maschera);
        scala = s;
        const auto px = crtMaschera(Framebuffer::WIDTH, Framebuffer::HEIGHT, s);
        maschera = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC,
                                     Framebuffer::WIDTH * s, Framebuffer::HEIGHT * s);
        if (!maschera) { SDL_Log("filtro CRT: maschera non creata: %s", SDL_GetError()); return; }
        SDL_UpdateTexture(maschera, nullptr, px.data(), Framebuffer::WIDTH * s * 4);
        SDL_SetTextureBlendMode(maschera, SDL_BLENDMODE_MOD);
        SDL_Log("filtro CRT: maschera %dx%d (scala %d)", Framebuffer::WIDTH * s, Framebuffer::HEIGHT * s, s);
    }
    void chiudi() {
        if (maschera) SDL_DestroyTexture(maschera);
        maschera = nullptr;
    }
};

bool inizializzaSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
        SDL_Log("SDL_Init fallita: %s", SDL_GetError());
        return false;
    }
    return true;
}

// Pad: preferisce SDL_GameController (mappature standard per pad USB/Bluetooth),
// altrimenti il primo joystick generico.
struct Controller {
    SDL_GameController* pad = nullptr;
    SDL_Joystick* joy = nullptr;

    void apri() {
        if (pad || joy) return;
        for (int i = 0; i < SDL_NumJoysticks(); ++i) {
            if (SDL_IsGameController(i) && (pad = SDL_GameControllerOpen(i))) {
                SDL_Log("pad: %s", SDL_GameControllerName(pad));
                return;
            }
        }
        if (SDL_NumJoysticks() > 0 && (joy = SDL_JoystickOpen(0)))
            SDL_Log("joystick generico: %s", SDL_JoystickName(joy));
    }
    void chiudi() {
        if (pad) SDL_GameControllerClose(pad);
        if (joy) SDL_JoystickClose(joy);
        pad = nullptr;
        joy = nullptr;
    }
};

u64 leggiPadHeld(const Uint8* kb, const Controller& c, f32& axisLX, f32& axisLY) {
    u64 held = 0;
    axisLX = 0.0f;
    axisLY = 0.0f;
    if (kb[SDL_SCANCODE_UP] || kb[SDL_SCANCODE_W])    held |= Pad::Up;
    if (kb[SDL_SCANCODE_DOWN] || kb[SDL_SCANCODE_S])  held |= Pad::Down;
    if (kb[SDL_SCANCODE_LEFT] || kb[SDL_SCANCODE_A])  held |= Pad::Left;
    if (kb[SDL_SCANCODE_RIGHT] || kb[SDL_SCANCODE_D]) held |= Pad::Right;
    if (kb[SDL_SCANCODE_Z] || kb[SDL_SCANCODE_LCTRL]) held |= Pad::A;   // accelera
    if (kb[SDL_SCANCODE_X] || kb[SDL_SCANCODE_LALT])  held |= Pad::B;   // razzo
    // Invio = Start (pausa in gara, conferma nel menu): Invio principale, quello
    // del tastierino e il secondo Invio di alcune tastiere vecchie (Andrea 08/11).
    if (kb[SDL_SCANCODE_RETURN] || kb[SDL_SCANCODE_KP_ENTER] || kb[SDL_SCANCODE_RETURN2]) held |= Pad::Start;
    if (kb[SDL_SCANCODE_BACKSPACE])                   held |= Pad::Select;

    if (c.pad) {
        auto btn = [&](SDL_GameControllerButton b, u64 bit) {
            if (SDL_GameControllerGetButton(c.pad, b)) held |= bit;
        };
        btn(SDL_CONTROLLER_BUTTON_DPAD_UP, Pad::Up);
        btn(SDL_CONTROLLER_BUTTON_DPAD_DOWN, Pad::Down);
        btn(SDL_CONTROLLER_BUTTON_DPAD_LEFT, Pad::Left);
        btn(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, Pad::Right);
        btn(SDL_CONTROLLER_BUTTON_A, Pad::A);
        btn(SDL_CONTROLLER_BUTTON_B, Pad::B);
        btn(SDL_CONTROLLER_BUTTON_X, Pad::X);
        btn(SDL_CONTROLLER_BUTTON_Y, Pad::Y);
        btn(SDL_CONTROLLER_BUTTON_START, Pad::Start);
        btn(SDL_CONTROLLER_BUTTON_BACK, Pad::Select);
        // Grilletti: destro = acceleratore, sinistro = freno.
        if (SDL_GameControllerGetAxis(c.pad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 16000) held |= Pad::A;
        if (SDL_GameControllerGetAxis(c.pad, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 16000) held |= Pad::Down;
        axisLX = SDL_GameControllerGetAxis(c.pad, SDL_CONTROLLER_AXIS_LEFTX) / 32768.0f;
        axisLY = SDL_GameControllerGetAxis(c.pad, SDL_CONTROLLER_AXIS_LEFTY) / 32768.0f;
    } else if (c.joy) {
        axisLX = SDL_JoystickGetAxis(c.joy, 0) / 32768.0f;
        axisLY = SDL_JoystickGetAxis(c.joy, 1) / 32768.0f;
        // Pad USB generici non riconosciuti da SDL (es. cloni Super Nintendo):
        // 4 tasti frontali 0-3, Select 8 e Start 9 come nella maggior parte dei
        // cloni; la croce arriva dagli assi (sopra) o dal primo hat.
        const int nb = SDL_JoystickNumButtons(c.joy);
        auto jb = [&](int i, u64 bit) { if (nb > i && SDL_JoystickGetButton(c.joy, i)) held |= bit; };
        // Cloni SNES piu' diffusi: 0 = X, 1 = A, 2 = B, 3 = Y. Accelera il tasto
        // in basso (B del SNES, 2) come il tasto 0; il tasto a destra (A, 1) e'
        // l'azione B del gioco. Stesso schema del core libretro (RetroPad B = gas).
        jb(0, Pad::A); jb(1, Pad::B); jb(2, Pad::X); jb(3, Pad::Y);
        jb(2, Pad::A);
        jb(8, Pad::Select); jb(9, Pad::Start);
        if (SDL_JoystickNumHats(c.joy) > 0) {
            const Uint8 h = SDL_JoystickGetHat(c.joy, 0);
            if (h & SDL_HAT_UP)    held |= Pad::Up;
            if (h & SDL_HAT_DOWN)  held |= Pad::Down;
            if (h & SDL_HAT_LEFT)  held |= Pad::Left;
            if (h & SDL_HAT_RIGHT) held |= Pad::Right;
        }
    }
    if (axisLX < -0.5f) held |= Pad::Left;
    if (axisLX >  0.5f) held |= Pad::Right;
    if (axisLY < -0.5f) held |= Pad::Up;
    if (axisLY >  0.5f) held |= Pad::Down;
    return held;
}

#ifdef __EMSCRIPTEN__
// Versione browser (pagina web): il ciclo resta lo stesso del desktop, ma a ogni
// fotogramma restituisce il controllo al browser (Asyncify) aspettando il suo refresh.
EM_ASYNC_JS(void, webAttendiFotogramma, (), {
    await new Promise(function (r) { requestAnimationFrame(function () { r(); }); });
});
// Salvataggi (classifica, nome) nella memoria del browser (IndexedDB): la cartella
// salvataggi/ si monta prima di partire e si rilegge quello che c'era.
EM_ASYNC_JS(void, webMontaSalvataggi, (), {
    var dir = '/sglavinated_src/salvataggi';
    try { FS.mkdir(dir); } catch (e) {}
    try { FS.mount(IDBFS, {}, dir); } catch (e) { return; }
    await new Promise(function (r) { FS.syncfs(true, function () { r(); }); });
});
// Copia i salvataggi in IndexedDB (ogni qualche secondo e alla chiusura della pagina).
EM_JS(void, webScriviSalvataggi, (), {
    if (Module.sglSync) return;
    Module.sglSync = true;
    FS.syncfs(false, function () { Module.sglSync = false; });
});
// Voce ESCI del menu: nel browser non si chiude niente, decide la pagina (Module.sglEsci).
EM_JS(void, webEsci, (), {
    if (typeof Module.sglEsci === 'function') Module.sglEsci();
});
#endif

} // namespace

int main(int argc, char** argv) {
    const bool checkSDL = argc == 2 && std::strcmp(argv[1], "--check-sdl-init") == 0;
    if (checkSDL) {
        if (!inizializzaSDL()) return 1;
        std::puts("SDL_Init: OK");
        SDL_Quit();
        return 0;
    }

    const Opzioni opz = leggiOpzioni(argc, argv);
#ifdef __EMSCRIPTEN__
    webMontaSalvataggi();
#endif
    if (!trovaRadiceProgetto(argv[0], opz.radiceDati)) return 2;
    {
        std::error_code ec;
        const std::string lv = "motore_nuovo/dati/livelli/" + opz.livello + "/livello.json";
        if (!opz.livello.empty() && !std::filesystem::exists(lv, ec)) {
            std::fprintf(stderr, "--livello %s: file %s non trovato\n", opz.livello.c_str(), lv.c_str());
            return 2;
        }
    }
    if (!inizializzaSDL()) return 1;

    // Pixel nitidi: scala "nearest" (stile arcade), ridimensionamento fatto dalla GPU.
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    Uint32 winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    if (opz.fullscreen) winFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_Window* win = SDL_CreateWindow("Sglavinated — motore nuovo (prototipo)",
                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       sgl::Framebuffer::WIDTH * WINDOW_SCALE,
                                       sgl::Framebuffer::HEIGHT * WINDOW_SCALE,
                                       winFlags);
    if (!win) { SDL_Log("finestra fallita: %s", SDL_GetError()); return 1; }
    if (opz.fullscreen) SDL_ShowCursor(SDL_DISABLE);

    // Vsync: il frame si presenta al refresh dello schermo (niente tearing).
    // Se il driver non lo offre, si torna al renderer senza vsync e il ciclo dorme da solo.
    SDL_Renderer* sdlRen = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
                                              (opz.vsync ? SDL_RENDERER_PRESENTVSYNC : 0));
    if (!sdlRen) sdlRen = SDL_CreateRenderer(win, -1, 0);
    if (!sdlRen) { SDL_Log("renderer fallito: %s", SDL_GetError()); return 1; }
    SDL_RendererInfo renInfo{};
    SDL_GetRendererInfo(sdlRen, &renInfo);
    const bool vsync = (renInfo.flags & SDL_RENDERER_PRESENTVSYNC) != 0;
    SDL_Log("renderer: %s, vsync %s", renInfo.name, vsync ? "si" : "no");
    if (opz.misura) {
        SDL_DisplayMode modo{};
        if (SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(win), &modo) == 0)
            SDL_Log("misura: schermo %dx%d a %d Hz, video driver %s",
                    modo.w, modo.h, modo.refresh_rate, SDL_GetCurrentVideoDriver());
    }

    // Risoluzione logica fissa 320x224 con scala intera: proporzioni giuste a ogni
    // dimensione di finestra o schermo (bande nere ai lati se servono).
    SDL_RenderSetLogicalSize(sdlRen, sgl::Framebuffer::WIDTH, sgl::Framebuffer::HEIGHT);
    SDL_RenderSetIntegerScale(sdlRen, SDL_TRUE);
    SDL_Texture* tex = SDL_CreateTexture(sdlRen, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         sgl::Framebuffer::WIDTH, sgl::Framebuffer::HEIGHT);

    // Audio a coda (SDL_QueueAudio): ogni frame di gioco accoda i suoi campioni,
    // SDL li suona nell'ordine. Il formato richiesto e' garantito (SDL converte
    // se il dispositivo e' diverso), quindi il core non deve sapere nulla del driver.
    SDL_AudioSpec want{}, have{};
    want.freq = 44100;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 1024;
    want.callback = nullptr;
    SDL_AudioDeviceID audioDev = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    if (audioDev == 0) SDL_Log("audio non disponibile: %s (si gioca senza suono)", SDL_GetError());
    else SDL_Log("audio: %d Hz, %d canali, blocco %d campioni", have.freq, have.channels, have.samples);
    const u32 audioFrameBytes = (u32)(sgl::Game::AUDIO_FRAME_SAMPLES * 2 * sizeof(sgl::i16));
    // Precarica un frame di silenzio: assorbe le piccole irregolarita' del primo avvio.
    std::vector<sgl::i16> silenzio((size_t)sgl::Game::AUDIO_FRAME_SAMPLES * 2, 0);
    if (audioDev) {
        SDL_QueueAudio(audioDev, silenzio.data(), audioFrameBytes);
        SDL_PauseAudioDevice(audioDev, 0);
    }

    sgl::Game game(20261015u); // seme fisso: deterministico

    // Cartelloni sponsor: decodificati qui (runner, SDL2_image) e consegnati al
    // core PRIMA di init(), che li posiziona bordo strada in modo deterministico.
    auto sponsorSprites = sgl::caricaSponsorSprite("GRAFICA/sponsor"); // (sponsor_id, Sprite) per abbinamento a livello.json
    game.setSponsorSprites(std::move(sponsorSprites));

    // Auto rivali: sprite veri dal catalogo (raw prodotti da strumenti_py/png_a_raw.py),
    // consegnati al core prima di avviaGara(). Senza i raw si mantiene la sagoma sintetica (RIV punto b).
    std::vector<Sprite> rivaliSprites;
    // Un'auto per rivale (Andrea 29/10), stesso ordine di png_a_raw.py e prova_gate.cpp.
    static const char* kRivaleRetro[7] = { "rosso", "azzurro", "giallo", "verde", "bianco", "arancio", "viola" };
    for (const char* colore : kRivaleRetro) {
        Sprite s = caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/rivali/auto_rivale_") + colore + "_retro.sglraw");
        if (s.width > 0) rivaliSprites.push_back(std::move(s));
        else std::printf("rivali: raw mancante auto_rivale_%s_retro.sglraw\n", colore);
    }
    game.setRivaliSprites(std::move(rivaliSprites));

    // Traffico comune (Andrea 27/10): veicoli civili, stessi raw di png_a_raw.py.
    std::vector<Sprite> trafficoSprites;
    static const char* kTraffico[4] = { "utilitaria_bianca", "furgone_grigio", "berlina_blu", "camioncino_beige" };
    for (const char* nome : kTraffico) {
        Sprite s = caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/traffico/traffico_") + nome + "_retro.sglraw");
        if (s.width > 0) trafficoSprites.push_back(std::move(s));
        else std::printf("traffico: raw mancante traffico_%s_retro.sglraw\n", nome);
    }
    game.setTrafficoSprites(std::move(trafficoSprites));

    // Default: gara a tappe con bivi (dati/percorso_gara.json). --livello <id>:
    // una sola pista che gira in tondo, per provarla.
    // Partenza cinematografica (Andrea 08/11): griglia, gruppi di supporto,
    // 3-2-1-START. Vale per ogni gara avviata da qui o dal menu.
    game.abilitaPartenza(true);
    if (!opz.livello.empty()) game.init(opz.livello);
    else if (!game.avviaGara()) game.init();
    // Blocco R2: senza --livello e --subito si parte da Splash -> Menu
    // (corsa singola, gara a tappe, esci); poi Gara -> FineTempo -> Risultati -> Menu.
    if (opz.livello.empty() && !opz.subito) game.mostraMenu();
    std::printf("cartelloni sponsor sulla pista: %zu\n", game.numeroCartelloni());

    Controller ctrl;
    ctrl.apri();

    FiltroCrt crt;
    crt.acceso = opz.crt >= 0 ? opz.crt == 1 : leggiFiltroCrtDaConfig();
    SDL_Log("filtro CRT %s (tasto C per cambiarlo)", crt.acceso ? "acceso" : "spento");

    // Passo fisso: la logica avanza di 1/frequenza s (config.json, 60 Hz di
    // default), il video si presenta a ogni refresh. L'accumulatore decide
    // quanti tick servono. Prima qui c'era 50 fisso mentre il core usa dt=1/60:
    // il gioco andava all'83% e l'audio (735 campioni a tick) restava a corto.
    const u64 freq = SDL_GetPerformanceFrequency();
    const u64 tickLen = freq / (u64)game.frequenzaHz();
    // Byte di un tick di audio (735 campioni stereo a 60 Hz, 882 a 50 Hz).
    const u32 tickAudioBytes = (u32)(44100 / game.frequenzaHz()) * 2 * (u32)sizeof(sgl::i16);
    u64 attese = 0; // tick rimandati perche' la coda audio era piena (log)
    u64 prev = SDL_GetPerformanceCounter();
    u64 acc = tickLen; // primo frame: un tick subito
    u64 heldPrev = 0;
    bool rPrev = false;
    u64 frames = 0;
    MisuraRunner misura;
    misura.attiva = opz.misura;
    if (misura.attiva) misura.avvia(prev, freq, game.frequenzaHz());
    bool running = true;
    bool aggiornaTexture = false;
    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
#ifndef __EMSCRIPTEN__ // nel browser Esc serve a uscire dallo schermo intero
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = false;
#endif
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_F11) {
                const bool fs = (SDL_GetWindowFlags(win) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
                SDL_SetWindowFullscreen(win, fs ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                SDL_ShowCursor(fs ? SDL_ENABLE : SDL_DISABLE);
            }
            if (ev.type == SDL_KEYDOWN && !ev.key.repeat && ev.key.keysym.scancode == SDL_SCANCODE_C) {
                crt.acceso = !crt.acceso;
                aggiornaTexture = true; // si ricopia subito il fotogramma, anche in pausa
                SDL_Log("filtro CRT %s", crt.acceso ? "acceso" : "spento");
            }
            if (ev.type == SDL_CONTROLLERDEVICEADDED || ev.type == SDL_JOYDEVICEADDED) ctrl.apri();
            if (ev.type == SDL_CONTROLLERDEVICEREMOVED || ev.type == SDL_JOYDEVICEREMOVED) {
                ctrl.chiudi();
                ctrl.apri();
            }
        }

        const u64 now = SDL_GetPerformanceCounter();
        acc += now - prev;
        prev = now;
        if (acc > tickLen * MAX_TICK_PER_FRAME) acc = tickLen * MAX_TICK_PER_FRAME;

        // Passo del tick corretto di poco secondo la coda audio (vedi sopra).
        u64 passo = tickLen;
        if (audioDev) {
            const u32 inCoda = SDL_GetQueuedAudioSize(audioDev) / tickAudioBytes;
            if (inCoda > AUDIO_FRAME_OBIETTIVO) passo = tickLen + (u64)((f64)tickLen * AUDIO_CORREZIONE_PASSO);
            else if (inCoda < AUDIO_FRAME_OBIETTIVO) passo = tickLen - (u64)((f64)tickLen * AUDIO_CORREZIONE_PASSO);
        }

        bool aggiornato = false;
        bool attesaAudio = false;
        while (acc >= passo && running) {
            // Coda piena: il tick aspetta il prossimo giro invece di buttare musica.
            if (audioDev && SDL_GetQueuedAudioSize(audioDev) >= tickAudioBytes * AUDIO_MAX_FRAME_IN_CODA) {
                ++attese;
                attesaAudio = true;
                break;
            }
            acc -= passo;

            // Input del tick con i fronti (pressed/released) calcolati in un solo punto.
            const Uint8* kb = SDL_GetKeyboardState(nullptr);
            sgl::Pad pad;
            pad.held = leggiPadHeld(kb, ctrl, pad.axisLX, pad.axisLY);
            if (opz.pilotaAuto) {
                // Solo per le misure: gas sempre, sterzo verso il centro quando l'auto si allarga
                // (stessa regola di prova_gate e misura_pi). Da usare con --subito: i menu restano a mano.
                pad.held |= Pad::A;
                const f32 x = game.state().playerX;
                if (x > 120.0f) pad.held |= Pad::Left;
                else if (x < -120.0f) pad.held |= Pad::Right;
            }
            pad.pressed = pad.held & ~heldPrev;
            pad.released = heldPrev & ~pad.held;
            heldPrev = pad.held;
            game.setInput(pad);

            // Uscita dal pad (sul Pi spesso non c'e' tastiera): Select + Start.
#ifndef __EMSCRIPTEN__
            if ((pad.held & (Pad::Select | Pad::Start)) == (Pad::Select | Pad::Start)) running = false;
#endif

            // Tasto R (tastiera) oppure Start+B (pad): prossimo brano radio,
            // solo al fronte (tenendo premuto non salta piu' brani).
            const bool rPressed = (kb[SDL_SCANCODE_R] != 0) && !rPrev;
            rPrev = (kb[SDL_SCANCODE_R] != 0);
            const u64 startB = Pad::Start | Pad::B;
            const bool startBNuovo = (pad.held & startB) == startB && (pad.pressed & startB) != 0;
            if (rPressed || startBNuovo) {
                game.nextBranoRadio();
                SDL_Log("radio: prossimo brano di gara");
            }

            const u64 tickInizio = misura.attiva ? SDL_GetPerformanceCounter() : 0;
            game.update();
#ifdef __EMSCRIPTEN__
            if (game.uscitaRichiesta()) { webEsci(); running = false; } // ESCI: la pagina decide dove andare
            if (frames % 300 == 0) webScriviSalvataggi();
#else
            if (game.uscitaRichiesta()) running = false; // voce ESCI del menu
#endif
            // Render a ogni tick, come prima: il framebuffer (e quindi frameHash)
            // dipende solo dai tick, non da quante volte lo schermo si aggiorna.
            game.render();
            if (misura.attiva) misura.tick(MisuraRunner::ms(tickInizio, SDL_GetPerformanceCounter(), freq));
            aggiornato = true;

            // Audio del tick in coda: sempre, cosi' la musica non salta mai avanti.
            if (audioDev) {
                const auto& ab = game.audio();
                SDL_QueueAudio(audioDev, ab.data(), (Uint32)(ab.size() * sizeof(sgl::i16)));
            }

            ++frames;
            if (frames % 60 == 0 && !misura.attiva) {
                SDL_Log("frame %llu  hash=%08x  speed=%.1f x=%.0f offroad=%d  audio attese=%llu",
                        (unsigned long long)frames, game.frameHash(),
                        game.state().playerSpeed, game.state().playerX,
                        game.state().playerOffroad ? 1 : 0, (unsigned long long)attese);
            }
        }

        const u64 copiaInizio = misura.attiva ? SDL_GetPerformanceCounter() : 0;
        const bool copiato = aggiornato || aggiornaTexture;
        if (aggiornato || aggiornaTexture) {
            aggiornaTexture = false;
            // Copia il framebuffer software nella texture SDL (con il filtro
            // CRT acceso passa prima dalla sbavatura del composito).
            void* px = nullptr;
            int pitch = 0;
            if (SDL_LockTexture(tex, nullptr, &px, &pitch) == 0) {
                const sgl::u32* src = game.fb().data();
                if (crt.acceso) {
                    crtSbava(src, crt.sbavato.data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
                    src = crt.sbavato.data();
                }
                auto* dst = static_cast<Uint8*>(px);
                for (i32 y = 0; y < sgl::Framebuffer::HEIGHT; ++y)
                    SDL_memcpy(dst + (size_t)y * pitch, src + y * sgl::Framebuffer::WIDTH,
                               sgl::Framebuffer::WIDTH * 4);
                SDL_UnlockTexture(tex);
            }
        }

        const u64 presentaInizio = misura.attiva ? SDL_GetPerformanceCounter() : 0;
        SDL_SetRenderDrawColor(sdlRen, 0, 0, 0, 255);
        SDL_RenderClear(sdlRen);
        SDL_RenderCopy(sdlRen, tex, nullptr, nullptr);
        if (crt.acceso) {
            crt.preparaMaschera(sdlRen);
            if (crt.maschera) SDL_RenderCopy(sdlRen, crt.maschera, nullptr, nullptr);
        }
        SDL_RenderPresent(sdlRen); // con vsync attende il refresh dello schermo
        if (misura.attiva) {
            const u64 ora = SDL_GetPerformanceCounter();
            misura.giro(ora, copiato, MisuraRunner::ms(copiaInizio, presentaInizio, freq),
                        MisuraRunner::ms(presentaInizio, ora, freq));
            misura.stampaFinestra(ora, attese);
            if (opz.misuraSecondi > 0 &&
                MisuraRunner::ms(misura.inizio, ora, freq) >= opz.misuraSecondi * 1000.0)
                running = false;
        }
#ifdef __EMSCRIPTEN__
        webAttendiFotogramma(); // il browser disegna e gestisce tastiera/pad
        continue;
#endif

        // Senza vsync il ciclo dorme fino al prossimo tick (non consuma CPU a vuoto).
        if (!vsync && attesaAudio) SDL_Delay(1); // la scheda audio deve ancora suonare la coda
        else if (!vsync) {
            const u64 dopo = SDL_GetPerformanceCounter();
            const u64 accOra = acc + (dopo - prev);
            if (accOra < tickLen) {
                const u32 ms = (u32)((tickLen - accOra) * 1000 / freq);
                if (ms > 1) SDL_Delay(ms - 1);
            }
        }
    }

    if (misura.attiva) misura.riassunto(SDL_GetPerformanceCounter(), attese);
    if (audioDev) SDL_CloseAudioDevice(audioDev);
    ctrl.chiudi();
    crt.chiudi();
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(sdlRen);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
