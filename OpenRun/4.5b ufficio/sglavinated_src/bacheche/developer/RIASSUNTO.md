# Riassunto developer — 05/11/2026 20:23 (turno 2)
- P1 punto 4 fatto: sprite vero auto giocatore (auto_giocatore.sglraw 64x32, catalogo approvato D5); drawPlayerCar disegna il sprite vero, rettangolo sintetico come fallback.
- P3 fatto: parapetti ponte come oggetti decorativi (parapetto.sglraw, ogni ~400 unita' sui segmenti con acqua, lv_01/lv_04/lv_06; G-P1 del @grafico consegnato 19:56).
- RIV punto 1 fatto: percorso proprio per ogni rivale da percorso_gara.json (seme fisso, deterministico), rivali visibili solo sulla tappa del giocatore, tempo d'arrivo registrato, calcolaClassifica esclude i non arrivati entro il tempo limite.
- RIV punto 3: in attesa del @grafico (fotogrammi sterzo/sbandata auto giocatore; Gianni/Gesu/UFO senza PNG).
- PIST1 J1 e J2: in attesa del @developer-junior. H e T1-RUN: in attesa di Andrea (runner Mac v91, Pi 400).
- gate non eseguito questo turno; ultimo esito noto: gate_t1.sh exit 0 alle 20:15, 6/6 GATE PERCORSO OK, ms/frame max 3.13 (limite 20), replay 2x IDENTICO, gara 4/4 IDENTICO, frequenze 60/50 Hz OK.
