// SPDX-License-Identifier: MIT
#include "audio/mixer.h"
#include <cmath>
#include <cstdio>
#include <algorithm>

namespace sgl {

Mixer::Mixer(u32 sampleRate) : sampleRate_(sampleRate) {}

i32 Mixer::loadRadio(const RadioDef& def) {
    radioLoaded_ = false;
    radioData_.clear();
    numTracks_ = 0;
    curTrack_ = 0;
    playPos_ = 0.0f;

    FILE* f = std::fopen(def.path.c_str(), "rb");
    if (!f) return 0;

    std::fseek(f, 0, SEEK_END);
    long sz = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);

    if (sz <= 0) { std::fclose(f); return 0; }

    radioData_.resize((size_t)sz);
    const size_t letti = std::fread(radioData_.data(), 1, (size_t)sz, f);
    std::fclose(f);
    if (letti != (size_t)sz) { radioData_.clear(); return 0; } // file troncato: radio muta, nessun crash

    banksPerTrack_ = def.banksPerTrack;
    radioRate_ = def.sampleRate;
    i32 bytesPerTrack = banksPerTrack_ * BANK_SIZE;
    numTracks_ = (i32)radioData_.size() / bytesPerTrack;
    if (numTracks_ <= 0) { radioData_.clear(); return 0; }

    radioLoaded_ = true;
    curTrack_ = 0;
    playPos_ = 0.0f;
    return numTracks_;
}

void Mixer::selezionaBrano(i32 indice) {
    if (!radioLoaded_ || indice < 0 || indice >= numTracks_) return;
    curTrack_ = indice;
    playPos_ = 0.0f;
}

void Mixer::nextBrano() {
    if (!radioLoaded_ || numTracks_ <= 0) return;
    curTrack_ = (curTrack_ + 1) % numTracks_;
    playPos_ = 0.0f;
}

f32 Mixer::readSample() {
    if (!radioLoaded_) return 0.0f;

    i32 trackOffset = curTrack_ * banksPerTrack_ * BANK_SIZE;
    i32 trackLen = banksPerTrack_ * BANK_SIZE;

    i32 pos0 = (i32)playPos_;
    i32 pos1 = pos0 + 1;
    f32 frac = playPos_ - (f32)pos0;

    // Wrap / next track al termine del brano
    if (pos0 >= trackLen) {
        nextBrano();
        playPos_ = 0.0f;
        pos0 = 0; pos1 = 1; frac = 0.0f;
    }
    if (pos1 >= trackLen) pos1 = pos0;

    // PCM 8-bit unsigned, centrato su 128, range 1-255 (mkradio2.py)
    f32 s0 = ((f32)radioData_[trackOffset + pos0] - 128.0f) / 128.0f;
    f32 s1 = ((f32)radioData_[trackOffset + pos1] - 128.0f) / 128.0f;

    // Interpolazione lineare per upsample
    return s0 + frac * (s1 - s0);
}

void Mixer::mix(i16* out, i32 frames, f32 engineRpm) {
    if (!enabled_) {
        for (i32 i = 0; i < frames * 2; ++i) out[i] = 0;
        return;
    }

    // Rapporto di upsample: sorgente radioRate_ -> output sampleRate_
    f32 step = (radioLoaded_ && radioRate_ > 0)
             ? (f32)radioRate_ / (f32)sampleRate_
             : 0.0f;

    // Suono motore: due oscillatori con fase accumulata (niente click tra frame).
    f32 base = 40.0f + engineRpm * 0.6f; // Hz fondamentale
    f32 amp = 3000.0f * (engineRpm / 8000.0f);
    if (amp > 12000.0f) amp = 12000.0f;

    f32 phaseInc = base / (f32)sampleRate_;

    for (i32 i = 0; i < frames; ++i) {
        // Motore sintetico (fase accumulata, deterministico)
        f32 eng = amp * (std::sin(2.0f * PI * phase_) +
                         0.4f * std::sin(2.0f * PI * phase_ * 2.0f));

        // Musica radio
        f32 mus = 0.0f;
        if (radioLoaded_) {
            mus = readSample() * 16000.0f; // scala a int16 range
            playPos_ += step;
        }

        // Mix finale
        f32 v = eng * engineVol_ + mus * musicVol_;
        i16 s = (i16)std::max(-32000.0f, std::min(32000.0f, v));
        out[i * 2]     = s; // L
        out[i * 2 + 1] = s; // R (mono -> stereo)

        phase_ += phaseInc;
        if (phase_ > 1.0f) phase_ -= 1.0f;
    }
}

} // namespace sgl
