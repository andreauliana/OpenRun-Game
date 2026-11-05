# Esito blocco DECO1 passo 2d — 27/10/2026 turno 1 (@developer)

Blocco: DECO1 [x] passo 2d — densita' cartelloni 20-30% dei segmenti (D2b).

Modifiche:
- motore_nuovo/src/game/game.h: firma leggiCartelloniDaFile estesa con std::vector<bool>& segmentoOccupato (segna i segmenti con cartellone esplicito da livello.json).
- motore_nuovo/src/game/game.cpp: init() posiziona i cartelloni espliciti del file e riempie i segmenti ancora liberi in modo deterministico (Rng(seed_), 1 segmento su 4 -> ~25% totale, dentro il 20-30% di D2b), zero alloc per frame, nessun sovrapposizione con le posizioni esplicite; stampa di diagnostica della densita' in init.

Comandi ed esiti (tutti da sglavinated_src, senza pipe/comandi composti):
- cmake --build UFFICIO/Officina/build --target prova_gate -> exit 0
- sh UFFICIO/Officina/TOOL/gate_t1.sh -> exit 0, GATE T1: TUTTO OK
- sh UFFICIO/Officina/TOOL/run_tests.sh -> ctest 21/21 verde (gate decorazioni incluso)

Misure (log UFFICIO/Officina/Test area/gate_t1.log):
- densita' cartelloni: lv_01_neon 26.8% (1141), lv_02_atomico 26.4% (1492), lv_03_foglie 27.0% (1268), lv_04_cromo 26.7% (1414), lv_05_spray 26.6% (1249), lv_06_desktop 26.6% (1288)
- ms/frame medio <= 0.65, max 2.696 (limite 20); rettilineo 55-71%; curva max 0.72-0.98; fuori strada 0%; replay 2x IDENTICO
- gara 4 combinazioni OK; frequenza 60 Hz e 50 Hz OK; screenshot P1 OK

Residui: DECO1 2e (parallasse leggera in curva), poi R2. T1-RUN [~] e H [~] in attesa Andrea (runner Mac v91, poi fps/temperatura Pi 400); nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac. FORK1 [~] attende CP-007 @capo (review incrociate gia' positive).
