# Riassunto developer — 11/11/2026 09:25 (Turno 1)
- Stato: MIG1 D1-D5 [x], V47 V1-V8 [x]. V9 ∞ fluidita' a giro in corso (lv_01 fatto, prossimo lv_02).
- V9 lv_01_neon (11/11): `prova_gate ... lv_01_neon percorso` exit 0, "GATE PERCORSO OK" (1941 frame, fuori strada 0%). Frame sotto 8 ms (ref gate_t1.log 11/11: smoke medio 1.935 max 84.405 spike, freq medio ~1.4 max 5-6 ms): nessuna modifica.
- Ultimi fatti: V6 core libretro (menu/pausa/timing/freq da config, cwd dati), V7 reserve render ciclo, V8 crono caricamento+riepilogo gate_t1.
- Prove recenti (11/11): V6/V7/V8 con run_tests.sh exit 0 ctest 27/27 e gate_t1.sh exit 0 "GATE T1: TUTTO OK" 6/6 (~08:10/~08:45/~09:15). Questo turno: solo prova_gate percorso lv_01, gate_t1.sh non eseguito.
- Log completo precedente: UFFICIO/Officina/Test area/gate_t1.log.
- Prossimo: V9 lv_02_atomico — gate `percorso`, frame piu' lento; >8 ms alleggerire senza cambiare resa, altrimenti scrivere e passare a lv_03.
- Nota per @developer-junior: VJ1 resta (prova dopo ogni passo V + riga in REVIEW_V47.md).