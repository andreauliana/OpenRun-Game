# STATI PROPOSTI (capo) — 2026-11-23 Turno 3 (ora 4 di 12)

Tappa attiva: **MIG1 [~] / V47 [~] continuo (passi ∞)**. Gate Tappa 1 (TAPPE.md): base legacy 08/11 intoccabile — ANDREA_REVIEW precedenza assoluta. Nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: 0. SCAMBIO_RECENTE: CHK auto [APERTE], non peer review. Capo non riesegue gate. Nessun CP con gate+review indipendenti registrati → nessuna chiusura. FINITO: no (esistono passi ∞).

## Stati proposti
- MIG1: [~] — passi numerati esauriti; lavoro su V47 ∞.
- V47: [~] — continuo, un passo ∞ per turno per ruolo.

## Checkpoint
- CP-005/006/007: CHIUSI (non riaprire).
- CP-002/003/004: APERTI — review indipendente/prova mancante; nessuna chiusura.
- DECO1/F60: citati APERTI in STATO_CORRENTE ma dossier assenti in cartella — lasciare APERTI, non inventare dossier.

## Handoff letti (evidenze dichiarate, non rieseguite dal capo)
- Developer `UFFICIO/out/developer/ULTIMO_TURNO.md` 23/11 10:06: V9 lv_05_spray 33°; build exit 0; smoke/percorso exit 0, replay IDENTICO (frameHash 4250821778), medio 0.603 max 3.022 ms, percorso 2920f 0% fuori; gate_t1 exit 0 6/6 "TUTTO OK". Prossimo lv_06_desktop 33°.
- Developer-junior `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 23/11 10:38: VJ5 ∞ review `Renderer::drawImboccoGalleria()` (lv_02, renderer.cpp:1732-1738); hoist dx0/dx1/pilastro corretto, resa/replay identici; rilievo non bloccante (guardare `if(inFronte)`, precalcolo cx±fronteW). `run_tests.sh` exit 0 ctest 27/27 (135.38s); prova_gate lv_02_atomico percorso exit 0, replay/GATE OK. VJ4 chiuso 6/6.
- Grafico `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 23/11 10:01: VG3 giro 167, 0 conversioni; vg3_date_png_raw.py exit 1 solo flag furgone `.png.png` area Andrea; vg3_raw_senza_catalogo.py exit 0/--autotest exit 0 (11 OK); vg3_png_non_catalogati.py exit 0. Nessun asset/catalogo modificato.

## Deleghe (AGENTS 4.7, percorsi UFFICIO/out/<ruolo>/)
| Task | Responsabile | Revisore indipendente | Criteri completamento |
|---|---|---|---|
| V9 ∞ lv_06_desktop 33°: gate `percorso`/smoke, ms/frame; se >8ms Mac alleggerire senza cambiare resa/replay | @developer | @developer-junior (VJ5 review diff src) | replay IDENTICO, exit 0, misure in `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| VJ5 ∞: review prossimo cambio @developer in `src/` (file/riga/livello diverso); ricompila prima | @developer-junior | @developer (peer su prove/test) | `run_tests.sh` prima di prove, esito/exit in `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` e `REVIEW_V47.md`; no modifica src |
| VG3 ∞: confronta PNG vs `.sglraw` in `GRAFICA/`; converti solo se PNG più nuovo con script catalogo | @grafico | @capo verifica handoff/catalogo (non peer review tecnica) | nessun ritocco, 0 conversioni inutili; stato in `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |

## Blocchi / decisioni
- Nessuna domanda nuova per Andrea. Se serve: ANDREA_REVIEW.md con DEFAULT, poi proseguire; `RISPOSTA DI ANDREA` prevale sul DEFAULT.
- Pi 400 non verificato (blocco H sospeso, macOS).
- Grafico: `.png.png` furgone incrocio e 3 `car-custom*`/4 raw sx mancanti/5 pose `_b` = area Andrea, DEFAULT non toccare.
- CP-002/003/004/DECO1/F60: restano APERTI; nessuna review indipendente registrata in dossier.
