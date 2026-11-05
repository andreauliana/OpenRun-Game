# Guida grafica per Andrea: quali PNG disegnare

Regola d'oro: **stesso nome, stessa misura, PNG RGBA con fondo trasparente**.
Metti il tuo file al posto di quello esistente, poi dalla cartella `sglavinated_src` lancia:

    python3 motore_nuovo/strumenti_py/png_a_raw.py

e rilancia il gioco. (Se un tipo di file non viene convertito da quello script, chiedi a Claude.)
Tutti i percorsi qui sotto partono da `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/`.
Stile: palette piccola (max 16 colori per sprite) + dithering (Bayer, Atkinson, halftone), pixel art.
Gli agenti non sovrascrivono mai un tuo PNG.

## 1. Auto del giocatore (da fare per prima, blocco AUTO1)

Cartella: `sprite/giocatore/` (se non c'e' ancora, creala). Auto vista da dietro e un po' dall'alto,
centrata, ruote appoggiate al bordo basso del PNG.

| File | Misura | Cosa disegnare | Obbligatorio |
|---|---|---|---|
| `auto_dritto.png` | 64x32 | auto dritta | si' |
| `auto_sx1.png` | 64x32 | sterzo leggero a sinistra: muso un po' girato, si intravede il fianco destro | si' |
| `auto_sx2.png` | 64x32 | sterzo pieno a sinistra: fianco piu' visibile | si' |
| `auto_dx1.png`, `auto_dx2.png` | 64x32 | come sopra a destra | no (se mancano il gioco specchia sx1/sx2) |
| `auto_salita.png` | 64x32 | dritta in salita (si vede piu' il tetto) | no |
| `auto_discesa.png` | 64x32 | dritta in discesa (si vede piu' il paraurti) | no |
| `<nome>_b.png` | 64x32 | stesso fotogramma con le ruote al secondo passo (es. `auto_dritto_b.png`, `auto_sx1_b.png`) | no (senza: solo rimbalzo) |
| `auto_sbandata.png` | 64x32 | auto di traverso dopo un urto | no |
| `auto_ribalta_1.png` ... `auto_ribalta_4.png` | 64x48 | quattro pose del ribaltamento | no (senza: il gioco ruota il fotogramma) |

Il gioco sceglie da solo il fotogramma in base a sterzo, salita/discesa, velocita' e incidente.
Stop, ombra, fumo e lampo dell'urto li aggiunge il motore sopra il tuo disegno.

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
