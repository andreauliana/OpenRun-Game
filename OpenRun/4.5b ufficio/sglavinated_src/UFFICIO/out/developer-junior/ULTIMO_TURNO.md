# ULTIMO_TURNO @developer-junior — 09/11/2026 10:15 (MIG1 J2/J3/J4)
- Blocco/passo: MIG1 J2+J3 esecuzione prove, J4 review aggiornata.
- File cambiati (turno): `UFFICIO/bacheche/developer-junior/diario.md`, `UFFICIO/bacheche/developer-junior/RIASSUNTO.md`, `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`.
- Cmd: `cmake --build "UFFICIO/Officina/build" --target prova_gate` → exit 0 (09/11 10:15).
- Cmd: `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/mig1_j2_pali" lv_01_neon pali` → exit 27 (09/11 10:15).
  - pali: hit_rilevati=0, frame_primo_hit=-1, screenshot=0, dir `mig1_j2_pali/`. FALLITO (serve hit>0 e screenshot>=2). Nota test: clamp baseY<=208 riga ruote.
- Cmd: `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/mig1_j3_sportellata" lv_01_neon sportellata` → exit 0 (09/11 10:15).
  - sportellata: GATE SPORTELLATA OK, colpo_a_segno=SI a 0.52s (frame 31), urti_gara=8, ripresa_proxy Z 0-2s=30168.0 2-4s=10325.4 4-7s=-40493.4 (proxy; `sbandataS` privato).
- gate_t1.sh: non eseguito.
- Residuo: J2 FALLITO senza hit/screenshot (verificare setup modo pali vs stato D1/paletti); J3 colpo misurato ma recupero ~5s solo proxy. Next: indagine J2, poi affinare prova o chiedere chiarimento dev.