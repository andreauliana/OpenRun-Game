# out/traduttore — consegna BLOCCO D (@traduttore)

Stato: [x] (consegnato, turno 1 / 16 ott 2026). Agente: @traduttore.
Cartelle: motore_nuovo/dati/, motore_nuovo/src/gameplay/.

## Contenuto (blocco D — SPEC_GAMEPLAY + modulo gameplay)
- motore_nuovo/dati/SPEC_GAMEPLAY.md (spec gameplay: compagni di viaggio Gianni/Gesu', trigger e priorita', escalation 0-3 con decadimento a tempo, inattivita' 10 s, interfaccia verso il nucleo; CC-BY-4.0)
- motore_nuovo/dati/show_logica.md (logica di riferimento v85c; CC-BY-4.0)
- motore_nuovo/dati/frasi.json (156 frasi id 0-155: Gianni 97, Gesu' 59; range personaggio, tabelle v87 otab/xtab/ph_tab/jtab/ctab/jtab, escalation g1t-g3t/j1t-j3t, pr_tab v74 + nota v77; CC-BY-4.0)
- motore_nuovo/dati/personaggi/{gianni,gesu}/fumetti.json + manifest.json (CC-BY-4.0, crediti Andrea Uliana)
- motore_nuovo/src/gameplay/{manifest.json, personaggi.h, escalation.h, frasi.h/.cpp, ufo.h, show.h/.cpp} (MIT, C++17, zero SDL, zero codice 68000, zero asset Sega)
- motore_nuovo/docs/CATALOGO_PERSONAGGI.md (righe gianni/gesu/fumetti: stato "nuovo")

## Fix di questo turno
- show_logica.md: inattivita' 500 frame a 50 fps (non 60); decadimento 600 frame = ~12 s (non 20 s).
- SPEC_GAMEPLAY.md: decadimento ~12 s puliti a 50 fps.
- escalation.h: commento decadimento corretto a 12 s.
- show.h/show.cpp: offRoadAttesa (250 frame) ora usata: l'evento off-road in attesa (SBORDATA v85c) si serve solo dopo i 250 frame di attesa; frameOffRoadAttesa_ tracciato in reset()/cambiaStage/avvio.

## Note aperte (per @capo/@coder/@debugger)
- pr_tab: il modulo usa default [200,180,165,150,140] (v77+, HANDOVER §0); frasi.json/show_logica.md riportano ancora i valori v74 [500,440,400,360,320]. Dato in ShowConfig::prTab: decidere se allineare i due documenti.
- @coder: aggiungere src/gameplay/*.h/.cpp a sglavinated_core in CMakeLists.txt (già fatto nel rifinitura round 2 del blocco C per escalation.h, frasi.h/cpp, personaggi.h, show.h/cpp, ufo.h; confermare completezza).
- Build/test rotazione e no-ripetizione non eseguibili qui (nessun compilatore, regola 21): rimandati a @debugger (blocco H) e Pi 400.
- Nessun codice 68000, nessun asset/grafica/audio Sega; limiti hardware della v87 esclusi (SPEC_GAMEPLAY §8).
