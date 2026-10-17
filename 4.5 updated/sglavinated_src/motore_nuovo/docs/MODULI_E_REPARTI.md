# Moduli e reparti (regola: ogni cosa ha il suo scomparto)

## Reparti
| Reparto | Agenti | Possiede | Non tocca |
|---|---|---|---|
| **Grafico** | @cercaasset, @asset, @testi, @scenografo, @tipografo | `assets_originali/`, `dati/personaggi/*/` (sprite, palette, fumetti, manifest) | codice C++ |
| **Tecnico** | @architetto, @coder, @traduttore, @debugger | `src/`, `strumenti_py/`, test | sprite e testi (li legge soltanto) |
Si parlano solo tramite il **manifest** di ogni personaggio/effetto e SCAMBIO.md.

## Linguaggi
- Motore: C++17. Strumenti, build, conversione asset, controlli: **Python 3** (come nella hack v87: pipeline unica, controlli che fanno fallire la build).

## Scomparti del motore (`src/`), ognuno separato e testabile da solo
| Cartella | Compito |
|---|---|
| core/ | frame counter, RNG con seme, stato, replay |
| road/ | strada a segmenti, curve, colline, traffico |
| render/ | framebuffer 320x224, sprite, palette |
| audio/ | mixer int16 stereo; brani e radio letti da dati/audio/playlist.json (musica = riferimento, sostituibile) |
| input/ | pad tipo RetroPad |
| personaggi/ | Gianni, Gesu', UFO: logica show, scelta frasi |
| fx/<nome>/ | **una cartella per funzione extra**: razzo (B), sollevamento auto, effetti colore... |
| ui/ | menu, titolo, HUD letti da dati/ui/*.json (font e grafica: @tipografo) |
## Regola dei moduli extra
Ogni funzione extra e' un modulo con: `init()`, `update(stato)`, `draw(fb)`, un flag on/off in `dati/config.json`, i propri dati e un test. Il core non dipende da nessun extra: spegnendo un modulo il gioco gira uguale. Un extra si elabora, si prova e si ritira senza toccare gli altri.

## Catalogo (obbligatorio, altrimenti i personaggi «scompaiono»)
- `dati/personaggi/<nome>/manifest.json`: id, ruolo, pose/frame, file sprite, palette, fumetti, crediti, licenza, stato.
- `docs/CATALOGO_PERSONAGGI.md`: inventario di tutti (anche quelli della hack v87 da portare).
- `strumenti_py/check_catalogo.py` confronta cartelle e catalogo e fallisce se non coincidono. Va eseguito prima di ogni consegna.
