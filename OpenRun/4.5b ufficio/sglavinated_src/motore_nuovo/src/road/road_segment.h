// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

namespace sgl {

// Un segmento della strada pseudo-3D.
// Tecniche di riferimento (solo idee, nessun codice copiato):
//   L. Gorenfeld, "Lou's Pseudo 3d Page", https://www.extentofthejam.com/pseudo/
//   Jake Gordon, "JavaScript Racer", https://jakesgordon.com/writing/javascript-racer/
//
// Un segmento ha: lunghezza (unita' del mondo), curva (pendenza laterale) e
// collina (pendenza verticale); le pendenze sono raccordate fra segmenti.
// La strada e' una lista circolare
// di segmenti; la posizione assoluta si avvolge (wrap) sulla lunghezza totale.
struct RoadSegment {
    i32 index = 0;           // indice assoluto nella lista
    i32 length = 200;        // lunghezza in unita' mondo (default = Road::SEG_LEN)
    f32 curve = 0.0f;        // curva (valore di livello.json, gia' raccordato); curvatura = curve * Road::CURVE_SCALE
    f32 hill = 0.0f;         // collina del tratto (quota finale = hill * Road::HILL_HEIGHT)
    f32 larghezza = 1.0f;    // larghezza della carreggiata (1 = 2 corsie, 2 = 4 corsie), gia' raccordata
    bool acqua = false;      // ponte: ai lati acqua al posto dell'erba (tratto "ponte", campo acqua)
    u32 colorRoad = 0xFF808080;  // colore asfalto (palette index -> render)
    u32 colorRumble = 0xFFFFFFFF; // colore strisce laterali
    // world coordinates del primo punto (calcolati da Road al build)
    f32 p1x = 0.0f, p1y = 0.0f, p1z = 0.0f;
    f32 p2x = 0.0f, p2y = 0.0f, p2z = 0.0f;
};

} // namespace sgl
