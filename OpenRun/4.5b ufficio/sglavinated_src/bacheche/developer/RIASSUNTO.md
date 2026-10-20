# Riassunto developer — 20/10 turno 1 (R1 [~])
Blocco attivo: R1 (tracciati 6 piste), stato [~].
R1-1 road.cpp: FATTO (raccordo entrata/uscita min(n/3,60), deriva solo segmenti curva != 0).
R1-2/R1-3: motore_nuovo/dati/tracciati/tratti.json (8 tratti) e regole.json CREATI; 6 livello.json con campo tracciato attivo (loader espande da tratti.json, fallback su segmenti).
Gate 20/10 13:12: sh UFFICIO/Officina/TOOL/gate_t1.sh — build exit 0, lv_01/lv_02 percorso exit 0, timeout 180s su lv_03 senza exit code finale => gate non eseguito in questo turno.
Ultimo gate completo noto verde: 19/10 11:45 e 20/10 pomeriggio — 6/6 percorsi GATE PERCORSO OK, soak 6/6 OK, replay 2x IDENTICO, ms/frame medio ~0.46 <= 20 (log Test area/gate_t1.log).
Compito 3 prova_gate legge regole.json (no soglie hardcodate, return riga 536): BLOCCATO, edit negato su motore_nuovo/test/prova_gate.cpp; serve sblocco edit + CP-005 al Capo.
Scritture: bacheche/developer/, out/developer/.
