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
// Deterministico: tempo in secondi con dt (1/frequenzaHz_), tabelle a rotazione
// spiazzate dal seed, nessun orologio di sistema, nessuna dipendenza da SDL.
//
namespace sgl::gameplay {

// Configurazione tutta in dati: i numeri della v87 sono valori di partenza,
// non limiti (SPEC_GAMEPLAY §6: ad es. pr_tab e' un dato, non un vincolo).
// Tutte le durate sono in SECONDI (indipendenti da 50/60 Hz) cosicche' il
// comportamento sia identico a entrambe le frequenze.
struct ShowConfig {
    f32 durataShow           = 4.333333f; // show in secondi (260 frame a 60 Hz)
    f32 cooldownShow         = 0.48f;     // minimo fra due show (24 frame a 50 Hz)
    f32 offRoadSoglia        = 0.16f;     // secondi fuori strada prima del trigger (8 frame a 50 Hz)
    f32 offRoadAttesa        = 5.0f;      // evento off-road in attesa se uno show e' a schermo (250 frame a 50 Hz)
    f32 offRoadTagliaPeriodico = 2.0f;    // show periodico di >= 2 s fa spazio (100 frame a 50 Hz)
    f32 inattivitaSoglia     = 10.0f;     // 10 s senza tasti (500 frame a 50 Hz) (id once per pausa)
    f32 decadimentoErrori    = 12.0f;     // secondi puliti per -1 errore (~20 s)
    f32 minGaraS             = 5.0f;      // guardia: gara avviata da >= 5 s (250 frame a 50 Hz)
    f32 minStageS            = 2.56f;     // guardia dopo il cambio stage (v77: 256 -> 128 frame a 50 Hz)
    f32 bollaAppare          = 0.6f;      // secondi di show in cui appare la bolla (30 frame a 50 Hz)
    f32 bollaNascondiGianni  = 0.6f;      // bolla nascosta con meno di N secondi rimasti
    f32 bollaNascondiGesu    = 1.1f;      // (55 frame a 50 Hz)
    // Intervallo periodico per stage (secondi; v77+: 4.0/3.6/3.3/3.0/2.8 a 50 Hz).
    // Dato: si cambia qui, non nel codice.
    f32 prTab[5] = { 4.0f, 3.6f, 3.3f, 3.0f, 2.8f };
};

// Fatti del frame, passati dal nucleo. Il modulo conta da solo i >= 8 frame
// off-road e i 10 s di inattivita': qui arrivano solo booleani grezzi.
struct StatoGameplayInput {
    u64 frame = 0;          // frame corrente (mantenuto per contatori esterni)
    f32 dt = 0;             // secondi trascorsi dal frame precedente (1/frequenzaHz_)
    i32 stage = 0;          // 0-4 (prTab si satura oltre)
    bool inGara = false;    // gara in corso (no menu, demo, pausa, caricamento)
    bool inputAttivo = false; // almeno un tasto premuto
    bool offRoad = false;   // ruote fuori dalla strada
};

// Stato dello show per il renderer e per i test.
struct DatiShow {
    bool attivo = false;
    Personaggio personaggio = Personaggio::Nessuno;
    OrigineShow origine = OrigineShow::Nessuna;
    u32 idFrase = 0;      // indice in dati/frasi.json
    f32 tempoInizio = 0;  // secondi di avvio dello show
    f32 durata = 0;       // durata dello show in secondi

    f32 tempoRimasto(f32 tempoCorrente) const {
        if (!attivo || tempoCorrente < tempoInizio) return 0;
        const f32 vissuti = tempoCorrente - tempoInizio;
        return (vissuti >= durata) ? 0 : static_cast<f32>(durata - vissuti);
    }
    f32 progresso(f32 tempoCorrente) const {
        if (!attivo || durata == 0) return 0.0f;
        return clamp(static_cast<f32>(tempoCorrente - tempoInizio) /
                     static_cast<f32>(durata), 0.0f, 1.0f);
    }
};

class ShowManager {
public:
    explicit ShowManager(u32 seed = 1) { reset(seed); }

    void setConfig(const ShowConfig& c) { cfg_ = c; }
    const ShowConfig& config() const { return cfg_; }
    void setUfoConfig(const UfoConfig& c) { ufo_.setConfig(c); }

    // Nuova partita: azzera tutto (chiamare con lo stesso seed per lo stesso run).
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

    f32 tempoS_ = 0;            // secondi di gioco accumulati (aggiornati con dt)
    f32 tempoGaraInizioS_ = 0;  // primi secondi con inGara (per la guardia minGaraS)
    f32 tempoStageS_ = 0;       // secondi dell'ultimo cambio stage
    f32 tempoUltimoShowS_ = 0;  // secondi di fine (o inizio, se primo) ultimo show
    f32 tempoOffRoadS_ = 0;     // secondi consecutivi fuori strada
    f32 tempoIdleS_ = 0;        // secondi consecutivi senza input
    bool offRoadPending_ = false;   // evento off-road in attesa (v85c SBORDATA)
    f32 tempoOffRoadAttesaS_ = 0;   // secondi in cui e' scattato l'evento in attesa
    bool crashPending_ = false;
    bool inattivitaArmata_ = false; // serve un input per riarmare (una volta per pausa)
    bool garaVista_ = false;
    u32 rotRampa_ = 0;         // contatore alternanza Gianni/Gesu' ai livelli 1-2

    void avviaShow(f32 tempoS, Personaggio p, OrigineShow o, u32 idFrase);
    void chiudiShow();

    f32 prTab(i32 stage) const {
        const u32 i = (stage < 0) ? 0u : (static_cast<u32>(stage) < 5u
                        ? static_cast<u32>(stage) : 4u);
        return cfg_.prTab[i];
    }
    bool prontoPerShow(f32 tempoS) const {
        return (tempoS - tempoGaraInizioS_) >= cfg_.minGaraS &&
               (tempoS - tempoStageS_) >= cfg_.minStageS;
    }

    // Scelte frase: crash / off-road / periodico / inattivita'.
    u32 fraseCrash();
    u32 fraseOffRoad() { return sel_.scegli(TabId::Otab, tabelle::otab, 32); }
    u32 fraseInattivita() { return sel_.scegli(TabId::Itab, tabelle::itab, 4); }
    bool parlaGesuPeriodico(i32 livello);
    void frasePeriodica(i32 livello, Personaggio& p, TabId& tab, const u32*& ids,
                        u32& n);
};

} // namespace sgl::gameplay
