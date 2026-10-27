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

    // Carreggiata su cui sta l'auto (al bivio: il ramo piu' vicino). x resta
    // assoluto; fuoristrada e limite laterale si misurano da centroCorsia.
    f32 centroCorsia = 0.0f;
    f32 larghezzaCorsia = 1.0f; // 1 = strada intera, meno sui rami del bivio
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
    // Sterzo: spostamento laterale massimo 600 unita'/secondo, zero da fermi,
    // pieno a STEER_V_PIENA * MAX_SPEED, -15% alla punta (510 > spinta curva 0.8
    // a velocita' massima, ~356). Gate: >= +/-200 in 0.6 s partendo da fermi.
    static constexpr f32 STEER_MAX = 600.0f;
    static constexpr f32 STEER_V_PIENA = 0.25f;
    static constexpr f32 STEER_CALO_PUNTA = 0.15f;
    static constexpr f32 STEER_RAMPA_PER_S = 8.0f;   // 0 -> pieno in 0.125 s
    static constexpr f32 STEER_RITORNO_PER_S = 12.0f; // pieno -> 0 in ~0.08 s
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
