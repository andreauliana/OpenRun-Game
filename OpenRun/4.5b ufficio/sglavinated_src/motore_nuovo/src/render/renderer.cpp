// SPDX-License-Identifier: MIT
#include "render/renderer.h"
#include "render/font_hud.h"
#include "render/font_mini.h"
#include <cstdio>
#include <algorithm>
#include <cmath> // std::fmod per il wrap di zRel
#include <utility> // std::move

namespace sgl {
namespace {
// Ciclo lungo (10 segmenti): a velocita' massima la pista avanza ~500 unita' a
// frame, con un ciclo di 800 le strisce sembravano ferme o all'indietro.
constexpr f32 LANE_DASH_CYCLE = 2000.0f;
constexpr f32 LANE_DASH_LENGTH = 1000.0f;
constexpr f32 LINEA_ARRIVO_PROF = 600.0f; // traguardo: 3 file di scacchi da 200 unita'
constexpr f32 ROADSIDE_MARKER_SPACING = 600.0f;
}

Renderer::Renderer(Framebuffer& fb, const Palette& pal) : fb_(fb), pal_(pal) {
    // Preallocato una volta: zero allocazioni per frame nel ciclo di gioco.
    projs_.reserve(Road::DRAW_DIST);
}

i32 Renderer::aggiungiSponsorSprite(Sprite sprite) {
    // Caricato una volta all'avvio: lo sprite resta di proprieta' del Renderer;
    // SpriteInstance ne memorizza solo il puntatore (nessuna copia per frame).
    sponsorSprites_.push_back(std::move(sprite));
    return (i32)sponsorSprites_.size() - 1;
}

i32 Renderer::aggiungiOggettoSprite(Sprite sprite) {
    // Caricato una volta in caricaLivello: lo sprite resta di proprieta' del
    // Renderer; SpriteInstance ne memorizza solo il puntatore.
    oggettoSprites_.push_back(std::move(sprite));
    return (i32)oggettoSprites_.size() - 1;
}

void Renderer::impostaSkyline(Sprite sprite) {
    // Skyline della tappa in corso (DECO1 2b). Copia una volta: zero alloc nel
    // ciclo di gioco. Sprite vuoto ripristina il cielo piatto. skylineA_ e' il
    // bersaglio di una eventuale transizione di tema (FORK1 punto 6).
    skylineA_ = std::move(sprite);
    skylineSprite_ = skylineA_;
}

void Renderer::scorrereSkyline(f32 dx) {
    skylineScrollX_ += dx; // accumulato, il wrap avviene in drawSkyline (zero alloc)
}

void Renderer::drawRoad(const Road& road, f32 playerX, f32 playerZ, i32 latoBivio) {
    const auto& segs = road.segments();
    if (segs.empty()) return;

    projs_.clear();
    road.project(playerX, playerZ, projs_, latoBivio);
    if (projs_.empty()) return;

    // playerX del frame: drawSprites riusa la stessa proiezione della strada.
    lastPlayerX_ = playerX; // (riservato; drawSprites usa il centro strada proiettato)
    giro_ = (f32)road.totalLength();

    // Cielo (parte superiore dello schermo) e terra (sotto l'orizzonte).
    fb_.fillRect(0, 0, Framebuffer::WIDTH - 1, HORIZON_Y - 1, tema_.cielo);
    // Skyline sopra l'orizzonte (DECO1 2b: ogni tappa il suo skyline; durante la
    // transizione di tema si incrocia con il precedente, FORK1 punto 6).
    if (transizioneT_ < 1.0f) drawSkylineTransizione(skylineDa_, skylineA_, transizioneT_);
    else if (skylineSprite_.width > 0) drawSkyline(skylineSprite_);
    fb_.fillRect(0, HORIZON_Y, Framebuffer::WIDTH - 1, Framebuffer::HEIGHT - 1, tema_.erba);

    // Disegna i segmenti dal piu' LONTANO al piu' VICINO: projs_ e' ordinato
    // vicino->lontano (i=0 e' il segmento piu' vicino), quindi si itera a ritroso.
    // Il vicino, disegnato per ultimo, copre correttamente il lontano (painter).
    for (size_t i = projs_.size() - 1; i > 0; --i) {
        const auto& vicino = projs_[i - 1];
        const auto& lontano = projs_[i];
        // Salta se il segmento vicino non e' sotto il lontano (degenero/contigui).
        if (lontano.screenY >= vicino.screenY) continue;
        // Bande alternate ancorate ai segmenti del mondo, non allo schermo.
        drawSegmentTrapezoid(vicino, lontano, ((vicino.index / BAND_SEGS) & 1) == 0, playerZ);
        // Piloni del ponte: al confine fra due segmenti, ogni PILONE_OGNI_SEG,
        // dai bordi dell'impalcato giu' fino all'acqua.
        if (vicino.acqua && lontano.acqua && lontano.index != vicino.index &&
            (lontano.index % PILONE_OGNI_SEG) == 0)
            drawPilone(vicino);
    }
    drawRoadsideMarkers(playerZ);
}

void Renderer::drawSegmentTrapezoid(const Road::ProjectedSeg& cur,
                                    const Road::ProjectedSeg& next,
                                    bool band, f32 playerZ) {
    // cur = bordo VICINO (y maggiore), next = bordo LONTANO (y minore).
    // Riferimento tecnico: bande alternate di colore per segmento come nei racer
    // pseudo-3D (Lou's Pseudo 3d Page, Cannonball come riferimento di resa) — solo idea.
    const i32 cy = (i32)cur.screenY;
    const i32 ny = (i32)next.screenY;
    // Strada sospesa (ponte e sue rampe): il terreno ai lati e' un piano piu'
    // in basso, a quotaAcqua. Ogni tratto dipinge prima la sua fascia di
    // terreno (acqua sul ponte, erba sulle rampe: Andrea 06/11, prima l'erba
    // saliva con la rampa fino alla carreggiata), poi la strada sopra; dal
    // lontano al vicino, quindi la strada vicina copre il terreno.
    const bool sospeso = cur.sospeso && next.sospeso;
    const bool ponte = sospeso && cur.acqua && next.acqua;
    if (sospeso) {
        const i32 a0 = std::max((i32)next.acquaY, HORIZON_Y);
        const i32 a1 = std::min((i32)cur.acquaY, Framebuffer::HEIGHT - 1);
        const u32 acqua = ponte ? (band ? tema_.acqua : tema_.acquaAlt) : (band ? tema_.erba : tema_.erbaAlt);
        const u32 riflesso = lerpColore(tema_.acqua, 0xFFFFFFFFu, 0.35f);
        const f32 ya = next.acquaY, yb = cur.acquaY;
        for (i32 y = a0; y <= a1; ++y) {
            fb_.hline(0, Framebuffer::WIDTH - 1, y, acqua);
            if (!ponte) continue; // erba sotto le rampe: niente onde
            // Riflessi sulle onde in prospettiva: piccoli dall'alto, danno la
            // misura dell'altezza. Ancorati al mondo (z e x), deterministici.
            const f32 t = yb > ya ? ((f32)y - ya) / (yb - ya) : 1.0f;
            const f32 invZ = (1.0f - t) / next.worldZ + t / cur.worldZ;
            if (invZ <= 0.0f) continue;
            const f32 zw = 1.0f / invZ;
            const i32 fila = (i32)std::floor((playerZ + zw) / ONDA_PASSO_Z);
            if ((fila & 1) == 0) continue;
            const f32 xPerPx = zw / (Road::DEPTH * (Framebuffer::WIDTH / 2.0f)); // unita' mondo per pixel
            for (i32 x = 0; x < Framebuffer::WIDTH; ++x) {
                const f32 xw = ((f32)x - Framebuffer::WIDTH / 2.0f) * xPerPx + lastPlayerX_;
                const i32 cella = (i32)std::floor(xw / ONDA_PASSO_X);
                const u32 hsh = (u32)(fila * 73856093) ^ (u32)(cella * 19349663);
                if ((hsh % 7u) != 0u) continue;
                const f32 dentro = xw / ONDA_PASSO_X - (f32)cella;
                if (dentro < 0.45f) fb_.put(x, y, riflesso);
            }
        }
    }
    const i32 dy = cy - ny;
    if (dy <= 0) return;

    // Mezza larghezza strada in px, coerente con Road::project (W/2 * scala * ROAD_HALF_WORLD).
    // Al bivio ci sono due rami: stesso disegno, centro spostato e larghezza ridotta.
    const f32 pxNear = cur.scale  * (Framebuffer::WIDTH / 2.0f);
    const f32 pxFar  = next.scale * (Framebuffer::WIDTH / 2.0f);
    const f32 hwNear = pxNear * Road::ROAD_HALF_WORLD * cur.carr.fattore;
    const f32 hwFar  = pxFar  * Road::ROAD_HALF_WORLD * next.carr.fattore;
    const i32 rami = (cur.carr.doppia || next.carr.doppia) ? 2 : 1;

    // Strada sospesa: parapetto basso grigio/bianco al posto del rumble; ai
    // lati niente erba alla quota della strada (il terreno e' gia' stato
    // dipinto piu' in basso, sopra).
    const u32 grass  = band ? tema_.erba : tema_.erbaAlt;
    const u32 road   = band ? pal_[2] : pal_[3];
    const u32 rumble = sospeso ? (band ? pal_[5] : 0xFF9A9AA6) : (band ? pal_[5] : tema_.rumbleAlt); // bianco / colore della tappa
    const u32 lane   = pal_[1];

    for (i32 y = ny; y <= cy; ++y) {
        // t = 0 sul bordo lontano (y = ny), 1 sul bordo vicino (y = cy).
        const f32 t  = (f32)(y - ny) / (f32)dy;
        const f32 hw = hwFar + (hwNear - hwFar) * t;
        if (y >= HORIZON_Y && !sospeso) // sopra l'orizzonte resta il cielo (le colline non cambiano il fondale)
            fb_.hline(0, Framebuffer::WIDTH - 1, y, grass);
        // Strada allargata (fattore > 1, una sola carreggiata): rumble e strisce
        // restano della misura della strada normale.
        const f32 fattore = next.carr.fattore + (cur.carr.fattore - next.carr.fattore) * t;
        const f32 hwBase = (rami == 1 && fattore > 1.0f) ? hw / fattore : hw;
        // Corsie larghe come quelle della strada normale (mezza strada = una
        // corsia): strisce al centro e ogni hwBase verso i bordi. Le corsie
        // nuove nascono dai bordi mentre la strada si allarga (2 -> 4 corsie).
        i32 stricePerLato = 0;
        while ((f32)(stricePerLato + 1) * hwBase < hw - 0.25f * hwBase) ++stricePerLato;
        // Rumble: larghezza proporzionale alla larghezza locale, quindi continua
        // fra segmenti contigui (nessun gradino ai confini).
        i32 rw = (i32)(hwBase * 0.14f);
        if (rw < 1) rw = 1;
        const f32 invZ = (1.0f - t) / next.worldZ + t / cur.worldZ;
        const f32 distance = invZ > 0.0f ? 1.0f / invZ : next.worldZ;
        // Fase ancorata alla pista (playerZ + distanza), come i paletti: le
        // strisce scorrono con la velocita' invece di restare ferme sullo schermo.
        f32 dashPhase = std::fmod(playerZ + distance, LANE_DASH_CYCLE);
        if (dashPhase < 0.0f) dashPhase += LANE_DASH_CYCLE;
        // Traguardo: tre file di scacchi appena dopo la fine del tracciato.
        i32 filaScacchi = -1;
        if (lineaArrivo_ && giro_ > 0.0f) {
            f32 zPista = std::fmod(playerZ + distance, giro_);
            if (zPista < 0.0f) zPista += giro_;
            if (zPista < LINEA_ARRIVO_PROF) filaScacchi = (i32)(zPista * 3.0f / LINEA_ARRIVO_PROF);
        }
        for (i32 r = 0; r < rami; ++r) {
            const f32 cNear = cur.screenX + cur.carr.centro[r] * pxNear;
            const f32 cFar  = next.screenX + next.carr.centro[r] * pxFar;
            const f32 cx = cFar + (cNear - cFar) * t;
            const i32 lx = (i32)(cx - hw);
            const i32 rx = (i32)(cx + hw);
            if (rx < 0 || lx >= Framebuffer::WIDTH) continue; // ramo fuori schermo
            fb_.hline(lx, rx, y, road);
            fb_.hline(lx, lx + rw - 1, y, rumble);
            fb_.hline(rx - rw + 1, rx, y, rumble);
            if (filaScacchi >= 0) {
                // 16 caselle da bordo a bordo, alternate a ogni fila.
                const f32 casella = (f32)(rx - lx + 1) / 16.0f;
                for (i32 c = 0; c < 16; ++c) {
                    const i32 x0 = lx + (i32)((f32)c * casella), x1 = lx + (i32)((f32)(c + 1) * casella) - 1;
                    fb_.hline(x0, x1, y, ((c + filaScacchi) & 1) ? 0xFFF4F4F4u : 0xFF141414u);
                }
                continue;
            }
            if (dashPhase < LANE_DASH_LENGTH) {
                i32 lw = (i32)(hwBase * 0.02f);
                if (lw < 1) lw = 1;
                for (i32 k = -stricePerLato; k <= stricePerLato; ++k) {
                    const i32 sx = (i32)(cx + hwBase * (f32)k);
                    fb_.hline(sx - lw, sx + lw, y, lane);
                }
            }
        }
    }
}

void Renderer::drawPilone(const Road::ProjectedSeg& p) {
    // Torri del ponte alto (Andrea 31/10, "piu' cinematografico"): due torri
    // fuori dalla carreggiata, dall'acqua fino a sopra la strada, unite in
    // alto da una trave (si passa sotto un portale). Solo se il ponte e' alto.
    const i32 deck = (i32)p.screenY;
    const i32 base = std::min((i32)p.acquaY, Framebuffer::HEIGHT - 1);
    if (base <= deck + 2) return; // ponte a filo d'acqua: niente torri
    const f32 pxUnita = p.scale * (Framebuffer::WIDTH / 2.0f);
    const f32 hw = pxUnita * Road::ROAD_HALF_WORLD * p.carr.fattore;
    const f32 pxY = p.scale * 112.0f; // pixel per unita' di quota a questa distanza
    const i32 cima = deck - (i32)(TORRE_SOPRA * pxY);
    const i32 w = std::clamp((i32)(pxUnita * TORRE_LARGHEZZA), 1, 60);
    const i32 xs[2] = {(i32)(p.screenX - hw * TORRE_DISTANZA), (i32)(p.screenX + hw * TORRE_DISTANZA)};
    for (i32 cx : xs) {
        if (cx + w < 0 || cx - w >= Framebuffer::WIDTH) continue;
        fb_.fillRect(cx - w / 2, cima, cx + w / 2, base, 0xFF3C3C4Au);
        fb_.fillRect(cx - w / 2, cima, cx - w / 2 + std::max(0, w / 4), base, 0xFF707084u); // lato in luce
        if (w >= 4) fb_.fillRect(cx, cima, cx, base, tema_.rumbleAlt); // filo di luce del tema
    }
    // Trave del portale e filo di luce del tema sotto.
    const i32 trave = std::max(1, (i32)(TORRE_TRAVE * pxY));
    fb_.fillRect(xs[0] - w / 2, cima, xs[1] + w / 2, cima + trave, 0xFF2E2E3Cu);
    fb_.hline(xs[0] + w / 2, xs[1] - w / 2, cima + trave, tema_.rumbleAlt);
}

void Renderer::drawRoadsideMarkers(f32 playerZ) {
    for (const Road::ProjectedSeg& p : projs_) {
        f32 phase = std::fmod(playerZ + p.worldZ, ROADSIDE_MARKER_SPACING);
        if (phase < 0.0f) phase += ROADSIDE_MARKER_SPACING;
        if (phase >= 50.0f || p.screenY < HORIZON_Y ||
            p.screenY >= (f32)Framebuffer::HEIGHT)
            continue;
        // Al bivio i paletti apparirebbero su un solo ramo: niente paletti finche'
        // le carreggiate sono due (feedback Andrea 23/10).
        if (p.carr.doppia) continue;
        // Ponte e rampe (Andrea 06/11): bordo aperto sul vuoto, niente paletti.
        if (p.sospeso) continue;

        const f32 pxUnita = p.scale * (Framebuffer::WIDTH / 2.0f);
        const f32 halfRoad = pxUnita * Road::ROAD_HALF_WORLD * p.carr.fattore;
        const f32 sinistra = std::min(p.carr.centro[0], p.carr.centro[1]) * pxUnita;
        const f32 destra = std::max(p.carr.centro[0], p.carr.centro[1]) * pxUnita;
        const i32 height = std::clamp((i32)(p.scale * 300.0f * (Framebuffer::HEIGHT / 2.0f)), 2, 28);
        const i32 width = std::clamp((i32)(p.scale * 35.0f * (Framebuffer::WIDTH / 2.0f)), 1, 4);
        const i32 bottom = (i32)p.screenY;
        for (i32 side : {-1, 1}) {
            const i32 x = (i32)(p.screenX + (side < 0 ? sinistra : destra) + side * (halfRoad + width * 2.0f));
            const u32 color = side < 0 ? pal_[1] : pal_[14];
            fb_.fillRect(x - width / 2, bottom - height, x + width / 2, bottom, color);
            fb_.hline(x - width / 2, x + width / 2, bottom - height, pal_[5]);
        }
    }
}

void Renderer::drawDetrito(i32 x, i32 y, i32 w, i32 h, f32 angolo, u32 colore) {
    const f32 co = std::cos(angolo), si = std::sin(angolo);
    const i32 r = (w + h) / 2 + 1;
    for (i32 py = y - r; py <= y + r; ++py)
        for (i32 px = x - r; px <= x + r; ++px) {
            const f32 dx = (f32)(px - x), dy = (f32)(py - y);
            const f32 lx = co * dx + si * dy, ly = -si * dx + co * dy;
            if (std::abs(lx) > w * 0.5f || std::abs(ly) > h * 0.5f) continue;
            fb_.put(px, py, ly < -h * 0.5f + 2.0f ? pal_[5] : colore);
        }
}

void Renderer::drawSkyline(const Sprite& s) {
    // Skyline: largo quanto lo schermo, ancorato all'orizzonte, proporzioni
    // della fonte. Indice 0 = trasparente (resta il colore del cielo).
    if (s.width <= 0 || s.height <= 0) return;
    const f32 scalaX = (f32)Framebuffer::WIDTH / (f32)s.width;
    i32 h = (i32)((f32)s.height * scalaX);
    if (h < 1) h = 1;
    if (h > HORIZON_Y) h = HORIZON_Y; // entra interamente nella fascia cielo
    const i32 y0 = HORIZON_Y - h;
    const bool palettePropria = !s.palette.empty();
    const i32 nPalette = (i32)s.palette.size();
    for (i32 sy = 0; sy < h; ++sy) {
        const i32 ty = (i32)((f32)sy / (f32)h * (f32)s.height);
        const u8* src = s.pixels.data() + (size_t)ty * (size_t)s.width;
        u32* dst = fb_.data() + (size_t)(y0 + sy) * (size_t)Framebuffer::WIDTH;
        for (i32 x = 0; x < Framebuffer::WIDTH; ++x) {
            // Parallasse in curva (DECO1 2e): skyline scorre orizzontalmente
            // rispetto alla strada, con wrap sulla larghezza della fonte.
            const f32 txf = ((f32)x + skylineScrollX_) / (f32)Framebuffer::WIDTH * (f32)s.width;
            i32 tx = (i32)std::fmod(txf, (f32)s.width);
            if (tx < 0) tx += (i32)s.width;
            u8 idx = src[tx];
            if (idx == 0) continue; // trasparente
            u32 col = palettePropria ? s.palette[(size_t)idx % (size_t)nPalette]
                                     : pal_[s.paletteBase + idx];
            if ((col >> 24) == 0) continue; // trasparente
            dst[x] = col;
        }
    }
}

void Renderer::drawSkylineTransizione(const Sprite& da, const Sprite& a, f32 t) {
    // Crossfade skyline durante la transizione di tema: t=0 disegna solo da
    // (tappa che finisce), t=1 solo a (tappa nuova). I pixel trasparenti
    // (alpha 0) lasciano vedere il cielo di tema_, gia' interpolato da drawRoad.
    if (da.width <= 0 && a.width <= 0) return;
    const f32 scalaX = (f32)Framebuffer::WIDTH;
    auto scalaAltezza = [&](const Sprite& s) -> i32 {
        if (s.width <= 0 || s.height <= 0) return 0;
        i32 h = (i32)((f32)s.height * scalaX / (f32)s.width);
        if (h < 1) h = 1;
        if (h > HORIZON_Y) h = HORIZON_Y;
        return h;
    };
    const i32 hDa = scalaAltezza(da), hA = scalaAltezza(a), h = std::max(hDa, hA);
    if (h < 1) return;
    const bool palDa = !da.palette.empty();
    const bool palA = !a.palette.empty();
    const i32 nDa = (i32)da.palette.size(), nA = (i32)a.palette.size();
    u32* dstBase = fb_.data();
    for (i32 sy = 0; sy < h; ++sy) {
        u32* dst = dstBase + (size_t)sy * (size_t)Framebuffer::WIDTH;
        const i32 tyDa = hDa > 0 ? (i32)((f32)sy / (f32)h * (f32)da.height) : 0;
        const i32 tyA = hA > 0 ? (i32)((f32)sy / (f32)h * (f32)a.height) : 0;
        const u8* srcDa = da.width > 0 ? da.pixels.data() + (size_t)tyDa * (size_t)da.width : nullptr;
        const u8* srcA = a.width > 0 ? a.pixels.data() + (size_t)tyA * (size_t)a.width : nullptr;
        for (i32 sx = 0; sx < Framebuffer::WIDTH; ++sx) {
            const i32 txDa = da.width > 0 ? (i32)((f32)sx / (f32)Framebuffer::WIDTH * (f32)da.width) : 0;
            const i32 txA = a.width > 0 ? (i32)((f32)sx / (f32)Framebuffer::WIDTH * (f32)a.width) : 0;
            u8 idxDa = srcDa ? srcDa[txDa] : 0;
            u8 idxA = srcA ? srcA[txA] : 0;
            u32 cDa = 0, cA = 0;
            bool opDa = false, opA = false;
            if (idxDa != 0) {
                cDa = palDa ? da.palette[(size_t)idxDa % (size_t)nDa] : pal_[idxDa];
                opDa = (cDa >> 24) != 0;
            }
            if (idxA != 0) {
                cA = palA ? a.palette[(size_t)idxA % (size_t)nA] : pal_[idxA];
                opA = (cA >> 24) != 0;
            }
            if (!opDa && !opA) continue; // cielo gia' disegnato da drawRoad
            u32 col;
            if (!opDa) col = lerpColore(dst[sx], cA, t);      // da svanisce: resta cielo + a
            else if (!opA) col = lerpColore(cDa, dst[sx], t); // a appare: da + cielo
            else col = lerpColore(cDa, cA, t);                // incrocio da -> a
            dst[sx] = col;
        }
    }
}

void Renderer::iniziaTransizioneTema(const TemaColori& da, const Sprite& daSkyline, f32 durataSecondi) {
    temaDa_ = da;                       // tema della tappa che finisce
    skylineDa_ = daSkyline;             // skyline della tappa che finisce (foto prima del cambio)
    // temaA_ e' il tema di destinazione (impostato da impostaTema) e resta fisso:
    // aggiornaTransizioneTema porta tema_ da temaDa_ a temaA_. Prima si
    // interpolava verso tema_ stesso, sovrascritto a ogni frame, e la tappa
    // nuova restava con i colori della precedente.
    transizioneDurataS_ = durataSecondi;
    transizioneT_ = 0.0f;
    tema_ = da;
}

bool Renderer::aggiornaTransizioneTema(f32 dt) {
    if (transizioneT_ >= 1.0f || transizioneDurataS_ <= 0.0f) {
        transizioneT_ = 1.0f;
        tema_ = temaA_;
        skylineSprite_ = skylineA_; // fine transizione: lo skyline della tappa nuova diventa corrente
        return false;
    }
    transizioneT_ += dt / transizioneDurataS_;
    if (transizioneT_ > 1.0f) transizioneT_ = 1.0f;
    tema_.cielo = lerpColore(temaDa_.cielo, temaA_.cielo, transizioneT_);
    tema_.erba = lerpColore(temaDa_.erba, temaA_.erba, transizioneT_);
    tema_.erbaAlt = lerpColore(temaDa_.erbaAlt, temaA_.erbaAlt, transizioneT_);
    tema_.rumbleAlt = lerpColore(temaDa_.rumbleAlt, temaA_.rumbleAlt, transizioneT_);
    tema_.acqua = lerpColore(temaDa_.acqua, temaA_.acqua, transizioneT_);
    tema_.acquaAlt = lerpColore(temaDa_.acquaAlt, temaA_.acquaAlt, transizioneT_);
    return transizioneT_ < 1.0f;
}

u32 Renderer::lerpColore(u32 da, u32 a, f32 t) {
    const auto canale = [](u32 c, i32 shift) { return (i32)((c >> shift) & 0xFF); };
    const auto mix = [](i32 x, i32 y, f32 t) { return (i32)(x + (y - x) * t); };
    const i32 ra = canale(da, 16), ga = canale(da, 8), ba = canale(da, 0);
    const i32 rb = canale(a, 16), gb = canale(a, 8), bb = canale(a, 0);
    return 0xFF000000u |
           ((u32)(mix(ra, rb, t) & 0xFF) << 16) |
           ((u32)(mix(ga, gb, t) & 0xFF) << 8) |
           ((u32)(mix(ba, bb, t) & 0xFF));
}

void Renderer::drawSprites(std::vector<SpriteInstance>& sprites, f32 playerZ, f32 totalLen) {
    if (projs_.empty()) return; // drawRoad deve girare prima (stessa proiezione)
    // Ordina per distanza dalla telecamera decrescente (dietro prima). La
    // distanza tiene conto del giro: vicino al punto di wrap gli sprite di
    // inizio pista sono lontani anche se hanno worldZ piccolo.
    auto distanza = [playerZ, totalLen](const SpriteInstance& s) {
        f32 z = s.worldZ - playerZ;
        if (totalLen > 0.0f) {
            z = std::fmod(z, totalLen);
            if (z < 0.0f) z += totalLen;
        }
        return z;
    };
    std::sort(sprites.begin(), sprites.end(),
              [&distanza](const SpriteInstance& a, const SpriteInstance& b) {
                  return distanza(a) > distanza(b);
              });
    // projs_ e' ordinato per worldZ crescente (distanza relativa alla telecamera).
    i32 nSegnalini = 0;
    for (auto& s : sprites) {
        // zRel con modulo totalLen: playerZ avvolge sul giro mentre worldZ e'
        // assoluto, quindi vicino al punto di wrap gli sprite non spariscono né
        // appaiono in anticipo.
        f32 zRel = s.worldZ - playerZ;
        if (totalLen > 0.0f) {
            zRel = std::fmod(zRel, totalLen);
            if (zRel < 0.0f) zRel += totalLen;
        }
        // Dietro la telecamera, o cosi' vicino da coprire lo schermo intero
        // (succede passando accanto a un cartello): non si disegna.
        if (zRel < Road::Z_NEAR) continue;
        // Stessa proiezione della strada: s = depth/z, sx = W/2 + s*(worldX - camX)*W/2.
        f32 scl = Road::DEPTH / zRel;
        // Base verticale: screenY del primo segmento con worldZ >= zRel
        // (lo sprite appoggia sulla carreggiata al suo punto di profondita').
        // lower_bound su projs_ ordinato: O(log DRAW_DIST) invece di O(sprite x DRAW_DIST).
        auto it = std::lower_bound(projs_.begin(), projs_.end(), zRel,
            [](const Road::ProjectedSeg& p, f32 val) { return p.worldZ < val; });
        f32 sy = (it != projs_.end()) ? it->screenY : -1.0f;
        if (sy < 0.0f) continue; // oltre DRAW_DIST: fuori portata
        // worldX e' laterale rispetto all'asse della strada: si parte dal centro strada
        // proiettato a quella profondita' (it->screenX, che include curve e camera), cosi'
        // cartelloni e oggetti seguono le curve invece di restare su una retta.
        f32 sx = it->screenX + scl * s.worldX * (f32)Framebuffer::WIDTH / 2;
        if (!s.sprite) {
            const f32 projectionScale = scl * (f32)Framebuffer::WIDTH / 2;
            const i32 w = std::max(4, (i32)(26.0f * projectionScale));
            const i32 h = std::max(3, (i32)(18.0f * projectionScale));
            const i32 cx = (i32)sx;
            const i32 cy = (i32)sy;
            fb_.fillRect(cx - w / 2, cy - h, cx + w / 2, cy, pal_[s.paletteBase]);
            fb_.fillRect(cx - w / 4, cy - h + 1, cx + w / 4, cy - h / 2, pal_[10]);
            fb_.fillRect(cx - w / 2, cy - 2, cx - w / 3, cy + 1, pal_[14]);
            fb_.fillRect(cx + w / 3, cy - 2, cx + w / 2, cy + 1, pal_[14]);
            if (s.segnalino && nSegnalini < SEGNALINI_MAX) segnalini_[nSegnalini++] = {cx, cy - h, h, s.segnalino};
            continue;
        }
        // Scala in pixel: stessa proiezione della strada (scl * W/2 per unita' mondo)
        // per le unita' mondo di un pixel sprite. Prima si passava scl da solo
        // (~0.001 a distanza): w e h valevano 0 e nessuno sprite compariva mai.
        const f32 scala = scl * (f32)Framebuffer::WIDTH / 2 * SPRITE_WORLD_PX * s.scale;
        drawSprite(s, sx, sy, scala);
        if (s.segnalino && nSegnalini < SEGNALINI_MAX) {
            const i32 h = (i32)(s.sprite->height * scala);
            segnalini_[nSegnalini++] = {(i32)sx, (i32)sy - h, h, s.segnalino};
        }
    }
    for (i32 i = 0; i < nSegnalini; ++i) drawSegnalino(segnalini_[i]);
}

void Renderer::drawSegnalino(const Segnalino& e) {
    // Freccia (7 px, bordo nero) del colore dell'auto sopra il tetto del
    // rivale. Solo pixel pieni.
    // Auto gia' a fianco (base sotto lo schermo): niente freccia sospesa sull'erba.
    if (e.y < 4 || e.y + e.h >= Framebuffer::HEIGHT) return;
    const i32 punta = e.y - 2 - std::min(4, e.h / 6); // stacco dal tetto, cresce un poco da vicino
    for (i32 r = 0; r <= 5; ++r) // bordo nero: la freccia allargata di 1 px
        fb_.hline(e.x - std::min(r, 4), e.x + std::min(r, 4), punta + 1 - r, 0xFF000000u);
    for (i32 r = 0; r <= 3; ++r)
        fb_.hline(e.x - r, e.x + r, punta - r, e.colore);
}

void Renderer::drawSprite(const SpriteInstance& s, f32 screenX, f32 screenY, f32 scale) {
    if (!s.sprite) return;
    i32 w = (i32)(s.sprite->width * scale);
    i32 h = (i32)(s.sprite->height * scale);
    if (w <= 0 || h <= 0) return;
    i32 x0 = (i32)screenX - w / 2;
    i32 y0 = (i32)screenY - h;
    const bool palettePropria = !s.sprite->palette.empty();
    const i32 nPalette = (i32)s.sprite->palette.size();
    // Ritaglio allo schermo: da vicino lo sprite puo' essere molto piu' grande del
    // framebuffer, e senza ritaglio il ciclo scorrerebbe pixel invisibili.
    const i32 syIni = y0 < 0 ? -y0 : 0;
    const i32 syFin = (y0 + h > Framebuffer::HEIGHT) ? Framebuffer::HEIGHT - y0 : h;
    const i32 sxIni = x0 < 0 ? -x0 : 0;
    const i32 sxFin = (x0 + w > Framebuffer::WIDTH) ? Framebuffer::WIDTH - x0 : w;
    const i32 sw = s.sprite->width;
    const i32 sh = s.sprite->height;
    // Colore del texel (tx, ty); alpha 0 = trasparente.
    auto texel = [&](i32 tx, i32 ty) -> u32 {
        const u8 idx = s.sprite->pixels[ty * sw + tx];
        // Palette RGBA di proprietà dello sprite (colori dei cartelloni sponsor).
        if (palettePropria) return s.sprite->palette[idx % nPalette];
        // Palette del motore: indice 0 = trasparente.
        return idx == 0 ? 0u : pal_[s.paletteBase + idx];
    };
    for (i32 sy = syIni; sy < syFin; ++sy) {
        // Righe sorgente coperte da questo pixel schermo: [ty0, ty1).
        const i32 ty0 = (i32)((i64)sy * sh / h);
        i32 ty1 = (i32)((i64)(sy + 1) * sh / h);
        if (ty1 <= ty0) ty1 = ty0 + 1;
        const i32 ty = (ty0 + ty1) / 2;
        for (i32 sx = sxIni; sx < sxFin; ++sx) {
            const i32 tx0 = (i32)((i64)sx * sw / w);
            i32 tx1 = (i32)((i64)(sx + 1) * sw / w);
            if (tx1 <= tx0) tx1 = tx0 + 1;
            u32 col = texel((tx0 + tx1) / 2, ty);
            // Sprite rimpicciolito: un pixel schermo copre piu' texel. Se il
            // texel centrale e' trasparente si cerca un texel pieno nel resto
            // dell'area, cosi' i dettagli sottili (il fusto di un palo largo 2 px
            // in uno sprite da 32) non spariscono a intermittenza quando la
            // larghezza a schermo cambia di frame in frame. Da vicino (area di
            // 1 texel) il risultato e' identico al campionamento semplice.
            for (i32 ry = ty0; (col >> 24) == 0 && ry < ty1; ++ry)
                for (i32 rx = tx0; (col >> 24) == 0 && rx < tx1; ++rx)
                    col = texel(rx, ry);
            if ((col >> 24) == 0) continue; // trasparente (canale alpha nullo)
            fb_.put(x0 + sx, y0 + sy, col);
        }
    }
}

const Sprite* Renderer::risolviPosa(i32 posa, bool& specchia, f32& sterzoResiduo) const {
    specchia = false;
    sterzoResiduo = 0.0f;
    auto c = [this](i32 p) { return giocatoreSprites_[p].width > 0 ? &giocatoreSprites_[p] : nullptr; };
    const bool sx = posa >= POSA_GIOCATORE_SX_1 && posa <= POSA_GIOCATORE_SX_4;
    const bool dx = posa >= POSA_GIOCATORE_DX_1 && posa <= POSA_GIOCATORE_DX_4;
    if (sx || dx) {
        const i32 livello = sx ? posa - POSA_GIOCATORE_SX_1 + 1 : posa - POSA_GIOCATORE_DX_1 + 1; // 1..4
        const i32 base = sx ? POSA_GIOCATORE_SX_1 : POSA_GIOCATORE_DX_1;
        const i32 altro = sx ? POSA_GIOCATORE_DX_1 : POSA_GIOCATORE_SX_1;
        const f32 segno = sx ? -1.0f : 1.0f;
        for (i32 k = livello; k >= 1; --k) {
            const Sprite* p = c(base + k - 1);
            bool sp = false;
            if (!p && (p = c(altro + k - 1))) sp = true;
            if (p) { specchia = sp; sterzoResiduo = segno * (f32)(livello - k) / 4.0f; return p; }
        }
        sterzoResiduo = segno * (f32)livello / 4.0f;
    } else if (const Sprite* p = c(posa)) {
        return p;
    }
    return &giocatoreSprites_[POSA_GIOCATORE_DRITTA];
}

void Renderer::drawPlayerCar(f32 speed, f32 offsetX, bool offroad, f32 rollAngle, f32 sollevamentoPx,
                             bool frena, f32 fumoS, const DanniAuto* danni, i32 posa,
                             f32 distanzaM) {
    // P1 punto 4 + RIV punto 3: sprite vero dell'auto del giocatore dal catalogo
    // auto.giocatore.* (finto 3D alla OutRun). Game sceglie il fotogramma con
    // `posa` (sterzo leggero/forte, salita, discesa); se il fotogramma chiesto
    // non c'e' si ricade su quello dritto, poi sulla sagoma sintetica.
    // Nessun materiale Sega.
    const bool lampo = danni && danni->dallUrtoS >= 0.0f && danni->dallUrtoS < 0.1f;
    i32 cx = 160, cy = 200, w = 34, h = 18;
    bool specchia = false;
    f32 sterzoResiduo = 0.0f;
    const Sprite* s = risolviPosa(posa, specchia, sterzoResiduo);
    // AUTO1 A2: rettangolo del fotogramma e componenti da auto_giocatore.json.
    // Restano a default nel ramo sintetico (li usa solo il fumo, senza punti).
    i32 x0 = 0, y0 = 0;
    bool fotogrammaGriglia = false;
    const PuntiComponenti* pt = nullptr;
    if (s->width > 0) {
        // Sprite vero (AUTO1 A4-A5, Claude 06/11): disegnato a grandezza piena
        // (64x32, scala 1, niente rimpicciolimento a 18 px); l'auto resta
        // centrata (cx=160), e' la strada che si muove sotto di lei. offsetX
        // accenna lo sterzo, sollevamentoPx la solleva alle alte velocita' e
        // rollAngle (ribaltamento, scodata) RUOTA il fotogramma attorno al suo
        // centro, come faceva la sagoma a rettangoli. Nessun lampo sull'urto
        // (Andrea 06/11: non realistico).
        // Scala intera (pixel netti) per arrivare a circa AUTO_GIOCATORE_LARGHEZZA px,
        // calcolata sulla posa dritta cosi' tutte le pose hanno la stessa grandezza
        // (Andrea 06/11: "a bit small"; la scala 1.0 degli agenti la rimpiccioliva a 25 px).
        const Sprite& dritta = giocatoreSprites_[POSA_GIOCATORE_DRITTA];
        // Larghezza della carrozzeria vera (pixel non trasparenti), non del
        // riquadro comune a tutte le pose, che include ruote e fianchi delle curve.
        const Sprite& rif = dritta.width > 0 ? dritta : *s;
        i32 xMin = rif.width, xMax = -1;
        for (i32 y = 0; y < rif.height; ++y)
            for (i32 x = 0; x < rif.width; ++x) {
                const u8 idx = rif.pixels[y * rif.width + x];
                const u32 c = !rif.palette.empty() ? rif.palette[idx % (i32)rif.palette.size()] : (idx ? 0xFF000000u : 0u);
                if ((c >> 24) != 0) { xMin = std::min(xMin, x); xMax = std::max(xMax, x); }
            }
        const i32 baseW = xMax >= xMin ? xMax - xMin + 1 : rif.width;
        const f32 scala = (f32)std::max(1, (i32)std::lround((f32)AUTO_GIOCATORE_LARGHEZZA / (f32)baseW));
        w = std::max(1, (i32)(s->width * scala));
        h = std::max(1, (i32)(s->height * scala));
        cx = 160 + (i32)(offsetX * 0.035f);
        const i32 sollevamento = (i32)(sollevamentoPx + 0.5f);
        cy = AUTO_GIOCATORE_TERRA_Y - sollevamento; // in basso come nelle referenze di Andrea
        if (sollevamento > 0) {
            // Ombra a terra solo quando l'auto e' staccata (salto): a scacchiera,
            // piu' stretta della carrozzeria, niente barra nera piena.
            const i32 mezza = w / 2 - w / 8;
            for (i32 y = AUTO_GIOCATORE_TERRA_Y - 1; y <= AUTO_GIOCATORE_TERRA_Y; ++y)
                for (i32 x = cx - mezza; x <= cx + mezza; ++x)
                    if (((x + y) & 1) == 0) fb_.put(x, y, pal_[0]);
        }
        // Sterzo che la posa non mostra (pose mancanti): leggera inclinazione.
        rollAngle -= STERZO_IMBARDATA * sterzoResiduo;
        // Geometria del disegno (AUTO1 A2): rettangolo del fotogramma e
        // centro di rotazione, usati sia dai componenti sia dai due rami qui
        // sotto (dritto e ruotato).
        x0 = cx - w / 2;
        y0 = cy - h;
        const f32 pcx = (f32)cx, pcy = (f32)cy - h * 0.5f;
        const f32 co = std::cos(rollAngle), si = std::sin(rollAngle);
        // Componenti (ruote, luci stop): valgono per i fotogrammi della
        // griglia di auto_giocatore.json (64x32, disegno separato); le pose
        // di Andrea hanno ruote e fanali gia' nella carrozzeria (nota in
        // auto_giocatore.json) e usano solo il fumo dello scarico.
        const i32 idxPosa = (i32)(s - giocatoreSprites_);
        pt = (idxPosa >= 0 && idxPosa < POSA_GIOCATORE_COUNT) ? &punti_[idxPosa] : nullptr;
        fotogrammaGriglia = pt && pt->validi && s->width == grigliaW_ && s->height == grigliaH_;
        // Componente centrato su un punto della griglia e ruotato con la
        // carrozzeria attorno a (pcx, pcy), con lo stesso campionamento
        // inverso del fotogramma qui sotto; `specchiaC` = lato destro.
        auto disegnaComponente = [&](const Sprite& cp, f32 gx, f32 gy, bool specchiaC) {
            if (cp.width <= 0 || cp.height <= 0 || cp.palette.empty()) return;
            const f32 cw = (f32)cp.width * (f32)scala, ch = (f32)cp.height * (f32)scala;
            const f32 lx = (f32)x0 + gx / (f32)grigliaW_ * (f32)w - pcx;
            const f32 ly = (f32)y0 + gy / (f32)grigliaH_ * (f32)h - pcy;
            const f32 cxs = pcx + co * lx - si * ly;
            const f32 cys = pcy + si * lx + co * ly;
            const i32 r = (i32)(0.5f * std::sqrt(cw * cw + ch * ch)) + 1;
            for (i32 y = (i32)cys - r; y <= (i32)cys + r; ++y)
                for (i32 x = (i32)cxs - r; x <= (i32)cxs + r; ++x) {
                    const f32 dx = (f32)x - cxs, dy = (f32)y - cys;
                    const f32 ul = co * dx + si * dy + cw * 0.5f;
                    const f32 vl = -si * dx + co * dy + ch * 0.5f;
                    if (ul < 0.0f || vl < 0.0f || ul >= cw || vl >= ch) continue;
                    i32 u = (i32)(ul / (f32)scala), v = (i32)(vl / (f32)scala);
                    if (u < 0 || v < 0 || u >= cp.width || v >= cp.height) continue;
                    if (specchiaC) u = cp.width - 1 - u;
                    const u32 col = cp.palette[(u32)cp.pixels[(i32)v * cp.width + u] % cp.palette.size()];
                    if ((col >> 24) == 0) continue;
                    fb_.put(x, y, col);
                }
        };
        // Ruote del componente SOTTO la carrozzeria: il passo 1-2-3 avanza
        // con la distanza percorsa (a ferme non cambia), la destra e' la
        // sinistra specchiata. Sul fotogramma della griglia la carrozzeria
        // le ricopre dove sono gia' disegnate.
        if (fotogrammaGriglia) {
            const i32 passo = distanzaM > 0.0f ? ((i32)(distanzaM * 2.0f) % 3) : 0;
            const Sprite& ruota = ruotaSprites_[passo];
            if (ruota.width > 0) {
                disegnaComponente(ruota, pt->ruotaSx[0], pt->ruotaSx[1], false);
                disegnaComponente(ruota, pt->ruotaDx[0], pt->ruotaDx[1], true);
            }
        }
        // Stop accesi: disegno della posa con i soli fanali accesi (stessa misura).
        const Sprite* stop = nullptr;
        if (frena) {
            const Sprite* st = &stopSprites_[idxPosa];
            if (st->width == s->width && st->height == s->height) stop = st;
        }
        auto grezzo = [&](const Sprite* sp, i32 tx, i32 ty) -> u32 {
            const u8 idx = sp->pixels[ty * sp->width + tx];
            if (!sp->palette.empty()) return sp->palette[idx % (i32)sp->palette.size()];
            return idx == 0 ? 0u : pal_[idx];
        };
        // Ammaccature (Andrea 29/10 e 06/11): restano per tutta la gara, una per
        // botta fino a DANNI_MAX, in punti fissi della carrozzeria (in texel
        // della posa, quindi grandi come i pixel del disegno e ruotano con lei):
        // texel scurito + graffio chiaro in alto a destra. Solo sulla carrozzeria.
        static constexpr f32 SEGNI[DanniAuto::DANNI_MAX][2] = {
            {0.18f, 0.62f}, {0.80f, 0.55f}, {0.34f, 0.80f}, {0.66f, 0.76f},
            {0.12f, 0.45f}, {0.88f, 0.70f}, {0.50f, 0.66f}, {0.42f, 0.50f}};
        const i32 nSegni = danni ? std::min(danni->segni, DanniAuto::DANNI_MAX) : 0;
        auto texel = [&](i32 tx, i32 ty) -> u32 {
            const i32 sx = tx;
            if (specchia) tx = s->width - 1 - tx;
            u32 col = grezzo(s, tx, ty);
            if ((col >> 24) == 0) return col;
            if (stop) {
                const u32 acceso = grezzo(stop, tx, ty);
                if ((acceso >> 24) != 0) col = acceso;
            }
            for (i32 k = 0; k < nSegni; ++k) {
                const i32 dx = (i32)(SEGNI[k][0] * (f32)(s->width - 1) + 0.5f);
                const i32 dy = (i32)(SEGNI[k][1] * (f32)(s->height - 1) + 0.5f);
                if (sx == dx && ty == dy) {
                    const u32 r = ((col >> 16) & 0xFF) * 30 / 100, g = ((col >> 8) & 0xFF) * 30 / 100, b = (col & 0xFF) * 30 / 100;
                    return 0xFF000000u | (r << 16) | (g << 8) | b;
                }
                if (sx == dx + 1 && ty == dy - 1) {
                    const u32 r = (((col >> 16) & 0xFF) + 3 * 0xEC) / 4, g = (((col >> 8) & 0xFF) + 3 * 0xE0) / 4, b = ((col & 0xFF) + 3 * 0xD4) / 4;
                    return 0xFF000000u | (r << 16) | (g << 8) | b;
                }
            }
            return col;
        };
        if (std::abs(rollAngle) < 0.01f) {
            for (i32 y = 0; y < h; ++y)
                for (i32 x = 0; x < w; ++x) {
                    u32 col = texel((i32)(x / scala), (i32)(y / scala));
                    if ((col >> 24) == 0) continue;
                    fb_.put(x0 + x, y0 + y, col);
                }
        } else {
            // Rotazione con campionamento inverso: per ogni pixel dello schermo
            // nel quadrato che contiene l'auto ruotata si cerca il texel.
            // (co, si, pcx, pcy sono calcolati sopra per i componenti.)
            const i32 r = (i32)(0.5f * std::sqrt((f32)(w * w + h * h))) + 1;
            for (i32 y = (i32)pcy - r; y <= (i32)pcy + r; ++y)
                for (i32 x = (i32)pcx - r; x <= (i32)pcx + r; ++x) {
                    const f32 dx = (f32)x - pcx, dy = (f32)y - pcy;
                    const f32 lx = co * dx + si * dy + w * 0.5f;
                    const f32 ly = -si * dx + co * dy + h * 0.5f;
                    if (lx < 0.0f || ly < 0.0f || lx >= (f32)w || ly >= (f32)h) continue;
                    u32 col = texel((i32)(lx / scala), (i32)(ly / scala));
                    if ((col >> 24) == 0) continue;
                    fb_.put(x, y, col);
                }
        }
        // Luci stop del componente (AUTO1 A2): quando la posa non ha la
        // variante con i fanali accesi, il fanale del @grafico sui punti
        // della griglia, destra specchiata. Ordine: ombra, ruote, carrozzeria,
        // stop, fumo.
        if (fotogrammaGriglia && frena && !stop && luciStopSprite_.width > 0) {
            disegnaComponente(luciStopSprite_, pt->fanaleSx[0], pt->fanaleSx[1], false);
            disegnaComponente(luciStopSprite_, pt->fanaleDx[0], pt->fanaleDx[1], true);
        }
    } else {
        // Sagoma sintetica (nessun asset Sega): corpo, abitacolo, ruote, ombra.
        u32 body = offroad ? pal_[12] : pal_[9];
        // Nessun lampo bianco sull'urto (Andrea 06/11).
        u32 cabin = pal_[10];
        // La carrozzeria segue visibilmente la linea scelta, restando nel framebuffer.
        cx = 160 + (i32)(offsetX * 0.035f);
        // Sollevamento alle velocita' piu' alte: l'ombra resta a terra, la
        // carrozzeria sale di sollevamentoPx (moduli_fx.sollevamento_auto_alta_velocita).
        const i32 sollevamento = (i32)(sollevamentoPx + 0.5f);
        if (sollevamento > 0) fb_.fillRect(cx - 15, 199, cx + 15, 201, pal_[0]);
        cy = 200 - sollevamento;
        w = 34; h = 18;
        auto drawRotatedRect = [this, rollAngle](i32 centerX, i32 centerY, i32 rw, i32 rh, u32 color) {
            const f32 c = std::cos(rollAngle);
            const f32 s = std::sin(rollAngle);
            const i32 extent = (rw + rh) / 2 + 1;
            for (i32 y = centerY - extent; y <= centerY + extent; ++y) {
                for (i32 x = centerX - extent; x <= centerX + extent; ++x) {
                    const f32 dx = (f32)(x - centerX);
                    const f32 dy = (f32)(y - centerY);
                    const f32 localX = c * dx + s * dy;
                    const f32 localY = -s * dx + c * dy;
                    if (std::abs(localX) <= rw / 2.0f && std::abs(localY) <= rh / 2.0f)
                        fb_.put(x, y, color);
                }
            }
        };
        drawRotatedRect(cx, cy - h / 2, w, h, body);
        drawRotatedRect(cx, cy - h / 2 - 4, w / 2, h / 2, cabin);
        drawRotatedRect(cx - 13, cy - h / 2 + 5, 5, h - 3, pal_[14]);
        drawRotatedRect(cx + 13, cy - h / 2 + 5, 5, h - 3, pal_[14]);
        drawRotatedRect(cx, cy - 2, w - 4, 2, pal_[11]);
        if (danni && danni->segni > 0) {
            // Ammaccature: restano per tutta la gara, una per botta (fino a
            // DANNI_MAX), in punti fissi del retro; ombra scura e graffio chiaro.
            static constexpr i32 SEGNI[DanniAuto::DANNI_MAX][2] = {
                {-12, -4}, {11, -6}, {-6, -1}, {7, -2}, {-15, -8}, {14, -3}, {-2, -5}, {3, -8}};
            for (i32 k = 0; k < std::min(danni->segni, DanniAuto::DANNI_MAX); ++k) {
                const i32 sx = cx + SEGNI[k][0], sy = cy + SEGNI[k][1];
                drawRotatedRect(sx, sy, 3, 2, 0xFF402020u);
                drawRotatedRect(sx + 1, sy - 1, 2, 1, 0xFFE0D0C0u);
            }
        }
        if (frena) {
            // Stop accesi: rossi e piu' grandi.
            drawRotatedRect(cx - 10, cy - h / 2 + 2, 5, 3, 0xFFFF2020u);
            drawRotatedRect(cx + 10, cy - h / 2 + 2, 5, 3, 0xFFFF2020u);
        } else if (speed > 0.0f && !offroad) {
            drawRotatedRect(cx - 10, cy - h / 2 + 2, 3, 2, 0xFFFFF4B0u);
            drawRotatedRect(cx + 10, cy - h / 2 + 2, 3, 2, 0xFFFFF4B0u);
        }
    }
    if (fumoS >= 0.0f) {
        // Fumo delle gomme nel primo secondo di frenata: uno sbuffo ogni 0,1 s
        // per ruota posteriore, sale, si allarga e si dirada (pixel a
        // scacchiera, niente trasparenze). AUTO1 A2: con i componenti di
        // auto_giocatore.json lo sbuffo e' il fotogramma fumo_1..3 sullo
        // scarico; senza componenti vale il disegno sintetico di oggi.
        constexpr f32 OGNI_S = 0.1f, VITA_S = 0.6f, FUMO_S = 1.0f;
        for (i32 n = 0; (f32)n * OGNI_S <= std::min(fumoS, FUMO_S); ++n) {
            const f32 eta = fumoS - (f32)n * OGNI_S;
            if (eta > VITA_S) continue;
            const f32 k = eta / VITA_S;
            const i32 r = 2 + (i32)(k * 6.0f);
            for (i32 lato = -1; lato <= 1; lato += 2) {
                // Punto di partenza: lo scarico della griglia (fotogrammi con
                // componenti) oppure la posizione di sempre (cx +/- 13).
                f32 baseX = (f32)cx + (f32)lato * 13.0f, baseY = (f32)cy - 1.0f;
                if (fotogrammaGriglia && pt) {
                    const f32* sc = lato < 0 ? pt->scaricoSx : pt->scaricoDx;
                    baseX = (f32)x0 + sc[0] / (f32)grigliaW_ * (f32)w;
                    baseY = (f32)y0 + sc[1] / (f32)grigliaH_ * (f32)h;
                }
                const i32 px = (i32)(baseX + (f32)lato * (k * 10.0f)) + ((n & 1) ? 2 : -2);
                const i32 py = (i32)(baseY - k * 12.0f);
                const Sprite& fs = fumoSprites_[(n + (lato > 0 ? 1 : 0)) % 3];
                // Solo sui fotogrammi della griglia degli agenti: le pose di
                // Andrea tengono il fumo di sempre (REGOLE_AUTO_GIOCATORE.md).
                if (fotogrammaGriglia && fs.width > 0 && fs.height > 0 && !fs.palette.empty()) {
                    // Fotogramma del @grafico, centrato sullo sbuffo, che si
                    // dirada come il disegno sintetico mentre l'eta' avanza.
                    const i32 hx = fs.width / 2, hy = fs.height / 2;
                    for (i32 yy = 0; yy < fs.height; ++yy)
                        for (i32 xx = 0; xx < fs.width; ++xx) {
                            if (k >= 0.5f && ((px + xx + py + yy) & 1) != 0) continue;
                            const u32 col = fs.palette[(u32)fs.pixels[yy * fs.width + xx] % fs.palette.size()];
                            if ((col >> 24) == 0) continue;
                            fb_.put(px - hx + xx, py - hy + yy, col);
                        }
                } else {
                    const u32 col = k < 0.4f ? 0xFFE8E8E8u : 0xFFB8B8C0u;
                    for (i32 dy = -r; dy <= r; ++dy)
                        for (i32 dx = -r; dx <= r; ++dx)
                            if (dx * dx + dy * dy <= r * r && (k < 0.5f || ((px + dx + py + dy) & 1) == 0))
                                fb_.put(px + dx, py + dy, col);
                }
            }
        }
    }
    if (!danni) return;
    if (danni->dallUrtoS >= 0.0f && danni->dallUrtoS < 0.4f) {
        // Scintille dal lato della botta (dietro/davanti: da entrambi i lati):
        // punti gialli e bianchi che volano via e cadono.
        const f32 t = danni->dallUrtoS;
        for (i32 n = 0; n < 8; ++n) {
            const i32 lato = danni->lato != 0 ? danni->lato : ((n & 1) ? 1 : -1);
            const f32 vx = 60.0f + 25.0f * (f32)(n % 4);
            const f32 vy = -50.0f - 18.0f * (f32)((n * 5) % 4);
            const i32 px = cx + lato * (17 + (i32)(vx * t));
            const i32 py = cy - h / 2 + (n % 3) * 3 + (i32)(vy * t + 220.0f * t * t);
            const u32 col = (n % 3 == 0) ? 0xFFFFFFFFu : 0xFFFFD040u;
            fb_.put(px, py, col);
            if (t < 0.2f) fb_.put(px - lato, py, 0xFFFF8020u); // scia
        }
    }
    if (danni->fumo) {
        // Fumo dal cofano dopo tante botte: sbuffi grigi che salgono e si
        // allargano, uno ogni 0,25 s (pixel a scacchiera, niente trasparenze).
        constexpr f32 OGNI_S = 0.25f, VITA_S = 1.0f;
        const f32 fase = std::fmod(danni->tempoS, OGNI_S);
        for (i32 n = 0; n < 4; ++n) {
            const f32 eta = fase + (f32)n * OGNI_S;
            if (eta > VITA_S) continue;
            const f32 k = eta / VITA_S;
            const i32 r = 2 + (i32)(k * 4.0f);
            const i32 px = cx + ((n & 1) ? 3 : -3) + (i32)(k * 4.0f);
            const i32 py = cy - h - 2 - (i32)(k * 20.0f);
            const u32 col = k < 0.4f ? 0xFFE0E0E8u : 0xFF404048u; // chiaro sul nero, scuro sull'asfalto
            for (i32 dy = -r; dy <= r; ++dy)
                for (i32 dx = -r; dx <= r; ++dx)
                    if (dx * dx + dy * dy <= r * r && (k < 0.6f || ((px + dx + py + dy) & 1) == 0))
                        fb_.put(px + dx, py + dy, col);
        }
    }
}

void Renderer::drawHud(f32 speedPct, i32 gear, bool offroad) {
    // Pannello info in basso a sinistra: barra velocita, marcia, avviso offroad.
    const i32 x0 = 8, y0 = 204, wBarMax = 120, hBar = 6;
    f32 pct = speedPct < 0.0f ? 0.0f : (speedPct > 1.0f ? 1.0f : speedPct);
    // Sfondo e bordo del pannello.
    fb_.fillRect(x0 - 2, y0 - 2, x0 + wBarMax + 36, y0 + hBar + 16, pal_[4]);
    fb_.fillRect(x0 - 1, y0 - 1, x0 + wBarMax + 35, y0 + hBar + 15, pal_[7]);
    // Barra velocita: piena a 100%, gialla oltre l'85%.
    if (pct > 0.0f)
        fb_.fillRect(x0, y0, x0 + (i32)(pct * (f32)wBarMax) - 1, y0 + hBar - 1,
                     pct > 0.85f ? pal_[11] : pal_[9]);
    // Tacche 33% e 66%.
    fb_.fillRect(x0 + wBarMax / 3, y0 - 2, x0 + wBarMax / 3, y0 + hBar + 1, pal_[15]);
    fb_.fillRect(x0 + 2 * wBarMax / 3, y0 - 2, x0 + 2 * wBarMax / 3, y0 + hBar + 1, pal_[15]);
    // Marcia: due quadratini, quelli raggiunti verdi.
    const i32 gx = x0 + wBarMax + 8;
    for (i32 g = 1; g <= 2; ++g) {
        const u32 col = (gear >= g) ? pal_[13] : pal_[15];
        fb_.fillRect(gx + (g - 1) * 10, y0, gx + (g - 1) * 10 + 7, y0 + 7, col);
    }
    // Avviso fuoristrada: rosso fisso quando offroad.
    if (offroad)
        fb_.fillRect(gx + 20, y0, gx + 27, y0 + 7, pal_[12]);
}

void Renderer::drawGara(i32 posizione, i32 concorrenti, i32 tappa, bool arrivo, u64 frame) {
    // In alto a sinistra: concorrenti in fila, il primo a sinistra; il
    // giocatore e' il quadratino giallo. Sotto: una tacca per tappa raggiunta.
    const i32 x0 = 8, y0 = 8, lato = 6, passo = 9;
    fb_.fillRect(x0 - 2, y0 - 2, x0 + concorrenti * passo, y0 + lato + 8, pal_[4]);
    for (i32 i = 0; i < concorrenti; ++i) {
        const u32 col = (i + 1 == posizione) ? pal_[11] : pal_[15];
        fb_.fillRect(x0 + i * passo, y0, x0 + i * passo + lato - 1, y0 + lato - 1, col);
    }
    for (i32 t = 0; t < tappa; ++t)
        fb_.fillRect(x0 + t * passo, y0 + lato + 3, x0 + t * passo + lato - 1, y0 + lato + 5, pal_[10]);
    if (arrivo && (frame / 25) % 2 == 0) {
        // Bandiera a scacchi in alto al centro.
        for (i32 r = 0; r < 4; ++r)
            for (i32 c = 0; c < 16; ++c)
                fb_.fillRect(96 + c * 8, 24 + r * 8, 103 + c * 8, 31 + r * 8, ((r + c) & 1) ? pal_[0] : pal_[1]);
    }
}

i32 Renderer::larghezzaTesto(const char* testo, i32 scala) {
    i32 n = 0;
    while (testo[n]) ++n;
    return scala <= 0 ? n * FONT_MINI_W - 1 : n * FONT_HUD_W * scala;
}

void Renderer::drawTesto(i32 x, i32 y, const char* testo, u32 colore, i32 scala, bool ombra) {
    if (scala <= 0) {
        // Font mini 3x5 per le etichette, stessa ombra di 1 px.
        for (i32 passo = ombra ? 0 : 1; passo < 2; ++passo) {
            const i32 d = passo == 0 ? 1 : 0;
            const u32 col = passo == 0 ? 0xFF000000u : colore;
            i32 cx = x;
            for (const char* c = testo; *c; ++c, cx += FONT_MINI_W) {
                const u8* righe = fontMini(*c);
                for (i32 r = 0; r < FONT_MINI_H; ++r)
                    for (i32 k = 0; k < 3; ++k)
                        if (righe[r] & (4u >> k)) fb_.put(cx + k + d, y + r + d, col);
            }
        }
        return;
    }
    // Font pixel 5x7 della variante B (approvato): una cella 6x8 per carattere.
    for (i32 passo = ombra ? 0 : 1; passo < 2; ++passo) {
        const i32 d = passo == 0 ? 1 : 0; // prima l'ombra, spostata di 1 px
        const u32 col = passo == 0 ? 0xFF000000u : colore;
        i32 cx = x;
        for (const char* c = testo; *c; ++c, cx += FONT_HUD_W * scala) {
            const i32 codice = (u8)*c;
            if (codice < 32 || codice > 126) continue;
            const u8* righe = FONT_HUD[codice - 32];
            for (i32 r = 0; r < FONT_HUD_H; ++r)
                for (i32 k = 0; k < FONT_HUD_W; ++k)
                    if (righe[r] & (1u << (FONT_HUD_W - 1 - k)))
                        fb_.fillRect(cx + k * scala + d, y + r * scala + d,
                                     cx + (k + 1) * scala - 1 + d, y + (r + 1) * scala - 1 + d, col);
        }
    }
}

void Renderer::drawHudDati(const HudDati& d) {
    // Striscia compatta in alto da hud.json (Andrea 07/11): velocita' a
    // sinistra, radio al centro (Game::drawMusica), tempo e gara a destra.
    // Niente box ne' cornici: solo testo con ombra di 1 px, la scena resta libera.
    char buf[32];
    for (const HudElemento& e : hud_) {
        if (e.tipo == "barra") {
            // Riga sottile in fondo al gruppo, senza cornice: tacche da 3 px con
            // 1 px d'aria (contagiri) o un segmento per tappa (tappe); spente a
            // puntini, accese piene. Contagiri: le ultime tacche nel colore del limite.
            const bool tappe = e.id == "tappe";
            if (tappe && !d.gara) continue;
            const i32 n = tappe ? std::max(1, d.tappe) : std::max(1, (e.w + 1) / 4);
            const i32 passo = (e.w + 1) / n, largo = passo - 1;
            f32 pieno = tappe ? 0.0f : d.giri;
            pieno = pieno < 0.0f ? 0.0f : (pieno > 1.0f ? 1.0f : pieno);
            const i32 accese = tappe ? d.tappa : (i32)(pieno * (f32)n + 0.5f);
            const i32 limite = tappe ? n : n - std::max(1, n / 6);
            for (i32 i = 0; i < n; ++i) {
                const i32 x = e.x + i * passo;
                const u32 col = i >= limite ? e.coloreBordo : e.coloreRiempimento;
                // Tappa in corso: lampeggia piano tra pieno e puntini.
                const bool corrente = tappe && i == d.tappa - 1 && (d.frame / 30) % 2;
                if (i < accese && !corrente) {
                    fb_.fillRect(x + 1, e.y + 1, x + largo, e.y + e.h, 0xFF000000u); // ombra
                    fb_.fillRect(x, e.y, x + largo - 1, e.y + e.h - 1, col);
                } else {
                    for (i32 y = e.y; y < e.y + e.h; ++y)
                        for (i32 k = 0; k < largo; ++k)
                            if (((x + k + y) & 1) == 0) fb_.put(x + k, y, i >= limite ? e.coloreBordo : e.coloreLabel);
                }
            }
            continue;
        }
        // testo: valore vivo per id; gli elementi di gara spariscono fuori gara.
        const bool soloGara = e.id == "tempo" || e.id == "totale" || e.id == "tappa" || e.id == "posizione";
        if (e.id == "show" && !d.show) continue; // SHOW ON solo quando lo show e' in scena
        if (soloGara && !d.gara) continue;
        const char* valore = nullptr;
        u32 coloreValore = e.colore;
        if (e.id == "distanza") { std::snprintf(buf, sizeof(buf), "%dM", (i32)(d.km * 1000.0f)); valore = buf; }
        else if (e.id == "velocita") { std::snprintf(buf, sizeof(buf), "%3d", d.kmh); valore = buf; }
        else if (e.id == "tempo") {
            std::snprintf(buf, sizeof(buf), "%2d", d.tempoS);
            valore = buf;
            if (d.tempoS <= 10 && (d.frame / 12) % 2) coloreValore = e.coloreLabel; // lampeggia negli ultimi 10 s
        }
        else if (e.id == "totale") {
            const i32 cs = (i32)(d.totaleS * 100.0f);
            std::snprintf(buf, sizeof(buf), "%d'%02d\"%02d", cs / 6000, (cs / 100) % 60, cs % 100);
            valore = buf;
        }
        else if (e.id == "tappa") { std::snprintf(buf, sizeof(buf), "%d/%d", d.tappa, d.tappe); valore = buf; }
        else if (e.id == "posizione") { std::snprintf(buf, sizeof(buf), "%d/%d", d.posizione, d.concorrenti); valore = buf; }
        // allinea "destra": label_pos e valore_pos sono il bordo destro del testo.
        const auto xa = [&e](i32 x, const char* t, i32 scala) { return e.destra ? x - larghezzaTesto(t, scala) : x; };
        if (e.lx >= 0) drawTesto(xa(e.lx, e.label.c_str(), e.scalaLabel), e.ly, e.label.c_str(), e.coloreLabel, e.scalaLabel);
        if (e.id == "posizione" && d.sorpasso > 0.0f && valore && e.vx >= 0) {
            // Sorpasso (Andrea 29/10): la posizione fa un saltello, lampeggia
            // giallo/bianco con una freccia in su accanto, poi torna normale.
            const f32 t = 1.0f - d.sorpasso; // 0 all'inizio del sorpasso
            const i32 su = t < 0.2f ? (i32)(3.0f * std::sin(t / 0.2f * 3.14159f) + 0.5f) : 0;
            const u32 col = (d.frame / 4) % 2 ? 0xFFFFE040u : 0xFFFFFFFFu;
            const i32 vx = xa(e.vx, valore, e.scalaValore);
            drawTesto(vx, e.vy - su, valore, col, e.scalaValore);
            // Freccia verde accanto, verso l'interno dello schermo.
            const i32 fx = e.destra ? vx - 8 : vx + larghezzaTesto(valore, e.scalaValore) + 3;
            const i32 fy = e.vy - su + 1;
            for (i32 r = 0; r <= 2; ++r) fb_.hline(fx + 2 - r, fx + 2 + r, fy + r, 0xFF40E060u);
            fb_.fillRect(fx + 1, fy + 3, fx + 3, fy + 5, 0xFF40E060u);
            continue;
        }
        if (valore && e.vx >= 0) drawTesto(xa(e.vx, valore, e.scalaValore), e.vy, valore, coloreValore, e.scalaValore);
        if (e.tx >= 0) {
            const char* t = e.id == "show" ? "SHOW ON" : e.testo.c_str();
            drawTesto(xa(e.tx, t, e.scala), e.ty, t, e.colore, e.scala);
        }
    }

    // Messaggi al centro dello schermo.
    auto centro = [this](i32 y, const char* t, i32 scala) {
        drawTesto((Framebuffer::WIDTH - larghezzaTesto(t, scala)) / 2, y, t, 0xFFFFFFFFu, scala);
    };
    if (d.tempoScaduto) centro(96, "TIME UP", 2);
    else if (d.arrivo) {
        centro(70, "GOAL", 3);
        std::snprintf(buf, sizeof(buf), "POS %d/%d", d.posizione, d.concorrenti);
        centro(100, buf, 1);
    } else if (d.tempoExtra > 0) {
        if ((d.frame / 8) % 2 == 0) centro(70, "TIME EXTRA", 2);
        std::snprintf(buf, sizeof(buf), "+%d", d.tempoExtra);
        centro(92, buf, 2);
    } else if (d.bivio && (d.frame / 15) % 2 == 0) {
        drawTesto(16, 112, "<< LEFT", 0xFFFFFFFFu, 1);
        drawTesto(Framebuffer::WIDTH - 16 - larghezzaTesto("RIGHT >>"), 112, "RIGHT >>", 0xFFFFFFFFu, 1);
    }
    // Fuori strada: nessuna scritta (Andrea 29/10, ridondante): lo dicono gia'
    // l'erba, il sobbalzo e le scintille.
}

} // namespace sgl

