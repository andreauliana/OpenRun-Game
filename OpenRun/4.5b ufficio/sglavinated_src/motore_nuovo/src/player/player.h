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
    static constexpr f32 STEER_INC = 320.0f;   // 320 unita'/secondo^2 (tarato per 60 Hz: sterzo >= +/-200 in 30 frame)
    // Rallentamento fuori strada: decadimento al secondo. pow(0.99,50) ~ 0.605.
    static constexpr f32 OFFROAD_SLOW_PER_SEC = 0.6050f;
};

} // namespace sgl
