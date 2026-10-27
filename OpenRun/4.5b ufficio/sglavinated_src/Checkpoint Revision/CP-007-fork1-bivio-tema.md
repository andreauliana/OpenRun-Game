# CP-007 — FORK1 bivio e transizione di tema

| Campo | Valore |
|---|---|
| Stato | CHIUSO |
| Aperto / ultima modifica | 2026-10-27 / 2026-10-27 |
| Ambito e criteri di completamento | Blocco FORK1 (RICHIESTA_APERTA.md): cambio tappa senza scatto di prospettiva; corsia a scelta aperta senza salto di `centroCorsia`; bordi senza intrappolamento né ribaltamento improprio; transizione di tema graduale (colori interpolati in secondi, tema di arrivo fisso dopo la scelta); gate `bivio_uscita`, `bivio_bordi`, `tema_transizione` verdi; ctest verde; replay 2x IDENTICO; soglie di `regole.json` non abbassate. |
| Responsabile | @capo (dossier e chiusura); codice @developer (`game.cpp`, `renderer.cpp`, `road.h`, `BIVIO_SEG`); gate @developer-junior (`prova_gate.cpp`, `regole.json`) |
| Revisore indipendente | @developer-junior → codice @developer (`REVIEW_FORK1_26OTT.md` 07:14, POSITIVA); @developer → gate @developer-junior (`REVIEW_FORK1_26OTT.md` 07:20, POSITIVA) |
| Percorsi autorizzati | `Checkpoint Revision/` (solo dossier); lettura `out/developer/`, `out/developer-junior/`, `motore_nuovo/`, `UFFICIO/Officina/Test area/` |
| Decisioni richieste ad Andrea | Nessuna (nessuna domanda su chiusura FORK1 in `ANDREA_REVIEW.md`; chiusura operativa del @capo dopo review indipendenti positive; regole 8 e 20 di `README.md`) |
| Handoff Designer (grafica) | non applicabile (elemento di passaggio al bivio del @grafico: facoltativo, non blocca) |
| Handoff Officina (codice/build/test) | `UFFICIO/Officina/STATUS_OFFICINA.md` (report 26/10 e 27/10: gate verdi, ctest 20/20) |

## Sintesi

- Obiettivo: chiudere FORK1 dopo le due review incrociate positive e il ctest verde.
- Esito corrente: review @developer-junior POSITIVA (07:14, punti 1-3 e 6: raccordo curva/collina, corsia a scelta aperta, bordi, transizione tema); review @developer POSITIVA (07:20, gate 4 `bivio_uscita`, 5 `bivio_bordi`, 7 `tema_transizione`); `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0 `GATE T1: TUTTO OK` (26/10/2026 07:00); `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0, ctest 20/20 verde inclusi i 3 gate FORK1.
- Blocco o prossimo passo: nessuno per FORK1 (chiuso). Dopo FORK1: DECO1 [ ] (passo 2a skyline lv_01 del @developer), poi RIV [ ], R2 [ ]; T1-RUN [~] (D19 = B, CP-006 APERTO).

## Modifiche

| Percorso/file | Scopo | Autore |
|---|---|---|
| Nessuna in questo checkpoint | Dossier di chiusura; nessuna scrittura di codice/asset in questo turno | — |

## Prove

| Data | Comando/verifica | Exit code/esito | Misura essenziale | Log o evidenza |
|---|---|---|---|---|
| 26/10/2026 07:00 | `sh UFFICIO/Officina/TOOL/gate_t1.sh` | exit 0, `GATE T1: TUTTO OK` | 6/6 percorsi OK, ms/frame medio 0.20 max 0.95 <= 20, replay 2x IDENTICO | `UFFICIO/Officina/Test area/gate_t1.log` |
| 26/10/2026 07:00 | `sh UFFICIO/Officina/TOOL/run_tests.sh` | exit 0, ctest 20/20 | inclusi `gate_bivio_uscita`, `gate_bivio_bordi`, `gate_tema_transizione` | `UFFICIO/Officina/build/Testing/Temporary/LastTest.log` |
| 26/10/2026 07:14 | `prova_gate ... lv_01_neon bivio_uscita` | exit 0, `GATE BIVIO USCITA OK` | 4 combinazioni OK; maxDelta larghezza 16.0 <= 400, centro 6.3, x_rel 12.7 <= 40, curva/collina 0.00 <= 0.05 | `UFFICIO/Officina/build/test-results/bivio_uscita/` |
| 25/10/2026 08:46 | `prova_gate ... bivio_bordi` | exit 0, `GATE BIVIO BORDI OK` | 2 cambi tappa, maxOffroad 33 <= 60, nessun ribaltamento | `UFFICIO/Officina/build/test-results/bivio_bordi/` |
| 25/10/2026 07:47 | `prova_gate ... tema_transizione` | exit 0, `GATE TEMA TRANSIZIONE OK` | variazione <= 12 canale/frame, durata 2.0 s (`transizione_tema_s`) | `UFFICIO/Officina/build/test-results/` |

## Review indipendente

- Revisore: @developer-junior (07:14) su `game.cpp`/`renderer.cpp` (punti 1-3, 6 FORK1) — POSITIVA, rilievi non bloccanti (soglie "bivio" da validare in tuning, non abbassare).
- Revisore: @developer (07:20) su gate 4/5/7 di `prova_gate.cpp` — POSITIVA, nessun rilievo bloccante; determinismo invariato; nessun file `src/runner/` toccato.
- Status di reparto consultati: `STATUS_OFFICINA.md` (report 26/10 e 27/10) e `out/grafico/STATUS_GRAFICO_PER_CAPO.md` (elemento bivio facoltativo, non blocca).

## Decisione di Andrea

- Stato: NON RICHIESTA.
- Decisione e data: nessuna domanda di Andrea su chiusura FORK1 in `ANDREA_REVIEW.md`; chiusura a carico del @capo dopo review indipendenti positive e prove registrate.

## Eventi

- `2026-10-27` — apertura: responsabile @capo, revisori @developer-junior e @developer, ambito FORK1 chiusura con review già registrate.
- `2026-10-27` — chiusura: due review indipendenti POSITIVE + ctest 20/20 verde + gate FORK1 verdi; CP-007 CHIUSO senza decisione di Andrea (non richiesta).

## Chiusura

- Esito finale: FORK1 [x] chiuso.
- Evidenze/review completate: `out/developer-junior/REVIEW_FORK1_26OTT.md` POSITIVA; `out/developer/REVIEW_FORK1_26OTT.md` POSITIVA; ctest 20/20 verde; gate `bivio_uscita`/`bivio_bordi`/`tema_transizione` verdi.
- Decisione di Andrea o motivazione per cui non è richiesta: NON RICHIESTA (peer review incrociata completa; nessun D# per chiusura FORK1 in `ANDREA_REVIEW.md`).
- Chiuso da Capo il `2026-10-27`.
