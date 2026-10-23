# Musica del gioco (brani tracker di Andrea)

Metti qui i brani originali in formato **.xm** (FastTracker 2), uno per file, con nomi semplici senza spazi (es. `moonlight.xm`).

- Licenza e autore: come `playlist.json` (Andrea Uliana, CC-BY-4.0), salvo diversa indicazione nel nome o qui sotto.
- Il motore oggi NON legge ancora i .xm: la riproduzione con libxmp e il visualizzatore nell'HUD (canali, pattern, strumenti letti dalla traccia) sono un blocco da aprire per gli agenti.
- Quando i file ci sono, vanno elencati in `../playlist.json` (sezione da aggiungere, es. `"xm": {"moonlight": "musica/moonlight.xm"}`) e assegnati a livelli o fasi di gioco.
- Formati affini (.mod, .s3m, .it) libxmp li legge comunque, ma il riferimento del progetto resta .xm.
