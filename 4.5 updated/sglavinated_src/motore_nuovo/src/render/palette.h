// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include <array>
#include <vector>

namespace sgl {

// Palette indicizzata: 256 colori 0xAARRGGBB.
// I colori sono definiti a mano (nessun asset esterno).
class Palette {
public:
    Palette();
    u32 operator[](i32 idx) const { return colors_[idx & 255]; }
    i32 size() const { return 256; }
private:
    std::array<u32, 256> colors_;
};

} // namespace sgl
