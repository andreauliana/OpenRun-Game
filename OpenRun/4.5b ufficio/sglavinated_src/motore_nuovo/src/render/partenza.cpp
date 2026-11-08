// SPDX-License-Identifier: MIT
// Conto alla rovescia della partenza (Andrea 08/11). Vedi partenza.h.
// Pixel art: luci piene e aloni a trama Bayer, contorno scuro alle cifre,
// tutto calcolato dal tempo: deterministico, nessuna allocazione.
#include "render/partenza.h"
#include "render/framebuffer.h"
#include "render/renderer.h"
#include <algorithm>
#include <cmath>

namespace sgl {

namespace {

constexpr u8 BAYER[16] = {0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5};
inline bool trama(i32 x, i32 y, f32 k) { return k * 16.0f > (f32)BAYER[((y & 3) << 2) | (x & 3)]; }

constexpr u32 SCURO = 0xFF101018u;
constexpr u32 ROSSO = 0xFFFF3030u, ROSSO_SPENTO = 0xFF401418u;
constexpr u32 VERDE = 0xFF40FF70u;
// Una cifra per colore: 3 rosso, 2 arancio, 1 giallo, START verde.
constexpr u32 COLORE_CONTO[4] = {VERDE, 0xFFFFE040u, 0xFFFF9A2Au, ROSSO};

void punto(Framebuffer& fb, i32 x, i32 y, u32 c) {
    if (x >= 0 && y >= 0 && x < Framebuffer::WIDTH && y < Framebuffer::HEIGHT) fb.fillRect(x, y, x, y, c);
}

// Testo con contorno scuro di 1 px per scala (si legge su cielo e skyline).
void testoContornato(Renderer& ren, i32 cx, i32 cy, const char* t, u32 c, i32 scala) {
    const i32 w = Renderer::larghezzaTesto(t, scala);
    const i32 h = 8 * scala;
    const i32 x = cx - w / 2, y = cy - h / 2;
    const i32 o = std::max(1, scala / 2);
    for (i32 dy = -o; dy <= o; dy += o)
        for (i32 dx = -o; dx <= o; dx += o)
            if (dx || dy) ren.drawTesto(x + dx, y + dy, t, SCURO, scala, false);
    ren.drawTesto(x, y, t, c, scala, false);
}

// Luce rotonda (raggio r) con alone a trama quando e' accesa.
void luce(Framebuffer& fb, i32 cx, i32 cy, i32 r, u32 c, bool accesa, f32 alone) {
    if (accesa && alone > 0.0f) {
        const i32 ra = r + 6;
        for (i32 y = -ra; y <= ra; ++y)
            for (i32 x = -ra; x <= ra; ++x) {
                const f32 d = std::sqrt((f32)(x * x + y * y));
                if (d <= (f32)r || d > (f32)ra) continue;
                const f32 k = alone * (1.0f - (d - (f32)r) / 6.0f) * 0.6f;
                if (trama(cx + x, cy + y, k)) punto(fb, cx + x, cy + y, c);
            }
    }
    for (i32 y = -r; y <= r; ++y)
        for (i32 x = -r; x <= r; ++x)
            if (x * x + y * y <= r * r) punto(fb, cx + x, cy + y, accesa ? c : ROSSO_SPENTO);
    if (accesa) punto(fb, cx - r / 2, cy - r / 2, 0xFFFFFFFFu); // riflesso
}

} // namespace

i32 contoPartenza(f32 t) {
    if (t < PARTENZA_ATTESA_S) return -1;
    if (t < PARTENZA_VIA_S) return 3 - (i32)((t - PARTENZA_ATTESA_S) / PARTENZA_PASSO_S);
    if (t < PARTENZA_FINE_S) return 0;
    return -2;
}

void disegnaPartenza(Framebuffer& fb, Renderer& ren, f32 t) {
    const i32 conto = contoPartenza(t);
    if (conto == -2) return;
    const i32 cx = Framebuffer::WIDTH / 2;

    // Semaforo: barra scura con tre luci sopra la scritta. Si accende una
    // luce rossa per ogni numero; allo START tutte verdi.
    const i32 ly = PARTENZA_CENTRO_Y - 40;
    fb.fillRect(cx - 34, ly - 9, cx + 34, ly + 9, SCURO);
    fb.fillRect(cx - 34, ly - 9, cx + 34, ly - 9, 0xFF505060u);
    const i32 accese = conto < 0 ? 0 : (conto == 0 ? 3 : 4 - conto);
    for (i32 i = 0; i < 3; ++i) {
        const bool on = i < accese;
        // L'ultima luce accesa pulsa un attimo quando si accende.
        const f32 dal = conto > 0 ? std::fmod(t - PARTENZA_ATTESA_S, PARTENZA_PASSO_S) : t - PARTENZA_VIA_S;
        const f32 alone = (i == accese - 1 || conto == 0) ? std::max(0.35f, 1.0f - dal * 1.5f) : 0.35f;
        luce(fb, cx - 22 + i * 22, ly, 6, conto == 0 ? VERDE : ROSSO, on, alone);
    }

    if (conto < 0) {
        // Attesa: "GET READY" fermo, si spegne solo un attimo prima del 3.
        if (t < PARTENZA_ATTESA_S - 0.2f) testoContornato(ren, cx, PARTENZA_CENTRO_Y, "GET READY", 0xFFFFFFFFu, 3);
        return;
    }
    // Cifra o START: entra grande e si assesta (8 -> 6), sparisce poco prima del numero dopo.
    const f32 dal = conto > 0 ? std::fmod(t - PARTENZA_ATTESA_S, PARTENZA_PASSO_S) : t - PARTENZA_VIA_S;
    if (conto > 0) {
        if (dal > 0.82f) return;
        // Disco scuro a trama dietro la cifra: si legge sopra skyline e insegne.
        const i32 r = 30;
        for (i32 y = -r; y <= r; ++y)
            for (i32 x = -r; x <= r; ++x) {
                const f32 d = std::sqrt((f32)(x * x + y * y)) / (f32)r;
                if (d <= 1.0f && trama(cx + x, PARTENZA_CENTRO_Y + y, 0.85f - 0.5f * d))
                    punto(fb, cx + x, PARTENZA_CENTRO_Y + y, SCURO);
            }
        const i32 scala = dal < 0.05f ? 8 : (dal < 0.1f ? 7 : 6);
        const char cifra[2] = {(char)('0' + conto), 0};
        testoContornato(ren, cx, PARTENZA_CENTRO_Y, cifra, COLORE_CONTO[conto], scala);
    } else {
        const i32 scala = dal < 0.05f ? 6 : 5;
        // START lampeggia verde/bianco nell'ultimo terzo, poi si spegne.
        const bool bianco = dal > PARTENZA_START_S * 0.66f && std::fmod(dal, 0.16f) < 0.08f;
        testoContornato(ren, cx, PARTENZA_CENTRO_Y, "START!", bianco ? 0xFFFFFFFFu : VERDE, scala);
    }
}

} // namespace sgl
