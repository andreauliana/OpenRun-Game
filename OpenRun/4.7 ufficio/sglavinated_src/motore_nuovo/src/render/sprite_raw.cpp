// SPDX-License-Identifier: MIT
#include "render/sprite_raw.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <vector>

namespace sgl {

namespace {

u32 leggiU32LE(const u8* p) {
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

// Distanza quadrata tra due colori RGB.
i32 dist2(u32 a, u32 b) {
    i32 dr = (i32)((a >> 16) & 0xFF) - (i32)((b >> 16) & 0xFF);
    i32 dg = (i32)((a >> 8) & 0xFF) - (i32)((b >> 8) & 0xFF);
    i32 db = (i32)(a & 0xFF) - (i32)(b & 0xFF);
    return dr * dr + dg * dg + db * db;
}

} // namespace

Sprite caricaSpriteRaw(const std::string& path) {
    Sprite s;
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return s;
    // Lettura header e pixel; il file è chiuso in ogni uscita.
    bool ok = false;
    char magic[4] = {0, 0, 0, 0};
    if (std::fread(magic, 1, 4, f) == 4 && std::memcmp(magic, "SGLR", 4) == 0) {
        u8 dim[8];
        if (std::fread(dim, 1, 8, f) == 8) {
            u32 w = leggiU32LE(dim), h = leggiU32LE(dim + 4);
            if (w > 0 && h > 0 && w < 4096 && h < 4096) {
                size_t npix = (size_t)w * (size_t)h;
                std::vector<u8> rgba(npix * 4);
                if (std::fread(rgba.data(), 1, npix * 4, f) == npix * 4) {
                    ok = true;
                    s.width = (i32)w;
                    s.height = (i32)h;
                    s.pixels.assign(npix, (u8)0);
                    s.palette.push_back(0x00000000u); // indice 0: trasparente
                    for (size_t i = 0; i < npix; ++i) {
                        u8 r = rgba[4 * i], g = rgba[4 * i + 1], b = rgba[4 * i + 2], a = rgba[4 * i + 3];
                        u32 col = 0xFF000000u | (u32)r << 16 | (u32)g << 8 | (u32)b;
                        i32 idx = 0;
                        if (a >= 128) {
                            i32 migliore = 0;
                            i32 migliorDistanza = 0x7FFFFFFF;
                            for (i32 c = 1; c < (i32)s.palette.size(); ++c) {
                                i32 d = dist2(s.palette[c], col);
                                if (d < migliorDistanza) {
                                    migliorDistanza = d;
                                    migliore = c;
                                }
                            }
                            if (migliorDistanza > SOGLIA_NUOVO_COLORE * SOGLIA_NUOVO_COLORE && (i32)s.palette.size() < 256) {
                                s.palette.push_back(col);
                                migliore = (i32)s.palette.size() - 1;
                            }
                            idx = migliore;
                        }
                        s.pixels[i] = (u8)idx;
                    }
                }
            }
        }
    }
    std::fclose(f);
    if (!ok) {
        s = Sprite(); // vuoto in caso di errore
    }
    return s;
}

} // namespace sgl
