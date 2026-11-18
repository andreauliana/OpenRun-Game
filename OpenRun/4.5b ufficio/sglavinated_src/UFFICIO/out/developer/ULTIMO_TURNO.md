# ULTIMO TURNO — developer (Turno 1, 18/11/2026 12:41)

- Blocco/passo: V47 V9 ∞ fluidita' — lv_06_desktop (diciassettesimo giro).
- File cambiati:
  - `motore_nuovo/src/render/renderer.cpp` (reserve terraOrdine_ 384->512, spriteOrd_ 512->1024)
  - `UFFICIO/bacheche/developer/diario.md` (append 1 riga)
  - `UFFICIO/bacheche/developer/RIASSUNTO.md` (riscritto)
  - `UFFICIO/out/developer/ULTIMO_TURNO.md` (riscritto)
- Comandi ed exit code:
  - `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` — exit 0
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv06_turno21" lv_06_desktop percorso` — exit 0 ("GATE PERCORSO OK", 2113 frame, 35.2 s, 0% fuori)
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv06_turno21_smoke" lv_06_desktop smoke` — exit 0 ("GATE OK", ms/frame medio 1.386 max 19.988, replay IDENTICO)
  - `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` — exit 0 (rebuild)
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv06_turno21_post" lv_06_desktop percorso` — exit 0 ("GATE PERCORSO OK")
  - `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv06_turno21_post_smoke" lv_06_desktop smoke` — exit 0 ("GATE OK", ms/frame medio 1.332 max 46.368, replay IDENTICO)
- Gate: gate_t1.sh non eseguito.
- Prossimo: V9 ∞ lv_01_neon (diciottesimo giro).