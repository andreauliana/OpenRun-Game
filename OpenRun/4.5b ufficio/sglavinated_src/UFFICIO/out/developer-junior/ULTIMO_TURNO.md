# ULTIMO_TURNO — developer-junior (11/11/2026 11:40)

## Blocco e passo
- BLOCCO V47 / VJ5 ∞ review del codice (file nuovo rispetto al turno prima: `render/sprite_raw.cpp`).

## File cambiati
- `motore_nuovo/test/prova_gate.cpp`: aggiunto `spriteVisibile()` + warning se un raw sponsor/rivale/traffico decodifica ma resta tutto trasparente (pixel tutti indice 0).
- `UFFICIO/out/developer-junior/REVIEW_V47.md` (riscritto), `UFFICIO/bacheche/developer-junior/diario.md` (1 riga), `RIASSUNTO.md` (riscritto).

## Comandi ed exit code
- `cmake --build UFFICIO/Officina/build --target prova_gate` — exit 0.
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_rawvis_1111" lv_02_atomico smoke` — exit 0 GATE OK: 21/21 sponsor e 7/7 rivali caricati e visibili, 0 warning trasparenti, replay 2x IDENTICO, ms/frame medio 0.930 max 3.235.

## Review VJ5 (dettagli in REVIEW_V47.md)
- `src/render/sprite_raw.cpp:46-67`: scansione lineare palette per ogni pixel, O(npix x |palette|) al caricamento (incide sulle misure V8); consiglio cache colore->indice (risultato identico, voci esistenti immutabili).
- `sprite_raw.cpp:73-75`: errore → Sprite vuoto silenzioso, nessuna distinzione file mancante vs corrotto (chiamanti come `game.cpp:634` non loggano).

## Esito
- VJ5 ∞ fatto su file nuovo; nessuno sprite tutto trasparente nei dati attuali. gate_t1.sh non eseguito.
