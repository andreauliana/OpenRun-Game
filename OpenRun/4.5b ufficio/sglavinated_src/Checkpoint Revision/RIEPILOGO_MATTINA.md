# RIEPILOGO_MATTINA — OpenRun 4.6
Aggiornamento: 2026-10-20 | Fonti: STATO_CORRENTE.md, REGISTRO_REVISIONI.md, ANDREA_REVIEW.md, STATUS_OFFICINA.md, STATUS_GRAFICO_PER_CAPO.md

## Fatto (blocco/agente)
- @developer (builder-developer): R1 tracciati — road.h/road.cpp (raccordo entrata/uscita, curva costante, correzione direzione solo su curve != 0) + game.cpp campo `tracciato` sui 6 livello.json; `sh tools/gate_t1.sh soak` exit 0, GATE T1 TUTTO OK, 6/6 percorsi verdi (rettilineo 52-65%, curva max 0.37-0.88, fuori strada 0%, ms/frame medio 0.187 <= 20, replay 2x IDENTICO). Log: UFFICIO/Officina/Test area/gate_t1.log. Consegne out/developer/VOCE_SCAMBIO_1174.md e _1175.md.
- @developer-junior: peer review indipendente T1-RUN/VOCE_SCAMBIO_1175: POSITIVO (6/6 criteri), gate riconfermato exit 0. Solo letture e consegne di turno, nessun codice.
- @grafico: inventario catalogo verificato (52 voci: 30 approvate, 16 pronte, 2 in costruzione, 4 mancanti; 100 PNG non catalogati segnalati). Fumetti testi completi (156). Mancano sprite concorrenti/personaggi (Gianni, Gesù, UFO), 3 finali animati e 6 percorsi distinti (attivi solo 3 temi).

## Non funziona / blocchi
- CP-005 (R1) aperto: gate verde registrato ma nessuna review indipendente che chiuda il dossier; road.h/road.cpp/game.cpp da recensire col perimetro R1.
- libretro.h mancante -> `BUILD_LIBRETRO=ON` non compila (Pi 400/RetroPie bloccato).
- SDL2_image non installabile qui (conflitti Homebrew libtool) -> runner senza caricamento PNG (D10: conversione PNG->raw offline).
- v91 Andrea su runner Mac SDL2 + fps/temperatura Pi 400 non eseguiti (blocco H, T1-Q3).
- CP-002, CP-003, CP-004 in attesa di review indipendente assegnata.

## Decisioni aperte in ANDREA_REVIEW.md (top 3)
1. D12 / T1-Q3: Andrea ri-verifica runner su Mac (v91, SDL2) dopo R1 verde, prima dei test Pi 400/RetroPie (risposta A: sì).
2. Feedback 19 ott: 60 Hz predefinito + opzione 50 Hz, misure in secondi (non frame); checkpoint dedicato da aprire dopo R1, prima di R2 (developer autore, junior revisore).
3. T1-Q5 / R1: logica tracciati consecutivi per la gara, soglie giro guidato non abbassate, tuning un parametro alla volta; chiudere CP-005 con review indipendente.

## Prossimo passo consigliato
Assegnare al developer-junior la review indipendente di CP-005 (diff road.cpp/tratti.json/regole.json + gate soak exit 0); se positiva, chiudere CP-005 e aprire il checkpoint 60 Hz. Poi attendere v91 di Andrea su Mac prima di qualsiasi lavoro Pi 400.
