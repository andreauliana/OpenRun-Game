# Esito blocco P1b — Tappa 1: niente collisioni fantasma (@developer, 17 ott 2026 07:15-07:30)

Stato RICHIESTA.md: P1b [~] (in corso fino a build/replay reale su Pi 400, blocco H).
Versione: v90 (prima consegna del turno; numero riportato qui e in voce SCAMBIO 70 da trascrivere da @capo).

File modificati (solo motore_nuovo/src/, tutti SPDX MIT riga 1, nessun file cancellato, nessun asset/tracciato Sega, nessun tocco di ../):

1. motore_nuovo/src/player/player.h — aggiunta `static constexpr f32 X_LIMIT = 900.0f;` (clamp laterale playerX).
2. motore_nuovo/src/player/player.cpp — dopo lo sterzo, `playerX` clampato a [-X_LIMIT, X_LIMIT]. Prima: dopo 60 frame di sterzata a destra playerX = 125 e oltre e l'auto spariva dallo schermo.
3. motore_nuovo/src/render/renderer.cpp — `drawPlayerCar` usa `cx = 160 + offsetX * 0.15f` (fattore coerente con `road.project`, road.cpp:87); prima era `* 4.0f`, che portava cx fuori dai 320 px quasi subito. Con clamp +/-900: cx in [25, 295], auto (larghezza 34 px) sempre interamente visibile.
4. motore_nuovo/src/game/game.h — nuovo membro `bool trafficoAttivo_ = false;` (P1b: placeholder traffico disattivato di default).
5. motore_nuovo/src/game/game.cpp — `spawnTraffic()` e loop collisioni (`|dx| < 700`) eseguiti solo se `trafficoAttivo_` (default false): niente crash infinito sterzando oltre x = 100, resta solo strada + auto. `spawnTraffic()` e logica collisione restano intatte per T5 (BLOCCO T5, stato [-], non avviato).

P1 punti 1-2 (Pad::A/B accelerazione/freno in player.cpp:15-16; `find_package(SDL2_image CONFIG QUIET)` in CMakeLists.txt:56) gia' su disco da voci precedenti (66/67). P1 punti 3-5 (segmenti da livello.json, sprite veri, show con fumetto) dipendono da P2 (@livelli) e P3 (@traduttore): non avviati per FEEDBACK_ANDREA.md (priorita' assoluta al primo Gate C++ giocabile, squadra coder+debugger, niente tappe future).

Gate P1b non verificabile qui: prova headless/runner (sterzare a sx/dx per 10 s, auto visibile, nessun crash) e build cmake richiedono compilatore (shell negata, regola 21). Rimandato a @debugger e Pi 400 con toolchain (Andrea, regola 9/21).

Voce SCAMBIO 70 (CODER -> CAPO [CHIUSA]) da trascrivere in SCAMBIO.md da @capo (scrittura negata a @developer/@developer-junior, regola 15/21).
