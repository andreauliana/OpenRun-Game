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
    f32 densitaAlberi = 1.0f;             // livello.json paesaggio.densita_alberi
};

// Dispone gli oggetti lungo tutto il giro. zonaSeg: zona di ogni segmento
// della Road. Mai sul tracciato, mai sul ponte e sulle rampe; nella zona del
// bivio (z >= zBivioInizio) il margine parte dal bordo esterno dei due rami.
void disponiPaesaggio(const Road& road, const std::vector<u8>& zonaSeg, const PezziPaesaggio& pezzi,
                      f32 zBivioInizio, u32 seme, std::vector<SpriteInstance>& out);

} // namespace sgl
