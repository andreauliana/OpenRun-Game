// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include <vector>

namespace sgl {

// Sprite software: bitmap indicizzata.
// Indice 0 = trasparente. I colori vengono da Palette del motore
// (paletteBase) oppure, se non vuota, dalla palette RGBA di proprietà (usata
// per gli sprite caricati da asset esterni, es. cartelloni sponsor).
struct Sprite {
    i32 width = 0;
    i32 height = 0;
    std::vector<u8> pixels;      // indice di palette per pixel
    std::vector<u32> palette;    // colori propri 0xAARRGGBB; se vuoto si usa pal_ + paletteBase
    i32 paletteBase = 0;         // offset in pal_ (ignored se palette non vuota)
};

} // namespace sgl
