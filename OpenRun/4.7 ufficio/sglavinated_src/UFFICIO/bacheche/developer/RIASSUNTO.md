# RIASSUNTO developer (max 12 righe)
- Data/ora: 21/11/2026 08:30 (Turno 1).
- Blocco: V9 ∞ fluidita' lv_04_cromo (ventiseiesimo giro).
- File codice: motore_nuovo/src/render/renderer.cpp (drawTerraSospesa ramo spiaggia: soglia Bayer `((BAYER4+0.5f)/16.0f) < sfuma` con `sfuma` invariante per fetta -> precalcolata `sogliaBayerSfuma = sfuma*16` e confronto `(BAYER4+0.5f) < sogliaBayerSfuma`; divisione e prodotto per potenze di 2 esatti -> stessa resa, -1 divisione per pixel).
- Workspace: cartella rinominata 4.5b->4.7 ufficio a meta' turno; sessione spostata, prepara_build.sh ha accantonato la build con cache a vecchi path e ricreato UFFICIO/Officina/build.
- Build: ricompilata da gate_t1.sh (cmake -S/-B + --target prova_gate) -> exit 0.
- Smoke: prova_gate lv_04_cromo smoke exit 0 "GATE OK" medio 0.863 max 3.589 ms (frame 0), replay 2x IDENTICO, frameHash 0 4118572995 = riferimento storico turno 58 (resa invariata).
- Percorso: prova_gate lv_04_cromo percorso exit 0 "GATE PERCORSO OK" (2125 frame, 35.4 s, 0% fuori).
- Gate: sh UFFICIO/Officina/TOOL/gate_t1.sh eseguito 21/11/2026 08:30 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK".
- Esito: frameHash invariato (4118572995), replay IDENTICO, nessuna regressione; medio ~0.86 ms (< 8 ms, Pi 400 <= 12 ms). Prossimo V9 ∞ lv_05_spray (ventiseiesimo giro).
