# RIEPILOGO MATTINA — OpenRun v4.7 (2026-11-26)

> Fonte: STATO_CORRENTE, REGISTRO_REVISIONI (ultime righe), handoff dev/jr/grafico, ANDREA_REVIEW.md, RICHIESTA.md. Capo non riesegue gate.

## Fatto per blocco/agente (V47 [~], passi ∞)
| Agente | Ultimo handoff | Fatto | Prove (dichiarate) |
|---|---|---|---|
| @developer | 26/11 10:25, V9 lv_02 giro40 | Nessuna modifica codice/test/dati; controllo lv_02. Prossimo: D20 poi lv_03. | percorso exit 0 (2476f,41.3s,0% fuori); smoke exit 0 medio 0.630 max 2.993ms fr0; replay IDENTICO 2961894142 |
| @developer-junior | 26/11 10:32, VJ5 | Review: confermati 5 hunk `drawPlayerCar` bloccati (REGOLA BLOCCATA §2); drawEffettiAuto bit-identico. VJ3: lv_05 50/60Hz FAIL 2.40% deterministico, altri 5 OK. | run_tests 27/27 exit 0; lv_03 curve exit 0 |
| @grafico | 26/11 09:27, VG3 g212 | Confronto PNG/raw scenari: 0 conversioni, 6/6 skyline, 165 PNG/133 confrontati, 155 raw/155 tracciati 0 senza voce, 0 da decidere. | vg3_raw_senza_catalogo exit 0 + autotest 28 OK; vg3_date_png_raw exit 0 + 27 OK; vg3_png_non_catalogati exit 0 + 24 OK |
| @capo | T2 3/12 | Coordinamento: STATO/STATI/REGISTRO/RIASS/diario aggiornati; 0 CP chiudibili. | Gate non rieseguito dal capo |

## Cosa non funziona / blocchi
- **D20 [BLOCCANTE, REGOLA BLOCCATA ANDREA_REVIEW §2]:** 5 hunk in `renderer.cpp` `drawPlayerCar` (:2798-2816,:2875-2889,:2890-2905,:2910-2922,:2932-2952) — area intoccabile senza autorizzazione Andrea o revert. Dev dichiara nessuna mod ultima, ma hunk restano nel codice. Nessuna peer review indipendente che chiuda il punto.
- **VJ3 lv_05_spray 50/60Hz:** FAIL 2.40% > 2.00% deterministico; altri 5 livelli OK. VJ3 non chiudibile.
- **CP aperti senza chiusura:** CP-002/003/004 APERTI; DECO1/F60 citati senza dossier. Nessun CP ha gate+review indipendenti registrati → 0 chiudibili.
- **Limiti prova:** gate non rieseguito dal capo; Pi 400 non verificato (blocco H sospeso).

## Decisioni aperte in ANDREA_REVIEW.md (le 3 più importanti)
- **ANDREA_REVIEW.md §7: "Nessuna" decisione aperta.** Nessuna voce D<n> [APERTA] registrata.
- **Blocchi operativi che richiedono attenzione Andrea (non in §7, non decisioni registrate):**
  1. **D20 drawPlayerCar:** autorizzare i 5 hunk o chiedere revert (REGOLA BLOCCATA §2). Impatta V9/dev.
  2. **VJ3 lv_05 50/60Hz diff 2.40%:** decidere se accettare tolleranza/indagare come bug o vincolo noto; blocca chiusura VJ3.
  3. **Prova Pi 400 (blocco H sospeso):** nessuna misura reale Pi 400; criterio V47 "<=12 ms/frame" non verificato su target.

## Prossimo passo consigliato
1. **@developer (priorità alta):** risolvere D20 — revert dei 5 hunk `drawPlayerCar` o ottenere/autorizzare per iscritto da Andrea; poi `run_tests.sh` exit 0 e smoke+percorso lv_02 replay IDENTICO. Solo dopo, riprendere V9 ∞ su lv_03.
2. **@developer-junior:** review indipendente della risoluzione D20 (file/riga in `REVIEW_V47.md`); ripetere/indagare VJ3 lv_05 con log.
3. **@capo:** non chiudere CP; registrare esito D20 nel dossier/prossimo registro quando arriva prova+review.