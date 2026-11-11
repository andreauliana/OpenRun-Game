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
constexpr i32 SCOPERTA_PASSO = 12;     // segmenti fra un gruppo e l'altro sullo sfondo del ponte
constexpr f32 H_FRECCIA = 760.0f;
constexpr f32 PX_EDIFICIO = 27.0f;          // un pixel di palazzo = 27 unita' (alto 64x128 = 3456)
constexpr f32 H_ARREDO_ALTO = 1280.0f;      // come prima (OGGETTO_ALTEZZA_MONDO)
// Distanza minima dal bordo della carreggiata al bordo dello sprite: il gate
// decorazioni chiede almeno 200 (mai sul tracciato).
constexpr f32 MARGINE_MIN = 260.0f;
// Ai capi di una zona citta' i palazzi si diradano e si allontanano in
// RAMPA_CITTA segmenti: si entra in citta' passando dalla periferia.
// Andrea 08/11: 45 segmenti erano un muro di palazzi "da un momento
// all'altro"; ora ~5 s a piena velocita', prima bassi e radi poi la gola.
constexpr f32 RAMPA_CITTA = 170.0f;
// Inizio di ogni livello (cambio tema): palazzi radi e bassi nei primi
// RAMPA_LIVELLO segmenti anche fuori citta', il tema entra con gradualita'.
constexpr f32 RAMPA_LIVELLO = 260.0f;

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
        // PERC1 D4: dentro la galleria niente oggetti/cartelloni/alberi (ne' frecce).
        if (z >= giro || road.sospesoAt(z) || road.acquaAt(z) || road.galleriaAt(z)) return;
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
        // Rampa morbida (smoothstep): ai capi cresce piano, poi accelera.
        const f32 rampaLiv = std::clamp((f32)seg / RAMPA_LIVELLO, 0.0f, 1.0f);
        f32 rampa = std::min(std::clamp((f32)dalCapo[(size_t)seg] / RAMPA_CITTA, 0.0f, 1.0f), rampaLiv);
        rampa = rampa * rampa * (3.0f - 2.0f * rampa);
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
                if (caso() < 0.06f * rampaLiv) { const Sprite* s = scegli(pz.edifici); if (s) metti(s, lato, zz, tra(1800.0f, 5500.0f), PX_EDIFICIO / Renderer::SPRITE_WORLD_PX); }
                if (caso() < 0.06f * pz.densitaAlberi) { const Sprite* s = scegli(pz.alberi); if (s) metti(s, lato, zz, tra(900.0f, 5000.0f), scalaPer(s, H_ALBERO * tra(0.85f, 1.15f))); }
                if (caso() < 0.08f) { const Sprite* s = scegli(pz.cespugli); if (s) metti(s, lato, zz, tra(400.0f, 2000.0f), scalaPer(s, H_CESPUGLIO * tra(0.8f, 1.3f))); }
                break;
            case Zona::Citta:
                // Palazzi in fila, vicini alla strada: una "gola" di facciate.
                // Ai capi della zona si diradano e si allontanano (periferia).
                if ((k & 1) == 0 && caso() < 0.12f + 0.83f * rampa && !pz.edifici.empty()) {
                    const size_t n = pz.edifici.size();
                    // Al centro della citta' i piu' alti, ai capi i piu' bassi:
                    // con rampa bassa si pesca quasi sempre fra i bassi.
                    size_t idx = rnd.next((u32)n);
                    if (n > 1 && caso() > rampa) idx = n / 2 + rnd.next((u32)(n - n / 2));
                    if (rampa < 0.3f && caso() < 0.7f) idx = n - 1;
                    metti(pz.edifici[idx], lato, z + tra(0.0f, PASSO * 0.3f),
                          520.0f + (1.0f - rampa) * 2600.0f + tra(0.0f, 300.0f), PX_EDIFICIO / Renderer::SPRITE_WORLD_PX);
                } else if ((k & 1) == 0 && rampa < 0.8f && caso() < 0.35f * (1.0f - rampa)) {
                    // Al posto dei palazzi mancanti: verde di periferia.
                    const bool albero = caso() < 0.5f * pz.densitaAlberi;
                    const Sprite* s = albero ? scegli(pz.alberi) : scegli(pz.cespugli);
                    if (s) metti(s, lato, zz, tra(500.0f, 2400.0f),
                                 scalaPer(s, (albero ? H_ALBERO : H_CESPUGLIO) * tra(0.85f, 1.15f)));
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
        // Ai due capi del ponte e prima del bivio (Andrea 07/11: "all'uscita
        // del ponte i lati sono proprio spogli"): un boschetto fitto lungo le
        // luci di avvicinamento, cosi' si entra e si esce da una "porta" verde.
        const RoadSegment& sg = road.segments()[(size_t)seg];
        if (sg.luci >= 0 && sg.luci < 60 && (zona == Zona::Aperto || zona == Zona::Periferia || zona == Zona::Autostrada)) {
            for (i32 l = 0; l < 2; ++l) {
                const f32 lato = l == 0 ? -1.0f : 1.0f;
                const f32 zz = z + tra(0.0f, PASSO * 0.6f);
                // Un po' radi (Andrea 07/11 sera), con gli oggetti del tema del livello.
                if (caso() < 0.3f) { const Sprite* s = scegli(pz.ciuffi); if (s) metti(s, lato, zz, tra(500.0f, 1400.0f), scalaPer(s, H_CIUFFO * tra(0.8f, 1.2f))); }
                if (caso() < 0.18f) { const Sprite* s = scegli(pz.cespugli); if (s) metti(s, lato, zz, tra(700.0f, 2200.0f), scalaPer(s, H_CESPUGLIO * tra(0.9f, 1.3f))); }
                if (caso() < 0.14f) { const Sprite* s = scegli(pz.alberi); if (s) metti(s, lato, zz, tra(1200.0f, 4200.0f), scalaPer(s, H_ALBERO * tra(0.85f, 1.15f))); }
                if (caso() < 0.1f) mettiArredo(tuttoArredo, lato, zz, tra(300.0f, 1200.0f));
            }
        }
        // Curve strette: cartelli a freccia sul lato esterno, uno ogni 5 file (2000).
        // PERC1 D3: nelle curve cieche (dune > 0) usa la freccia con segno del dosso se presente.
        const f32 curva = road.curvaAt(z);
        if (std::abs(curva) >= 0.6f && k % 5 == 0) {
            const bool cieca = road.segments()[(size_t)seg].dune;
            const Sprite* sNorm = curva > 0.0f ? pz.frecciaDx : pz.frecciaSx;
            const Sprite* sCresta = curva > 0.0f ? pz.frecciaCrestaDx : pz.frecciaCrestaSx;
            const Sprite* s = (cieca && sCresta) ? sCresta : sNorm;
            if (s) metti(s, curva > 0.0f ? -1.0f : 1.0f, z, MARGINE_MIN, scalaPer(s, H_FRECCIA));
        }
    }
}

void disponiTerraPonte(const Road& road, const PezziPaesaggio& pz, const PezziPonte& pp, u32 seme,
                       std::vector<SpriteInstance>& out) {
    const auto& segs = road.segments();
    const i32 n = (i32)segs.size();
    if (n == 0) return;
    // Andrea 07/11 sera: barche piu' grandi; capi del ponte un po' radi, con
    // gli oggetti del tema; elementi inattesi e in movimento (anima, Renderer).
    constexpr f32 H_BOA = 460.0f, H_BARCA = 2600.0f, H_MOTOSCAFO = 1100.0f, H_SCOGLIO = 560.0f,
                  H_FARO = 5600.0f, H_OMBRELLONE = 760.0f;
    const f32 bordo = Road::ROAD_WIDTH * 0.5f;
    Rng rnd(seme ^ 0x504F4E54u);
    auto caso = [&rnd]() { return (f32)rnd.next(10000) / 10000.0f; };
    auto tra = [&caso](f32 a, f32 b) { return a + (b - a) * caso(); };
    auto scegli = [&rnd](const std::vector<const Sprite*>& v) -> const Sprite* {
        return v.empty() ? nullptr : v[rnd.next((u32)v.size())];
    };
    auto metti = [&](const Sprite* s, f32 lato, f32 z, f32 distanza, f32 altezza, u8 anima = 0) {
        if (!s && anima < 5) return;
        const f32 scala = s ? scalaPer(s, altezza) : altezza; // senza sprite: scale = quota
        const f32 meta = s ? 0.5f * (f32)s->width * Renderer::SPRITE_WORLD_PX * scala : 0.0f;
        SpriteInstance o;
        o.worldZ = z;
        o.worldX = lato * (bordo + std::max(500.0f, distanza) + meta); // mai sotto l'impalcato
        o.scale = scala;
        o.sprite = s;
        o.anima = anima;
        o.fase = tra(0.0f, 6.28f);
        out.push_back(o);
    };
    // Oggetti del tema del livello (lampioni, insegne...) con la loro scala solita.
    std::vector<const Sprite*> tema = pz.arredo;
    tema.insert(tema.end(), pz.lampioni.begin(), pz.lampioni.end());
    auto mettiTema = [&](f32 lato, f32 z, f32 distanza) {
        const Sprite* s = scegli(tema);
        if (s) metti(s, lato, z, distanza, (f32)s->height * Renderer::SPRITE_WORLD_PX * scalaArredo(s));
    };
    i32 ponti = 0;
    for (i32 i = 0; i < n; i += 2) {
        const RoadSegment& sg = segs[(size_t)i];
        if (!sg.sospeso) continue;
        const f32 z = (f32)i * (f32)Road::SEG_LEN;
        // Un faro per ponte, su uno scoglio, dove la strada lascia la costa.
        if (sg.acqua && !segs[(size_t)((i + n - 2) % n)].acqua) {
            const f32 lato = (ponti++ & 1) ? -1.0f : 1.0f;
            metti(pp.faro, lato, z + 600.0f, 5200.0f, H_FARO, 4);
            metti(pp.scoglio, lato, z + 300.0f, 4700.0f, H_SCOGLIO * 1.6f);
        }
        const i32 riva = sg.acqua ? 0 : sg.riva;
        const bool mare = riva >= 0 && riva < Road::RIVA_ACQUA - 2;
        const bool battigia = riva >= Road::RIVA_ACQUA - 2 && riva <= Road::RIVA_ACQUA + 3;
        const bool spiaggia = riva > Road::RIVA_ACQUA + 3 && riva < Road::RIVA_ACQUA + Road::RIVA_SABBIA + Road::RIVA_SFUMA / 2;
        // Lontano dai capi del ponte: il motoscafo naviga avanti e indietro di 5000.
        bool largo = sg.acqua;
        for (i32 d = 1; largo && d <= 40; ++d)
            largo = segs[(size_t)((i + d) % n)].acqua && segs[(size_t)((i - d + n) % n)].acqua;
        for (i32 l = 0; l < 2; ++l) {
            const f32 lato = l == 0 ? -1.0f : 1.0f;
            const f32 zz = z + tra(0.0f, 300.0f);
            if (mare) {
                // Boe a file regolari (segnano il canale), barche al largo, qualche scoglio.
                if ((i % 16) == 0) metti(pp.boa, lato, zz, 1400.0f + tra(0.0f, 400.0f), H_BOA, 3);
                if (caso() < 0.03f) metti(pp.barca, lato, zz, tra(3000.0f, 14000.0f), H_BARCA * tra(0.85f, 1.15f), 1);
                if (largo && caso() < 0.012f) metti(pp.motoscafo, lato, zz, tra(2500.0f, 6000.0f), H_MOTOSCAFO, 2);
                if (caso() < 0.025f) metti(pp.scoglio, lato, zz, tra(2500.0f, 9000.0f), H_SCOGLIO * tra(0.8f, 1.4f));
                if (caso() < 0.03f) metti(nullptr, lato, zz, tra(1500.0f, 7000.0f), 0.0f, 6);           // pesce
                if (caso() < 0.05f) metti(nullptr, lato, zz, tra(800.0f, 5000.0f), tra(1500.0f, 4500.0f), 5); // gabbiano
            } else if (battigia) {
                if (caso() < 0.3f) metti(pp.scoglio, lato, zz, tra(600.0f, 9000.0f), H_SCOGLIO * tra(0.7f, 1.5f));
                if (caso() < 0.06f) metti(nullptr, lato, zz, tra(500.0f, 4000.0f), tra(900.0f, 2500.0f), 5);
            } else if (spiaggia) {
                if (caso() < 0.18f) metti(pp.ombrellone, lato, zz, tra(700.0f, 6000.0f), H_OMBRELLONE * tra(0.9f, 1.1f));
                if (caso() < 0.08f) metti(pp.scoglio, lato, zz, tra(900.0f, 7000.0f), H_SCOGLIO * tra(0.6f, 1.1f));
                if (caso() < 0.05f) mettiTema(lato, zz, tra(600.0f, 3000.0f));
            } else {
                // Prato basso sotto le rampe: la campagna e gli oggetti del tema, radi.
                if (caso() < 0.35f) { const Sprite* s = scegli(pz.ciuffi); metti(s, lato, zz, tra(500.0f, 2500.0f), H_CIUFFO * tra(0.8f, 1.2f)); }
                if (caso() < 0.16f) { const Sprite* s = scegli(pz.cespugli); metti(s, lato, zz, tra(600.0f, 4000.0f), H_CESPUGLIO * tra(0.9f, 1.3f)); }
                if (caso() < 0.12f * pz.densitaAlberi) { const Sprite* s = scegli(pz.alberi); metti(s, lato, zz, tra(800.0f, 7000.0f), H_ALBERO * tra(0.85f, 1.15f)); }
                if (caso() < 0.06f) mettiTema(lato, zz, tra(600.0f, 2500.0f));
            }
        }
    }

    // Sfondo da scoprire (Andrea 11/11): dal ponte alto si vede tutta la costa
    // sotto la discesa. Non oggetti sparsi a caso ma piccoli gruppi composti,
    // anche molto lontani di lato: si leggono da lassu' come paesaggio e poi,
    // scendendo, scivolano ai lati uno alla volta. Generatore a parte: gli
    // oggetti di prima restano dove erano.
    Rng rndS(seme ^ 0x53434F50u);
    auto casoS = [&rndS]() { return (f32)rndS.next(10000) / 10000.0f; };
    auto traS = [&casoS](f32 a, f32 b) { return a + (b - a) * casoS(); };
    auto scegliS = [&rndS](const std::vector<const Sprite*>& v) -> const Sprite* {
        return v.empty() ? nullptr : v[rndS.next((u32)v.size())];
    };
    auto mettiS = [&](const Sprite* s, f32 lato, f32 z, f32 distanza, f32 altezza, u8 anima = 0) {
        if (!s) return;
        const f32 scala = scalaPer(s, altezza);
        SpriteInstance o;
        o.worldZ = z;
        o.worldX = lato * (bordo + distanza + 0.5f * (f32)s->width * Renderer::SPRITE_WORLD_PX * scala);
        o.scale = scala;
        o.sprite = s;
        o.anima = anima;
        o.fase = traS(0.0f, 6.28f);
        out.push_back(o);
    };
    for (i32 i = 0; i < n; i += SCOPERTA_PASSO) {
        const RoadSegment& sg = segs[(size_t)i];
        if (!sg.sospeso || sg.acqua) continue;
        const f32 z = (f32)i * (f32)Road::SEG_LEN;
        const i32 riva = sg.riva;
        const bool mare = riva >= 0 && riva < Road::RIVA_ACQUA - 2;
        const bool spiaggia = riva >= Road::RIVA_ACQUA + 2 && riva < Road::RIVA_ACQUA + Road::RIVA_SABBIA;
        const bool prato = riva < 0 || riva >= Road::RIVA_ACQUA + Road::RIVA_SABBIA + Road::RIVA_SFUMA;
        // Un lato per volta, alternato con un po' di caso: la vista resta leggibile.
        const f32 lato = ((i / SCOPERTA_PASSO) & 1) ? 1.0f : -1.0f;
        const f32 lontano = traS(3500.0f, 22000.0f);
        if (mare) {
            // Barche ormeggiate lungo la costa, lontane: si vedono dal ponte.
            if (casoS() < 0.5f) mettiS(pp.barca, lato, z, lontano, H_BARCA * traS(0.8f, 1.1f), 1);
        } else if (spiaggia) {
            // Stabilimento: una fila di ombrelloni parallela alla battigia.
            const i32 fila = 3 + (i32)rndS.next(4);
            const f32 d0 = traS(1500.0f, 12000.0f);
            for (i32 k = 0; k < fila; ++k)
                mettiS(pp.ombrellone, lato, z + traS(-150.0f, 150.0f), d0 + (f32)k * 900.0f, H_OMBRELLONE);
            if (casoS() < 0.4f) mettiS(pp.scoglio, -lato, z, traS(4000.0f, 16000.0f), H_SCOGLIO * traS(1.0f, 1.8f));
        } else if (prato) {
            if (casoS() < 0.65f) {
                // Boschetto: alberi stretti attorno a un centro, piu' alti in mezzo.
                const i32 alberi = 4 + (i32)rndS.next(5);
                for (i32 k = 0; k < alberi; ++k) {
                    const f32 r = traS(0.0f, 1.0f);
                    mettiS(scegliS(pz.alberi), lato, z + traS(-900.0f, 900.0f) * r, lontano + traS(-1600.0f, 1600.0f) * r,
                           H_ALBERO * (1.2f - 0.4f * r) * traS(0.9f, 1.1f));
                }
                for (i32 k = 0; k < 3; ++k)
                    mettiS(scegliS(pz.cespugli), lato, z + traS(-1100.0f, 1100.0f), lontano + traS(-2000.0f, 2000.0f),
                           H_CESPUGLIO * traS(0.9f, 1.4f));
            } else {
                // Un oggetto del tema da solo nel prato, come un punto di riferimento.
                const Sprite* s = scegliS(tema);
                if (s) mettiS(s, lato, z, traS(2500.0f, 9000.0f), (f32)s->height * Renderer::SPRITE_WORLD_PX * scalaArredo(s));
            }
        }
    }
}

} // namespace sgl
