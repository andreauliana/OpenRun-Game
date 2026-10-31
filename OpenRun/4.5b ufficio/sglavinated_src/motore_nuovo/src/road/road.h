// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "road/road_segment.h"
#include <vector>

namespace sgl {

// Strada a segmenti con curve e salite.
// Genera il tracciato da una lista di descrittori (curve, colline) e
// proietta i segmenti visibili nello spazio dello schermo (320x224).
class Road {
public:
    static constexpr i32 SEG_LEN = 200;      // lunghezza di un segmento (unita' mondo)
    static constexpr i32 DRAW_DIST = 300;    // quanti segmenti disegnare davanti
    // --- Costanti di proiezione (da tarare su Pi 400) ---
    static constexpr f32 ROAD_WIDTH = 2400.0f;    // larghezza intera carreggiata, unita' mondo
    static constexpr f32 ROAD_HALF_WORLD = 1200.0f; // bordo strada; playerX clampato a ±900 (player.cpp) sta dentro
    static constexpr f32 DEPTH = 0.84f;           // 1/tan(FOV/2), FOV 100°; scala di proiezione s = DEPTH/z
    static constexpr f32 Z_NEAR = 60.0f;          // sotto questa distanza il segmento non si proietta (evita 1/z esploso)
    static constexpr f32 CAM_HEIGHT = 1000.0f; // altezza telecamera
    static constexpr f32 CAM_Z = 0.0f;       // (relativa all'auto, gestita in update)
    // curva di livello.json -> curvatura (variazione di pendenza laterale per unita').
    // Con curva 0.8 la strada all'orizzonte si sposta di circa mezzo schermo.
    static constexpr f32 CURVE_SCALE = 3.0e-4f;
    // collina di livello.json -> quota assoluta della strada (unita' mondo).
    static constexpr f32 HILL_HEIGHT = 6000.0f;
    // Bivio a fine tappa (solo in gara): la strada si apre in due rami.
    static constexpr i32 BIVIO_SEG = 1200;         // segmenti finali occupati dal bivio. Aumentato (FORK1 @developer, 25/10: raggio piu' ampio nell'apertura del bivio, i due rami si separano piu' dolcemente su piu' segmenti, senza scatti di prospettiva; alla scelta bloccata d == BIVIO_DIST e centro del ramo scelto == 0, coerente con il ricentramento in game.cpp).
    // Rettilineo in piano prima e dentro il bivio (Andrea 29/10): parte
    // BIVIO_RETTILINEO_PRIMA segmenti prima dell'apertura e finisce
    // BIVIO_RETTILINEO_DOPO segmenti dopo il blocco della scelta; curve e
    // quote entrano ed escono in BIVIO_RACCORDO segmenti (~1 s a tutta velocita').
    static constexpr i32 BIVIO_RETTILINEO_PRIMA = 200;
    static constexpr i32 BIVIO_RETTILINEO_DOPO = 0;
    static constexpr i32 BIVIO_RACCORDO = 120;
    static constexpr f32 BIVIO_DIST = 1900.0f;     // centro di ogni ramo a bivio aperto (unita' mondo)
    static constexpr f32 BIVIO_SVOLTA = 4.0e-4f;   // quanto il ramo non scelto si allontana (curva via). Aumentato (feedback Andrea 23/10, FORK1: prova runner Mac): i due rami del bivio divergono di piu' cosi' l'auto sul ramo scelto non tocca i bordi della parte centrale al bivio.

    Road();
    explicit Road(u32 seed);

    // Costruisce il tracciato con un segmento per valore (curva, collina).
    // seed per RNG eventuale.
    void build(std::vector<f32> curves, std::vector<f32> hills, u32 seed);

    // Descrittore di un segmento (dati di livello.json): curva/collina/lunghezza.
    // Usato da Game::init per costruire il tracciato da dati.
    struct SegmentDesc {
        f32 curva = 0.0f;
        f32 collina = 0.0f;
        i32 lunghezza = SEG_LEN;
        // Raccordo in entrata/uscita del tratto (segmenti). -1 = automatico:
        // min(lunghezza/3, 60). 0 disabilita il raccordo per quel lato.
        // L'uscita di un tratto raccorda alla curva del tratto successivo
        // (anello: l'ultimo torna al primo).
        i32 entrata = -1;
        i32 uscita = -1;
        // Larghezza della carreggiata (Andrea 27/10: strada che si allarga a piu'
        // corsie): 1 = 2 corsie, 2 = 4 corsie. Raccordata in LARGHEZZA_RACCORDO
        // segmenti; sempre 1 negli ultimi BIVIO_SEG segmenti (bivio).
        f32 larghezza = 1.0f;
        // Dune / rialzi (Andrea 27/10): `onde` dossi alti `dune` (stessa unita'
        // di collina) sopra la quota del tratto; tornano a zero ai due capi.
        // onde = 1 e' un dosso solo (cresta cieca), 3-5 sono dune.
        f32 dune = 0.0f;
        i32 onde = 0;
        // Campo `acqua` (0/1, default 0): quando 1, ai lati del tratto vengono usati
        // i colori `acqua` del tema al posto di `erba`/`erbaAlt` (regola PIST1 punto 1).
        f32 acqua = 0.0f;
        // Quota dell'acqua sotto il ponte (stessa unita' di collina, default 0):
        // con il ponte piu' alto (collina 0.6) l'acqua resta in basso e il
        // ponte si vede sospeso, con i piloni (Andrea 31/10).
        f32 acquaQuota = 0.0f;
    };
    // Segmenti per allargare/stringere la strada: ~1 s a tutta velocita', cosi'
    // chi e' nella corsia esterna ha il tempo di rientrare.
    static constexpr i32 LARGHEZZA_RACCORDO = 150;

// Campo `acqua` (0/1, default 0): quando 1, ai lati del tratto vengono usati
// i colori `acqua` del tema al posto di `erba`/`erbaAlt` (regola PIST1 punto 1).
// Scritta in SegmentDesc e letta in leggiCampiTratto.
    f32 acqua = 0.0f;

    // Costruisce il tracciato da descrittori: ogni descrittore diventa
    // "lunghezza" segmenti da SEG_LEN, con curva e collina raccordate.
    void build(const std::vector<SegmentDesc>& descs, u32 seed);

    // Avanza la posizione "occhi" di dt unita' lungo la strada (chiamato ogni frame).
    void advance(f32 dt);

    // Restituisce la curva (seg.curve) del segmento che contiene la posizione
    // z (unita' mondo). Cammina le lunghezze cumulative con wrap sul tracciato
    // circolare. Restituisce 0 se il tracciato e' vuoto o z non e' trovato.
    // Zero alloc, deterministico, nessun orologio di sistema.
    f32 curvaAt(f32 z) const;
    // Quota della strada (unita' mondo) alla posizione z, interpolata nel segmento.
    f32 quotaAt(f32 z) const;
    // Larghezza della carreggiata (moltiplicatore di ROAD_WIDTH) alla posizione z.
    f32 larghezzaAt(f32 z) const;
    // true se z e' su un ponte (segmento con acqua ai lati).
    bool acquaAt(f32 z) const;
    // Indice del segmento che contiene z (con wrap): O(1), tutti i segmenti
    // sono lunghi SEG_LEN (prima si scorreva tutta la lista a ogni chiamata).
    i32 indiceSegmento(f32 z) const;

    // Proietta i segmenti visibili a partire da playerZ e riempie out (max DRAW_DIST).
    // Carreggiate a una posizione del tracciato: una sola, o i due rami del bivio.
    // centro[] e' lo spostamento laterale dal centro strada (unita' mondo),
    // fattore la larghezza di ciascun ramo rispetto alla strada intera.
    struct Carreggiate {
        f32 centro[2] = {0.0f, 0.0f};
        f32 fattore = 1.0f;
        bool doppia = false;
    };
    // Attiva il bivio sugli ultimi BIVIO_SEG segmenti (gara a tappe). La scelta
    // si blocca a bivioBloccoZ(): da li' i rami sono separati dallo spartitraffico.
    void impostaBivio(bool attivo) { bivio_ = attivo; }
    bool bivioAttivo() const { return bivio_; }
    f32 bivioInizioZ() const { return (f32)(totalLen_ - BIVIO_SEG * SEG_LEN); }
    f32 bivioBloccoZ() const { return bivioInizioZ() + 0.6f * (f32)(BIVIO_SEG * SEG_LEN); }
    // zAbs: distanza dall'inizio della tappa (anche oltre la fine). lato: 0 = scelta
    // aperta (rami a -D e +D), -1/+1 = ramo scelto, ricentrato a 0 (l'altro curva via).
    Carreggiate carreggiate(f32 zAbs, i32 lato) const;

    struct ProjectedSeg {
        i32 index;
        f32 worldX, worldY, worldZ;  // centro strada: x laterale rispetto alla telecamera, quota, distanza
        f32 scale;                   // scala di proiezione (cameraY / zRel)
        f32 screenX;                 // centro orizzontale sullo schermo
        f32 screenY;                 // base sullo schermo
        Carreggiate carr;            // rami del bivio a questa profondita'
        bool acqua = false;          // ponte: acqua ai lati
        f32 acquaY = 0.0f;           // riga dello schermo del pelo dell'acqua a questa distanza
    };
    void project(f32 playerX, f32 playerZ, std::vector<ProjectedSeg>& out, i32 latoBivio = 0) const;

    const std::vector<RoadSegment>& segments() const { return segs_; }
    i32 totalLength() const { return totalLen_; }

    f32 playerX = 0.0f;   // offset laterale dell'auto (input del frame)
    f32 playerZ = 0.0f;   // posizione assoluta sul tracciato
    f32 cameraY = CAM_HEIGHT;

    // Tempo (secondi) trascorso dalla scelta del ramo al bivio. Serve a far
    // tornare gradualmente il fattore del ramo scelto a 1.0 (circa 1 secondo)
    // cosi' il cambio tappa non scatta in larghezza.
    f32 secondiDaSceltaBivio_ = 0.0f;
    void setSecondiDaSceltaBivio(f32 secondi) { secondiDaSceltaBivio_ = secondi; }

    // Curva/collina di inizio tappa: se impostate, build() le usa come punto di
    // partenza del raccordo della prima curva/collina, raccordando il nuovo
    // tracciato con la tappa che finisce (nessuno scatto di prospettiva).
    f32 curvaInizioTappa_ = 0.0f;
    bool curvaInizioTappaSet_ = false;
    f32 collinaInizioTappa_ = 0.0f;
    bool collinaInizioTappaSet_ = false;
    void setCurvaInizioTappa(f32 c) { curvaInizioTappa_ = c; curvaInizioTappaSet_ = true; }
    void setCollinaInizioTappa(f32 c) { collinaInizioTappa_ = c; collinaInizioTappaSet_ = true; }

private:
    std::vector<RoadSegment> segs_;
    i32 totalLen_ = 0;
    f32 curX = 0.0f, curY = 0.0f, curZ = 0.0f; // cursore di costruzione
    u32 seed_ = 1;
    bool bivio_ = false;
};

} // namespace sgl
