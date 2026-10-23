// SPDX-License-Identifier: MIT
#include "player/player.h"
#include <algorithm>
#include <cmath>

namespace sgl {

Player::Player() = default;

void Player::input(const Pad& pad, f32 dt) {
    // Sterzo con rampa: il comando digitale porta steer_ verso -1/0/+1 in una
    // frazione di secondo (entrata morbida, ritorno al centro un po' piu' rapido).
    f32 obiettivo = 0.0f;
    if (pad.held & Pad::Left)  obiettivo -= 1.0f;
    if (pad.held & Pad::Right) obiettivo += 1.0f;
    const f32 rampa = (obiettivo == 0.0f ? STEER_RITORNO_PER_S : STEER_RAMPA_PER_S) * dt;
    if (steer_ < obiettivo) steer_ = std::min(obiettivo, steer_ + rampa);
    else if (steer_ > obiettivo) steer_ = std::max(obiettivo, steer_ - rampa);
    accel_ = 0.0f;
    // Acceleratore: A (pad) o Su (tastiera). Freno: B o Giu'.
    if (pad.held & (Pad::Up | Pad::A))    accel_ = 1.0f;
    if (pad.held & (Pad::Down | Pad::B))  accel_ = -1.0f;
}

void Player::update(f32 dt) {
    // Sterzo legato alla velocita': da ferma l'auto non si sposta di lato,
    // la risposta cresce fino a ~1/4 della velocita' massima e poi cala un po'
    // verso la punta (piu' stabile in alto, ma sempre sopra la spinta delle curve).
    const f32 v = speed / MAX_SPEED;
    const f32 risposta = std::min(1.0f, v / STEER_V_PIENA) * (1.0f - STEER_CALO_PUNTA * v * v);
    x += steer_ * STEER_MAX * risposta * dt;
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
