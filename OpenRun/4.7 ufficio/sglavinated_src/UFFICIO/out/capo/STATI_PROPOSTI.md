# STATI PROPOSTI — @capo (T3, ora 5/12, 2026-11-25)

## Contesto
- Fonti lette: TAPPE.md, ANDREA_REVIEW.md (prec. ass., nessuna `RISPOSTA DI ANDREA`; G1 ANNULLATO/sponsor solo Andrea; legacy 08/11+vocabolario intoccati; decisioni aperte 0), DIREZIONE_ARTISTICA.md, CONTROLLO.md, AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md, STATO_CORRENTE, REGISTRO (coda).
- Handoff citati (capo non riesegue): dev `UFFICIO/out/developer/ULTIMO_TURNO.md` 25/11 12:36; jr `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 25/11 12:45; gf `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 24/11 13:20; STATUS_OFFICINA arretrato 18/11.

## Tappa/gate
- Tappa attiva: MIG1 [~] + V47 [~] continuo (∞). Gate V47: ctest verde, `gate_t1.sh` exit 0, replay IDENTICO, Pi 400 <=12 ms/frame. Gate non rieseguito dal capo.
- SCAMBIO_RECENTE CHK auto [APERTE] = controlli automatici, non peer review. CP-006 CHIUSO (no riaprire). Nessun CP con gate+review indipendenti registrati → **nessuna chiusura**.

## Deleghe (ruoli 4.7)

| Task | Responsabile | Revisore indipendente | Criteri | Esito verifiche (handoff) | Stato |
|---|---|---|---|---|---|
| V9 ∞ lv_06_desktop giro38b | @developer | @developer-junior | percorso/smoke exit 0, replay IDENTICO, ms/frame; >8ms Mac alleggerire senza cambiare resa/replay | 25/11 12:36: lv_05_spray giro38b ok, pre/post percorso+smoke exit 0, replay IDENTICO 4250821778 (9/9), 0.590/2.960ms (<8ms), lv_02 invariante 2961894142; gate_t1 ref 24/11 10:03 exit 0 | IN CORSO |
| VJ4 soak lv_05_spray poi lv_06_desktop; VJ5 ∞ review prossimo diff src | @developer-junior | @developer se fix / @capo verifica | `run_tests.sh` exit 0 prima prova; soak exit 0 replay IDENTICO ms/frame; review file/riga in `REVIEW_V47.md`; no src | 25/11 12:45: VJ4 soak lv_04 exit 0, 30000f, replay IDENTICO, 0.660/2.757ms <20; run_tests 27/27 exit 0; VJ5 review renderer.cpp/renderer.h 12:41:39 nit non bloccante skylineLut_ | IN CORSO |
| VG3 ∞ PNG vs `.sglraw` scenari | @grafico | @capo verifica catalogo/licenza/provenienza | Solo script catalogo; converti solo PNG più nuovo; no area Andrea; no risorse esterne non verificate | 24/11 13:20 g183: 0 conv; vg3_date exit 1 (solo flag Andrea), vg3_raw_senza_catalogo exit 0, vg3_png_non_catalogati exit 0, autotest 18 OK | IN CORSO |
| Coordinamento CP/registro | @capo | — (non revisore tecnico) | REGISTRO append-only; STATO/STATI/RIASS/diario allineati; chiusura solo con gate+review indie in dossier | Nessun CP chiudibile; CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier assenti) | APERTO |

## Blocchi/decisioni
- DEFAULT Andrea (nessuna RISPOSTA): `traffico_incrocio/lato_furgone_bianco.png.png` — NON rinominare/convertire senza RISPOSTA.
- Pi 400 non verificato (H sospeso). Peer review V47 non registrate in dossier → no chiusura CP.
- Residuo: VJ4 soak lv_01 max 39.042ms frame 17884 >20 output ma GATE OK/replay IDENTICO/nessun crash (monitorare); VJ4 lv_04 OK <20.
- Nessun PER_CLAUDE, nessuna domanda nuova. Nessun FINITO (∞).
