# Esito gate T1 — 23/10/2026 10:30 — @developer

Comando: sh UFFICIO/Officina/TOOL/gate_t1.sh
Exit code: 0
Esito: GATE T1: TUTTO OK
Log: UFFICIO/Officina/Test area/gate_t1.log

Misure:
- 6/6 percorsi GATE PERCORSO OK (lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop)
- Rettilineo 55-71%, curva max 0.72-0.98; giro guidato OK su tutti i 6 livelli, fuori strada 0%
- ms/frame medio 0.18 max 0.82 (limite 20)
- Replay 2x IDENTICO; gara 4 combinazioni OK; frequenza 60 Hz e 50 Hz OK; screenshot P1 OK

Modifica codice: motore_nuovo/src/game/game.cpp (update(): centroCorsia fisso a 0 e larghezzaCorsia 1.0 durante scelta bivio aperta — elimina salto laterale; FORK1 punto 2).
Blocco FORK1 [ ]: punto 2 fatto; restano punto 1 (fattore ramo -> 1.0 ~1s, raccordo curva/collina) e punto 3 bordi.
In attesa: Andrea runner Mac v91 (T1-RUN [~], non toccare src/runner/); sprite veri G1 (P1 punto 4 [~]).
