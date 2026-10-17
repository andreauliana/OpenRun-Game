# RIEPILOGO CONSEGNA BLOCCO B — @architetto (Turno 1, 16 ott 2026)

Stato: consegnato. Blocchi RICHIESTA.md: B [x] (B0 [x], FED [x] già consegnati in precedenza); unico [ ] rimanente: BLOCCO N (attende C su Pi 400 con toolchain, regole 9/21).

## File prodotti in motore_nuovo/docs/ (copie condivise qui in out/architetto/)

| Documento | Percorso ufficiale | Copia out/ | Licenza | Contenuto |
|---|---|---|---|---|
| ARCHITETTURA.md | motore_nuovo/docs/ARCHITETTURA.md | ARCHITETTURA.md | MIT (SPDX) | Moduli core/road/render/audio/input/personaggi/fx/ui, ciclo gioco 9 passi, determinismo (frame counter + RNG seme), replay input + hash frame ogni 60 frame, zero alloc ciclo di gioco, moduli extra src/fx/<nome>/ con flag on/off in dati/config.json, piano blocchi, fatti vs ipotesi. |
| FORMATI_DATI.md | motore_nuovo/docs/FORMATI_DATI.md | FORMATI_DATI.md | MIT (SPDX) | livello.json (tema, stile, lunghezza, seme, traffico, punti show, moduli fx, scenario), sprite PNG + manifest personaggi (check_catalogo.py), frasi.json (nessun tetto), playlist.json (radio -> brani .xm), config.json (seed, fps_target 50, moduli on/off), ui/menu.json e ui/hud.json (320x224, @tipografo). |
| BUILD.md | motore_nuovo/docs/BUILD.md | BUILD.md | MIT (SPDX) | Dipendenze (tabella con licenze [C] da verificare prima dell'installazione), struttura CMake, target sglavinated_core / sglavinated_runner / sglavinated_libretro (OFF default), comandi cmake Mac (brew) e Pi 400 (apt), obiettivo <= 20 ms/frame (<= 12 ms), 50 fps PAL, test determinismo e vcgencmd Pi 400. |
| LICENZE.md | motore_nuovo/docs/LICENZE.md | LICENZE.md | MIT (SPDX) | D1 chiusa Andrea: codice MIT (motore_nuovo/LICENSE), grafiche/testi/dati CC-BY-4.0 (manifest per cartella), tabella documenti e dati, tabella librerie (SDL2 zlib, SDL2_image zlib, SDL2_mixer zlib, nlohmann/json MIT, libxmp LGPL [C], Python PSFL [C], CMake [C]), fonti tecniche solo idee (mai codice/asset Sega), obblighi per ogni nuovo file, quota open soglia 50% (ultimo valore noto 91.8% > 50%). |

## Coerenza

- VISIONE.md: core C++17 indipendente dalla piattaforma, render software 320x224, 50 Hz, audio int16 stereo, input RetroPad, tempo = contatore di frame, RNG con seed, nessun orologio di sistema, core libretro + runner SDL2, dati in file (JSON/PNG/.xm).
- PIANO_V3.md: pista C decisa da Andrea; D1 MIT/CC-BY-4.0; D3 albero 5 percorsi x 5 tappe = 15 tappe, 5 finali; prototipo 3 livelli di prova stili diversi.
- RAPPORTO_FATTIBILITA.md / RISORSE_TECNICHE.md: librerie compatibili Pi 400, API libretro, build con CMake.
- DECISIONI_ANDREA.md: D1 e D3 chiuse (applicate).

## Strumenti (ultimi valori noti, shell bloccata regola 21 non consente riesecuzione qui)

- `check_catalogo.py`: exit 0, catalogo 11 righe, 2 cartelle.
- `quota_open.py`: 91.8% (247656/269832 byte, soglia 50%).

## Prossimo ciclo

Review licenze da @reviewer (CHECK 2 ore) e build/misura fps/replay su Pi 400 da @debugger (blocco H) per sbloccare BLOCCO N.
