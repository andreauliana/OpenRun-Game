// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "audio/mixer.h"

namespace sgl {

class Framebuffer;

// Spettro della musica tracker in gara, al centro della striscia dell'HUD
// (Andrea 07/11: piu' semplice, come lo spettro di WinAmp ma moderno).
// Barre piene dai bassi (sinistra) agli acuti (destra) secondo la nota che
// ogni canale sta suonando; il colore dipende dall'altezza della riga (ciano
// in basso, poi viola, magenta e giallo in cima), con il passaggio tra due
// colori a trama Bayer: pochi colori pieni, mai mescolati (palette limitata).
// Sopra ogni barra un cappello bianco di 1 px che resta un attimo e poi ricade.
// Movimento morbido e ondulato (Andrea 07/11): ogni barra e' una molla che
// segue il suo obiettivo (sale svelta, si assesta senza scatti) e l'obiettivo
// e' smussato con le barre vicine, cosi' lo spettro si muove come un'onda.
// Niente scintille, esplosioni o anelli; tutto sopra la linea di base.
// Deterministico, zero alloc.
class Visualizzatore {
public:
    static constexpr i32 BARRE = 22;
    static constexpr i32 PASSO = 4;        // barra 3 px + 1 px di aria
    static constexpr i32 ALTEZZA_MAX = 10; // pixel (Andrea 07/11: basso)
    static constexpr i32 LARGHEZZA_TOT = BARRE * PASSO - 1;
    void aggiorna(const InfoMusica& m, f32 dt);
    // x0, yBase: angolo in basso a sinistra delle barre (pixel schermo); la
    // riga yBase e' l'ultima riga accesa, la linea di base sono le righe
    // yBase + 3 e yBase + 4 (allineata alle righe sottili degli altri gruppi).
    void disegna(Framebuffer& fb, i32 x0, i32 yBase) const;

private:
    f32 barra_[BARRE] = {};
    f32 vBarra_[BARRE] = {};     // velocita' della molla di ogni barra (altezze/s)
    f32 picco_[BARRE] = {};
    f32 vPicco_[BARRE] = {};
    f32 fermoPicco_[BARRE] = {}; // secondi in cui il cappello resta fermo prima di ricadere
    f32 battito_ = 0.0f;
    f32 fase_ = 0.0f;
};

} // namespace sgl
