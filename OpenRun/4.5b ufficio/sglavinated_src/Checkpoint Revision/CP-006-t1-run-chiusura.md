TURNO 1 ORA 2 DI 12 — SINTESI ESECUTIVA

TASK ATTIVO: TAPPA 1 (TAPPE.md) — "Il gioco parte, si visualizza il payer che si muove, si pilota"
GATE: strada che scorre, auto che accelera (A), frena (B), sterza (sinistra/destra)

RESPONSABILE: @capo (aggregazione esiti, checkpoint, documenti Andrea)
REVISORE INDIPENDENTE: review tecniche già registrate in CP-005 (developer→developer-junior e viceversa)

ESITO VERIFICHE:
- Gate `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` → exit 0, 6/6 piste OK, replay 2x IDENTICO, ms/frame <= 20 ✅
- CP-005 review incrociata indipendente positiva ✅
- T1-RUN: gate verde tecnico, ma bloccato da ANDREA_REVIEW.md D19 = B
- G1: chiuso [x] (check STATUS_GRAFICO_PER_CAPO.md, 181 elementi, CC0-1.0)
- FORK1: chiuso [x] (CP-007)
- DECO1: parzialmente fatto [~] (passi 2a-2d fatti, 2f fatti @grafico, manca review incrociata + CP DECO1)
- R2: in corso [~] (passi 1/1b/1c fatti, ctest 22/22 verde gate `ciclo`)
- PIST1, RIV, FIN: [ ] (da iniziare)

BLOCHI NON [x] (FINITO NON CREABILE):
- T1-RUN [~] — D19 = B richiede nuova prova/decisione Andrea su runner Mac (D12/D14 = A: prima Mac, poi Pi 400/RetroPie)
- H [~] — v91 runner Mac in attesa
- DECO1 [~] — review incrociata + CP DECO1 da assegnare
- R2 [~] — prossimo @developer passo 1d (nome a 3 lettere), poi RIV punti a-c, poi PIST1 punto 1
- RIV [~] — punti a,b,c (bump, sprite rivali, comportamento realistico); punto 1 dopo PIST1
- PIST1 [ ] — gate `tratti` da implementare
- FIN [ ] — dopo R2 e RIV

DECISIONI ANCORA NECESSARIE:
1. Andrea D19: nuova risposta richiesta per chiudere T1-RUN e CP-006 (DEFAULT non applicabile poiché risposta Andrea presente)
2. Dopo D19 positiva: CP-006 CHIUSO, avvio R2 subordinato a conferma formale Andrea (D18 = B)
3. DECO1: review incrociata developer↔developer-junior, @grafico rivede screenshot Test area/, @capo apre CP-008
4. R2: @developer passo 1d, poi RIV punti a-c, poi PIST1 punto 1

PERCORSI SCRITTURA:
- `out/capo/STATI_PROPOSTI.md` — riscritto con stati 4.6 e consegne out/<ruolo>/
- `Checkpoint Revision/REGISTRO_REVISIONI.md` — nuova riga 2026-10-31 registrata
- `Checkpoint Revision/CP-006-t1-run-chiusura.md` — rimane APERTO

## Evento 2026-11-07 (Turno 2, ora 3 di 12) — review negativa gate T1

- Fonte: SCAMBIO_RECENTE.md, CHECKPOINT 1 | REVIEWER | 2026-11-07 09:41 | [APERTA] (revisore @developer-junior).
- Comando: `sh UFFICIO/Officina/TOOL/gate_t1.sh` → exit 12.
- Esito: GATE T1 FALLITO. screenshot P1 incompleti (personaggio+fumetto frame -1); GARA e FREQUENZA 60/50 Hz OK, replay 2x IDENTICO.
- Rilievo: personaggio+fumetto non catturato (frame -1) → screenshot P1 incompleto.
- Correzione assegnata a: @developer (autore di gate_t1.sh).
- Nuova review indipendente richiesta dopo correzione: revisore @developer-junior (diverso dall'autore).
- CP-006 resta APERTO. T1-RUN [~] (D19 RISPOSTA DI ANDREA: B; DEFAULT non applicabile). Nuova prova/decisione Andrea su runner Mac richiesta prima della chiusura (D12/D14 = A: prima Mac, poi Pi 400/RetroPie).
- Nessun cambiamento agli stati [x]/[~]. Nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac formale.

STATO CORRENTE: APERTO (CP-006). Rilievo gate T1 registrato in CP-006 e REGISTRO_REVISIONI; correzione assegnata a @developer; nuova review indipendente richiesta dopo correzione. Nessun blocco richiede PER_CLAUDE.md. Si prosegue autonomamente.

## Evento 2026-11-08 (Turno 1, ora 0 di 12) — CHIUSURA CP-006

- Fonte decisionale: `ANDREA_REVIEW.md` D19 aggiornato 08/11: **CHIUSA** — "Andrea ha chiesto a Claude di sbloccare gli agenti (08/11): il gioco gira sul Mac di Andrea (prove del 06 e 07/11) e il gate T1 e' TUTTO OK (08/11). T1-RUN [x]. Il @capo chiude CP-006. Non riaprirla."
- Prova gate: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0 (08/11), `GATE T1: TUTTO OK`, 6/6 piste `GATE PERCORSO OK`, accelerazione/ribaltamento/sterzo/curve continue/gara OK, frequenza 60 Hz e 50 Hz OK (replay IDENTICO diff=0.00%), ms/frame medio 1.079–1.758 max 17.003 (limite 20), screenshot P1 `personaggio+fumetto frame 446` > 0 (era -1). Log: `UFFICIO/Officina/Test area/gate_t1.log`.
- Review indipendenti: developer-junior → developer su `gate_t1.sh` POSITIVA (`out/developer-junior/REVIEW_GATE_T1_P1_2026-11-08.md`); developer → developer-junior su `prova_gate.cpp:969` POSITIVA. Autore ≠ revisore su entrambe le correzioni.
- Esito: T1-RUN → **[x]**; CP-006 → **CHIUSO** su mandato di Andrea (D19 chiusa, regola 8: decisione registrata in `ANDREA_REVIEW.md`). Non riaprire.
- Condizioni rimaste valide: nessun lavoro dedicato a Pi 400/RetroPie prima della prova di Andrea (ordine test T1-Q5/D12/D14 = A, prima Mac poi Pi 400); H [~] resta di Andrea. R2/RIV/PIST1/FIN non subordinati a CP-006 (Andrea 27/10).
- Tappa attiva dopo chiusura: **BLOCCO PERC1** (TAPPE.md 08/11 "rifinitura del percorso"). @developer D1 · @developer-junior J1 · @grafico G1 · @capo riga di registro.