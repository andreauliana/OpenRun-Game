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
    // Gara a tappe (Game::avviaGara)
    bool gara = false;
    i32 tappa = 1;         // numero della tappa (1 = partenza)
    i32 latoBivio = 0;     // 0 = scelta aperta, -1 sinistra, +1 destra
    i32 posizione = 0;     // 1 = primo
    i32 concorrenti = 0;   // giocatore compreso
    bool arrivo = false;   // tappa finale completata
    u32 frameGara = 0;     // tempo di gara in frame (50 Hz), fermo all'arrivo
    u32 tempoResiduoFrame = 0; // tempo rimasto (ogni tappa aggiunge tempo_limite_s)
    bool tempoScaduto = false; // game over: tempo finito prima dell'arrivo
};

} // namespace sgl
