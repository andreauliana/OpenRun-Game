# Voce scambio builder-developer — 19/10/2026 ~11:45 (turno 1)

## Blocco R1 (tracciati modificabili per le 6 piste) — stato [~]
File modificati (scrittura consentita):
- `motore_nuovo/src/road/road.h`: `SegmentDesc` con nuovi campi `entrata`/`uscita` (default -1 = automatico `min(n/3, 60)`).
- `motore_nuovo/src/road/road.cpp`: raccordo entrata/uscita a lunghezza propria, curva costante al centro del tratto, uscita verso la curva del tratto successivo (anello); correzione della direzione distribuita SOLO su segmenti con curva != 0 (mai sui rettilinei).
- `motore_nuovo/src/game/game.cpp`: loader supporto campo `tracciato` di `livello.json` (espansione da `motore_nuovo/dati/tracciati/tratti.json`; se il campo manca o il file manca, si usa `segmenti` — compatibile).
- `motore_nuovo/dati/livelli/lv_0{1..6}_*/livello.json`: campo `tracciato` aggiunto a tutti e 6 i livelli (rettilineo_lungo/corto, curva_dolce/stretta dx/sx, collina_su/giu), carattere tema invariato; lv_02_atomico resta riferimento.
- `motore_nuovo/dati/tracciati/tratti.json` e `regole.json`: NON creabili (scrittura nuovi file in `dati/` negata a questo turno — fornire contenuto su richiesta al Capo/Designer per creazione). Di conseguenza il loader usa oggi `segmenti` e i `tracciato` inseriti attendono la libreria per attivarsi.

## Gate eseguito in questo turno (19/10/2026 ~11:45, Mac)
Comando: `sh tools/gate_t1.sh soak` — exit 0 — `GATE T1: TUTTO OK`.
- 6/6 percorsi: GATE PERCORSO OK — lv_01_neon rettilineo 65% (piu' lungo 1309) curva 0.57/0.53; lv_02_atomico 63% (1548) 0.88/0.62; lv_03_foglie 58% (1991) 0.67/0.83; lv_04_cromo 54% (973) 0.37/0.63; lv_05_spray 73% (966) 1.01/0.99; lv_06_desktop 52% (840) 0.51/0.59. Tutti: giro guidato finito, fuori strada 0%.
- Soak lv_01_neon 30000 frame: ms/frame medio 0.187, max 36.6 (outlier singolo di scheduling, medio ampiamente sotto il limite 20; il gate fallisce solo sul medio). speed@60=161.79 invariato. Replay 2x IDENTICO su tutti i livelli.
- Accelerazione, ribaltamento, sterzo, curve continue, gara a tappe (4 combinazioni, arrivo, replay IDENTICO), screenshot P1: OK.
- Log: `UFFICIO/Officina/Test area/gate_t1.log`.

## Bloccati / prossimi passi (non miei, permessi o altrui attori)
- Creazione `dati/tracciati/tratti.json` e `regole.json` (R1 compiti 2-3 dati) + lettura di `regole.json` da `prova_gate.cpp` (test file, scrittura negata al mio ruolo): il gate e' gia' verde con i soglie attive; quando i file dati esistono, `prova_gate` dovra' leggerli al posto delle soglie hardcodate.
- `docs/FORMATI_DATI.md`: documentazione tracciato/regole (scrittura negata questo turno).
- R1 compiti 4-5 (gate stampa tratto violato + test espansione tracciati; riscrittura 6 livello.json con tracciato): in attesa di @developer-builder-junior; review incrociata prevista (Junior rivede 1-3, Builder rivede 4-5), poi @capo apre CP-005.
- T1-RUN punti 2-3 (colline, wrap sprite): gia' presenti nel codice corrente (verificati dai report 22:56); v91 Andrea runner Mac SDL2 e fps/temperatura Pi 400 in attesa di Andrea (T1-Q3/T1-Q8); ordine test: prima Mac, poi Pi/RetroPie (ANDREA_REVIEW T1-Q5).
- Zero scritture in `motore_nuovo/` e `tools/` oltre ai file sopra elencati; nessun test del Junior modificato.
