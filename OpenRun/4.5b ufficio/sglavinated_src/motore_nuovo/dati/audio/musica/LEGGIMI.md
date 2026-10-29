# Musica del gioco (brani tracker di Andrea)

Qui ci sono i brani originali in formato **.xm** (FastTracker 2), uno per file, con nomi semplici senza spazi.
Licenza e autore come `../playlist.json` (Andrea Uliana, CC-BY-4.0).

- `beatmastaz.xm` (102 BPM): il brano del **menu**.
- `septejungle2.xm` (160 BPM) e `asplajungle4.xm` (168 BPM): i brani della **gara**.
- Il motore li suona con **libxmp-lite** (inclusa in `motore_nuovo/terzeparti/`, si compila col gioco: niente Homebrew).
- Quale brano va dove: `../playlist.json`, voce `"menu"` (uno) e lista `"gara"`.
- In gara i brani cambiano a ogni tappa, a giro; ogni nuova gara parte dal brano successivo. Il tasto R (o Start+B sul pad) passa al prossimo. A schermo c'e' solo lo spettro, senza titolo.
- Per un'altra versione del gioco: sostituisci i file e aggiorna la lista. Vanno bene anche .mod e .s3m (non .it).
- La musica pilota lo spettro, il battito e i fuochi della scena finale (`InfoMusica` in `src/audio/mixer.h`).
