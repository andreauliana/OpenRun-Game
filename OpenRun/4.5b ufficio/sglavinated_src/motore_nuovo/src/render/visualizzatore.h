// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "audio/mixer.h"
#include <array>

namespace sgl {

class Framebuffer;

// Spettro della musica tracker in gara (Andrea 29/10: reazioni divertenti).
// Le barre vanno dai bassi (sinistra) agli acuti (destra) secondo la nota che
// ogni canale sta suonando: si vede la melodia muoversi. Barre a LED con
// cappello che ricade, riflesso sotto la base, rimbalzo sul battito e
// scintille quando una barra arriva in cima. Deterministico, zero alloc.
class Visualizzatore {
public:
    static constexpr i32 BARRE = 16;
    void aggiorna(const InfoMusica& m, f32 dt);
    // x0, yBase: angolo in basso a sinistra delle barre (pixel schermo).
    void disegna(Framebuffer& fb, i32 x0, i32 yBase) const;

private:
    struct Scintilla { f32 x = 0, y = 0, vx = 0, vy = 0, vita = 0; };
    void scintilla(i32 barra, f32 forza);
    f32 caso(); // 0..1, LCG con seme fisso

    f32 barra_[BARRE] = {};
    f32 picco_[BARRE] = {};
    f32 vPicco_[BARRE] = {};
    f32 battitoPrec_ = 0.0f;
    f32 rimbalzo_ = 0.0f;
    f32 lampo_ = 0.0f;
    f32 fase_ = 0.0f;
    u32 seme_ = 0x2F6E2B1u;
    std::array<Scintilla, 40> scintille_{};
};

} // namespace sgl
