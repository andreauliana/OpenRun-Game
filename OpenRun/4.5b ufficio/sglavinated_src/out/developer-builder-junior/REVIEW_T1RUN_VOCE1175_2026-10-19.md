# Peer review indipendente — T1-RUN tuning percorsi (VOCE_SCAMBIO_1175)

**Revisore:** Developer/Builder Junior (ruolo) — l'autore (builder-developer) non approva il proprio lavoro.
**Data:** 2026-10-19
**Oggetto:** review del lavoro di builder-developer su T1-RUN (tuning percorsi), consegna [VOCE_SCAMBIO_1175.md](../builder-developer/VOCE_SCAMBIO_1175.md).
**Esito: POSITIVO** (tutti i 6 criteri soddisfatti; 3 note di perimetro non bloccanti sotto).

## File letti (sola lettura)

- `out/builder-developer/VOCE_SCAMBIO_1175.md` (diff dichiarato, comando, exit code, misure, problemi residui, licenza)
- `bacheche/builder-developer/diario.md` e `bacheche/builder-developer/RIASSUNTO.md`
- `UFFICIO/Officina/STATUS_OFFICINA.md` (esito verde e log `gate_t1.log`)
- `UFFICIO/Officina/Test area/gate_t1.log` (log dell'autore, letto **prima** della mia esecuzione)
- `tools/gate_t1.sh`, `motore_nuovo/test/prova_gate.cpp` (soglie), `ANDREA_REVIEW.md` (T1-Q3/T1-Q5)
- `out/builder-developer/VOCE_SCAMBIO_1174.md` (perimetro del blocco R1, per attribuzione dei diff)
- diff git: `git status --porcelain`, `git diff`, `git diff --stat`, `git log`, `git diff 4dd8f06^ 4dd8f06`, `git diff e3c1f7d^ e3c1f7d`

Nessun file di `out/builder-developer/` modificato; nessun file eliminato; nessun checkpoint toccato.

## Comando di verifica eseguito

`sh tools/gate_t1.sh soak` (dalla radice `sglavinated_src/`, nessun pipe, nessun `2>&1`, nessun `;`) → **exit 0**, ultima riga video `GATE T1: TUTTO OK` (il wrapper stampa quel rigo solo se RC=0, `tools/gate_t1.sh:91`).
Comandi git di controllo: `git status --porcelain`, `git diff`, `git diff --stat`, `git log --format=... -12`, `git diff 4dd8f06^ 4dd8f06 -- motore_nuovo/dati/livelli/`, `git diff e3c1f7d^ e3c1f7d -- motore_nuovo/dati/livelli/` — tutti exit 0.

**Provenienza del log:** la mia esecuzione ha rigenerato `UFFICIO/Officina/Test area/gate_t1.log` (output generato dal wrapper autorizzato). Le misure dell'autore citate sotto provengono dal log originale letto prima della mia esecuzione; quelle della mia esecuzione sono distinte.

## Verifica dei criteri

**1. Perimetro del diff — SOLO i 4 `livello.json`, niente core, niente asset nuovi — SODDISFATTO per T1-RUN.**
- Le modifiche di tuning descritte in 1175 corrispondono esattamente ai soli dati dei 4 file dichiarati: `lv_01_neon`, `lv_03_foglie`, `lv_04_cromo` (commit `4dd8f06` 2026-10-19 11:09, campo `segmenti`/`lunghezza`/`durata_stimata_s`) e `lv_05_spray` (commit `4dd8f06` + `e3c1f7d` + aggancio finale non committato). Verificato campo per campo: lv_01 rettilinei 900–1300 con curve ±0.35–0.6; lv_03 tratto 1100 passato da curva −0.9 a 0.0; lv_04 rettilinei 700–1000; lv_05 schema allineato (rettilinei 400–900, curve 0.3–0.9 bilanciate, collina 0.0).
- Nessuna modifica ai `segmenti` di `lv_02_atomico` e `lv_06_desktop` (il loro diff è solo il campo `tracciato`, vedi nota A).
- Nessun nuovo asset esterno: nessun file non tracciato in `motore_nuovo/` (gli unici `??` sono report out/ e il dossier CP-005), nessun PNG/JSON nuovo, nessun materiale esterno introdotto. `motore_nuovo/test/` e `tools/` non compaiono nel diff (soglie gate e wrapper invariati).
- **Nota A (perimetro, non imputabile a T1-RUN):** il working tree contiene anche `motore_nuovo/src/game/game.cpp`, `src/road/road.cpp`, `src/road/road.h` e il campo `tracciato` in tutti e 6 i `livello.json`. Queste modifiche sono il blocco R1 documentato in [VOCE_SCAMBIO_1174.md](../builder-developer/VOCE_SCAMBIO_1174.md) (righe 5-7), consegna precedente e distinta; 1175 le dichiara esplicitamente non toccate e il contenuto del diff corrisponde a 1174, non a 1175. Vanno recensite con il perimetro R1 (review incrociata prevista dal RIASSUNTO del Builder), non con questa.

**2. Gate soak exit 0 e `GATE T1: TUTTO OK` — SODDISFATTO.**
- Log dell'autore: 6 prove `GATE OK`, 6 `GATE PERCORSO OK`, `GATE ACCELERAZIONE/RIBALTAMENTO/STERZO/CURVE CONTINUE/GARA/SCREENSHOT P1 OK`, zero righe `FALLITO`. La stringa `GATE T1: TUTTO OK` non è nel log per design: il wrapper la stampa solo a video (`tools/gate_t1.sh:91`).
- Riproduzione indipendente mia: `sh tools/gate_t1.sh soak` → tutti i passi `exit 0`, `GATE T1: TUTTO OK`.

**3. Giro guidato OK su tutti e 6 i livelli — SODDISFATTO.**
Valori identici nella mia esecuzione e nel log dell'autore:

| Livello | Rettilineo | Più lungo | Curva max D | Curva max S | Esito |
|---|---|---|---|---|---|
| lv_01_neon | 65% | 1309 | 0.57 | 0.53 | GATE PERCORSO OK |
| lv_02_atomico | 63% | 1548 | 0.88 | 0.62 | GATE PERCORSO OK |
| lv_03_foglie | 58% | 1991 | 0.67 | 0.83 | GATE PERCORSO OK |
| lv_04_cromo | 54% | 973 | 0.37 | 0.63 | GATE PERCORSO OK |
| lv_05_spray | 55% | 1241 | 0.83 | 0.67 | GATE PERCORSO OK |
| lv_06_desktop | 52% | 840 | 0.51 | 0.59 | GATE PERCORSO OK |

Rettilinei tutti in [25%, 75%] (range 52–65%), curva max fra 0.37 e 0.88 (≥0.3 in entrambe le direzioni), giro finito, fuori strada 0 frame (0.0%) su tutti e 6. Riferimento ANDREA_REVIEW T1-Q5 (`lv_02_atomico` 40% / curva max 0.86) verificato nel documento; vedi nota B.

**4. Performance e determinismo — SODDISFATTO.**
- Log autore (letto prima della mia esecuzione): soak 30000 frame, `ms/frame medio=0.184 max=0.480 (limite 20)`.
- Mia esecuzione: soak `ms/frame medio=0.185 max=2.443 (limite 20)` — media identica, massimo diverso per un singolo frame outlier dovuto al carico di sistema della macchina durante la review; comunque ben sotto il limite 20 e senza effetto su hash/risultati (vedi nota C).
- Replay 2x `IDENTICO` ovunque: smoke dei 6 livelli, avvolgimento (60000 frame), inattività, soak, 4 combinazioni gara.
- `speed@60=161.79`, `speed@1=9.90`; accelerazione, ribaltamento/ripartenza, sterzo (±218.3), curve continue (salto max 0.02), gara 4 bivi (arrivo, replay IDENTICO), screenshot P1: tutti OK.

**5. Licenza — SODDISFATTO.**
Tutti i `livello.json` (i 4 ritoccati compresi) dichiarano `"licenza": "CC-BY-4.0"`, autore `@livelli`, "nessun materiale Sega"; file già presenti nel repo, nessun materiale esterno introdotto. Codice esistente SPDX MIT. La modifica è solo dati open: compatibile con la quota open ≥ 50%.

**6. Proprietà del core — SODDISFATE (con precisazione).**
- Core senza SDL: `sglavinated_core` compilato, nessun include SDL nel diff (`git diff -- motore_nuovo/src/` su `SDL|chrono|time(|random_device|system_clock|malloc` → zero corrispondenze).
- Determinismo: nessun orologio di sistema, nessun RNG aggiunto; contatore frame e seed invariati, provato dai replay 2x IDENTICO e dagli hash frame del gate.
- Zero alloc nel ciclo di gioco: le uniche nuove allocazioni (`std::map`/vettore in `leggiTracciato`) avvengono al caricamento livello (`Game::caricaLivello`), non nel loop di update; il loop è invariato.
- **Precisazione:** i file del core sono cambiati, ma per il blocco R1 (nota A), non per T1-RUN; le proprietà sopra risultano preservate. `motore_nuovo/test/prova_gate.cpp` è invariato → le soglie non sono state abbassate (richiesta esplicita di Andrea in ANDREA_REVIEW T1-Q5).

## Note per il Capo (non bloccanti)

- **Nota A** — perimetro misto nel working tree: accanto agli aggiustamenti dati di T1-RUN ci sono le modifiche core del blocco R1 (`road.h`/`road.cpp`/`game.cpp`) e il campo `tracciato` nei 6 livelli, documentate in VOCE_SCAMBIO_1174. Da convogliare nella review R1 dedicata; il criterio 1 è verificato sul perimetro T1-RUN.
- **Nota B** — il riferimento storico `lv_02_atomico` 40% / 0.86 (ANDREA_REVIEW T1-Q3/T1-Q5) oggi si misura 63% / 0.88 pur con i `segmenti` di lv_02 invariati: lo spostamento deriva dal raccordo entrata/uscita introdotto da R1 in `road.cpp` (più segmenti contati come dritti). Soglie gate invariate (25–75%, rettilineoMax ≥ 300, curva ≥ 0.3) e giro guidato con 0 frame fuori strada su tutti i livelli: la guidabilità resta verificata. Utile segnalarlo ad Andrea come cambiamento della metrica, non dei dati di lv_02.
- **Nota C** — `UFFICIO/Officina/Test area/gate_t1.log` è stato rigenerato dalla mia esecuzione di verifica (output generato, nessuna scrittura manuale); le misure dell'autore (0.184 / 0.480) sono registrate in questo report come lette dal log originale.
- Durante la review è comparso il file non tracciato `out/builder-developer/RISULTATI_TURNO.md`: non è stato scritto da me (sessione autore attiva in parallelo).
- Residui già noti e fuori scope T1-RUN: runner SDL Mac (v91), fps/temperatura Pi 400, `dati/tracciati/tratti.json` ancora assente (il campo `tracciato` resta quindi inerte con fallback su `segmenti`), G1 grafico.

## Riferimenti

- Consegna recensita: [out/builder-developer/VOCE_SCAMBIO_1175.md](../builder-developer/VOCE_SCAMBIO_1175.md)
- Handoff tecnico: [UFFICIO/Officina/STATUS_OFFICINA.md](../../UFFICIO/Officina/STATUS_OFFICINA.md)
- Log gate: `UFFICIO/Officina/Test area/gate_t1.log` (rigenerato dalla verifica di questa review)
- Blocco R1 (perimetro separato): [out/builder-developer/VOCE_SCAMBIO_1174.md](../builder-developer/VOCE_SCAMBIO_1174.md)
