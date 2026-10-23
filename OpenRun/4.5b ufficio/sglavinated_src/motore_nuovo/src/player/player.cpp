// SPDX-License-Identifier: MIT
#include "player/player.h"
#include <cmath>

namespace sgl {

Player::Player() = default;

void Player::input(const Pad& pad, f32 dt) {
    (void)dt; // riservato: input analogico con rampa
    steer_ = 0.0f;
    accel_ = 0.0f;
    if (pad.held & Pad::Left)  steer_ = -1.0f;
    if (pad.held & Pad::Right) steer_ = 1.0f;
    // Acceleratore: A (pad) o Su (tastiera). Freno: B o Giu'.
    if (pad.held & (Pad::Up | Pad::A))    accel_ = 1.0f;
    if (pad.held & (Pad::Down | Pad::B))  accel_ = -1.0f;
}

void Player::update(f32 dt) {
    // Sterzo: influenza x in proporzione alla velocita' (piu' veloci = piu' reattivi).
    x += steer_ * STEER_INC * dt * (1.0f + speed / MAX_SPEED);
    vincolaAllaCorsia();
    // Accelerazione / freno (valori al secondo * dt = incremento al frame).
    if (accel_ > 0.0f) {
        const f32 acceleration =
            ACCEL_BASE + ACCEL_BOOST * std::exp(-speed / ACCEL_BOOST_SPEED);
        speed += acceleration * dt * (1.0f - speed / MAX_SPEED);
    } else if (accel_ < 0.0f) speed -= BRAKE * dt;
    else speed *= std::pow(COAST_PER_SEC, dt); // attrito naturale (gas rilasciato)
    // Rallentamento fuori strada: l'erba/terra frena l'auto.
    if (offroad) speed *= std::pow(OFFROAD_SLOW_PER_SEC, dt);
    if (speed < 0.0f) speed = 0.0f;
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    // Avanzamento sulla strada (gestito anche da Game, qui si tiene la z locale).
    z += speed * dt;
    // Marcia: semplice per HUD (soglia in unita'/secondo, come speed).
    gear = (speed > 110.0f) ? 2 : 1;
}

void Player::vincolaAllaCorsia() {
    // Offroad: troppo lontano dal centro della corsia. Calcolato PRIMA del clamp a
    // X_LIMIT: OFFROAD_LIMIT=700 < X_LIMIT=900, quindi il clamp non nasconde mai
    // l'offroad. Il clamp tiene l'auto sullo schermo (cx resta in [25, 295] su 320 px).
    const f32 rel = x - centroCorsia;
    offroad = (rel < -OFFROAD_LIMIT * larghezzaCorsia) || (rel > OFFROAD_LIMIT * larghezzaCorsia);
    if (rel > X_LIMIT)  x = centroCorsia + X_LIMIT;
    if (rel < -X_LIMIT) x = centroCorsia - X_LIMIT;
}

} // namespace sgl
