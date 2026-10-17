// SPDX-License-Identifier: MIT
#pragma once
#include "render/sprite.h"
#include <string>
#include <utility>
#include <vector>

namespace sgl {

// Carica i cartelloni sponsor da <radice>/<id>/cartellone_<stile>.png
// (riferimento: assets_originali/sponsor/CATALOGO_SPONSOR.json; 7 sponsor x
// 3 stili neon/atomico/foglie, sprite originali 128x64, nessun asset Sega).
// Restituisce coppie (sponsor_id, Sprite) cosi' il core puo' abbinare lo
// sponsor_id di livello.json allo sprite corrispondente.
// La decodifica immagini spetta al runner (SDL2_image): il core resta senza SDL.
// Se SDL2_image non e' disponibile, restituisce un vettore vuoto e il gioco
// gira comunque (cartelloni assenti).
std::vector<std::pair<std::string, Sprite>> caricaSponsorSprite(const std::string& radice = "assets_originali/sponsor");

} // namespace sgl
