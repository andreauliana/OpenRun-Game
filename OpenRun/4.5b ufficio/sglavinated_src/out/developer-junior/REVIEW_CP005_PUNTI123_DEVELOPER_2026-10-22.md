# Review indipendente CP-005 — punti 1-3 (blocco R1 tracciati 6 piste)

**Revisore:** Developer/Builder Junior (@developer-junior) — revisore indipendente, autore distinto.  
**Autore del lavoro in esame:** @developer / builder-developer (punti 1-3 del blocco R1).  
**Data:** 2026-10-22  
**Checkpoint:** CP-005 (`Checkpoint Revision/CP-005-tracciati-percorsi.md`), stato APERTO.  
**Esito: POSITIVA** — nessun rilievo bloccante.  
**Modalità:** sola lettura. Nessuna scrittura, build o test eseguita da questo ruolo in questa task; `git status` working tree pulito (modifiche R1 già committate, v. commit `2488a81` e successivi).

---

## Mandato e criteri di completamento (CP-005 dossier)

Blocco R1 di `RICHIESTA_APERTA.md`: `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` verde su tutte e 6 le piste (giro guidato OK 6/6, riferimento lv_02_atomico rettilineo ~40% / curva max 0.86), `ctest`, replay 2x IDENTICO, ms/frame <= 20. **Soglie di gate NON si abbassano**: si correggono i tracciati (`ANDREA_REVIEW.md` T1-Q5, DEFAULT A). Decisione di Andrea non richiesta (T1-Q5 DEFAULT A già registrato).

Ruoli e perimetro (CP-005 / AGENTS.md):
- builder-developer (autore, punti 1-3): `motore_nuovo/src/road/road.cpp`, `motore_nuovo/dati/tracciati/tratti.json`, `motore_nuovo/dati/tracciati/regole.json`, supporto campo `tracciato` in `motore_nuovo/src/game/game.cpp`.
- developer-builder-junior (punti 4-5, oggetto di review separata già registrata): `motore_nuovo/test/prova_gate.cpp`, 6 `dati/livelli/*/livello.json`.

---

## File e diff esaminati (sola lettura)

| File | Ruolo nel blocco R1 |
|---|---|
| `motore_nuovo/src/road/road.h` | `SegmentDesc` con campi `entrata`/`uscita`; default -1 = automatico `min(lunghezza/3, 60)`; 0 disabilita il lato. |
| `motore_nuovo/src/road/road.cpp` | Raccordo entrata/uscita a lunghezza propria, curva costante al centro del tratto, uscita verso curva del tratto successivo (anello); correzione deriva solo su segmenti con `curva != 0`. |
| `motore_nuovo/src/game/game.cpp` | `leggiTracciato()`: espansione campo `tracciato` di `livello.json` da `dati/tracciati/tratti.json`; fallback su `segmenti` se campo mancante/file illeggibile. |
| `motore_nuovo/dati/tracciati/tratti.json` | 8 tratti (rettilineo_corto/lungo, curva_dolce/stretta dx/sx, collina_su/giu), licenza CC-BY-4.0, tracciati originali (nessun materiale Sega). |
| `motore_nuovo/dati/tracciati/regole.json` | Regole per 6 livelli + `default`, identiche per tutti i livelli, CC-BY-4.0. |
| `motore_nuovo/dati/livelli/lv_0{1..6}_*/livello.json` | 6 file con campo `tracciato` (lista nomi tratto), carattere tema invariato, lv_02_atomico riferimento. |
| `motore_nuovo/test/prova_gate.cpp` | Gate `percorso` (legge `regole.json`, stampa tratto + segmento iniziale di ciascuna violazione) e test `espansione` — esaminato per contesto; è file del Junior (punti 4-5), non oggetto di questa review. |

Diff ancorato a `git show 2488a81` (R1: `road.h` +6 righe, `road.cpp` +79/-…, `game.cpp` +97, 6 `livello.json` con `tracciato`); `git status` e `git diff --stat` working tree pulito (HEAD coincide con i file esaminati). Commit di contesto `4dd8f06`/`e3c1f7d` toccano solo dati di 4 `livello.json` (tuning, non soglie gate).

---

## Verifica requisiti punti 1-3

**Punto 1 — road.h/road.cpp (raccordo entrata/uscita, default min(n/3,60), deriva solo curva != 0): CONFORME.**
- `road.h`: `SegmentDesc` eredita `entrata = -1`, `uscita = -1` con commento esplicito del default `min(lunghezza/3, 60)`.
- `road.cpp`: lambda `raccordoAuto` = `lunghezza/3` capped a 60 e min 0; se `entrata`/`uscita` >= 0 si usano i valori espliciti; se `ee + uu > n` si ridimensionano entrambi (no sovrapposizione).
- Passata 1: rampa entrata `curvaPrima -> curvaDentro`, centro costante `curvaDentro`, rampa uscita `curvaDentro -> curvaFuori` dove `curvaFuori` è la curva del tratto successivo (anello: ultimo torna al primo).
- Passata 2: `residuo` e `correzione` accumulati SOLO su segmenti con `s.curve != 0.0f`; applicazione `if (seg.curve != 0.0f) seg.curve -= correzione;` — i rettilinei restano perfettamente dritti e la direzione torna a zero a fine giro. CONFORME alla specifica "deriva solo su segmenti con curva != 0".
- Osservazione non bloccante (R1-OSS-1): il raccordo di entrata è computato ma di fatto no-op per i tratti 2..n (`curvaPrima` viene precaricato con `curvaFuori` del tratto precedente, cioè già uguale a `curvaDentro`); risulta attivo sul primo tratto (chiusura anello). Su tutti e 6 i livelli attuali non ha effetto perché iniziano e finiscono in rettilineo (`curva 0.0`); confermato da gate 6/6 + replay 2x IDENTICO. Da verificare solo se un futuro livello iniziasse/finisse in curva (continuità al wrap).

**Punto 2 — tratti.json (8 tratti): CONFORME.** Otto tratti nominati con `curva`/`collina`/`lunghezza`/`entrata`/`uscita`, licenza CC-BY-4.0, autore @developer, dichiarati originali (nessun materiale Sega).

**Punto 3 — regole.json (6 livelli + default) + game.cpp loader + 6 livello.json: CONFORME.**
- `regole.json`: voce `default` + 6 voci livello, tutte con valori identici (rettilineo 0.25-0.75, rettilineo più lungo min 300 segmenti, curva max 1.0, curva dx/sx min 0.3, fuori strada max 0.01 = 1%), coerenti con lv_02_atomico come richiesto da ANDREA_REVIEW T1-Q5. Soglie **non abbassate**.
- `game.cpp` `leggiTracciato()`: legge `tratti.json`, costruisce libreria di `SegmentDesc`, espande ogni voce di `tracciato` (stringa nome o oggetto `{"tratto":…}` con override `curva`/`collina`/`lunghezza`/`entrata`/`uscita`); voce sconosciuta saltata; se `out` vuoto il chiamante usa `segmenti` (compatibile). `caricaLivello` prova `tracciato` prima di `segmenti`, poi fallback hardcoded 400 segmenti; ripete il pattern fino a >= 2*DRAW_DIST. CONFORME ("campo tracciato, fallback su segmenti").
- Osservazione non bloccante (R1-OSS-2): nome di tratto sconosciuto viene saltato silenziosamente; se tutte le voci saltano si cade in fallback `segmenti` senza errore esplicito. Il gate `espansione` di `prova_gate.cpp` già rileva tratti sconosciuti (esito FALLITO, rc 11). Da tenere presente per nuovi livelli.
- Tutti e 6 i `livello.json` contengono il campo `tracciato` (9-12 tratti ciascuno), tema invariato.

---

## Confronto con le misure gate registrate (NON rieseguite in questa task)

Esiti già registrati e riscontrati nei log/status (`STATUS_OFFICINA.md`, `CP-005-tracciati-percorsi.md`, esiti in `out/developer/`):

1. **20/10 ~14:53** — `sh UFFICIO/Officina/TOOL/gate_t1.sh`: exit 0, `GATE T1: TUTTO OK`, **6/6 GATE PERCORSO OK**, ms/frame medio ~0.17 max 0.26 (limite 20), replay 2x IDENTICO. Log `UFFICIO/Officina/Test area/gate_t1.log`; esito `out/developer/ESITO_TURNO_20OTT_1453.md`.
2. **20/10 19:23** — `gate_t1.sh`: exit 0, `GATE T1: TUTTO OK`, **6/6 GATE PERCORSO OK** (rettilineo 55-71%, curva max 0.72-0.98, fuori strada 0%), ms/frame medio 0.171 max 0.323 (limite 20), replay 2x IDENTICO. Log `gate_t1.log`; esito `out/developer/ESITO_GATE_20OTT_1923.md`.
3. **21/10 ~09:00** — `gate_t1.sh` (smoke): exit 0, `GATE T1: TUTTO OK`, **6/6 GATE PERCORSO OK** (rettilineo in [25%,75%], curva max <= 1.0, dx/sx >= 0.3), ms/frame medio=0.291 max=16.094 (limite 20), replay 2x IDENTICO, speed@60=161.79, speed@1=9.90. Log `gate_t1.log`; esito `out/developer/ESITO_GATE_21OTT_0900.md`.
4. **19/10 ~11:45** — `gate_t1.sh soak`: exit 0, `GATE T1: TUTTO OK`, 6/6 percorsi GATE PERCORSO OK (rettilineo 52-73%, curva 0.37-1.01), soak ms/frame medio 0.187 (outlier singolo max 36.6, medio ampiamente sotto 20), replay 2x IDENTICO, speed@60=161.79. Log `gate_t1.log`; `out/developer/VOCE_SCAMBIO_1174.md`.

**Riferimento storico (rosso, superato):** Andrea 19/10 09:39 `gate_t1.sh soak` exit 13, giro guidato FALLITO 5/6 (solo lv_02_atomico 40%/0.86 passava); azione di tuning assegnata a @developer (ANDREA_REVIEW T1-Q3). Dopo R1 tutti e 6 i livelli passano il giro guidato con 0 frame fuori strada.

**Conferma soglie non abbassate:** `MAX_MS_FRAME = 20.0` invariato in `prova_gate.cpp`; `regole.json` identico `default` per tutti i 6 livelli; `motore_nuovo/test/` e `UFFICIO/Officina/TOOL/` invariati (zero soglie modificate; `git diff` su `SDL|chrono|time(|random_device|system_clock` nel core = zero corrispondenze per quanto attiene questa review).

---

## Rilievi e correzioni richieste

| ID | Rilievo | Bloccante? | Correzione richiesta / nota |
|---|---|---|---|
| R1-OSS-1 | Raccordo entrata di fatto no-op per tratti 2..n (nessun impatto sui 6 livelli attuali). | No | Documentare; rieseguire solo se un livello futuro iniziasse/finisse in curva. |
| R1-OSS-2 | `leggiTracciato` salta silenziosamente i nomi di tratto sconosciuti (fallback `segmenti`). | No | Coperto dal gate `espansione` (rc 11); da tenere presente per nuovi livelli. |
| R1-OSS-3 | Dossier CP-005 cita `ctest 17/17`, ma `STATUS_OFFICINA.md` segnala 7 test scoperti nel build esistente, da rieseguire e registrare prima della chiusura CP-005. | No (per chiusura formale) | Riconciliare col @capo prima di dichiarare CP-005 chiuso; non riguarda i punti 1-3 esaminati qui. |

Nessun rilievo bloccante. Il lavoro del @developer sui punti 1-3 è coerente con i requisiti CP-005 e con le misure gate registrate.

---

## Cross-reference: review sviluppatore -> Junior (punti 4-5)

Già registrata come **POSITIVA con rilievo non bloccante** (`out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md`, 2026-10-20): `verificaPercorso` stampa metriche aggregate e VIOLAZIONI con suffisso `[tratto X da segmento Y]` per rettilineo più lungo e curve dx/sx, ma non stampa nome tratto/segmento iniziale per la violazione %% rettilineo, "giro non completato" e "fuori strada" (compito 4 da completare). Tale rilievo riguarda il file del Junior (`prova_gate.cpp`, punti 4-5) e non influenza l'esito di questa review sui punti 1-3 del @developer.

---

## Conclusione

Esito review punti 1-3: **POSITIVA**. Nessun rilievo bloccante. Le soglie di gate non sono state abbassate; le misure registrate (6/6 percorsi OK, ms/frame medio <= 20, replay 2x IDENTICO) sono confermate dai log consultati. CP-005 può procedere alla chiusura formale da parte del @capo dopo: (a) registrazione di entrambe le review incrociate, (b) riesecuzione/registrazione `ctest` e riconciliazione del conteggio test (7 vs 17 citato nel dossier), (c) eventuale presa d'atto di R1-OSS-1/R1-OSS-2. Nessun lavoro Pi 400/RetroPie prima dell'ok Mac di Andrea (D12 DEFAULT A, ordine test T1-Q5).

**Log consultati:** `UFFICIO/Officina/Test area/gate_t1.log` (esiti 20/10 14:53, 19:23, 21/10 ~09:00, 19/10 ~11:45), `UFFICIO/Officina/STATUS_OFFICINA.md` (report 20/10 14:53, 17:01, 19:23, 21/10 ~09:00), `ANDREA_REVIEW.md` (T1-Q3, T1-Q5, D12, D13, D14, D18), `Checkpoint Revision/STATO_CORRENTE.md`, `Checkpoint Revision/CP-005-tracciati-percorsi.md`, `out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md`, `out/developer-junior/REVIEW_T1RUN_VOCE1175_2026-10-19.md`, `out/developer/VOCE_SCAMBIO_1174.md`, `out/developer/ESITO_GATE_21OTT_0900.md`.

Firmato: Developer/Builder Junior (revisore indipendente), 2026-10-22.
