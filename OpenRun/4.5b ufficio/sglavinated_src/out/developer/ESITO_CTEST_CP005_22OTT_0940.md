# Esito riesecuzione CTest — chiusura CP-005 (R1 tracciati 6 piste)

- Data esecuzione: 22 ottobre 2026, ore 09:37–09:41 (ora macchina)
- Ruolo: @developer (autore della run; la verifica indipendente spetta a @developer-junior e @capo)
- Perimetro: riesecuzione completa CTest su tutti e 6 i livelli (smoke + percorsi) + 5 modi extra
- Sorgenti modificati: NESSUNI (sola esecuzione)

## Comando eseguito

```
sh UFFICIO/Officina/TOOL/run_tests.sh
```

Eseguito una sola volta, in riga singola, dalla radice `sglavinated_src/`, senza pipe, senza `2>&1`, senza `;`, senza `echo`.

## Exit code

**0** (run completata; nessuna riga di errore dal wrapper e chiusura con `100% tests passed`).

## Esito CTest

**17/17 test passati — 100% tests passed out of 17 — Total Test time (real) = 210.63 sec.**

Dettaglio (dall'output CTest):

| # | Test | Esito | Tempo (sec) |
|---|------|-------|-------------|
| 1 | gate_smoke_lv_01_neon | Passed | 34.57 |
| 2 | gate_percorso_lv_01_neon | Passed | 0.03 |
| 3 | gate_smoke_lv_02_atomico | Passed | 33.31 |
| 4 | gate_percorso_lv_02_atomico | Passed | 0.04 |
| 5 | gate_smoke_lv_03_foglie | Passed | 33.46 |
| 6 | gate_percorso_lv_03_foglie | Passed | 0.04 |
| 7 | gate_smoke_lv_04_cromo | Passed | 34.59 |
| 8 | gate_percorso_lv_04_cromo | Passed | 0.04 |
| 9 | gate_smoke_lv_05_spray | Passed | 39.78 |
| 10 | gate_percorso_lv_05_spray | Passed | 0.04 |
| 11 | gate_smoke_lv_06_desktop | Passed | 33.61 |
| 12 | gate_percorso_lv_06_desktop | Passed | 0.04 |
| 13 | gate_accelerazione | Passed | 0.09 |
| 14 | gate_crash | Passed | 0.06 |
| 15 | gate_sterzo | Passed | 0.00 |
| 16 | gate_curve | Passed | 0.00 |
| 17 | gate_gara | Passed | 0.90 |

### Misure

- Test scoperti ed eseguiti: **17** (6 livelli × smoke + 6 livelli × percorso + 5 modi)
- Test falliti: **0**
- Tempo totale reale CTest: **210.63 sec**
- Build: `sglavinated_core` (87%) e `prova_gate` (100%) già aggiornati — nessuna ricompilazione necessaria
- Nota di config: `SDL2_image non trovata` → runner compilato senza decodifica PNG (condizione già nota e documentata; il core CTest è core-only e non dipende da SDL)

## Log

**Log CTest ufficiale della run** (scritto da CTest, non da gate_t1.sh):

- `UFFICIO/Officina/build/Testing/Temporary/LastTest.log` (21.624 byte, 22 ott 2026 09:40:11)

**Output dei singoli test** (scritti da `prova_gate` durante questa run CTest):

- `UFFICIO/Officina/build/test-results/lv_01_neon/` … `lv_06_desktop/` (log + screenshot PPM, cartelle aggiornate 22 ott 09:37–09:40)
- `UFFICIO/Officina/build/test-results/percorso_lv_01_neon/` … `percorso_lv_06_desktop/`
- `UFFICIO/Officina/build/test-results/{accelerazione,crash,sterzo,curve,gara}/`

**Chiarimento percorsi** (attenzione a non confondere): `UFFICIO/Officina/Test area/` contiene i log storici prodotti da `gate_t1.sh` (ultima run: 21 ott 14:37–14:41, file `gate_t1.log` e `lv_*/prova_gate.log`). La presente run CTest, invece, per come è definito `GATE_TEST_OUTPUT` in `motore_nuovo/CMakeLists.txt` (righe 131–155), scrive i propri artefatti sotto `UFFICIO/Officina/build/test-results/` e il log aggregato sotto `UFFICIO/Officina/build/Testing/Temporary/LastTest.log`. Nessun file è stato scritto o modificato sotto `Test area/` da questa run.

## Copertura dei 6 tratti

Sono presenti ed eseguiti tutti e 6 i livelli con smoke e percorso:
lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop — tutti Passed su entrambe le modalità, più i 5 modi di simulazione (accelerazione, crash, sterzo, curve, gara).

## Blocchi

Nessun blocco. La riesecuzione è verde su tutti e 17 i test. Resta ferma la regola di processo: questo report è l'esito dell'autore e non costituisce approvazione; la chiusura formale di CP-005 richiede la review indipendente di @developer-junior e la decisione del @capo (aggiornamento di `Checkpoint Revision/` di competenza del @capo, non di questo ruolo).
