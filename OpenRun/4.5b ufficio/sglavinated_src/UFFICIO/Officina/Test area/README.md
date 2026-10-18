# Test area

Contiene log, screenshot e prove generati dai comandi di build/test; il revisore li consulta in sola lettura. Il riepilogo corrente destinato al Capo è in [STATUS_OFFICINA.md](../STATUS_OFFICINA.md). Le fonti dei test restano temporaneamente in `../../../motore_nuovo/test/` per mantenere funzionanti CMake e gli script esistenti.

I file sotto `strumenti/` sono riservati a strumenti di test assegnati al Developer/Builder Junior; gli output generati non vanno modificati a mano.

Il gate integrato scrive `gate_t1.log` e i risultati per livello qui; CTest conserva i risultati generati sotto `../build/test-results/`.
