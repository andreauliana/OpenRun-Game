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
// Cartelloni sponsor: caricati dai raw .sglraw in motore_nuovo/dati/sprite_raw/sponsor/
// (la decodifica PNG resta solo nel runner, senza SDL2_image nel core).

#include "libretro/libretro.h"
#include "game/game.h"
#include "render/sprite_raw.h"

#include <cstdarg>
#include <cstdio>
#include <filesystem>
#include <string>
#include <vector>

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
    // Come il runner (impostaRadice): cartella corrente = radice dati. Serve a
    // config.json (frequenza 50/60 Hz) e a salvataggi/giocatore.json (nome
    // giocatore e classifica), che leggono percorsi relativi.
    std::error_code ec;
    std::filesystem::current_path(radice, ec);
    if (ec) logInfo("cartella corrente non impostata: %s", ec.message().c_str());

    // Auto rivali e traffico: raw prodotti da strumenti_py/png_a_raw.py, come nel
    // runner. Caricati prima di avviaGara()/init() (sprite_raw e' I/O puro, senza SDL).
    std::vector<sgl::Sprite> rivaliSprites;
    static const char* kRivaleRetro[7] = { "rosso", "azzurro", "giallo", "verde", "bianco", "arancio", "viola" };
    for (const char* colore : kRivaleRetro) {
        sgl::Sprite s = sgl::caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/rivali/auto_rivale_") + colore + "_retro.sglraw");
        if (s.width > 0) rivaliSprites.push_back(std::move(s));
    }
    g_game->setRivaliSprites(std::move(rivaliSprites));
    std::vector<sgl::Sprite> trafficoSprites;
    static const char* kTraffico[4] = { "utilitaria_bianca", "furgone_grigio", "berlina_blu", "camioncino_beige" };
    for (const char* nome : kTraffico) {
        sgl::Sprite s = sgl::caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/traffico/traffico_") + nome + "_retro.sglraw");
        if (s.width > 0) trafficoSprites.push_back(std::move(s));
    }
    g_game->setTrafficoSprites(std::move(trafficoSprites));
    // Auto del giocatore (P1 punto 4): raw vero se presente, altrimenti sagoma sintetica.
    sgl::Sprite giocatore = sgl::caricaSpriteRaw("motore_nuovo/dati/sprite_raw/giocatore/auto_giocatore.sglraw");
    if (giocatore.width > 0) g_game->impostaGiocatoreSprite(std::move(giocatore));
    // Cartelloni sponsor: raw .sglraw (chiave "sponsor_id/stile"), come fallback del
    // runner; decodifica PNG non disponibile nel core.
    {
        std::vector<std::pair<std::string, sgl::Sprite>> sponsorSprites;
        std::error_code sec;
        for (const auto& e : std::filesystem::directory_iterator(
                 "motore_nuovo/dati/sprite_raw/sponsor", sec)) {
            if (sec || !e.is_regular_file()) continue;
            const auto& p = e.path();
            if (p.extension() != ".sglraw") continue;
            const std::string stem = p.stem().string();
            const auto pos = stem.rfind('_');
            if (pos == std::string::npos) continue;
            const std::string id = stem.substr(0, pos);
            const std::string stile = stem.substr(pos + 1);
            sgl::Sprite s = sgl::caricaSpriteRaw(p.string());
            if (s.width > 0) sponsorSprites.emplace_back(id + "/" + stile, std::move(s));
        }
        if (!sponsorSprites.empty()) g_game->setSponsorSprites(std::move(sponsorSprites));
    }

    g_game->abilitaPartenza(true); // griglia e 3-2-1-START (Andrea 08/11)
    if (!g_game->avviaGara()) g_game->init(); // gara a tappe; senza percorso_gara.json pista singola
    // Flusso come il runner senza --subito: Splash -> Menu -> (voce del menu)
    // Gara -> FineTempo -> Risultati -> Menu. mostraMenu() parte da Splash;
    // avviaGara() ha gia' preparato la gara che il menu avviera'.
    g_game->mostraMenu();
    g_heldPrev = 0;
    // Frequenza da config.json (50/60 Hz): dopo il caricamento del gioco e'
    // nota; si aggiorna il timing del frontend se diverso dal default.
    if (env_cb) {
        retro_system_av_info av{};
        retro_get_system_av_info(&av);
        env_cb(RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO, &av);
    }
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
    // Frequenza logica 50/60 Hz da config.json (leggida in Game::init): il
    // frontend chiama retro_run a quella frequenza e il core genera
    // 44100/frequenza campioni audio per chiamata, quindi il tempo resta
    // uguale alle due frequenze. Senza gara caricata: 60 Hz di default.
    const double fps = g_game ? (double)g_game->frequenzaHz() : 60.0;
    info->timing.fps = fps;
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
    g_game->mostraMenu(); // reset come all'avvio: Splash -> Menu
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

RETRO_API unsigned retro_get_region(void) {
    // PAL (50 Hz) se config.json chiede 50, NTSC altrimenti.
    return (g_game && g_game->frequenzaHz() == 50) ? RETRO_REGION_PAL : RETRO_REGION_NTSC;
}

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
