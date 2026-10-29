// SPDX-License-Identifier: MIT
#include "render/visualizzatore.h"
#include "render/framebuffer.h"
#include <algorithm>
#include <cmath>

namespace sgl {

namespace {
constexpr i32 LARGHEZZA = 3, PASSO = 4; // barra 3 px + 1 px di aria
constexpr i32 ALTEZZA_MAX = 30;         // pixel
constexpr i32 NOTA_BASSA = 30, NOTE = 54; // da F#-2 a C-7: dove stanno quasi tutte le note
constexpr f32 SALITA = 30.0f;           // attacco rapido (1/s)
constexpr f32 CADUTA = 1.6f;            // altezza persa al secondo
constexpr f32 GRAVITA_PICCO = 2.2f;     // il cappello ricade accelerando
constexpr f32 SOGLIA_SCINTILLA = 0.78f;

u32 rgb(f32 r, f32 g, f32 b) {
    auto c = [](f32 v) { return (u32)std::clamp(v, 0.0f, 255.0f); };
    return 0xFF000000u | (c(r) << 16) | (c(g) << 8) | c(b);
}

// Colore di un LED all'altezza t (0 = base, 1 = cima): blu, ciano, magenta, bianco.
u32 coloreLed(f32 t, f32 luce) {
    struct Tappa { f32 t, r, g, b; };
    static constexpr Tappa SCALA[4] = {
        {0.00f, 20, 90, 200}, {0.45f, 60, 220, 255}, {0.80f, 255, 60, 200}, {1.00f, 255, 255, 255}};
    i32 i = 0;
    while (i < 2 && t > SCALA[i + 1].t) ++i;
    const f32 k = std::clamp((t - SCALA[i].t) / (SCALA[i + 1].t - SCALA[i].t), 0.0f, 1.0f);
    const f32 l = 0.75f + 0.25f * luce;
    return rgb((SCALA[i].r + (SCALA[i + 1].r - SCALA[i].r) * k) * l,
               (SCALA[i].g + (SCALA[i + 1].g - SCALA[i].g) * k) * l,
               (SCALA[i].b + (SCALA[i + 1].b - SCALA[i].b) * k) * l);
}

u32 scurito(u32 c, u32 percento) {
    const u32 r = ((c >> 16) & 0xFFu) * percento / 100, g = ((c >> 8) & 0xFFu) * percento / 100, b = (c & 0xFFu) * percento / 100;
    return 0xFF000000u | (r << 16) | (g << 8) | b;
}
} // namespace

f32 Visualizzatore::caso() {
    seme_ = seme_ * 1664525u + 1013904223u;
    return (f32)(seme_ >> 8) / (f32)(1u << 24);
}

void Visualizzatore::scintilla(i32 barra, f32 forza) {
    for (Scintilla& s : scintille_) {
        if (s.vita > 0.0f) continue;
        s.x = (f32)(barra * PASSO + 1);
        s.y = -barra_[barra] * (f32)ALTEZZA_MAX - 2.0f;
        s.vx = (caso() - 0.5f) * 40.0f;
        s.vy = -(35.0f + 55.0f * caso()) * forza;
        s.vita = 0.5f + 0.4f * caso();
        return;
    }
}

void Visualizzatore::aggiorna(const InfoMusica& m, f32 dt) {
    fase_ += dt * 6.0f;
    f32 obiettivo[BARRE] = {};
    if (m.attiva) {
        // Ogni canale che suona accende la barra della sua nota e, meno, le vicine.
        for (i32 c = 0; c < m.canali; ++c) {
            if (m.nota[c] < 0 || m.livello[c] < 0.02f) continue;
            const f32 pos = std::clamp((f32)(m.nota[c] - NOTA_BASSA) / (f32)NOTE, 0.0f, 1.0f) * (f32)(BARRE - 1);
            for (i32 b = 0; b < BARRE; ++b) {
                const f32 vicino = 1.0f - std::fabs((f32)b - pos) / 2.2f;
                if (vicino > 0.0f) obiettivo[b] += m.livello[c] * vicino;
            }
        }
        // Un'onda leggera che segue il volume, cosi' lo spettro non sta mai fermo.
        for (i32 b = 0; b < BARRE; ++b)
            obiettivo[b] = std::min(1.0f, obiettivo[b] + m.energia * (0.10f + 0.08f * std::sin(fase_ + (f32)b * 0.8f)));
        // Primo tempo della battuta: rimbalzo, lampo della base, scintille dalle barre piu' alte.
        if (m.battito > battitoPrec_ + 0.5f) {
            rimbalzo_ = 1.0f;
            lampo_ = 1.0f;
            i32 alta = 0;
            for (i32 b = 1; b < BARRE; ++b) if (barra_[b] > barra_[alta]) alta = b;
            scintilla(alta, 1.2f);
            scintilla(std::clamp(alta + (caso() < 0.5f ? -2 : 2), 0, BARRE - 1), 1.0f);
        }
        battitoPrec_ = m.battito;
    }

    for (i32 b = 0; b < BARRE; ++b) {
        const f32 prima = barra_[b];
        if (obiettivo[b] > barra_[b]) barra_[b] += (obiettivo[b] - barra_[b]) * std::min(1.0f, SALITA * dt);
        else barra_[b] = std::max(obiettivo[b], barra_[b] - CADUTA * dt);
        if (barra_[b] >= SOGLIA_SCINTILLA && prima < SOGLIA_SCINTILLA) scintilla(b, 1.0f);
        if (barra_[b] >= picco_[b]) { picco_[b] = barra_[b]; vPicco_[b] = 0.0f; }
        else { vPicco_[b] += GRAVITA_PICCO * dt; picco_[b] = std::max(barra_[b], picco_[b] - vPicco_[b] * dt); }
    }
    rimbalzo_ = std::max(0.0f, rimbalzo_ - dt * 5.0f);
    lampo_ = std::max(0.0f, lampo_ - dt * 3.0f);

    for (Scintilla& s : scintille_) {
        if (s.vita <= 0.0f) continue;
        s.vy += 140.0f * dt;
        s.x += s.vx * dt;
        s.y += s.vy * dt;
        s.vita -= dt;
    }
}

void Visualizzatore::disegna(Framebuffer& fb, i32 x0, i32 yBase) const {
    const f32 spinta = 1.0f + 0.18f * rimbalzo_;
    for (i32 b = 0; b < BARRE; ++b) {
        const i32 x = x0 + b * PASSO;
        const i32 h = std::min(ALTEZZA_MAX + 4, (i32)(barra_[b] * (f32)ALTEZZA_MAX * spinta + 0.5f));
        // LED da 2 px con 1 px di aria, colore secondo l'altezza.
        for (i32 y = 0; y + 1 < h; y += 3) {
            const u32 col = coloreLed((f32)y / (f32)ALTEZZA_MAX, lampo_);
            fb.fillRect(x, yBase - y - 1, x + LARGHEZZA - 1, yBase - y, col);
            // Riflesso sotto la base: un LED ogni due, scuro, schiacciato.
            const i32 yr = yBase + 2 + y / 3;
            if ((y / 3) % 2 == 0 && yr < yBase + 7) fb.fillRect(x, yr, x + LARGHEZZA - 1, yr, scurito(col, 35));
        }
        // Cappello: ricade piano dopo i colpi.
        const i32 yp = yBase - (i32)(picco_[b] * (f32)ALTEZZA_MAX * spinta) - 2;
        if (picco_[b] > 0.04f) fb.fillRect(x, yp, x + LARGHEZZA - 1, yp, 0xFFFFFFFFu);
    }
    // Base: si accende sul battito.
    fb.fillRect(x0 - 1, yBase + 1, x0 + BARRE * PASSO - 1, yBase + 1, rgb(40 + 215 * lampo_, 40 + 20 * lampo_, 90 + 110 * lampo_));
    // Scintille: bianche, poi magenta, poi si spengono.
    for (const Scintilla& s : scintille_) {
        if (s.vita <= 0.0f) continue;
        const i32 px = x0 + (i32)s.x, py = yBase + (i32)s.y;
        const u32 col = s.vita > 0.45f ? 0xFFFFFFFFu : (s.vita > 0.2f ? 0xFFFF50D0u : 0xFF8030A0u);
        if (s.vita > 0.45f) fb.fillRect(px, py, px + 1, py + 1, col);
        else fb.put(px, py, col);
    }
}

} // namespace sgl
