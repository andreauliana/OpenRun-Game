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
- `AUTO_GIOCATORE_LARGHEZZA = 100`: larghezza a schermo cercata, scala intera
  calcolata sulla carrozzeria della posa dritta (pixel netti).
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
