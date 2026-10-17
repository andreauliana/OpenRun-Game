# RIEPILOGO @coder — voce SCAMBIO 1136 — 17/10/2026 12:45

Blocco: voce 93 (priorita' massima, P1 Tappa 1) + rilievi reviewer CHECKPOINT 1 voce 1114.
Perimetro: motore_nuovo/src/ + motore_nuovo/CMakeLists.txt (regola 15). SPDX MIT. Zero alloc ciclo. Determinismo preservato.

Modifiche:
1. CMakeLists.txt — intestazione `# SPDX-License-Identifier: MIT` in testa (mancava; tutti i .cpp/.h ce l'avevano).
2. src/road/road.cpp `advance()` — wrap playerX allineato a +/-ROAD_HALF_WORLD (900) invece di +/-4000 (coerente con X_LIMIT=900 e col clamp di game.cpp).
3. src/render/renderer.cpp `drawSprites()` — corretto confronto profondita' sprite (`p.worldZ >= zRel` invece di `p.worldZ >= s.worldZ`, che mixava relativo/assoluto e saltava tutti i sprite/cartelloni); sostituita scansione lineare O(sprite x DRAW_DIST) con `std::lower_bound` su projs_ ordinato per worldZ (O(log DRAW_DIST), Pi 400).

Non toccato: motore_nuovo/docs/BUILD.md (nlohmann/json righe 29/70/99) — fuori perimetro @coder (regola 15), segnalato a @capo/@architetto.

Non eseguibile qui (regola 21): build cmake, target prova_gate, replay 2x, ms/frame, fps, verifica visiva strada -> @debugger su Pi 400/Mac.
SCAMBIO.md: scrittura negata (permesso); esito in bacheche/coder/diario.md e RIASSUNTO.md.

Stati invariati: C [x], H [~], P1 [~], P1b [~].
