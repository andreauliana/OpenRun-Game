# Regole dell'auto del giocatore — BLOCCATE (Andrea, 06/11)

Queste regole le ha decise Andrea dopo le prove al runner. **Gli agenti non le
modificano**: niente cambi a valori, file o codice elencati qui. Se un task
sembra richiederlo, l'agente si ferma e lo scrive in PER_CLAUDE.md. Solo Andrea
(o Claude su richiesta di Andrea) cambia queste regole.

## Grafica
- L'auto la disegna **solo Andrea** a mano (Pixelorama). Cartella:
  `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/sprite/giocatore_pose/`.
- Pose: `auto_dritta.png`, `auto_sx1..sx4.png` (sterzo a sinistra, 4 = pieno),
  `auto_dx1..dx4.png` facoltative (senza, il gioco specchia le sx),
  `auto_salita.png` / `auto_discesa.png` facoltative. Stop accesi:
  `auto_stop.png` e `auto_stop_<posa>.png`. Tutte della stessa misura.
- `auto_sx1..sx4` e i loro stop ora sono segnaposto di
  `TOOLS/genera_pose_sterzo.py`; quando Andrea li ridisegna lo script non li
  tocca piu'. Conversione: `TOOLS/converti_pose_auto.py`.
- Gli agenti non disegnano, generano, ritoccano o cancellano file di quella
  cartella ne' i raw in `motore_nuovo/dati/sprite_raw/giocatore/pose/`.

## Posizione e grandezza (motore_nuovo/src/render/renderer.h)
- `AUTO_GIOCATORE_LARGHEZZA = 50` (Andrea 07/11: 100 era il doppio dei rivali):
  grande come un rivale sulla stessa riga. Larghezza a schermo cercata, scala intera
  calcolata sulla carrozzeria della posa dritta (pixel netti): con la posa
  di oggi (carrozzeria 25 px) le misure possibili sono 50, 75 o 100 px.
- `AUTO_GIOCATORE_TERRA_Y = 222`: le ruote appoggiano sulla strada in basso.
- L'auto resta centrata (x = 160 + piccolo spostamento con lo sterzo).

## Comportamento (motore_nuovo/src/game/game.cpp)
- Nessun sollevamento ad alta velocita': `SOLLEVAMENTO_MAX_PX = 0`.
- Salti sui dossi piccoli: `SALTO_PX_PER_UNITA = 0.02`, `SALTO_MAX_PX = 6`.
- All'arrivo l'auto resta a terra (niente sollevamento ne' salti).
- Con le pose disegnate l'auto **non si inclina** in curva: la curva la mostra
  la posa. Fuori strada solo una leggera inclinazione (0.05 rad).
- Restano: rotazione nell'incidente, scodata e sobbalzo negli urti, fumo delle
  gomme, scintille, fumo dal cofano, ammaccature (fino a 8), stop accesi in
  frenata. Niente lampo bianco sugli urti, niente luci disegnate dal motore.
- Frequenza 60 Hz (config.json `frequenza_hz`).

## Guida e reazioni (Andrea 06/11 sera)
- **Curve:** spingono fuori col quadrato della velocita':
  `CURVA_SPINTA_V2 = 1200` (game.cpp, unita'/s a curva 1 e punta). Fuori
  strada la spinta e' un quarto. A solo gas l'auto esce nelle curve; sterzando
  le curve si tengono, nelle piu' strette conviene alzare il piede.
- **Sterzo legato alla velocita'** (player.h): `STEER_MAX = 700`, pieno da 1/4
  della punta, `STEER_CALO_PUNTA = 0.35` (alla punta ~455 unita'/s): piano si
  gira di piu', forte di meno.
- **Fuori strada:** rallenta, l'auto trema sul terreno; rientrando
  sull'asfalto un piccolo sobbalzo (`RIENTRO_S = 0.35`). Incidente solo dopo
  2 s fuori strada **e** sopra il 30% della punta (`OFFROAD_INCIDENTE_V`);
  piano nell'erba rallenta e basta. Il ciglio resta: primo tocco rallenta e
  si ricorda, il secondo forte e' incidente.
- **Sportellate ripetute:** memoria `colpiMemoria_` (+1 rivale, +1.5
  traffico, -1 ogni 5 s); a 3.5 e sopra il 40% della punta l'auto vola e fa
  l'incidente.
- **Incidente:** l'auto sale fino a 30 px girando in aria, atterra, scivola al
  centro della corsia e riparte da ferma nello stesso punto della pista.
- **Rivali con memoria dei danni:** ogni botta del giocatore -3% di punta (fino
  a 3), alla 4a KO per 4 s (si fa da parte quasi fermo, poi riparte). Il
  triangolo si accende a ogni botta.
- **Oggetti:** i paletti del bordo si abbattono passandoci sopra (oltre
  `PALO_X = 860` dal centro corsia): volano via girando, -3% di velocita',
  piccolo sobbalzo. Cartelloni: sopra meta' punta incidente, sotto botta secca
  (-40%).
