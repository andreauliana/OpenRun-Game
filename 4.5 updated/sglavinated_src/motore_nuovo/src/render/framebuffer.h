// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include <array>

namespace sgl {

// Framebuffer software 320x224, 32 bit per pixel (0xAARRGGBB).
// Nessuna dipendenza da piattaforma: solo memoria.
class Framebuffer {
public:
    static constexpr i32 WIDTH = 320;
    static constexpr i32 HEIGHT = 224;

    Framebuffer() { clear(0xFF000000); }

    void clear(u32 color);
    u32  get(i32 x, i32 y) const;
    void put(i32 x, i32 y, u32 color);
    // linea orizzontale (inclusivi)
    void hline(i32 x0, i32 x1, i32 y, u32 color);
    // rettangolo riempito
    void fillRect(i32 x0, i32 y0, i32 x1, i32 y1, u32 color);

    const u32* data() const { return buf_.data(); }
    u32* data() { return buf_.data(); }

private:
    std::array<u32, WIDTH * HEIGHT> buf_;
};

} // namespace sgl
