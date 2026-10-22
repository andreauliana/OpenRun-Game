// SPDX-License-Identifier: MIT
#include "game/game.h"
#include "runner/sponsor_loader.h" // decodifica cartelloni sponsor (solo runner)
#include <SDL2/SDL.h>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

using namespace sgl; // runner: puo' usare i tipi del nucleo senza prefisso

namespace {

constexpr i32 WINDOW_SCALE = 2; // finestra 640x448 per comodita' sul desktop
constexpr i32 TARGET_FPS = 50;  // frequenza della logica (il core e' a passo fisso 1/50 s)
constexpr i32 MAX_TICK_PER_FRAME = 5; // recupero massimo dopo un intoppo (evita la "spirale")
// Coda audio: si tengono in coda al massimo questi frame di gioco (~80 ms a 50 Hz).
// Oltre, il frame si scarta: la latenza resta bassa anche se il video rallenta.
constexpr u32 AUDIO_MAX_FRAME_IN_CODA = 4;
constexpr const char* PROJECT_DATA_MARKER =
    "motore_nuovo/dati/livelli/lv_01_neon/livello.json";

struct Opzioni {
    bool fullscreen = false;
    bool vsync = true;      // --no-vsync: per driver che bloccano la presentazione
    std::string radiceDati; // --data: cartella sglavinated_src (vuota = ricerca automatica)
    std::string livello;    // --livello: solo quella pista, in tondo (vuoto = gara a tappe)
};

Opzioni leggiOpzioni(int argc, char** argv) {
    Opzioni o;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--fullscreen") == 0) o.fullscreen = true;
        else if (std::strcmp(argv[i], "--window") == 0) o.fullscreen = false;
        else if (std::strcmp(argv[i], "--no-vsync") == 0) o.vsync = false;
        else if (std::strcmp(argv[i], "--data") == 0 && i + 1 < argc) o.radiceDati = argv[++i];
        else if (std::strcmp(argv[i], "--livello") == 0 && i + 1 < argc) o.livello = argv[++i];
    }
    return o;
}

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
    if (kb[SDL_SCANCODE_RETURN])                      held |= Pad::Start;
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
        const int nb = SDL_JoystickNumButtons(c.joy);
        if (nb > 0 && SDL_JoystickGetButton(c.joy, 0)) held |= Pad::A;
        if (nb > 1 && SDL_JoystickGetButton(c.joy, 1)) held |= Pad::B;
    }
    if (axisLX < -0.5f) held |= Pad::Left;
    if (axisLX >  0.5f) held |= Pad::Right;
    if (axisLY < -0.5f) held |= Pad::Up;
    if (axisLY >  0.5f) held |= Pad::Down;
    return held;
}

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
    auto sponsorSprites = sgl::caricaSponsorSprite("UFFICIO/Dipartimento Grafico/Libreria/assets_originali/sponsor"); // (sponsor_id, Sprite) per abbinamento a livello.json
    game.setSponsorSprites(std::move(sponsorSprites));

    // Default: gara a tappe con bivi (dati/percorso_gara.json). --livello <id>:
    // una sola pista che gira in tondo, per provarla.
    if (!opz.livello.empty()) game.init(opz.livello);
    else if (!game.avviaGara()) game.init();
    std::printf("cartelloni sponsor sulla pista: %zu\n", game.numeroCartelloni());

    Controller ctrl;
    ctrl.apri();

    // Passo fisso: la logica avanza sempre di 1/50 s (determinismo), il video si
    // presenta a ogni refresh. L'accumulatore decide quanti tick servono.
    const u64 freq = SDL_GetPerformanceFrequency();
    const u64 tickLen = freq / TARGET_FPS;
    u64 prev = SDL_GetPerformanceCounter();
    u64 acc = tickLen; // primo frame: un tick subito
    u64 heldPrev = 0;
    bool rPrev = false;
    u64 frames = 0;
    bool running = true;
    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = false;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_F11) {
                const bool fs = (SDL_GetWindowFlags(win) & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0;
                SDL_SetWindowFullscreen(win, fs ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
                SDL_ShowCursor(fs ? SDL_ENABLE : SDL_DISABLE);
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

        bool aggiornato = false;
        while (acc >= tickLen && running) {
            acc -= tickLen;

            // Input del tick con i fronti (pressed/released) calcolati in un solo punto.
            const Uint8* kb = SDL_GetKeyboardState(nullptr);
            sgl::Pad pad;
            pad.held = leggiPadHeld(kb, ctrl, pad.axisLX, pad.axisLY);
            pad.pressed = pad.held & ~heldPrev;
            pad.released = heldPrev & ~pad.held;
            heldPrev = pad.held;
            game.setInput(pad);

            // Uscita dal pad (sul Pi spesso non c'e' tastiera): Select + Start.
            if ((pad.held & (Pad::Select | Pad::Start)) == (Pad::Select | Pad::Start)) running = false;

            // Tasto R (tastiera) oppure Start+B (pad): prossimo brano radio,
            // solo al fronte (tenendo premuto non salta piu' brani).
            const bool rPressed = (kb[SDL_SCANCODE_R] != 0) && !rPrev;
            rPrev = (kb[SDL_SCANCODE_R] != 0);
            const u64 startB = Pad::Start | Pad::B;
            const bool startBNuovo = (pad.held & startB) == startB && (pad.pressed & startB) != 0;
            if (rPressed || startBNuovo) {
                game.nextBranoRadio();
                SDL_Log("radio: brano %d/%d in riproduzione", game.branoRadio() + 1, game.numBraniRadio());
            }

            game.update();
            // Render a ogni tick, come prima: il framebuffer (e quindi frameHash)
            // dipende solo dai tick, non da quante volte lo schermo si aggiorna.
            game.render();
            aggiornato = true;

            // Audio del tick in coda (scartato solo se la coda e' gia' piena).
            if (audioDev && SDL_GetQueuedAudioSize(audioDev) < audioFrameBytes * AUDIO_MAX_FRAME_IN_CODA) {
                const auto& ab = game.audio();
                SDL_QueueAudio(audioDev, ab.data(), (Uint32)(ab.size() * sizeof(sgl::i16)));
            }

            ++frames;
            if (frames % 60 == 0) {
                SDL_Log("frame %llu  hash=%08x  speed=%.1f x=%.0f offroad=%d",
                        (unsigned long long)frames, game.frameHash(),
                        game.state().playerSpeed, game.state().playerX,
                        game.state().playerOffroad ? 1 : 0);
            }
        }

        if (aggiornato) {
            // Copia il framebuffer software nella texture SDL.
            void* px = nullptr;
            int pitch = 0;
            if (SDL_LockTexture(tex, nullptr, &px, &pitch) == 0) {
                const sgl::u32* src = game.fb().data();
                auto* dst = static_cast<Uint8*>(px);
                for (i32 y = 0; y < sgl::Framebuffer::HEIGHT; ++y)
                    SDL_memcpy(dst + (size_t)y * pitch, src + y * sgl::Framebuffer::WIDTH,
                               sgl::Framebuffer::WIDTH * 4);
                SDL_UnlockTexture(tex);
            }
        }

        SDL_SetRenderDrawColor(sdlRen, 0, 0, 0, 255);
        SDL_RenderClear(sdlRen);
        SDL_RenderCopy(sdlRen, tex, nullptr, nullptr);
        SDL_RenderPresent(sdlRen); // con vsync attende il refresh dello schermo

        // Senza vsync il ciclo dorme fino al prossimo tick (non consuma CPU a vuoto).
        if (!vsync) {
            const u64 dopo = SDL_GetPerformanceCounter();
            const u64 accOra = acc + (dopo - prev);
            if (accOra < tickLen) {
                const u32 ms = (u32)((tickLen - accOra) * 1000 / freq);
                if (ms > 1) SDL_Delay(ms - 1);
            }
        }
    }

    if (audioDev) SDL_CloseAudioDevice(audioDev);
    ctrl.chiudi();
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(sdlRen);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
