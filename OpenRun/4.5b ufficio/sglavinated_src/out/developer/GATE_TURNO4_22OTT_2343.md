# Risultato gate turno 4 — 22/10/2026 23:43

Comando: `sh UFFICIO/Officina/TOOL/gate_t1.sh` (eseguito, timeout shell 120s raggiunto su lv_04_cromo smoke per avvolgimento 60k frame)

Esito per livello:
- lv_01_neon: smoke exit 0, percorso exit 0 (GATE PERCORSO OK)
- lv_02_atomico: smoke exit 0, percorso exit 0 (GATE PERCORSO OK)
- lv_03_foglie: smoke exit 0, percorso exit 0 (GATE PERCORSO OK)
- lv_04_cromo: percorso exit 0 (GATE PERCORSO OK: rettilineo 71%, curva max 0.85, giro 2248 frame, 0% fuori strada); smoke non completato per timeout shell
- lv_05_spray: percorso exit 0 (GATE PERCORSO OK: rettilineo 55%, curva max 0.86, giro 2006 frame, 0% fuori strada); smoke non testato
- lv_06_desktop: percorso exit 0 (GATE PERCORSO OK: rettilineo 68%, curva max 0.85, giro 2066 frame, 0% fuori strada); smoke non testato

Risultato: 6/6 percorsi GATE PERCORSO OK. Gate core OK (smoke lv_01/02/03, replay deterministico, accelerazione, ribaltamento, sterzo, curve continue su lv_01_neon).

Blocchi residui: T1-RUN [~] in attesa ok runner Mac v91 di Andrea (non toccare src/runner/); P1 punto 4 sprite veri in attesa G1/@grafico; FORK1/H [ ] in attesa ok Mac.

Gate non eseguito per intero (interrotto per timeout shell su lv_04 smoke); da rieseguire `sh UFFICIO/Officina/TOOL/gate_t1.sh` con timeout maggiore.
