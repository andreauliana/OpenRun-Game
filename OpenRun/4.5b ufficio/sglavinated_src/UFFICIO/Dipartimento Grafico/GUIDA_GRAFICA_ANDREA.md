# Guida grafica per Andrea: quali PNG disegnare

Regola d'oro: **stesso nome, stessa misura, PNG RGBA con fondo trasparente**.
Metti il tuo file al posto di quello esistente, poi dalla cartella `sglavinated_src` lancia:

    python3 motore_nuovo/strumenti_py/png_a_raw.py

e rilancia il gioco. (Se un tipo di file non viene convertito da quello script, chiedi a Claude.)
Tutti i percorsi qui sotto partono da `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/`.
Stile: palette piccola (max 16 colori per sprite) + dithering (Bayer, Atkinson, halftone), pixel art.
Gli agenti non sovrascrivono mai un tuo PNG.

## 1. Auto del giocatore (pose disegnate da Andrea, decisione 06/11)

Cartella: `sprite/giocatore_pose/`. Auto vista **da dietro e un po' dall'alto** (tre quarti), come le referenze.
Tutte le pose hanno la **stessa misura del PNG della dritta** (oggi 42x25: i margini vuoti vengono tagliati da soli).
Il gioco le ingrandisce a scala intera fino a circa 52 px di larghezza (la tua rossa oggi e' x2).

| File | Cosa disegnare | Obbligatoria |
|---|---|---|
| `auto_dritta.png` | auto dritta (c'e' gia': la tua rossa) | si' |
| `auto_sx1.png` ... `auto_sx4.png` | curva a sinistra, da appena girata (1) a sterzo pieno (4): si vede sempre di piu' il fianco destro e il muso va a sinistra | consigliate |
| `auto_dx1.png` ... `auto_dx4.png` | curva a destra | no (se mancano il gioco specchia le sx) |
| `auto_salita.png`, `auto_discesa.png` | dritta in salita / in discesa | no |

Se una posa manca il gioco usa la piu' vicina e inclina un po' l'auto. Dopo aver salvato i PNG lancia
`python3 "UFFICIO/Dipartimento Grafico/TOOLS/converti_pose_auto.py"`.
Il motore aggiunge da solo: ombra, sollevamento in velocita', rotazione nel ribaltamento, scodata, fumo e scintille.
Fanali e stop sono nel tuo disegno; lo stop acceso in frenata potra' diventare un piccolo PNG sopra la posa (`auto_stop.png`, da decidere).

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
