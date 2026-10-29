// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "audio/mixer.h"
#include <array>

namespace sgl {

class Framebuffer;

// Spettro della musica tracker in gara (Andrea 29/10: animazione moderna ma
// pixelata e minimale, palette sci-fi con buon contrasto, niente sotto la
// linea di base, animazioni pazze sui picchi).
// Le barre vanno dai bassi (sinistra) agli acuti (destra) secondo la nota che
// ogni canale sta suonando: si vede la melodia muoversi. Barre a LED con
// ombra di 1 px, colori neon che scorrono da una barra all'altra, onda che
// parte dalla barra piu' alta sul battito. Quando una barra arriva in cima:
// lampo, cappello sparato in alto, esplosione di pixel e anello che si allarga.
// Le sfumature sono a trama (dithering ordinato Bayer), mai colori mescolati.
// Deterministico, zero alloc.
class Visualizzatore {
public:
    static constexpr i32 BARRE = 16;
    void aggiorna(const InfoMusica& m, f32 dt);
    // x0, yBase: angolo in basso a sinistra delle barre (pixel schermo).
    // Si disegna tutto sopra la linea di base, che e' la riga yBase + 2.
    void disegna(Framebuffer& fb, i32 x0, i32 yBase) const;

private:
    // gravita 0: scintilla che sale e si spegne; > 0: pezzo d'esplosione che
    // vola e ricade cambiando colore a ogni frame.
    struct Scintilla { f32 x = 0, y = 0, vx = 0, vy = 0, vita = 0, tinta = 0, gravita = 0; };
    void scintilla(i32 barra, f32 forza);
    void esplosione(i32 barra);
    f32 caso(); // 0..1, LCG con seme fisso
    f32 onda(i32 barra) const; // 0..1: quanto l'onda del battito sta passando sulla barra
    f32 tinta(i32 barra) const; // posizione nella palette neon (giro 0..1)

    f32 barra_[BARRE] = {};
    f32 picco_[BARRE] = {};
    f32 vPicco_[BARRE] = {};
    f32 dalPicco_[BARRE] = {}; // secondi da quando il cappello e' stato spinto in cima
    f32 lampo_[BARRE] = {};    // secondi di lampo rimasti dopo un colpo in cima
    f32 anello_[BARRE] = {};   // eta' dell'anello che si allarga (<= 0 = nessuno)
    f32 anelloY_[BARRE] = {};  // altezza in pixel da cui parte l'anello
    f32 riposo_[BARRE] = {};   // attesa prima di un'altra esplosione sulla stessa barra
    f32 battitoPrec_ = 0.0f;
    f32 deriva_ = 0.0f;        // i colori scorrono piano lungo le barre
    f32 fase_ = 0.0f;
    i32 ondaOrigine_ = 0;
    f32 ondaRaggio_ = 0.0f, ondaForza_ = 0.0f;
    u32 seme_ = 0x2F6E2B1u;
    std::array<Scintilla, 64> scintille_{};
};

} // namespace sgl
