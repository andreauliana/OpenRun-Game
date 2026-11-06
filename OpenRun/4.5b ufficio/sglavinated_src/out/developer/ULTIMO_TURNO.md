Turno 1 @developer — blocco P1 punto 2 (colline)
Passo: fix double integration colline in road.cpp - collina quota assoluta per segmento invece che cumulativa
File cambiati: motore_nuovo/src/road/road.cpp
Comando: cmake --build UFFICIO/Officina/build --target prova_gate
Exit code: 0
Esito: gate smoke OK lv_01_neon, lv_02_atomico, lv_03_foglie (GATE PERCORSO OK). Le prime 3 porte passano con collina assolta, ms/frame ~1.0 max 4.4 (soglia 20). Timeout soak test.