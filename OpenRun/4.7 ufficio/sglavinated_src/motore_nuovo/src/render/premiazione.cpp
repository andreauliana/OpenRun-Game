// SPDX-License-Identifier: MIT
// Premiazione all'arrivo (Andrea 07/11): festa intorno all'auto ferma sul
// traguardo, diversa per tema e per posizione. Vedi premiazione.h.
#include "render/premiazione.h"
#include "render/framebuffer.h"
#include "render/renderer.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

namespace sgl {

namespace {

constexpr f32 PI = 3.14159265f;

// Pochi colori per tema (Andrea 29/10: palette limitata + trama).
// 0 principale, 1 secondo, 2 accento, 3 luce, 4 scuro.
struct ColoriPremio { u32 c[5]; };
constexpr ColoriPremio COLORI[6] = {
    {{0xFFFF2AD4u, 0xFF22E8FFu, 0xFF8A3CFFu, 0xFFFFFFFFu, 0xFF1A0A3Au}}, // neon: synthwave
    {{0xFF9FF0C8u, 0xFFFFA8C8u, 0xFFFFF0A0u, 0xFFE8503Au, 0xFF3A4A5Au}}, // atomico: pastelli anni '50, latta
    {{0xFF6FD04Au, 0xFFFF9A2Au, 0xFFC060F0u, 0xFFFFE060u, 0xFF2A3A10u}}, // foglie: psichedelico
    {{0xFFC8D4E8u, 0xFF7AE0FFu, 0xFFFFD040u, 0xFFFFFFFFu, 0xFF4A5878u}}, // cromo: argento, ghiaccio, oro
    {{0xFFFF3A9Au, 0xFFFFE020u, 0xFF30E0E0u, 0xFF90FF30u, 0xFF141414u}}, // spray: vernici punk
    {{0xFF008080u, 0xFFC0C0C0u, 0xFF000080u, 0xFFFFFFFFu, 0xFF000000u}}, // desktop: finestre anni '90
};

// Trama Bayer 4x4: soglia fissa per pixel (0..15).
constexpr u8 BAYER[16] = {0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5};
inline bool trama(i32 x, i32 y, f32 k) { return k * 16.0f > (f32)BAYER[((y & 3) << 2) | (x & 3)]; }

inline u32 hash(u32 x) {
    x ^= x >> 16; x *= 0x7FEB352Du; x ^= x >> 15; x *= 0x846CA68Bu; x ^= x >> 16;
    return x;
}
inline f32 caso(u32 i, u32 sale) { return (f32)(hash(i * 2654435761u + sale) & 0xFFFFu) / 65535.0f; }
inline f32 fraz(f32 v) { return v - std::floor(v); }

// Misure dell'auto a schermo: le stesse costanti del renderer (bloccate,
// REGOLE_AUTO_GIOCATORE.md), cosi' se cambia la grandezza la festa segue.
struct Auto {
    i32 cx, terra, w, h, alto;
};
Auto misure(const DatiPremiazione& d) {
    Auto a;
    a.cx = d.autoX;
    a.terra = AUTO_GIOCATORE_TERRA_Y;
    a.w = AUTO_GIOCATORE_LARGHEZZA;
    a.h = a.w * 3 / 5;
    a.alto = a.terra - a.h;
    return a;
}
// 2 = primo, 1 = podio, 0 = gli altri.
inline i32 livello(const DatiPremiazione& d) { return d.posizione <= 1 ? 2 : (d.posizione <= 3 ? 1 : 0); }

void blocco(Framebuffer& fb, i32 x, i32 y, i32 s, u32 c) { fb.fillRect(x, y, x + s - 1, y + s - 1, c); }

void linea(Framebuffer& fb, f32 x0, f32 y0, f32 x1, f32 y1, u32 c, f32 kFine = 1.0f) {
    const i32 n = (i32)std::max(std::abs(x1 - x0), std::abs(y1 - y0)) + 1;
    for (i32 i = 0; i <= n; ++i) {
        const f32 k = (f32)i / (f32)n;
        const i32 x = (i32)(x0 + (x1 - x0) * k), y = (i32)(y0 + (y1 - y0) * k);
        if (kFine < 1.0f && !trama(x, y, 1.0f - k * (1.0f - kFine))) continue; // la punta sfuma a trama
        fb.put(x, y, c);
    }
}

// Alone ellittico a trama: pieno al centro, puntinato verso il bordo.
void alone(Framebuffer& fb, i32 cx, i32 cy, i32 rx, i32 ry, f32 forza, u32 c) {
    if (rx <= 0 || ry <= 0) return;
    for (i32 y = cy - ry; y <= cy + ry; ++y)
        for (i32 x = cx - rx; x <= cx + rx; ++x) {
            const f32 dx = (f32)(x - cx) / (f32)rx, dy = (f32)(y - cy) / (f32)ry;
            const f32 r = dx * dx + dy * dy;
            if (r < 1.0f && trama(x, y, (1.0f - r) * forza)) fb.put(x, y, c);
        }
}

// Anello su un'ellisse: parte dietro (sin < 0) o davanti all'auto.
void anello(Framebuffer& fb, f32 cx, f32 cy, f32 rx, f32 ry, bool davanti, u32 c, u32 luce, f32 forza) {
    const i32 n = std::max(24, (i32)(rx * 3.0f));
    for (i32 i = 0; i < n; ++i) {
        const f32 a = 2.0f * PI * (f32)i / (f32)n;
        const f32 s = std::sin(a);
        if ((s >= 0.0f) != davanti) continue;
        const i32 x = (i32)(cx + rx * std::cos(a)), y = (i32)(cy + ry * s);
        if (!trama(x, y, forza)) continue;
        const u32 col = (davanti && std::abs(std::cos(a)) < 0.35f) ? luce : c; // riflesso davanti
        fb.put(x, y, col);
        fb.put(x, y + 1, col);
    }
}

// --- Icone sopra l'auto (pixel a mano, 11x12) ---------------------------
// '.' vuoto, 'o' oro/metallo, 's' ombra, 'l' luce, 'r' nastro, 'k' nero, 'w' bianco
const char* const TROFEO[12] = {
    "o.ooooooo.o",
    "o.olllooo.o",
    "o.olooooo.o",
    "oooloooosoo",
    "..ooooooss.",
    "...ooooss..",
    "....ooss...",
    ".....os....",
    ".....os....",
    "...ooooss..",
    "..ooooooss.",
    "..sssssss..",
};
const char* const MEDAGLIA[12] = {
    "rr.......rr",
    ".rr.....rr.",
    "..rr...rr..",
    "...rrrrr...",
    "...ooooo...",
    "..olloooo..",
    ".ollooooos.",
    ".olooooooss",
    ".ooooooooss",
    ".ooooooooss",
    "..ooooooss.",
    "...osssss..",
};
const char* const BANDIERA[12] = {
    "skwkwkwkw..",
    "swkwkwkwk..",
    "skwkwkwkw..",
    "swkwkwkwk..",
    "skwkwkwkw..",
    "s..........",
    "s..........",
    "s..........",
    "s..........",
    "s..........",
    "s..........",
    "s..........",
};

void icona(Framebuffer& fb, const char* const righe[12], i32 x0, i32 y0, i32 s, u32 oro, u32 ombra, u32 luce, u32 nastro) {
    for (i32 r = 0; r < 12; ++r)
        for (i32 k = 0; k < 11 && righe[r][k]; ++k) {
            u32 c = 0;
            switch (righe[r][k]) {
                case 'o': c = oro; break;
                case 's': c = ombra; break;
                case 'l': c = luce; break;
                case 'r': c = nastro; break;
                case 'k': c = 0xFF101010u; break;
                case 'w': c = 0xFFF0F0F0u; break;
                default: continue;
            }
            blocco(fb, x0 + k * s, y0 + r * s, s, c);
        }
}

// --- Effetti per tema ---------------------------------------------------

// Neon: raggi laser a ventaglio da dietro l'auto, che oscillano.
void laserNeon(Framebuffer& fb, const Auto& a, const ColoriPremio& p, i32 lv, f32 t) {
    if (t < 0.15f) return;
    const i32 n = 2 + 2 * lv;
    const f32 lungo = std::min(1.0f, (t - 0.15f) / 0.35f) * 260.0f;
    const f32 x0 = (f32)a.cx, y0 = (f32)(a.alto + a.h / 3);
    for (i32 i = 0; i < n; ++i) {
        const f32 ang = ((f32)i - (f32)(n - 1) * 0.5f) * 0.30f + 0.22f * std::sin(t * 1.8f + (f32)i * 1.3f);
        const f32 x1 = x0 + std::sin(ang) * lungo, y1 = y0 - std::cos(ang) * lungo;
        const u32 col = (i & 1) ? p.c[1] : p.c[0];
        linea(fb, x0 - 1, y0, x1 - 1, y1, col, 0.25f);
        linea(fb, x0 + 1, y0, x1 + 1, y1, col, 0.25f);
        linea(fb, x0, y0, x1, y1, p.c[3], 0.25f);
    }
}

// Neon: scintille che cadono e lampeggiano.
void scintilleNeon(Framebuffer& fb, const ColoriPremio& p, i32 lv, f32 t) {
    if (t < 0.3f) return;
    const i32 n = 12 + 12 * lv;
    for (i32 i = 0; i < n; ++i) {
        const f32 v = 0.25f + 0.35f * caso((u32)i, 1);
        const f32 q = fraz((t - 0.3f) * v + caso((u32)i, 2));
        const i32 x = 20 + (i32)(caso((u32)i, 3) * 280.0f) + (i32)(std::sin(t * 2.0f + (f32)i) * 4.0f);
        const i32 y = 24 + (i32)(q * 170.0f);
        if (((i32)(t * 12.0f) + i) % 5 == 0) continue; // lampeggio
        const u32 col = p.c[i % 3 == 0 ? 3 : (i & 1)];
        fb.put(x, y, col);
        if (i % 4 == 0) { fb.put(x - 1, y, col); fb.put(x + 1, y, col); fb.put(x, y - 1, col); fb.put(x, y + 1, col); }
    }
}

// Atomico: orbite di elettroni intorno all'auto; davanti o dietro.
void orbiteAtomico(Framebuffer& fb, const Auto& a, const ColoriPremio& p, i32 lv, f32 t, bool davanti) {
    if (t < 0.2f) return;
    const i32 n = 1 + lv;
    const f32 cx = (f32)a.cx, cy = (f32)(a.terra - a.h * 70 / 100);
    const f32 rx = (f32)a.w * 0.95f, ry = (f32)a.h * 0.55f;
    const f32 crescita = std::min(1.0f, (t - 0.2f) / 0.4f);
    for (i32 j = 0; j < n; ++j) {
        const f32 th = (f32)j * PI / 3.0f - 0.35f;
        const f32 ct = std::cos(th), st = std::sin(th);
        // percorso puntinato
        for (i32 i = 0; i < 64; i += 2) {
            const f32 ang = 2.0f * PI * (f32)i / 64.0f;
            if ((std::sin(ang) >= 0.0f) != davanti) continue;
            const f32 ex = rx * crescita * std::cos(ang), ey = ry * crescita * std::sin(ang);
            fb.put((i32)(cx + ex * ct - ey * st), (i32)(cy + ex * st + ey * ct), p.c[j % 2 ? 1 : 0]);
        }
        // elettrone con scia
        for (i32 s = 4; s >= 0; --s) {
            const f32 ang = t * 4.2f + (f32)j * 2.1f - (f32)s * 0.12f;
            if ((std::sin(ang) >= 0.0f) != davanti) continue;
            const f32 ex = rx * crescita * std::cos(ang), ey = ry * crescita * std::sin(ang);
            const i32 x = (i32)(cx + ex * ct - ey * st), y = (i32)(cy + ex * st + ey * ct);
            if (s == 0) { fb.fillRect(x - 1, y - 1, x + 1, y + 1, p.c[2]); fb.put(x, y, 0xFFFFFFFFu); }
            else fb.put(x, y, p.c[1]);
        }
    }
}

// Atomico: razzo di latta che parte da dietro l'auto e scoppia in stelle pastello.
void razzoAtomico(Framebuffer& fb, const Auto& a, const ColoriPremio& p, f32 t) {
    if (t < 0.5f) return;
    const f32 q = std::fmod(t - 0.5f, 2.6f) / 1.3f;
    const i32 xb = a.cx + a.w / 3;
    if (q < 1.0f) {
        const i32 x = xb + (i32)(std::sin(q * 3.0f) * 6.0f);
        const i32 y = a.alto - (i32)(q * q * 120.0f);
        fb.fillRect(x - 1, y - 6, x + 1, y + 2, 0xFFB8C0C8u);      // corpo di latta
        fb.fillRect(x, y - 6, x + 1, y + 1, 0xFFE8ECF0u);          // riflesso
        fb.fillRect(x - 1, y - 8, x + 1, y - 7, p.c[3]);           // punta rossa
        fb.put(x, y - 9, p.c[3]);
        fb.fillRect(x - 3, y + 1, x - 2, y + 3, p.c[3]);           // alette
        fb.fillRect(x + 2, y + 1, x + 3, y + 3, p.c[3]);
        const i32 fiamma = 2 + (((i32)(t * 30.0f)) & 3);
        fb.fillRect(x - 1, y + 3, x + 1, y + 3 + fiamma, p.c[2]);
        fb.put(x, y + 4 + fiamma, 0xFFFFFFFFu);
        for (i32 s = 0; s < 6; ++s) // fumo puntinato
            if (trama(x, y + 8 + s * 3, 0.6f - 0.1f * (f32)s)) fb.put(x + (s & 1), y + 8 + s * 3, p.c[1]);
    } else if (q < 2.0f) {
        const f32 k = q - 1.0f;
        const i32 x = xb + (i32)(std::sin(3.0f) * 6.0f), y = a.alto - 120;
        for (i32 i = 0; i < 14; ++i) {
            const f32 ang = 2.0f * PI * (f32)i / 14.0f;
            const i32 px = x + (i32)(std::cos(ang) * k * 42.0f);
            const i32 py = y + (i32)(std::sin(ang) * k * 42.0f + k * k * 14.0f);
            if (!trama(px, py, 1.0f - k * 0.9f)) continue;
            const u32 col = p.c[i % 3];
            fb.put(px, py, col); fb.put(px - 1, py, col); fb.put(px + 1, py, col); fb.put(px, py - 1, col); fb.put(px, py + 1, col);
        }
    }
}

// Foglie: vortice di foglie che sale intorno all'auto (davanti/dietro).
void vorticeFoglie(Framebuffer& fb, const Auto& a, const ColoriPremio& p, i32 lv, f32 t, bool davanti) {
    if (t < 0.15f) return;
    const i32 n = 10 + 8 * lv;
    for (i32 k = 0; k < n; ++k) {
        const f32 sal = fraz(t * 0.32f + (f32)k / (f32)n);
        const f32 ang = t * 2.6f + (f32)k * 2.0f * PI / (f32)n * 3.0f;
        if ((std::sin(ang) >= 0.0f) != davanti) continue;
        const f32 r = (f32)a.w * (0.55f + 0.45f * sal);
        const i32 x = a.cx + (i32)(r * std::cos(ang));
        const i32 y = a.terra - 6 - (i32)(sal * (f32)a.h * 2.8f) + (i32)(std::sin(ang) * r * 0.22f);
        if (!trama(x, y, 1.6f - sal * 1.6f)) continue; // in alto si diradano
        const u32 col = p.c[k % 4];
        if (((i32)(t * 8.0f) + k) & 1) fb.fillRect(x, y, x + 2, y + 1, col); // foglia che gira
        else fb.fillRect(x, y, x + 1, y + 2, col);
        fb.put(x + 1, y + 1, p.c[4]); // nervatura
    }
}

// Foglie: cerchi psichedelici che si allargano dietro l'auto, colori che girano.
void cerchiFoglie(Framebuffer& fb, const Auto& a, const ColoriPremio& p, i32 lv, f32 t) {
    if (t < 0.1f) return;
    const i32 n = 2 + lv;
    for (i32 i = 0; i < n; ++i) {
        const f32 q = fraz(t * 0.45f + (f32)i / (f32)n);
        const f32 rx = (f32)a.w * (0.4f + 1.6f * q);
        const u32 col = p.c[((i32)(t * 3.0f) + i) % 4];
        anello(fb, (f32)a.cx, (f32)(a.terra - a.h / 2), rx, rx * 0.55f, false, col, col, 1.0f - q);
        anello(fb, (f32)a.cx, (f32)(a.terra - a.h / 2), rx, rx * 0.55f, true, col, col, (1.0f - q) * 0.5f);
    }
}

// Cromo: anelli cromati che salgono dal suolo (davanti/dietro).
void anelliCromo(Framebuffer& fb, const Auto& a, const ColoriPremio& p, i32 lv, f32 t, bool davanti) {
    if (t < 0.1f) return;
    const i32 n = 2 + lv;
    for (i32 i = 0; i < n; ++i) {
        const f32 q = fraz(t * 0.55f + (f32)i / (f32)n);
        const f32 y = (f32)a.terra - 3.0f - q * (f32)a.h * 2.3f;
        const f32 rx = (f32)a.w * (0.68f - 0.12f * q);
        anello(fb, (f32)a.cx, y, rx, rx * 0.2f, davanti, i == 0 ? p.c[2] : p.c[0], p.c[3], 1.2f - q);
    }
}

// Cromo: stelline a quattro punte che brillano intorno alla carrozzeria.
void stelleCromo(Framebuffer& fb, const Auto& a, const ColoriPremio& p, i32 lv, f32 t) {
    if (t < 0.3f) return;
    const i32 n = 4 + 3 * lv;
    for (i32 i = 0; i < n; ++i) {
        const f32 periodo = 0.7f + 0.5f * caso((u32)i, 7);
        const f32 q = fraz(t / periodo + caso((u32)i, 8));
        const u32 ciclo = (u32)((t / periodo) + caso((u32)i, 8)); // posizione nuova a ogni scintilla
        const i32 x = a.cx - a.w * 3 / 4 + (i32)(caso((u32)i * 31u + ciclo, 9) * (f32)a.w * 1.5f);
        const i32 y = a.alto - 18 + (i32)(caso((u32)i * 17u + ciclo, 10) * (f32)(a.h + 22));
        const i32 r = (i32)(std::sin(q * PI) * (2.0f + (f32)(i % 3)));
        if (r <= 0) continue;
        const u32 col = i % 3 == 0 ? p.c[2] : p.c[1];
        for (i32 k = 1; k <= r; ++k) { fb.put(x - k, y, col); fb.put(x + k, y, col); fb.put(x, y - k, col); fb.put(x, y + k, col); }
        fb.put(x, y, p.c[3]);
    }
}

// Spray: schizzi di vernice sparati dall'auto che restano sulla strada.
void schizziSpray(Framebuffer& fb, const Auto& a, const ColoriPremio& p, i32 lv, f32 t) {
    const i32 n = 4 + 4 * lv;
    for (i32 i = 0; i < n; ++i) {
        const f32 t0 = 0.2f + 0.2f * (f32)i;
        if (t < t0) continue;
        const f32 k = (t - t0) / 0.45f;
        const f32 lato = (i & 1) ? 1.0f : -1.0f;
        const f32 lx = (f32)a.cx + lato * (f32)a.w * (0.7f + 1.0f * caso((u32)i, 11));
        const f32 ly = (f32)a.terra - 4.0f - caso((u32)i, 12) * 34.0f;
        const u32 col = p.c[i % 4];
        if (k < 1.0f) {
            const f32 x = (f32)a.cx + (lx - (f32)a.cx) * k;
            const f32 y = (f32)a.alto + (ly - (f32)a.alto) * k - std::sin(PI * k) * 50.0f;
            fb.fillRect((i32)x - 1, (i32)y - 1, (i32)x + 1, (i32)y + 1, col);
            continue;
        }
        // macchia: cerchio irregolare con punte e gocce intorno
        const i32 r = 5 + (i32)(caso((u32)i, 13) * 5.0f + (f32)lv);
        const i32 cx = (i32)lx, cy = (i32)ly;
        const f32 schiaccia = 0.55f; // sulla strada la macchia e' vista di sbieco
        const f32 crescita = std::min(1.0f, (k - 1.0f) * 6.0f + 0.4f); // si allarga quando tocca terra
        for (i32 y = -r; y <= r; ++y)
            for (i32 x = -r - 3; x <= r + 3; ++x) {
                const f32 yy = (f32)y / schiaccia;
                const f32 ang = std::atan2(yy, (f32)x);
                const f32 bordo = (f32)r * crescita * (0.75f + 0.45f * caso((u32)(i * 16 + (i32)((ang + PI) * 2.5f)), 14));
                if ((f32)(x * x) + yy * yy <= bordo * bordo) fb.put(cx + x, cy + y, col);
            }
        for (i32 g = 0; g < 3; ++g) {
            const f32 ang = caso((u32)(i * 5 + g), 15) * 2.0f * PI;
            const i32 d = r + 3 + (i32)(caso((u32)(i * 5 + g), 16) * 5.0f);
            fb.put(cx + (i32)(std::cos(ang) * (f32)d), cy + (i32)(std::sin(ang) * (f32)d * schiaccia), col);
        }
    }
}

// Desktop: finestrina anni '90 con barra del titolo, testo e tasto OK.
void finestra(Framebuffer& fb, Renderer& ren, const ColoriPremio& p, i32 x, i32 y, i32 w, i32 h,
              f32 apertura, const char* titolo, const char* testo, bool premuto) {
    const i32 hv = std::max(3, (i32)((f32)h * std::min(1.0f, apertura)));
    const i32 y0 = y + (h - hv) / 2;
    fb.fillRect(x + 2, y0 + 2, x + w + 1, y0 + hv + 1, p.c[4]);   // ombra
    fb.fillRect(x, y0, x + w - 1, y0 + hv - 1, p.c[1]);
    fb.fillRect(x, y0, x + w - 1, y0, p.c[3]);                     // bordo in rilievo
    fb.fillRect(x, y0, x, y0 + hv - 1, p.c[3]);
    fb.fillRect(x, y0 + hv - 1, x + w - 1, y0 + hv - 1, 0xFF808080u);
    fb.fillRect(x + w - 1, y0, x + w - 1, y0 + hv - 1, 0xFF808080u);
    if (apertura < 1.0f) return;
    fb.fillRect(x + 2, y + 2, x + w - 3, y + 9, p.c[2]);            // barra del titolo
    ren.drawTesto(x + 4, y + 4, titolo, p.c[3], 0, false);
    fb.fillRect(x + w - 10, y + 3, x + w - 4, y + 8, p.c[1]);       // tasto chiudi
    ren.drawTesto(x + w - 9, y + 4, "X", p.c[4], 0, false);
    ren.drawTesto(x + w / 2 - Renderer::larghezzaTesto(testo, 1) / 2, y + 14, testo, p.c[4], 1, false);
    const i32 bx = x + w / 2 - 12, by = y + h - 13;
    fb.fillRect(bx, by, bx + 23, by + 9, p.c[1]);
    fb.fillRect(bx, by, bx + 23, by, premuto ? 0xFF808080u : p.c[3]);
    fb.fillRect(bx, by, bx, by + 9, premuto ? 0xFF808080u : p.c[3]);
    fb.fillRect(bx, by + 9, bx + 23, by + 9, premuto ? p.c[3] : p.c[4]);
    fb.fillRect(bx + 23, by, bx + 23, by + 9, premuto ? p.c[3] : p.c[4]);
    ren.drawTesto(bx + 8 + (premuto ? 1 : 0), by + 3 + (premuto ? 1 : 0), "OK", p.c[4], 0, false);
}

// Freccia del mouse (7x11), punta in (x, y).
void cursore(Framebuffer& fb, i32 x, i32 y) {
    static const char* const F[11] = {
        "k......", "kk.....", "kwk....", "kwwk...", "kwwwk..", "kwwwwk.",
        "kwwwwwk", "kwwwkkk", "kwkwk..", "kk.kwk.", "....kk.",
    };
    for (i32 r = 0; r < 11; ++r)
        for (i32 k = 0; k < 7; ++k)
            if (F[r][k] != '.') fb.put(x + k, y + r, F[r][k] == 'k' ? 0xFF000000u : 0xFFFFFFFFu);
}

void finestreDesktop(Framebuffer& fb, Renderer& ren, const Auto& a, const ColoriPremio& p, i32 lv, i32 pos, f32 t) {
    struct Posto { i32 dx, dy; };
    static constexpr Posto POSTI[3] = {{-150, -118}, {50, -132}, {-118, -64}};
    const i32 n = 1 + lv;
    char testo[16];
    if (pos == 1) std::snprintf(testo, sizeof(testo), "YOU WIN!");
    else if (pos <= 3) std::snprintf(testo, sizeof(testo), "PODIUM!");
    else std::snprintf(testo, sizeof(testo), "FINISHED");
    static const char* const TITOLI[3] = {"WINNER.EXE", "TROPHY.BMP", "RACE.TXT"};
    i32 okX = 0, okY = 0;
    f32 arrivoCursore = 0.0f;
    for (i32 i = 0; i < n; ++i) {
        const f32 t0 = 0.35f + 0.45f * (f32)i;
        if (t < t0) continue;
        const i32 x = std::clamp(a.cx + POSTI[i].dx, 4, 320 - 96);
        const i32 y = std::clamp(a.terra + POSTI[i].dy, 34, 224 - 48);
        const f32 click = 1.6f + 0.45f * (f32)n;
        finestra(fb, ren, p, x, y, 92, 42, (t - t0) / 0.15f, TITOLI[pos > 3 ? 2 : (pos > 1 ? 1 - i : i)], testo, i == n - 1 && t > click && t < click + 0.25f);
        okX = x + 46; okY = y + 33;
        arrivoCursore = t0 + 0.3f;
    }
    if (okX == 0) return;
    // il cursore va verso il tasto OK dell'ultima finestra e lo preme
    const f32 k = std::clamp((t - arrivoCursore) / 0.8f, 0.0f, 1.0f);
    const f32 e = k * k * (3.0f - 2.0f * k);
    const i32 sx = a.cx + 70, sy = a.terra - 12;
    cursore(fb, sx + (i32)((f32)(okX - sx) * e), sy + (i32)((f32)(okY - sy) * e));
}

// Coriandoli (primo posto, tutti i temi tranne neon che ha le scintille).
void coriandoli(Framebuffer& fb, const ColoriPremio& p, f32 t) {
    if (t < 0.4f) return;
    for (i32 i = 0; i < 34; ++i) {
        const f32 q = fraz((t - 0.4f) * (0.18f + 0.12f * caso((u32)i, 21)) + caso((u32)i, 22));
        const i32 x = 8 + (i32)(caso((u32)i, 23) * 304.0f) + (i32)(std::sin(t * 3.0f + (f32)i) * 6.0f);
        const i32 y = 20 + (i32)(q * 200.0f);
        const u32 col = p.c[i % 4];
        if (((i32)(t * 10.0f) + i) & 1) fb.fillRect(x, y, x + 1, y, col);
        else fb.fillRect(x, y, x, y + 1, col);
    }
}

// Scritta della posizione: lettere che cadono una alla volta e ondeggiano.
void scritta(Framebuffer& fb, Renderer& ren, TemaPremio tema, const ColoriPremio& p, i32 lv, i32 pos, f32 t) {
    if (t < 0.5f) return;
    static const char* const SUF[4] = {"TH", "ST", "ND", "RD"};
    char riga1[24];
    std::snprintf(riga1, sizeof(riga1), "%d%s PLACE", pos, SUF[(pos >= 1 && pos <= 3) ? pos : 0]);
    static const char* const MOTTO[6] = {
        "NIGHT CHAMPION", "ATOMIC HERO", "FLOWER POWER", "CHROME KING", "KING OF THE STREET", "WINNER.EXE"};
    const char* riga2 = lv == 2 ? MOTTO[(i32)tema] : (lv == 1 ? "PODIUM!" : "RACE COMPLETE");
    const i32 scala = 2;
    const i32 n = (i32)std::strlen(riga1);
    const i32 passo = 6 * scala;
    const i32 x0 = 160 - n * passo / 2, y0 = 76; // sotto il riquadro dell'HUD
    // nastro dietro la scritta (primo e podio): si apre dal centro
    if (lv >= 1) {
        const f32 k = std::min(1.0f, (t - 0.5f) / 0.3f);
        const i32 mezza = (i32)(k * (f32)(n * passo / 2 + 18));
        for (i32 y = y0 - 5; y <= y0 + 19; ++y)
            for (i32 x = 160 - mezza; x <= 160 + mezza; ++x)
                if (y == y0 - 5 || y == y0 + 19) fb.put(x, y, p.c[lv == 2 ? 2 : 0]);
                else if (trama(x, y, 0.35f)) fb.put(x, y, p.c[4]);
    }
    for (i32 i = 0; i < n; ++i) {
        const f32 ti = 0.5f + 0.05f * (f32)i;
        if (t < ti) continue;
        const f32 k = std::min(1.0f, (t - ti) / 0.22f);
        i32 dy = -(i32)((1.0f - k) * (1.0f - k) * 36.0f);
        if (k >= 1.0f && lv >= 1) dy = (i32)(std::sin(t * 5.0f + (f32)i * 0.6f) * 2.0f);
        const u32 col = lv == 2 ? p.c[(i + (i32)(t * 6.0f)) % 4] : (lv == 1 ? p.c[0] : p.c[3]);
        const char c[2] = {riga1[i], 0};
        ren.drawTesto(x0 + i * passo, y0 + dy, c, col, scala, true);
        // spray: la vernice cola sotto le lettere
        if (tema == TemaPremio::Spray && k >= 1.0f && riga1[i] != ' ') {
            const i32 lungo = std::min(3 + (i32)(caso((u32)i, 31) * 9.0f), (i32)((t - ti) * 10.0f));
            const i32 gx = x0 + i * passo + 2 + (i32)(caso((u32)i, 32) * 7.0f);
            fb.fillRect(gx, y0 + 15, gx, y0 + 15 + lungo, col);
            fb.fillRect(gx - 1, y0 + 15 + lungo, gx + 1, y0 + 16 + lungo, col);
        }
    }
    const f32 t2 = 0.5f + 0.05f * (f32)n + 0.15f;
    if (t >= t2) {
        const u32 col = ((i32)(t * 4.0f) & 1) && lv == 2 ? p.c[3] : p.c[1];
        ren.drawTesto(160 - Renderer::larghezzaTesto(riga2, 1) / 2, y0 + 24, riga2, col, 1, true);
    }
}

} // namespace

TemaPremio temaPremioDaLivello(const std::string& id) {
    static const char* const NOMI[6] = {"neon", "atomico", "foglie", "cromo", "spray", "desktop"};
    for (i32 i = 0; i < 6; ++i)
        if (id.find(NOMI[i]) != std::string::npos) return (TemaPremio)i;
    return TemaPremio::Neon;
}

void disegnaPremiazioneDietro(Framebuffer& fb, const DatiPremiazione& d) {
    const f32 t = d.tempoS;
    if (t < 0.0f) return;
    const Auto a = misure(d);
    const ColoriPremio& p = COLORI[(i32)d.tema];
    const i32 lv = livello(d);
    // alone dietro l'auto che pulsa
    const f32 accende = std::min(1.0f, t / 0.3f);
    const f32 forza = accende * (0.3f + 0.1f * (f32)lv) * (0.85f + 0.15f * std::sin(t * 6.0f));
    alone(fb, a.cx, a.terra - a.h / 2, a.w * (65 + 15 * lv) / 100, a.h * (9 + 2 * lv) / 10, forza, p.c[0]);
    // onda d'urto sulla strada appena l'auto si ferma
    if (t < 0.7f) {
        const f32 k = t / 0.7f;
        const f32 rx = (f32)a.w * (0.6f + 1.8f * k);
        anello(fb, (f32)a.cx, (f32)a.terra - 2.0f, rx, rx * 0.22f, false, p.c[1], p.c[3], 1.0f - k);
        anello(fb, (f32)a.cx, (f32)a.terra - 2.0f, rx, rx * 0.22f, true, p.c[1], p.c[3], 1.0f - k);
    }
    switch (d.tema) {
        case TemaPremio::Neon:    laserNeon(fb, a, p, lv, t); break;
        case TemaPremio::Atomico: orbiteAtomico(fb, a, p, lv, t, false); break;
        case TemaPremio::Foglie:  cerchiFoglie(fb, a, p, lv, t); vorticeFoglie(fb, a, p, lv, t, false); break;
        case TemaPremio::Cromo:   anelliCromo(fb, a, p, lv, t, false); break;
        case TemaPremio::Spray:   break;
        case TemaPremio::Desktop: break;
    }
}

void disegnaPremiazioneDavanti(Framebuffer& fb, Renderer& ren, const DatiPremiazione& d) {
    const f32 t = d.tempoS;
    if (t < 0.0f) return;
    const Auto a = misure(d);
    const ColoriPremio& p = COLORI[(i32)d.tema];
    const i32 lv = livello(d);
    switch (d.tema) {
        case TemaPremio::Neon:    scintilleNeon(fb, p, lv, t); break;
        case TemaPremio::Atomico: orbiteAtomico(fb, a, p, lv, t, true); if (lv == 2) razzoAtomico(fb, a, p, t); break;
        case TemaPremio::Foglie:  vorticeFoglie(fb, a, p, lv, t, true); break;
        case TemaPremio::Cromo:   anelliCromo(fb, a, p, lv, t, true); stelleCromo(fb, a, p, lv, t); break;
        case TemaPremio::Spray:   schizziSpray(fb, a, p, lv, t); break;
        case TemaPremio::Desktop: if (!d.soloAmbiente) finestreDesktop(fb, ren, a, p, lv, d.posizione, t); break;
    }
    if (lv == 2 && d.tema != TemaPremio::Neon) coriandoli(fb, p, t);
    if (d.soloAmbiente) return;
    // icona sopra l'auto: trofeo, medaglia d'argento/bronzo o bandiera a scacchi
    if (t >= 0.45f) {
        const f32 k = t - 0.45f;
        const i32 s = k < 0.1f ? 3 : 2;                       // scatto quando compare
        const i32 bob = (i32)(std::sin(t * 3.2f) * 2.0f);
        const i32 x0 = a.cx - 11 * s / 2, y0 = a.alto - 12 * s - 6 + bob;
        if (lv == 2) {
            alone(fb, a.cx, y0 + 5 * s, 18, 14, 0.5f + 0.2f * std::sin(t * 8.0f), p.c[2]);
            icona(fb, TROFEO, x0, y0, s, 0xFFFFD040u, 0xFFC08820u, 0xFFFFF4B0u, p.c[0]);
        } else if (lv == 1) {
            const bool argento = d.posizione == 2;
            const u32 ombra = argento ? 0xFF8890A8u : 0xFF985828u;
            alone(fb, a.cx, y0 + 7 * s, 15, 12, 0.35f + 0.15f * std::sin(t * 6.0f), p.c[1]);
            icona(fb, MEDAGLIA, x0, y0, s, argento ? 0xFFD8E0F0u : 0xFFE09050u, ombra, 0xFFFFFFFFu, p.c[0]);
            const char cifra[2] = {(char)('0' + d.posizione), 0};
            ren.drawTesto(a.cx - 3 * s / 2 - 1, y0 + 5 * s + 1, cifra, ombra, s / 2, false);
        } else {
            icona(fb, BANDIERA, x0 + s * 2, y0 + (((i32)(t * 6.0f)) & 1), s, 0, 0xFFB0B0B0u, 0, 0);
        }
    }
    scritta(fb, ren, d.tema, p, lv, d.posizione, t);
}

} // namespace sgl
