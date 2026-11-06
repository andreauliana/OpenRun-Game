# Guida grafica per Andrea: quali PNG disegnare

Regola d'oro: **stesso nome, stessa misura, PNG RGBA con fondo trasparente**.
Metti il tuo file al posto di quello esistente, poi dalla cartella `sglavinated_src` lancia:

    python3 motore_nuovo/strumenti_py/png_a_raw.py

e rilancia il gioco. (Se un tipo di file non viene convertito da quello script, chiedi a Claude.)
Tutti i percorsi qui sotto partono da `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/`.
Stile: palette piccola (max 16 colori per sprite) + dithering (Bayer, Atkinson, halftone), pixel art.
Gli agenti non sovrascrivono mai un tuo PNG.

## 1. Auto del giocatore (da fare per prima, blocco AUTO1)

**Decisione 06/11: l'auto e' fatta A PEZZI** (come il vecchio modello a rettangoli). I pezzi sono in `sprite/giocatore/`:

| File | Misura | Cosa e' |
|---|---|---|
| `carrozzeria.png` | 52x15 | retro dell'auto senza tetto e senza ruote (fanali spenti, targa, paraurti) |
| `abitacolo.png` | 30x7 | tetto e lunotto, sta dietro la carrozzeria |
| `alettone.png` | 46x3 | alettone sopra il baule |
| `ruota_1.png`, `ruota_2.png`, `ruota_3.png` | 7x12 | ruota posteriore, 3 passi del battistrada |
| `fanale_acceso.png` | 9x2 | fanale stop acceso (frenata), usato a sinistra e a destra |

Ridisegna uno di questi (stesso nome e misura), poi lancia
`python3 "UFFICIO/Dipartimento Grafico/TOOLS/disegna_auto_pezzi.py"`: tiene i tuoi PNG e rifa' i raw.
Il motore li muove, li inclina e li ruota. Le tabelle qui sotto (fotogrammi interi) sono superate e restano solo come riferimento.

**Aggiornamento 06/11:** il grafico ha gia' fatto le 7 pose della carrozzeria in `sprite/auto_gt/`, con questi nomi:
`auto_giocatore_dritta.png`, `auto_giocatore_sterzo_sx_leggero.png`, `auto_giocatore_sterzo_sx_forte.png`,
`auto_giocatore_sterzo_dx_leggero.png`, `auto_giocatore_sterzo_dx_forte.png`, `auto_giocatore_salita.png`,
`auto_giocatore_discesa.png` (tutte 64x32). Per ridisegnarle tu, sostituisci QUESTI file (stesso nome, stessa misura).
La tabella qui sotto resta valida per cosa disegnare in ogni posa; i componenti (ruote, stop, fumo) vanno in `sprite/giocatore/`.

Cartella dei componenti: `sprite/giocatore/` (se non c'e' ancora, creala). Auto vista da dietro e un po' dall'alto,
centrata, ruote appoggiate al bordo basso del PNG.

| File | Misura | Cosa disegnare | Obbligatorio |
|---|---|---|---|
| `auto_dritto.png` | 64x32 | auto dritta | si' |
| `auto_sx1.png` | 64x32 | sterzo leggero a sinistra: muso un po' girato, si intravede il fianco destro | si' |
| `auto_sx2.png` | 64x32 | sterzo pieno a sinistra: fianco piu' visibile | si' |
| `auto_dx1.png`, `auto_dx2.png` | 64x32 | come sopra a destra | no (se mancano il gioco specchia sx1/sx2) |
| `auto_salita.png` | 64x32 | dritta in salita (si vede piu' il tetto) | no |
| `auto_discesa.png` | 64x32 | dritta in discesa (si vede piu' il paraurti) | no |
| `auto_sbandata.png` | 64x32 | auto di traverso dopo un urto | no |
| `auto_ribalta_1.png` ... `auto_ribalta_4.png` | 64x48 | quattro pose del ribaltamento | no (senza: il gioco ruota il fotogramma) |

L'auto e' **a componenti**: i fotogrammi qui sopra sono solo la carrozzeria, **senza le ruote posteriori**.
Le parti animate si disegnano una volta sola e il gioco le riusa su tutti i fotogrammi:

| File | Misura | Cosa disegnare |
|---|---|---|
| `ruota_1.png`, `ruota_2.png`, `ruota_3.png` | 12x10 | ruota posteriore sinistra vista da dietro, 3 passi di rotazione (la destra e' specchiata) |
| `luci_stop.png` | 12x4 | fanale posteriore sinistro acceso (il destro e' specchiato) |
| `fumo_1.png`, `fumo_2.png`, `fumo_3.png` | 16x16 | sbuffo di fumo in 3 passi |

Dove va ogni ruota e ogni fanale per ogni fotogramma lo dice `auto_giocatore.json` nella stessa cartella
(lo prepara il @grafico; se sposti le ruote nel tuo disegno, cambia li' i numeri in pixel).
Il gioco sceglie da solo il fotogramma in base a sterzo, salita/discesa, velocita' e incidente;
la carrozzeria rimbalza leggermente mentre le ruote restano a terra. Se un componente manca, il gioco
usa il disegno semplice di oggi. Ombra e lampo dell'urto li aggiunge il motore.

## 2. Il resto che si vede gia' in gioco

| Cosa | Dove | Misura |
|---|---|---|
| Rivali (7 colori) | `sprite/rivali/auto_rivale_<colore>_retro.png` | 48x24 |
| Traffico (4 veicoli) | `sprite/traffico/traffico_<nome>_retro.png` | quella del file attuale (34-42 x 22-32) |
| Pubblico all'arrivo | `sprite/pubblico/pubblico_<a/b>_<giu/su>.png` | 40x26 |
| Cartelloni sponsor | `sponsor/<sponsor>/cartellone_<neon/atomico/foglie>.png` | 128x64 |
| Parapetto del ponte / guardrail | `scenari/comuni/oggetti/parapetto.png` / `guardrail.png` | 32x48 / 48x32 |
| Skyline e oggetti di ogni livello | il file indicato in `CATALOGO_GRAFICO.json` (campo `file`) della voce `scenario.<livello>.skyline` o dell'oggetto | quella del file attuale (skyline 256x128) |

Per sapere quale PNG diventa quale sprite nel gioco: in `CATALOGO_GRAFICO.json` ogni voce ha `file` (il tuo PNG)
e, quando c'e', `raw` (il file che legge il motore).
