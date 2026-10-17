// SPDX-License-Identifier: MIT
#include "render/renderer.h"
#include <algorithm>
#include <cmath> // std::fmod per il wrap di zRel
#include <utility> // std::move

namespace sgl {

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

void Renderer::drawRoad(const Road& road, f32 playerX, f32 playerZ) {
    const auto& segs = road.segments();
    if (segs.empty()) return;

    projs_.clear();
    road.project(playerX, playerZ, projs_);
    if (projs_.empty()) return;

    // playerX del frame: drawSprites riusa la stessa proiezione della strada.
    lastPlayerX_ = playerX; // (riservato; drawSprites usa il centro strada proiettato)

    // Cielo (parte superiore dello schermo) e terra (sotto l'orizzonte).
    fb_.fillRect(0, 0, Framebuffer::WIDTH - 1, HORIZON_Y - 1, pal_[7]);   // cielo notturno
    fb_.fillRect(0, HORIZON_Y, Framebuffer::WIDTH - 1, Framebuffer::HEIGHT - 1, pal_[8]); // erba

    // Disegna i segmenti dal piu' LONTANO al piu' VICINO: projs_ e' ordinato
    // vicino->lontano (i=0 e' il segmento piu' vicino), quindi si itera a ritroso.
    // Il vicino, disegnato per ultimo, copre correttamente il lontano (painter).
    for (size_t i = projs_.size() - 1; i > 0; --i) {
        const auto& vicino = projs_[i - 1];
        const auto& lontano = projs_[i];
        // Salta se il segmento vicino non e' sotto il lontano (degenero/contigui).
        if (lontano.screenY >= vicino.screenY) continue;
        // Bande alternate ancorate al MONDO (indice segmento), non allo schermo: asfalto,
        // erba, rumble e tratteggio di corsia cambiano insieme e scorrono allineati.
        drawSegmentTrapezoid(vicino, lontano, ((vicino.index / BAND_SEGS) & 1) == 0);
    }
}

void Renderer::drawSegmentTrapezoid(const Road::ProjectedSeg& cur,
                                    const Road::ProjectedSeg& next,
                                    bool band) {
    // cur = bordo VICINO (y maggiore), next = bordo LONTANO (y minore).
    // Riferimento tecnico: bande alternate di colore per segmento come nei racer
    // pseudo-3D (Lou's Pseudo 3d Page, Cannonball come riferimento di resa) — solo idea.
    const i32 cy = (i32)cur.screenY;
    const i32 ny = (i32)next.screenY;
    const i32 dy = cy - ny;
    if (dy <= 0) return;

    // Mezza larghezza strada in px, coerente con Road::project (W/2 * scala * ROAD_HALF_WORLD).
    const f32 hwNear = cur.scale  * Road::ROAD_HALF_WORLD * (Framebuffer::WIDTH / 2.0f);
    const f32 hwFar  = next.scale * Road::ROAD_HALF_WORLD * (Framebuffer::WIDTH / 2.0f);

    const u32 grass  = band ? pal_[8] : GRASS_ALT;
    const u32 road   = band ? pal_[2] : pal_[3];
    const u32 rumble = band ? pal_[5] : pal_[6]; // bianco / magenta neon
    const u32 lane   = pal_[1];

    for (i32 y = ny; y <= cy; ++y) {
        // t = 0 sul bordo lontano (y = ny), 1 sul bordo vicino (y = cy).
        const f32 t  = (f32)(y - ny) / (f32)dy;
        const f32 cx = next.screenX + (cur.screenX - next.screenX) * t;
        const f32 hw = hwFar + (hwNear - hwFar) * t;
        const i32 lx = (i32)(cx - hw);
        const i32 rx = (i32)(cx + hw);
        // Rumble: larghezza proporzionale alla larghezza locale, quindi continua
        // fra segmenti contigui (nessun gradino ai confini).
        i32 rw = (i32)(hw * 0.14f);
        if (rw < 1) rw = 1;

        if (y >= HORIZON_Y) { // sopra l'orizzonte resta il cielo (le colline non cambiano il fondale)
            fb_.hline(0, lx - 1, y, grass);
            fb_.hline(rx + 1, Framebuffer::WIDTH - 1, y, grass);
        }
        fb_.hline(lx, rx, y, road);
        fb_.hline(lx, lx + rw - 1, y, rumble);
        fb_.hline(rx - rw + 1, rx, y, rumble);
        if (band) { // tratteggio di corsia solo sulle bande "chiare", centrato sulla strada
            i32 lw = (i32)(hw * 0.02f);
            if (lw < 1) lw = 1;
            fb_.hline((i32)cx - lw, (i32)cx + lw, y, lane);
        }
    }
}

void Renderer::drawSprites(std::vector<SpriteInstance>& sprites, f32 playerZ, f32 totalLen) {
    if (projs_.empty()) return; // drawRoad deve girare prima (stessa proiezione)
    // Ordina per distanza decrescente (dietro prima).
    std::sort(sprites.begin(), sprites.end(),
              [](const SpriteInstance& a, const SpriteInstance& b) {
                  return a.worldZ > b.worldZ;
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
        if (zRel <= 0.0f) continue; // dietro la telecamera
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
        drawSprite(s, sx, sy, scl);
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
    for (i32 sy = 0; sy < h; ++sy) {
        i32 ty = (i32)((f32)sy / (f32)h * (f32)s.sprite->height);
        for (i32 sx = 0; sx < w; ++sx) {
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

void Renderer::drawPlayerCar(f32 speed, f32 offsetX, bool offroad) {
    // Auto del giocatore: sagoma sintetica (nessun asset Sega).
    // Il corpo e' un rettangolo centrale, l'abitacolo una striscia diversa,
    // l'ombra sotto. L'auto resta CENTRATA sullo schermo (cx=160): e' la
    // strada che si sposta sotto di lei tramite camX=playerX in road.project(),
    // come in OutRun. offsetX e' usato solo per un leggero accenno di
    // sterzo visibile (inclinazione del corpo) senza spostare l'auto.
    u32 body = offroad ? pal_[12] : pal_[9];
    u32 cabin = pal_[10];
    // Leggero spostamento visibile in proporzione allo sterzo (max ~12 px),
    // molto meno del vecchio offsetX*0.15f (che arrivava a 135 px e faceva
    // uscire l'auto dalla carreggiata quando la strada si spostava).
    i32 cx = 160 + (i32)(offsetX * 0.013f);
    i32 cy = 200;
    i32 w = 34, h = 18;
    fb_.fillRect(cx - w/2, cy - h, cx + w/2, cy, body);
    fb_.fillRect(cx - w/4, cy - h + 2, cx + w/4, cy - 6, cabin);
    // ombra
    fb_.fillRect(cx - w/2 - 2, cy + 1, cx + w/2 + 2, cy + 3, pal_[14]);
    (void)speed; // riservato per animazione sospensione / salto
}

} // namespace sgl
