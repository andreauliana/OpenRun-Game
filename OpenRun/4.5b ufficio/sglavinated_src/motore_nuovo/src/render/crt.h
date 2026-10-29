// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include <vector>

namespace sgl {

// Filtro CRT opzionale (Andrea 29/10): il gioco visto "come sul televisore".
// Solo resa a schermo, dopo il render: il framebuffer del core e frameHash
// non cambiano. Spento di default (config.json "filtro_crt", tasto C nel runner).
// Due pezzi, entrambi a costo quasi nullo, pensati per il Pi 400 a 60 Hz:
//  1. crtSbava: il segnale composito impasta ogni pixel con i vicini in
//     orizzontale (1-2-1) e alza un poco la luce per compensare la maschera.
//     Lavora a 320x224, una passata.
//  2. crtMaschera: immagine fissa grande quanto lo schermo (320*scala x
//     224*scala) con le righe scure tra le linee del fascio e le tre strisce
//     dei fosfori R, G, B. Si calcola una volta sola (o al cambio di scala);
//     il runner la moltiplica sopra il gioco con la GPU (blend MOD).
// Niente pixel 8:7: in 320 di larghezza il Mega Drive li ha quasi quadrati,
// e la scala intera tiene i pixel nitidi.

// src e dst: w*h pixel 0xAARRGGBB (possono non coincidere).
void crtSbava(const u32* src, u32* dst, i32 w, i32 h);

// Maschera (w*scala) x (h*scala), 0xAARRGGBB. Con scala 1 resta solo la
// maschera dei fosfori (righe scure impossibili sotto i 2 pixel per riga).
std::vector<u32> crtMaschera(i32 w, i32 h, i32 scala);

} // namespace sgl
