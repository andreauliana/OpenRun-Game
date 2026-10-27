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

// Dichiarazioni anticipate per prova_gate.cpp (definizioni locali statiche
// in prova_gate.cpp; queste dichiarazioni servono solo prima della riga 256).
sgl::i32 leggiFrequenzaHz();
sgl::i32 impostaFrequenzaHzTemporanea(sgl::i32 hz);

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
    static constexpr i32 AUDIO_FRAME_SAMPLES = 44100 / 60; // campioni per frame a 60 Hz (frequenza di default)

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
    // Sprite dello skyline della tappa in corso (DECO1 2a), consegnato dal runner
    // prima di init(); vuoto = cielo piatto. Il core resta senza SDL.
    void setSkylineSprite(Sprite sprite);
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
    // Frequenza logica (Hz) di dati/config.json: il runner fa un tick ogni 1/frequenza s.
    i32 frequenzaHz() const { return frequenzaHz_; }
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
    Sprite skylineSprite_;                // skyline della tappa in corso (DECO1 2a), svuotato dopo caricaLivello
    std::vector<SpriteInstance> cartelloni_;   // posizioni bordo strada fissate in init(), nessun alloc dopo
    std::vector<SpriteInstance> scenaSprites_; // riutilizzato per merge traffico + cartelloni
    std::vector<SpriteInstance> oggettiStrada_; // oggetti bordo strada (DECO1 2c), posizioni fisse
    bool inCollision_ = false; // fronte di salita collisione giocatore/traffico
    bool trafficoAttivo_ = true; // traffico sintetico visibile e collisioni nel percorso
    bool playerFlipping_ = false;
    // Sollevamento dell'auto vicino alla velocita' massima, sui rettilinei
    // (livello.json moduli_fx.sollevamento_auto_alta_velocita).
    // Passaggio assistito al bivio: 0 = guida normale, 1 = assistenza piena.
    // Sale entrando nel bivio e cala piano a inizio tappa nuova.
    f32 assistenzaBivio_ = 0.0f;
    f32 playerXIncidente_ = 0.0f; // x al momento del ribaltamento (ramo su cui ripartire al bivio)
    bool sollevamentoAttivo_ = false;
    f32 sollevamento_ = 0.0f;      // 0..1, segue l'obiettivo con un ritardo
    f32 sollevamentoFaseS_ = 0.0f; // tempo per l'ondeggio mentre l'auto e' sollevata
    u32 playerFlipFrame_ = 0;
    u32 offroadFrames_ = 0;
    static constexpr f32 FLIP_ANIMATION_S = 1.5f;    // durata ribaltamento in secondi (indipendente dalla frequenza)
    static constexpr f32 OFFROAD_CRASH_S = 2.0f;     // secondi fuori strada prima del ribaltamento

    // Dati cartelloni da livello.json (letto una volta in init).
    // radice: livello.json gia' analizzato (nullptr se il file manca o non e' valido).
    // segmentoOccupato (dimensione nSeg del tracciato) viene segnato true per i
    // segmenti che ricevono un cartellone esplicito dal file.
    bool leggiCartelloniDaFile(const JsonValue* radice, std::vector<bool>& segmentoOccupato);
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
    void caricaOggettiStrada(const std::string& livelloId); // DECO1 2c: carica sprite oggetti bordo strada
    i32 tempoLimiteTappaS_ = 60; // tempo_limite_s della tappa caricata
    f32 tempoResiduoS_ = 0.0f;   // tempo rimasto in gara (secondi, frequenza indipendente)
    bool tempoScaduto_ = false;
    i32 frequenzaHz_ = 60;       // frequenza logica (Hz), letta da dati/config.json
f32 transizioneTemaS_ = 1.0f; // durata transizione graduale tema tra tappe (secondi, dati/config.json)
    f32 km_ = 0.0f;              // distanza percorsa (contachilometri dell'HUD)
    f32 tempoGiocoS_ = 0;        // secondi di gioco accumulati (frequenza indipendente)
    std::string stileCartellone_; // "neon" | "atomico" | "foglie" (tema_grafico.stile_cartellone)

    // Tema grafico della tappa (tema_grafico di livello.json): id del catalogo
    // dello skyline sopra l'orizzonte e degli oggetti a bordo strada. Letto una
    // volta in leggiTemaDaFile(); i sprite veri si disegnano quando @grafico
    // consegna le conversioni raw e il renderer le aggancia (DECO1 punto 1/2).
    std::string skylineId_;               // tema_grafico.skyline
    std::vector<std::string> oggettiBordoStrada_; // tema_grafico.oggetti_bordo_strada

    const std::string& skylineId() const { return skylineId_; }
    const std::vector<std::string>& oggettiBordoStrada() const { return oggettiBordoStrada_; }

    // --- Gara a tappe ---
    struct Tappa {
        std::string id, sinistra, destra; // rami vuoti = arrivo
    };
    struct Rivale {
        f32 progresso = 0.0f; // distanza percorsa dalla partenza (tutte le tappe)
        f32 x = 0.0f, xObiettivo = 0.0f; // posizione nella corsia
        f32 passo = 0.0f, passoMax = 0.0f; // velocita' in unita' di strada al secondo
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
    f32 secondiDaSceltaBivio_ = 0.0f; // secondi dalla scelta del ramo al bivio (fattore ramp)
    Rivale rivali_[RIVALI];
    Rng rndRivali_{1u};
};

// Dichiarazioni anticipate per prova_gate.cpp (definizioni locali statiche
// in prova_gate.cpp; servono prima della definizione locale alla riga 256).
sgl::i32 leggiFrequenzaDaConfig();
sgl::i32 impostaFrequenzaDaConfig(sgl::i32 hz);

} // namespace sgl
