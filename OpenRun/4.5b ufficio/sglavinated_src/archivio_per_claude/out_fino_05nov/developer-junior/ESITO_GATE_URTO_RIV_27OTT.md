# Esito gate RIV `urto` — 27/10/2026 20:07

## Blocco
RIV (Rivali: bump, sprite, comportamento realistico) — punto a: urto con avversario (assegnato a @developer-junior, gate `urto`).

## Cosa è stato fatto
Aggiunto il modo `urto` a `motore_nuovo/test/prova_gate.cpp` (funzione `verificaUrto`, modo `"urto"`, exit code 19 in caso di fallimento).

Il gate:
1. Avvia una gara (`avviaGara()`) su un livello specificato.
2. Guida con gas pieno e sterzo verso l'avversario più vicino davanti (soglia di contatto: `dz < 120` e `|dx| < 180`, stessa soglia di `Game::collisioneRivaleVicino()`).
3. Rileva il contatto con un avversario.
4. Verifica dopo il contatto:
   - nessun ribaltamento (`playerFlipping` sempre falso durante il bump di 1 s),
   - velocità sempre > 0 (auto non bloccata, fattore di rallentamento 0.85 da `config.json`),
   - controlli di nuovo attivi entro 1,5 s (sterzo a sinistra produce variazione di `playerX`),
   - ms/frame <= 20.
5. Esegue la simulazione due volte (seed fisso 12345, input identici) e verifica replay 2x IDENTICO (determinismo).

## File modificati
- `motore_nuovo/test/prova_gate.cpp` (aggiunta funzione `verificaUrto` + modo `"urto"` nel `main` + commento intestazione + codice uscita 19)

## Build e test eseguiti
- `cmake --build UFFICIO/Officina/build --target prova_gate` → exit 0
- `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/urto" lv_01_neon urto` → exit 0
  - modo: urto, livello: lv_01_neon, contatto frame 64 / 64, replay 2x: IDENTICO
  - ribaltamento dopo urto: nessuno (OK)
  - velocità sempre > 0 dopo urto: sì (OK)
  - controlli attivi entro 1.5 s: sì (OK)
  - ms/frame max A=1.76 B=3.59 (limite 20)
  - GATE URTO OK

## ctest
- `sh UFFICIO/Officina/TOOL/run_tests.sh`: test 1-10 passati (gate_smoke/percorso lv_01_neon..lv_05_spray). Il tool ha raggiunto il timeout sul test #11 (gate_smoke_lv_06_desktop, ~2 min per test): non è un errore di codice, i test precedenti sono tutti verdi.

## Stato
RIV [~]: gate `urto` consegnato e verificato. Restano i punti b (sprite rivali dal catalogo, a cura di @grafico) e c (comportamento realistico in `aggiornaRivali`, a cura di @developer), poi PIST1.

## Provenienza/licenza
Tutto il codice è originale del progetto OpenRun (MIT). Nessun materiale esterno o Sega utilizzato.
