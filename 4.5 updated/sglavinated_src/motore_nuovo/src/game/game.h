// SPDX-License-Identifier: MIT
#pragma once
#include "core/frame_counter.h"
#include "core/rng.h"
#include "core/replay.h"
#include "core/state.h"
#include "road/road.h"
#include "render/framebuffer.h"
#include "render/palette.h"
#include "render/renderer.h"
#include "input/pad.h"
#include "audio/mixer.h"
#include "player/player.h"
#include "gameplay/show.h"
#include "gameplay/disegno_show.h" // scena show: personaggio/fumetto/UFO in px
#include "gameplay/testi_frasi.h"  // testi frasi.json per i fumetti
#include <string>
#include <utility>
#include <vector>

namespace sgl {
using gameplay::ShowManager;
using gameplay::DatiShow;
using gameplay::StatoGameplayInput;
using gameplay::Ufo;
using gameplay::DisegnoShow;
using gameplay::TestiFrasi;
using gameplay::ScenaShow;
using gameplay::ShowLayout;

// Gioco: tiene insieme stato, strada, rendering, audio, input e replay.
// E' l'interfaccia usata sia dal runner SDL2 che (in futuro) dal core libretro.
// Deterministico: nessun orologio di sistema, tutto deriva da frame + seed.
class Game {
public:
    static constexpr i32 AUDIO_FRAME_SAMPLES = 44100 / 50; // campioni per frame a 50 Hz

    Game();
    explicit Game(u32 seed);

    // Inizializza il tracciato e lo stato (chiamato una volta all'avvio).
    // livelloId: id della cartella in motore_nuovo/dati/livelli/<id>/ (default "lv_01_neon");
    // legge "cartelloni_sponsor" da <id>/livello.json (tutte le allocazioni qui).
    void init(const std::string& livelloId = "lv_01_neon");

    // Passa gli input del frame corrente (dal runner o dal core libretro).
    void setInput(const Pad& pad);

    // Carica i cartelloni sponsor (chiamato dal runner PRIMA di init(): le
    // posizioni bordo strada vengono fissate durante init()).
    // Ogni sprite e' accompagnato dal suo sponsor_id (catalogo: v. sponsor_loader.h);
    // init() abbina lo sponsor_id di livello.json allo sprite corrispondente.
    void setSponsorSprites(std::vector<std::pair<std::string, Sprite>> sprites);

    // Radio: passa al brano successivo (input del runner: tasto R o Start+B).
    void nextBranoRadio() { mixer_.nextBrano(); }
    i32 branoRadio() const { return mixer_.currentTrack(); }    // indice 0-based del brano in corso
    i32 numBraniRadio() const { return mixer_.numTracks(); }    // totale brani caricati (0 se radio assente)

    // Avanza di un frame: logica + audio. Deterministico.
    void update();

    // Disegna il frame corrente nel framebuffer interno.
    void render();

    // Esporta lo stato (per HUD, moduli fx, replay, test).
    const GameState& state() const { return state_; }
    const Pad& input() const { return input_; }
    Framebuffer& fb() { return fb_; }
    const std::vector<i16>& audio() const { return audioBuf_; } // stereo interleaved

    // Hash del frame: usato dai test di replay (ogni N frame identico su esecuzioni uguali).
    u32 frameHash() const;

private:
    u32 seed_;
    FrameClock clock_;
    Rng rng_;
    Replay replay_;
    Pad input_;
    Road road_;
    Framebuffer fb_;
    Palette pal_;
    Renderer ren_;
    Mixer mixer_;
    Player player_;
    ShowManager showManager_; // gameplay (blocco D): show, escalation, UFO
    DisegnoShow disegnoShow_; // disegno scena show (personaggio, fumetto, UFO) in px
    TestiFrasi testiFrasi_;   // testi frasi.json caricati una volta in init()
    GameState state_;

    // Scena show del frame corrente (dopo update()); renderer la disegna in render().
    const ScenaShow& scenaShow() const { return disegnoShow_.scena(); }
    const TestiFrasi& testiFrasi() const { return testiFrasi_; }
    std::vector<i16> audioBuf_; // stereo interleaved, riempito ogni update
    std::vector<SpriteInstance> sprites_; // traffico base (placeholder)
    // Cartelloni sponsor: gli sprite sono di proprieta' del Renderer
    // (setSponsorSprites li registra li); qui si tengono solo gli indici.
    std::vector<i32> sponsorSpriteIdx_;   // indice per ren_.sponsorSprite() (caricati una volta)
    std::vector<std::string> sponsorId_;  // sponsorId_[i] e' l'id di sponsorSpriteIdx_[i] (per abbinare livello.json)
    std::vector<SpriteInstance> cartelloni_;   // posizioni bordo strada fissate in init(), nessun alloc dopo
    std::vector<SpriteInstance> scenaSprites_; // riutilizzato per merge traffico + cartelloni
    bool inCollision_ = false; // fronte di salita collisione giocatore/traffico
    bool trafficoAttivo_ = false; // P1b: placeholder traffico disattivato di default (solo strada + auto)

    // Dati cartelloni da livello.json (letto una volta in init).
    bool leggiCartelloniDaFile(const std::string& livelloId);
    // Segmenti del tracciato da livello.json (campo "segmenti"): curva/collina/lunghezza.
    // Ritorna false se il file e' illeggibile o il campo manca/vuoto (init usa il fallback).
    bool leggiSegmentiDaFile(const std::string& livelloId, std::vector<Road::SegmentDesc>& out) const;
    i32 risolviSponsorSprite(const std::string& sponsorId) const;

    // Accessori verso renderer/HUD/fx (dati show, nessun testo qui).
    const DatiShow& showDati() const { return showManager_.datiShow(); }
    bool showAttivo() const { return showManager_.showAttivo(); }
    i32 livelloEscalation() const { return showManager_.livelloEscalation(); }
    const Ufo& ufoDati() const { return showManager_.ufo(); }

    void spawnTraffic();
};

} // namespace sgl
