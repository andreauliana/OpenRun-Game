// SPDX-License-Identifier: MIT
// Segnale dei personaggi da fermo (Andrea 11/11). Vedi segnale_personaggi.h.
#include "render/segnale_personaggi.h"
#include "render/framebuffer.h"
#include <cmath>

namespace sgl {

namespace {

constexpr u8 BAYER[16] = {0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5};
inline bool trama(i32 x, i32 y, f32 k) { return k * 16.0f > (f32)BAYER[((y & 3) << 2) | (x & 3)]; }

constexpr u32 SCURO = 0xFF101018u;
constexpr u32 BIANCO = 0xFFF4F0FFu;

struct Colori { u32 chiaro, pieno, ombra; };
constexpr Colori VERDE = {0xFFB8FFC8u, 0xFF34E07Au, 0xFF13663Au}; // Gianni
constexpr Colori BLU = {0xFF9CC4FFu, 0xFF2E6BFFu, 0xFF16246Au};   // Gesu'

// Centro del segnale: dove entreranno i personaggi (ShowLayout x = 240),
// sopra il prato a destra, sotto l'HUD.
constexpr i32 CX = 252, CY = 100;

void punto(Framebuffer& fb, i32 x, i32 y, u32 c, f32 k) {
    if (x < 0 || y < 0 || x >= Framebuffer::WIDTH || y >= Framebuffer::HEIGHT) return;
    if (k < 1.0f && !trama(x, y, k)) return; // comparsa/uscita a trama
    fb.put(x, y, c);
}

// Cubetto che gira: quadrato ruotato a scatti di 15 gradi (come fotogrammi
// di uno sprite), bordo scuro, faccia chiara in alto a sinistra, alone a trama.
void cubetto(Framebuffer& fb, i32 cx, i32 cy, f32 ang, const Colori& c, f32 k) {
    constexpr f32 PASSO = 3.14159265f / 12.0f;
    ang = std::floor(ang / PASSO) * PASSO;
    const f32 cs = std::cos(ang), sn = std::sin(ang);
    constexpr f32 R = 5.5f;
    for (i32 y = -11; y <= 11; ++y)
        for (i32 x = -11; x <= 11; ++x) {
            const f32 u = (f32)x * cs + (f32)y * sn;
            const f32 v = -(f32)x * sn + (f32)y * cs;
            const f32 m = std::fmax(std::fabs(u), std::fabs(v));
            if (m <= R - 1.0f) {
                // faccia: chiara verso l'alto a sinistra, ombra in basso a destra
                const f32 luce = (f32)(-x - y);
                punto(fb, cx + x, cy + y, luce > 2.0f ? c.chiaro : (luce < -3.0f ? c.ombra : c.pieno), k);
            } else if (m <= R + 0.5f) {
                punto(fb, cx + x, cy + y, SCURO, k);
            } else {
                const f32 d = std::sqrt((f32)(x * x + y * y));
                if (d < 11.0f && trama(cx + x, cy + y, 0.35f * (1.0f - d / 11.0f))) punto(fb, cx + x, cy + y, c.pieno, k);
            }
        }
    punto(fb, cx - 1, cy - 2, BIANCO, k); // riflesso
}

// Fumetto piccolo 21x11 con la coda in basso e i puntini che si scrivono.
void fumetto(Framebuffer& fb, i32 cx, i32 y0, f32 t, f32 k) {
    constexpr i32 W = 21, H = 11;
    const i32 x0 = cx - W / 2;
    for (i32 y = 0; y < H; ++y)
        for (i32 x = 0; x < W; ++x) {
            const bool angolo = (x == 0 || x == W - 1) && (y == 0 || y == H - 1);
            if (angolo) continue;
            const bool bordo = x == 0 || y == 0 || x == W - 1 || y == H - 1;
            punto(fb, x0 + x, y0 + y, bordo ? SCURO : BIANCO, k);
        }
    // coda verso i cubetti
    punto(fb, cx - 1, y0 + H - 1, BIANCO, k);
    punto(fb, cx, y0 + H - 1, BIANCO, k);
    punto(fb, cx - 1, y0 + H, SCURO, k); punto(fb, cx, y0 + H, BIANCO, k); punto(fb, cx + 1, y0 + H, SCURO, k);
    punto(fb, cx, y0 + H + 1, SCURO, k);
    // puntini: uno ogni 0.3 s, poi una pausa, e si ricomincia
    const i32 n = (i32)std::fmod(t / 0.3f, 5.0f); // 0..4: 0,1,2,3 puntini, 4 = tutti (pausa)
    const i32 accesi = n > 3 ? 3 : n;
    for (i32 i = 0; i < accesi; ++i) {
        const i32 px = cx - 6 + i * 5;
        const i32 py = y0 + 4;
        punto(fb, px, py, SCURO, k); punto(fb, px + 1, py, SCURO, k);
        punto(fb, px, py + 1, SCURO, k); punto(fb, px + 1, py + 1, SCURO, k);
    }
}

} // namespace

void disegnaSegnalePersonaggi(Framebuffer& fb, f32 t, f32 k) {
    if (k <= 0.0f) return;
    // I due cubetti girano veloci su se stessi e uno attorno all'altro,
    // con un piccolo saltello: Gianni (verde) e Gesu' (blu) che scalpitano.
    const f32 giro = t * 2.2f;                       // giro lento attorno al centro
    const i32 ox = (i32)std::lround(11.0f * std::cos(giro));
    const i32 oy = (i32)std::lround(3.0f * std::sin(giro)); // ellisse schiacciata: profondita'
    const i32 salto = (std::fmod(t, 0.8f) < 0.12f) ? -1 : 0;
    // Prima quello "dietro" (y piu' piccola), poi quello davanti.
    const bool verdeDietro = oy < 0;
    const i32 vx = CX + ox, vy = CY + oy + salto;
    const i32 bx = CX - ox, by = CY - oy + salto;
    if (verdeDietro) {
        cubetto(fb, vx, vy, t * 9.0f, VERDE, k);
        cubetto(fb, bx, by, -t * 9.0f, BLU, k);
    } else {
        cubetto(fb, bx, by, -t * 9.0f, BLU, k);
        cubetto(fb, vx, vy, t * 9.0f, VERDE, k);
    }
    // Il fumetto arriva dopo i cubetti (come la bolla degli show).
    if (t > 0.8f) fumetto(fb, CX, CY - 30, t - 0.8f, k);
}

} // namespace sgl
