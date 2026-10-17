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
    static constexpr f32 MAX_SPEED = 250.0f; // velocita' di gioco, resa come alta velocita'
    // Crescita progressiva: ~63% in 2 s, ~92% in 5 s e ~99% in 9 s a 50 Hz.
    static constexpr f32 ACCEL = 125.0f;
    // Freno: ~1.8 s da MAX_SPEED a 0 (prima 10 = ~22 s, frenava meno di quanto accelera).
    static constexpr f32 BRAKE = 120.0f;
    // Gas rilasciato: moltiplicatore per frame. 0.995 = perde ~22% al secondo a 50 Hz
    // (prima 0.98 = ~63% al secondo: l'auto si fermava appena si mollava il gas).
    static constexpr f32 COAST = 0.995f;
    static constexpr f32 STEER_INC = 90.0f; // unita' x per secondo di sterzo
    static constexpr f32 OFFROAD_LIMIT = 700.0f; // |x| oltre cui si e' offroad
    static constexpr f32 X_LIMIT = 900.0f; // clamp laterale: |x| oltre questo valore l'auto esce dallo schermo
    // Rallentamento fuori strada: moltiplicatore per frame quando offroad.
    // 0.99 = perde ~1% della velocita' per frame a 50 Hz: rallentamento percepibile
    // ma non brusco (prima 0.85 crollava da ~160 a ~6 in pochi frame).
    static constexpr f32 OFFROAD_SLOW = 0.99f;
};

} // namespace sgl
