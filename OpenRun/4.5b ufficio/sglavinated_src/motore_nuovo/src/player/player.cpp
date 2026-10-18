// SPDX-License-Identifier: MIT
#include "player/player.h"
#include <cmath>

namespace sgl {

Player::Player() = default;

void Player::input(const Pad& pad, f32 dt) {
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
    // Offroad: troppo lontano dal centro. Calcolato PRIMA del clamp a X_LIMIT:
    // OFFROAD_LIMIT=700 < X_LIMIT=900, quindi il clamp non nasconde mai l'offroad
    // (con 1200 > 900 sarebbe sempre stato falso: codice morto pal_[12] in renderer).
    offroad = (x < -OFFROAD_LIMIT) || (x > OFFROAD_LIMIT);
    // Clamp laterale (P1b): dopo 60 frame di sterzata a destra playerX arrivava a 125 e oltre,
    // cx = 160 + x*4 in renderer portava l'auto fuori dallo schermo (spariva).
    // Con X_LIMIT=900 e fattore 0.15f in drawPlayerCar, cx resta in [25, 295] su 320 px:
    // l'auto e' sempre interamente visibile e i numeri restano limitati (determinismo).
    if (x > X_LIMIT)  x = X_LIMIT;
    if (x < -X_LIMIT) x = -X_LIMIT;
    // Accelerazione / freno.
    if (accel_ > 0.0f) {
        // La spinta extra e' piu' forte a bassa velocita' e cala gradualmente;
        // il fattore finale mantiene una crescita progressiva verso MAX_SPEED.
        const f32 acceleration =
            ACCEL_BASE + ACCEL_BOOST * std::exp(-speed / ACCEL_BOOST_SPEED);
        speed += acceleration * dt * (1.0f - speed / MAX_SPEED);
    } else if (accel_ < 0.0f) speed -= BRAKE * dt;
    else speed *= COAST; // attrito naturale (gas rilasciato)
    // Rallentamento fuori strada: l'erba/terra frena l'auto.
    if (offroad) speed *= OFFROAD_SLOW;
    if (speed < 0.0f) speed = 0.0f;
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    // Avanzamento sulla strada (gestito anche da Game, qui si tiene la z locale).
    z += speed * dt;
    // Marcia: semplice per HUD.
    gear = (speed > 110.0f) ? 2 : 1;
}

} // namespace sgl
