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
- `AUTO_GIOCATORE_TERRA_Y = 219`: le ruote appoggiano sulla strada in basso;
  sotto c'e' sempre l'ombra di contatto (`AUTO_OMBRA_RIGHE = 3`, scurisce la
  strada, bordi a scacchiera; nei salti resta a terra e si stringe).
- Rivali: `SCALA_RIVALE = 1.7` (game.cpp), un po' piu' grandi del giocatore
  alla stessa riga (Andrea 07/11).
- Curve: sterzando dentro una curva veloce la posa sale di 1-2 livelli e
  l'auto scivola fino a ~5 px verso l'esterno (`CURVA_DERIVA_SCHERMO = 140`).
- L'auto resta centrata (x = 160 + piccolo spostamento con lo sterzo).

## Comportamento (motore_nuovo/src/game/game.cpp)
- **Curve (Andrea 08/11):** la curva spinge l'auto verso l'ESTERNO (curva a destra -> spinta a sinistra, col quadrato della velocita'); si tiene la curva sterzando DENTRO, cioe' a destra in una curva a destra. Prima il segno era invertito e bisognava sterzare dalla parte opposta: non rimetterlo.
- **Razzo alla punta** (Andrea 07/11, sostituisce il "niente sollevamento"
  del 06/11): dopo `RAZZO_ATTESA_S = 1` s alla punta in rettilineo l'auto si
  alza fino a `SOLLEVAMENTO_MAX_PX = 7` px e galleggia; due getti ciano sotto
  la pancia, fiammelle dagli scarichi, scie di velocita' ai lati. Ricade in
  0.3 s in curva, frenando o fuori strada. L'ombra resta a terra.
- **Animazioni** (`EffettiAuto`, renderer): sgommata con fumo dalla ruota
  esterna e segni a terra sterzando forte dentro una curva veloce; zolle
  d'erba/terra fuori strada; all'atterraggio dei salti le sospensioni si
  schiacciano (1-3 px) e la polvere scappa ai lati (scintille se il salto era
  alto); sul ponte un "tac" di 1 px ogni `PONTE_GIUNTO_M = 12` m.
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
  della punta, `STEER_CALO_PUNTA = 0.25` (08/11, era 0.35; alla punta ~525
  unita'/s): piano si gira di piu', forte di meno, ma alla punta lo sterzo
  resta piu' forte della spinta anche nelle curve strette (lv_05 a tutto gas:
  max 652 su 700).
- **Sportellata (Andrea 08/11):** sterzando verso un'auto affiancata lo sterzo
  e' pieno anche alla punta (`AFFIANCATA_DX = 650`, game.cpp), cosi' si riesce
  a colpirla di lato. I rivali che arrivano da dietro passano a
  `RIVALE_SORPASSO_DX = 360` dal giocatore (prima scappavano a 450 dal centro).
- **Tempi delle tappe (Andrea 08/11: "basta schiacciare l'acceleratore e
  arrivi alla fine"):** `tempo_limite_s` di ogni livello = circa 1.45 volte il
  tempo del bot pulito del gate gara (lv_01 50, lv_02 48, lv_03 59, lv_04 48,
  lv_05 54, lv_06 47); al cambio di tappa si aggiunge l'80%
  (`tempo_extra_fattore` 0.8 in config.json). Guida pulita: arrivo con ~25 s
  di margine; un incidente per tappa: si arriva per poco; solo acceleratore:
  il tempo finisce alla 2a o 3a tappa. Se si allunga o accorcia un livello,
  ricalcolare il suo `tempo_limite_s` con la stessa regola; non alzarli a caso.
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
