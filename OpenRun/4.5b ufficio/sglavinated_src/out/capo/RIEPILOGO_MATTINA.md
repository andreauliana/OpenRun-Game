# RIEPILOGO MATTINA — OpenRun 4.6
Data: 2026-10-20. Fonte: STATO_CURRENTE.md, REGISTRO_REVISIONI.md, ANDREA_REVIEW.md, STATUS_OFFICINA.md, STATUS_GRAFICO_PER_CAPO.md.

## Fatto per blocco/agente
- Capo: STATO_CURRENTE.md e REGISTRO_REVISIONI.md aggiornati; ANDREA_REVIEW.md fonte unica; CP-001 chiuso; CP-002/003/004/005 aperti, in attesa di review indipendente; archivio v4 classificato (126 file, nessuno rimosso).
- Builder/Developer: gate T1 smoke+soak exit 0 (18/10 22:56); 6 livelli OK, replay 2x IDENTICO, ms/frame medio 0.185 max 0.518 (<=20), speed@60=161.79. cmake --build exit 0, runner SDL compila, PNG disabilitato; CTest 7 test scoperti non eseguiti.
- R1 (19/10 11:45): road.h/road.cpp/game.cpp (campo tracciato in 6 livello.json); gate soak exit 0, GATE T1 TUTTO OK, 6/6 percorsi OK (rettilineo 52-65%, curva max 0.37-0.88, fuori strada 0). tratti.json/regole.json pronti ma non scritti in dati/tracciati/.
- Developer/Builder Junior: letture obbligatorie fatte; peer review T1-RUN (VOCE_SCAMBIO_1175) POSITIVA su 6 criteri; nessun codice/test/build proprio.
- Designer/Grafico: catalogo 52 voci (30 approvate, 16 pronte, 2 costruzione, 4 mancanti), 100 PNG non catalogati; 3 livelli presenti, 6 percorsi/3 finali/sprite concorrente e personaggi mancanti; nulla migrato in Shared asset/.

## Non funziona / blocchi
- CP-002/003/004/005: review indipendente non assegnata (autore distinto dal revisore).
- Andrea 09:39: GATE T1 FALLITO exit 13 (giro guidato KO 5/6); fix builder-developer 11:45, ma ri-verifica Mac (D12) ancora in attesa; v91 e fps/temperatura Pi 400 non registrati.
- Pi 400/RetroPie (blocco H): non testato; libretro.h manca, BUILD_LIBRETRO=ON non compila. Solo dopo ok Mac di Andrea.
- SDL2_image non installabile: PNG disabilitato, prova_gate disegna rettangoli (risposta D10: conversione PNG->raw in strumenti_py da verificare).
- CTest 7 test non eseguiti; CMake Tools VS Code senza progetto attivo.

## Decisioni aperte in ANDREA_REVIEW.md (3 piu' importanti)
1. D12 [APERTA]: Andrea ri-verifica runner su Mac (v91, SDL2) dopo R1 + gate verde, prima di Pi 400/RetroPie (risposta A sì, esecuzione pendente; blocca H).
2. Blocco H/T1-Q3: prove Pi 400/RetroPie FPS/temperatura/throttling — solo dopo ok Mac Andrea; libretro.h mancante blocca core libretro.
3. D10/D11 [risposta registrata, verifica tecnica da checkpoint]: conversione PNG->raw zero dipendenze; rallentamento off-road graduale ~0.85 in player.cpp.

## Prossimo passo consigliato
Capo assegna revisori indipendenti a CP-002/003/004/005 (diversi dagli autori); esegue `sh tools/run_tests.sh` registrando comando, exit code e misure CTest; sollecita ad Andrea la ri-verifica Mac (D12) prima di ogni lavoro Pi 400; poi scrittura di dati/tracciati/tratti.json e regole.json.
