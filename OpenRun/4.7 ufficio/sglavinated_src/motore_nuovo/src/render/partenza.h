// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

namespace sgl {

class Framebuffer;
class Renderer;

// Partenza della gara (Andrea 08/11): auto ferme in griglia, i gruppi di
// supporto ai lati che chiacchierano, poi il conto alla rovescia 3, 2, 1,
// START! al centro in alto. Non fa parte dell'HUD (bloccato): e' un
// sovrimpressione a parte, disegnata sopra la scena solo alla partenza.
//
// Tempi (secondi dall'avvio della gara):
//   0 .. PARTENZA_ATTESA_S          "GET READY", gente in relax
//   ATTESA + 0, +1, +2              3, 2, 1 (un bip ciascuno, una luce rossa in piu')
//   PARTENZA_VIA_S                  START! (bip acuto, luci verdi, musica, si parte)
//   VIA .. VIA + PARTENZA_START_S   la scritta resta un attimo mentre si parte
constexpr f32 PARTENZA_ATTESA_S = 1.6f;
constexpr f32 PARTENZA_PASSO_S = 1.0f;
constexpr f32 PARTENZA_VIA_S = PARTENZA_ATTESA_S + 3.0f * PARTENZA_PASSO_S;
constexpr f32 PARTENZA_START_S = 1.2f;
constexpr f32 PARTENZA_FINE_S = PARTENZA_VIA_S + PARTENZA_START_S;

// Centro verticale della scritta: sezione aurea dell'altezza dall'alto
// (224 / phi^2 ~ 86), sopra l'orizzonte (112) e sotto l'HUD.
constexpr i32 PARTENZA_CENTRO_Y = 86;

// Conto in corso a tempo t: 3, 2, 1; 0 = START; -1 = attesa; -2 = finito.
i32 contoPartenza(f32 t);

// Disegna il conto alla rovescia (t = secondi dall'avvio della gara).
void disegnaPartenza(Framebuffer& fb, Renderer& ren, f32 t);

} // namespace sgl
