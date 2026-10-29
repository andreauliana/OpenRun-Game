// SPDX-License-Identifier: MIT
#include "road/road.h"
#include "render/framebuffer.h" // Framebuffer::WIDTH/HEIGHT per la proiezione
#include <algorithm> // std::clamp, std::min, std::max
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
    // La curva entra con un raccordo in `entrata`, resta costante, poi esce
    // con un raccordo in `uscita` verso la curva del tratto successivo
    // (anello: l'ultimo tratto torna al primo). Default raccordo:
    // min(lunghezza/3, 60) segmenti per lato, mai sovrapposti. La collina e'
    // una quota assoluta (collina * HILL_HEIGHT) raggiunta in modo morbido
    // lungo tutto il tratto. Il tracciato e' un anello: il giunto di fine
    // giro e' continuo come gli altri.
    // Passata 1: curve raccordate con rampe di entrata/uscita a lunghezza
    // propria e curva costante in mezzo.
    auto raccordoAuto = [](const SegmentDesc& d) -> i32 {
        i32 def = d.lunghezza > 0 ? d.lunghezza / 3 : 0;
        if (def > 60) def = 60;
        if (def < 0) def = 0;
        return def;
    };
    // Raccordo con la tappa precedente: se impostato, la prima curva parte dal
    // valore finale della tappa precedente invece che dall'ultimo tratto del
    // tracciato corrente (cambio tappa senza scatto di prospettiva).
    // Senza tappa precedente si parte dalla curva del primo tratto: l'ultimo
    // tratto esce gia' verso quella curva, quindi il giunto di fine giro e'
    // continuo (prima si partiva dalla curva dell'ultimo tratto e, se era una
    // curva, al giro dopo la curva saltava di colpo, fino a 0.85).
    f32 curvaPrima = curvaInizioTappaSet_ ? curvaInizioTappa_ : descs.front().curva;
    f32 larghezzaPrima = 1.0f; // ogni tappa parte (e finisce) a larghezza normale
    i32 indice = 0;
    for (i32 d = 0; d < (i32)descs.size(); ++d) {
        const SegmentDesc& desc = descs[d];
        const i32 n = desc.lunghezza > 0 ? desc.lunghezza : 1;
        i32 ee = desc.entrata >= 0 ? desc.entrata : raccordoAuto(desc);
        i32 uu = desc.uscita >= 0 ? desc.uscita : raccordoAuto(desc);
        if (ee < 0) ee = 0;
        if (uu < 0) uu = 0;
        if (ee + uu > n) { // i raccordi non si sovrappongono
            const f32 scala = (f32)n / (f32)(ee + uu);
            ee = (i32)((f32)ee * scala);
            uu = (i32)((f32)uu * scala);
        }
        const f32 larghezzaDentro = desc.larghezza > 0.5f ? desc.larghezza : 1.0f;
        const i32 rl = std::min(LARGHEZZA_RACCORDO, std::max(1, n / 2));
        const f32 curvaDentro = desc.curva;
        const f32 curvaFuori = descs[(d + 1) % (i32)descs.size()].curva; // anello
        for (i32 k = 1; k <= n; ++k) {
            RoadSegment seg;
            seg.index = indice;
            seg.length = SEG_LEN;
            f32 c;
            if (ee > 0 && k <= ee) {
                c = curvaPrima + (curvaDentro - curvaPrima) * raccordo((f32)k / (f32)ee);
            } else if (uu > 0 && k > n - uu) {
                c = curvaDentro + (curvaFuori - curvaDentro) * raccordo((f32)(k - (n - uu)) / (f32)uu);
            } else {
                c = curvaDentro;
            }
            seg.curve = c;
            seg.hill = desc.collina;
            seg.larghezza = k <= rl ? larghezzaPrima + (larghezzaDentro - larghezzaPrima) * raccordo((f32)k / (f32)rl)
                                    : larghezzaDentro;
            seg.colorRoad = colors[indice % 2];
            seg.colorRumble = rumbles[indice % 2];
            totalLen_ += SEG_LEN;
            segs_.push_back(seg);
            ++indice;
        }
        curvaPrima = curvaFuori; // a fine uscita si e' gia' alla curva del tratto seguente
        larghezzaPrima = larghezzaDentro;
    }
    // Bivio e cambio tappa a larghezza normale: gli ultimi BIVIO_SEG segmenti
    // sono larghi 1 e i LARGHEZZA_RACCORDO prima tornano a 1 con dolcezza.
    {
        const i32 n = (i32)segs_.size();
        const i32 fine = std::max(0, n - BIVIO_SEG);
        const i32 inizio = std::max(0, fine - LARGHEZZA_RACCORDO);
        const f32 da = inizio < n ? segs_[inizio].larghezza : 1.0f;
        for (i32 i = inizio; i < n; ++i) {
            const f32 t = i >= fine ? 1.0f : raccordo((f32)(i - inizio + 1) / (f32)(fine - inizio));
            segs_[i].larghezza = da + (1.0f - da) * t;
        }
    }

    // Chiusura di direzione: la somma ponderata delle curve non si bilancia
    // mai esattamente. La residua si distribuisce SOLO sui segmenti con curva
    // diversa da zero (mai sui rettilinei), cosi' i dritti restano perfettamente
    // dritti e a fine giro la direzione torna a zero.
    f32 residuo = 0.0f;
    i32 lunghezzaCurva = 0;
    for (const RoadSegment& s : segs_) {
        if (s.curve != 0.0f) {
            residuo += s.curve * CURVE_SCALE * (f32)s.length;
            lunghezzaCurva += s.length;
        }
    }
    const f32 correzione =
        lunghezzaCurva > 0 ? residuo / ((f32)lunghezzaCurva * CURVE_SCALE) : 0.0f;

    // Passata 2: posizioni assolute del tracciato.
    f32 quotaPrima = collinaInizioTappaSet_ ? collinaInizioTappa_ * HILL_HEIGHT : descs.back().collina * HILL_HEIGHT;
    f32 direzione = 0.0f; // pendenza laterale del mondo (solo per p1x/p2x)
    curY = quotaPrima;
    size_t at = 0;
    for (const SegmentDesc& d : descs) {
        const i32 n = d.lunghezza > 0 ? d.lunghezza : 1;
        const f32 quota = d.collina * HILL_HEIGHT;
        for (i32 k = 1; k <= n; ++k) {
            RoadSegment& seg = segs_[at++];
            if (seg.curve != 0.0f) seg.curve -= correzione;
            seg.p1x = curX; seg.p1y = curY; seg.p1z = curZ;
            direzione += seg.curve * CURVE_SCALE * (f32)SEG_LEN;
            seg.p2x = curX + direzione * (f32)SEG_LEN;
            seg.p2y = quotaPrima + (quota - quotaPrima) * raccordo((f32)k / (f32)n);
            if (d.dune != 0.0f && d.onde > 0) {
                // sin^2: dossi morbidi, pendenza zero ai capi del tratto.
                const f32 onda = std::sin(PI * (f32)d.onde * (f32)k / (f32)n);
                seg.p2y += d.dune * HILL_HEIGHT * onda * onda;
            }
            seg.p2z = curZ + (f32)SEG_LEN;

            curX = seg.p2x;
            curY = seg.p2y;
            curZ = seg.p2z;
        }
        quotaPrima = quota;
    }
    // I valori di inizio tappa vengono consumati una sola volta.
    curvaInizioTappaSet_ = false;
    collinaInizioTappaSet_ = false;
}

void Road::advance(f32 dt) {
    playerZ += dt;
    if (playerZ >= (f32)totalLen_) playerZ -= (f32)totalLen_;
    // Wrap di playerX a +/-ROAD_HALF_WORLD: playerX arriva già clampato a ±900
    // da player.cpp (X_LIMIT), quindi resta sempre dentro la carreggiata visibile.
    while (playerX >  ROAD_HALF_WORLD) playerX -= 2.0f * ROAD_HALF_WORLD;
    while (playerX < -ROAD_HALF_WORLD) playerX += 2.0f * ROAD_HALF_WORLD;
}

i32 Road::indiceSegmento(f32 z) const {
    if (segs_.empty() || totalLen_ <= 0) return -1;
    // Wrap di z sul tracciato circolare (stessa logica di advance).
    z = std::fmod(z, (f32)totalLen_);
    if (z < 0.0f) z += (f32)totalLen_;
    const i32 i = (i32)(z / (f32)SEG_LEN);
    return std::clamp(i, 0, (i32)segs_.size() - 1);
}

f32 Road::curvaAt(f32 z) const {
    const i32 i = indiceSegmento(z);
    return i >= 0 ? segs_[i].curve : 0.0f;
}

f32 Road::quotaAt(f32 z) const {
    const i32 i = indiceSegmento(z);
    if (i < 0) return 0.0f;
    const RoadSegment& s = segs_[i];
    f32 t = std::fmod(z, (f32)totalLen_);
    if (t < 0.0f) t += (f32)totalLen_;
    t = std::clamp(t / (f32)SEG_LEN - (f32)i, 0.0f, 1.0f);
    return s.p1y + (s.p2y - s.p1y) * t;
}

f32 Road::larghezzaAt(f32 z) const {
    const i32 i = indiceSegmento(z);
    return i >= 0 ? segs_[i].larghezza : 1.0f;
}

Road::Carreggiate Road::carreggiate(f32 zAbs, i32 lato) const {
    Carreggiate c;
    if (totalLen_ <= 0) return c;
    const f32 inizio = bivioInizioZ(), blocco = bivioBloccoZ();
    // Fuori dal bivio: una carreggiata, larga quanto il segmento (piu' corsie).
    // Oltre la fine della tappa (zAbs >= giro) il ramo scelto resta largo 1.
    if (!bivio_ || zAbs <= inizio) {
        c.fattore = zAbs < (f32)totalLen_ ? larghezzaAt(zAbs) : 1.0f;
        return c;
    }
    const f32 apertura = zAbs >= blocco ? 1.0f : raccordo((zAbs - inizio) / (blocco - inizio));
    const f32 d = apertura * BIVIO_DIST;
    c.doppia = true;
    if (lato == 0) {
        c.centro[0] = -d;
        c.centro[1] = d;
        c.fattore = 1.0f - 0.4f * apertura; // strada intera in scelta aperta
    } else {
        // Il giocatore e' stato ricentrato sul ramo scelto al blocco della scelta.
        const f32 oltre = zAbs > blocco ? zAbs - blocco : 0.0f;
        const f32 l = (f32)lato;
        c.centro[0] = l * (d - BIVIO_DIST);
        c.centro[1] = -l * (d + BIVIO_DIST) - l * BIVIO_SVOLTA * oltre * oltre;
        // Subito dopo la scelta il ramo e' stretto (fattore 0.6); torna a 1.0 in
        // circa 1 secondo cosi' il cambio tappa non scatta in larghezza.
        const f32 ramp = secondiDaSceltaBivio_ >= 1.0f ? 1.0f : secondiDaSceltaBivio_;
        c.fattore = 1.0f - 0.4f * apertura * (1.0f - ramp);
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
    const i32 startIdx = indiceSegmento(z0);
    const f32 acc = (f32)startIdx * (f32)SEG_LEN;
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
