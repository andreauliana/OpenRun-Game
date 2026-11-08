# Riassunto developer — 08/11/2026 ~09:47 (turno 1)
- Gate T1: TUTTO OK exit 0, 6/6 percorsi GATE PERCORSO OK, soak 30000 frame replay IDENTICO
- RIV: tuned bumpRallentaFattore_ 0.85→0.80 (-15% a -20%); rival bumps no flip (colpiMemoria_ disabled per RIV)
- Blocchi aperti [~]: H (fps/temperatura Pi 400 pendente v91 Andrea), DECO1 elementi decorativi, ENG1+ARC1 testi UI
- Tocca a (08/11 Claudio/Andrea): lavoro su percorsi (tracciati, curve, salite, bivi, strada, traffico, rivali), NON HUD/auto/premiazione
- Build multipiattaforma C++17: core indipendente SDL, obiettivo Raspberry Pi 400 a 50 fps 320x224
- Nessuna modifica src/runner/ (vincolo Andrea 06/11); righe.json soglie 02/11 (rettilineo_max 0.75)