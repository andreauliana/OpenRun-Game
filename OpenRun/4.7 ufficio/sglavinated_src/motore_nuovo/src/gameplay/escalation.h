// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

// Escalation degli errori: livelli 0-3 con decadimento a tempo.
// Regole (SPEC_GAMEPLAY §2):
//   - +1 a ogni crash; +1 ogni 2 uscite di strada (una su due conta, alternata).
//   - L'uscita di strada aggiorna l'istante dell'ultimo errore senza contare.
//   - Livello = errori / 2, massimo 3 (0-1 -> 0, 2-3 -> 1, 4-5 -> 2, >= 6 -> 3).
//   - Ogni "decadimentoFrame" senza errori -> errori -1 (~600 frame = 12 s a 50 fps).
//   - Reset al cambio stage.
// Riferimento v87: VAR186 (errori), VAR188 (istante ultimo errore), VAR190 (livello),
// v78_ck/v78_dl — valori e regole, non codice.

namespace sgl::gameplay {

class Escalation {
public:
    // Nuova partita o cambio stage: errori azzerati.
    void reset() { errori_ = 0; ultimoErrore_ = 0; altUscita_ = 0; }

    // Il nucleo chiama su collisione/capottamento.
    void notificaCrash(u64 frame) {
        errori_++;
        ultimoErrore_ = frame;
    }

    // Chiamata quando l'uscita di strada supera la soglia di 8 frame.
    // Una su due conta come errore; l'istante dell'ultimo errore si aggiorna sempre.
    void notificaUscitaStrada(u64 frame) {
        ultimoErrore_ = frame;
        if ((altUscita_++ & 1u) == 0u) errori_++;
    }

    // Avanza il decadimento: chiamare una volta per frame.
    void aggiorna(u64 frame, u32 decadimentoFrame) {
        if (errori_ == 0 || decadimentoFrame == 0) {
            ultimoErrore_ = frame;
            return;
        }
        const u64 trascorsi = frame - ultimoErrore_;
        const u32 periodi = static_cast<u32>(trascorsi / decadimentoFrame);
        if (periodi == 0) return;
        if (periodi >= errori_) {
            errori_ = 0;
            ultimoErrore_ = frame;
        } else {
            errori_ -= periodi;
            ultimoErrore_ += static_cast<u64>(periodi) * decadimentoFrame;
        }
    }

    u32 errori() const { return errori_; }

    // 0-3, saturato.
    i32 livello() const {
        const u32 liv = errori_ / 2;
        return static_cast<i32>(liv > 3 ? 3 : liv);
    }

private:
    u32 errori_ = 0;       // VAR186
    u64 ultimoErrore_ = 0; // VAR188 (contatore di frame)
    u32 altUscita_ = 0;    // VAR240: alterna "conta/non conta" fra uscite di strada
};

} // namespace sgl::gameplay
