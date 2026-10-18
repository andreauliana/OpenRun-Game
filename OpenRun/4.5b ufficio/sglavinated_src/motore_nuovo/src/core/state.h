// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

namespace sgl {

// Stato di gioco esportato: solo dati, nessuna logica.
// Usato da moduli fx, HUD, replay e test di regressione.
struct GameState {
    u64 frame = 0;         // contatore di frame
    u32 seed = 0;          // seme RNG della partita
    f32 playerSpeed = 0;   // velocita' auto giocatore (unita' per frame)
    f32 playerX = 0;       // offset laterale dal centro strada
    f32 playerZ = 0;       // posizione assoluta sul tracciato
    bool playerOffroad = false;
    bool playerFlipping = false;
    u32 playerFlipFrame = 0;
};

} // namespace sgl
