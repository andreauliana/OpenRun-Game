// SPDX-License-Identifier: MIT
#include "gameplay/frasi.h"

// Tabelle di id — copia di lavoro di motore_nuovo/dati/frasi.json (CC-BY-4.0,
// autore Andrea; fonte v87: tools/gianni12.py + OPENCODE_REFERENCE_v74.json).
// SOLO indici: nessun testo, nessun asset. Confronto automatico con i range
// personaggio in verificaTabelle().

namespace sgl::gameplay::tabelle {

// Gianni, uscita di strada — ph+otab v87 (id anche in ph_tab: la provenienza
// la decide il trigger, non la frase).
const u32 otab[32] = {
    134, 135, 136, 121, 142, 143, 144, 145,
    146, 152, 153, 122, 123, 124,  44,  19,
     54,  64,  65, 134, 135, 136, 121, 142,
    143, 144, 145, 146, 152, 153, 122, 123,
};

// Gesu', crash al livello 0.
const u32 xtab[32] = {
    126, 130, 141, 147, 148, 149, 150, 151,
    154, 155,  73,  76,  80,  82, 106, 107,
    108, 109, 126, 130, 141, 147, 148, 149,
    150, 151, 154, 155,  73,  76,  80,  82,
};

// Gianni, periodico livello 0 (v87 "ph_tab", 32 entrate piatte).
const u32 ptab[32] = {
     88,  89,  90,  94,  95,  96,  97, 103,
     88, 137, 138, 139, 140,  88,  89,  90,
     94,  95,  96,  97, 103,  88, 137, 138,
    139, 140,  88,  89,  90,  94,  95,  96,
};

// Gesu', periodico livello 0 quando la rampa lo sceglie.
const u32 jtab[32] = {
    125, 126, 127, 128, 129, 141, 147, 148,
    149, 150, 151, 154, 155, 106, 107, 108,
    109, 114, 115, 130, 131, 132, 133, 120,
     73,  76,  80,  82, 125, 126, 127, 128,
};

// Gianni, inattivita' (id esclusivi, come l'itab della v87).
const u32 itab[4] = { 35, 36, 37, 38 };

// Escalation Gianni: tentatore duro (patti, anima, fuoco).
const u32 g1t[6] = {  0,  1,  2,  3,  4,  5 };
const u32 g2t[6] = {  6,  7,  8,  9, 10, 12 };
const u32 g3t[6] = { 14, 15, 17, 18, 20, 21 };

// Escalation Gesu': predicatore (luce, risveglio, "alzati e cammina").
const u32 j1t[6] = { 27, 28, 29, 30, 31, 32 };
const u32 j2t[6] = { 66, 67, 68, 69, 70, 71 };
const u32 j3t[6] = { 72, 105, 110, 111, 112, 113 };

} // namespace sgl::gameplay::tabelle

namespace sgl::gameplay {

// Range personaggio, da frasi.json "range_personaggio_v87" (invariati dalla v87):
// Gianni 0-26, 34-65, 85-104, 121-124, 134-140, 142-146, 152-153.
bool eGianni(u32 id) {
    return (id <= 26) || (id >= 34 && id <= 65) || (id >= 85 && id <= 104) ||
           (id >= 121 && id <= 124) || (id >= 134 && id <= 140) ||
           (id >= 142 && id <= 146) || (id >= 152 && id <= 153);
}

// Gesu' 27-33, 66-84, 105-120, 125-133, 141, 147-151, 154-155.
bool eGesu(u32 id) {
    return (id >= 27 && id <= 33) || (id >= 66 && id <= 84) ||
           (id >= 105 && id <= 120) || (id >= 125 && id <= 133) ||
           id == 141 || (id >= 147 && id <= 151) || (id >= 154 && id <= 155);
}

namespace {
template <typename F>
bool tutti(const u32* ids, u32 n, F f) {
    for (u32 i = 0; i < n; ++i)
        if (!f(ids[i])) return false;
    return true;
}
} // namespace

bool verificaTabelle() {
    using namespace tabelle;
    return tutti(otab, 32, eGianni) && tutti(ptab, 32, eGianni) &&
           tutti(itab, 4, eGianni) &&
           tutti(g1t, 6, eGianni) && tutti(g2t, 6, eGianni) && tutti(g3t, 6, eGianni) &&
           tutti(xtab, 32, eGesu) && tutti(jtab, 32, eGesu) &&
           tutti(j1t, 6, eGesu) && tutti(j2t, 6, eGesu) && tutti(j3t, 6, eGesu);
}

u32 SelezioneFrasi::scegli(TabId tab, const u32* ids, u32 n) {
    if (n == 0) return 0;
    const u32 i = static_cast<u32>(tab);
    u32 k = rot_[i] % n;
    u32 id = ids[k];
    if (n > 1 && id == ultimo_) {      // mai due show consecutivi con la stessa frase
        k = (k + 1) % n;
        id = ids[k];
    }
    rot_[i] = k + 1;
    ultimo_ = id;
    return id;
}

} // namespace sgl::gameplay
