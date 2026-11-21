# STATI PROPOSTI — Turno 1 (ora 0 di 12), 2026-11-21, @capo

**Tappa attiva:** v4.7 MIG1 [~] + V47 [~] continuo (passi ∞). Gate/chiusure tappa solo con gate+review indipendenti registrati (autore≠revisore). **Base legacy 08/11 e vocabolario intoccati** (ANDREA_REVIEW precedenza assoluta). G1 ANNULLATO; sponsor/cartelloni solo Andrea. Nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: nessuna. Gate non rieseguito dal capo.

## Stati proposti (evidenza)

| Voce | Stato | Evidenza (file) | Nota breve |
|---|---|---|---|
| CP-006 | CHIUSO | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | CHIUSI | `UFFICIO/Checkpoint Revision/REGISTRO_REVISIONI.md` | — |
| CP-002/003/004, DECO1, F60 | APERTI [~] | `UFFICIO/Checkpoint Revision/CP-002..004.md` | Mancano gate+review indipendenti in dossier; nessuna chiusura. |
| SCAMBIO_RECENTE (CHK auto) | [APERTA] | `SCAMBIO_RECENTE.md` | Controlli verdi, non peer review in dossier. |
| V9 ∞ (@developer) | [~] | `UFFICIO/out/developer/ULTIMO_TURNO.md` (2026-11-21 14:13 in STATO_CORRENTE) | Prossimo: **lv_03_foglie 25°** (`percorso`/`smoke`, replay IDENTICO, exit 0, ms medio/max). No legacy/vocabolario/costanti guida/sponsor. |
| VJ3 ∞ | [~] 4/6 | `UFFICIO/out/developer-junior/REVIEW_V47.md` | Mancano lv_05_spray, lv_06_desktop (50/60 Hz). |
| VJ4 ∞ | [x] 6/6 | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` | Soak 6/6 chiuso (picco lv_06 95.641 ms per V9). |
| VJ5 ∞ | [~] | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (20/11 14:04) | Review diff `renderer.cpp` hunk V9 lv_01 26° ok; prossimo VJ5 su prossimo diff `src/` (ricompila prima). |
| VG3 ∞ (@grafico) | [~] | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (20/11 10:18) | Solo script confronto/conversione con script catalogo, niente disegni. Blocco noto: `lato_berlina_verde.sglraw` senza PNG → **DEFAULT: attendere Andrea, invariato**. |

## Checkpoint
- Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere. Nessuna apertura/chiusura in questo turno.

## Deleghe (ruoli 4.7, AGENTS.md)

| Ruolo | Handoff | Responsabile | Revisore indipendente | Consegna/criteri (breve) | Prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞ lv_03_foglie 25°: `percorso`/`smoke`, exit, ms medio/max, replay IDENTICO; se >8 ms alleggerire senza cambiare resa/replay. | Exit 0, replay IDENTICO, misure in log (`UFFICIO/Officina/Test area/`). |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md` | developer-junior | developer | VJ5 ∞ su prossimo diff `src/` (file/riga); VJ3 4/6 se utile; prove solo in `motore_nuovo/test/*.cpp` o tool assegnati, no `src/` edit. | Righe REVIEW_V47 + exit/replay/ms. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | Capo verifica handoff | VG3 ∞: confronta PNG vs `.sglraw`, converti solo se PNG più nuovo con script catalogo, nessun ritocco disegno. Blocco berlina verde: **DEFAULT invariato**. | Comandi+exit, elenco file o “0 da convertire”. |

## Vincoli
- Capo scrive solo `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*`. Percorsi relativi. `REGISTRO_REVISIONI.md` append-only.
- `RICHIESTA.md`/`SCAMBIO.md` sola lettura. Nessun PER_CLAUDE, nessun FINITO (∞). Pi 400 non verificato.