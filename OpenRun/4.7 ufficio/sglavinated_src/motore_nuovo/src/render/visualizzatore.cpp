// SPDX-License-Identifier: MIT
#include "render/visualizzatore.h"
#include "render/framebuffer.h"
#include <algorithm>
#include <cmath>

namespace sgl {

namespace {
constexpr i32 LARGHEZZA = 3;
constexpr i32 NOTA_BASSA = 30, NOTE = 54; // da F#-2 a C-7: dove stanno quasi tutte le note
constexpr f32 MOLLA = 900.0f;             // rigidezza (omega 30 rad/s): reattiva
constexpr f32 SMORZA = 36.0f;             // smorzamento (zeta ~0,6): un filo di rimbalzo, poi ferma
constexpr f32 FERMO_PICCO_S = 0.35f;      // il cappello resta in cima, poi cade
constexpr f32 GRAVITA_PICCO = 2.4f;       // e ricade accelerando
constexpr u32 BIANCO = 0xFFFFFFFFu;

// Dithering ordinato Bayer 4x4 (Andrea 29/10): soglia 0..1 fissa per pixel.
f32 soglia(i32 x, i32 y) {
    static constexpr u8 BAYER[4][4] = {{0, 8, 2, 10}, {12, 4, 14, 6}, {3, 11, 1, 9}, {15, 7, 13, 5}};
    return ((f32)BAYER[y & 3][x & 3] + 0.5f) / 16.0f;
}

// Scala di colore per altezza (0 = base, 1 = cima): 4 neon sci-fi pieni,
// tra uno e l'altro una trama Bayer al posto della sfumatura.
u32 coloreAltezza(f32 k, f32 s) {
    static constexpr u32 SCALA[4] = {0xFF00E6FFu, 0xFF963CFFu, 0xFFFF28B4u, 0xFFE6FF32u};
    const f32 p = std::clamp(k, 0.0f, 1.0f) * 3.0f;
    const i32 i = std::min(2, (i32)p);
    return (p - (f32)i) > s ? SCALA[i + 1] : SCALA[i];
}
} // namespace

void Visualizzatore::aggiorna(const InfoMusica& m, f32 dt) {
    fase_ += dt * 6.0f;
    f32 obiettivo[BARRE] = {};
    if (m.attiva) {
        // Ogni canale che suona accende la barra della sua nota e, meno, le vicine.
        for (i32 c = 0; c < m.canali; ++c) {
            if (m.nota[c] < 0 || m.livello[c] < 0.02f) continue;
            const f32 pos = std::clamp((f32)(m.nota[c] - NOTA_BASSA) / (f32)NOTE, 0.0f, 1.0f) * (f32)(BARRE - 1);
            for (i32 b = 0; b < BARRE; ++b) {
                const f32 vicino = 1.0f - std::fabs((f32)b - pos) / 2.5f;
                if (vicino > 0.0f) obiettivo[b] += m.livello[c] * vicino;
            }
        }
        // Un'onda leggera che segue il volume e scorre da sinistra a destra,
        // cosi' lo spettro non sta mai fermo.
        for (i32 b = 0; b < BARRE; ++b)
            obiettivo[b] += m.energia * (0.12f + 0.10f * std::sin(fase_ - (f32)b * 0.55f));
        battito_ = m.battito;
    } else {
        battito_ = 0.0f;
    }
    // Smussatura con le vicine (1-2-3-2-1): niente denti, una curva continua.
    f32 liscio[BARRE];
    for (i32 b = 0; b < BARRE; ++b) {
        f32 somma = 0.0f, peso = 0.0f;
        for (i32 d = -2; d <= 2; ++d) {
            const i32 j = b + d;
            if (j < 0 || j >= BARRE) continue;
            const f32 w = (f32)(3 - std::abs(d));
            somma += obiettivo[j] * w;
            peso += w;
        }
        liscio[b] = std::min(1.0f, somma / peso * 1.4f); // 1.4: la smussatura abbassa i picchi
    }
    const f32 passo = std::min(dt, 1.0f / 30.0f); // molla stabile anche a 50 Hz
    for (i32 b = 0; b < BARRE; ++b) {
        vBarra_[b] += (MOLLA * (liscio[b] - barra_[b]) - SMORZA * vBarra_[b]) * passo;
        barra_[b] += vBarra_[b] * passo;
        if (barra_[b] < 0.0f) { barra_[b] = 0.0f; vBarra_[b] = 0.0f; }
        if (barra_[b] > 1.0f) { barra_[b] = 1.0f; vBarra_[b] = std::min(vBarra_[b], 0.0f); }
        if (barra_[b] >= picco_[b]) { picco_[b] = barra_[b]; vPicco_[b] = 0.0f; fermoPicco_[b] = FERMO_PICCO_S; }
        else if (fermoPicco_[b] > 0.0f) fermoPicco_[b] -= dt;
        else {
            vPicco_[b] += GRAVITA_PICCO * dt;
            picco_[b] = std::max(barra_[b], picco_[b] - vPicco_[b] * dt);
        }
    }
}

void Visualizzatore::disegna(Framebuffer& fb, i32 x0, i32 yBase) const {
    for (i32 b = 0; b < BARRE; ++b) {
        const i32 x = x0 + b * PASSO;
        const i32 h = std::min(ALTEZZA_MAX, (i32)(barra_[b] * (f32)ALTEZZA_MAX + 0.5f));
        for (i32 r = 0; r < h; ++r) {
            const i32 y = yBase - r;
            const f32 k = (f32)r / (f32)(ALTEZZA_MAX - 1);
            for (i32 px = x; px < x + LARGHEZZA; ++px) fb.put(px, y, coloreAltezza(k, soglia(px, y)));
        }
        // Cappello: 1 px bianco sopra la barra (con 1 px d'aria quando e' staccato).
        const i32 hp = std::min(ALTEZZA_MAX, (i32)(picco_[b] * (f32)ALTEZZA_MAX + 0.5f));
        if (picco_[b] > 0.03f) fb.fillRect(x, yBase - hp, x + LARGHEZZA - 1, yBase - hp, BIANCO);
        // Linea di base (riga sottile in fondo al gruppo, come contagiri e
        // tappe): 2 px a puntini, piena sul primo tempo della battuta.
        for (i32 y = yBase + 3; y <= yBase + 4; ++y)
            for (i32 px = x; px < x + LARGHEZZA + (b < BARRE - 1 ? 1 : 0); ++px)
                if (((px + y) & 1) == 0 || battito_ > 0.5f)
                    fb.put(px, y, battito_ > 0.5f ? coloreAltezza(0.0f, 1.0f) : 0xFF6A6A80u);
    }
}

} // namespace sgl
