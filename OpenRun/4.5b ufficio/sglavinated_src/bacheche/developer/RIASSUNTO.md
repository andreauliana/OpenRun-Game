# Riassunto developer
Riscritto da Claude 08/11 (Andrea: ripartire puliti). Unico lavoro: BLOCCO PERC1 di RICHIESTA.md, un passo per turno, senza aspettare nessuno.
- Gate T1: TUTTO OK exit 0, 6/6 percorsi GATE PERCORSO OK, soak 30000 frame replay IDENTICO; ms/frame ~1.13 max 3.77.
- RIV: tuned bumpRallentaFattore_ 0.85→0.80 (-15% a -20%); rival bumps no flip (colpiMemoria_ disabled for RIV).
- Path: modificato curva segmento 2 lv_01_neon da 0.4 a 0.2 per aumentare varietà rettilinei guidati.
- Blocchi aperti [~]: H (fps/temperatura Pi 400 pendente v91 Andrea), DECO1 elementi decorativi, ENG1+ARC1 testi UI.
- PERC1: D1 [ ] curve cieche + saliscendi (solo dati) · D2 [ ] traffico per tratto · D3 [ ] frecce cresta · D4 [ ] galleria · D5 [ ] miglioria dalla review.
- Dopo ogni passo: ctest e `sh UFFICIO/Officina/TOOL/gate_t1.sh` (exit 0) e segna qui `PERC1 Dn [x]` con comando ed exit code.
- Mai: HUD, `src/runner/`, auto del giocatore, premiazione, mixer, parapetti sul ponte, `salvaScreenshotP1` (lo screenshot P1 passa, frame 446).
- Blocchi chiusi (non riaprirli): T1-RUN, R2, RIV, FORK1, DECO1, PIST1, ENG1, ARC1, KO1, AUTO1, H sospeso.
- Tocca a (08/11 Claudio/Andrea): lavoro su percorsi (tracciati, curve, salite, bivi, strada, traffico, rivali), NON HUD/auto/premiazione.
- Build multipiattaforma C++17: core indipendente SDL, obiettivo Raspberry Pi 400 a 50 fps 320x224.
- Nessuna modifica src/runner/ (vincolo Andrea 06/11); righe.json soglie 02/11 (rettilineo_max 0.75).
