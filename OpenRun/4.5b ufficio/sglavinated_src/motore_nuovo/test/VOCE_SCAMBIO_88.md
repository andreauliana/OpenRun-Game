### 88. @developer/@developer-junior -> CAPO | stato [APERTA] | 17 ott 2026 09:58
Richiesta/Esito (Turno 1): BLOCCO H [~] — verifica rilievi reviewer CHECKPOINT 2 (09:09) sul codice corrente. Tutti e 3 già sanati da @developer/@developer-junior (voce 87) e confermati per lettura (nessun file prodotto toccato, regola 15):
1) offroad: player.h:33 OFFROAD_LIMIT=700.0f < X_LIMIT=900.0f; player.cpp:25 offroad calcolato prima del clamp (linee 30-31), nessun duplicato (linea 29 commento).
2) wiring show: game.cpp:305 disegnoShow_.aggiorna() in update(); init() testiFrasi_.carica/verifica/setTesti/layout; render() disegna scena() dopo drawPlayerCar.
3) frameHash (game.cpp:374-377): playerX, playerZ, playerOffroad, rng_.state() già inclusi.
4) Rilievo A build: CMakeLists.txt 48-52 già elenca disegno_show.* e testi_frias.*.
Sintassi nucleo: OK 34/34 (già voci 76/78). Build cmake, replay deterministico (stesso input 2x -> stesso hash ogni 60 frame), ms/frame <=20 ms (obiettivo <=12), fps 50, temperatura: NON eseguibili qui (shell negata regola 21, cmake assente) -> Pi 400 con toolchain Andrea.
Diff proposti (NON applicati, spetta a @developer/@developer-junior): target CMake sglavinated_prova_gate (10 s headless, 3 screenshot in out/debugger/, log velocita' + posizione, per gate FEEDBACK_ANDREA) + replay I/O su file (--record/--replay/--frames) per test deterministico. Esito in motore_nuovo/test/ESITO_VERIFICA_RILIEVI_TURNO1.md e out/debugger/RIEPILOGO_TURNO1_17OTT_0958.md. Attesa Pi 400 per chiudere H -> [x].
@capo: copiare questa voce in fondo a SCAMBIO.md (edit diretto negato a @developer/@developer-junior, regola 21).
