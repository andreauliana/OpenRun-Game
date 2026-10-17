// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "gameplay/personaggi.h"
#include <vector>

// Testi dei fumetti, letti da motore_nuovo/dati/frasi.json (CC-BY-4.0,
// autore Andrea). Il modulo gameplay logico (show.h) gira solo su id: qui
// invece gli id diventano caratteri, per il renderer del fumetto.
// Specifica: dati/SPEC_GAMEPLAY.md §5 ("l'id frase basta al renderer") e §9.
// Regole personaggio <-> frase (invarianti v87): frasi.h eGianni()/eGesu(),
// qui riverificate in verifica().
// Deterministico: nessun orologio di sistema; il file si legge una volta
// (carica(), chiamare in init), mai nel ciclo di gioco.

namespace sgl::gameplay {

// Una frase: 1-2 righe di massimo MAX_RIGA-1 caratteri (nessun limite 88 px
// della v87: qui e' solo un salvaguardia di buffer).
struct FraseTesto {
    static constexpr u32 MAX_RIGA = 48;
    char linea1[MAX_RIGA] = {0};
    char linea2[MAX_RIGA] = {0}; // vuota se la frase ha una sola riga
    u8 righe = 0;                // 1 o 2 (0 = voce vuota)
    Personaggio personaggio = Personaggio::Nessuno;
};

class TestiFrasi {
public:
    // Legge frasi.json (percorso relativo alla working dir del runner, come
    // livello.json in game.cpp). Ritorna false se il file o il campo "frasi"
    // mancano; in quel caso resta tutto vuoto (il renderer disegna solo la
    // scatola del fumetto, senza testo). Tutte le allocazioni stanno qui.
    bool carica(const char* percorso);

    // Testo di un id frase; nullptr se l'id non esiste o la voce e' vuota.
    const FraseTesto* get(u32 id) const {
        return (static_cast<size_t>(id) < f_.size()) ? &f_[id] : nullptr;
    }

    u32 conteggio() const { return static_cast<u32>(f_.size()); }

    // Invariante personaggio <-> frase: ogni voce deve appartenere ai range
    // del suo personaggio (Gianni 97 id, Gesu' 59). Da chiamare una volta
    // dopo carica() (test/init); false = dati incoerenti.
    bool verifica() const;

private:
    std::vector<FraseTesto> f_; // indice = id frase
};

} // namespace sgl::gameplay
