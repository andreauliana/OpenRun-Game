// SPDX-License-Identifier: MIT
#include "game/paesaggio.h"
#include "core/rng.h"
#include <algorithm>
#include <cmath>

namespace sgl {

namespace {
// Una fila di oggetti ogni PASSO unita' di strada (2 segmenti).
constexpr f32 PASSO = 400.0f;
// Altezza a schermo di ogni tipo di oggetto (unita' mondo, come le auto: la
// strada e' larga 2400). La scala si ricava dall'altezza del PNG, quindi un
// disegno nuovo di Andrea con la stessa misura resta della stessa taglia.
constexpr f32 H_CIUFFO = 300.0f;
constexpr f32 H_CESPUGLIO = 600.0f;
constexpr f32 H_ALBERO = 2900.0f;
constexpr f32 H_FRECCIA = 760.0f;
constexpr f32 PX_EDIFICIO = 27.0f;          // un pixel di palazzo = 27 unita' (alto 64x128 = 3456)
constexpr f32 H_ARREDO_ALTO = 1280.0f;      // come prima (OGGETTO_ALTEZZA_MONDO)
// Distanza minima dal bordo della carreggiata al bordo dello sprite: il gate
// decorazioni chiede almeno 200 (mai sul tracciato).
constexpr f32 MARGINE_MIN = 260.0f;
// Ai capi di una zona citta' i palazzi si diradano e si allontanano in
// RAMPA_CITTA segmenti: si entra in citta' passando dalla periferia.
constexpr f32 RAMPA_CITTA = 45.0f;

f32 scalaPer(const Sprite* s, f32 altezzaMondo) {
    return altezzaMondo / ((f32)s->height * Renderer::SPRITE_WORLD_PX);
}
f32 scalaArredo(const Sprite* s) {
    return std::clamp(H_ARREDO_ALTO / ((f32)s->height * Renderer::SPRITE_WORLD_PX), 1.0f, 4.0f);
}
} // namespace

Zona zonaDaNome(const std::string& nome, Zona riserva) {
    if (nome == "aperto") return Zona::Aperto;
    if (nome == "periferia") return Zona::Periferia;
    if (nome == "citta") return Zona::Citta;
    if (nome == "autostrada") return Zona::Autostrada;
    if (nome == "nessuna") return Zona::Nessuna;
    return riserva;
}

const char* nomeZona(Zona z) {
    switch (z) {
    case Zona::Aperto: return "aperto";
    case Zona::Periferia: return "periferia";
    case Zona::Citta: return "citta";
    case Zona::Autostrada: return "autostrada";
    default: return "nessuna";
    }
}

Zona zonaPredefinitaTratto(const std::string& tratto, Zona base) {
    if (tratto == "autostrada") return Zona::Autostrada;
    if (tratto == "citta") return Zona::Citta;
    if (tratto == "ponte" || tratto == "rampa_ponte" || tratto == "discesa_ponte") return Zona::Nessuna;
    return base;
}

void disponiPaesaggio(const Road& road, const std::vector<u8>& zonaSeg, const PezziPaesaggio& pz,
                      f32 zBivioInizio, u32 seme, std::vector<SpriteInstance>& out) {
    const f32 giro = (f32)road.totalLength();
    const i32 nSeg = (i32)zonaSeg.size();
    if (giro <= 0.0f || nSeg == 0) return;

    // Posizione di ogni segmento dentro la sua zona (segmenti dal capo piu'
    // vicino): serve alle rampe di ingresso/uscita della citta'.
    std::vector<i32> dalCapo((size_t)nSeg, 0);
    for (i32 i = 0; i < nSeg;) {
        i32 j = i;
        while (j < nSeg && zonaSeg[(size_t)j] == zonaSeg[(size_t)i]) ++j;
        for (i32 k = i; k < j; ++k) dalCapo[(size_t)k] = std::min(k - i, j - 1 - k);
        i = j;
    }

    const f32 bordo = Road::ROAD_WIDTH * 0.5f;
    const f32 bordoBivio = Road::BIVIO_DIST + bordo;
    Rng rnd(seme ^ 0x5041455Au);
    auto caso = [&rnd]() { return (f32)rnd.next(10000) / 10000.0f; };
    auto tra = [&caso](f32 a, f32 b) { return a + (b - a) * caso(); };
    auto scegli = [&rnd](const std::vector<const Sprite*>& v) -> const Sprite* {
        return v.empty() ? nullptr : v[rnd.next((u32)v.size())];
    };

    f32 bordoQui = bordo;
    auto metti = [&](const Sprite* s, f32 lato, f32 z, f32 distanza, f32 scala) {
        if (!s || scala <= 0.0f) return;
        if (z >= giro || road.sospesoAt(z) || road.acquaAt(z)) return; // ponte e rampe: niente nel vuoto
        const f32 meta = 0.5f * (f32)s->width * Renderer::SPRITE_WORLD_PX * scala;
        SpriteInstance o;
        o.worldZ = z;
        o.worldX = lato * (bordoQui + std::max(MARGINE_MIN, distanza) + meta);
        o.scale = scala;
        o.sprite = s;
        o.paletteBase = 0;
        out.push_back(o);
    };
    // Arredo: lampioni e oggetti del tema con la stessa scala di prima.
    auto mettiArredo = [&](const std::vector<const Sprite*>& v, f32 lato, f32 z, f32 distanza) {
        const Sprite* s = scegli(v);
        if (s) metti(s, lato, z, distanza, scalaArredo(s));
    };

    std::vector<const Sprite*> tuttoArredo = pz.arredo;
    tuttoArredo.insert(tuttoArredo.end(), pz.lampioni.begin(), pz.lampioni.end());

    i32 k = 0;
    for (f32 z = PASSO; z < giro - PASSO; z += PASSO, ++k) {
        const i32 seg = std::clamp(road.indiceSegmento(z), 0, nSeg - 1);
        const Zona zona = (Zona)zonaSeg[(size_t)seg];
        bordoQui = z >= zBivioInizio ? bordoBivio : bordo;
        if (zona == Zona::Nessuna) continue;
        const f32 rampa = std::clamp((f32)dalCapo[(size_t)seg] / RAMPA_CITTA, 0.0f, 1.0f);
        // Boschetti: gli alberi vengono a macchie lungo la strada, non uniformi.
        const f32 bosco = 0.5f + 0.5f * std::sin(z / 23000.0f + (f32)(seme % 97u));

        for (i32 l = 0; l < 2; ++l) {
            const f32 lato = l == 0 ? -1.0f : 1.0f;
            const f32 zz = z + tra(0.0f, PASSO * 0.6f); // file non allineate
            switch (zona) {
            case Zona::Aperto:
                if (caso() < 0.55f) { const Sprite* s = scegli(pz.ciuffi); if (s) metti(s, lato, zz, tra(260.0f, 1300.0f), scalaPer(s, H_CIUFFO * tra(0.8f, 1.2f))); }
                if (caso() < 0.14f) { const Sprite* s = scegli(pz.cespugli); if (s) metti(s, lato, zz, tra(500.0f, 2800.0f), scalaPer(s, H_CESPUGLIO * tra(0.8f, 1.3f))); }
                if (caso() < 0.11f * pz.densitaAlberi * (0.2f + bosco)) { const Sprite* s = scegli(pz.alberi); if (s) metti(s, lato, zz, tra(900.0f, 6000.0f), scalaPer(s, H_ALBERO * tra(0.85f, 1.15f))); }
                if (caso() < 0.02f) mettiArredo(pz.arredo, lato, zz, tra(260.0f, 900.0f));
                break;
            case Zona::Periferia:
                if (caso() < 0.30f) { const Sprite* s = scegli(pz.ciuffi); if (s) metti(s, lato, zz, tra(260.0f, 1000.0f), scalaPer(s, H_CIUFFO * tra(0.8f, 1.2f))); }
                // Arredo del tema come prima: una fila ogni 1600, lato alternato.
                if (k % 4 == 0 && ((k / 4 + l) & 1)) mettiArredo(tuttoArredo, lato, zz, tra(260.0f, 900.0f));
                if (caso() < 0.06f) { const Sprite* s = scegli(pz.edifici); if (s) metti(s, lato, zz, tra(1800.0f, 5500.0f), PX_EDIFICIO / Renderer::SPRITE_WORLD_PX); }
                if (caso() < 0.06f * pz.densitaAlberi) { const Sprite* s = scegli(pz.alberi); if (s) metti(s, lato, zz, tra(900.0f, 5000.0f), scalaPer(s, H_ALBERO * tra(0.85f, 1.15f))); }
                if (caso() < 0.08f) { const Sprite* s = scegli(pz.cespugli); if (s) metti(s, lato, zz, tra(400.0f, 2000.0f), scalaPer(s, H_CESPUGLIO * tra(0.8f, 1.3f))); }
                break;
            case Zona::Citta:
                // Palazzi in fila, vicini alla strada: una "gola" di facciate.
                // Ai capi della zona si diradano e si allontanano (periferia).
                if ((k & 1) == 0 && caso() < 0.35f + 0.6f * rampa && !pz.edifici.empty()) {
                    const size_t n = pz.edifici.size();
                    // Al centro della citta' i piu' alti, ai capi i piu' bassi.
                    size_t idx = rnd.next((u32)n);
                    if (rampa < 0.5f && idx + 1 < n && caso() < 0.6f) idx = n - 1;
                    metti(pz.edifici[idx], lato, z + tra(0.0f, PASSO * 0.3f),
                          520.0f + (1.0f - rampa) * 1800.0f + tra(0.0f, 300.0f), PX_EDIFICIO / Renderer::SPRITE_WORLD_PX);
                }
                if (k % 4 == 0) mettiArredo(pz.lampioni.empty() ? pz.arredo : pz.lampioni, lato, z, 260.0f);
                else if (k % 8 == 2 && caso() < 0.5f) mettiArredo(pz.arredo, lato, zz, tra(260.0f, 380.0f));
                break;
            case Zona::Autostrada:
                // Lampioni a coppie a ritmo regolare: il senso di velocita'.
                if (k % 5 == 0) mettiArredo(pz.lampioni.empty() ? pz.arredo : pz.lampioni, lato, z, 300.0f);
                if (caso() < 0.18f) { const Sprite* s = scegli(pz.ciuffi); if (s) metti(s, lato, zz, tra(600.0f, 1600.0f), scalaPer(s, H_CIUFFO * tra(0.8f, 1.2f))); }
                if (caso() < 0.05f) { const Sprite* s = scegli(pz.cespugli); if (s) metti(s, lato, zz, tra(1200.0f, 3000.0f), scalaPer(s, H_CESPUGLIO)); }
                if (caso() < 0.03f * pz.densitaAlberi) { const Sprite* s = scegli(pz.alberi); if (s) metti(s, lato, zz, tra(2500.0f, 7000.0f), scalaPer(s, H_ALBERO * tra(0.85f, 1.15f))); }
                break;
            default:
                break;
            }
        }
        // Curve strette: cartelli a freccia sul lato esterno, uno ogni 5 file (2000).
        const f32 curva = road.curvaAt(z);
        if (std::abs(curva) >= 0.6f && k % 5 == 0) {
            const Sprite* s = curva > 0.0f ? pz.frecciaDx : pz.frecciaSx;
            if (s) metti(s, curva > 0.0f ? -1.0f : 1.0f, z, MARGINE_MIN, scalaPer(s, H_FRECCIA));
        }
    }
}

} // namespace sgl
