# RIASSUNTO developer (max 12 righe)
- Data/ora: 13/11/2026 09:12 (Turno 1)
- V47 V9 ∞ fluidita' lv_01_neon (sesto giro): percorso (v9_lv01_turno6) exit 0 "GATE PERCORSO OK" (1978 frame, 33.0 s, 0% fuori).
- Smoke (v9_lv01_turno6_smoke) exit 0 "GATE OK": medio 1.278 ms, max 6.463 ms (<8 ms), replay 2x IDENTICO.
- Caricamento 4.2-8.8 ms, init 7.5-15.7 ms (<1 s). Nessun alleggerimento: gia' sotto soglia, resa invariata.
- run_tests.sh/ctest non eseguiti in questo turno (solo prova_gate percorso/smoke lv_01_neon).
- Nota: il max oltre 8 ms visto su lv_03 resta solo primo frame a freddo (08:25).
- Prossimo V9 ∞ lv_02_atomico (sesto giro).
- Gate: gate_t1.sh non eseguito.
- Vincoli: HUD/auto giocatore/runner/costanti gameplay/tracciati non toccati; nessun sorgente modificato.