# Esito blocco RIV punto a (bump) — 27/10/2026 19:41

## Blocco
RIV (Rivali: bump, sprite, comportamento realistico) — punto a: urto con avversario.

## Cosa e' stato fatto
- Urto con un avversario (sprites_) = bump: spinta laterale leggera che allontana dall'auto urtata + rallentamento ~ -15% (fattore 0.85), nessun ribaltamento, controlli sempre attivi (auto non bloccata), durata 1 s.
- Anche il rivale urtato perde un po' di velocita' (passo * 0.90).
- Contatto con cartellone bordo strada = nessun ribaltamento (ostacolo decorativo).
- Ribaltamento solo per fuori strada prolungato (invariato).
- Costanti lette da dati/config.json: bump_rallenta 0.85, bump_durata_s 1.0, bump_spinta_laterale 200.0.

## File modificati
- motore_nuovo/dati/config.json (aggiunte chiavi bump_*)
- motore_nuovo/src/game/game.h (membri bump + metodi collisioneRivaleVicino/collisioneCartelloneVicino)
- motore_nuovo/src/game/game.cpp (init legge config, init inizializza stato bump, aggiornaGuida logica bump, implementazioni collisione)

## Build e test
- cmake --build UFFICIO/Officina/build --target prova_gate : exit 0
- sh UFFICIO/Officina/TOOL/run_tests.sh : ctest 22/22 verde (713.46 s), inclusi gate_crash (fuoristrada/ribaltamento invariato), gate_gara (gara con rivali, determinismo OK), gate_ciclo (ciclo fasi R2), 6 livelli smoke+percorso OK.
- gate_t1.sh non eseguito in questo turno (ultimo gate 27/10/2026 19:11 exit 0, 6/6 percorsi GATE PERCORSO OK, ms/frame medio <=0.87 max 1.16, replay 2x IDENTICO).

## Prossimi passi (RIV)
- Punto b: rivali usano sprite del catalogo invece dei rettangoli (richiede asset @grafico già presenti).
- Punto c: comportamento realistico in aggiornaRivali (rallentamento in curva, bravura dal seme, cambio corsia morbido, distanza minima tra rivali).
- Poi PIST1 (tipi tratto: chicane, ponte, autostrada, citta').

## Bloccati / in attesa
- T1-RUN [~] e H [~]: ok runner Mac v91 mancante, poi fps/temperatura Pi 400 (D19=B).
- Nessun lavoro dedicato Pi 400/RetroPie prima di ok Mac.
- FORK1 [~]: attende CP-007 di @capo.
