# STATI PROPOSTI — @capo (Turno 5, ora 9/12, 2026-11-26)

## Contesto
- Fonti lette: TAPPE.md, ANDREA_REVIEW.md (prec. ass., §7 nessuna RISPOSTA/decisione aperta; REGOLA BLOCCATA drawPlayerCar §2; G1 ANNULLATO; legacy 08/11+vocabolario intoccati), DIREZIONE_ARTISTICA.md, CONTROLLO.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Checkpoint: STATO_CORRENTE + coda REGISTRO_REVISIONI lette. CP-001/005/006/007 CHIUSI (no riaprire). CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier assenti). **Nessun CP con gate+review indipendenti registrati → nessuna chiusura.**
- Handoff (capo non riesegue): dev `UFFICIO/out/developer/ULTIMO_TURNO.md` 26/11 16:36 V9 lv_04 g40; jr `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 26/11 16:52 VJ5/VJ1; gf `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 26/11 09:27 g212; Officina `STATUS_OFFICINA.md` parzialmente arretrato.
- SCAMBIO_RECENTE CHK auto [APERTE] = controlli automatici, non peer review/CP.

## Tappa/gate
- Tappa attiva: **V47 [~] continuo** (∞), MIG1 [~]. Gate V47: ctest verde, `gate_t1.sh` exit 0, replay IDENTICO, Pi 400 <=12 ms/frame. Gate non rieseguito dal capo (rif. 25/11 13:04 exit 0).

## Rilievi
- **D20 (REGOLA BLOCCATA, ANDREA_REVIEW §2):** 5 hunk `drawPlayerCar` presenti (`renderer.cpp:2798-2816,:2875-2889,:2890-2905,:2910-2922,:2932-2952`). Jr 16:52: `git diff HEAD -- motore_nuovo/src` pulito, nessun cambio nuovo, D20 invariato. Dev 16:36 non ha toccato codice. **DEFAULT (proseguo senza aspettare):** @developer revert/ottiene autorizzazione Andrea registrata per i 5 hunk, poi verifica jr; nessuna altra modifica drawPlayerCar/auto giocatore.
- **VJ3 lv_05_spray 50/60 Hz:** FAIL 2.40% > 2.00% deterministico; altri 5 OK. VJ4 chiuso 6/6. **DEFAULT:** FAIL aperto, non chiudere VJ3; indagare con log.

## Deleghe (ruoli 4.7)

| Task | Responsabile | Revisore indipendente | Criteri misurabili | Esito verifiche (handoff) | Stato proposto |
|---|---|---|---|---|---|
| D20 revert/autorizzazione drawPlayerCar; poi V9 ∞ lv_05_spray | @developer (`UFFICIO/out/developer/`) | @developer-junior | 5 hunk drawPlayerCar risolti (revert o autorizzazione Andrea registrata); run_tests exit 0; smoke+percorso lv_02 replay IDENTICO; poi lv_05 percorso/smoke exit 0, replay IDENTICO, ms log; indagare VJ3 lv05 | 26/11 16:36: lv_04 g40 no mod codice, percorso exit 0 (2125f,35.4s,0% fuori), smoke exit 0 medio 0.621 max 2.762ms, replay IDENTICO 1415607011; prossimo lv_05 | BLOCCATO da D20 (DEFAULT: risolvere D20 prima) |
| Verifica D20; indagine VJ3 lv_05; VJ5 ∞ prossimo diff | @developer-junior (`UFFICIO/out/developer-junior/`) | @developer (se fix) / @capo verifica | `run_tests.sh` prima prova; review file/riga in `REVIEW_V47.md`; no src; ripetere/log VJ3 lv_05 | 26/11 16:52: VJ5/VJ1, run_tests 27/27 exit 0, lv_05 smoke exit 0 (0.584/2.758ms, IDENTICO), lv_04 smoke hash 1415607011; D20 5 hunk invariati; VJ3 lv05 FAIL aperto | IN CORSO |
| VG3 ∞ PNG vs `.sglraw` scenari | @grafico (`UFFICIO/out/grafico/`) | @capo verifica catalogo/licenza/provenienza (no peer review tecnica) | Solo script catalogo; converti solo PNG più nuovo; no ritocco area Andrea; no risorse esterne non verificate | 26/11 09:27 g212: 0 conv, 6/6 skyline, 165 PNG, 155 raw/155 tracciati 0 senza voce, 0 da decidere, autotest OK, exit 0 | IN CORSO |
| Coordinamento CP/registro | @capo (`UFFICIO/out/capo/`) | — (non revisore tecnico) | REGISTRO append-only; STATO/STATI allineati; chiusura CP solo con gate+review indipendenti in dossier | Nessun CP chiudibile; D20 e VJ3 lv05 aperti | APERTO |

## Blocchi / decisioni
- **D20 [BLOCCO PROCEDURALE]:** 5 hunk drawPlayerCar, REGOLA BLOCCATA ANDREA_REVIEW §2. Non in §7; capo non scrive ANDREA_REVIEW. DEFAULT: revert/autorizzazione, verifica jr, poi V9. Proseguo senza aspettare.
- **VJ3 lv_05 50/60 Hz:** diff 2.40% deterministico. DEFAULT: FAIL aperto, non chiudere VJ3.
- **DEFAULT:** `traffico_incrocio/lato_furgone_bianco.png.png` — NON rinominare/convertire senza RISPOSTA DI ANDREA.
- **Pi 400 non verificato** (blocco H sospeso). Nessuna peer review V47 registrata in dossier CP → no chiusura.
- Nessun PER_CLAUDE. Nessun FINITO (passi ∞ attivi; D20 e VJ3 lv05 aperti).