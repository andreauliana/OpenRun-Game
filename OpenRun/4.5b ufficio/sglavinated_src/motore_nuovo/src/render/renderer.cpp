// SPDX-License-Identifier: MIT
#include "render/renderer.h"
#include "render/font_hud.h"
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
    const i32 dy = cy - ny;
    if (dy <= 0) return;

    // Mezza larghezza strada in px, coerente con Road::project (W/2 * scala * ROAD_HALF_WORLD).
    // Al bivio ci sono due rami: stesso disegno, centro spostato e larghezza ridotta.
    const f32 pxNear = cur.scale  * (Framebuffer::WIDTH / 2.0f);
    const f32 pxFar  = next.scale * (Framebuffer::WIDTH / 2.0f);
    const f32 hwNear = pxNear * Road::ROAD_HALF_WORLD * cur.carr.fattore;
    const f32 hwFar  = pxFar  * Road::ROAD_HALF_WORLD * next.carr.fattore;
    const i32 rami = (cur.carr.doppia || next.carr.doppia) ? 2 : 1;

    const u32 grass  = band ? tema_.erba : tema_.erbaAlt;
    const u32 road   = band ? pal_[2] : pal_[3];
    const u32 rumble = band ? pal_[5] : tema_.rumbleAlt; // bianco / colore della tappa
    const u32 lane   = pal_[1];

    for (i32 y = ny; y <= cy; ++y) {
        // t = 0 sul bordo lontano (y = ny), 1 sul bordo vicino (y = cy).
        const f32 t  = (f32)(y - ny) / (f32)dy;
        const f32 hw = hwFar + (hwNear - hwFar) * t;
        if (y >= HORIZON_Y) // sopra l'orizzonte resta il cielo (le colline non cambiano il fondale)
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
            continue;
        }
        // Scala in pixel: stessa proiezione della strada (scl * W/2 per unita' mondo)
        // per le unita' mondo di un pixel sprite. Prima si passava scl da solo
        // (~0.001 a distanza): w e h valevano 0 e nessuno sprite compariva mai.
        drawSprite(s, sx, sy, scl * (f32)Framebuffer::WIDTH / 2 * SPRITE_WORLD_PX * s.scale);
    }
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
    for (i32 sy = syIni; sy < syFin; ++sy) {
        i32 ty = (i32)((f32)sy / (f32)h * (f32)s.sprite->height);
        for (i32 sx = sxIni; sx < sxFin; ++sx) {
            i32 tx = (i32)((f32)sx / (f32)w * (f32)s.sprite->width);
            u8 idx = s.sprite->pixels[ty * s.sprite->width + tx];
            u32 col;
            if (palettePropria) {
                // Palette RGBA di proprietà dello sprite (colori dei cartelloni sponsor).
                col = s.sprite->palette[idx % nPalette];
            } else {
                // Palette del motore: indice 0 = trasparente.
                if (idx == 0) continue;
                col = pal_[s.paletteBase + idx];
            }
            if ((col >> 24) == 0) continue; // trasparente (canale alpha nullo)
            fb_.put(x0 + sx, y0 + sy, col);
        }
    }
}

void Renderer::drawPlayerCar(f32 speed, f32 offsetX, bool offroad, f32 rollAngle, f32 sollevamentoPx) {
    // Auto del giocatore: sagoma sintetica (nessun asset Sega).
    // Il corpo e' un rettangolo centrale, l'abitacolo una striscia diversa,
    // l'ombra sotto. L'auto resta CENTRATA sullo schermo (cx=160): e' la
    // strada che si sposta sotto di lei tramite camX=playerX in road.project(),
    // come in OutRun. offsetX e' usato solo per un leggero accenno di
    // sterzo visibile (inclinazione del corpo) senza spostare l'auto.
    u32 body = offroad ? pal_[12] : pal_[9];
    u32 cabin = pal_[10];
    // La carrozzeria segue visibilmente la linea scelta, restando nel framebuffer.
    i32 cx = 160 + (i32)(offsetX * 0.035f);
    // Sollevamento alle velocita' piu' alte: l'ombra resta a terra, la
    // carrozzeria sale di sollevamentoPx (moduli_fx.sollevamento_auto_alta_velocita).
    const i32 sollevamento = (i32)(sollevamentoPx + 0.5f);
    if (sollevamento > 0) fb_.fillRect(cx - 15, 199, cx + 15, 201, pal_[0]);
    i32 cy = 200 - sollevamento;
    i32 w = 34, h = 18;
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
    if (speed > 0.0f && !offroad) {
        drawRotatedRect(cx - 10, cy - h / 2 + 2, 3, 2, 0xFFFFF4B0u);
        drawRotatedRect(cx + 10, cy - h / 2 + 2, 3, 2, 0xFFFFF4B0u);
    }
    (void)speed; // riservato per animazione sospensione / salto
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
    return n * FONT_HUD_W * scala;
}

void Renderer::drawTesto(i32 x, i32 y, const char* testo, u32 colore, i32 scala, bool ombra) {
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

namespace {
void cornicePiena(Framebuffer& fb, i32 x, i32 y, i32 w, i32 h, i32 sp, u32 col) {
    fb.fillRect(x, y, x + w - 1, y + sp - 1, col);
    fb.fillRect(x, y + h - sp, x + w - 1, y + h - 1, col);
    fb.fillRect(x, y, x + sp - 1, y + h - 1, col);
    fb.fillRect(x + w - sp, y, x + w - 1, y + h - 1, col);
}
} // namespace

void Renderer::drawHudDati(const HudDati& d) {
    // Disposizione da hud.json (riferimento strutturale all'originale: distanza
    // in alto a sinistra, velocita' e tempo in alto a destra). Tema bianco e nero.
    char buf[32];
    for (const HudElemento& e : hud_) {
        if (e.tipo == "cornice") {
            cornicePiena(fb_, e.x + 1, e.y + 1, e.w, e.h, e.spessore, 0xFF000000u); // ombra
            cornicePiena(fb_, e.x, e.y, e.w, e.h, e.spessore, e.colore);
            continue;
        }
        if (e.tipo == "barra") {
            f32 pieno = e.id == "giri" ? d.giri : 0.0f;
            pieno = pieno < 0.0f ? 0.0f : (pieno > 1.0f ? 1.0f : pieno);
            if (e.lx >= 0) drawTesto(e.lx, e.ly, e.label.c_str(), e.coloreLabel);
            cornicePiena(fb_, e.x, e.y, e.w, e.h, e.spessore, e.coloreBordo);
            const i32 interno = e.w - 2 * e.spessore;
            const i32 px = (i32)(pieno * (f32)interno);
            fb_.fillRect(e.x + e.spessore, e.y + e.spessore, e.x + e.w - e.spessore - 1, e.y + e.h - e.spessore - 1, 0xFF000000u);
            if (px > 0)
                fb_.fillRect(e.x + e.spessore, e.y + e.spessore, e.x + e.spessore + px - 1, e.y + e.h - e.spessore - 1,
                             pieno > 0.9f ? e.coloreBordo : e.coloreRiempimento);
            continue;
        }
        // testo: valore vivo per id; gli elementi di gara spariscono fuori gara.
        const bool soloGara = e.id == "tempo" || e.id == "tappa" || e.id == "posizione";
        if (soloGara && !d.gara) continue;
        const char* valore = nullptr;
        u32 coloreValore = e.colore;
        if (e.id == "distanza") { std::snprintf(buf, sizeof(buf), "%05.1f", d.km); valore = buf; }
        else if (e.id == "velocita") { std::snprintf(buf, sizeof(buf), "%3d", d.kmh); valore = buf; }
        else if (e.id == "tempo") {
            std::snprintf(buf, sizeof(buf), "%2d", d.tempoS);
            valore = buf;
            if (d.tempoS <= 10 && (d.frame / 12) % 2) coloreValore = e.coloreLabel; // lampeggia negli ultimi 10 s
        }
        else if (e.id == "tappa") { std::snprintf(buf, sizeof(buf), "%d/%d", d.tappa, d.tappe); valore = buf; }
        else if (e.id == "posizione") { std::snprintf(buf, sizeof(buf), "%d/%d", d.posizione, d.concorrenti); valore = buf; }
        else if (e.id == "marcia") valore = d.marciaAlta ? "HIGH" : "LOW";
        if (e.lx >= 0) drawTesto(e.lx, e.ly, e.label.c_str(), e.coloreLabel);
        if (valore && e.vx >= 0) drawTesto(e.vx, e.vy, valore, coloreValore, e.scalaValore);
        if (e.tx >= 0) {
            const char* t = e.id == "show" ? (d.show ? "SHOW ON" : "SHOW OFF") : e.testo.c_str();
            drawTesto(e.tx, e.ty, t, e.colore, e.scala);
        }
    }

    // Messaggi al centro dello schermo.
    auto centro = [this](i32 y, const char* t, i32 scala) {
        drawTesto((Framebuffer::WIDTH - larghezzaTesto(t, scala)) / 2, y, t, 0xFFFFFFFFu, scala);
    };
    if (d.tempoScaduto) centro(96, "TEMPO SCADUTO", 2);
    else if (d.arrivo) {
        centro(70, "ARRIVO", 3);
        std::snprintf(buf, sizeof(buf), "POSIZIONE %d/%d", d.posizione, d.concorrenti);
        centro(100, buf, 1);
    } else if (d.bivio && (d.frame / 15) % 2 == 0) {
        drawTesto(16, 112, "<< SINISTRA", 0xFFFFFFFFu, 1);
        drawTesto(Framebuffer::WIDTH - 16 - larghezzaTesto("DESTRA >>"), 112, "DESTRA >>", 0xFFFFFFFFu, 1);
    } else if (d.fuoristrada && (d.frame / 10) % 2 == 0) {
        centro(150, "FUORI STRADA", 1);
    }
}

} // namespace sgl

