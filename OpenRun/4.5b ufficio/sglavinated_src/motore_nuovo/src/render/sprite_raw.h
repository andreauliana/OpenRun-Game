// SPDX-License-Identifier: MIT
#pragma once
#include "render/sprite.h"
#include <string>

namespace sgl {

// Carica uno sprite raw prodotto da png_a_raw.py (out/sviluppatore/):
// "SGLR" + u32 little-endian larghezza + u32 little-endian altezza +
// w*h*4 byte RGBA8888.
// Decodifica con solo <cstdio>/<cstdint>: niente SDL, niente libpng, niente
// zlib (i file raw sono già decompressi). Il core resta indipendente dalla
// piattaforma. I colori vengono quantizzati a palette (max 256 colori,
// indice 0 = trasparente), coerentemente col decoder SDL2_image del runner
// (sponsor_loader.cpp), così lo stesso sprite viene disegnato ovunque.
// Restituisce Sprite vuoto (width=0) se il file manca o è danneggiato.
Sprite caricaSpriteRaw(const std::string& path);

} // namespace sgl
