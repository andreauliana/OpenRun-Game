// SPDX-License-Identifier: MIT
#include "render/crt.h"

namespace sgl {
namespace {
// Luce in piu' dopo la sbavatura (x1.25): la maschera toglie circa un quarto.
constexpr u32 LUCE_NUM = 5;
constexpr u32 LUCE_DEN_SHIFT = 4; // (somma 1-2-1 = 4x) * 5 / 16 = x1.25

// Fosforo "spento" nella sua striscia: 200/255 (sotto si vede la griglia,
// sopra l'immagine diventa troppo scura).
constexpr u32 FOSFORO_ALTRI = 200;
// Riga scura tra due linee del fascio, per scala (indice = scala, max 4+).
constexpr u32 RIGA_SCURA[5] = { 255, 255, 150, 140, 130 };

inline u32 canale(u32 l, u32 c, u32 r, u32 shift) {
    const u32 somma = ((l >> shift) & 0xFF) + 2 * ((c >> shift) & 0xFF) + ((r >> shift) & 0xFF);
    const u32 v = (somma * LUCE_NUM) >> LUCE_DEN_SHIFT;
    return (v > 255 ? 255 : v) << shift;
}
} // namespace

void crtSbava(const u32* src, u32* dst, i32 w, i32 h) {
    for (i32 y = 0; y < h; ++y) {
        const u32* s = src + (size_t)y * w;
        u32* d = dst + (size_t)y * w;
        u32 l = s[0];
        for (i32 x = 0; x < w; ++x) {
            const u32 c = s[x];
            const u32 r = x + 1 < w ? s[x + 1] : c;
            d[x] = 0xFF000000u | canale(l, c, r, 16) | canale(l, c, r, 8) | canale(l, c, r, 0);
            l = c;
        }
    }
}

std::vector<u32> crtMaschera(i32 w, i32 h, i32 scala) {
    if (scala < 1) scala = 1;
    const i32 mw = w * scala;
    const i32 mh = h * scala;
    const u32 scura = RIGA_SCURA[scala < 4 ? scala : 4];
    std::vector<u32> m((size_t)mw * mh);
    for (i32 y = 0; y < mh; ++y) {
        // Ultima riga di ogni pixel del gioco = spazio tra due linee del fascio.
        const u32 riga = (scala >= 2 && y % scala == scala - 1) ? scura : 255;
        for (i32 x = 0; x < mw; ++x) {
            const i32 fosforo = x % 3; // 0 rosso, 1 verde, 2 blu
            const u32 rr = (fosforo == 0 ? 255 : FOSFORO_ALTRI) * riga / 255;
            const u32 gg = (fosforo == 1 ? 255 : FOSFORO_ALTRI) * riga / 255;
            const u32 bb = (fosforo == 2 ? 255 : FOSFORO_ALTRI) * riga / 255;
            m[(size_t)y * mw + x] = 0xFF000000u | (rr << 16) | (gg << 8) | bb;
        }
    }
    return m;
}

} // namespace sgl
