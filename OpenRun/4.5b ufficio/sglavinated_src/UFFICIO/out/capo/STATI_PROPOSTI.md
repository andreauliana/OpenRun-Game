# STATI PROPOSTI — Turno 4 (ora 6 di 12), 18/11/2026, @capo

**Tappa attiva:** v4.7 — **BLOCCO MIG1 [~]** (review) + **BLOCCO V47 [~]** continuo (passi ∞). Gate T1 storico verde (**CP-006 CHIUSO, non riaprire**). Base legacy 08/11 approvata e **NON si tocca** (`ANDREA_REVIEW.md`, precedenza assoluta). G1 ANNULLATO (sponsor/cartelloni solo Andrea); nessuna `RISPOSTA DI ANDREA` nuova; decisioni aperte: nessuna. Gate non rieseguito dal capo. `SCAMBIO_RECENTE.md`: CHECKPOINT 1 (17/11 10:16), CHECKPOINT 2 (18/11 15:30) e CHECKPOINT 3 (18/11 17:38) [APERTE], controlli automatici verdi (**non peer review in dossier**). Nessun checkpoint con gate+review indipendenti (autore≠revisore) registrati da chiudere.

## Stati proposti

| Blocco/voce | Stato | Evidenza (handoff/log/comando) | Note |
|---|---|---|---|
| CP-006 (T1-RUN) | **CHIUSO** | `UFFICIO/Checkpoint Revision/CP-006-t1-run-chiusura.md` | Non riaprire. |
| CP-005, CP-007 | **CHIUSO** | `UFFICIO/Checkpoint Revision/REGISTRO_REVISIONI.md` | — |
| CP-002, CP-003, CP-004, DECO1, F60 | **APERTI [~]** | dossier esistenti | Review indipendenti mancanti; nessuna chiusura. |
| MIG1 | **[x] chiuso (Claude 10/11)** | archiviato in `UFFICIO/archivio_per_claude/RICHIESTA_BLOCCHI_CHIUSI_FINO_18NOV.md` | Non riaprire. Costanti di guida/vocabolario/legacy NON si toccano. |
| V47 V1-V5, VJ1, VG2 | **[x] handoff / [~]** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | V6 review da consolidare. VJ2 [~] ispezione Andrea, **DEFAULT non blocca**. |
| V47 V9 ∞ | **[~]** | `UFFICIO/out/developer/ULTIMO_TURNO.md` 18/11 16:43 | lv_03_foglie 20° giro: `renderer.cpp` ottimizzato; build exit 0. `percorso` exit 0 post (2495f, 41.6s, 0% fuori, `GATE PERCORSO OK`, `v9_lv03_turno26_post`); smoke post exit 0 medio 1.275 max 4.691 ms < 8, replay IDENTICO. **Prossimo:** lv_04_cromo 20°. |
| V47 VJ3 ∞ | **[~] 4/6** | `UFFICIO/out/developer-junior/REVIEW_V47.md` | lv_01-lv_04 50/60 Hz exit 0, replay IDENTICO; lv_03 delta 4.70% segnalato. **Prossimi:** lv_05_spray, lv_06_desktop (ricompila prima). |
| V47 VJ4 ∞ | **[x] 6/6** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 17/11 | Soak 6/6 exit 0, replay IDENTICO, nessun crash (può ripartire da lv_01 se utile). |
| V47 VJ5 ∞ | **[~]** | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` 18/11 16:47, `REVIEW_V47.md` | Review working tree `renderer.cpp` (37+/18-, `git diff --stat` exit 0): rilievi puntatori stabili/reserve fisse e possibili rialloc (`aggiungiIncrocioSprite`, `spriteOrd_`, `terraOrdine_`, `graffiti_`); nessuna prova. Rilievi cumulativi aperti (`mixer.cpp:127-130`, `fasi.cpp:576`, `rng.h:19`, audio 50 Hz, `game.cpp:1809-1814`, salvataggi/classifica/segnale, ecc.). |
| V47 VG3 ∞ | **[~]** | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` 18/11 16:14 | Giro 94: 3 script exit 0, 0 da convertire (145 PNG, 133 confrontati, 12 non motore, 86 raw scenari + 47 altri gruppi, 142 tracciati catalogo, 126 pari, pose 2/2). 0 PNG non catalogati (1 noto bloccato Andrea: `GRAFICA/sprite/giocatore_pose/car-custom-v2.png`). Zero esterni/Sega. |

## Checkpoint
- Nessun checkpoint con **gate+review indipendenti (autore≠revisore) registrati in dossier** da chiudere. Nessuna apertura/chiusura questo turno.
- SCAMBIO_RECENTE CHECKPOINT 1/2/3 [APERTE]: esiti automatici verdi, **non equivalgono a review indipendente in dossier**.
- CP-002/003/004, DECO1, F60, MIG1/V47: restano APERTI/[~] fino a prove e review registrate in dossier.

## Deleghe (ruoli 4.6)

| Ruolo | Consegna (`UFFICIO/out/<ruolo>/`) | Responsabile | Revisore indipendente | Percorsi/criteri | Gate/prova attesa |
|---|---|---|---|---|---|
| @developer | `UFFICIO/out/developer/ULTIMO_TURNO.md` | developer | developer-junior | V9 ∞: **lv_04_cromo 20° giro** — gate `percorso`/`smoke`, exit code, ms/frame medio/max, replay IDENTICO; se >8 ms alleggerire **senza cambiare resa/replay**. Valutare rilievi junior VJ5 su `renderer` solo se compatibili e senza toccare legacy/vocabolario/costanti guida/MIG1/HUD/auto/tracciati/audio/sponsor. No `gate_t1.sh` completo salvo richiesta. | Exit code 0, replay IDENTICO, misure ms/frame. |
| @developer-junior | `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md` | developer-junior | developer | VJ3 **lv_05_spray o lv_06_desktop** 50/60 Hz (ricompila prima; non usare `prova_gate` vecchio), exit code, replay IDENTICO, delta tempi; oppure VJ5 ∞ altro cambio `src/`/consolidare review V6/MIG1/V47 autore≠revisore con file/riga/prove in `REVIEW_*`. VJ4 6/6 completato (può ripartire da lv_01 se utile). Nuove prove solo in `motore_nuovo/test/*.cpp` o tool assegnati; non modificare `src/` prodotto. | Righe in `REVIEW_*` (file/riga); eventuale exit code, replay IDENTICO, ms/frame o delta 50/60. |
| @grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` | grafico | developer-junior (se materiale) / Capo verifica handoff | VG3 ∞: confronta date/contenuto PNG scenari vs `.sglraw`; converti solo se PNG più nuovo con script catalogo, **senza ritoccare il disegno**; se allineato, 1 riga. No sponsor/asset Andrea, no `frasi.json`, no codice/livelli/tracciati. Zero risorse esterne/Sega. | Comandi+exit code, elenco file o "0 da convertire". |

## Vincoli
- Capo scrive solo in `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/bacheche/capo/*`. Percorsi relativi. Nessun file in radice. `REGISTRO_REVISIONI.md` append-only.
- Legacy 08/11, vocabolario, costanti di guida, HUD/auto/partenza/tracciati/audio, sponsor/cartelloni: **intoccabili**. `RICHIESTA.md` e `SCAMBIO.md` sola lettura.
- `FINITO` non creato (passi ∞, blocchi non tutti [x]). `PER_CLAUDE.md` non necessario. Domande ad Andrea: nessuna (DEFAULT prosegue; VJ2 non blocca). Pi 400 non verificato.
