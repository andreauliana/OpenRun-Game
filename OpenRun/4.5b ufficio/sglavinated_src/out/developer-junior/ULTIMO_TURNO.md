Blocco/passo: PERC1 J2 + J3 (+ J4 review) — 08/11 13:23.
File cambiati: `motore_nuovo/test/prova_gate.cpp` (J3: check galleria lv_02/lv_05 in `verificaTratti`, screenshot PPM in `UFFICIO/Officina/Test area/perc1_j3_galleria_*.ppm`, rilievo oggetti in galleria).
Comandi ed exit code:
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/j2_check" lv_01_neon traffico` — exit 0 (J2 OK: autostrada 0.0047 > ponte 0.0040 auto/1000u, deterministico).
- `sh UFFICIO/Officina/TOOL/run_tests.sh` — exit 8; 26/27 ctest OK, `gate_tratti` (27) Failed per `espansione=NO` pre-esistente lv_02/lv_05 (`segmenti=5200/5640`, dichiarati 17/15); J3 galleria: lv_02 260 seg, lv_05 260 seg, screenshot salvati; rilievo oggetti in galleria 187/162.
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/perc1_j3" lv_01_neon tratti` — exit 26 (stesso fallimento espansione pre-esistente).
- `ls "UFFICIO/Officina/Test area"/perc1_j3_galleria_*.ppm` — exit 0 (2 file presenti).
gate_t1.sh: non eseguito.
Rilievi: oggetti decorativi placement dentro galleria (D4 renderer li omette? da verificare @developer); espansione=NO lv_02/lv_05 pre-esistente blocca gate_tratti/ctest 27.