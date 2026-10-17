// MSG_CONTROLLO_NEGATIVO (BLOCCO H, voce 74/76) — NON COMPILARE, NON COSTRUIRE.
//
// Scopo: prova che il gate `clang++ -std=c++17 -fsyntax-only` NON e' vuoto,
// cioe' che fallirebbe davvero se il nucleo avesse un errore di sintassi.
// Contiene un errore voluto (parentesi graffa non chiusa + token inventato).
//
// Questo file NON fa parte di alcun target: motore_nuovo/CMakeLists.txt elenca i
// sorgenti a mano (nessun GLOB), e nessun target include una sottocartella
// test/. Non spostarlo in src/. Licenza: SPDX-License-Identifier: MIT
//
// Atteso: clang++ -std=c++17 -fsyntax-only -> error: expected '}'
//         esito "NEGATO" (exit != 0) = controllo negativo superato.

namespace sgl { namespace test_negativo {

int rompi_qui() {
    return 0;
// manca la graffa di chiusura di proposito
