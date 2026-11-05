# RIEPILOGO VERIFICA DOCUMENTI BLOCCO B — @architetto (Turno 1, 16 ott 2026)

Stato: verifica conclusa. Nessun nuovo documento scritto: i 4 documenti di BLOCCO B erano gia' consegnati (T2, SCAMBIO voce 16) e risultano presenti e coerenti su disco.

## File ufficiali in motore_nuovo/docs/ (con copie in out/architetto/)

| Documento | Percorso | Licenza | Contenuto chiave | Coerenza |
|---|---|---|---|---|
| ARCHITETTURA.md | motore_nuovo/docs/ARCHITETTURA.md | MIT (SPDX) | Moduli core/road/render/audio/input/personaggi/gameplay/fx/ui e responsabilita'; ciclo gioco 9 passi; determinismo (frame counter, RNG seme fisso, replay input, hash frame ogni 60); zero alloc nel ciclo di gioco; moduli extra src/fx/<nome>/ con flag in dati/config.json; piano blocchi; fatti vs ipotesi. | VISIONE.md (architettura obbligatoria), codice BLOCCO C (src/), CMakeLists.txt |
| FORMATI_DATI.md | motore_nuovo/docs/FORMATI_DATI.md | MIT (SPDX) | livello.json (tema, stile, lunghezza, seme, traffico, punti show, moduli fx, scenario), sprite PNG + manifest personaggi, frasi.json (nessun tetto artificiale), playlist.json radio->brani, config.json (seed, fps_target 50, moduli on/off), ui/menu.json e ui/hud.json (320x224). | PIANO_V3.md (dati in file, nessun limite artificiale), regola 8 (audio guidato da dati) |
| BUILD.md | motore_nuovo/docs/BUILD.md | MIT (SPDX) | Dipendenze con licenze [C] da verificare prima dell'installazione; struttura CMake; target sglavinated_core (no SDL), sglavinated_runner (SDL2), sglavinated_libretro (OFF default); comandi cmake Mac (brew) e Pi 400 (apt); obiettivo <= 20 ms/frame (<= 12 ms), 50 fps PAL; test determinismo e vcgencmd Pi 400. | RISORSE_TECNICHE.md (API libretro, toolchain) |
| LICENZE.md | motore_nuovo/docs/LICENZE.md | MIT (SPDX) | D1 chiusa da Andrea: codice MIT (motore_nuovo/LICENSE), grafiche/testi/dati CC-BY-4.0 (manifest per cartella); tabella documenti e dati; tabella librerie (SDL2 zlib, SDL2_image zlib, SDL2_mixer zlib, nlohmann/json MIT, libxmp LGPL [C], Python PSFL [C], CMake [C]); fonti tecniche solo idee (mai codice/asset Sega); obblighi per ogni nuovo file; quota open soglia 50%. | DECISIONI_ANDREA.md D1, AGENTS.md regola 10 |

## Altri blocchi @architetto

| Blocco | Stato | Nota |
|---|---|---|
| BLOCCO B0 (FONTI.md) | [x] | Pista C go (Andrea 2026-10-15), A2/Cannonball no-go, censimento motori incluso. |
| BLOCCO FED (FEDELTA.md) | [x] | 10 tratti strutturali, punteggio 50.0% = soglia minima; proposta D3 DEFAULT (5 tappe per percorso con bivi, 3 livelli di prova stili diversi) chiusa da Andrea. |
| BLOCCO N (FATTIBILITA_ROM_MD.md) | [ ] | Scritto (licenza MIT) ma bloccato: regola 9 aspetta che C giri su Pi 400 con toolchain; regola 21 nessun compilatore C++ qui, build/fps/strumenti Python non eseguibili. |

## Strumenti (ultimi valori noti; shell bloccata regola 21, non rieseguibili qui)

- check_catalogo.py: exit 0, catalogo 11 righe, 2 cartelle.
- quota_open.py: 91.8% > 50% (valore noto; non rieseguibile in questo ambiente).

## Prossimo ciclo

Review licenze da @reviewer e build/misura fps/replay deterministico su Pi 400 da @debugger (blocco H) per sbloccare BLOCCO N.
