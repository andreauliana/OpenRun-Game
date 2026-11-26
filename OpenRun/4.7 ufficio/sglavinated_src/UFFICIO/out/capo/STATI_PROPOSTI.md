# STATI PROPOSTI — @capo (Turno 1, ora 1/12, 2026-11-26)

## Contesto
- Fonti lette: TAPPE.md, ANDREA_REVIEW.md (prec. ass., §7 "Nessuna" decisione aperta; G1 ANNULLATO/sponsor solo Andrea; legacy 08/11+vocabolario intoccati; REGOLA BLOCCATA drawPlayerCar §2/§6), DIREZIONE_ARTISTICA.md, CONTROLLO.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
- Checkpoint: STATO_CORRENTE + coda REGISTRO_REVISIONI letti. CP-006 CHIUSO (no riaprire). CP-001/005/007 CHIUSI. CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier assenti). **Nessun CP con gate+review indipendenti registrati → nessuna chiusura.**
- Handoff citati (capo non riesegue): dev `UFFICIO/out/developer/ULTIMO_TURNO.md` 26/11 giro39 lv_05_spray; jr `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 26/11 08:29 VJ3+VJ5; gf `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 26/11 08:22 g207; Officina `STATUS_OFFICINA.md` arretrato (18/11).

## Tappa/gate
- Tappa attiva: **V47 [~] continuo** (passi ∞), MIG1 [~]. Base legacy 08/11 e vocabolario intoccati.
- Gate V47: ctest verde, `gate_t1.sh` exit 0, replay IDENTICO, Pi 400 <=12 ms/frame. Gate non rieseguito dal capo.
- SCAMBIO_RECENTE CHK 4-6/1 auto [APERTE] = controlli automatici, non peer review/CP.

## Rilievo bloccante
- **D20 (blocco procedurale, REGOLA BLOCCATA Andrea 06/11, ANDREA_REVIEW §2):** jr VJ5 segnala hunk `drawPlayerCar` :2628-2642/:2715-2727 ancora in area bloccata ("autorizzazione o revert"). Dev giro38c lo ha toccato (bit-identico/replay IDENTICO) ma dev giro39 è passato a lv_05 drawGalleria (area non bloccata). D20 **non è registrata in ANDREA_REVIEW §7** (capo non può scriverlo; solo Andrea/Claude). **DEFAULT (proseguo senza aspettare):** @developer revert solo hunk drawPlayerCar bloccati, tenere hunk 1 `drawDetrito` se presente; `run_tests.sh` + smoke/percorso lv_02 replay IDENTICO; @developer-junior verifica indipendente. Nessuna altra modifica a drawPlayerCar/auto giocatore.
- **VJ3 lv_05_spray FAIL 50/60 Hz:** 48.67s/47.50s, diff 2.40%>2.00%, deterministico (ripetizione identica). Altri 5 livelli OK (0.01-0.21%). Da indagare (@developer), non chiude VJ3.

## Deleghe (ruoli 4.7)

| Task | Responsabile | Revisore indipendente | Criteri misurabili | Esito verifiche (handoff, capo non riesegue) | Stato proposto |
|---|---|---|---|---|---|
| V9 ∞ — prima sbloccare D20: revert hunk drawPlayerCar bloccati; poi lv_06_desktop giro39 | @developer (`UFFICIO/out/developer/`) | @developer-junior | revert solo hunk drawPlayerCar; run_tests/build exit 0; smoke+percorso lv_02 replay IDENTICO; poi lv_06 percorso/smoke exit 0, replay IDENTICO, ms log; indagare VJ3 lv_05 2.40% | 26/11: giro39 lv_05_spray drawGalleria exit 0, replay IDENTICO 4250821778, medio 0.679 max 9.828ms (rumore), percorso 2920f/48.7s/0%; regressione lv_02 hash invariato. D20 drawPlayerCar ancora segnalato da jr | BLOCCATO da D20 (DEFAULT revert prima di nuovo V9) |
| VJ3 lv_05 FAIL — indagine/verifica fix; VJ5 ∞ verifica revert D20 | @developer-junior (`UFFICIO/out/developer-junior/`) | @developer (se fix codice) / @capo verifica completezza | `run_tests.sh` prima prova; ripetere frequenza lv_05 con log; review file/riga in `REVIEW_V47.md`; no src | 26/11 08:29: VJ3 6/6 eseguiti, lv_05 60/50Hz 48.67/47.50s diff 2.40% FAIL deterministico, altri 5 OK; VJ5 drawImboccoGalleria corretto/bit-identico; run_tests 27/27 exit 0; drawPlayerCar bloccante ancora aperto | IN CORSO |
| VG3 ∞ PNG vs `.sglraw` scenari | @grafico (`UFFICIO/out/grafico/`) | @capo verifica catalogo/licenza/provenienza (no peer review tecnica) | Solo script catalogo; converti solo PNG più nuovo; no ritocco area Andrea; no risorse esterne non verificate | 26/11 08:22 g207: 0 conv, 6/6 skyline, 165 PNG, 155 raw/155 tracciati 0 senza voce, 3 PNG noti non-asset 0 da decidere, autotest 26 OK, altri script exit 0 | IN CORSO |
| Coordinamento CP/registro | @capo (`UFFICIO/out/capo/`) | — (non revisore tecnico) | REGISTRO append-only; STATO/STATI allineati; chiusura CP solo con gate+review indipendenti in dossier | Nessun CP chiudibile; D20 e VJ3-lv05 aperti | APERTO |

## Blocchi / decisioni
- **D20 [BLOCCO PROCEDURALE]:** drawPlayerCar hunk bloccato da REGOLA BLOCCATA (ANDREA_REVIEW §2). Non registrato in §7 (capo non può scrivere ANDREA_REVIEW.md). DEFAULT: revert hunk bloccati, verifica jr, poi proseguire V9. Proseguo senza aspettare.
- **VJ3 lv_05 50/60 Hz:** diff 2.40% deterministico; @developer indaga (gioco vs euristica autopilota sogliaX prova_gate.cpp:3865, come da jr). DEFAULT: trattare come FAIL aperto, non chiudere VJ3 finché spiegato/fixato.
- **DEFAULT esistente:** `traffico_incrocio/lato_furgone_bianco.png.png` — NON rinominare/convertire senza RISPOSTA DI ANDREA.
- **Pi 400 non verificato** (blocco H sospeso). Peer review V47 non registrate in dossier → no chiusura CP.
- Nessun PER_CLAUDE. Nessun FINITO (passi ∞ attivi; D20 e VJ3-lv05 aperti).