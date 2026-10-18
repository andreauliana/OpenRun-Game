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
    };

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

    // Proietta i segmenti visibili a partire da playerZ e riempie out (max DRAW_DIST).
    struct ProjectedSeg {
        i32 index;
        f32 worldX, worldY, worldZ;  // centro strada: x laterale rispetto alla telecamera, quota, distanza
        f32 scale;                   // scala di proiezione (cameraY / zRel)
        f32 screenX;                 // centro orizzontale sullo schermo
        f32 screenY;                 // base sullo schermo
    };
    void project(f32 playerX, f32 playerZ, std::vector<ProjectedSeg>& out) const;

    const std::vector<RoadSegment>& segments() const { return segs_; }
    i32 totalLength() const { return totalLen_; }

    f32 playerX = 0.0f;   // offset laterale dell'auto (input del frame)
    f32 playerZ = 0.0f;   // posizione assoluta sul tracciato
    f32 cameraY = CAM_HEIGHT;
private:
    std::vector<RoadSegment> segs_;
    i32 totalLen_ = 0;
    f32 curX = 0.0f, curY = 0.0f, curZ = 0.0f; // cursore di costruzione
    u32 seed_ = 1;
};

} // namespace sgl
