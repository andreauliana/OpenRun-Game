# Riassunto developer — 08/11/2026 ~12:21 (turno 1)
- Gate T1: TUTTO OK exit 0, 6/6 percorsi GATE PERCORSO OK, soak 30000 frame replay IDENTICO
- Percorso D1 PERC1: curva_cieca_dx/sx e saliscendi presenti su tratti.json e lv_02_atomico, lv_03_foglie, lv_05_spray; gate verde su tutti i livelli confermato
- Build multipiattaforma C++17: core indipendente SDL, obiettivo Raspberry Pi 400 a 50 fps 320x224
- Nessuna modifica src/runner/ (vincolo Andrea 06/11); soglie gate: rettilineo ~35-75%, curva max 0.86
- PRODUZIONE: dati percorso D1 aggiunti e verificati via gate_t1.sh, nessun file sorgente modificato questo turno
- Tocca a (08/11 Claudio/Andrea): lavoro su percorsi (tracciati, curve, salite, bivi, strada, traffico, rivali), NON HUD/auto/premiazione
- Blocchi aperti [~]: gate T1 verification completo