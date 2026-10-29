// SPDX-License-Identifier: MIT
// Caricamento cartelloni sponsor per il runner SDL2.
// Decodifica i PNG di assets_originali/sponsor/ (originali, CC0-1.0, generati da
// assets_originali/sponsor/genera_sponsor.py) in sgl::Sprite indicizzati con
// palette RGBA di proprieta'. Il core C++ non decodifica immagini: resta privo
// di SDL e di qualsiasi dipendenza da piattaforma.
#include "sponsor_loader.h"
#include "render/sprite_raw.h"
#include <cstdio>
#include <string>
#include <vector>

#ifdef SGL_HAVE_SDL2_IMAGE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

namespace sgl {

namespace {

// I 7 sponsor fittizi del catalogo e i 3 stili (neon, atomico, foglie): i nomi
// corrispondono alle sottocartelle di assets_originali/sponsor/ (v. CATALOGO_SPONSOR.json).
const char* kSponsorId[7] = {
    "curve_pericolose", "brinis_udine", "piston_high", "canna_bis",
    "slow_down", "erba_medica", "green_box"};
const char* kStile[3] = { "neon", "atomico", "foglie" };

#ifdef SGL_HAVE_SDL2_IMAGE
// Converte una superficie RGBA in Sprite indicizzato.
Sprite convertiSprite(SDL_Surface* surf) {
    Sprite s;
    s.width = surf->w;
    s.height = surf->h;
    s.pixels.assign((size_t)surf->w * (size_t)surf->h, (u8)0);
    s.palette.push_back(0x00000000u); // indice 0: trasparente

    const u8* src = static_cast<const u8*>(surf->pixels);
    const i32 pitch = surf->pitch;
    for (i32 y = 0; y < surf->h; ++y) {
        const u8* row = src + (size_t)y * (size_t)pitch;
        for (i32 x = 0; x < surf->w; ++x) {
            const u8* px = row + (size_t)x * 4; // RGBA8888 dopo SDL_ConvertSurfaceFormat
            u8 r = px[0], g = px[1], b = px[2], a = px[3];
            i32 idx = 0; // trasparente per default
            if (a >= 128) {
                i32 migliore = 0;
                i32 migliorDistanza = 0x7FFFFFFF;
                for (i32 c = 1; c < (i32)s.palette.size(); ++c) {
                    u32 col = s.palette[c];
                    i32 dr = (i32)((col >> 16) & 0xFF) - (i32)r;
                    i32 dg = (i32)((col >> 8) & 0xFF) - (i32)g;
                    i32 db = (i32)(col & 0xFF) - (i32)b;
                    i32 d = dr * dr + dg * dg + db * db;
                    if (d < migliorDistanza) {
                        migliorDistanza = d;
                        migliore = c;
                    }
                }
                // Soglia oltre la quale si crea un nuovo colore di palette.
                if (migliorDistanza > SOGLIA_NUOVO_COLORE * SOGLIA_NUOVO_COLORE && (i32)s.palette.size() < 256) {
                    s.palette.push_back(0xFF000000u | (u32)r << 16 | (u32)g << 8 | (u32)b);
                    migliore = (i32)s.palette.size() - 1;
                }
                idx = migliore;
            }
            s.pixels[(size_t)y * (size_t)surf->w + (size_t)x] = (u8)idx;
        }
    }
    return s;
}
#endif // SGL_HAVE_SDL2_IMAGE

} // namespace

std::vector<std::pair<std::string, Sprite>> caricaSponsorSprite(const std::string& radice) {
    std::vector<std::pair<std::string, Sprite>> out;
    i32 daPng = 0, daRaw = 0;
    for (i32 si = 0; si < 7; ++si) {
        for (i32 ti = 0; ti < 3; ++ti) {
            const std::string chiave = std::string(kSponsorId[si]) + "/" + kStile[ti];
            bool ok = false;
#ifdef SGL_HAVE_SDL2_IMAGE
            const std::string path = radice + "/" + kSponsorId[si] + "/cartellone_" + kStile[ti] + ".png";
            if (SDL_Surface* surf = IMG_Load(path.c_str())) {
                SDL_Surface* conv = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA8888, 0);
                SDL_FreeSurface(surf);
                if (conv) {
                    out.push_back({chiave, convertiSprite(conv)});
                    SDL_FreeSurface(conv);
                    ok = true;
                    ++daPng;
                }
            }
#else
            (void)radice;
#endif
            if (!ok) {
                // Ripiego senza SDL2_image: stessi cartelloni gia' convertiti in raw
                // (dati/sprite_raw/sponsor/, stesso formato usato da prova_gate).
                const std::string raw = std::string("motore_nuovo/dati/sprite_raw/sponsor/") +
                                        kSponsorId[si] + "_" + kStile[ti] + ".sglraw";
                Sprite s = caricaSpriteRaw(raw);
                if (s.width > 0) {
                    out.push_back({chiave, std::move(s)});
                    ++daRaw;
                } else {
                    std::printf("sponsor_loader: cartellone mancante %s\n", chiave.c_str());
                }
            }
        }
    }
    std::printf("sponsor_loader: %zu/21 cartelloni caricati (%d da PNG, %d da raw)\n",
                out.size(), daPng, daRaw);
    return out;
}

} // namespace sgl
