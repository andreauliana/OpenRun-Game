# Test area

Contiene log, screenshot e prove generati dai comandi di build/test; il revisore li consulta in sola lettura. Il riepilogo corrente destinato al Capo è in [STATUS_OFFICINA.md](../STATUS_OFFICINA.md). I sorgenti dei test stanno in `../../../motore_nuovo/test/` e restano lì: CMake li compila da quella cartella.

I file sotto `strumenti/` sono riservati a strumenti di test assegnati al Developer/Builder Junior; gli output generati non vanno modificati a mano.

Il gate integrato scrive `gate_t1.log` e i risultati per livello qui (esclusi da git con `.gitignore`: si rigenerano a ogni gate); CTest conserva i risultati generati sotto `../build/test-results/`.
