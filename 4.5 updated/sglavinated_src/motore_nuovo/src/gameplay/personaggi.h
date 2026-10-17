// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

// Modulo gameplay — personaggi e fumetti reattivi (Gianni, Gesu', UFO).
// Regole: motore_nuovo/dati/SPEC_GAMEPLAY.md e motore_nuovo/dati/show_logica.md.
// Testi/id: motore_nuovo/dati/frasi.json (CC-BY-4.0, autore Andrea).
// Il codice e' MIT; le tabelle di id rispecchiano frasi.json.

namespace sgl::gameplay {

// Chi puo' apparire a schermo con un fumetto (l'UFO non parla, non e' qui).
enum class Personaggio : u8 {
    Nessuno = 0,
    Gianni = 1, // il tentatore: off-road, periodico, inattivita'
    Gesu   = 2, // il correttore: crash, periodico di livello
};

// Perche' e' nato lo show: la provenienza va tenuta perche' 16 id compaiono
// sia in otab sia in ph_tab — la frase da sola non la distingue (SPEC_GAMEPLAY §8).
enum class OrigineShow : u8 {
    Nessuna = 0,
    Crash,        // priorita' massima -> Gesu'
    UscitaStrada, // -> Gianni, non conta come errore (1 su 2 si)
    Periodico,    // a tempo, chi parla dipende dal livello di escalation
    Inattivita,   // 10 s senza tasti -> Gianni, una volta per pausa
};

// Nome in ASCII per log e test (nessuna dipendenza da <string>).
inline const char* nome(Personaggio p) {
    switch (p) {
        case Personaggio::Gianni: return "Gianni";
        case Personaggio::Gesu:   return "Gesu";
        default:                  return "nessuno";
    }
}

inline const char* nome(OrigineShow o) {
    switch (o) {
        case OrigineShow::Crash:        return "crash";
        case OrigineShow::UscitaStrada: return "uscita_di_strada";
        case OrigineShow::Periodico:    return "periodico";
        case OrigineShow::Inattivita:   return "inattivita";
        default:                        return "nessuna";
    }
}

} // namespace sgl::gameplay
