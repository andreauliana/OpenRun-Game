// SPDX-License-Identifier: MIT
#include "game/game.h"
#include "runner/sponsor_loader.h" // decodifica cartelloni sponsor (solo runner)
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>

using namespace sgl; // runner: puo' usare i tipi del nucleo senza prefisso

namespace {

constexpr i32 WINDOW_SCALE = 2; // finestra 640x448 per comodita' sul desktop
constexpr i32 TARGET_FPS = 50;
constexpr u32 FRAME_MS = 1000 / TARGET_FPS;

sgl::Pad sdlToPad(const Uint8* kb, SDL_Joystick* joy) {
    sgl::Pad p;
    p.pressed = 0; p.held = 0; p.released = 0; // runner semplice: usa held come pressed
    if (kb[SDL_SCANCODE_UP] || kb[SDL_SCANCODE_W])    p.held |= sgl::Pad::Up;
    if (kb[SDL_SCANCODE_DOWN] || kb[SDL_SCANCODE_S])  p.held |= sgl::Pad::Down;
    if (kb[SDL_SCANCODE_LEFT] || kb[SDL_SCANCODE_A])  p.held |= sgl::Pad::Left;
    if (kb[SDL_SCANCODE_RIGHT] || kb[SDL_SCANCODE_D]) p.held |= sgl::Pad::Right;
    if (kb[SDL_SCANCODE_Z] || kb[SDL_SCANCODE_LCTRL]) p.held |= sgl::Pad::A;   // accelera
    if (kb[SDL_SCANCODE_X] || kb[SDL_SCANCODE_LALT])  p.held |= sgl::Pad::B;   // razzo
    if (kb[SDL_SCANCODE_RETURN])                      p.held |= sgl::Pad::Start;
    if (joy) {
        Sint16 ax = SDL_JoystickGetAxis(joy, 0);
        Sint16 ay = SDL_JoystickGetAxis(joy, 1);
        p.axisLX = ax / 32768.0f;
        p.axisLY = ay / 32768.0f;
        if (p.axisLX < -0.5f) p.held |= sgl::Pad::Left;
        if (p.axisLX >  0.5f) p.held |= sgl::Pad::Right;
        if (p.axisLY < -0.5f) p.held |= sgl::Pad::Up;
        if (p.axisLY >  0.5f) p.held |= sgl::Pad::Down;
        for (int b = 0; b < SDL_JoystickNumButtons(joy); ++b)
            if (SDL_JoystickGetButton(joy, b)) {
                if (b == 0) p.held |= sgl::Pad::A;
                if (b == 1) p.held |= sgl::Pad::B;
            }
    }
    return p;
}

SDL_AudioDeviceID g_audioDev = 0;
std::vector<sgl::i16> g_audioFrame; // un frame di audio pronto (stereo interleaved)
bool g_audioReady = false;
SDL_mutex* g_audioMutex = nullptr;

u64 g_padHeldPrev = 0;   // stato pad del frame precedente (fronte di salita)
bool g_rPrev = false;    // stato tasto R del frame precedente

void audioCallback(void* userdata, Uint8* stream, int len) {
    (void)userdata;
    // Silenzio di default: parti sempre da un buffer azzerato.
    SDL_memset(stream, 0, (size_t)len);
    SDL_LockMutex(g_audioMutex);
    if (g_audioReady) {
        // Byte realmente disponibili nel frame audio (stereo interleaved i16).
        const size_t avail = g_audioFrame.size() * sizeof(sgl::i16);
        const size_t n = (avail < (size_t)len) ? avail : (size_t)len;
        if (n > 0) SDL_memcpy(stream, g_audioFrame.data(), n);
        g_audioReady = false; // frame consumato (il resto di stream resta a zero)
    }
    SDL_UnlockMutex(g_audioMutex);
}

} // namespace

int main(int, char**) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        SDL_Log("SDL_Init fallita: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Sglavinated — motore nuovo (prototipo)",
                                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                       sgl::Framebuffer::WIDTH * WINDOW_SCALE,
                                       sgl::Framebuffer::HEIGHT * WINDOW_SCALE,
                                       SDL_WINDOW_SHOWN);
    if (!win) { SDL_Log("finestra fallita: %s", SDL_GetError()); return 1; }
    SDL_Renderer* sdlRen = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* tex = SDL_CreateTexture(sdlRen, SDL_PIXELFORMAT_ARGB8888,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         sgl::Framebuffer::WIDTH, sgl::Framebuffer::HEIGHT);

    g_audioMutex = SDL_CreateMutex();
    SDL_AudioSpec want{}, have{};
    want.freq = 44100;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = sgl::Game::AUDIO_FRAME_SAMPLES / 2; // frame stereo (SDL conta per canale)
    want.callback = audioCallback;
    g_audioDev = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
    SDL_PauseAudioDevice(g_audioDev, 0);

    sgl::Game game(20261015u); // seme fisso: deterministico

    // Cartelloni sponsor: decodificati qui (runner, SDL2_image) e consegnati al
    // core PRIMA di init(), che li posiziona bordo strada in modo deterministico.
    auto sponsorSprites = sgl::caricaSponsorSprite("assets_originali/sponsor"); // (sponsor_id, Sprite) per abbinamento a livello.json
    game.setSponsorSprites(std::move(sponsorSprites));

    game.init();

    bool running = true;
    SDL_Joystick* joy = nullptr;
    if (SDL_NumJoysticks() > 0) joy = SDL_JoystickOpen(0);

    u32 nextTick = SDL_GetTicks();
    u64 frames = 0;
    while (running) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) running = false;
            if (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                running = false;
        }
        const Uint8* kb = SDL_GetKeyboardState(nullptr);
        sgl::Pad pad = sdlToPad(kb, joy);
        game.setInput(pad);

        // Tasto R (tastiera) oppure Start+B (RetroPad): prossimo brano radio,
        // solo al fronte di salita (tenendo premuto non salta piu' brani).
        bool rPressed = (kb[SDL_SCANCODE_R] != 0) && !g_rPrev;
        g_rPrev = (kb[SDL_SCANCODE_R] != 0);
        bool startBPrev = (g_padHeldPrev & (sgl::Pad::Start | sgl::Pad::B)) == (sgl::Pad::Start | sgl::Pad::B);
        g_padHeldPrev = pad.held;
        bool startB = (pad.held & (sgl::Pad::Start | sgl::Pad::B)) == (sgl::Pad::Start | sgl::Pad::B);
        if (rPressed || (startB && !startBPrev)) {
            game.nextBranoRadio();
            SDL_Log("radio: brano %d/%d in riproduzione", game.branoRadio() + 1, game.numBraniRadio());
        }

        game.update();
        game.render();

        // Audio: rendi disponibile il frame appena mixato al callback SDL.
        {
            const auto& ab = game.audio();
            SDL_LockMutex(g_audioMutex);
            g_audioFrame.assign(ab.begin(), ab.end());
            g_audioReady = true;
            SDL_UnlockMutex(g_audioMutex);
        }

        // Copia il framebuffer software nella texture SDL.
        void* px = nullptr;
        int pitch = 0;
        SDL_LockTexture(tex, nullptr, &px, &pitch);
        const sgl::u32* src = game.fb().data();
        sgl::u32* dst = (sgl::u32*)px;
        for (i32 y = 0; y < sgl::Framebuffer::HEIGHT; ++y) {
            SDL_memcpy(dst + y * (pitch / 4), src + y * sgl::Framebuffer::WIDTH,
                       sgl::Framebuffer::WIDTH * 4);
        }
        SDL_UnlockTexture(tex);

        SDL_RenderClear(sdlRen);
        SDL_RenderCopy(sdlRen, tex, nullptr, nullptr);
        SDL_RenderPresent(sdlRen);

        // Fixed timestep 50 fps (solo il runner usa l'orologio di sistema; il core no).
        u32 now = SDL_GetTicks();
        i32 wait = (i32)(nextTick - now);
        if (wait > 0) SDL_Delay((u32)wait);
        nextTick += FRAME_MS;
        if (SDL_GetTicks() > nextTick + 100) nextTick = SDL_GetTicks() + FRAME_MS;

        ++frames;
        if (frames % 60 == 0) {
            SDL_Log("frame %llu  hash=%08x  speed=%.1f x=%.0f offroad=%d",
                    (unsigned long long)frames, game.frameHash(),
                    game.state().playerSpeed, game.state().playerX,
                    game.state().playerOffroad ? 1 : 0);
        }
    }

    SDL_CloseAudioDevice(g_audioDev);
    if (joy) SDL_JoystickClose(joy);
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(sdlRen);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
