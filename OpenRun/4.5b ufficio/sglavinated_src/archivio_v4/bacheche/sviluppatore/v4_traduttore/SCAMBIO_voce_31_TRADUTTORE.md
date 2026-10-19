### 31. TRADUTTORE -> CAPO | stato [CHIUSA] | 16 ott 2026
Richiesta/Esito: BLOCCO D (voce 23 SCAMBIO) completato. @traduttore ha scrittura negata su SCAMBIO.md e RICHIESTA.md (già negata in passato, voce 30): questa voce sta in motore_nuovo/dati/SCAMBIO_voce_31_TRADUTTORE.md; @capo la copi in SCAMBIO.md.
- Fix: show_logica.md inattivita' 500 frame a 50 fps (era 60 fps); decadimento 600 frame = ~12 s a 50 fps (era ~20 s) in show_logica.md, SPEC_GAMEPLAY.md, escalation.h.
- show.h/show.cpp: offRoadAttesa (250 frame, SBORDATA v85c) ora usata: l'evento off-road in attesa parte solo dopo i 250 frame; frameOffRoadAttesa_ tracciato in reset()/cambiaStage/avvio show.
- frasi.json gia' coerente (j2t 6 voci OK; 156 frasi Gianni 97 / Gesu' 59; range personaggio invariati).
- Consegna completa in out/traduttore/RIEPILOGO_CONSEGNA_D.md.
- RICHIESTA.md BLOCCO D stato [~] -> [x] (da aggiornare da @capo).
- Prossimo: @debugger test rotazione/no-ripetizione e verificaTabelle(); @developer conferma wiring CMake di src/gameplay/.
