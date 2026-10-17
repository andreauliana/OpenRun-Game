# Riepilogo verifica BLOCCO B — Turno 1 (@architetto)

Data: 16 ott 2026. Stato: BLOCCO B [x] (consegnato T2, verifica riconfermata questo turno).

## Documenti prodotti/verificati in motore_nuovo/docs/

| Documento | Contenuto | Licenza | Coerenza |
|---|---|---|---|
| ARCHITETTURA.md | moduli core/road/render/audio/input/gameplay/fx/ui, ciclo gioco 9 passi, determinismo (frame counter + RNG seme, replay input, hash frame ogni 60), piano blocchi | MIT | PIANO_V3.md, RAPPORTO_FATTIBILITA.md, RISORSE_TECNICHE.md, VISIONE.md, DECISIONI_ANDREA.md (D1/D3 chiuse), codice BLOCCO C |
| FORMATI_DATI.md | livello.json, sprite PNG, frasi.json (nessun tetto), playlist.json audio (radio->brani), ui/*.json, config.json (moduli on/off) | MIT (doc) / CC-BY-4.0 (dati) | PIANO_V3.md, VISIONE.md (dati in file, nessun limite artificiale) |
| BUILD.md | comandi cmake Mac (brew) + Pi 400 (apt), runner SDL2, core libretro OFF default, dipendenze con licenze, obiettivo fps Pi 400 | MIT | RISORSE_TECNICHE.md, VISIONE.md (core senza SDL) |
| LICENZE.md | D1 chiusa (Andrea 2026-10-16): codice MIT, grafiche/testi CC-BY 4.0; tabella documenti + librerie; obblighi per nuovo file; quota open soglia 50% | MIT (doc) | DECISIONI_ANDREA.md D1, regola 10 |

## Piccolo allineamento applicato
- BUILD.md albero sorgente: `personaggi/` -> `gameplay/`; `core_libretro/libretro.cpp` -> `src/libretro/sglavinated_libretro.cpp` (coerente con motore_nuovo/CMakeLists.txt e con il codice consegnato).

## Blocco restante
- BLOCCO N [ ]: fattibilita ROM Mega Drive .md (FATTIBILITA_ROM_MD.md gia' scritto, licenza MIT). Bloccato da regola 9 (attende che C giri su Pi 400 con toolchain) e regola 21 (nessun compilatore C++ qui; build/misura fps/strumenti Python non eseguibili).

## Strumenti (ultimi valori noti, shell bloccata qui per regola 21)
- check_catalogo.py: OK (catalogo: 11 righe, 2 cartelle).
- quota_open.py: 91.8% > 50% (soglia raggiunta).

## Consegna condivise
- Copie dei 4 documenti in out/architetto/.
- Voce SCAMBIO.md 53 (ARCHITETTO -> CAPO, [CHIUSA]).
- Riga in bacheche/architetto/diario.md.
- Riga pausa in BAR.md (nessuno SPUNTO).

## Prossimo ciclo
Review licenze @reviewer; build/misura fps/replay su Pi 400 @debugger (blocco H) per sbloccare BLOCCO N.
