Blocco/passo: V47 V9 ∞ fluidita' lv_03_foglie + V7 prealloc render (turno 11/11/2026 13:22).
File cambiati:
- motore_nuovo/src/render/renderer.cpp (terraOrdine_ reserve 64->128)
- UFFICIO/bacheche/developer/diario.md (riga turno)
- UFFICIO/bacheche/developer/RIASSUNTO.md (riscritto)
- UFFICIO/out/developer/ULTIMO_TURNO.md (riscritto)

Comandi ed exit code:
- ./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v47_v9_lv03_percorso" lv_03_foglie percorso -> exit 0 ("GATE PERCORSO OK", 2273 frame, 0% fuori)
- ./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v47_v9_lv03_smoke" lv_03_foglie smoke -> exit 0 ("GATE OK", ms/frame medio 0.984 max 3.630, replay IDENTICO, caricamento 2.8-7.5 ms)
- cmake --build UFFICIO/Officina/build --target prova_gate -> exit 0

Gate: gate_t1.sh non eseguito.
Prossimo: V47 V9 ∞ fluidita' lv_04_cromo.