# Riassunto developer — 27/10/2026 turno 1 (19:41)
- R2 [~] RIV punto a (sviluppatore) fatto: urto con avversario = bump (spinta laterale + rallentamento -15%, nessun ribaltamento, controlli attivi); costanti in dati/config.json (bump_rallenta 0.85, bump_durata_s 1.0, bump_spinta_laterale 200.0); ribaltamento solo per fuori strada prolungato.
- Build cmake --build UFFICIO/Officina/build --target prova_gate exit 0.
- sh UFFICIO/Officina/TOOL/run_tests.sh ctest 22/22 verde (713s): gate_crash, gate_gara, gate_ciclo, 6 livelli smoke+percorso OK; gate_t1.sh non eseguito (ultimo gate 27/10/2026 19:11 exit 0, 6/6 percorsi OK).
- RIV prossimi: punti b (sprite rivali da catalogo, @grafico) e c (comportamento realistico in aggiornaRivali), poi PIST1 (tipi tratto: chicane, ponte, autostrada, citta').
- T1-RUN [~] e H [~] in attesa Andrea (ok runner Mac v91, poi fps/temperatura Pi 400); nessun lavoro Pi 400/RetroPie prima di ok Mac. FORK1 [~] attende CP-007 di @capo.
- Scritture: motore_nuovo/dati/config.json, motore_nuovo/src/game/game.h, motore_nuovo/src/game/game.cpp, diario.md, RIASSUNTO.md, out/developer/ESITO_RIV_PUNTOA_27OTT.md.
