// SPDX-License-Identifier: MIT
#include "render/visualizzatore.h"
#include "render/framebuffer.h"
#include <algorithm>
#include <cmath>

namespace sgl {

namespace {
constexpr i32 LARGHEZZA = 3, PASSO = 4; // barra 3 px + 1 px di aria
constexpr i32 ALTEZZA_MAX = 30;         // pixel
constexpr i32 LED = 3;                  // LED da 2 px + 1 px di aria
constexpr i32 NOTA_BASSA = 30, NOTE = 54; // da F#-2 a C-7: dove stanno quasi tutte le note
constexpr f32 SALITA = 30.0f;           // attacco rapido (1/s)
constexpr f32 CADUTA = 1.6f;            // altezza persa al secondo
constexpr f32 GRAVITA_PICCO = 2.2f;     // il cappello ricade accelerando
constexpr f32 SOGLIA_SCINTILLA = 0.78f;
constexpr f32 ONDA_VELOCITA = 36.0f;    // barre al secondo: attraversa lo spettro in ~0,4 s
constexpr f32 ONDA_ALZA_PX = 4.0f;      // quanto l'onda solleva una barra
constexpr f32 DERIVA = 0.05f;           // giri di palette al secondo
constexpr f32 TINTA_BARRA = 0.035f;     // scarto di colore tra una barra e la vicina

u32 rgb(f32 r, f32 g, f32 b) {
    auto c = [](f32 v) { return (u32)std::clamp(v, 0.0f, 255.0f); };
    return 0xFF000000u | (c(r) << 16) | (c(g) << 8) | c(b);
}

// Dithering ordinato (Bayer 4x4, Andrea 29/10): soglia 0..1 fissa per cella.
// Le sfumature diventano trame tra due colori pieni invece di colori mescolati.
// Sulle barre la cella e' un LED (ogni LED resta di un colore solo, niente
// rumore dentro i 3x2 pixel), sugli effetti leggeri (alone, scintille, base) e' il pixel.
f32 soglia(i32 x, i32 y) {
    static constexpr u8 BAYER[4][4] = {{0, 8, 2, 10}, {12, 4, 14, 6}, {3, 11, 1, 9}, {15, 7, 13, 5}};
    return ((f32)BAYER[y & 3][x & 3] + 0.5f) / 16.0f;
}

// Palette neon sci-fi fissa di 9 colori: menta, ciano, viola, magenta, gli
// stessi schiariti a meta' verso il bianco, e il bianco. Pochi colori e trama
// al posto delle sfumature (Andrea 29/10: palette limitata, portabile su Pi,
// browser, Windows XP). Si percorre avanti e indietro (niente salto tra magenta
// e menta). Nessun colore scuro: sullo sfondo chiaro il nero appesantisce.
// giro: posizione nella palette; bianco 0..1: quanto schiarire; s: soglia della cella.
u32 neon(f32 giro, f32 bianco, f32 s) {
    struct Colore { f32 r, g, b; };
    static constexpr Colore SCALA[4] = {{70, 255, 200}, {0, 220, 255}, {150, 80, 255}, {255, 60, 200}};
    const f32 f = giro - std::floor(giro);
    const f32 p = (1.0f - std::fabs(2.0f * f - 1.0f)) * 3.0f; // 0..3 e ritorno
    const i32 i = std::min(2, (i32)p);
    const Colore& c = (p - (f32)i) > s ? SCALA[i + 1] : SCALA[i];
    // Tre gradini verso il bianco (colore, schiarito, bianco), anche questi a trama.
    const i32 gradino = std::clamp((i32)(std::clamp(bianco, 0.0f, 1.0f) * 2.0f + s), 0, 2);
    const f32 w = gradino == 0 ? 0.0f : (gradino == 1 ? 0.5f : 1.0f);
    return rgb(c.r + (255.0f - c.r) * w, c.g + (255.0f - c.g) * w, c.b + (255.0f - c.b) * w);
}
} // namespace

f32 Visualizzatore::caso() {
    seme_ = seme_ * 1664525u + 1013904223u;
    return (f32)(seme_ >> 8) / (f32)(1u << 24);
}

f32 Visualizzatore::tinta(i32 barra) const { return deriva_ + (f32)barra * TINTA_BARRA; }

f32 Visualizzatore::onda(i32 barra) const {
    if (ondaForza_ <= 0.0f) return 0.0f;
    const f32 d = std::fabs((f32)(barra - ondaOrigine_));
    return std::max(0.0f, 1.0f - std::fabs(d - ondaRaggio_) / 1.5f) * ondaForza_;
}

void Visualizzatore::scintilla(i32 barra, f32 forza) {
    for (Scintilla& s : scintille_) {
        if (s.vita > 0.0f) continue;
        s.x = (f32)(barra * PASSO + 1);
        s.y = -barra_[barra] * (f32)ALTEZZA_MAX - 3.0f;
        s.vx = (caso() - 0.5f) * 14.0f;
        s.vy = -(16.0f + 20.0f * caso()) * forza;
        s.vita = 0.6f + 0.4f * caso();
        s.tinta = tinta(barra) + 0.12f;
        return;
    }
}

void Visualizzatore::aggiorna(const InfoMusica& m, f32 dt) {
    fase_ += dt * 6.0f;
    deriva_ += dt * DERIVA;
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
        // Primo tempo della battuta: parte un'onda dalla barra piu' alta, i colori
        // fanno un passo avanti e salgono due scintille.
        if (m.battito > battitoPrec_ + 0.5f) {
            i32 alta = 0;
            for (i32 b = 1; b < BARRE; ++b) if (barra_[b] > barra_[alta]) alta = b;
            ondaOrigine_ = alta;
            ondaRaggio_ = 0.0f;
            ondaForza_ = 1.0f;
            deriva_ += 0.06f;
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
        dalPicco_[b] += dt;
        if (barra_[b] >= picco_[b]) { picco_[b] = barra_[b]; vPicco_[b] = 0.0f; dalPicco_[b] = 0.0f; }
        else { vPicco_[b] += GRAVITA_PICCO * dt; picco_[b] = std::max(barra_[b], picco_[b] - vPicco_[b] * dt); }
    }
    ondaRaggio_ += ONDA_VELOCITA * dt;
    ondaForza_ = std::max(0.0f, ondaForza_ - dt * 1.8f);

    // Scintille: salgono, rallentano e ondeggiano un poco.
    for (Scintilla& s : scintille_) {
        if (s.vita <= 0.0f) continue;
        s.vy *= std::max(0.0f, 1.0f - 2.0f * dt);
        s.x += (s.vx + 6.0f * std::sin(fase_ * 1.3f + s.tinta * 20.0f)) * dt;
        s.y += s.vy * dt;
        s.vita -= dt;
    }
}

void Visualizzatore::disegna(Framebuffer& fb, i32 x0, i32 yBase) const {
    for (i32 b = 0; b < BARRE; ++b) {
        const i32 x = x0 + b * PASSO;
        const f32 o = onda(b), t0 = tinta(b);
        const i32 alza = (i32)(o * ONDA_ALZA_PX + 0.5f);
        const i32 h = std::min(ALTEZZA_MAX + 4, (i32)(barra_[b] * (f32)ALTEZZA_MAX + 0.5f) + alza);
        // LED da 2 px con 1 px di aria; l'ultimo e' mezzo LED se la barra non ci arriva
        // (sale e scende di un pixel alla volta). Il colore sale lungo la palette con
        // l'altezza, la punta e' piu' chiara, l'onda del battito accende i LED a trama.
        for (i32 y = 0, led = 0; y < h; y += LED, ++led) {
            const i32 righe = std::min(LED - 1, h - y);
            const f32 s = soglia(b, led);
            const f32 bianco = (y + LED >= h ? 0.5f : 0.0f) + 0.6f * o;
            fb.fillRect(x, yBase - y - righe + 1, x + LARGHEZZA - 1, yBase - y,
                        neon(t0 + 0.25f * (f32)y / (f32)ALTEZZA_MAX, bianco, s));
        }
        // Alone a trama sopra la punta mentre passa l'onda: 3 righe sempre piu' rade.
        if (o > 0.05f) {
            static constexpr f32 DENSITA[3] = {0.6f, 0.35f, 0.15f};
            for (i32 r = 0; r < 3; ++r) {
                const i32 py = yBase - h - r;
                for (i32 px = x - 1; px <= x + LARGHEZZA; ++px)
                    if (soglia(px, py) < DENSITA[r] * o) fb.put(px, py, neon(t0 + 0.25f, 0.5f, 0.5f));
            }
        }
        // Cappello: bianco quando viene spinto su, poi prende il colore della barra mentre ricade.
        const i32 yp = yBase - (i32)(picco_[b] * (f32)ALTEZZA_MAX + 0.5f) - alza - 2;
        if (picco_[b] > 0.04f)
            fb.fillRect(x, yp, x + LARGHEZZA - 1, yp, neon(t0 + 0.25f, std::max(0.0f, 1.0f - dalPicco_[b] * 1.5f), 0.5f));
        // Linea di base a puntini (1 px sotto i LED), piena e chiara dove passa l'onda.
        for (i32 px = x - 1; px < x + LARGHEZZA; ++px)
            if (((px - x0) & 1) == 0 || o > 0.3f) fb.put(px, yBase + 2, neon(t0, 0.25f + 0.75f * o, 0.5f));
    }
    // Scintille: un pixel che sale; chiaro all'inizio, poi del suo colore, alla fine si dirada e sparisce.
    for (const Scintilla& s : scintille_) {
        if (s.vita <= 0.0f) continue;
        const i32 px = x0 + (i32)s.x, py = yBase + (i32)s.y;
        if (s.vita < 0.3f && soglia(px, py) > s.vita / 0.3f) continue;
        fb.put(px, py, neon(s.tinta, s.vita > 0.5f ? 0.6f : 0.0f, 0.5f));
    }
}

} // namespace sgl
