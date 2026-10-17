# SPDX-License-Identifier: CC0-1.0
# Riepilogo consegna BLOCCO L — @scenografo

Data: 16 ott 2026 | Blocco: L (dopo K [x]) | Stato: varianti consegnate, in attesa scelta definitiva di Andrea

## Prodotti (in assets_originali/scenari/<id>/)

| livello | scenario | stile | varianti | skyline | oggetti per variante | palette | manifest |
|---|---|---|---|---|---|---|---|
| lv_01_neon | scenario_neon_01 | synthwave | A, B, C | skyline.png 256x128 | 5 / 5 / 4 (obj_*.png in oggetti/) | 16 colori + palette.json | manifest.json per variante |
| lv_02_atomico | scenario_atomico_02 | sci-fi anni '50 | A, B, C | skyline.png 256x128 | 6 / 6 / 8 (obj_*.png in oggetti/) | 16 colori + palette.json | manifest.json per variante |
| lv_03_foglie | scenario_foglie_03 | psichedelico / cannabinoidi | A, B, C | skyline.png 256x128 | 5 / 5 / 8 (obj_*.png in oggetti/) | 16 colori + palette.json | manifest.json per variante |

## Note
- Generatore: bacheche/scenografo/genera_bloccoL.py (Python 3 + Pillow, disegnato da zero, seme fisso -> deterministico).
- Licenza: CC0-1.0 su ogni variante; originale, nessun materiale Sega/tracciato.
- Crediti: assets_originali/CREDITI.md (3 nuove righe).
- Ogni file ha licenza dichiarata nel suo manifest.json.
- Max 16 colori per sprite (oggetti); gli sfondi usano gradienti su fondo scuro come consentito da DIREZIONE_ARTISTICA.md.

## Verifica (16 ott 2026)
- python3 bacheche/scenografo/verify.py: 9/9 varianti OK; oggetti 4-8 per variante; palette 16 colori + palette.json; manifest livello/variante coerenti.
- check_catalogo.py OK ("catalogo: 11 righe, 2 cartelle"); quota_open.py 91.8% (247656/269832 byte, soglia 50%).
- Skyline con gradienti di fondo scuro (DIREZIONE_ARTISTICA.md: nebbia/gradienti consentiti); max 16 colori = vincolo sugli sprite/oggetti, rispettato.

## Prossimo passo
Andrea sceglie una variante definitiva (A, B o C) per ciascun livello. Dopo la scelta, confermo in SCAMBIO.md e il BLOCCO L si intende chiuso (RICHIESTA.md L -> [x]).
