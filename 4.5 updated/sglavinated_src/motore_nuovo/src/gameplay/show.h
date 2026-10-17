// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "gameplay/escalation.h"
#include "gameplay/frasi.h"
#include "gameplay/personaggi.h"
#include "gameplay/ufo.h"

// Show di Gianni e Gesu': trigger, priorita', cooldown, escalation, fumetto.
// Specifica: motore_nuovo/dati/SPEC_GAMEPLAY.md (blocco D).
// Riferimenti di comportamento (non codice): hack v87, HANDOVER.md §0/§2/§4,
// SPEC_personaggi_v74.md §4-§7, dati/show_logica.md.
//
// Interfaccia verso il nucleo (una volta per frame):
//   1. il nucleo riempie StatoGameplayInput con i fatti del frame;
//   2. chiama crash() se c'e' stata una collisione/capottamento;
//   3. chiama aggiorna() e legge il DatiShow restituito (renderer della bolla).
// Deterministico: tempo = contatore di frame, tabelle a rotazione spiazzate dal
// seed, nessun orologio di sistema, nessuna dipendenza da SDL.

namespace sgl::gameplay {

// Configurazione tutta in dati: i numeri della v87 sono valori di partenza,
// non limiti (SPEC_GAMEPLAY §6: ad es. pr_tab e' un dato, non un vincolo).
struct ShowConfig {
    u32 durataShow           = 260; // SHDUR v85+ (5,2 s a 50 fps); scalabile per stage
    u32 cooldownShow         = 24;  // minimo fra due show (frame)
    u32 offRoadSoglia        = 8;   // frame fuori strada prima del trigger
    u32 offRoadAttesa        = 250; // evento off-road in attesa se uno show e' a schermo
    u32 offRoadTagliaPeriodico = 100; // show periodico di >= 100 frame fa spazio
    u32 inattivitaSoglia     = 500; // 10 s senza tasti a 50 fps (id once per pausa)
    u32 decadimentoErrori    = 600; // frame puliti per -1 errore (~20 s)
    u32 minGaraFrame         = 250; // guardia: gara avviata da >= 250 frame
    u32 minStageFrame        = 128; // guardia dopo il cambio stage (v77: 256 -> 128)
    u32 bollaAppare          = 30;  // frame dello show in cui appare la bolla
    u32 bollaNascondiGianni  = 30;  // bolla nascosta con meno di N frame rimasti
    u32 bollaNascondiGesu    = 55;
    // Intervallo periodico per stage (v77+: 200/180/165/150/140; v74 era
    // 500/440/400/360/320, ancora in frasi.json/show_logica.md). Dato: si cambia qui.
    u32 prTab[5] = { 200, 180, 165, 150, 140 };
};

// Fatti del frame, passati dal nucleo. Il modulo conta da solo i >= 8 frame
// off-road e i 10 s di inattivita': qui arrivano solo booleani grezzi.
struct StatoGameplayInput {
    u64 frame = 0;
    i32 stage = 0;          // 0-4 (prTab si satura oltre)
    bool inGara = false;    // gara in corso (no menu, demo, pausa, caricamento)
    bool inputAttivo = false; // almeno un tase premuto
    bool offRoad = false;   // ruote fuori dalla strada
};

// Stato dello show per il renderer e per i test.
struct DatiShow {
    bool attivo = false;
    Personaggio personaggio = Personaggio::Nessuno;
    OrigineShow origine = OrigineShow::Nessuna;
    u32 idFrase = 0;      // indice in dati/frasi.json
    u64 frameInizio = 0;
    u32 durata = 0;

    u32 frameRimasti(u64 frameCorrente) const {
        if (!attivo || frameCorrente < frameInizio) return 0;
        const u64 vissuti = frameCorrente - frameInizio;
        return (vissuti >= durata) ? 0 : static_cast<u32>(durata - vissuti);
    }
    f32 progresso(u64 frameCorrente) const {
        if (!attivo || durata == 0) return 0.0f;
        return clamp(static_cast<f32>(frameCorrente - frameInizio) /
                     static_cast<f32>(durata), 0.0f, 1.0f);
    }
};

class ShowManager {
public:
    explicit ShowManager(u32 seed = 1) { reset(seed); }

    void setConfig(const ShowConfig& c) { cfg_ = c; }
    const ShowConfig& config() const { return cfg_; }
    void setUfoConfig(const UfoConfig& c) { ufo_.setConfig(c); }

    // Nuova partita: azzaera tutto (chiamare con lo stesso seed per lo stesso run).
    void reset(u32 seed = 1);

    // Cambio stage: escalation e contatori azzerati, show attivo chiuso.
    void cambiaStage();

    // Il nucleo chiama su collisione/capottamento. Priorita' massima: al prossimo
    // aggiorna() sostituisce qualsiasi show in corso (crash -> sempre Gesu').
    void crash();

    // Avanza di un frame e restituisce lo show attivo (o vuoto).
    const DatiShow& aggiorna(const StatoGameplayInput& in);

    bool showAttivo() const { return show_.attivo; }
    const DatiShow& datiShow() const { return show_; }

    // Bolla di testo: quando deve essere visibile per il renderer (SPEC_GAMEPLAY §5).
    bool bollaVisibile() const;

    // Per altri moduli (effetti colore post-crash, HUD, test).
    i32 livelloEscalation() const { return esc_.livello(); }
    u32 erroriEscalation() const { return esc_.errori(); }

    const Ufo& ufo() const { return ufo_; }

private:
    ShowConfig cfg_{};
    Escalation esc_;
    SelezioneFrasi sel_;
    Ufo ufo_;
    DatiShow show_{};

    u64 frame_ = 0;            // ultimo frame aggiornato
    u64 frameGaraInizio_ = 0;  // primo frame con inGara (per la guardia minGaraFrame)
    u64 frameStage_ = 0;       // frame dell'ultimo cambio stage
    u64 ultimoShow_ = 0;       // frame di fine (o inizio, se primo) ultimo show
    u32 offRoadFrames_ = 0;    // frame consecutivi fuori strada
    u32 idleFrames_ = 0;       // frame consecutivi senza input
    bool offRoadPending_ = false;   // evento off-road in attesa (v85c SBORDATA)
    u64 frameOffRoadAttesa_ = 0;    // frame in cui e' scattato l'evento in attesa (250 frame v85c)
    bool crashPending_ = false;
    bool inattivitaArmata_ = false; // serve un input per riarmare (una volta per pausa)
    bool garaVista_ = false;
    u32 rotRampa_ = 0;         // contatore alternanza Gianni/Gesu' ai livelli 1-2

    void avviaShow(u64 frame, Personaggio p, OrigineShow o, u32 idFrase);
    void chiudiShow();

    u32 prTab(i32 stage) const {
        const u32 i = (stage < 0) ? 0u : (static_cast<u32>(stage) < 5u
                        ? static_cast<u32>(stage) : 4u);
        return cfg_.prTab[i];
    }
    bool prontoPerShow(u64 frame) const {
        return (frame - frameGaraInizio_) >= cfg_.minGaraFrame &&
               (frame - frameStage_) >= cfg_.minStageFrame;
    }

    // Scelte frase: crash / off-road / periodico (rampa) / inattivita'.
    u32 fraseCrash();
    u32 fraseOffRoad() { return sel_.scegli(TabId::Otab, tabelle::otab, 32); }
    u32 fraseInattivita() { return sel_.scegli(TabId::Itab, tabelle::itab, 4); }
    bool parlaGesuPeriodico(i32 livello);
    void frasePeriodica(i32 livello, Personaggio& p, TabId& tab, const u32*& ids,
                        u32& n);
};

} // namespace sgl::gameplay
