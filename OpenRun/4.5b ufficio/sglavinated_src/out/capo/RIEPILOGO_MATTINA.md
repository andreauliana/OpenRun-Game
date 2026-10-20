# RIEPILOGO_MATTINA — OpenRun 4.6 (2026-10-20)

## Fatto per blocco / agente (20/10)
- @developer (R1 + T1-RUN): road.h/road.cpp (raccordo entrata/uscita, deriva solo curve != 0), game.cpp campo `tracciato` su 6 livello.json, tratti.json (8 tratti) e regole.json creati. `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0, GATE T1 TUTTO OK, 6/6 percorsi OK, ms/frame ~0.18-0.20 (< 2), replay 2x IDENTICO. Log: UFFICIO/Officina/Test area/gate_t1.log.
- @developer-junior: prova_gate legge regole.json, test espansione OK 6/6. Peer review VOCE1175 positiva 19/10. Compito 3 BLOCCATO: edit negato su motore_nuovo/test/prova_gate.cpp.
- @grafico (Designer): G1 attivato (T1-Q4). Inventario CP-002 verificato: catalogo 52 voci (30 approvate, 16 pronte, 2 costruzione, 4 mancanti), 100 PNG non catalogati. Mancano sprite concorrenti/personaggi e 3 finali animati.

## Non funziona / blocchi aperti
- CP-005 APERTO: gate verde ma review incrociata indipendente mancante (sviluppatore-junior → sviluppatore punti 1-3; sviluppatore → sviluppatore-junior punti 4-5); ctest (7 test) da rieseguire e registrare.
- v91 Andrea su runner Mac SDL2 non rieseguito dopo il tuning 20/10 (ultimo esito 09:39: GATE T1 FALLITO exit 13, giro guidato 5/6). Blocca D12 e Pi 400.
- libretro.h manca → BUILD_LIBRETRO=ON non compila; nessun lavoro Pi 400/RetroPie finché Andrea non dà ok su Mac (ordine Andrea).
- SDL2_image non installabile: runner senza caricamento PNG (D10: conversione PNG->raw offline, zero nuove dipendenze).

## Decisioni aperte in ANDREA_REVIEW.md (top 3)
1. D12 [APERTA, risposta A = sì]: Andrea ri-verifica runner su Mac dopo R1 verde e gate 6/6, prima di Pi 400/RetroPie.
2. Frequenza di gioco (19 ott): 60 Hz default + opzione 50 Hz; tutte le misure in secondi (dt), non frame. Checkpoint dedicato dopo R1, prima di R2.
3. T1-Q5 / R1: logica tracciati consecutivi per la gara, soglie giro guidato non abbassate, tuning un parametro alla volta; chiudere CP-005 con review indipendente.

## Prossimo passo consigliato
Registrare la review incrociata indipendente su CP-005, poi riconfermare gate soak + ctest e chiedere ad Andrea la ri-verifica Mac (D12). Solo dopo: chiudere T1-RUN [x] e attivare G1 (catalogo 100 PNG).
