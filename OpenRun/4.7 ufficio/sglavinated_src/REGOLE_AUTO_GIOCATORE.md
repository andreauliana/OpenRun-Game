# Regole dell'auto del giocatore — BLOCCATE (Andrea, 06/11; guida e partenza riviste l'11/11)

Queste regole le ha decise Andrea dopo le prove al runner. **Gli agenti non le
modificano**: niente cambi a valori, file o codice elencati qui. Se un task
sembra richiederlo, l'agente si ferma e lo scrive in PER_CLAUDE.md. Solo Andrea
(o Claude su richiesta di Andrea) cambia queste regole.

## Grafica
- L'auto la disegna **solo Andrea** a mano (Pixelorama). Cartella:
  `GRAFICA/sprite/giocatore_pose/`.
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
- Derapata (Andrea 16/11, "nelle curve maggiore derapata"): coda fuori di
  piu' a schermo (`DERAPATA_DERIVA_SCHERMO = 170`, era 120). Pose disegnate da
  Andrea invariate.
- Cambio automatico a tre marce (Andrea 16/11, prima due): 1a -> 2a a 90,
  2a -> 3a a 170 unita'/s (scalata a 75 e 155). Contagiri e suono del motore
  salgono in ogni marcia e calano a ogni cambiata (`Player::giri()`,
  `MARCIA_SU`, `MARCIA_GIU`, `GIRI_DOPO_CAMBIATA = 0.55` in player.h).

## Comportamento (motore_nuovo/src/game/game.cpp)
- **Curve (Andrea 08/11):** la curva spinge l'auto verso l'ESTERNO (curva a destra -> spinta a sinistra, col quadrato della velocita'); si tiene la curva sterzando DENTRO, cioe' a destra in una curva a destra. Prima il segno era invertito e bisognava sterzare dalla parte opposta: non rimetterlo.
- **Sollevamento leggero alla punta** (Andrea 08/11 sera, sostituisce il
  "razzo" del 07/11): dopo `RAZZO_ATTESA_S = 1` s alla punta in rettilineo
  l'auto si stacca appena dalla strada, `SOLLEVAMENTO_MAX_PX = 3` px, e
  galleggia piano; l'ombra resta a terra e diventa PIU' scura e netta. Niente
  getti sotto la pancia, niente fiamme dagli scarichi ("non e' un'astronave");
  restano solo leggere scie di velocita' ai lati. Ricade in 0.3 s in curva,
  frenando o fuori strada.
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
- **Sterzo legato alla velocita'** (player.h): `STEER_MAX = 840` (Andrea
  11/11: "quando e' veloce deve girare di lato un po' di piu'", prima 780),
  pieno da 1/4 della punta, `STEER_CALO_PUNTA = 0.05` (alla punta ~800
  unita'/s).
- **Peso (Andrea 11/11; rivisto la sera: "non gira di lato abbastanza in
  fretta", "curve lunghe piu' realistiche, un'auto da corsa grossa e forte,
  reattiva"):** la velocita' laterale ha due parti con ritardi diversi
  (player.h). Lo **sterzo** risponde subito anche alla punta
  (`INERZIA_STERZO_BASE_S = 0.03`, `+ INERZIA_STERZO_PUNTA_S = 0.05` alla
  punta; la mattina dell'11/11 era 0.24 s per tutto). La **spinta della curva**
  (`Player::spintaLat`) arriva piano (`INERZIA_SPINTA_BASE_S = 0.10`,
  `+ INERZIA_SPINTA_PUNTA_S = 0.35`): entrando in una curva lunga il carico
  sale in circa 1 s, in uscita lo slancio verso l'esterno resta un attimo e
  va ripreso.
- **Gomme che lavorano in curva (Andrea 11/11 sera):** sterzando DENTRO la
  curva l'auto perde `CURVA_ATTRITO = 0.12` x carico al secondo (carico =
  |curva| x v^2): tenere la curva costa velocita' (curva lunga a 0.45 a
  sterzo tenuto: circa -5% al secondo; a colpetti quasi niente). Stridio
  leggero delle gomme (`CURVA_STRIDIO = 0.30` x sgommata), anche negli scarti
  secchi alla punta.
- **Sovrasterzo e controsterzo (Andrea 11/11, sostituisce la derapata
  dell'08/11):** sotto il limite di aderenza (carico = |curva| x v^2 sotto
  `PRESA_LIMITE = 0.30`, era 0.38 fino al 16/11) l'auto e' incollata e segue lo sterzo. Oltre il
  limite, sterzando forte DENTRO (sterzo >= 0.4, era 0.5) col gas premuto, la coda
  parte: l'angolo (`sovrasterzo_`, 0..1) cresce (`SOVRA_CRESCITA = 1.3`/s a
  carico pieno, `PRESA_PIENA = 0.70`, era 0.80), il muso punta dentro e l'auto viene
  tirata verso l'interno (`SOVRA_LAT = 1500` unita'/s ad angolo pieno); lo
  sterzo sposta meno l'auto (`SOVRA_PRESA_STERZO = 0.75`: le ruote tengono
  l'angolo). Il **controsterzo** (sterzare dalla parte opposta al muso)
  riprende la coda (`SOVRA_RECUPERO = 2.4`/s); sterzo al centro
  (`SOVRA_CALO`), carico sotto il limite (`SOVRA_PRESA_TORNA = 0.8`, era 1.2: la scivolata dura di piu' in uscita) o gas alzato
  (`SOVRA_SENZA_GAS`) la fanno rientrare. Le gomme che strisciano tolgono
  velocita' (`SOVRA_FRENO_PER_S = 0.93`, era 0.88). Se l'angolo arriva in fondo:
  **testacoda** (velocita' al 40%, botta, l'auto gira su se stessa per 0.7 s
  con le pose piene alternate, sterzo inutile intanto). Si vede: posa che punta
  dentro la curva anche controsterzando, coda fuori di qualche pixel, 1 px di
  tremolio, fumo pieno, stridio (mixer `setSgommata`). Niente rotazione dello
  sprite. Fuori strada, al bivio, all'arrivo e nell'incidente niente
  sovrasterzo. Prove (11/11, gara SD): solo gas finisce il tempo alla 3a
  tappa con 9 incidenti; chi sterza a fondo dentro ogni curva fa 6 testacoda e
  arriva con 11 s; chi tiene la strada senza controsterzare 3-4 testacoda;
  chi controsterza nessun testacoda e arriva con ~26 s.
- **Partenza (Andrea 11/11: "se tengo giu' l'acceleratore per secondi al
  conto alla rovescia gli altri restano tutti dietro: deve essere
  realistica"):** in griglia il gas fa salire i giri (`GIRI_SALITA_S = 0.7`,
  a gas alzato scendono in `GIRI_DISCESA_S = 0.9`); il contagiri e il motore li
  mostrano, al limitatore il motore balbetta. Allo START: giri tra
  `GIRI_BUONI_MIN = 0.55` e `GIRI_BUONI_MAX = 0.85` (perfetti a 0.70) =
  partenza pulita; al limitatore (gas tenuto a fondo, `GIRI_LIMITATORE =
  0.95`) le ruote pattinano per `PATTINA_S = 1.6` s (spinta -75%, fumo da
  tutte e due le ruote, stridio, coda che ondeggia, motore che urla); poco gas
  = motore seduto, parte piano. Nei primi `LANCIO_S = 3` s la spinta e'
  limitata dall'aderenza (`LANCIO_ACC = 55` unita'/s^2, ~150 su 250 in 3 s):
  nessuno scappa al via. I **rivali** nei primi 15 s accelerano come il
  giocatore (`Player::spintaA`, stessa aderenza della partenza) e ognuno ha il
  suo spunto (riflessi 0-0.25 s, c'e' chi pattina, chi si siede, chi parte
  perfetto); in griglia non alzano il piede per la distanza minima, chi parte
  male fa da tappo. Prove (11/11, gas a fondo per tutto il conto): 7o dopo 3 s,
  in mezzo al gruppo; partenza perfetta: 2o dopo 3 s, primo dopo ~6 s;
  nessun gas fino allo START: ultimo e rimonta. Dopo un incidente si riparte
  con un po' di pattinamento (`RIPARTENZA_PATTINA = 0.5`). Senza partenza
  (gate, prove) niente limite: l'accelerazione resta quella di sempre.
- **Aperto agli agenti dal 09/11 (BLOCCO MIG1, Andrea 08/11 sera):** SOLO la
  sportellata contro i rivali (`AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`,
  `RIVALE_SORPASSO_DX`), danni/KO/recupero dei rivali e il volo dei paletti
  abbattuti. Sterzo (`STEER_*`), peso (`INERZIA_*`, `vLat`), curve
  (`CURVA_SPINTA_V2`, `CURVA_ATTRITO`), sportellata del giocatore
  (`SPORTELLATA_*`), sovrasterzo e testacoda (`PRESA_*`, `SOVRA_*`,
  `TESTACODA_*`), partenza e pattinamento (`GIRI_*`, `LANCIO_*`, `PATTINA_*`,
  spunto dei rivali), incidenti e tempi restano bloccati.
- **Sportellata (Andrea 08/11; 11/11 sera: "e' difficile colpire di lato
  un'altra auto"):** sterzando verso un'auto vicina (`AFFIANCATA_DX = 650` di
  lato, da poco dietro a `SPORTELLATA_Z_AVANTI = 600` davanti) lo sterzo e'
  pieno anche alla punta e lo scarto e' piu' secco (`SPORTELLATA_GUIZZO =
  1.45`). Arrivandole addosso di lato con almeno `SPORTELLATA_V_LAT = 300`
  unita'/s il colpo vale anche sul suo muso (`SPORTELLATA_Z_DIETRO = 250`
  dietro la finestra di contatto: prima ci si passava attraverso) e sulla sua
  coda (`SPORTELLATA_Z_DAVANTI = 250` davanti: prima ci si infilava dietro).
  Piu' lo scarto e' veloce piu' il rivale viene spostato (fino a +60%,
  `SPORTELLATA_FORZA_EXTRA`). Prove (12 partenze in mezzo al gruppo): i
  tentativi a segno passano dal 38% a circa il 50%, in 0.45 s invece di 0.56.
  Il comportamento dei rivali non cambia (Andrea: "va bene cosi'"). I rivali
  che arrivano da dietro passano a `RIVALE_SORPASSO_DX = 360` dal giocatore.
- **Tempi delle tappe (Andrea 08/11: "basta schiacciare l'acceleratore e
  arrivi alla fine"):** `tempo_limite_s` di ogni livello = circa 1.45 volte il
  tempo del bot pulito del gate gara (lv_01 50, lv_02 58, lv_03 59, lv_04 48,
  lv_05 61, lv_06 47; lv_02 e lv_05 ricalcolati l'08/11 dopo la galleria); al cambio di tappa si aggiunge l'80%
  (`tempo_extra_fattore` 0.8 in config.json). Guida pulita: arrivo con ~25 s
  di margine; un incidente per tappa: si arriva per poco; solo acceleratore:
  il tempo finisce alla 2a o 3a tappa. Se si allunga o accorcia un livello,
  ricalcolare il suo `tempo_limite_s` con la stessa regola; non alzarli a caso.
- **Fuori strada:** rallenta, l'auto trema sul terreno; rientrando
  sull'asfalto un piccolo sobbalzo (`RIENTRO_S = 0.35`). Incidente solo dopo
  2 s fuori strada **e** sopra il 30% della punta (`OFFROAD_INCIDENTE_V`);
  piano nell'erba rallenta e basta. Una botta nell'erba (cartellone) che fa
  scendere sotto il 30% non azzera il conto, lo fa calare (11/11): prima un
  cartellone ogni mezzo secondo bastava a non fare mai l'incidente. Il ciglio resta: primo tocco rallenta e
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

## Rivista da Andrea il 18/11 (curva, bordi, sterzo, pose)
- **Centrifuga** (game.cpp `CENTRIFUGA_*`): "se mollo lo sterzo in velocita' devo
  andare fuori piu' facilmente; nelle curve ampie si deve sentire la centrifuga".
  Oltre `CENTRIFUGA_DA = 0.20` di carico (|curva| x v^2), con lo sterzo che non
  tiene la curva (sotto `CENTRIFUGA_TIENE = 0.4` dentro), la spinta verso
  l'esterno cresce fino al doppio (`CENTRIFUGA_EXTRA = 1.0`) in 0.35 s; fumo,
  stridio, coda fuori e muso girato dentro (posa di mezzo). Al bordo con
  centrifuga >= 0.3 e sopra il 55% della punta: incidente. Prove (sterzo mollato
  alla punta): curve 0.6 incidente in ~0.9 s quasi sempre, curve 0.45 circa due
  volte su tre in 1-1.5 s, curve 0.3 di rado.
- **Ciglio**: al primo tocco l'auto rimbalza verso la strada (`CIGLIO_RIMBALZO = 380`
  unita'/s) con un sobbalzo, invece di strisciare nell'erba.
- **Sterzo** (player.h, documento "Esperienza di guida di riferimento", proposte 2
  e 3): volante pieno in 0.06 s (`STEER_RAMPA_PER_S = 16`, era 8), ritorno al
  centro in 0.12 s (`STEER_RITORNO_PER_S = 8`, era 12: ora piu' lento della
  pressione) e risposta non lineare (`STEER_MORBIDO = 0.35`: un colpetto sposta
  poco, pieno resta pieno).
- **Pareggio** (proposte 1 e 4): ogni curva ha la sua velocita' giusta, quella a
  cui lo sterzo pieno (`Player::sterzoPieno`) pareggia la spinta
  (`CURVA_SPINTA_V2` x |curva| x v^2). Dall'85% del pareggio, a sterzo pieno
  dentro, stridio leggero d'avviso (`PAREGGIO_AVVISO`, `PAREGGIO_STRIDIO`); oltre,
  centrifuga anche a sterzo pieno (piena al +30%, `PAREGGIO_MARGINE`). Col gas il
  sovrasterzo resta come prima.
- **Fuori strada a due zone** (proposta 5, player.h `ERBA_*`): nell'erba vicino al
  bordo la velocita' scende verso il 72% della punta (~210 km/h) a 100
  unita'/s^2; oltre 100 unita' piu' in fuori verso il 30% (~88 km/h) a 150; sotto
  il tetto l'erba frena solo un poco. Prima: -40% al secondo ovunque.
- **Pose di mezzo**: la posa mostrata passa per `auto_sx2`/`auto_dx2` (24 livelli
  al secondo, ritorno al doppio, `POSA_LIVELLI_PER_S`): prima lo sterzo digitale
  saltava da dritta a sx4 in 2 frame.
- **Paletti**: si abbattono all'altezza dell'auto (`PALO_AUTO_Z = 900` davanti alla
  telecamera) e quello in piedi sparisce: prima il volo partiva gia' dietro lo
  schermo e non si vedeva.

## Aggiunto il 25/11 (volante che torna al centro, blocco nuovo)
- **Sterzo analogico** (player.h `ANALOGICO_*`): con uno stick o un volante USB
  il volante segue la posizione in proporzione (zona morta 15%, pieno al 90%
  della corsa) e torna al centro quando lo stick si raddrizza, al passo del
  ritorno (`STEER_RITORNO_PER_S`). Prima lo stick sterzava solo a fondo oltre
  meta' corsa. Tastiera e croce restano identiche (stesse rampe 0.06 / 0.12 s).

## Riferimento di guida (studio del 18/11, Andrea 27/11: resta come metro)
Riassunto dello studio "Esperienza di guida di riferimento" fatto in sola lettura sul gioco arcade
del 1991. **E' un metro per la fisicita' della guida, non un modello da copiare**: niente codice,
dati o grafica di altri giochi nel repo, solo comportamenti riscritti a modo nostro. Chi tocca guida,
rivali, urti o tempi controlla prima che il risultato resti coerente con questi punti:
- **Pareggio in curva**: per ogni curva c'e' una velocita' a cui lo sterzo pieno tiene la linea;
  piu' veloce scivoli fuori, piu' lento tagli dentro. Gia' nel gioco (sezione 18/11).
- **Volante**: pressione rapida, ritorno al centro piu' lento; tocchi brevi correggono poco, sterzo
  pieno deciso. Gia' nel gioco (0.06 / 0.12 s, `STEER_MORBIDO`).
- **Presa**: sopra circa il 40% della punta la presa laterale non cala, nemmeno alla punta.
- **Fuori strada a due zone** con tetti di velocita' (circa 72% e 30% della punta). Gia' nel gioco.
- **Stridio** solo come avviso vicino al limite, non come freno.
- **Urti col traffico** (non ancora applicato, solo se Andrea lo chiede): l'auto rallenta e sbanda,
  e la sbandata si chiude col controsterzo. **Oggetti a bordo strada** per velocita': piano ci si
  ferma, a media velocita' si scivola o si fa incidente, forte e' incidente grosso.
- Non applicati e da non fare senza Andrea: cambio manuale, esiti degli urti a soglie di velocita'.

## Aggiunto il 27/11 (stop accesi in gara)
- **Stop accesi** (game.cpp, `player_.freno()`): col freno premuto (B o Giu') gli stop dell'auto si
  accendono anche in gara, non solo dopo il traguardo. Spenti durante l'incidente. Disegno invariato.
