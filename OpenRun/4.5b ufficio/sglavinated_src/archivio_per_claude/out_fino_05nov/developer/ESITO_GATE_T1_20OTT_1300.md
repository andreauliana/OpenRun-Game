# Esito gate T1 — 20/10 ~13:00 (developer, blocco R1 [~])

Comando: `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (da radice sglavinated_src/, comando singolo, senza pipe/;/*/&&)
Esito comando: interrotto dal timeout dello strumento (240 s) durante lv_04_cromo, prima del riepilogo finale. Non è un errore di codice/build.

Build: cmake configurazione + cmake --build exit 0. Target prova_gate compilato.
Log: `UFFICIO/Officina/Test area/gate_t1.log`

Risultati parziali (tutti exit 0 fino all'interruzione):
- lv_01_neon: percorso GATE PERCORSO OK — rettilineo 63% (piu' lungo 659), curva max dx 0.85 sx 0.85, giro guidato finito 1527 frame (30.5 s), fuori strada 0 frame (0.0%), ms/frame medio 0.638 max 64.082 (outlier scheduling, limite 20 sul medio), replay 2x IDENTICO.
- lv_02_atomico: percorso GATE PERCORSO OK — rettilineo 65% (piu' lungo 1440), curva max dx 0.72 sx 0.98, giro guidato finito 1999 frame (40.0 s), fuori strada 0 frame (0.0%), ms/frame medio 0.363 max 9.072, replay 2x IDENTICO.
- lv_03_foglie: percorso GATE PERCORSO OK — rettilineo 57% (piu' lungo 590), curva max dx 0.75 sx 0.95, giro guidato finito 1679 frame (33.6 s), fuori strada 0 frame (0.0%), ms/frame medio 0.489 max 12.995, replay 2x IDENTICO.

Ultimo esito COMPLETO noto (tutte e 6 le piste verdi):
- 19/10 11:45 e 20/10 pomeriggio: `sh tools/gate_t1.sh soak` exit 0, `GATE T1: TUTTO OK`, 6/6 percorsi GATE PERCORSO OK (lv_01 63%/659/0.85-0.85, lv_02 65%/1440/0.72-0.98, lv_03 57%/590/0.75-0.95, lv_04 71%/1457/0.85-0.85, lv_05 55%/607/0.86-0.84, lv_06 68%/813/0.85-0.85), giro guidato finito, fuori strada 0%, soak 6/6 OK, ms/frame medio ~0.46, replay 2x IDENTICO.

Blocco residuo:
- Compito 3 R1 (prova_gate legge regole.json al posto delle soglie hardcodate, return riga 536): BLOCCATO — edit negato su motore_nuovo/test/prova_gate.cpp (file di test del Junior, non modificabile senza sblocco). Serve sblocco edit + CP-005 aperto dal Capo per chiudere R1.
- Compiti 4-5 (stampa tratto violato nel gate + test espansione tratti; riscrittura 6 livello.json con tracciato): competenza Junior (@developer-junior).

Scritture questo turno: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), out/developer/ESITO_GATE_T1_20OTT_1300.md, UFFICIO/Officina/STATUS_OFFICINA.md. Zero scritture nuove in motore_nuovo/ e tools/.
