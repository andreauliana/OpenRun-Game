# STATI PROPOSTI — @capo (26/11/2026, T1 ora 1/12)

## Tappa attiva
- **V47 [~] continuo (∞)** — `RICHIESTA_APERTA.md`. Base legacy 08/11 e vocabolario ANDREA_REVIEW §2-3 **intoccati**; si aggiunge, non si ritocca. Gate V47: `ctest` verde, `gate_t1.sh` exit 0, replay identico, Pi 400 <= 12 ms (non verificato). Gate Tappa 1 `TAPPE.md` riferimento storico; v4.6 chiusa 08/11.
- **ANDREA_REVIEW §7:** nessuna `RISPOSTA DI ANDREA`; nessuna domanda aperta nuova. REGOLA BLOCCATA §2: `drawPlayerCar` / pose giocatore non modificabili.

## Checkpoint
| CP | Stato proposto | Motivo | Azione capo |
|---|---|---|---|
| CP-001/005/006/007 | CHIUSO | già chiusi in dossier | nessuna riapertura |
| CP-002 | APERTO | review indipendente mancante (`CP-002-inventario-grafico.md:5`) | assegnare reviewer != autore quando prova disponibile |
| CP-003 | APERTO | review indipendente + prova CTest mancanti (`CP-003-status-officina-e-test.md:5`) | non chiudere senza prova/review registrata |
| CP-004 | APERTO | review indipendente mancante (`CP-004-review-e-archivio-v4.md:5`) | assegnare review indipendente |
| DECO1/F60 | APERTO (citati) | dossier assenti in `UFFICIO/Checkpoint Revision/` | nessuna chiusura; creare solo se richiesto dal flusso con evidenza |

**Nessun CP chiudibile**: nessun gate+review indipendente nuovo registrato nei dossier. CHK automatici in `SCAMBIO_RECENTE.md` = non peer review.

## Handoff verificati (lettura, non riesecuzione capo)
- **@grafico** `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (26/11 09:27 g212): VG3 0 conversioni, exit 0; blocchi auto/sponsor = area Andrea. Nessuna modifica GRAFICA/catalogo.
- **@developer** `UFFICIO/out/developer/ULTIMO_TURNO.md` (26/11 17:47 g40 lv02): no mod codice; `prova_gate ... percorso` exit 0 (2476f,41.3s,0% fuori); smoke exit 0 medio 0.811 max 61.088ms spike frame 248 (gate OK), replay IDENTICO 2961894142. Prossimo lv03.
- **@developer-junior** `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (26/11 17:46): `run_tests.sh` 27/27 exit 0; VJ1 lv01 smoke exit 0 hash 3594390208 identico dev; gate pali lv06 exit 27 (soglia tarata lv01, non regressione). D20 invariato 5 hunk drawPlayerCar; VJ3 lv05 50/60Hz FAIL 2.40% deterministico aperto; VJ4 6/6 chiuso.
- **Officina** `UFFICIO/Officina/STATUS_OFFICINA.md`: coerente con handoff jr/dev; gate_t1 riferimento 25/11 13:04 exit 0, non rieseguito dal capo.

## Deleghe (ruoli 4.7 AGENTS.md)
| Responsabile | Task | Percorsi scrivibili | Revisore indipendente | Criteri completamento / evidenza |
|---|---|---|---|---|
| @developer | **D20 DEFAULT:** revert 5 hunk `drawPlayerCar` in `motore_nuovo/src/render/renderer.cpp` (2798-2816,2875-2889,2890-2905,2910-2922,2932-2952) oppure ottenere autorizzazione Andrea registrata; poi V9 ∞ lv_03_foglie; indagare VJ3 lv05 50/60Hz 2.40%. | `motore_nuovo/src/**`, dati gioco/CMake assegnati, `UFFICIO/Officina/TOOL/**`, `UFFICIO/out/developer/ULTIMO_TURNO.md`, `UFFICIO/bacheche/developer/*` | @developer-junior | `run_tests.sh` exit 0; smoke+percorso lv_02 replay IDENTICO; diff/commit, comando, exit code, ms/frame; nessuna modifica REGOLA BLOCCATA senza autorizzazione Andrea. |
| @developer-junior | Verificare risoluzione D20 quando presente; indagare VJ3 lv05 (log, ripetizione, file/riga); VJ5 ∞ prossimo diff; valutare portabilità gate pali lv06. | `motore_nuovo/test/*.cpp`, `motore_nuovo/dati/tracciati/regole.json`, `UFFICIO/Officina/TOOL/**`, `UFFICIO/out/developer-junior/*`, `UFFICIO/bacheche/developer-junior/*` | @developer (se fix) / @capo verifica | `run_tests.sh` prima prova; comando+exit+misura in `REVIEW_V47.md`; no modifica src; review negativa = ritorno autore. |
| @grafico | VG3 ∞: confronta PNG `GRAFICA/scenari/` vs `.sglraw`; converti solo se PNG più nuovo con script catalogo; nessun ritocco area Andrea (auto/sponsor/logo/sfondi). | `GRAFICA/**`, `UFFICIO/Dipartimento Grafico/**`, `motore_nuovo/dati/sprite_raw/**` via `png_a_raw.py`, `dati/ui/testi/frasi.json` se assegnati, `UFFICIO/out/grafico/*`, `UFFICIO/bacheche/grafico/*` | @capo (catalogo/licenza/provenienza) | exit code strumenti, 0 conversioni o lista file convertiti; no risorse esterne non verificate; `traffico_incrocio/lato_furgone_bianco.png.png` non rinominare/convertire senza RISPOSTA Andrea. |

## Blocchi / decisioni necessarie
- **D20 BLOCCANTE:** 5 hunk in `drawPlayerCar` violano REGOLA BLOCCATA §2. **Decisione Andrea necessaria:** autorizzare esplicitamente gli hunk oppure confermare revert. **DEFAULT operativo:** nessun merge/chiusura su D20; @developer prepara revert pulito, non tocca altre aree bloccate.
- **VJ3 lv_05 50/60Hz:** FAIL 2.40% deterministico. Decisione non richiesta ad Andrea; serve indagine dev/jr con prova registrata.
- **Pi 400:** non verificato (limite 12 ms). Non dichiarare portabilità verificata.
- Nessun `FINITO`: passi ∞ restano disponibili.