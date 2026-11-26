# STATI PROPOSTI — @capo (Turno 2, ora 3/12, 2026-11-26)

## Contesto
- Fonti lette: TAPPE.md, ANDREA_REVIEW.md (prec. ass., §7 nessuna RISPOSTA/decisione aperta; G1 ANNULLATO; REGOLA BLOCCATA drawPlayerCar §2; legacy 08/11+vocabolario intoccati), DIREZIONE_ARTISTICA.md, CONTROLLO.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Checkpoint: STATO_CORRENTE + coda REGISTRO_REVISIONI letti. CP-001/005/006/007 CHIUSI (no riaprire). CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier assenti). **Nessun CP con gate+review indipendenti registrati → nessuna chiusura.**
- Handoff (capo non riesegue): dev `UFFICIO/out/developer/ULTIMO_TURNO.md` 26/11 10:25 lv_02 giro40; jr `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 26/11 10:27-10:32 VJ5; gf `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 26/11 09:27 g212; Officina `STATUS_OFFICINA.md` arretrato.

## Tappa/gate
- Tappa attiva: **V47 [~] continuo** (passi ∞), MIG1 [~]. Base legacy 08/11 e vocabolario intoccati.
- Gate V47: ctest verde, `gate_t1.sh` exit 0, replay IDENTICO, Pi 400 <=12 ms/frame. Gate non rieseguito dal capo.
- SCAMBIO_RECENTE CHK auto [APERTE] = controlli automatici, non peer review/CP.

## Rilievi
- **D20 (REGOLA BLOCCATA, ANDREA_REVIEW §2):** jr VJ5 10:32 conferma 5 hunk in `drawPlayerCar` ancora presenti (`renderer.cpp:2798-2816,:2875-2889,:2890-2905,:2910-2922,:2932-2952`) — area intoccabile, serve autorizzazione Andrea o revert. Dev 10:25 (lv_02 giro40) dichiara nessuna modifica a codice/test/dati; prove dev: percorso exit 0 (2476f,41.3s,0% fuori), smoke exit 0 medio 0.630 max 2.993ms frame 0, replay IDENTICO 2961894142. Jr 10:32: run_tests 27/27 exit 0, prova_gate lv_03_foglie curve exit 0. **DEFAULT (proseguo senza aspettare):** @developer revert/ottiene autorizzazione per i 5 hunk drawPlayerCar bloccati prima di altri cambi in quell'area; @developer-junior verifica indipendente. Nessuna altra modifica a drawPlayerCar/auto giocatore.
- **VJ3 lv_05_spray FAIL 50/60 Hz:** 2.40%>2.00% deterministico (da jr/STATO precedente); altri 5 livelli OK. Da indagare, non chiude VJ3.

## Deleghe (ruoli 4.7)

| Task | Responsabile | Revisore indipendente | Criteri misurabili | Esito verifiche (handoff) | Stato proposto |
|---|---|---|---|---|---|
| D20: revert/autorizzazione hunk drawPlayerCar; poi V9 ∞ lv_03_foglie | @developer (`UFFICIO/out/developer/`) | @developer-junior | 5 hunk drawPlayerCar risolti (revert o autorizzazione Andrea registrata); run_tests exit 0; smoke+percorso lv_02 replay IDENTICO; poi lv_03 percorso/smoke exit 0, replay IDENTICO, ms log | 26/11 10:25: lv_02 giro40, nessuna mod codice, percorso exit 0, smoke exit 0 medio 0.630 max 2.993ms, replay IDENTICO 2961894142; jr segnala 5 hunk drawPlayerCar bloccati | BLOCCATO da D20 (DEFAULT: risolvere D20 prima) |
| Verifica D20; indagine VJ3 lv_05; VJ5 ∞ prossimo diff | @developer-junior (`UFFICIO/out/developer-junior/`) | @developer (se fix) / @capo verifica | `run_tests.sh` prima prova; review file/riga in `REVIEW_V47.md`; no src; ripetere/log VJ3 lv_05 | 26/11 10:32: VJ5 5 hunk drawPlayerCar bloccanti; drawEffettiAuto bit-identico; run_tests 27/27 exit 0; lv_03 curve exit 0; VJ3 lv05 FAIL aperto | IN CORSO |
| VG3 ∞ PNG vs `.sglraw` scenari | @grafico (`UFFICIO/out/grafico/`) | @capo verifica catalogo/licenza/provenienza (no peer review tecnica) | Solo script catalogo; converti solo PNG più nuovo; no ritocco area Andrea; no risorse esterne non verificate | 26/11 09:27 g212: 0 conv, 6/6 skyline, 165 PNG, 155 raw/155 tracciati 0 senza voce, 0 da decidere, autotest 28/27/24 OK, exit 0 | IN CORSO |
| Coordinamento CP/registro | @capo (`UFFICIO/out/capo/`) | — (non revisore tecnico) | REGISTRO append-only; STATO/STATI allineati; chiusura CP solo con gate+review indipendenti in dossier | Nessun CP chiudibile; D20 e VJ3 lv05 aperti | APERTO |

## Blocchi / decisioni
- **D20 [BLOCCO PROCEDURALE]:** 5 hunk in drawPlayerCar (jr 10:32), REGOLA BLOCCATA ANDREA_REVIEW §2. Non in §7; capo non scrive ANDREA_REVIEW. DEFAULT: revert/autorizzazione, verifica jr, poi V9. Proseguo senza aspettare.
- **VJ3 lv_05 50/60 Hz:** diff 2.40% deterministico; @developer indaga. DEFAULT: FAIL aperto, non chiudere VJ3.
- **DEFAULT:** `traffico_incrocio/lato_furgone_bianco.png.png` — NON rinominare/convertire senza RISPOSTA DI ANDREA.
- **Pi 400 non verificato** (blocco H sospeso). Nessuna peer review V47 registrata in dossier CP → no chiusura.
- Nessun PER_CLAUDE. Nessun FINITO (passi ∞ attivi; D20 e VJ3 lv05 aperti).