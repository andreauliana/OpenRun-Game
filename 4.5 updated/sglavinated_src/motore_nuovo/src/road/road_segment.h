// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

namespace sgl {

// Un segmento della strada pseudo-3D.
// Tecniche di riferimento (solo idee, nessun codice copiato):
//   L. Gorenfeld, "Lou's Pseudo 3d Page", https://www.extentofthejam.com/pseudo/
//   Jake Gordon, "JavaScript Racer", https://jakesgordon.com/writing/javascript-racer/
//
// Un segmento ha: lunghezza fissa (unita' del mondo), curva (variante di
// posizione laterale lungo il segmento, in rad per segmento) e collina
// (variante di altezza lungo il segmento). La strada e' una lista circolare
// di segmenti; la posizione assoluta si avvolge (wrap) sulla lunghezza totale.
struct RoadSegment {
    i32 index = 0;           // indice assoluto nella lista
    i32 length = 200;        // lunghezza in unita' mondo (default = Road::SEG_LEN)
    f32 curve = 0.0f;        // curvatura: delta di world_x per passo lungo lo segmento
    f32 hill = 0.0f;         // collina: delta di world_y per passo lungo lo segmento
    u32 colorRoad = 0xFF808080;  // colore asfalto (palette index -> render)
    u32 colorRumble = 0xFFFFFFFF; // colore strisce laterali
    // world coordinates del primo punto (calcolati da Road al build)
    f32 p1x = 0.0f, p1y = 0.0f, p1z = 0.0f;
    f32 p2x = 0.0f, p2y = 0.0f, p2z = 0.0f;
};

} // namespace sgl
