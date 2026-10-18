// SPDX-License-Identifier: MIT
#include "render/palette.h"

namespace sgl {

Palette::Palette() {
    // Palette base notturna/sci-fi, tutta costruita a mano.
    // Indici riservati: 0 nero, 1 bianco, 2-7 grigi strada/rumble, 8-15 UI/HUD.
    u32 c[256];
    for (i32 i = 0; i < 256; ++i) c[i] = 0xFF000000;

    c[0]  = 0xFF000000; // nero
    c[1]  = 0xFFFFFFFF; // bianco
    c[2]  = 0xFF7A7A7A; // asfalto chiaro
    c[3]  = 0xFF5A5A5A; // asfalto scuro
    c[4]  = 0xFF3A3A3A; // bordo strada
    c[5]  = 0xFFFFFFFF; // rumble chiaro
    c[6]  = 0xFFFF00FF; // rumble scuro (magenta)
    c[7]  = 0xFF202040; // cielo notturno
    c[8]  = 0xFF00AA00; // erba/bordo
    c[9]  = 0xFFFF8800; // HUD arancio
    c[10] = 0xFF00FFFF; // HUD ciano
    c[11] = 0xFFFFFF00; // HUD giallo
    c[12] = 0xFFFF0000; // rosso pericolo
    c[13] = 0xFF00FF00; // verde ok
    c[14] = 0xFF444444; // ombra
    c[15] = 0xFFAAAAAA; // grigio chiaro UI

    for (i32 i = 16; i < 256; ++i) {
        // gradienti sintetici per sfondo / sprite futuri
        u8 v = (u8)((i * 7) & 255);
        c[i] = 0xFF000000 | (u32)((v * 17) << 16) | (u32)((v * 31) << 8) | (u32)(v * 13);
    }
    colors_.fill(0);
    for (i32 i = 0; i < 256; ++i) colors_[i] = c[i];
}

} // namespace sgl
