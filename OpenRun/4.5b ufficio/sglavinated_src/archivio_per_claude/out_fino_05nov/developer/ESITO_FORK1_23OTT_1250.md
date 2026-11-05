# Esito FORK1 — 23/10/2026 12:50

Comando: sh UFFICIO/Officina/TOOL/gate_t1.sh (eseguito da solo, nessun compositore di comandi)
Exit code: 0
Misure (UFFICIO/Officina/Test area/gate_t1.log): GATE T1: TUTTO OK; 6/6 percorsi GATE PERCORSO OK (fuori strada 0% su tutti i livelli); ms/frame medio 0.169-0.175, max 0.361 (limite 20); replay 2x IDENTICO su tutti i livelli e sulla gara; gara a tappe 4 combinazioni IDENTICO; frequenza 60 Hz OK, 50 Hz OK; screenshot P1 OK.
Modifica codice: motore_nuovo/src/road/road.h — BIVIO_SVOLTA 2.0e-4f -> 3.0e-4f (feedback Andrea 23/10, prova runner Mac: incrementare leggermente la curvatura dei due Fork per evitare crash sui bordi della parte centrale al bivio).
Restano: tema transizione graduale (FORK1 punto 6, Renderer interpola TemaColori), gate Junior (bivio_uscita/bivio_bordi/tema_transizione), DECO1 (@grafico).
