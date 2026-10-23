// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "render/framebuffer.h"
#include "render/palette.h"
#include "render/sprite.h" // Sprite (usato da SpriteInstance e dagli sprite sponsor)
#include "road/road.h"
#include <string>
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

// Un elemento dell'HUD da dati/ui/hud.json (variante B, bianco e nero).
// tipo: "cornice" | "testo" | "barra"; l'id lega l'elemento a un dato vivo.
struct HudElemento {
    std::string id, tipo, label, testo;
    i32 x = 0, y = 0, w = 0, h = 0, spessore = 2;   // cornice / barra (barra_pos, barra_w, barra_h)
    i32 lx = -1, ly = -1, vx = -1, vy = -1, tx = -1, ty = -1; // label_pos, valore_pos, pos
    i32 scalaValore = 1, scala = 1;
    u32 colore = 0xFFFFFFFF, coloreLabel = 0xFFA0A0A0, coloreBordo = 0xFFFFFFFF, coloreRiempimento = 0xFF787878;
};

// Dati vivi dell'HUD per il frame (nessuna allocazione: solo numeri).
struct HudDati {
    i32 kmh = 0;          // velocita' mostrata
    f32 km = 0.0f;        // distanza percorsa
    f32 giri = 0.0f;      // 0..1 contagiri
    bool gara = false;    // tempo, tappa e posizione solo in gara
    i32 tempoS = 0;       // secondi rimasti
    i32 tappa = 1, tappe = 3, posizione = 0, concorrenti = 0;
    bool marciaAlta = false;
    bool show = false;
    bool fuoristrada = false;
    i32 bivio = 0;        // 1 = bivio in vista e scelta aperta: frecce SX / DX
    bool arrivo = false, tempoScaduto = false;
    u64 frame = 0;        // per i lampeggi
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
    void impostaTema(const TemaColori& t) { tema_ = t; temaA_ = t; transizioneT_ = 1.0f; }
    const TemaColori& tema() const { return tema_; }
    // Avvia la transizione graduale del tema: da `da` (tema della tappa che
    // finisce) al tema attuale (impostato con impostaTema, tema della tappa
    // che comincia) in durataSecondi secondi (FORK1 punto 6). Durante la
    // transizione drawRoad disegna con i colori interpolati canale per canale.
    void iniziaTransizioneTema(const TemaColori& da, f32 durataSecondi);
    // Avanza la transizione di dt secondi (Game::update, frequenza indipendente).
    // Ritorna true mentre la transizione e' in corso.
    bool aggiornaTransizioneTema(f32 dt);
    // Interpolazione lineare di due colori 0xAARRGGBB, canale per canale.
    static u32 lerpColore(u32 da, u32 a, f32 t);

    // Disegna gli sprite ordinati per distanza (piu' lontani prima).
    // totalLen: lunghezza totale del tracciato (per il wrap di zRel quando
    // playerZ avvolge sul giro; gli sprite usano worldZ assoluto).
    void drawSprites(std::vector<SpriteInstance>& sprites, f32 playerZ, f32 totalLen);

    // Disegna l'auto del giocatore al centro dello schermo (proporzionale alla velocita').
    void drawPlayerCar(f32 speed, f32 offsetX, bool offroad, f32 rollAngle = 0.0f, f32 sollevamentoPx = 0.0f);

    // Pannello di info (tappa 1): velocita (barra 0..1), marcia (quadrati),
    // fuoristrada (rosso). Nessun font: solo barre della palette HUD.
    void drawHud(f32 speedPct, i32 gear, bool offroad);

    // HUD da hud.json: se impostato, sostituisce drawHud/drawGara.
    void impostaHud(std::vector<HudElemento> elementi) { hud_ = std::move(elementi); }
    bool haHud() const { return !hud_.empty(); }
    void drawHudDati(const HudDati& d);
    // Testo col font pixel dell'HUD (font_hud.h); scala intera, ombra nera di 1 px.
    void drawTesto(i32 x, i32 y, const char* testo, u32 colore, i32 scala = 1, bool ombra = true);
    static i32 larghezzaTesto(const char* testo, i32 scala = 1);

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
    // Resetta i cartelloni sponsor: va invocato prima di una nuova registrazione
    // (es. test ripetuti nello stesso processo) per non accumulare sprite e non
    // lasciare puntatori appesi dopo riallocazioni del vettore.
    void clearSponsorSprites() { sponsorSprites_.clear(); }

private:
    Framebuffer& fb_;
    const Palette& pal_;

    // Bande alternate (asfalto, erba, rumble) ancorate al segmento del mondo.
    static constexpr i32 BAND_SEGS = 2;
    static constexpr i32 HORIZON_Y = 112;       // riga dell'orizzonte: cielo sopra, terra sotto
    void drawSegmentTrapezoid(const Road::ProjectedSeg& cur, const Road::ProjectedSeg& next, bool band, f32 playerZ);
    void drawRoadsideMarkers(f32 playerZ);
    void drawSprite(const SpriteInstance& s, f32 screenX, f32 screenY, f32 scale);

    std::vector<Road::ProjectedSeg> projs_; // riutilizzato frame su frame (zero alloc a regime)
    std::vector<Sprite> sponsorSprites_;    // cartelloni 128x64, caricati una volta (zero alloc in gioco)
    f32 lastPlayerX_ = 0.0f;                // playerX dell'ultima drawRoad (per drawSprites)
    TemaColori tema_;                       // colori di ambiente della tappa
    // Stato della transizione graduale di tema tra due tappe (FORK1 punto 6).
    TemaColori temaDa_;                     // tema di partenza (tappa che finisce)
    TemaColori temaA_;                      // tema di arrivo (tappa che comincia), fisso durante la transizione
    f32 transizioneT_ = 1.0f;               // 1.0 = transizione finita (tema_ e' il tema attuale)
    f32 transizioneDurataS_ = 0.0f;         // durata in secondi (0 = scatto immediato)
    std::vector<HudElemento> hud_;          // elementi di hud.json (caricati una volta)
};

} // namespace sgl
