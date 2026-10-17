// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

namespace sgl {

// Input logico tipo RetroPad (8 tasti mappabili).
// Il core conosce solo questi bit; il runner SDL2 (o il core libretro) li traduce.
struct Pad {
    enum Bit : u64 {
        Up    = 1ull << 0,
        Down  = 1ull << 1,
        Left  = 1ull << 2,
        Right = 1ull << 3,
        A     = 1ull << 4,  // acceleratore
        B     = 1ull << 5,  // razzo / azione
        X     = 1ull << 6,
        Y     = 1ull << 7,
        Start = 1ull << 8,
        Select= 1ull << 9,
    };
    u64 pressed = 0;  // tasti premuti questo frame
    u64 held    = 0;  // tasti tenuti
    u64 released= 0;  // tasti rilasciati
    f32 axisLX = 0.0f; // stick sinistro X (-1..1)
    f32 axisLY = 0.0f; // stick sinistro Y (-1..1)
};

} // namespace sgl
