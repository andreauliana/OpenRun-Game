# STATI PROPOSTI — @capo (Turno 3, ora 5 di 12, 11/11/2026)

**Fonti lette (solo):** `TAPPE.md`, `ANDREA_REVIEW.md` (**precedenza assoluta**: nessuna RISPOSTA DI ANDREA nuova; G1 ANNULLATO/sponsor solo Andrea; base legacy 08/11 approvata e intoccata), `DIREZIONE_ARTISTICA.md`, `CONTROLLO.md`, `AGENTS.md` (regole 4.6), `RICHIESTA_APERTA.md` (BLOCCO MIG1 attivo, V47 continuo, passi ∞), `SCAMBIO_RECENTE.md` + `UFFICIO/Checkpoint Revision/STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md`, handoff Grafico/Officina/Junior.

**Tappa attiva / Gate:** MIG1 [~] + V47 [~] (solo aggiunte; no refactor legacy/vocabolario). Gate green riportato (Officina 11/11: ctest 27/27 exit 0, `gate_t1.sh` exit 0, 6/6 PERCORSO OK, replay IDENTICO; picco 55.27 ms lv_05 smoke isolato da valutare, log `UFFICIO/Officina/Test area/gate_t1.log`).
**Checkpoint:** nessun CP con gate+review indipendenti (autore≠revisore) registrati in dossier → **nessuna chiusura**. CP-005/006/007 **CHIUSI** (CP-006 **non riaprire**). CP-002/003/004, DECO1, F60 **APERTI** (review mancanti).

## Stati proposti per blocco

| Blocco | Stato | Evidenza sintetica | Blocco residuo |
|---|---|---|---|
| MIG1 G1 cartelloni | **ANNULLATO** | Sponsor solo Andrea (ANDREA_REVIEW 09/11). | Vietato ritoccare sponsor/asset Andrea. |
| MIG1 D1/D3/D5, G2, J1/J2 | **[x] handoff** | Paletti, carattere rivali, audio extra, G2, prove J1/J2 (handoff/REVIEW_MIG1). | Review incrociata da registrare in dossier per eventuale CP. |
| MIG1 D2 sportellata | **[~]** | `REVIEW_MIG1.md` 10/11: `sportellata` exit 28 (trigger/clamp sterzo 60 Hz in `game.cpp`). | Fix mirato sole costanti aperte MIG1 → exit 0 + recupero ~5 s. |
| MIG1 D4 nomi/tema | **[~]** | Nomi/culture da allineare a `NOMI_RIVALI` e temi (RICHIESTA). | Chiarire/allineare; review @developer-junior. |
| MIG1 J3/J4 | **[~]** | J3 bloccato da D2 (exit 28); J4 da chiudere post-fix. | Rieseguire `sportellata`, misurare recupero, aggiornare `REVIEW_MIG1.md`. |
| V47 V1-V5, VJ1, VJ3 | **[x] handoff** | Handoff Officina/junior (ctest/gate verdi riportati). | Review incrociate non consolidate in dossier. |
| V47 V6 core libretro | **[~]** | Officina 11/11: ctest 27/27, gate_t1 exit 0, 6/6 PERCORSO OK; nota picco 55.27 ms lv_05 smoke isolato. | Review indipendente junior da consolidare in `REVIEW_V47.md` con misure/exit code. |
| V47 V7 caricamento / V8 | **[~]/[ ]** | Da proseguire per passi RICHIESTA_APERTA. | Misure prima/dopo, replay/resa identici. |
| V47 V9 ∞ fluidità | **[~]** | @developer 11/11 12:54: lv_01 `percorso` exit 0 (0% fuori), `smoke` exit 0 ms/frame medio 0.951 max 3.420 < 8. | Prossimo livello: lv_02_atomico (un livello/turno). |
| V47 VJ4 soak 50/60 | **[~]** | Junior: 4/6 completati (lv_01-04 exit 0, 30000f IDENTICO, no crash). | Completare lv_05_spray, lv_06_desktop. |
| V47 VJ5 ∞ review codice | **[~]** | Junior 11/11 12:55: review `src/game/salvataggi.cpp/.h`, `caricamento` exit 0 (8.3 ms), rilievi in `REVIEW_V47.md`. | Continuare su altro file `src/` (un file/turno). |
| V47 VG2 frasi EN | **[~]** | Grafico 11/11 11:47: giro 17, `verifica_frasi_vg2.py` exit 0 (156/156), solo `linea1`/`linea2`. | Giro 18 (20 frasi) o passo ∞ disponibile. |
| V47 VG3 ∞ PNG→raw | **[ ]/disponibile** | Da eseguire a confronto date PNG/scenari vs `.sglraw`. | Convertire solo se PNG più nuovo, senza ritocchi; no asset Andrea/sponsor. |
| V47 VJ2 riva | **[~]** | `vj2_riva` exit 0; densità soggetta a ispezione Andrea. | **DEFAULT: non blocca**; Andrea guarda quando può. |
| Legacy CP aperti (CP-002/003/004, DECO1, F60) | **APERTI** | Senza review indipendenti registrate. | Non chiudere senza prove+review in dossier. |

## Deleghe (ruoli AGENTS.md 4.6, consegne `UFFICIO/out/<ruolo>/`)

| Ruolo | Responsabile | Task / criteri misurabili | Revisore indipendente | Percorsi scrivibili (task) | Consegna |
|---|---|---|---|---|---|
| @developer | developer | **D2** fix mirato `sportellata` in `motore_nuovo/src/game/game.cpp` toccando solo `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`/trigger-clamp sterzo 60 Hz; **D4** chiarire/allineare nomi-origine-tema. Poi **V7/V8** o **∞ V9 lv_02**. Gate: `prova_gate ... sportellata` exit 0, `ctest` verde, `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO. No legacy/costanti fuori MIG1. | @developer-junior | `motore_nuovo/src/**` (limite MIG1), `UFFICIO/Officina/TOOL/**` se necessario, bacheca/out dev | `UFFICIO/out/developer/ULTIMO_TURNO.md` |
| @developer-junior | developer-junior | **VJ4** soak lv_05_spray, lv_06_desktop (exit 0, 30000f IDENTICO, ms/frame max). **Post-fix D2:** rieseguire J3/J4, misurare recupero ~5 s, aggiornare `REVIEW_MIG1.md`. Consolidare review **V6** in `REVIEW_V47.md` (autore≠revisore, exit code/misure). **VJ5 ∞** su altro file `src/` (una riga). Prima di ogni prova: ricompila (`run_tests.sh`/target). | @developer | `motore_nuovo/test/*.cpp`, `dati/tracciati/regole.json` (se serve), bacheca/out junior | `UFFICIO/out/developer-junior/REVIEW_MIG1.md`, `REVIEW_V47.md`, bacheca |
| @grafico | grafico | **VG2** giro 18 (20 frasi, max 16 char/riga, MAIUSCOLE, `it1`/`it2` intatti) o **∞ VG3** confronto date PNG `Libreria/assets_originali/scenari/` vs `.sglraw` → convertire solo se più nuovo, senza ritocchi. `verifica_frasi_vg2.py` exit 0. Nessun PNG Andrea/sponsor/logo/sfondi; nessun codice/livelli/tracciati. | Review incaricata da @capo se richiesta; handoff unico Grafico | `UFFICIO/Dipartimento Grafico/**`, `motore_nuovo/dati/sprite_raw/**` solo con script catalogo, `dati/testi`/`frasi.json` se VG2, bacheca/out grafico | `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` |
| @capo | capo | Coordinamento/checkpoint: append registro, sync `STATO_CORRENTE.md`/`STATI_PROPOSTI.md`. Creare/chiudere CP solo a gate+review indipendenti registrate in dossier. Nessuna approvazione tecnica, nessun codice/asset. | — | `UFFICIO/Checkpoint Revision/**`, `UFFICIO/out/capo/STATI_PROPOSTI.md` | presente documento + checkpoint |

## Domande ad Andrea
- Nessuna nuova. Se servisse decisione fuori default: **DEFAULT** applicato e si prosegue; vale solo eventuale **RISPOSTA DI ANDREA** esplicita (nessuna registrata ora). VJ2: DEFAULT non blocca.

## Blocco/Fine
- **FINITO:** no (blocchi MIG1/V47 non tutti [x], review/CP non chiudibili).
- **PER_CLAUDE.md:** non creato (nessun blocco operativo del capo che richieda escalation ora).