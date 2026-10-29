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
#include "render/visualizzatore.h"
#include "player/player.h"
#include "gameplay/show.h"
#include "gameplay/disegno_show.h" // scena show: personaggio/fumetto/UFO in px
#include "gameplay/testi_frasi.h"  // testi frasi.json per i fumetti
#include "game/fasi.h"             // blocco R2: macchina a stati di gioco
#include <array>
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
    // soloPrimaTappa (corsa singola del menu, Andrea 27/10): stessa partenza,
    // stessi tracciati, rivali e tempo limite della gara, ma senza bivio: la
    // prima tappa finisce con l'arrivo.
    bool avviaGara(const std::string& fileTappe = "motore_nuovo/dati/percorso_gara.json",
                   bool soloPrimaTappa = false);
    bool corsaSingola() const { return corsaSingola_; }
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
    // RIV punto b: consegna gli sprite delle auto rivali (decodati dal runner/test
    // dai raw di png_a_raw.py) prima di init()/avviaGara().
    void setRivaliSprites(std::vector<Sprite> sprites);
    // Traffico comune (Andrea 27/10): sprite dei veicoli civili, prima di avviaGara().
    void setTrafficoSprites(std::vector<Sprite> sprites);
    // Numero di cartelloni posizionati bordo strada nella pista corrente.
    size_t numeroCartelloni() const { return cartelloni_.size(); }
    // Oggetti decorativi della tappa (posizioni fisse; per i gate).
    const std::vector<SpriteInstance>& oggettiStrada() const { return oggettiStrada_; }

    // Traffico comune in gara (Andrea 27/10): pochi veicoli civili lenti nelle
    // corsie, deterministici (seme fisso). x e' laterale in unita' mondo dal
    // centro della carreggiata; al bivio ogni veicolo segue il suo ramo.
    struct Veicolo {
        f32 z = 0.0f;     // posizione nella tappa corrente
        f32 x = 0.0f;     // laterale (unita' mondo)
        f32 passo = 0.0f; // unita' di strada al secondo
        i32 corsia = 1;   // -2, -1 = corsie a sinistra; 1, 2 = a destra (2 solo su strada larga)
        i32 ramo = 1;     // ramo preso al bivio (-1 / +1)
        i32 spriteIdx = -1;
        bool attivo = false;
    };
    static constexpr i32 TRAFFICO = 4;
    const Veicolo& veicolo(i32 i) const { return traffico_[i]; }
    // Il giocatore tocca un'auto (avversario o traffico): stesso test del bump.
    bool collisioneConAuto() const { return collisioneRivaleVicino(); }
    // Centro della corsia di un veicolo alla posizione z (unita' mondo).
    f32 xCorsia(i32 corsia, f32 z) const;

    // --- Blocco R2: macchina a stati di gioco ---
    FaseId faseCorrente() const { return fasi_.attuale(); }
    // Dati per la schermata risultati (distanza, tappa, tempo residuo in secondi).
    f32 distanzaKm() const { return km_; }
    i32 numeroTappa() const { return numeroTappa_; }
    f32 tempoResiduoS() const { return tempoResiduoS_; }
    bool arrivato() const { return arrivo_; }
    // Posizione del giocatore nella gara (1 = primo). In arrivo riporta la
    // posizione registrata al traguardo (blocco FIN).
    i32 posizioneArrivo() const { return arrivo_ ? posizioneArrivo_ : posizioneGiocatore(); }
    // Dopo il traguardo l'auto frena da sola: secondi da quando e' ferma (-1 = si muove ancora).
    f32 fermoDopoArrivoS() const { return fermoS_; }
    // Memoria delle botte della gara (Andrea 29/10): auto urtate e tocchi del ciglio.
    i32 urtiGara() const { return urtiGara_; }
    i32 toccateCiglio() const { return toccateCiglio_; }
    // Apre lo Splash (poi Menu): il runner lo chiama all'avvio, dopo avviaGara().
    // Senza questa chiamata init()/avviaGara() partono subito in Gara (gate, --subito).
    void mostraMenu() { fasi_.cambia(FaseId::Splash, *this); }
    // Voce ESCI del menu: il runner legge il flag e chiude la finestra.
    void richiediUscita() { uscitaRichiesta_ = true; }
    bool uscitaRichiesta() const { return uscitaRichiesta_; }

    // Contatto tra auto (Andrea 29/10). La telecamera sta dietro l'auto del
    // giocatore: a schermo la sua carrozzeria copre le auto tra URTO_Z_MIN e
    // URTO_Z_MAX unita' davanti alla telecamera. Li' due auto si toccano se
    // |dx| < URTO_LARGHEZZA. Prima il contatto era a meno di 120 unita' (sotto
    // la telecamera) e a schermo le auto si attraversavano prima di toccarsi.
    static constexpr f32 URTO_Z_MIN = 750.0f;
    static constexpr f32 URTO_Z_MAX = 1250.0f;
    static constexpr f32 URTO_LARGHEZZA = 240.0f;

    // Classifica d'arrivo (Andrea 29/10): tempo di ognuno sul traguardo,
    // ordinata dal piu' veloce. Vuota finche' il giocatore non arriva.
    struct Classificato {
        const char* nome = "";
        f32 tempoS = 0.0f;
        bool giocatore = false;
    };
    const std::vector<Classificato>& classificaArrivo() const { return classifica_; }
    f32 tempoTotaleS() const { return (f32)frameGara_ / (f32)frequenzaHz_; }

    // Radio: brani tracker (.xm) di dati/audio/playlist.json, uno per il menu e
    // quelli della gara (Andrea 29/10). In gara il tasto R (o Start+B) passa al prossimo.
    void nextBranoRadio() { if (radioInGara_) ++radioGara_; }
    const InfoMusica& musica() const { return mixer_.info(); } // per lo spettro e i tocchi a ritmo
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
    // Radio: brani caricati una volta; aggiornaRadio sceglie quello della fase/tappa.
    void caricaMusica();
    void aggiornaRadio();
    bool musicaCaricata_ = false;
    i32 branoMenu_ = -1;      // indice nel mixer del brano del menu (-1 = nessuno)
    i32 primoBranoGara_ = 0;  // i brani della gara vengono dopo
    i32 radioGara_ = -1;      // avanza a ogni gara e con R / Start+B
    bool radioInGara_ = false;
    void drawMusica(); // spettro e battito in gara
    // Urti con le altre auto (Andrea 29/10): nessuna compenetrazione, urto
    // diverso per il traffico (botta forte, clacson) e per i rivali (sportellata).
    std::vector<i32> spritesFonte_; // parallelo a sprites_ in gara: >= 0 rivale, < 0 traffico (-1 - indice)
    f32 passoFonte(i32 fonte) const;
    void urto(i32 fonte, f32 dx, bool tamponamento);
    f32 urtoEffettoS_ = 0.0f, urtoEffettoDurataS_ = 1.0f;
    bool urtoTraffico_ = false;
    // Memoria delle botte (Andrea 29/10): restano ammaccature sull'auto, il
    // conteggio va nei risultati. Ciglio: il primo tocco rallenta, un altro
    // tocco a velocita' alta mentre si ricorda il primo e' un incidente.
    i32 urtiGara_ = 0, toccateCiglio_ = 0;
    // Sorpasso (Andrea 29/10): la POS dell'HUD si evidenzia per SORPASSO_S secondi.
    i32 posizionePrec_ = 0;
    f32 sorpassoS_ = 0.0f;
    static constexpr f32 SORPASSO_S = 1.0f;
    f32 dallUrtoS_ = -1.0f;      // secondi dall'ultima botta (scintille, lampo)
    i32 latoUrto_ = 0;
    void segnaBotta(i32 lato);
    bool offroadPrec_ = false;
    f32 ciglioMemoriaS_ = 0.0f;  // > 0: il tocco precedente e' ancora "ricordato"
    void toccaCiglio();
    void ribalta();
    void liberaRipartenza();
    f32 ciglioRallenta_ = 0.7f;      // config.json ciglio_rallenta
    f32 ciglioMemoriaDurataS_ = 6.0f; // config.json ciglio_memoria_s
    f32 ciglioCrashVelocita_ = 0.45f; // config.json ciglio_crash_velocita (frazione della punta)
    std::vector<Classificato> classifica_;
    void calcolaClassifica();
    void scuotiScena(i32 dx, i32 dy);
    std::vector<u32> scossaBuf_;
    Visualizzatore visualizzatore_;
    // Salto sui dossi ad alta velocita' (quota dell'auto sopra la strada, unita' mondo).
    void aggiornaSalto(f32 dt);
    f32 saltoAltezza_ = 0.0f, saltoVY_ = 0.0f;
    f32 saltoQuotaPrec_ = -1e9f, saltoVStradaPrec_ = 0.0f;
    f32 tempoExtraS_ = 0.0f; // avviso "TEMPO EXTRA" dopo un checkpoint
    // Arrivo sulla strada (Andrea 29/10): linea a scacchi, pubblico ai lati
    // dietro le transenne, l'auto frena da sola e si ferma, poi la classifica.
    void preparaArrivo();     // sulla tappa d'arrivo: linea e pubblico
    void caricaSpriteFolla(); // dati/sprite_raw/pubblico/ (PNG in assets_originali/sprite/pubblico/)
    void frenataArrivo(f32 dt);
    // La strada si disegna e si aggiorna in gara, a fine tempo e, dopo
    // l'arrivo, anche sotto la classifica.
    bool scenaStrada() const {
        const FaseId f = fasi_.attuale();
        return f == FaseId::Gara || f == FaseId::FineTempo || (arrivo_ && f == FaseId::Risultati);
    }
    std::array<Sprite, 4> spriteFolla_; // gruppi a, b x pose giu', su
    std::vector<SpriteInstance> folla_; // ordinata per worldZ
    f32 arrivoS_ = 0.0f;      // secondi dal traguardo
    f32 fermoS_ = -1.0f;      // secondi da fermo dopo il traguardo
    f32 velocitaArrivo_ = 0.0f;
    std::vector<SpriteInstance> sprites_; // traffico base (placeholder)
    // Cartelloni sponsor: gli sprite sono di proprieta' del Renderer
    // (setSponsorSprites li registra li); qui si tengono solo gli indici.
    std::vector<i32> sponsorSpriteIdx_;   // indice per ren_.sponsorSprite() (caricati una volta)
    std::vector<std::string> sponsorId_;  // sponsorId_[i] e' l'id di sponsorSpriteIdx_[i] (per abbinare livello.json)
    Sprite skylineSprite_;                // skyline della tappa in corso (DECO1 2a), svuotato dopo caricaLivello
    std::vector<SpriteInstance> cartelloni_;   // posizioni bordo strada fissate in init(), nessun alloc dopo
    // Ordina cartelloni e oggetti per worldZ (dopo ogni caricamento) e prenota scenaSprites_.
    void ordinaSpriteFissi();
    // Aggiunge a scenaSprites_ gli sprite fissi nel tratto visibile davanti all'auto.
    void aggiungiVisibili(const std::vector<SpriteInstance>& fissi);
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
    // Ramo seguito a scelta aperta (-1/+1, 0 = nessuno) con isteresi: cambia
    // solo se l'auto e' chiaramente piu' vicina all'altro ramo (prova Andrea 27/10).
    i32 ramoProvvisorio_ = 0;
    // Sterzo del giocatore al bivio: -1 sinistra, +1 destra, 0 nessuno o entrambi.
    i32 sterzoBivio() const {
        const bool sx = (input_.held & Pad::Left) != 0, dx = (input_.held & Pad::Right) != 0;
        return sx == dx ? 0 : (sx ? -1 : 1);
    }
    f32 centroRamoPrec_ = 0.0f; // centro del ramo seguito al frame prima (l'auto si muove con lui)
    f32 playerXIncidente_ = 0.0f; // x al momento del ribaltamento (ramo su cui ripartire al bivio)
    bool sollevamentoAttivo_ = false;
    f32 sollevamento_ = 0.0f;      // 0..1, segue l'obiettivo con un ritardo
    f32 sollevamentoFaseS_ = 0.0f; // tempo per l'ondeggio mentre l'auto e' sollevata
    u32 playerFlipFrame_ = 0;
    u32 offroadFrames_ = 0;
    bool playerBump_ = false;     // stato di bump dopo urto con un avversario (nessun ribaltamento)
    f32 playerBumpTimer_ = 0.0f;  // tempo restante del bump (secondi)
    f32 playerBumpSpintaX_ = 0.0f; // spinta laterale residua del bump (unita'/s)
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
    bool collisioneRivaleVicino() const;   // RIV: contatto con un avversario (bump, non ribaltamento)
    bool collisioneCartelloneVicino() const; // RIV: contatto con un cartellone bordo strada (nessun ribaltamento)

    // Blocco R2: logica separata per fase di guida e fase di menu.
    void aggiornaGuida(f32 dt);
    void aggiornaMenu(f32 dt);

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
    // RIV (Andrea 27/10): contatto con un avversario = bump, non ribaltamento.
    f32 bumpRallentaFattore_ = 0.85f; // fattore velocita' dopo urto con rivale (0.85 = -15%, config.json)
    f32 bumpDurataS_ = 1.0f;          // durata stato bump (secondi, config.json)
    f32 bumpSpintaLaterale_ = 200.0f; // spinta laterale urto rivale (unita'/s, config.json)
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
        i32 colore = 9; // indice di palette della sagoma (solo sagoma sintetica)
        f32 bravura = 1.0f; // bravura propria (0.92..1.08, seme fisso), scala passoMax
        i32 spriteIdx = -1; // indice sprite rivale in Renderer (RIV punto b), -1 = rettangolo sintetico
        f32 ritmoFase = 0.0f, ritmoVel = 0.2f; // ritmo proprio: un po' piu' veloce o lento nel tempo
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
    bool corsaSingola_ = false; // avviaGara(..., true): una tappa, niente bivio
    // La tappa id finisce con un bivio (gara a tappe, non corsa singola).
    bool tappaConBivio(const std::string& id) const;
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
    i32 nSpriteRivali_ = 0; // sprite rivali caricati (RIV punto b)
    Veicolo traffico_[TRAFFICO];
    Rng rndTraffico_{1u};
    i32 nSpriteTraffico_ = 0;
    void inizializzaTraffico();
    void aggiornaTraffico(f32 dt);
    // Mette il veicolo davanti, oltre la vista e dopo il veicolo piu' avanti;
    // nessun veicolo nuovo nella zona del bivio.
    void piazzaVeicolo(Veicolo& v, f32 zMin);
    // Aggiunge a sprites_ i veicoli visibili (dopo i rivali).
    void aggiungiSpriteTraffico();
    bool uscitaRichiesta_ = false; // voce ESCI del menu (blocco R2)
    GestoreFasi fasi_; // blocco R2: Splash -> Menu -> Gara -> FineTempo -> Risultati
};

// Dichiarazioni anticipate per prova_gate.cpp (definizioni locali statiche
// in prova_gate.cpp; servono prima della definizione locale alla riga 256).
sgl::i32 leggiFrequenzaDaConfig();
sgl::i32 impostaFrequenzaDaConfig(sgl::i32 hz);

} // namespace sgl
