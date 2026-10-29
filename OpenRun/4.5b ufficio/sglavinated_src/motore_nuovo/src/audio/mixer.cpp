// SPDX-License-Identifier: MIT
#include "audio/mixer.h"
#include "xmp.h"
#include <cmath>
#include <cstdio>
#include <algorithm>

namespace sgl {

namespace {
xmp_context ctx(void* p) { return static_cast<xmp_context>(p); }

// Secondi per far scendere a ~1/e livelli e battito: i canali a lungo, il battito corto.
constexpr f32 DECADIMENTO_CANALE_S = 0.18f;
constexpr f32 DECADIMENTO_BATTITO_S = 0.12f;
constexpr i32 RIGHE_PER_BATTITO = 4; // convenzione tracker: un quarto ogni 4 righe
} // namespace

Mixer::Mixer(u32 sampleRate) : sampleRate_(sampleRate) {}

Mixer::~Mixer() {
    if (!xmp_) return;
    if (suona_) { xmp_end_player(ctx(xmp_)); xmp_release_module(ctx(xmp_)); }
    xmp_free_context(ctx(xmp_));
}

bool Mixer::aggiungiBrano(const std::string& path, const std::string& titolo) {
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return false;
    Brano b;
    std::fseek(f, 0, SEEK_END);
    const long sz = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if (sz > 0) {
        b.dati.resize((size_t)sz);
        if (std::fread(b.dati.data(), 1, (size_t)sz, f) != (size_t)sz) b.dati.clear();
    }
    std::fclose(f);
    if (b.dati.empty()) return false;

    if (!xmp_) xmp_ = xmp_create_context();
    if (!xmp_) return false;
    // Prova a leggerlo subito: un file rotto non entra nella radio.
    struct xmp_test_info ti;
    if (xmp_test_module_from_memory(b.dati.data(), (long)b.dati.size(), &ti) != 0) return false;
    b.titolo = titolo.empty() ? std::string(ti.name) : titolo;
    for (char& c : b.titolo) if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A'); // font HUD maiuscolo
    brani_.push_back(std::move(b));
    if (brani_.size() == 1) { curTrack_ = 0; avviaBrano(); }
    return true;
}

const char* Mixer::titoloBrano(i32 i) const {
    return (i >= 0 && i < numTracks()) ? brani_[(size_t)i].titolo.c_str() : "";
}

void Mixer::avviaBrano() {
    if (!xmp_ || brani_.empty()) return;
    if (suona_) { xmp_end_player(ctx(xmp_)); xmp_release_module(ctx(xmp_)); suona_ = false; }
    info_ = InfoMusica{};
    rigaPrec_ = -1;
    const Brano& b = brani_[(size_t)curTrack_];
    if (xmp_load_module_from_memory(ctx(xmp_), b.dati.data(), (long)b.dati.size()) != 0) return;
    if (xmp_start_player(ctx(xmp_), (int)sampleRate_, 0) != 0) { xmp_release_module(ctx(xmp_)); return; }
    xmp_set_player(ctx(xmp_), XMP_PLAYER_INTERP, XMP_INTERP_SPLINE);
    xmp_set_player(ctx(xmp_), XMP_PLAYER_AMP, 2); // default 1: troppo bassa sotto il motore
    suona_ = true;
    struct xmp_module_info mi;
    xmp_get_module_info(ctx(xmp_), &mi);
    info_.attiva = true;
    info_.titolo = b.titolo.c_str();
    info_.canali = std::min(mi.mod->chn, InfoMusica::MAX_CANALI);
}

void Mixer::selezionaBrano(i32 indice) {
    if (indice < 0 || indice >= numTracks()) return;
    curTrack_ = indice;
    avviaBrano();
}

void Mixer::nextBrano() {
    if (numTracks() > 0) selezionaBrano((curTrack_ + 1) % numTracks());
}

void Mixer::aggiornaInfo(const i16* musica, i32 frames) {
    struct xmp_frame_info fi;
    xmp_get_frame_info(ctx(xmp_), &fi);
    const f32 dt = (f32)frames / (f32)sampleRate_;
    const f32 kCanale = std::exp(-dt / DECADIMENTO_CANALE_S);
    const f32 kBattito = std::exp(-dt / DECADIMENTO_BATTITO_S);

    info_.pattern = fi.pattern;
    info_.riga = fi.row;
    info_.bpm = fi.bpm;
    info_.battito *= kBattito;
    // Una nota tenuta resta accesa a meta' (in proporzione al volume del canale).
    for (i32 c = 0; c < info_.canali; ++c) {
        const struct xmp_channel_info& ci = fi.channel_info[c];
        info_.livello[c] = std::max(info_.livello[c] * kCanale, 0.45f * (f32)ci.volume / 64.0f);
        info_.nota[c] = (ci.volume > 0 && ci.note < XMP_MAX_KEYS) ? (i32)ci.note : -1;
    }

    // Riga nuova: ogni canale che attacca una nota si accende al suo volume.
    if (fi.row != rigaPrec_) {
        rigaPrec_ = fi.row;
        if (fi.row % RIGHE_PER_BATTITO == 0) info_.battito = 1.0f;
        for (i32 c = 0; c < info_.canali; ++c) {
            const struct xmp_channel_info& ci = fi.channel_info[c];
            if (ci.event.note > 0 && ci.event.note <= XMP_MAX_KEYS)
                info_.livello[c] = std::max(info_.livello[c], 0.35f + 0.65f * (f32)ci.volume / 64.0f);
        }
    }

    // Energia: valore medio assoluto del blocco, smorzato.
    f32 somma = 0.0f;
    for (i32 i = 0; i < frames * 2; ++i) somma += std::fabs((f32)musica[i]);
    const f32 e = std::min(1.0f, somma / (f32)(frames * 2) / 8000.0f);
    info_.energia = e > info_.energia ? e : info_.energia * kCanale + e * (1.0f - kCanale);
}

void Mixer::mix(i16* out, i32 frames, f32 engineRpm) {
    if (!enabled_) {
        for (i32 i = 0; i < frames * 2; ++i) out[i] = 0;
        return;
    }

    const i16* musica = nullptr;
    if (suona_) {
        bufMusica_.resize((size_t)frames * 2);
        // loop 0: il brano ricomincia da capo all'infinito, come la radio di OutRun.
        if (xmp_play_buffer(ctx(xmp_), bufMusica_.data(), frames * 4, 0) == 0) {
            musica = bufMusica_.data();
            aggiornaInfo(musica, frames);
        }
    }

    // Suono motore: due oscillatori con fase accumulata (niente click tra frame).
    f32 base = 40.0f + engineRpm * 0.6f; // Hz fondamentale
    f32 amp = 3000.0f * (engineRpm / 8000.0f);
    if (amp > 12000.0f) amp = 12000.0f;

    f32 phaseInc = base / (f32)sampleRate_;

    for (i32 i = 0; i < frames; ++i) {
        f32 eng = amp * (std::sin(2.0f * PI * phase_) +
                         0.4f * std::sin(2.0f * PI * phase_ * 2.0f));
        f32 l = eng * engineVol_, r = l;
        if (musica) {
            l += (f32)musica[i * 2] * musicVol_;
            r += (f32)musica[i * 2 + 1] * musicVol_;
        }
        out[i * 2]     = (i16)std::max(-32000.0f, std::min(32000.0f, l));
        out[i * 2 + 1] = (i16)std::max(-32000.0f, std::min(32000.0f, r));

        phase_ += phaseInc;
        if (phase_ > 1.0f) phase_ -= 1.0f;
    }
}

} // namespace sgl
