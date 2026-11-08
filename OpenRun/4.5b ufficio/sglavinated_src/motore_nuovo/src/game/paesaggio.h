// SPDX-License-Identifier: MIT
#pragma once
// Paesaggio a zone (Andrea 06/11): gli oggetti ai lati della strada non sono
// piu' sparsi a caso ma seguono la zona del tratto, come nei giochi di corsa
// arcade anni 80: campagna aperta (erba, cespugli, alberi a macchie),
// periferia (arredo del tema, qualche casa lontana), citta' (file di palazzi
// vicino alla strada, lampioni), autostrada (lampioni a ritmo regolare, poca
// erba). In curva: cartelli a freccia sul lato esterno.
// Le zone vengono da livello.json, campo "paesaggio" (vedi docs/LIVELLI.md).
// Tutte le allocazioni avvengono al caricamento della tappa: zero alloc per frame.
#include "core/types.h"
#include "render/renderer.h"
#include "road/road.h"
#include <string>
#include <vector>

namespace sgl {

enum class Zona : u8 { Nessuna = 0, Aperto, Periferia, Citta, Autostrada };

// "aperto" | "periferia" | "citta" | "autostrada" | "nessuna"; altro -> riserva.
Zona zonaDaNome(const std::string& nome, Zona riserva);
const char* nomeZona(Zona z);
// Zona di un tratto senza indicazione nel livello: autostrada e citta' hanno
// la loro, ponte e rampe nessuna (il ponte resta aperto sull'acqua), gli altri
// la zona base del livello.
Zona zonaPredefinitaTratto(const std::string& tratto, Zona base);

// Sprite disponibili per il paesaggio della tappa (di proprieta' del Renderer).
struct PezziPaesaggio {
    std::vector<const Sprite*> arredo;    // oggetti del tema bassi (bidoni, panchine, insegne...)
    std::vector<const Sprite*> lampioni;  // oggetti del tema alti (>= 96 px): lampioni, pali
    std::vector<const Sprite*> ciuffi;    // erba
    std::vector<const Sprite*> cespugli;
    std::vector<const Sprite*> alberi;
    std::vector<const Sprite*> edifici;   // ordinati dal piu' alto al piu' basso
    const Sprite* frecciaDx = nullptr;    // cartello di curva a destra (sul lato sinistro)
    const Sprite* frecciaSx = nullptr;
    // PERC1 D3: frecce con segno del dosso per curve cieche (dune > 0). Se assenti, si usano le frecce normali.
    const Sprite* frecciaCrestaDx = nullptr;
    const Sprite* frecciaCrestaSx = nullptr;
    f32 densitaAlberi = 1.0f;             // livello.json paesaggio.densita_alberi
};

// Dispone gli oggetti lungo tutto il giro. zonaSeg: zona di ogni segmento
// della Road. Mai sul tracciato, mai sul ponte e sulle rampe; nella zona del
// bivio (z >= zBivioInizio) il margine parte dal bordo esterno dei due rami.
void disponiPaesaggio(const Road& road, const std::vector<u8>& zonaSeg, const PezziPaesaggio& pezzi,
                      f32 zBivioInizio, u32 seme, std::vector<SpriteInstance>& out);

// Pezzi comuni dei dintorni del ponte (Andrea 07/11), da
// dati/sprite_raw/scenari/comuni/ (TOOLS/disegna_ponte.py). nullptr = assente.
struct PezziPonte {
    const Sprite* boa = nullptr;
    const Sprite* barca = nullptr;
    const Sprite* motoscafo = nullptr;
    const Sprite* scoglio = nullptr;
    const Sprite* faro = nullptr;
    const Sprite* ombrellone = nullptr;
};

// Oggetti sul terreno basso sotto il ponte e le sue rampe: boe, barche e
// scogli sul mare, un faro per ponte, ombrelloni e scogli sulla spiaggia,
// alberi e cespugli del livello sul prato. Il Renderer li disegna dentro
// drawRoad alla quota del terreno (SpriteInstance in out, worldX dal centro).
void disponiTerraPonte(const Road& road, const PezziPaesaggio& pz, const PezziPonte& pp, u32 seme,
                       std::vector<SpriteInstance>& out);

} // namespace sgl
