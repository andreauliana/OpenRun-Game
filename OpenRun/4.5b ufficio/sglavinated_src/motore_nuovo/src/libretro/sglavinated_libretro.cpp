// SPDX-License-Identifier: MIT
// Core libretro per "Sglavinated" (motore nuovo, pista C).
// Interfaccia: retro_init, retro_load_game, retro_run (un frame per chiamata),
// video una volta per chiamata, audio int16 stereo, input RetroPad.
// Riferimento: https://docs.libretro.com/development/cores/developing-cores/
//
// Header: src/libretro/libretro.h (ufficiale, libretro-common, licenza MIT).
// Tutta la logica e' in sgl::Game (core indipendente dalla piattaforma);
// qui solo il collegamento ai callback del frontend. Nessuna dipendenza da SDL.
//
// Cartella dati (sglavinated_src, quella che contiene motore_nuovo/dati/):
//   1) se si carica un file come contenuto (es. motore_nuovo/dati/percorso_gara.json),
//      si risale dalla sua cartella finche' si trova il marcatore;
//   2) <cartella system di RetroArch>/sglavinated_src;
//   3) cartella corrente (e sue cartelle superiori).
// Si puo' anche avviare senza contenuto ("Avvia core"): valgono 2) e 3).
// Cartelloni sponsor: non caricati nel core (la decodifica PNG e' solo nel runner).

#include "libretro/libretro.h"
#include "game/game.h"

#include <cstdarg>
#include <cstdio>
#include <filesystem>
#include <string>

namespace {

constexpr const char* MARCATORE_DATI = "motore_nuovo/dati/livelli/lv_01_neon/livello.json";

sgl::Game* g_game = nullptr;
sgl::u64 g_heldPrev = 0;

retro_environment_t        env_cb = nullptr;
retro_video_refresh_t      video_cb = nullptr;
retro_audio_sample_t       audio_cb = nullptr;
retro_audio_sample_batch_t audio_batch_cb = nullptr;
retro_input_poll_t         input_poll_cb = nullptr;
retro_input_state_t        input_state_cb = nullptr;
retro_log_printf_t         log_cb = nullptr;

void logInfo(const char* fmt, ...) {
    char buf[512];
    va_list ap;
    va_start(ap, fmt);
    std::vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (log_cb) log_cb(RETRO_LOG_INFO, "[sglavinated] %s\n", buf);
    else std::fprintf(stderr, "[sglavinated] %s\n", buf);
}

// Risale da 'inizio' verso la radice cercando la cartella con il marcatore dati.
bool cercaRadice(std::filesystem::path inizio, std::string& out) {
    std::error_code ec;
    for (int i = 0; i < 8 && !inizio.empty(); ++i) {
        if (std::filesystem::exists(inizio / MARCATORE_DATI, ec) && !ec) {
            out = inizio.string();
            return true;
        }
        auto padre = inizio.parent_path();
        if (padre == inizio) break;
        inizio = padre;
    }
    return false;
}

std::string trovaRadiceDati(const char* percorsoContenuto) {
    std::string radice;
    std::error_code ec;
    if (percorsoContenuto && *percorsoContenuto) {
        auto p = std::filesystem::absolute(percorsoContenuto, ec);
        if (!ec && cercaRadice(p.parent_path(), radice)) return radice;
    }
    const char* sys = nullptr;
    if (env_cb && env_cb(RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY, &sys) && sys) {
        if (cercaRadice(std::filesystem::path(sys) / "sglavinated_src", radice)) return radice;
    }
    auto cwd = std::filesystem::current_path(ec);
    if (!ec && cercaRadice(cwd, radice)) return radice;
    return {};
}

// RetroPad -> sgl::Pad. I tasti seguono la posizione del runner SDL2:
// tasto in basso (RetroPad B) = acceleratore (Pad::A), tasto a destra = Pad::B.
sgl::Pad leggiPad() {
    using sgl::Pad;
    sgl::Pad pad;
    if (!input_state_cb) return pad;
    struct Mappa { unsigned id; sgl::u64 bit; };
    static const Mappa mappa[] = {
        { RETRO_DEVICE_ID_JOYPAD_UP,     Pad::Up },
        { RETRO_DEVICE_ID_JOYPAD_DOWN,   Pad::Down },
        { RETRO_DEVICE_ID_JOYPAD_LEFT,   Pad::Left },
        { RETRO_DEVICE_ID_JOYPAD_RIGHT,  Pad::Right },
        { RETRO_DEVICE_ID_JOYPAD_B,      Pad::A },     // accelera
        { RETRO_DEVICE_ID_JOYPAD_A,      Pad::B },     // razzo
        { RETRO_DEVICE_ID_JOYPAD_Y,      Pad::X },
        { RETRO_DEVICE_ID_JOYPAD_X,      Pad::Y },
        { RETRO_DEVICE_ID_JOYPAD_START,  Pad::Start },
        { RETRO_DEVICE_ID_JOYPAD_SELECT, Pad::Select },
        { RETRO_DEVICE_ID_JOYPAD_R2,     Pad::A },     // grilletto destro = accelera
        { RETRO_DEVICE_ID_JOYPAD_L2,     Pad::Down },  // grilletto sinistro = freno
    };
    sgl::u64 held = 0;
    for (const auto& m : mappa)
        if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, m.id)) held |= m.bit;

    pad.axisLX = input_state_cb(0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,
                                RETRO_DEVICE_ID_ANALOG_X) / 32768.0f;
    pad.axisLY = input_state_cb(0, RETRO_DEVICE_ANALOG, RETRO_DEVICE_INDEX_ANALOG_LEFT,
                                RETRO_DEVICE_ID_ANALOG_Y) / 32768.0f;
    if (pad.axisLX < -0.5f) held |= Pad::Left;
    if (pad.axisLX >  0.5f) held |= Pad::Right;

    pad.held = held;
    pad.pressed = held & ~g_heldPrev;
    pad.released = g_heldPrev & ~held;
    g_heldPrev = held;
    return pad;
}

bool avviaGioco(const char* percorsoContenuto) {
    delete g_game;
    g_game = new sgl::Game(20261015u); // seme fisso: deterministico, come il runner
    const std::string radice = trovaRadiceDati(percorsoContenuto);
    if (radice.empty()) {
        logInfo("cartella dati non trovata (%s): metti sglavinated_src nella cartella system "
                "di RetroArch oppure carica motore_nuovo/dati/percorso_gara.json come contenuto",
                MARCATORE_DATI);
        return false;
    }
    logInfo("cartella dati: %s", radice.c_str());
    g_game->setRadiceDati(radice);
    if (!g_game->avviaGara()) g_game->init(); // gara a tappe; senza percorso_gara.json pista singola
    g_heldPrev = 0;
    return true;
}

} // namespace

extern "C" {

RETRO_API unsigned retro_api_version(void) { return RETRO_API_VERSION; }

RETRO_API void retro_set_environment(retro_environment_t cb) {
    env_cb = cb;
    bool senzaContenuto = true;
    cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &senzaContenuto);
    retro_log_callback logging;
    if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging)) log_cb = logging.log;
}

RETRO_API void retro_set_video_refresh(retro_video_refresh_t cb) { video_cb = cb; }
RETRO_API void retro_set_audio_sample(retro_audio_sample_t cb) { audio_cb = cb; }
RETRO_API void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) { audio_batch_cb = cb; }
RETRO_API void retro_set_input_poll(retro_input_poll_t cb) { input_poll_cb = cb; }
RETRO_API void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

RETRO_API void retro_get_system_info(struct retro_system_info* info) {
    if (!info) return;
    info->library_name = "Sglavinated";
    info->library_version = "v90";
    info->valid_extensions = "json";
    info->need_fullpath = true; // serve il percorso per trovare la cartella dati
    info->block_extract = false;
}

RETRO_API void retro_get_system_av_info(struct retro_system_av_info* info) {
    if (!info) return;
    // Il core produce AUDIO_FRAME_SAMPLES campioni per frame a 44100 Hz:
    // la frequenza video dichiarata ne segue (60 Hz di default).
    info->timing.fps = 44100.0 / (double)sgl::Game::AUDIO_FRAME_SAMPLES;
    info->timing.sample_rate = 44100.0;
    info->geometry.base_width = sgl::Framebuffer::WIDTH;
    info->geometry.base_height = sgl::Framebuffer::HEIGHT;
    info->geometry.max_width = sgl::Framebuffer::WIDTH;
    info->geometry.max_height = sgl::Framebuffer::HEIGHT;
    info->geometry.aspect_ratio = 4.0f / 3.0f;
}

RETRO_API void retro_set_controller_port_device(unsigned port, unsigned device) {
    (void)port; (void)device;
}

RETRO_API void retro_init(void) {}

RETRO_API void retro_deinit(void) {
    delete g_game;
    g_game = nullptr;
}

RETRO_API void retro_reset(void) {
    if (g_game && !g_game->avviaGara()) g_game->init();
    g_heldPrev = 0;
}

RETRO_API void retro_run(void) {
    if (!g_game) return;
    if (input_poll_cb) input_poll_cb();
    const sgl::Pad pad = leggiPad();
    g_game->setInput(pad);

    // Start+B: prossimo brano radio (come nel runner), solo al fronte.
    const sgl::u64 startB = sgl::Pad::Start | sgl::Pad::B;
    if ((pad.held & startB) == startB && (pad.pressed & startB) != 0) g_game->nextBranoRadio();

    g_game->update();
    g_game->render();

    if (video_cb)
        video_cb(g_game->fb().data(), sgl::Framebuffer::WIDTH, sgl::Framebuffer::HEIGHT,
                 sgl::Framebuffer::WIDTH * sizeof(sgl::u32));

    const auto& ab = g_game->audio(); // stereo interleaved
    if (audio_batch_cb && !ab.empty()) audio_batch_cb(ab.data(), ab.size() / 2);
}

RETRO_API bool retro_load_game(const struct retro_game_info* info) {
    // Framebuffer del core: u32 0x00RRGGBB, uguale a XRGB8888.
    enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
    if (!env_cb || !env_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt)) {
        logInfo("il frontend non supporta XRGB8888");
        return false;
    }
    return avviaGioco(info ? info->path : nullptr);
}

RETRO_API bool retro_load_game_special(unsigned type, const struct retro_game_info* info, size_t num) {
    (void)type; (void)info; (void)num;
    return false;
}

RETRO_API void retro_unload_game(void) {
    delete g_game;
    g_game = nullptr;
}

RETRO_API unsigned retro_get_region(void) { return RETRO_REGION_NTSC; } // 60 Hz

// Salvataggi di stato non supportati (per ora).
RETRO_API size_t retro_serialize_size(void) { return 0; }
RETRO_API bool retro_serialize(void* data, size_t size) { (void)data; (void)size; return false; }
RETRO_API bool retro_unserialize(const void* data, size_t size) { (void)data; (void)size; return false; }
RETRO_API void retro_cheat_reset(void) {}
RETRO_API void retro_cheat_set(unsigned index, bool enabled, const char* code) {
    (void)index; (void)enabled; (void)code;
}

RETRO_API void* retro_get_memory_data(unsigned id) { (void)id; return nullptr; }
RETRO_API size_t retro_get_memory_size(unsigned id) { (void)id; return 0; }

} // extern "C"
