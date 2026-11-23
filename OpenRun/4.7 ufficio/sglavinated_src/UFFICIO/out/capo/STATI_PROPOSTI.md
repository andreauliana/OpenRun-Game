# STATI PROPOSTI (capo) — 2026-11-23 Turno 2 (ora 2 di 12)

Tappa attiva: **MIG1 [~] / V47 [~] continuo (passi ∞)**. Gate Tappa 1 (TAPPE.md): base legacy 08/11 intoccabile (strada scorre, A/B/sterzo) — ANDREA_REVIEW precedenza assoluta. Nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: 0. SCAMBIO_RECENTE: CHK auto [APERTE], non peer review. Capo non riesegue gate. Nessun CP con gate+review indipendenti registrati → nessuna chiusura. FINITO: no (esistono passi ∞).

## Stati proposti
- MIG1: [~] — passi numerati esauriti; lavoro su V47 ∞.
- V47: [~] — continuo, un passo ∞ per turno per ruolo.

## Checkpoint
- CP-005/006/007: CHIUSI (non riaprire).
- CP-002/003/004: APERTI — review indipendente/prova mancante; nessuna chiusura.
- DECO1/F60: citati APERTI in STATO_CORRENTE ma dossier assenti in cartella — lasciare APERTI, non inventare dossier.

## Handoff letti (evidenze dichiarate, non rieseguite dal capo)
- Developer `UFFICIO/out/developer/ULTIMO_TURNO.md` 23/11 06:58: V9 lv_06_desktop 31°; smoke/percorso exit 0, replay IDENTICO (frameHash lv02 2961894142, lv05 4250821778, lv06 3627137594), gate_t1 exit 0 6/6; max <8ms Mac. Prossimo lv_01_neon 32°.
- Developer-junior `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 23/11 07:04: VJ5 review diff renderer ok (resa/replay invariati); `run_tests.sh` exit 0 ctest 27/27 (159.95s); prova_gate lv_02 percorso exit 0. VJ3 4/6, VJ4 6/6.
- Grafico `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 23/11 06:07: VG3 giro 153, 0 da convertire; blocco Andrea DEFAULT `lato_furgone_bianco.png.png` (non azionabile). Nessun asset/catalogo modificato.

## Deleghe (AGENTS 4.7, percorsi UFFICIO/out/<ruolo>/)
| Task | Responsabile | Revisore indipendente | Criteri completamento |
|---|---|---|---|
| V9 ∞ lv_01_neon 32°: gate `percorso`/smoke, ms/frame; se >8ms Mac alleggerire senza cambiare resa/replay | @developer | @developer-junior (VJ5 review diff src) | replay IDENTICO, exit 0, misure in `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| VJ5 ∞ review prossimo diff src; VJ3 4/6 se utile | @developer-junior | @developer (peer su prove/test) | ricompila prima (`run_tests.sh`), esito/exit in `UFFICIO/out/developer-junior/REVIEW_V47.md` e `ULTIMO_TURNO.md`; no modifica src |
| VG3 ∞: confronta PNG vs `.sglraw` in `GRAFICA/`; converti solo se PNG più nuovo con script catalogo | @grafico | @capo verifica handoff/catalogo (non peer review tecnica) | nessun ritocco, 0 conversioni inutili; stato in `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |

## Blocchi / decisioni
- Nessuna domanda nuova per Andrea. Se serve: ANDREA_REVIEW.md con DEFAULT, poi proseguire; `RISPOSTA DI ANDREA` prevale sul DEFAULT.
- Pi 400 non verificato (blocco H sospeso, macOS). Grafico scrive `STATUS_GRAFICO_PER_CAPO.md`, non `ULTIMO_TURNO.md`.
- Grafico: usare solo percorsi relativi nelle cartelle assegnate (CONTROLLO.md: permessi negati ricorrenti).
