# Esito Blocco H — @developer/@developer-junior | 17/10/2026 12:25 (copia in motore_nuovo/test/)

Vedere out/debugger/ESITO_BLOCCO_H_17OTT_1225.md per il report completo.

Sintesi:
- Shell negata (regola 21): nessun cmake/clang++/build eseguibile qui.
- Verifica statica voce 1113: CMakeLists.txt:1 SPDX MIT OK; :17 json_mini.h OK; road.cpp:87-88 wrap +/-ROAD_HALF_WORLD OK; renderer.cpp:115-118 lower_bound OK. Tutti i 3 rilievi già sanati da @developer/@developer-junior.
- Rilievo residuo: BUILD.md:29/70/99 elenca nlohmann/json (parser interno json_mini.h usato). Diff proposta a @developer/@developer-junior.
- prova_gate.cpp presente in motore_nuovo/test/ (145 righe). Gate su Pi 400 in attesa.
- Blocco H [~] invariato.
