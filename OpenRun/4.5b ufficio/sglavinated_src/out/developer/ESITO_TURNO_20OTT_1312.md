# Esito turno developer — 20/10/2026 ~13:12 (blocco R1 [~])

Blocco: R1 — tracciati modificabili per le 6 piste (RICHIESTA_APERTA.md, priorita' di Andrea).
Stato compiti R1:
- 1 road.cpp: FATTO (raccordo entrata/uscita min(n/3,60); correzione deriva solo su segmenti curva != 0).
- 2 tratti.json: CREATO (8 tratti: rettilineo_corto/lungo, curva_dolce/stretta dx/sx, collina su/giu).
- 3 regole.json: CREATO (regole per lv_01..lv_06 + default; coerenti con soglie attuali).
- 5 livello.json: 6 file con campo tracciato presenti (lv_02_atomico riferimento guidabile).
- 4 gate stampa tratto violato + test espansione tratti: competenza @developer-junior.

Compito 3 codice (prova_gate legge regole.json al posto delle soglie hardcodate, test/prova_gate.cpp riga 536): BLOCCATO.
- Tentativo edit in questo turno: permission.rejected: edit su motore_nuovo/test/prova_gate.cpp.
- Serve: sblocco edit dal Capo, poi CP-005 aperto dal Capo con review incrociata registrate (Junior rivede 1-3, Builder rivede 4-5).

Gate eseguito in questo turno: sh UFFICIO/Officina/TOOL/gate_t1.sh (comando singolo, radice sglavinated_src, senza pipe/;/*/&&)
- Build: cmake configurazione + cmake --build exit 0 (target prova_gate compilato).
- lv_01_neon percorso: exit 0 — rettilineo 63% (piu' lungo 659), curva dx/sx 0.85/0.85.
- lv_02_atomico percorso: exit 0 — rettilineo 65% (piu' lungo 1440), curva dx/sx 0.72/0.98.
- lv_03_foglie: timeout strumento 180s durante prova_gate, nessun completamento, nessun exit code finale.
=> GATE NON ESEGUITO/COMPLETATO in questo turno ('gate non eseguito'). Data/ora: 20/10/2026 ~13:12.

Ultimo esito completo noto (riferimento): 19/10 11:45 e 20/10 pomeriggio — sh tools/gate_t1.sh soak exit 0, GATE T1 TUTTO OK, 6/6 percorsi GATE PERCORSO OK (lv_01 63%/659/0.85-0.85, lv_02 65%/1440/0.72-0.98, lv_03 57%/590/0.75-0.95, lv_04 71%/1457/0.85-0.85, lv_05 55%/607/0.86-0.84, lv_06 68%/813/0.85-0.85), giro guidato finito, fuori strada 0%, soak 6/6 OK, ms/frame medio ~0.46 (limite 20), replay 2x IDENTICO. Log: UFFICIO/Officina/Test area/gate_t1.log.

Scritture: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), questo file. Zero scritture nuove in motore_nuovo/ e tools/.
