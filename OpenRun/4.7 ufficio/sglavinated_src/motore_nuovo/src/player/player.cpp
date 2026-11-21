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
    // Acceleratore: A (pad) o Su (tastiera).
    // Per garantire che Pad::A attivi l'accelerazione anche quando Up e' premuto,
    // trattiamo i due casi separatamente.
    if (pad.held & Pad::A)    accel_ = 1.0f;
    if (pad.held & Pad::Up)   accel_ = 1.0f;
    // Freno: B o Giu'.
    if (pad.held & (Pad::Down | Pad::B))  accel_ = -1.0f;
}

void Player::update(f32 dt) {
    // Sterzo legato alla velocita': da ferma l'auto non si sposta di lato,
    // la risposta cresce fino a ~1/4 della velocita' massima e poi cala un po'
    // verso la punta (piu' stabile in alto, ma sempre sopra la spinta delle curve).
    const f32 v = speed / MAX_SPEED;
    const f32 risposta = std::min(1.0f, v / STEER_V_PIENA) * (1.0f - caloPunta * v * v);
    // Peso (Andrea 11/11): la velocita' laterale insegue sterzo + spinte con
    // un ritardo che cresce con la velocita' (pronta piano, pesante forte).
    const f32 sterzoCurvo = steer_ * (STEER_MORBIDO + (1.0f - STEER_MORBIDO) * std::abs(steer_));
    const f32 obiettivoSterzo = sterzoCurvo * STEER_MAX * risposta * presaSterzo * guizzo;
    const f32 tauSterzo = INERZIA_STERZO_BASE_S + INERZIA_STERZO_PUNTA_S * v * v;
    const f32 tauSpinta = INERZIA_SPINTA_BASE_S + INERZIA_SPINTA_PUNTA_S * v * v;
    vSterzo += (obiettivoSterzo - vSterzo) * (1.0f - std::exp(-dt / tauSterzo));
    vSpinta += (spintaLat - vSpinta) * (1.0f - std::exp(-dt / tauSpinta));
    vLat = vSterzo + vSpinta;
    x += vLat * dt;
    vincolaAllaCorsia();
    // Accelerazione / freno (valori al secondo * dt = incremento al frame).
    if (accel_ > 0.0f) {
        const f32 acceleration =
            ACCEL_BASE + ACCEL_BOOST * std::exp(-speed / ACCEL_BOOST_SPEED);
        // Sul bordo (rumble) la spinta cala e la punta scende a BORDO_PUNTA:
        // si guadagna poco o niente, ma col gas premuto non si perde velocita'.
        const f32 punta = sulBordo ? BORDO_PUNTA * MAX_SPEED : MAX_SPEED;
        const f32 spinta = sulBordo ? BORDO_SPINTA : 1.0f;
        const f32 passo = std::min(acceleration * spinta * (1.0f - speed / punta), accelMax);
        speed += std::max(0.0f, passo * dt);
    } else if (accel_ < 0.0f) speed -= BRAKE * dt;
    else {
        speed *= std::pow(COAST_PER_SEC, dt); // attrito naturale (gas rilasciato)
        if (sulBordo) speed *= std::pow(BORDO_SLOW_PER_SEC, dt); // il bordo frena un po' di piu'
    }
    // Rallentamento fuori strada: l'erba/terra frena l'auto.
    if (offroad) {
        const bool profonda = fuoriOltre > ERBA_ZONA_PROFONDA;
        const f32 tetto = (profonda ? ERBA_TETTO_PROFONDO : ERBA_TETTO_VICINO) * MAX_SPEED;
        if (speed > tetto) speed = std::max(tetto, speed - (profonda ? ERBA_CALO_PROFONDO : ERBA_CALO_VICINO) * dt);
        else speed *= std::pow(ERBA_ATTRITO_PER_S, dt);
    }
    if (speed < 0.0f) speed = 0.0f;
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    // Avanzamento sulla strada (gestito anche da Game, qui si tiene la z locale).
    z += speed * dt;
    // Cambio automatico a tre marce (Andrea 16/11, prima due): sale alle
    // soglie MARCIA_SU, scende un po' sotto (MARCIA_GIU) cosi' non fa avanti e
    // indietro sulla soglia. I giri ripartono piu' bassi a ogni cambiata.
    if (gear < 1) gear = 1;
    while (gear < MARCE && speed > MARCIA_SU[gear - 1]) ++gear;
    while (gear > 1 && speed < MARCIA_GIU[gear - 2]) --gear;
}

f32 Player::giri() const {
    // Giri motore 0..1 nella marcia inserita: da GIRI_DOPO_CAMBIATA (appena
    // cambiato) a 1 sulla soglia della marcia dopo; in prima da 0.
    const f32 da = gear <= 1 ? 0.0f : MARCIA_SU[gear - 2];
    const f32 a = gear >= MARCE ? MAX_SPEED : MARCIA_SU[gear - 1];
    const f32 base = gear <= 1 ? 0.0f : GIRI_DOPO_CAMBIATA;
    const f32 t = std::clamp((speed - da) / (a - da), 0.0f, 1.0f);
    return base + (1.0f - base) * t;
}

f32 Player::spintaA(f32 v) {
    return std::max(0.0f, (ACCEL_BASE + ACCEL_BOOST * std::exp(-v / ACCEL_BOOST_SPEED)) * (1.0f - v / MAX_SPEED));
}

void Player::vincolaAllaCorsia() {
    // Offroad: troppo lontano dal centro della corsia. Calcolato PRIMA del clamp a
    // X_LIMIT: OFFROAD_LIMIT=700 < X_LIMIT=900, quindi il clamp non nasconde mai
    // l'offroad. Il clamp tiene l'auto sullo schermo (cx resta in [25, 295] su 320 px).
    // Strada piu' larga (larghezzaCorsia > 1, piu' corsie): i limiti si
    // spostano di quanto si sposta il bordo (MEZZA_STRADA per unita' di
    // larghezza), cosi' l'auto puo' usare tutte le corsie. Rami del bivio
    // (larghezzaCorsia < 1): scala proporzionale come prima.
    const f32 extra = larghezzaCorsia > 1.0f ? (larghezzaCorsia - 1.0f) * MEZZA_STRADA : 0.0f;
    const f32 limite = larghezzaCorsia > 1.0f ? OFFROAD_LIMIT + extra : OFFROAD_LIMIT * larghezzaCorsia;
    const f32 limiteX = X_LIMIT + extra;
    const f32 rel = x - centroCorsia;
    offroad = (rel < -limite) || (rel > limite);
    fuoriOltre = offroad ? std::abs(rel) - limite : 0.0f;
    sulBordo = !offroad && std::abs(rel) > limite - (1.0f - BORDO_INIZIO) * OFFROAD_LIMIT * std::min(1.0f, larghezzaCorsia);
    if (rel > limiteX)  { x = centroCorsia + limiteX; vSterzo = std::min(vSterzo, 0.0f); vSpinta = std::min(vSpinta, 0.0f); vLat = vSterzo + vSpinta; }
    if (rel < -limiteX) { x = centroCorsia - limiteX; vSterzo = std::max(vSterzo, 0.0f); vSpinta = std::max(vSpinta, 0.0f); vLat = vSterzo + vSpinta; }
}

} // namespace sgl
