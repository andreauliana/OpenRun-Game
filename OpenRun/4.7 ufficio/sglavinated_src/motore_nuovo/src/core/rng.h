// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

namespace sgl {

// RNG deterministico con seme (xorshift32, G. Marsaglia).
// Stesso seme = stessa sequenza su qualsiasi piattaforma: base del replay.
class Rng {
public:
    explicit Rng(u32 seed) { this->seed(seed); }
    void seed(u32 s) { state_ = (s == 0) ? 0x9E3779B9u : s; }
    u32 next() {
        state_ ^= state_ << 13;
        state_ ^= state_ >> 17;
        state_ ^= state_ << 5;
        return state_;
    }
    u32 next(u32 limit) { return next() % limit; }
    f32 next_f32() { return next() * (1.0f / 4294967296.0f); }
    u32 state() const { return state_; } // stato RNG: esposto per frameHash (determinismo/replay)
private:
    u32 state_ = 1;
};

} // namespace sgl
