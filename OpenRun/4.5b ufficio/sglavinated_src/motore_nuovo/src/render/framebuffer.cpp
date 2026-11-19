// SPDX-License-Identifier: MIT
#include "render/framebuffer.h"
#include <utility>

namespace sgl {

void Framebuffer::clear(u32 color) {
    buf_.fill(color);
}

u32 Framebuffer::get(i32 x, i32 y) const {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 0;
    return buf_[y * WIDTH + x];
}

void Framebuffer::put(i32 x, i32 y, u32 color) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    buf_[y * WIDTH + x] = color;
}

void Framebuffer::hline(i32 x0, i32 x1, i32 y, u32 color) {
    if (y < 0 || y >= HEIGHT) return;
    if (x0 > x1) std::swap(x0, x1);
    if (x0 < 0) x0 = 0;
    if (x1 >= WIDTH) x1 = WIDTH - 1;
    // V9 lv_01_neon turno30: base della riga calcolata una volta, non a ogni
    // pixel (hline e' il primitivo piu' usato dal renderer: strada, cordoli,
    // pareti, linee). Stessi pixel, stessa resa.
    u32* riga = buf_.data() + (size_t)y * WIDTH;
    for (i32 x = x0; x <= x1; ++x) riga[x] = color;
}

void Framebuffer::fillRect(i32 x0, i32 y0, i32 x1, i32 y1, u32 color) {
    if (x0 > x1) std::swap(x0, x1);
    if (y0 > y1) std::swap(y0, y1);
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= WIDTH) x1 = WIDTH - 1;
    if (y1 >= HEIGHT) y1 = HEIGHT - 1;
    for (i32 y = y0; y <= y1; ++y) {
        u32* riga = buf_.data() + (size_t)y * WIDTH; // V9 lv_01_neon turno30
        for (i32 x = x0; x <= x1; ++x)
            riga[x] = color;
    }
}

} // namespace sgl
