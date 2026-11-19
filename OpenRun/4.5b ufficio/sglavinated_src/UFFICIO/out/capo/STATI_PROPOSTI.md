# STATI PROPOSTI — Turno 1 (ora 0 di 12), coordinamento 19/11/2026, @capo

**Tappa attiva:** v4.7 — **MIG1 [~] (review)** + **V47 [~] continuo** (passi ∞). Gate T1 storico verde (**CP-006 CHIUSO, non riaprire**). Base legacy 08/11 e vocabolario **NON si toccano** (`ANDREA_REVIEW.md` precedenza assoluta). G1 ANNULLATO (sponsor/cartelloni solo Andrea); nessuna `RISPOSTA DI ANDREA`; decisioni aperte: nessuna. Gate non rieseguito dal capo. `SCAMBIO_RECENTE.md` CHECKPOINT 1-2-3 [APERTE]: controlli automatici verdi (**non peer review in dossier**). Nessun CP con gate+review indipendenti (autore≠revisore) registrati da chiudere.

## Stati proposti

| Blocco/voce | Stato | Evidenza (handoff/log/comando) | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSO** | `UFFICIO/Checkpoint Revision/REGISTRO_REVISIONI.md` | — |
| CP-002/003/004, DECO1, F60 | **APERTI [~]** | dossier in `UFFICIO/Checkpoint Revision/` | Review indipendenti mancanti; nessuna chiusura. |
| MIG1 (review) | **[~]** | archiviato `UFFICIO/archivio_per_claude/RICHIESTA_BLOCCHI_CHIUSI_FINO_18NOV.md`, handoff dev/junior | Costanti guida/vocabolario/legacy intoccabili; review incrociate non consolidate. |
| V47 V1-V5, VJ1, VG2 | **[x] handoff / [~]** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | V6 review da consolidare. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` 18/11 17:25 | lv_06_desktop 20°: build exit 0; `percorso` exit 0 pre/post (2113f, 35.2s, 0% fuori); smoke pre exit 0 medio 1.229 max 8.141 (>8), post exit 0 medio 1.170 max 3.947, replay IDENTICO. `gate_t1.sh` non eseguito. **Prossimo:** lv_01_neon 21°. |
| V47 VJ3 ∞ | **[~] 4/6** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | lv_01-lv_04 50/60 Hz exit 0, replay IDENTICO; lv_03 delta 4.70% segnalato. **Prossimi:** lv_05_spray, lv_06_desktop (ricompila prima). |
| V47 VJ4 ∞ | **[x] 6/6** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 17/11 | Soak 6/6 exit 0, replay IDENTICO, nessun crash (ripartibile se utile). |
| V47 VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 18/11 17:29, `REVIEW_V47.md` | Review `renderer.cpp` (44+/23-, `git diff --stat` exit 0): `copertura_` grow-only possibile semplificazione, cache maschera graffito per identità puntatore, reserve/puntatori stabili e rialloc (`spriteOrd_`/`terraOrdine_`>2048) da verificare; nessuna prova. Backlog warning/fragilità aperto. |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 18/11 16:14 | Giro 94: 3 script exit 0, 0 da convertire (145 PNG/133 confrontati/12 non motore, 86 raw scenari+47 altri, 142 tracciati catalogo, 126 pari, pose 2/2). 1 noto bloccato Andrea (`car-custom-v2.png`), zero esterni/Sega. |

## Checkpoint
- Nessun CP con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere. Nessuna apertura/chiusura.
- SCAMBIO_RECENTE CHECKPOINT 1-3 [APERTE]: esiti automatici verdi, **non equivalgono a peer review in dossier**.
- `UFFICIO/Officina/STATUS_OFFICINA.md` letto (handoff tecnico sintetico, datato 18/11 pulizia; più recenti i `ULTIMO_TURNO` dev/junior): **non prova di approvazione/review**.

## Deleghe (ruoli 4.6)

| Ruolo | Consegna (`UFFICIO/out/<ruolo>/`) | Responsabile | Revisore indipendente | Percorsi/criteri | Gate/prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: **lv_01_neon 21° giro** — gate `percorso`/`smoke`, exit code, ms/frame medio/max, replay IDENTICO; se >8 ms alleggerire **senza cambiare resa/replay**. Valutare rilievi junior VJ5 su `renderer` solo se compatibili e senza toccare legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. No `gate_t1.sh` completo salvo richiesta. | Exit code 0, replay IDENTICO, misure ms/frame, log in `UFFICIO/Officina/Test area/`. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md` | developer-junior | developer | VJ3 **lv_05_spray o lv_06_desktop** 50/60 Hz (ricompila prima; non usare `prova_gate` vecchio), exit code, replay IDENTICO, delta tempi; oppure VJ5 ∞ su altro cambio `src/`/consolidare review V6/MIG1/V47 autore≠revisore con file/riga/prove. VJ4 6/6 completato (ripartibile se utile). Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/` prodotto. | Righe in `REVIEW_*` (file/riga); eventuale exit code, replay IDENTICO, ms/frame o delta 50/60. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (se materiale) / Capo verifica handoff | VG3 ∞: confronta date/contenuto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero risorse esterne/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli
- Capo scrive solo in `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*`. Percorsi relativi. Nessun file in radice. `REGISTRO_REVISIONI.md` append-only.
- Legacy 08/11, vocabolario, costanti di guida, HUD/auto/partenza/tracciati/audio, sponsor/cartelloni: **intoccabili**. `RICHIESTA.md` e `SCAMBIO.md` sola lettura.
- `FINITO` non creato (passi ∞, blocchi non tutti [x]). `PER_CLAUDE.md` non creato. Domande ad Andrea: nessuna (DEFAULT prosegue; VJ2 non blocca). Pi 400 non verificato.
