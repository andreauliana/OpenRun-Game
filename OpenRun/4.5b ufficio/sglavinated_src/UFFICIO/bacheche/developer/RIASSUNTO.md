# Riassunto developer — 09/11/2026 12:59
- Stato blocchi: MIG1 D1-D5 [x], V47 V1-V4 [x], V47 V5 [~] bloccato.
- V47 V5 (testi schermo inglese semplice): parte codice/dati @developer coperta (`src/game/fasi.cpp` aiuti in inglese; `partenza/premiazione/game` PAUSE gia' in inglese). Verifica 12:59: nessun testo UI italiano trovato in `src/` (drawTesto/drawTitolo).
- Blocco residuo V5: `motore_nuovo/dati/frasi.json` (156 frasi Gianni/Gesu, italiano) e' dato @grafico (`frasi.json`, `dati/testi`) — write fuori perimetro @developer; serve @grafico/@capo.
- Ultimi comandi registrati (turni precedenti): `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 ctest 27/27 (11:23); `gate_t1.sh` non eseguito da allora. Questo turno: gate non eseguito.
- Perimetri fissi: no HUD, auto/pose, musica, tracciati, `src/runner/`, sterzo/curve/derapata/incidenti/tempi; solo costanti MIG1 aperte quando serve. gate_t1.sh exit 0 dopo codice/dati.
- Prossimo azionabile @developer: nessuno finche' V5 resta su frasi.json; passare solo se @capo apre blocco @developer indipendente.