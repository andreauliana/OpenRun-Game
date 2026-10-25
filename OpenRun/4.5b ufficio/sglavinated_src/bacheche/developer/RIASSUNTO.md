# Riassunto developer — 25/10/2026 turno 1 (06:41)

- Gate T1 su macOS (darwin) `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0 — GATE T1: TUTTO OK, 6/6 percorsi GATE PERCORSO OK (rettilineo 55-71%, curva max 0.72-0.98, fuori strada 0%), replay 2x IDENTICO, ms/frame medio 0.21 max 4.9 (limite 20), accelerazione/ribaltamento/sterzo/curve/gara 4 bivi/frequenza 60/50 Hz/screenshot P1 OK.
- R1 (varietà percorsi) verificato su Mac: tutti e 6 i livelli guidabili, risolve ANDREA_REVIEW T1-Q3/T1-Q5 (giro guidato era fallito 5/6 su Mac 19/10).
- Nessuna modifica di codice necessaria (gate già verde; config frequenza_hz=60, hud tempo OK, BIVIO_SVOLTA 4.0e-4, OFFROAD graduale fatti).
- Blocchi restanti: DECO1 in attesa asset @grafico; PIST1/RIV/R2 futuri; v91 runner Mac e fps/temp Pi 400 di Andrea.
- Scritture: diario.md, RIASSUNTO.md, out/developer/RISULTATI_25OTT_0641.md.
