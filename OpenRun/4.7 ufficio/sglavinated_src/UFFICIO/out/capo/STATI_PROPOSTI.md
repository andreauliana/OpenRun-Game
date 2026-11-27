# STATI PROPOSTI — @capo (27/11/2026, T2 ora 2/12 verifica)

## Tappa attiva
- **V47 [~] continuo (∞)** — `RICHIESTA_APERTA.md`. Base legacy 08/11 e vocabolario ANDREA_REVIEW §2-3 **intoccati**; si aggiunge, non si ritocca. Gate V47: `ctest` verde, `gate_t1.sh` exit 0, replay identico, Pi 400 <= 12 ms (non verificato).
- **ANDREA_REVIEW §7:** nessuna `RISPOSTA DI ANDREA`. REGOLA BLOCCATA §2: `drawPlayerCar`/pose giocatore non modificabili.

## Checkpoint
| CP | Stato proposto | Motivo | Azione capo |
|---|---|---|---|
| CP-001/005/006/007 | CHIUSO | già chiusi | nessuna riapertura |
| CP-002 | APERTO | review indipendente mancante | assegnare reviewer != autore con prova registrata |
| CP-003 | APERTO | review indipendente + prova CTest mancanti | non chiudere senza prova/review registrata |
| CP-004 | APERTO | review indipendente mancante | assegnare review indipendente |
| DECO1/F60 | APERTO (citati) | dossier assenti | nessuna chiusura; creare solo con evidenza |

**Nessun CP chiudibile**: nessun gate+review indipendente registrati nei dossier. CHK automatici `SCAMBIO_RECENTE.md` = controlli automatici, **non peer review**.

## Handoff verificati (lettura, non riesecuzione capo)
- **@grafico** `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (27/11 09:24 g257): VG3 0 conversioni; exit 0 (`vg3_date_png_raw`, autotest 43; `vg3_raw_senza_catalogo` 42; `vg3_png_non_catalogati` 37); nessuna modifica GRAFICA/catalogo; area auto/sponsor/pose `_b` = Andrea/non azionabile.
- **@developer** `UFFICIO/out/developer/ULTIMO_TURNO.md` (27/11 09:20 V9 lv_06_desktop g44): no mod codice; percorso exit 0 (2125f,35.4s,0% fuori); smoke exit 0 medio 0.671 max 4.006ms frame 0, replay IDENTICO hash 3627137594; prossimo V9 lv_01_neon g45.
- **@developer-junior** `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (27/11 09:21 VJ5): src invariato vs HEAD dad983b; rilievo `paesaggio.cpp:260-262` disponiTerraPonte conteggi O(n) (no REGOLA BLOCCATA, resa/replay invariati), smoke lv_04 exit 0 medio 0.694 max 4.014ms; VJ3 lv_05 50/60Hz FAIL 2.40% deterministico aperto; VJ4 6/6 chiuso.
- **Officina** `UFFICIO/Officina/STATUS_OFFICINA.md`: riferimento storico (18/11), non handoff fresco; usare handoff ruolo sopra.

## Deleghe (ruoli 4.7 AGENTS.md)
| Responsabile | Task | Percorsi scrivibili | Revisore indipendente | Criteri completamento / evidenza |
|---|---|---|---|---|
| @developer | **D20 DEFAULT:** revert 5 hunk `drawPlayerCar` in `motore_nuovo/src/render/renderer.cpp` (2798-2816,2875-2889,2890-2905,2910-2922,2932-2952) oppure ottenere autorizzazione Andrea registrata; poi V9 ∞ lv_01_neon g45; indagare VJ3 lv05 50/60Hz 2.40%. | `motore_nuovo/src/**`, dati gioco/CMake assegnati, `UFFICIO/Officina/TOOL/**`, `UFFICIO/out/developer/ULTIMO_TURNO.md`, `UFFICIO/bacheche/developer/*` | @developer-junior | `run_tests.sh` exit 0; smoke+percorso replay IDENTICO; diff/commit, comando, exit code, ms/frame; nessuna modifica REGOLA BLOCCATA senza autorizzazione Andrea. |
| @developer-junior | Verificare risoluzione D20 quando presente; indagare VJ3 lv05 (log, ripetizione, file/riga); VJ5 ∞ prossimo diff; valutare portabilità gate pali lv06. | `motore_nuovo/test/*.cpp`, `motore_nuovo/dati/tracciati/regole.json`, `UFFICIO/Officina/TOOL/**`, `UFFICIO/out/developer-junior/*`, `UFFICIO/bacheche/developer-junior/*` | @developer (se fix) / @capo verifica | `run_tests.sh` prima prova; comando+exit+misura in `REVIEW_V47.md`; no modifica src; review negativa = ritorno autore. |
| @grafico | VG3 ∞: confronta PNG `GRAFICA/scenari/` vs `.sglraw`; converti solo se PNG più nuovo con script catalogo; nessun ritocco area Andrea. | `GRAFICA/**`, `UFFICIO/Dipartimento Grafico/**`, `motore_nuovo/dati/sprite_raw/**` via `png_a_raw.py`, `dati/ui/testi/frasi.json` assegnati, `UFFICIO/out/grafico/*`, `UFFICIO/bacheche/grafico/*` | @capo (catalogo/licenza/provenienza) | exit code strumenti, 0 conversioni o lista file convertiti; no risorse esterne non verificate; `traffico_incrocio/lato_furgone_bianco.png.png` non rinominare/convertire senza RISPOSTA Andrea. |

## Blocchi / decisioni necessarie
- **D20 BLOCCANTE:** 5 hunk in `drawPlayerCar` violano REGOLA BLOCCATA §2. **Decisione Andrea necessaria:** autorizzare esplicitamente gli hunk oppure confermare revert. **DEFAULT operativo:** nessun merge/chiusura su D20; @developer prepara revert pulito, non tocca aree bloccate. Capo non scrive ANDREA_REVIEW.md.
- **VJ3 lv_05 50/60Hz:** FAIL 2.40% deterministico. Nessuna decisione Andrea richiesta; serve indagine dev/jr con prova registrata.
- **Pi 400:** non verificato (limite 12 ms). Non dichiarare portabilità verificata.
- Nessun `FINITO`: passi ∞ disponibili.
