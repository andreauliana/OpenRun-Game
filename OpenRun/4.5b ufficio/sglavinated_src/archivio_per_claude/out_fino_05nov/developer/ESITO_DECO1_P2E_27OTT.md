# Esito blocco DECO1 passo 2e — 27/10/2026 turno 1 (@developer)

Blocco: DECO1 passo 2e — parallasse leggero in curva dello skyline.

Modifiche:
- motore_nuovo/src/render/renderer.h: membro `skylineScrollX_` e metodo `scorrereSkyline(f32 dx)`.
- motore_nuovo/src/render/renderer.cpp: implementa `scorrereSkyline`; `drawSkyline` applica lo scorrere orizzontale con wrap sulla larghezza della fonte (parallasse in curva).
- motore_nuovo/src/game/game.cpp: `update()` chiama `ren_.scorrereSkyline(-curvaAt(player_.z) * passoGiocatore_ * 0.10f)` dopo l'avanzamento della strada (zero alloc, deterministico).

Comando ed esito:
- `cmake --build UFFICIO/Officina/build --target prova_gate` -> exit 0 (`renderer.cpp`, `game.cpp`, `prova_gate` compilano/linkano).

Note: `sh UFFICIO/Officina/TOOL/gate_t1.sh` avviato ma interrotto per timeout su `lv_04` (nessun exit code); ultimo gate pulito 07:55 exit 0 (2d, GATE T1: TUTTO OK, densita' 26-27%, ctest 21/21 verde, v. `out/developer/ESITO_DECO1_P2D_27OTT.md`). Gate T1 completo con le modifiche 2e da rieseguire. La parallasse modifica solo i pixel cielo (framebuffer), non la logica di gioco: determinismo e replay 2x IDENTICO preservati.

Residui: gate T1 completo da rieseguire; poi R2. T1-RUN [~], H [~] in attesa Andrea (runner Mac v91, poi fps/temperatura Pi 400); nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac.
