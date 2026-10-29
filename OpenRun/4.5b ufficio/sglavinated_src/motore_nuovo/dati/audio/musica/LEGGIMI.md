# Musica del gioco (brani tracker di Andrea)

Qui ci sono i brani originali in formato **.xm** (FastTracker 2), uno per file, con nomi semplici senza spazi.

- `beatmastaz.xm` e `asplajungle4.xm`: i due progetti di Andrea (29 ott). Licenza e autore come `../playlist.json` (Andrea Uliana, CC-BY-4.0).
- Il motore li suona con **libxmp-lite** (inclusa in `motore_nuovo/terzeparti/`, si compila col gioco: niente Homebrew).
- Elenco e titoli mostrati a schermo: sezione `"xm"` di `../playlist.json`. Il primo della lista e' quello che parte.
- Nel menu: SINISTRA/DESTRA cambia brano (suona subito), con equalizzatore a canali. In gara: tasto R (o Start+B sul pad).
- Per un'altra versione del gioco: sostituisci i file e aggiorna la lista. Vanno bene anche .mod e .s3m (non .it).
- La musica pilota gia' l'equalizzatore e il battito (`InfoMusica` in `src/audio/mixer.h`): e' la base del visualizzatore.
