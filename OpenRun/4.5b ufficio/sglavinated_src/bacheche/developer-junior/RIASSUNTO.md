# RIASSUNTO @developer-junior
Riscritto da Claude 08/11 (Andrea: ripartire puliti). Unico lavoro: BLOCCO PERC1 di RICHIESTA.md, un passo per turno, senza aspettare nessuno.
- Ruolo: Developer Junior (4.6); percorsi test in motore_nuovo/test/, gate in UFFICIO/Officina/TOOL/
- Stato verificato: gate_t1.sh soak exit 0, T1: TUTTO OK; PERCORSO OK su tutti i 6 livelli inclusi giro guidato; replay 2x IDENTICO; accelerazione/ribaltamento/sterzo/curva/gara/frequenza OK.
- Blocchi: PIST1 [x]; AUTO1 [~] (regole bloccate Andrea); ENG1+ARC1 [x]; KO1 [x]; T1-RUN [x].
- PERC1: J1 [ ] gate tratti: varietà (>= 7 tipi per livello, niente tipo ripetuto di fila) · J2 [ ] gate traffico autostrada > ponte (dopo D2) · J3 [ ] gate galleria + screenshot (dopo D4) · J4 [ ] review breve in out/developer-junior/REVIEW_PERC1.md dopo ogni passo D.
- Test in `motore_nuovo/test/prova_gate.cpp`; ctest verde dopo ogni modifica; segna qui `PERC1 Jn [x]` con comando ed exit code.
- Mai: HUD, `gate_t1.sh` (già a posto), `salvaScreenshotP1`.
- Prossimo: work UNO per turno, ctest verde; fuori auto giocatore/premiazione/musica urti.
- RIV: gate urto in attesa @developer (bump leggero, nessun ribaltamento, speed > 0, controlli 1.5 s).
