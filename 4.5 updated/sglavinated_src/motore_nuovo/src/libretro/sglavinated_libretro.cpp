// SPDX-License-Identifier: MIT
// Core libretro per "Sglavinated" (motore nuovo, pista C).
// Interfaccia: retro_init, retro_load_game, retro_run (un frame per chiamata),
// video una volta per chiamata, audio int16 stereo, input RetroPad.
// Riferimento: https://docs.libretro.com/development/cores/developing-cores/
//
// NOTA (stub, BLOCCO C): i header libretro (retro_common.h, libretro.h) non sono
// ancora nella cartella del progetto. Questo file espone le firme previste e
// delega tutta la logica a sgl::Game (core indipendente dalla piattaforma).
// Quando i header saranno disponibili, bastera' decommentare e collegare:
//   - retro_get_system_info / retro_set_environment / retro_set_video_refresh
//   - retro_set_audio_sample / retro_set_audio_sample_batch
//   - retro_set_input_poll / retro_set_input_state
//   - retro_run: game.setInput(pad) -> game.update() -> video refresh fb()
//                 -> audio batch mix()
// Nessuna dipendenza da SDL nel core.

#include "game/game.h"

extern "C" {

static sgl::Game* g_game = nullptr;

unsigned retro_api_version(void) { return RETRO_API_VERSION; }

void retro_get_system_info(struct retro_system_info* info) {
    if (!info) return;
    info->library_name = "Sglavinated (motore nuovo)";
    info->library_version = "v90";
    info->valid_extensions = "bin";
    info->need_fullpath = false;
    info->block_extract = false;
}

void retro_get_system_av_info(struct retro_system_av_info* info) {
    if (!info) return;
    info->timing.fps = 50.0;
    info->timing.sample_rate = 44100.0;
    info->geometry.base_width = sgl::Framebuffer::WIDTH;
    info->geometry.base_height = sgl::Framebuffer::HEIGHT;
    info->geometry.max_width = sgl::Framebuffer::WIDTH;
    info->geometry.max_height = sgl::Framebuffer::HEIGHT;
    info->geometry.aspect = (float)sgl::Framebuffer::WIDTH / sgl::Framebuffer::HEIGHT;
}

void retro_init(void) {
    g_game = new sgl::Game(20261015u);
    g_game->init();
}

void retro_deinit(void) {
    delete g_game;
    g_game = nullptr;
}

void retro_reset(void) {
    if (g_game) g_game->init();
}

static void retro_run_wrapper(void) {
    if (!g_game) return;
    // input, update, render sono gestiti dal frontend libretro tramite callback;
    // qui si espone lo schema. Stub: nessun callback collegato ancora.
    (void)g_game;
}

void retro_run(void) {
    retro_run_wrapper();
}

bool retro_load_game(const struct retro_game_info* info) {
    (void)info;
    return true; // nessun asset esterno richiesto dal core
}

void retro_unload_game(void) {}

unsigned retro_get_region(void) { return RETRO_REGION_PAL; } // 50 Hz

void* retro_get_memory_data(unsigned id) { (void)id; return nullptr; }
size_t retro_get_memory_size(unsigned id) { (void)id; return 0; }

// Input RetroPad: mappatura da retro_input_state_t a sgl::Pad (da collegare).
int16_t retro_input_state_t(unsigned port, unsigned device, unsigned index, unsigned id) {
    (void)port; (void)device; (void)index; (void)id;
    return 0;
}

} // extern "C"
