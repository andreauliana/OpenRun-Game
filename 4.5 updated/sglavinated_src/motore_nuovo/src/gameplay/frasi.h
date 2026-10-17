// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

// Tabelle degli id frase e selettore a rotazione.
// Copia di lavoro dei dati v87 in motore_nuovo/dati/frasi.json (CC-BY-4.0):
// qui ci sono SOLO indici, nessun testo. I testi restano in frasi.json,
// caricati dal runner/asset quando servono al renderer.
// Regole: SPEC_GAMEPLAY §6. Le frasi di escalation stanno in una sola tabella
// (mai mescolate con ph_tab/jtab), come nella v87.

namespace sgl::gameplay::tabelle {

// Gianni — uscita di strada (v87 "otab", 32 entrate, cicliche).
extern const u32 otab[32];
// Gesu' — crash, livello 0 (v87 "xtab").
extern const u32 xtab[32];
// Gianni — show periodico, livello 0 (v87 "ph_tab").
extern const u32 ptab[32];
// Gesu' — show periodico quando la rampa lo sceglie, livello 0 (v87 "jtab").
extern const u32 jtab[32];
// Gianni — inattivita' 10 s (v87 "itab", id esclusivi).
extern const u32 itab[4];
// Escalation Gianni L1/L2/L3 (v87 g1t/g2t/g3t, 6 per livello).
extern const u32 g1t[6];
extern const u32 g2t[6];
extern const u32 g3t[6];
// Escalation Gesu' L1/L2/L3 (v87 j1t/j2t/j3t); j-t sono anche le frasi di
// crash dai livelli 1 in su.
extern const u32 j1t[6];
extern const u32 j2t[6];
extern const u32 j3t[6];

} // namespace sgl::gameplay::tabelle

namespace sgl::gameplay {

// Range personaggio (frasi.json, invariati dalla v87): servono per i controlli.
bool eGianni(u32 id);
bool eGesu(u32 id);

// Controlla che ogni tabella contenga solo id del personaggio giusto.
bool verificaTabelle();

// Identita' della tabella: il selettore tiene un contatore di rotazione per ciascuna
// (come il VAR178 della v87), cosi' ogni tabella avanza con il suo passo.
enum class TabId : u8 {
    Otab = 0, Xtab, Ph, J, Itab, G1, G2, G3, J1, J2, J3, Conta
};

// Selezionatore a rotazione con divieto di ripetizione:
// stesso id due show di fila = gate fallito nella v77 (repeat 2-3), quindi se
// l'uscita corrente coincide con l'ultima frase mostrata si avanza di uno.
class SelezioneFrasi {
public:
    // Il seed spiazza la fase iniziale delle rotazioni: stesso seed = stessi show.
    void reset(u32 seed = 0) {
        for (u32 i = 0; i < kNumTabelle; ++i) rot_[i] = seed % 251u + i;
        ultimo_ = 0xFFFFFFFFu;
    }

    u32 scegli(TabId tab, const u32* ids, u32 n);
    u32 ultimoId() const { return ultimo_; }

private:
    static constexpr u32 kNumTabelle = static_cast<u32>(TabId::Conta);
    u32 rot_[kNumTabelle] = {};
    u32 ultimo_ = 0xFFFFFFFFu; // nessuna frase mostrata
};

} // namespace sgl::gameplay
