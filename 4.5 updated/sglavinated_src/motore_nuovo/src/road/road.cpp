// SPDX-License-Identifier: MIT
#include "road/road.h"
#include "render/framebuffer.h" // Framebuffer::WIDTH/HEIGHT per la proiezione
#include <cmath> // std::fmod per il wrap di z in curvaAt

namespace sgl {

Road::Road() = default;

Road::Road(u32 seed) : seed_(seed) {}

void Road::build(std::vector<f32> curves, std::vector<f32> hills, u32 seed) {
    seed_ = seed;
    segs_.clear();
    curX = 0.0f; curY = 0.0f; curZ = 0.0f;
    totalLen_ = 0;

    i32 n = (i32)curves.size();
    if (n == 0) n = 1;
    if ((i32)hills.size() < n) hills.resize(n, 0.0f);

    // Colori alternati per i blocchi di strada (nessun asset: solo palette).
    u32 colors[2] = {0xFF7A7A7A, 0xFF5A5A5A};
    u32 rumbles[2] = {0xFFFFFFFF, 0xFFFF00FF};

    for (i32 i = 0; i < n; ++i) {
        RoadSegment seg;
        seg.index = i;
        seg.curve = curves[i];
        seg.hill = hills[i];
        seg.colorRoad = colors[i % 2];
        seg.colorRumble = rumbles[i % 2];
        seg.p1x = curX; seg.p1y = curY; seg.p1z = curZ;

        // Il secondo punto e' il primo avanzato di SEG_LEN lungo la curva/collina.
        // Interpolazione semplice: curva e collina agiscono linearmente sul segmento.
        seg.p2x = curX + seg.curve * (f32)SEG_LEN;
        seg.p2y = curY + seg.hill * (f32)SEG_LEN;
        seg.p2z = curZ + (f32)SEG_LEN;

        curX = seg.p2x;
        curY = seg.p2y;
        curZ = seg.p2z;
        totalLen_ += SEG_LEN;

        segs_.push_back(seg);
    }
}

void Road::build(const std::vector<SegmentDesc>& descs, u32 seed) {
    seed_ = seed;
    segs_.clear();
    curX = 0.0f; curY = 0.0f; curZ = 0.0f;
    totalLen_ = 0;

    u32 colors[2] = {0xFF7A7A7A, 0xFF5A5A5A};
    u32 rumbles[2] = {0xFFFFFFFF, 0xFFFF00FF};

    for (size_t i = 0; i < descs.size(); ++i) {
        RoadSegment seg;
        seg.index = (i32)i;
        seg.length = descs[i].lunghezza;
        seg.curve = descs[i].curva;
        seg.hill = descs[i].collina;
        seg.colorRoad = colors[i % 2];
        seg.colorRumble = rumbles[i % 2];
        seg.p1x = curX; seg.p1y = curY; seg.p1z = curZ;

        seg.p2x = curX + seg.curve * (f32)seg.length;
        seg.p2y = curY + seg.hill * (f32)seg.length;
        seg.p2z = curZ + (f32)seg.length;

        curX = seg.p2x;
        curY = seg.p2y;
        curZ = seg.p2z;
        totalLen_ += seg.length;

        segs_.push_back(seg);
    }
}

void Road::advance(f32 dt) {
    playerZ += dt;
    if (playerZ >= (f32)totalLen_) playerZ -= (f32)totalLen_;
    // Wrap di playerX a +/-ROAD_HALF_WORLD: playerX arriva già clampato a ±900
    // da player.cpp (X_LIMIT), quindi resta sempre dentro la carreggiata visibile.
    while (playerX >  ROAD_HALF_WORLD) playerX -= 2.0f * ROAD_HALF_WORLD;
    while (playerX < -ROAD_HALF_WORLD) playerX += 2.0f * ROAD_HALF_WORLD;
}

f32 Road::curvaAt(f32 z) const {
    if (segs_.empty() || totalLen_ <= 0) return 0.0f;
    // Wrap di z sul tracciato circolare (stessa logica di advance).
    z = std::fmod(z, (f32)totalLen_);
    if (z < 0.0f) z += (f32)totalLen_;
    // Cammina le lunghezze cumulative fino al segmento che contiene z.
    f32 acc = 0.0f;
    for (i32 i = 0; i < (i32)segs_.size(); ++i) {
        if (acc + segs_[i].length > z) return segs_[i].curve;
        acc += segs_[i].length;
    }
    return 0.0f; // non trovato (non dovrebbe accadere dopo il wrap)
}

void Road::project(f32 playerX, f32 playerZ, std::vector<ProjectedSeg>& out) const {
    out.clear();
    out.reserve(DRAW_DIST);
    if (segs_.empty() || totalLen_ <= 0) return; // strada vuota: niente da proiettare

    // Proiezione pseudo-3D classica. Riferimento: "JavaScript Racer" di Jake Gordon
    // (licenza MIT, tutorial pubblico) e "Lou's Pseudo 3d Page" — solo idee tecniche,
    // nessun codice copiato (vedi core/types.h).
    // DEPTH = 1/tan(FOV/2) = 1/tan(50°) ~ 0.84, costante (determinismo, niente orologio).
    // Curve e colline sono già integrate in build() (p1x/p2x, p1y/p2y sono quote e
    // ascisse assolute del tracciato): qui NON si ri-integrano, così le colline
    // restano limitate e la strada non sale nel cielo (bug: prima dy += hill veniva
    // integrato due volte).
    const f32 camX0 = playerX; // offset laterale dell'auto dal centro strada

    // Wrap di playerZ sul tracciato circolare.
    f32 z0 = std::fmod(playerZ, (f32)totalLen_);
    if (z0 < 0.0f) z0 += (f32)totalLen_;

    // Segmento di partenza: posizione assoluta del punto sotto la telecamera
    // (centro strada sotto l'auto), interpolata dentro il segmento.
    i32 startIdx = 0;
    f32 acc = 0.0f;
    for (i32 i = 0; i < (i32)segs_.size(); ++i) {
        if (acc + segs_[i].length > z0) { startIdx = i; break; }
        acc += segs_[i].length;
    }
    const RoadSegment& startSeg = segs_[startIdx];
    f32 segT = (z0 - acc) / (f32)startSeg.length; // 0..1 dentro il segmento
    f32 camWorldX = startSeg.p1x + (startSeg.p2x - startSeg.p1x) * segT;
    f32 camWorldY = startSeg.p1y + (startSeg.p2y - startSeg.p1y) * segT;
    // Telecamera: punto sotto l'auto sollevata di CAM_HEIGHT. playerX è l'offset
    // laterale dell'auto; le coordinate ahead sono assolute rispetto al punto
    // sotto l'auto (x e y del segmento, non integrate due volte).
    const f32 camX = camWorldX + camX0;
    const f32 camY = camWorldY + CAM_HEIGHT;

    f32 curZ = 0.0f; // distanza relativa dalla telecamera

    for (i32 i = 0; i < DRAW_DIST; ++i) {
        i32 idx = (startIdx + i) % (i32)segs_.size();
        const RoadSegment& seg = segs_[idx];

        // Primo segmento: si avanza solo della parte ancora davanti alla telecamera,
        // poi interi segmenti. Le coordinate sono le quote assolute p2x/p2y (fine
        // segmento), cosi' i trapezi sono contigui e continui.
        const f32 frac = (i == 0) ? (1.0f - segT) : 1.0f;
        f32 curZInc = (f32)seg.length * frac;
        curZ += curZInc;
        f32 curX = seg.p2x;
        f32 curY = seg.p2y;

        f32 z = curZ; // distanza relativa alla telecamera (playerZ già avvolto sopra)
        if (z < Z_NEAR) continue; // troppo vicino: evita 1/z esploso

        f32 s = DEPTH / z;

        f32 sx = 160.0f + s * (curX - camX) * 160.0f;  // W/2 = 160
        f32 sy = 112.0f - s * (curY - camY) * 112.0f;  // H/2 = 112, orizzonte ~112

        ProjectedSeg ps;
        ps.index = idx;
        ps.worldX = curX;
        ps.worldY = curY;
        ps.worldZ = z; // RELATIVO alla telecamera
        ps.scale = s;
        ps.screenX = sx;
        ps.screenY = sy;
        out.push_back(ps);
    }
}

} // namespace sgl
