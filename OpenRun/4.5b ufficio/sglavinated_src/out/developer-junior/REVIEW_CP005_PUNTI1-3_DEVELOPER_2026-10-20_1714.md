# CP-005 — Review incrociata: @developer-junior rivede @developer (punti 1-3 di R1)

- Checkpoint: `Checkpoint Revision/CP-005-tracciati-percorsi.md` (APERTO)
- Blocco: R1 — Tracciati modificabili per le 6 piste (`RICHIESTA_APERTA.md`)
- Revisore: @developer-junior | Autore rivisto: @developer (builder-developer) | Data: 2026-10-20 17:14
- Regole: `AGENTS.md` (review peer, autore diverso dal revisore; l'autore non approva il proprio lavoro)

## File esaminati
- `motore_nuovo/src/road/road.cpp` (passata 1 raccordo curve, passata 2 quote/correzione deriva)
- `motore_nuovo/src/road/road.h` (`SegmentDesc` con `entrata`/`uscita`, default `min(n/3,60)`)
- `motore_nuovo/src/game/game.cpp` (`leggiTracciato`, loader campo `tracciato` con fallback su `segmenti`)
- `motore_nuovo/dati/tracciati/tratti.json` (8 tratti, CC-BY-4.0)
- `motore_nuovo/dati/tracciati/regole.json` (6 livelli + default, CC-BY-4.0, lette da `prova_gate::leggiRegolePercorso`)
- `motore_nuovo/dati/livelli/lv_01_neon/livello.json` (campo `tracciato` verificato; lo sviluppatore riporta tutti e 6 i livelli, `lv_02_atomico` riferimento)

## Esito: POSITIVA (con rilievi non bloccanti)

### Punto 1 — `road.cpp` raccordo entrata/uscita: CONFORME
Raccordo in entrata/uscita a lunghezza propria (default `min(lunghezza/3, 60)`, mai sovrapposti, ridefiniti se `ee+uu > n`), curva costante al centro del tratto, uscita che raccorda alla curva del tratto successivo (anello). Correzione della direzione residua distribuita solo su segmenti con curva != 0, mai sui rettilinei (`road.cpp` righe ~52-110). Coerente con `ANDREA_REVIEW.md` T1-Q5 (soglie invariate, `lv_02_atomico` riferimento).

### Punto 2 — `tratti.json`: CONFORME
8 tratti nominati (`rettilineo_corto/lungo`, `curva_dolce_dx/sx`, `curva_stretta_dx/sx`, `collina_su/giu`) con curva/collina/lunghezza/entrata/uscita, licenza CC-BY-4.0, contenuto originale (nessun materiale Sega). Rilievo non bloccante: la nota di testata cita "esse, tornante" non presenti negli 8 tratti — allineare nota o libreria se necessari.

### Punto 3 — `regole.json` e lettura da `prova_gate`: CONFORME
`regole.json` con voce per `lv_01`..`lv_06` + default (rettilineo 25-75%, rettilineo_piu_lungo_min 300, curva_max 1.0, curva_dx/sx_min 0.3, fuori_strada_max 0.01), coerente col profilo guidabile `lv_02_atomico` (`ANDREA_REVIEW.md` T1-Q5 DEFAULT A, soglie non abbassate). `prova_gate` legge `regole.json` (`leggiRegolePercorso`, fallback su default se il livello manca o il file e' illeggibile; righe 521-556). Soglie non hardcodate.

### Loader e `livello.json`: CONFORME
`game.cpp` espande il campo `tracciato` da `tratti.json` con fallback su `segmenti` se mancante (compatibile). `lv_01_neon` ha campo `tracciato`; lo sviluppatore riporta il campo presente su tutti e 6 i livelli.

## Rilievi non bloccanti (già noti, non impediscono il gate verde)
- Compito 4 (Junior): `verificaPercorso` stampa le violazioni aggregate (righe 603-640) ma non indica il TRATTO e il segmento iniziale violato (richiesto: "nome tratto + segmento iniziale"). Le `RoadSegment` non portano il nome di tratto; serve una mappatura in `prova_gate` o un campo tratto in `RoadSegment`. Non blocca il gate (6/6 `GATE PERCORSO OK`).
- Test espansione (compito 5): `verificaEspansioneTracciato` controlla `n >= somma lunghezze` e `n` multiplo esatto (pattern ripetuto per intero); interpreta la richiesta "numero segmenti = somma lunghezze" come pattern completo ripetuto.

## Gate di riferimento (non rieseguito in questo turno: shell negata in ambiente, 'gate non eseguito')
Ultimo esito noto: 20/10 14:53, `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, 6/6 `GATE PERCORSO OK`, ms/frame medio ~0.17 max 0.26 (limite 20), replay 2x IDENTICO, `GATE T1: TUTTO OK` (log `UFFICIO/Officina/Test area/gate_t1.log`; esito `out/developer/ESITO_TURNO_20OTT_1453.md`).

## Conseguenza per CP-005
- Review sviluppatore su Junior punti 4-5: già registrata (`out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md`, positiva con rilievo non bloccante su compito 4).
- Mia review punti 1-3: registrata con questo documento (POSITIVA, rilievi non bloccanti).
- Mancante: chiusura formale CP-005 da parte del @capo (gate verde + entrambe le review registrate), poi avvio R2. Nessuna decisione di Andrea necessaria (T1-Q5 DEFAULT A registrato; CP-005 "Decisioni richieste ad Andrea: Nessuna").
- Blocco esterno: R2 e H (Pi 400) restano in attesa del @capo (CP-005) e di Andrea (ri-verifica runner Mac, D12 = A; ordine: prima Mac R1+R2, poi Pi 400).

## Scritture di questo turno
Solo `out/developer-junior/` (questo report), `bacheche/developer-junior/diario.md` (1 riga), `bacheche/developer-junior/RIASSUNTO.md` (<=12 righe). Nessuna modifica a `motore_nuovo/`, `tools/`, `Checkpoint Revision/`, `RICHIESTA_APERTA.md`, `SCAMBIO.md`.
