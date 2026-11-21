# STATI PROPOSTI — Turno 1 (ora 0 di 12), 21/11/2026, @capo

**Tappa attiva:** v4.7 — **MIG1 [~] (review)** + **V47 [~] continuo** (passi ∞). Gate tappa: TAPPE.md riga 7 (T1 storica) non applicabile come chiusura V47; V47/MIG1 chiusi solo a gate+review indipendenti registrati. **Base legacy 08/11 e vocabolario intoccati** (ANDREA_REVIEW precedenza assoluta). G1 ANNULLATO; sponsor/cartelloni solo Andrea. Nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: nessuna. Gate non rieseguito dal capo.

## Stati proposti

| Voce | Stato | Evidenza | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSI** | `UFFICIO/Checkpoint Revision/REGISTRO_REVISIONI.md` | — |
| CP-002/003/004, DECO1, F60 | **APERTI [~]** | dossier `UFFICIO/Checkpoint Revision/` | Mancano gate/review indipendenti in dossier; nessuna chiusura. |
| SCAMBIO_RECENTE CHK auto (fino CHK2 20/11 13:54) | **[APERTA]** | `SCAMBIO_RECENTE.md` | Controlli verdi, **non peer review in dossier**. |
| MIG1 (review) | **[~]** | handoff dev/junior | Costanti guida/vocabolario/legacy intoccabili; review incrociate non consolidate. |
| V47 V1-V5, VJ1, VG2 | **[x]/[~]** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` (20/11 14:13) | lv_02_atomico 25°: `cmake --build ... --target prova_gate -j4` exit 0; smoke exit 0 medio 0.793 max 3.168 ms replay IDENTICO; percorso exit 0 (2476f, 41.3s, 0% fuori); `gate_t1.sh` exit 0 6/6. **Prossimo:** lv_03_foglie 25°. |
| V47 VJ3 ∞ | **[~] 4/6** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | Mancano lv_05_spray, lv_06_desktop 50/60 Hz. |
| V47 VJ4 ∞ | **[x] 6/6** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (20/11 11:31) | Soak lv_06 exit 0, 30000f, replay 2x IDENTICO, medio 1.381 max 95.641 ms; ctest 27/27. Picco 95.641 ms segnalato per V9. |
| V47 VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (20/11 14:04) | Review diff `renderer.cpp` hunk V9 lv_01 26° (`giroMenoFascia`), nessun errore; `run_tests.sh` exit 0 ctest 27/27 (226.16s); percorso lv_01 exit 0. **Prossimo:** VJ5 su prossimo diff `src/`. |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (20/11 10:18) | Giro 116: `lato_bus_rosso` convertito exit 0; 43 allineati, 0 da convertire. Blocco: `traffico_incrocio/lato_berlina_verde.sglraw` senza PNG (**DEFAULT: attendere Andrea, invariato**). |

## Checkpoint

- Nessun CP con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere. Nessuna apertura/chiusura.
- `UFFICIO/Officina/STATUS_OFFICINA.md` arretrato rispetto agli ULTIMO_TURNO; **non peer review/prova di approvazione**.

## Deleghe (ruoli 4.7, AGENTS.md)

| Ruolo | Handoff | Responsabile | Revisore indipendente | Consegna / criteri | Prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: **lv_03_foglie (25° giro)**. `percorso`/`smoke`, exit, ms/frame medio/max, replay IDENTICO; se >8 ms alleggerire senza cambiare resa/replay. Considerare picco soak lv_06 (95.641 ms) quando toccherà. No legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. | Exit 0, replay IDENTICO, misure; log in `UFFICIO/Officina/Test area/`. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `UFFICIO/out/developer-junior/REVIEW_V47.md` | developer-junior | developer | VJ5 ∞ su prossimo diff `src/` (file/riga; `run_tests.sh`/ricompila prima di prova). VJ3 4/6 (lv_05/lv_06 50/60 Hz) se utile; consolidare review V6/MIG1/V47 autore≠revisore solo se prove registrate. Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/`. | Righe in `REVIEW_V47.md`; exit/replay/ms per prove. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (materiale) / Capo verifica handoff | VG3 ∞: confronta PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**. Blocco: `lato_berlina_verde.sglraw` senza PNG (**DEFAULT: attendere Andrea, invariato**). No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero esterni/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli / decisioni

- Capo scrive solo `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*`. Percorsi relativi. `REGISTRO_REVISIONI.md` append-only.
- `RICHIESTA.md` e `SCAMBIO.md` sola lettura. Nessun CP chiuso. Nessun `PER_CLAUDE.md`. Nessun `FINITO` (passi ∞ attivi). Pi 400 non verificato.
- Domande ad Andrea: nessuna nuova; **DEFAULT prosegue** dove indicato. Se arriva `RISPOSTA DI ANDREA`, prevale sul DEFAULT.