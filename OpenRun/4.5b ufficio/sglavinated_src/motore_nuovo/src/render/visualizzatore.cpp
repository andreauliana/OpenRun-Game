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
constexpr f32 SOGLIA_PICCO = 0.86f;     // oltre: esplosione sul picco
constexpr f32 SPARO_CAPPELLO = 1.8f;    // velocita' verso l'alto del cappello sparato (altezze/s)
constexpr f32 CAPPELLO_MAX = 1.4f;      // il cappello puo' volare fin qui sopra la barra piena
constexpr f32 LAMPO_S = 0.15f;
constexpr f32 ANELLO_S = 0.3f, ANELLO_RAGGIO = 13.0f;
constexpr f32 RIPOSO_S = 0.3f;          // una barra non esplode piu' spesso di cosi'
constexpr f32 ONDA_VELOCITA = 36.0f;    // barre al secondo: attraversa lo spettro in ~0,4 s
constexpr f32 ONDA_ALZA_PX = 4.0f;      // quanto l'onda solleva una barra
constexpr f32 DERIVA = 0.05f;           // giri di palette al secondo
constexpr f32 TINTA_BARRA = 0.035f;     // scarto di colore tra una barra e la vicina
constexpr u32 OMBRA = 0xFF0C0818u;      // ombra di 1 px: stacca le barre anche sugli sfondi chiari
constexpr u32 BIANCO = 0xFFFFFFFFu;

u32 rgb(f32 r, f32 g, f32 b) {
    auto c = [](f32 v) { return (u32)std::clamp(v, 0.0f, 255.0f); };
    return 0xFF000000u | (c(r) << 16) | (c(g) << 8) | c(b);
}

// Dithering ordinato (Bayer 4x4, Andrea 29/10): soglia 0..1 fissa per cella.
// Le sfumature diventano trame tra due colori pieni invece di colori mescolati.
// Sulle barre la cella e' un LED (ogni LED resta di un colore solo: le barre
// sono troppo strette per una trama per pixel), sugli effetti e' il pixel.
f32 soglia(i32 x, i32 y) {
    static constexpr u8 BAYER[4][4] = {{0, 8, 2, 10}, {12, 4, 14, 6}, {3, 11, 1, 9}, {15, 7, 13, 5}};
    return ((f32)BAYER[y & 3][x & 3] + 0.5f) / 16.0f;
}

// Palette fissa: 4 neon sci-fi saturi (ciano, viola, magenta, giallo acido),
// il bianco e l'ombra. Pochi colori e trama al posto delle sfumature (Andrea
// 29/10: palette limitata, portabile su Pi, browser, Windows XP), colori pieni
// per il contrasto. Si percorre avanti e indietro (niente salto tra giallo e ciano).
// giro: posizione nella palette; bianco 0..1: quota di celle bianche; s: soglia della cella.
u32 neon(f32 giro, f32 bianco, f32 s) {
    struct Colore { f32 r, g, b; };
    static constexpr Colore SCALA[4] = {{0, 230, 255}, {150, 60, 255}, {255, 40, 180}, {230, 255, 50}};
    if (bianco > s) return BIANCO;
    const f32 f = giro - std::floor(giro);
    const f32 p = (1.0f - std::fabs(2.0f * f - 1.0f)) * 3.0f; // 0..3 e ritorno
    const i32 i = std::min(2, (i32)p);
    const Colore& c = (p - (f32)i) > s ? SCALA[i + 1] : SCALA[i];
    return rgb(c.r, c.g, c.b);
}

// Rettangolo con ombra di 1 px in basso a destra.
void conOmbra(Framebuffer& fb, i32 x0, i32 y0, i32 x1, i32 y1, u32 col) {
    fb.fillRect(x0 + 1, y0 + 1, x1 + 1, y1 + 1, OMBRA);
    fb.fillRect(x0, y0, x1, y1, col);
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
        s.gravita = 0.0f;
        return;
    }
}

void Visualizzatore::esplosione(i32 barra) {
    // Barra in cima: lampo, cappello sparato in alto, anello e 10 pezzi in tutte le direzioni.
    lampo_[barra] = LAMPO_S;
    anello_[barra] = 0.001f;
    anelloY_[barra] = barra_[barra] * (f32)ALTEZZA_MAX + 2.0f;
    picco_[barra] = std::max(picco_[barra], barra_[barra]);
    vPicco_[barra] = -SPARO_CAPPELLO;
    riposo_[barra] = RIPOSO_S;
    i32 n = 0;
    for (Scintilla& s : scintille_) {
        if (s.vita > 0.0f) continue;
        const f32 a = ((f32)n + caso() * 0.6f) * 0.628f; // 10 direzioni intorno
        const f32 v = 45.0f + 45.0f * caso();
        s.x = (f32)(barra * PASSO + 1);
        s.y = -anelloY_[barra];
        s.vx = std::cos(a) * v;
        s.vy = std::sin(a) * v - 30.0f; // spinta verso l'alto
        s.vita = 0.35f + 0.25f * caso();
        s.tinta = caso();
        s.gravita = 170.0f;
        if (++n == 10) return;
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
        riposo_[b] = std::max(0.0f, riposo_[b] - dt);
        lampo_[b] = std::max(0.0f, lampo_[b] - dt);
        if (anello_[b] > 0.0f && (anello_[b] += dt) > ANELLO_S) anello_[b] = 0.0f;
        if (barra_[b] >= SOGLIA_PICCO && prima < SOGLIA_PICCO && riposo_[b] <= 0.0f) esplosione(b);
        else if (barra_[b] >= SOGLIA_SCINTILLA && prima < SOGLIA_SCINTILLA) scintilla(b, 1.0f);
        dalPicco_[b] += dt;
        if (barra_[b] >= picco_[b]) { picco_[b] = barra_[b]; vPicco_[b] = 0.0f; dalPicco_[b] = 0.0f; }
        else {
            // Ricade accelerando; se e' stato sparato in alto prima sale e rallenta.
            vPicco_[b] += GRAVITA_PICCO * dt;
            picco_[b] = std::clamp(picco_[b] - vPicco_[b] * dt, barra_[b], CAPPELLO_MAX);
        }
    }
    ondaRaggio_ += ONDA_VELOCITA * dt;
    ondaForza_ = std::max(0.0f, ondaForza_ - dt * 1.8f);

    for (Scintilla& s : scintille_) {
        if (s.vita <= 0.0f) continue;
        if (s.gravita > 0.0f) s.vy += s.gravita * dt;           // pezzo d'esplosione: vola e ricade
        else {
            s.vy *= std::max(0.0f, 1.0f - 2.0f * dt);           // scintilla: sale, rallenta, ondeggia
            s.x += 6.0f * std::sin(fase_ * 1.3f + s.tinta * 20.0f) * dt;
        }
        s.x += s.vx * dt;
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
        // Lampo dopo un colpo in cima: LED alternati bianco e giallo che si scambiano a ogni frame.
        const i32 strobo = lampo_[b] > 0.0f ? 1 + ((i32)(lampo_[b] * 60.0f) & 1) : 0;
        // LED da 2 px con 1 px di aria e ombra; l'ultimo e' mezzo LED se la barra
        // non ci arriva (sale e scende di un pixel alla volta). Il colore sale lungo
        // la palette con l'altezza, la punta a volte e' bianca, l'onda del battito
        // accende i LED a trama.
        for (i32 y = 0, led = 0; y < h; y += LED, ++led) {
            const i32 righe = std::min(LED - 1, h - y);
            const f32 s = soglia(b, led);
            u32 col;
            if (strobo) col = ((led + strobo) & 1) ? BIANCO : neon(0.5f, 0.0f, 0.5f);
            else col = neon(t0 + 0.2f * (f32)y / (f32)ALTEZZA_MAX, (y + LED >= h ? 0.25f : 0.0f) + 0.4f * o, s);
            conOmbra(fb, x, yBase - y - righe + 1, x + LARGHEZZA - 1, yBase - y, col);
        }
        // Alone a trama sopra la punta mentre passa l'onda: 3 righe sempre piu' rade.
        if (o > 0.05f) {
            static constexpr f32 DENSITA[3] = {0.6f, 0.35f, 0.15f};
            for (i32 r = 0; r < 3; ++r) {
                const i32 py = yBase - h - r;
                for (i32 px = x - 1; px <= x + LARGHEZZA; ++px)
                    if (soglia(px, py) < DENSITA[r] * o) fb.put(px, py, neon(t0 + 0.3f, 0.0f, 0.5f));
            }
        }
        // Cappello: bianco appena spinto su (o sparato), poi del colore della barra mentre ricade.
        const i32 yp = yBase - (i32)(picco_[b] * (f32)ALTEZZA_MAX + 0.5f) - alza - 2;
        if (picco_[b] > 0.04f)
            conOmbra(fb, x, yp, x + LARGHEZZA - 1, yp, dalPicco_[b] < 0.15f || vPicco_[b] < 0.0f ? BIANCO : neon(t0 + 0.3f, 0.0f, 0.5f));
        // Anello che si allarga dal punto del colpo: un rombo di pixel che si dirada.
        if (anello_[b] > 0.0f) {
            const f32 k = anello_[b] / ANELLO_S;
            const i32 r = 2 + (i32)(k * ANELLO_RAGGIO);
            const i32 cx = x + 1, cy = yBase - (i32)anelloY_[b];
            for (i32 dy = -r; dy <= r; ++dy) {
                const i32 py = cy + dy;
                if (py > yBase) continue; // niente sotto la linea di base
                const i32 dx = r - std::abs(dy);
                for (i32 px : {cx - dx, cx + dx})
                    if (soglia(px, py) >= k) fb.put(px, py, k < 0.4f ? BIANCO : neon(t0 + 0.6f, 0.0f, 0.5f));
            }
        }
        // Linea di base a puntini (1 px sotto i LED), piena e chiara dove passa l'onda.
        for (i32 px = x - 1; px < x + LARGHEZZA; ++px)
            if (((px - x0) & 1) == 0 || o > 0.3f) fb.put(px, yBase + 2, neon(t0, 0.75f * o, soglia(px, yBase + 2)));
    }
    // Scintille e pezzi d'esplosione: un pixel. Le scintille si diradano alla
    // fine; i pezzi cambiano colore a ogni frame e nascono bianchi.
    for (const Scintilla& s : scintille_) {
        if (s.vita <= 0.0f) continue;
        const i32 px = x0 + (i32)s.x, py = yBase + (i32)s.y;
        if (py > yBase) continue; // niente sotto la linea di base
        if (s.gravita > 0.0f) {
            fb.put(px, py, s.vita > 0.45f ? BIANCO : neon(s.tinta + s.vita * 6.0f, 0.0f, 0.5f));
            continue;
        }
        if (s.vita < 0.3f && soglia(px, py) > s.vita / 0.3f) continue;
        fb.put(px, py, neon(s.tinta, s.vita > 0.5f ? 0.6f : 0.0f, soglia(px, py)));
    }
}

} // namespace sgl
