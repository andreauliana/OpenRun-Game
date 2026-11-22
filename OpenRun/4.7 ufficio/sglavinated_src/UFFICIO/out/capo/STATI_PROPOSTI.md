# STATI PROPOSTI (capo) — 2026-11-22 Turno 2 (ora 3 di 12)

Tappa attiva: MIG1 [~] + V47 [~] (passi ∞). Base legacy 08/11 e vocabolario NON si toccano (ANDREA_REVIEW precedenza assoluta). Gate T1 storico verde (CP-006 CHIUSO); nessun gate rieseguito dal capo. SCAMBIO_RECENTE: CHECKPOINT auto [APERTE], non peer review in dossier. Nessun CP con gate + review indipendente (autore≠revisore) registrati → nessuna chiusura.

## Stati proposti
- MIG1: [~] (nessun nuovo passo MIG1 in RICHIESTA_APERTA; focus V47∞).
- V47: [~] continuo (passi ∞). Nessun FINITO: esistono passi ∞ attivi.

## Deleghe (ruoli 4.7, percorsi UFFICIO/out/<ruolo>/)
- @developer (V9 ∞, `UFFICIO/out/developer/`): lv_01_neon 31°. Gate percorso/smoke, replay IDENTICO, exit 0, ms; se >8 ms alleggerire senza cambiare resa/replay. Solo `motore_nuovo/src/**`/CMake/dati/tool assegnati; no `GRAFICA/`. Handoff: `ULTIMO_TURNO.md`.
- @developer-junior (VJ4 poi VJ5 ∞, `UFFICIO/out/developer-junior/`): VJ5 ∞ sul prossimo diff src (ricompilare prima `run_tests.sh`); VJ3 4/6 se utile. Prove solo test/tool assegnati, no `src/`. Handoff: `ULTIMO_TURNO.md`, `REVIEW_V47.md`.
- @grafico (VG3 ∞, `UFFICIO/out/grafico/`): confronto PNG vs `.sglraw` in `GRAFICA/scenari/`; convertire solo se PNG più nuovo via script catalogo, nessun ritocco. Blocchi Andrea DEFAULT invariati (skyline lv_04 mancante/raw ok, `lato_furgone_bianco.png.png`). Handoff: `STATUS_GRAFICO_PER_CAPO.md`.

## Blocchi/decisioni
- Nessuna “RISPOSTA DI ANDREA” nuova (ANDREA_REVIEW). Nessuna domanda nuova; se servisse: porre in ANDREA_REVIEW con DEFAULT e proseguire.
- Pi 400 non verificato (macOS). Grafico: `ULTIMO_TURNO.md` permesso negato; usare `STATUS_GRAFICO_PER_CAPO.md`.
- CP-002/003/004 APERTI; CP-005/006/007 CHIUSI; DECO1/F60 citati APERTI (dossier non in cartella). Nessuna chiusura.