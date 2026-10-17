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

    f32 speed = 0.0f;         // unita' per frame (0..maxSpeed)
    f32 x = 0.0f;             // offset laterale dal centro strada
    f32 z = 0.0f;             // posizione assoluta sul tracciato
    bool offroad = false;     // fuori dalla carreggiata
    i32 gear = 1;             // marcia (1 o 2) - per HUD e feeling

    f32 maxSpeed() const { return MAX_SPEED; }

private:
    f32 steer_ = 0.0f;
    f32 accel_ = 0.0f;
    static constexpr f32 MAX_SPEED = 220.0f;
    // Accelerazione: con la formula esponenziale speed += ACCEL*dt*(1-speed/MAX),
    // ACCEL=30 raggiunge ~90% di MAX_SPEED in circa 15-18 s (sensazione OutRun:
    // crescita che cala avvicinandosi alla velocita' massima).
    static constexpr f32 ACCEL = 30.0f;
    static constexpr f32 BRAKE = 10.0f;
    static constexpr f32 STEER_INC = 90.0f; // unita' x per secondo di sterzo
    static constexpr f32 OFFROAD_LIMIT = 700.0f; // |x| oltre cui si e' offroad
    static constexpr f32 X_LIMIT = 900.0f; // clamp laterale: |x| oltre questo valore l'auto esce dallo schermo
    // Rallentamento fuori strada: moltiplicatore per frame quando offroad.
    // 0.99 = perde ~1% della velocita' per frame a 50 Hz: rallentamento percepibile
    // ma non brusco (prima 0.85 crollava da ~160 a ~6 in pochi frame).
    static constexpr f32 OFFROAD_SLOW = 0.99f;
};

} // namespace sgl
