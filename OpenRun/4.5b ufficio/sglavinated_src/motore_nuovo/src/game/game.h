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
struct JsonValue; // core/json_mini.h (usato solo in game.cpp)
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

    // Gara a tappe (dati/percorso_gara.json): parte dalla tappa di partenza con
    // gli avversari in griglia; a fine tappa un bivio porta, a sinistra o a
    // destra, alla tappa seguente; una tappa senza rami e' l'arrivo.
    // Senza avviaGara() il livello di init() gira in tondo (test, prove).
    bool avviaGara(const std::string& fileTappe = "motore_nuovo/dati/percorso_gara.json");
    const std::string& tappaCorrente() const { return tappaId_; }

    // Cartella dati (sglavinated_src) da usare al posto della cartella corrente:
    // serve al core libretro e a chi lancia il gioco da altre cartelle. Prima di init().
    void setRadiceDati(std::string radice) { radiceDati_ = std::move(radice); }

    // Passa gli input del frame corrente (dal runner o dal core libretro).
    void setInput(const Pad& pad);

    // Carica i cartelloni sponsor (chiamato dal runner PRIMA di init(): le
    // posizioni bordo strada vengono fissate durante init()).
    // Ogni sprite e' accompagnato dal suo sponsor_id (catalogo: v. sponsor_loader.h);
    // init() abbina lo sponsor_id di livello.json allo sprite corrispondente.
    void setSponsorSprites(std::vector<std::pair<std::string, Sprite>> sprites);
    // Numero di cartelloni posizionati bordo strada nella pista corrente.
    size_t numeroCartelloni() const { return cartelloni_.size(); }

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
    const Road& road() const { return road_; } // tracciato (per i test di percorso)
    const std::vector<SpriteInstance>& traffico() const { return sprites_; } // avversari/traffico del frame
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
    bool trafficoAttivo_ = true; // traffico sintetico visibile e collisioni nel percorso
    bool playerFlipping_ = false;
    u32 playerFlipFrame_ = 0;
    u32 offroadFrames_ = 0;
    static constexpr u32 FLIP_ANIMATION_FRAMES = 75; // 1.5 s a 50 Hz
    static constexpr u32 OFFROAD_CRASH_FRAMES = 100; // sbanda a lungo prima del ribaltamento

    // Dati cartelloni da livello.json (letto una volta in init).
    // radice: livello.json gia' analizzato (nullptr se il file manca o non e' valido).
    bool leggiCartelloniDaFile(const JsonValue* radice);
    // Segmenti del tracciato da livello.json (campo "segmenti"): curva/collina/lunghezza.
    // Ritorna false se il file e' illeggibile o il campo manca/vuoto (init usa il fallback).
    bool leggiSegmentiDaFile(const JsonValue* radice, std::vector<Road::SegmentDesc>& out) const;
    // Percorso di un file dati: relativo alla cartella corrente oppure a radiceDati_.
    std::string percorsoDati(const std::string& relativo) const;
    std::string radiceDati_; // vuota = cartella corrente (runner), es. sglavinated_src
    i32 risolviSponsorSprite(const std::string& sponsorId) const;

    // Accessori verso renderer/HUD/fx (dati show, nessun testo qui).
    const DatiShow& showDati() const { return showManager_.datiShow(); }
    bool showAttivo() const { return showManager_.showAttivo(); }
    i32 livelloEscalation() const { return showManager_.livelloEscalation(); }
    const Ufo& ufoDati() const { return showManager_.ufo(); }

    void spawnTraffic();
    bool collisioneOggettiVicini() const;

    // Tracciato, cartelloni e tema di una tappa (senza toccare auto e show).
    void caricaLivello(const std::string& livelloId);
    void leggiTemaDaFile(const JsonValue* radice);
    void leggiHud(); // dati/ui/hud.json -> Renderer (una volta)
    i32 tempoLimiteTappaS_ = 60; // tempo_limite_s della tappa caricata
    u32 tempoResiduoFrame_ = 0;  // tempo rimasto in gara (frame a 50 Hz)
    bool tempoScaduto_ = false;
    f32 km_ = 0.0f;              // distanza percorsa (contachilometri dell'HUD)
    std::string stileCartellone_; // "neon" | "atomico" | "foglie" (tema_grafico.stile_cartellone)

    // --- Gara a tappe ---
    struct Tappa {
        std::string id, sinistra, destra; // rami vuoti = arrivo
    };
    struct Rivale {
        f32 progresso = 0.0f; // distanza percorsa dalla partenza (tutte le tappe)
        f32 x = 0.0f, xObiettivo = 0.0f; // posizione nella corsia
        f32 passo = 0.0f, passoMax = 0.0f; // unita' di strada per frame
        i32 ramo = 1;   // ramo preso ai bivi (-1 sinistra, +1 destra)
        i32 colore = 9; // indice di palette della sagoma
    };
    static constexpr i32 RIVALI = 7;
    const Tappa* trovaTappa(const std::string& id) const;
    void preparaBivio();
    void inizializzaRivali();
    void aggiornaRivali();
    void fineTappa(f32 avanzo);
    i32 posizioneGiocatore() const;
    void aggiornaStatoGara();
    std::vector<Tappa> tappe_;
    std::string tappaId_;
    bool gara_ = false;
    bool arrivo_ = false;
    i32 numeroTappa_ = 1;
    i32 latoBivio_ = 0; // 0 = scelta aperta, -1 sinistra, +1 destra
    i32 posizioneArrivo_ = 0;
    u32 frameGara_ = 0;
    f32 progressoTappePrecedenti_ = 0.0f;
    f32 passoGiocatore_ = 0.0f; // strada percorsa dal giocatore nell'ultimo frame
    Rivale rivali_[RIVALI];
    Rng rndRivali_{1u};
};

} // namespace sgl
