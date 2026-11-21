# STATI PROPOSTI — Turno 1 (ora 1 di 12), 2026-11-21, @capo

**Tappa attiva:** v4.7 MIG1 [~] + V47 [~] continuo (∞). Chiusura solo con gate+review indipendenti registrati (autore≠revisore). **Base legacy 08/11 e vocabolario intoccati** (ANDREA_REVIEW precedenza assoluta). G1 ANNULLATO; sponsor/cartelloni solo Andrea. Nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: nessuna. Gate non rieseguito dal capo.

## Stati proposti (evidenza)

| Voce | Stato | Evidenza (file) | Nota breve |
|---|---|---|---|
| CP-006 | CHIUSO | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | CHIUSI | `UFFICIO/Checkpoint Revision/REGISTRO_REVISIONI.md` | — |
| CP-002/003/004 | APERTI [~] | `UFFICIO/Checkpoint Revision/CP-002..004.md` | Manca review indipendente/gate in dossier; nessuna chiusura. |
| DECO1, F60 | APERTI [~] (citati) | `STATO_CORRENTE.md` | Dossier dedicato non presente in cartella; non chiudere senza evidenza. |
| SCAMBIO_RECENTE CHK auto | [APERTA] | `SCAMBIO_RECENTE.md` | Controlli verdi, non peer review in dossier. |
| V9 ∞ (@developer) | [~] | `UFFICIO/out/developer/ULTIMO_TURNO.md` (21/11 11:48) | lv_01_neon 28° fatto: build exit 0, smoke exit 0 medio 0.593 max 2.911 ms post, percorso exit 0 1989f/33.2s/0% fuori, replay IDENTICO (frameHash 0 3594390208), gate_t1 exit 0 6/6 (lv_01 medio 0.714 max 6.480 ms). Prossimo: **lv_02_atomico 28°**. |
| VJ3 ∞ | [~] 4/6 | `UFFICIO/out/developer-junior/REVIEW_V47.md` | Mancano lv_05_spray, lv_06_desktop (50/60 Hz). |
| VJ4 ∞ | [x] 6/6 | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` | Soak 6/6 chiuso. |
| VJ5 ∞ (@developer-junior) | [~] | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (21/11 11:50) | Review hunk `drawTerraSospesa` onde riga (lv_01 28°): nessun errore, resa/replay IDENTICI. `run_tests.sh` exit 0 27/27; prova_gate percorso lv_01 exit 0. Prossimo VJ5 su prossimo diff `src/`. |
| VG3 ∞ (@grafico) | [~] | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (21/11 11:56) | Giro 136, 0 da convertire. Blocco: `GRAFICA/sprite/traffico_incrocio/lato_furgone_bianco.png.png` senza raw/catalogo → **DEFAULT: attendere Andrea, invariato**. |

## Checkpoint
- Nessun CP con gate+review indipendenti (autore≠revisore) registrati in dossier da chiudere. Nessuna apertura/chiusura in questo turno.

## Deleghe (ruoli 4.7, AGENTS.md)

| Ruolo | Handoff | Responsabile | Revisore indipendente | Consegna/criteri (breve) | Prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞ lv_02_atomico 28°: `percorso`/`smoke`, exit, ms medio/max, replay IDENTICO; se >8 ms alleggerire senza cambiare resa/replay. No legacy/vocabolario/costanti guida/sponsor. | Exit 0, replay IDENTICO, misure in log (`UFFICIO/Officina/Test area/`). |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md` | developer-junior | developer | VJ5 ∞ su prossimo diff `src/` (file/riga, rischio, semplificazione); VJ3 4/6 se utile. Prove solo in `motore_nuovo/test/*.cpp` o tool assegnati, no edit `src/`. Ricompila prima della prova. | Righe REVIEW_V47 + exit/replay/ms. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | Capo verifica handoff | VG3 ∞: confronta PNG vs `.sglraw`, converti solo se PNG più nuovo con script catalogo, nessun ritocco disegno. Blocco furgone_bianco.png.png: **DEFAULT attendere Andrea**. | Comandi+exit, elenco file o “0 da convertire”. |

## Vincoli / decisioni
- Capo scrive solo `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*`. Percorsi relativi. `REGISTRO_REVISIONI.md` append-only.
- `RICHIESTA.md`/`SCAMBIO.md` sola lettura. Nessun PER_CLAUDE, nessun FINITO (∞). Pi 400 non verificato.
- Domande ad Andrea: nessuna nuova. Se serve: porre in `ANDREA_REVIEW.md` con DEFAULT e proseguire (nessuna ora).