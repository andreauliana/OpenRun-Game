// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include <string>
#include <vector>

namespace sgl {

// Voce della classifica salvata (massimo 10, ordinata per punti).
struct ClassificaEntry {
    std::string nome; // fino a 6 caratteri: A-Z, 0-9, '.', spazio
    i32 punti = 0;
    std::string data; // "AAAA-MM-GG": nessun orologio di sistema nel core; valore
                      // conservato dal file precedente, altrimenti "0000-00-00".
};

// Dati del giocatore: nome salvato (fino a 6 caratteri) e classifica Top-10.
struct GiocatoreDati {
    char nome[7] = {'P', 'L', 'A', 'Y', 'E', 'R', '\0'}; // spazi = posto libero
    std::vector<ClassificaEntry> classifica;        // gara a tappe (RACE SERIES)
    std::vector<ClassificaEntry> classificaSingola; // corsa singola (Andrea 11/11: tabelle separate)
};

// Persistenza del giocatore (blocco ARC1).
// File: salvataggi/giocatore.json (fuori da dati/, in .gitignore).
// - se manca o e' danneggiato: default senza errori, si ricrea al primo salvataggio;
// - scrittura atomica: file temporaneo + rename;
// - mai I/O durante la gara (solo entrata/uscita dalla fase Risultati).
class SalvataggiGiocatore {
public:
    static const char* PERCORSO; // "salvataggi/giocatore.json"

    // Carica il file; se manca o e' illeggibile restituisce i default.
    static GiocatoreDati carica();
    // Salva in modo atomico (temporaneo + rename). Ritorna true se riuscito.
    static bool salva(const GiocatoreDati& g);

private:
    static bool creaDirectorySeManca();
};

} // namespace sgl
