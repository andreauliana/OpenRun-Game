// SPDX-License-Identifier: MIT
#include "road/road.h"
#include "render/framebuffer.h" // Framebuffer::WIDTH/HEIGHT per la proiezione
#include <cmath> // std::fmod per il wrap di z in curvaAt

namespace sgl {

Road::Road() = default;

Road::Road(u32 seed) : seed_(seed) {}

void Road::build(std::vector<f32> curves, std::vector<f32> hills, u32 seed) {
    // Un descrittore per segmento (lunghezza 1): stesso percorso di costruzione
    // dei livelli da file, quindi stesse curve raccordate e stesse colline.
    if (curves.empty()) curves.push_back(0.0f);
    if (hills.size() < curves.size()) hills.resize(curves.size(), 0.0f);
    std::vector<SegmentDesc> descs(curves.size());
    for (size_t i = 0; i < curves.size(); ++i) {
        descs[i].curva = curves[i];
        descs[i].collina = hills[i];
        descs[i].lunghezza = 1;
    }
    build(descs, seed);
}

namespace {
// Raccordo morbido 0..1 (smoothstep): niente spigoli all'ingresso delle curve.
f32 raccordo(f32 t) { return t * t * (3.0f - 2.0f * t); }
} // namespace

void Road::build(const std::vector<SegmentDesc>& descs, u32 seed) {
    seed_ = seed;
    segs_.clear();
    curX = 0.0f; curY = 0.0f; curZ = 0.0f;
    totalLen_ = 0;
    if (descs.empty()) return;

    u32 colors[2] = {0xFF7A7A7A, 0xFF5A5A5A};
    u32 rumbles[2] = {0xFFFFFFFF, 0xFFFF00FF};

    // "lunghezza" di un descrittore = numero di segmenti da SEG_LEN unita'
    // (i cartelloni di livello.json usano lo stesso indice di segmento).
    // La curva entra con un raccordo nel primo terzo del tratto partendo dal
    // valore del tratto precedente; la collina e' una quota assoluta
    // (collina * HILL_HEIGHT) raggiunta in modo morbido lungo tutto il tratto.
    // Il tracciato e' un anello: si parte dai valori dell'ultimo descrittore,
    // cosi' il giunto di fine giro e' continuo come gli altri.
    f32 curvaPrima = descs.back().curva;
    f32 quotaPrima = descs.back().collina * HILL_HEIGHT;
    f32 direzione = 0.0f; // pendenza laterale del mondo (solo per p1x/p2x)
    curY = quotaPrima;
    i32 indice = 0;
    for (const SegmentDesc& d : descs) {
        const i32 n = d.lunghezza > 0 ? d.lunghezza : 1;
        const i32 rampa = n >= 3 ? n / 3 : n;
        const f32 quota = d.collina * HILL_HEIGHT;
        for (i32 k = 1; k <= n; ++k) {
            RoadSegment seg;
            seg.index = indice;
            seg.length = SEG_LEN;
            const f32 tCurva = k >= rampa ? 1.0f : (f32)k / (f32)rampa;
            seg.curve = curvaPrima + (d.curva - curvaPrima) * raccordo(tCurva);
            seg.hill = d.collina;
            seg.colorRoad = colors[indice % 2];
            seg.colorRumble = rumbles[indice % 2];
            seg.p1x = curX; seg.p1y = curY; seg.p1z = curZ;
            direzione += seg.curve * CURVE_SCALE * (f32)SEG_LEN;
            seg.p2x = curX + direzione * (f32)SEG_LEN;
            seg.p2y = quotaPrima + (quota - quotaPrima) * raccordo((f32)k / (f32)n);
            seg.p2z = curZ + (f32)SEG_LEN;

            curX = seg.p2x;
            curY = seg.p2y;
            curZ = seg.p2z;
            totalLen_ += SEG_LEN;
            segs_.push_back(seg);
            ++indice;
        }
        curvaPrima = d.curva;
        quotaPrima = quota;
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

Road::Carreggiate Road::carreggiate(f32 zAbs, i32 lato) const {
    Carreggiate c;
    if (!bivio_ || totalLen_ <= 0) return c;
    const f32 inizio = bivioInizioZ(), blocco = bivioBloccoZ();
    if (zAbs <= inizio) return c;
    const f32 apertura = zAbs >= blocco ? 1.0f : raccordo((zAbs - inizio) / (blocco - inizio));
    const f32 d = apertura * BIVIO_DIST;
    c.fattore = 1.0f - 0.4f * apertura; // ogni ramo e' piu' stretto della strada intera
    c.doppia = true;
    if (lato == 0) {
        c.centro[0] = -d;
        c.centro[1] = d;
    } else {
        // Il giocatore e' stato ricentrato sul ramo scelto al blocco della scelta.
        const f32 oltre = zAbs > blocco ? zAbs - blocco : 0.0f;
        const f32 l = (f32)lato;
        c.centro[0] = l * (d - BIVIO_DIST);
        c.centro[1] = -l * (d + BIVIO_DIST) - l * BIVIO_SVOLTA * oltre * oltre;
    }
    return c;
}

void Road::project(f32 playerX, f32 playerZ, std::vector<ProjectedSeg>& out, i32 latoBivio) const {
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
    // Wrap di playerZ sul tracciato circolare.
    f32 z0 = std::fmod(playerZ, (f32)totalLen_);
    if (z0 < 0.0f) z0 += (f32)totalLen_;

    // Segmento sotto la telecamera e posizione dentro il segmento.
    i32 startIdx = 0;
    f32 acc = 0.0f;
    for (i32 i = 0; i < (i32)segs_.size(); ++i) {
        if (acc + segs_[i].length > z0) { startIdx = i; break; }
        acc += segs_[i].length;
    }
    const RoadSegment& startSeg = segs_[startIdx];
    const f32 segT = (z0 - acc) / (f32)startSeg.length; // 0..1 dentro il segmento
    const f32 camY = startSeg.p1y + (startSeg.p2y - startSeg.p1y) * segT + CAM_HEIGHT;

    // Curve: la telecamera guarda sempre lungo la strada (come in OutRun), quindi
    // il tratto sotto l'auto punta al centro dello schermo e solo la strada piu'
    // avanti si piega. x e dx sono spostamento e pendenza laterali rispetto alla
    // tangente sotto la telecamera; la curvatura (seg.curve * CURVE_SCALE) li fa
    // crescere con la distanza. Prima seg.curve era usata come pendenza e la
    // strada in curva usciva di sbieco come una retta storta.
    f32 x = 0.0f, dx = 0.0f;
    f32 curZ = 0.0f; // distanza relativa dalla telecamera

    constexpr i32 SAMPLES_PER_SEGMENT = 4;
    out.reserve((size_t)DRAW_DIST * SAMPLES_PER_SEGMENT);
    for (i32 i = 0; i < DRAW_DIST; ++i) {
        i32 idx = (startIdx + i) % (i32)segs_.size();
        const RoadSegment& seg = segs_[idx];
        const RoadSegment& prev = segs_[(idx + (i32)segs_.size() - 1) % (i32)segs_.size()];

        const f32 tStart = (i == 0) ? segT : 0.0f;
        const f32 passo = (f32)seg.length * (1.0f - tStart) / (f32)SAMPLES_PER_SEGMENT;
        for (i32 sample = 1; sample <= SAMPLES_PER_SEGMENT; ++sample) {
            const f32 t = tStart + (1.0f - tStart) *
                ((f32)sample / (f32)SAMPLES_PER_SEGMENT);
            const f32 tMezzo = t - 0.5f * (1.0f - tStart) / (f32)SAMPLES_PER_SEGMENT;
            const f32 k = (prev.curve + (seg.curve - prev.curve) * tMezzo) * CURVE_SCALE;
            x += dx * passo + 0.5f * k * passo * passo;
            dx += k * passo;
            curZ += passo;
            const f32 curY = seg.p1y + (seg.p2y - seg.p1y) * t;

            const f32 z = curZ; // distanza relativa alla telecamera
            if (z < Z_NEAR) continue;

            const f32 s = DEPTH / z;
            ProjectedSeg ps;
            ps.index = idx;
            ps.worldX = x;
            ps.worldY = curY;
            ps.worldZ = z;
            ps.scale = s;
            ps.screenX = 160.0f + s * (x - playerX) * 160.0f;
            ps.screenY = 112.0f - s * (curY - camY) * 112.0f;
            ps.carr = carreggiate(z0 + z, latoBivio);
            out.push_back(ps);
        }
    }
}

} // namespace sgl
