// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "input/pad.h"

namespace sgl {

// Auto del giocatore: stato fisico minimo, nessuna dipendenza da piattaforma.
class Player {
public:
    Player();

    // Applica l'input del frame e agisce sullo stato dell'auto.
    // speed dt: unita' di strada percorse questo frame (viene da Game.update).
    void input(const Pad& pad, f32 dt);
    void update(f32 dt);

    f32 speed = 0.0f;         // unita' al secondo (0..maxSpeed)
    f32 x = 0.0f;             // offset laterale dal centro strada
    f32 z = 0.0f;             // posizione assoluta sul tracciato
    bool offroad = false;     // fuori dalla carreggiata
    bool sulBordo = false;    // ancora in strada ma sul bordo (rumble): frena appena
    i32 gear = 1;             // marcia (1 o 2) - per HUD e feeling

    f32 maxSpeed() const { return MAX_SPEED; }
    // Comando di sterzo corrente -1 (sinistra) .. +1 (destra), con rampa
    // (player.cpp). Serve alla scelta del fotogramma dell'auto (RIV punto 3);
    // sola lettura, non cambia la fisica.
    f32 sterzo() const { return steer_; }

    // Carreggiata su cui sta l'auto (al bivio: il ramo piu' vicino). x resta
    // assoluto; fuoristrada e limite laterale si misurano da centroCorsia.
    f32 centroCorsia = 0.0f;
    f32 larghezzaCorsia = 1.0f; // 1 = strada intera, meno sui rami del bivio
    // Calo dello sterzo alla punta usato in questo frame: Game lo azzera quando
    // si sterza verso un'auto affiancata (Andrea 08/11: la sportellata deve riuscire).
    f32 caloPunta = STEER_CALO_PUNTA;
    static constexpr f32 caloPuntaNormale() { return STEER_CALO_PUNTA; }
    // Guida con peso (Andrea 11/11: "deve sembrare una sportiva incollata
    // all'asfalto"). L'auto ha una velocita' laterale vLat (unita'/s) che
    // insegue lo sterzo piu' le spinte esterne (spintaLat, curva e sovrasterzo,
    // messe da Game) con un ritardo che cresce con la velocita': piano e'
    // pronta, forte e' pesante e si porta dietro lo slancio (in uscita di
    // curva va ripresa). presaSterzo (0..1) e' quanto lo sterzo sposta l'auto:
    // con la coda di traverso le ruote davanti tengono l'angolo, non la
    // strada. accelMax limita la spinta (unita'/s^2): partenza e pattinamento.
    f32 vLat = 0.0f;           // totale = vSterzo + vSpinta (sola lettura fuori da Player)
    // Andrea 11/11 sera ("deve girare di lato subito, per colpire i rivali di
    // fianco"; "curve lunghe piu' realistiche, un'auto da corsa grossa e
    // reattiva"): due parti con ritardi diversi. Lo sterzo risponde presto
    // anche alla punta (INERZIA_STERZO_*); la spinta della curva cresce piano
    // (INERZIA_SPINTA_*), come il peso che si carica in una curva lunga, e in
    // uscita resta lo slancio.
    f32 vSterzo = 0.0f;
    // Guizzo della sportellata: Game lo alza sterzando verso un'auto vicina,
    // cosi' lo scarto di lato e' secco (1 = sterzo normale).
    f32 guizzo = 1.0f;
    f32 vSpinta = 0.0f;
    // Ferma (k = 0) o smorza la velocita' laterale (incidenti, testacoda, urti).
    void smorzaLaterale(f32 k) { vSterzo *= k; vSpinta *= k; vLat = vSterzo + vSpinta; }
    f32 spintaLat = 0.0f;
    f32 presaSterzo = 1.0f;
    f32 accelMax = 1.0e9f;
    bool gas() const { return accel_ > 0.0f; }
    // Spinta a tutto gas alla velocita' v (unita'/s^2), la stessa di update():
    // i rivali la usano per partire come il giocatore (Andrea 11/11).
    static f32 spintaA(f32 v);
    bool freno() const { return accel_ < 0.0f; }
    // Calcola offroad e applica il limite laterale rispetto alla corsia.
    void vincolaAllaCorsia();
    static constexpr f32 OFFROAD_LIMIT = 700.0f; // |x| oltre cui si e' offroad
    static constexpr f32 X_LIMIT = 900.0f; // clamp laterale: |x| oltre questo valore l'auto esce dallo schermo
    static constexpr f32 MEZZA_STRADA = 1200.0f; // = Road::ROAD_HALF_WORLD: bordo che si sposta quando la strada si allarga

private:
    f32 steer_ = 0.0f;
    f32 accel_ = 0.0f;
    // Tutti i valori sono "al secondo": moltiplicati per dt (secondi) in update().
    // speed e' di fatto unita' al secondo (già cosi' prima: speed*dt dava unità/frame).
    static constexpr f32 MAX_SPEED = 250.0f; // 250 unita'/secondo
    // Spinta iniziale piu' forte, che cala con la velocita': resta progressiva
    // e si avvicina a MAX_SPEED in circa 10 s.
    static constexpr f32 ACCEL_BASE = 56.0f;   // 56 unita'/secondo^2 (tarato per 60 Hz: distanza >= 12000 entro il frame 59)
    static constexpr f32 ACCEL_BOOST = 450.0f; // 450 unita'/secondo^2
    static constexpr f32 ACCEL_BOOST_SPEED = 100.0f; // 100 unita'/secondo
    // Freno: ~1.8 s da MAX_SPEED a 0.
    static constexpr f32 BRAKE = 120.0f;       // 120 unita'/secondo^2
    // Gas rilasciato: decadimento al secondo. pow(0.995,50) ~ 0.778 = perde ~22% al secondo.
    static constexpr f32 COAST_PER_SEC = 0.7782f;
    // Sterzo: spostamento laterale massimo STEER_MAX unita'/secondo, zero da
    // fermi, pieno a STEER_V_PIENA * MAX_SPEED, -5% alla punta (~800 unita'/s:
    // la strada, 1400 unita', si attraversa in meno di 2 s). Tiene a gas pieno
    // le curve fino a 0.6 (spinta 720); quelle strette si tengono col
    // sovrasterzo controllato (Game, SOVRA_*, Andrea 11/11). Gate: >= +/-200 in 0.6 s da fermi.
    static constexpr f32 STEER_MAX = 840.0f; // Andrea 06/11 sera 700 (era 600); 08/11 pomeriggio 780: "farei muovere di piu' il veicolo"; 11/11 840: "quando e' veloce deve girare di lato un po' di piu'"
    static constexpr f32 STEER_V_PIENA = 0.25f;
    static constexpr f32 STEER_CALO_PUNTA = 0.05f; // 06/11 0.35, 08/11 mattina 0.25; 08/11 pomeriggio 0.05: "fa fatica a muoversi tra destra e sinistra ad alte velocita'"
    static constexpr f32 STEER_RAMPA_PER_S = 8.0f;   // 0 -> pieno in 0.125 s
    static constexpr f32 STEER_RITORNO_PER_S = 12.0f; // pieno -> 0 in ~0.08 s
    // Peso (Andrea 11/11, rivisto la sera): ritardo da ferma + parte che
    // cresce col quadrato della velocita'. Sterzo: 0.03 -> 0.08 s alla punta
    // (prima 0.24 s per tutto: "non gira di lato abbastanza in fretta").
    // Spinta della curva: 0.10 -> 0.45 s alla punta (il carico sale in ~1 s).
    static constexpr f32 INERZIA_STERZO_BASE_S = 0.03f;
    static constexpr f32 INERZIA_STERZO_PUNTA_S = 0.05f;
    static constexpr f32 INERZIA_SPINTA_BASE_S = 0.10f;
    static constexpr f32 INERZIA_SPINTA_PUNTA_S = 0.35f;
    // Rallentamento fuori strada: decadimento al secondo. pow(0.99,50) ~ 0.605.
    static constexpr f32 OFFROAD_SLOW_PER_SEC = 0.6050f;
    // Bordo: ultimo 15% prima del fuori strada (feedback Andrea 23/10). Spinta
    // dimezzata, punta all'88%; a gas rilasciato perde un 15% in piu' al secondo.
    static constexpr f32 BORDO_INIZIO = 0.85f;
    static constexpr f32 BORDO_SPINTA = 0.5f;
    static constexpr f32 BORDO_PUNTA = 0.88f;
    static constexpr f32 BORDO_SLOW_PER_SEC = 0.85f;
};

} // namespace sgl
