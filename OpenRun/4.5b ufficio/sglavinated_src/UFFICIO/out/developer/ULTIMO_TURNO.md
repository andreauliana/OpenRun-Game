# ULTIMO_TURNO @developer (13/11/2026 09:12)
- Blocco/passo: V47 V9 ∞ fluidita' — lv_01_neon (sesto giro).
- Azione: misura ms/frame con prova_gate percorso e smoke; confronto soglia 8 ms.
- File cambiati: UFFICIO/bacheche/developer/diario.md, UFFICIO/bacheche/developer/RIASSUNTO.md, UFFICIO/out/developer/ULTIMO_TURNO.md.
- Comando: `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv01_turno6" lv_01_neon percorso` — exit 0, "GATE PERCORSO OK" (1978 frame, 33.0 s, fuori 0%).
- Comando: `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv01_turno6_smoke" lv_01_neon smoke` — exit 0, "GATE OK".
- Misure: ms/frame medio 1.278, max 6.463 (<8 ms); replay 2x IDENTICO; caricamento 4.2-8.8 ms, init 7.5-15.7 ms.
- Alleggerimento: nessuno (sotto soglia, resa invariata). Nessun sorgente/test/dati di gioco modificato.
- Gate_t1.sh: non eseguito. run_tests.sh/ctest: non eseguiti.
- Prossimo: V9 ∞ lv_02_atomico (sesto giro).