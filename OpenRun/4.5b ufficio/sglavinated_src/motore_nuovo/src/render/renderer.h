// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "render/framebuffer.h"
#include "render/palette.h"
#include "render/sprite.h" // Sprite (usato da SpriteInstance e dagli sprite sponsor)
#include "road/road.h"
#include <vector>

namespace sgl {

struct SpriteInstance {
    f32 worldX = 0.0f; // posizione laterale rispetto alla strada
    f32 worldZ = 0.0f; // posizione assoluta sulla strada
    f32 scale = 1.0f;
    const Sprite* sprite = nullptr;
    i32 paletteBase = 0; // offset di palette (per variazioni colore auto)
};

// Colori di ambiente della tappa (da livello.json "colori"); i default sono
// quelli storici della palette del motore.
struct TemaColori {
    u32 cielo = 0xFF202040;
    u32 erba = 0xFF00AA00;
    u32 erbaAlt = 0xFF008C00;
    u32 rumbleAlt = 0xFFFF00FF;
};

class Renderer {
public:
    // Unita' mondo coperte da un pixel sprite: un cartellone da 128 px e' largo
    // ~640 unita' (la carreggiata e' larga 2400).
    static constexpr f32 SPRITE_WORLD_PX = 5.0f;
    Renderer(Framebuffer& fb, const Palette& pal);

    // Disegna la strada (segmenti proiettati) con rumble laterali e fondo cielo.
    // latoBivio: ramo scelto al bivio di fine tappa (0 = ancora da scegliere).
    void drawRoad(const Road& road, f32 playerX, f32 playerZ, i32 latoBivio = 0);
    void impostaTema(const TemaColori& t) { tema_ = t; }
    const TemaColori& tema() const { return tema_; }

    // Disegna gli sprite ordinati per distanza (piu' lontani prima).
    // totalLen: lunghezza totale del tracciato (per il wrap di zRel quando
    // playerZ avvolge sul giro; gli sprite usano worldZ assoluto).
    void drawSprites(std::vector<SpriteInstance>& sprites, f32 playerZ, f32 totalLen);

    // Disegna l'auto del giocatore al centro dello schermo (proporzionale alla velocita').
    void drawPlayerCar(f32 speed, f32 offsetX, bool offroad, f32 rollAngle = 0.0f);

    // Pannello di info (tappa 1): velocita (barra 0..1), marcia (quadrati),
    // fuoristrada (rosso). Nessun font: solo barre della palette HUD.
    void drawHud(f32 speedPct, i32 gear, bool offroad);

    // Gara: posizione (un quadratino per concorrente, il giocatore evidenziato),
    // tappa raggiunta e, all'arrivo, bandiera a scacchi lampeggiante.
    void drawGara(i32 posizione, i32 concorrenti, i32 tappa, bool arrivo, u64 frame);

    Framebuffer& fb() { return fb_; }
    const Palette& pal() const { return pal_; }

    // Aggiunge uno sprite sponsor caricato una volta all'avvio (dal runner,
    // che decodifica i PNG; il core resta senza SDL). Ritorna l'indice da
    // assegnare a SpriteInstance::sprite per disegnarlo come oggetto bordo strada.
    i32 aggiungiSponsorSprite(Sprite sprite);
    const Sprite* sponsorSprite(i32 indice) const {
        return indice >= 0 && indice < (i32)sponsorSprites_.size() ? &sponsorSprites_[indice] : nullptr;
    }

private:
    Framebuffer& fb_;
    const Palette& pal_;

    // Bande alternate (asfalto, erba, rumble) ancorate al segmento del mondo.
    static constexpr i32 BAND_SEGS = 2;
    static constexpr i32 HORIZON_Y = 112;       // riga dell'orizzonte: cielo sopra, terra sotto
    void drawSegmentTrapezoid(const Road::ProjectedSeg& cur, const Road::ProjectedSeg& next, bool band);
    void drawRoadsideMarkers(f32 playerZ);
    void drawSprite(const SpriteInstance& s, f32 screenX, f32 screenY, f32 scale);

    std::vector<Road::ProjectedSeg> projs_; // riutilizzato frame su frame (zero alloc a regime)
    std::vector<Sprite> sponsorSprites_;    // cartelloni 128x64, caricati una volta (zero alloc in gioco)
    f32 lastPlayerX_ = 0.0f;                // playerX dell'ultima drawRoad (per drawSprites)
    TemaColori tema_;                       // colori di ambiente della tappa
};

} // namespace sgl
