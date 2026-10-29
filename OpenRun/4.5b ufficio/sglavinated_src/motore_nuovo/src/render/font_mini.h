// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

namespace sgl {

// Font pixel 3x5 per le etichette dell'HUD (TEMPO, TOTALE, VEL...): celle 4x6.
// Solo maiuscole, cifre e pochi segni; le minuscole usano le maiuscole.
// Ogni riga e' una maschera di 3 bit (bit alto = colonna sinistra).
constexpr i32 FONT_MINI_W = 4;
constexpr i32 FONT_MINI_H = 5;

inline const u8* fontMini(char c) {
    static constexpr u8 VUOTO[5] = {0, 0, 0, 0, 0};
    static constexpr u8 LETTERE[26][5] = {
        {2, 5, 7, 5, 5}, {6, 5, 6, 5, 6}, {3, 4, 4, 4, 3}, {6, 5, 5, 5, 6}, // A B C D
        {7, 4, 6, 4, 7}, {7, 4, 6, 4, 4}, {3, 4, 5, 5, 3}, {5, 5, 7, 5, 5}, // E F G H
        {7, 2, 2, 2, 7}, {1, 1, 1, 5, 2}, {5, 5, 6, 5, 5}, {4, 4, 4, 4, 7}, // I J K L
        {5, 7, 7, 5, 5}, {6, 5, 5, 5, 5}, {2, 5, 5, 5, 2}, {6, 5, 6, 4, 4}, // M N O P
        {2, 5, 5, 6, 3}, {6, 5, 6, 5, 5}, {3, 4, 2, 1, 6}, {7, 2, 2, 2, 2}, // Q R S T
        {5, 5, 5, 5, 7}, {5, 5, 5, 5, 2}, {5, 5, 7, 7, 5}, {5, 5, 2, 5, 5}, // U V W X
        {5, 5, 2, 2, 2}, {7, 1, 2, 4, 7},                                   // Y Z
    };
    static constexpr u8 CIFRE[10][5] = {
        {7, 5, 5, 5, 7}, {2, 6, 2, 2, 7}, {6, 1, 2, 4, 7}, {6, 1, 2, 1, 6}, {5, 5, 7, 1, 1},
        {7, 4, 6, 1, 6}, {3, 4, 7, 5, 7}, {7, 1, 2, 2, 2}, {7, 5, 7, 5, 7}, {7, 5, 7, 1, 6},
    };
    static constexpr u8 BARRA[5] = {1, 1, 2, 4, 4}, PUNTO[5] = {0, 0, 0, 0, 2},
                        MENO[5] = {0, 0, 7, 0, 0}, PIU[5] = {0, 2, 7, 2, 0},
                        DUEPUNTI[5] = {0, 2, 0, 2, 0}, APICE[5] = {2, 2, 0, 0, 0},
                        VIRGOLETTE[5] = {5, 5, 0, 0, 0};
    if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A');
    if (c >= 'A' && c <= 'Z') return LETTERE[c - 'A'];
    if (c >= '0' && c <= '9') return CIFRE[c - '0'];
    switch (c) {
        case '/': return BARRA;
        case '.': return PUNTO;
        case '-': return MENO;
        case '+': return PIU;
        case ':': return DUEPUNTI;
        case '\'': return APICE;
        case '"': return VIRGOLETTE;
        default: return VUOTO;
    }
}

} // namespace sgl
