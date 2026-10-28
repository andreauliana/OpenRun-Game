# Riassunto developer — 27/10/2026 turno 1 (22:38)
- R2 [x] completo (passo 1d nome 3 lettere nei Risultati, gate ciclo verde 27/10 18:44).
- RIV [x] a/b/c verificati (bump -15% senza ribaltamento, sprite rivali da catalogo, comportamento realistico in curva) 27/10 19:41-21:57.
- DECO1 [x] completo (2a-2e, ultimo gate 07:55 exit 0, 6/6 percorsi OK).
- PIST1 [ ] task 1-2 fatte (@developer): 6 nuovi tratti in tratti.json (chicane_dx/sx, ponte, autostrada, citta, valle) + tracciato su 6 livello.json; larghezza segmento supportata dal core (road.cpp raccordo senza scatti, clamp 1.0-2.0). Task 3 (@grafico oggetti) e 4 (@developer-junior gate tratti) posticipate.
- FIN [ ] compito 1 (@developer) fatto: fase Finale tra arrivo e Risultati (dati/finali.json), solo su arrivo (tempo scaduto salta il finale); cmake --build prova_gate exit 0, prova_gate ciclo GATE OK (sequenza preservata, determinismo/hash identici). Compito 2 @grafico e 3 @developer-junior posticipate.
- T1-RUN [~] e H [~] in attesa Andrea (D19=B, ok runner Mac v91, poi fps/temperatura Pi 400); nessun lavoro Pi 400/RetroPie prima di ok Mac. FORK1 [~] attende CP-007 @capo. P1 [~] punto 4 in attesa @grafico.
- Scritture: motore_nuovo/src/game/fasi.{h,cpp}, motore_nuovo/src/game/game.h, motore_nuovo/dati/finali.json, motore_nuovo/dati/tracciati/tratti.json, 6 motore_nuovo/dati/livelli/*/livello.json, diario.md, RIASSUNTO.md, out/developer/ESITO_FIN_FASE_27OTT.md.
