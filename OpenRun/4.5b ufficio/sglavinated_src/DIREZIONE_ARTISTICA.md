# Direzione artistica — OpenRun (documento di Andrea, spostato qui l'08/11 per trovarlo subito)

## Mood
Sci-fi, lo-fi, hip hop, jungle, urban, Pixel-Art, fast-felt-modern retro-game, pop culture, dreamy, alterated, psychedelic, Cannabis.
Inspirations: 'OutRun', 2YK, Road Race, Doom Engine, GTA, City-Pop, London, Japan, India, Italy.

## Regole
| Cosa | Regola |
|---|---|
| Stile | ispirarsi al genere e atmosfera, OutRun, Road Race, GTA, Hexen, Heretic, modernised retro-game |
| Personaggi | Gianni, Gesu', UFO. Restano e verranno inseriti da Andrea, da importare nel asset C++ |
| Palette | max 16 colori per sprite; gradienti, colori con toni vivi e accesi su fondali tendenzialmente piu scuri (a parte eccezzioni dovute al tema del livello), palette definite internamente |
| Scenari | 1) Neon, 2) Atomico, 3) Foglie, 4) Cromo, 5) Spray, 6) Desktop |
| Tracciati percorsi stradali | generati da zero (seed), un tema per percorso |
| Musica | tracker .xm file di Andrea: (in arrivo) |
| Nomi/loghi | inventati; nessun nome o marchio del gioco originale |

## Riferimento di metodo (non di contenuto)
Esistono progetti che rifanno un gioco con contenuti liberi (es. Freedoom per Doom): simile concetto per questo progetto di Gioco compilato in C++, ma con parti innovative e animazioni divertenti, una versione rivisitata del videogioco di riferimento 'OutRun'.

## Stili / temi dei livelli (da `motore_nuovo/dati/livelli/*/livello.json`)
Ogni livello ha uno stile, e lo dichiara in livello.json (`stile`):
| ID livello | Note creative |
|---|---|
| lv_01_neon | USA, Neon Lights, Dark Buildings, Heroin, shining metalli, futuristic, romantic, gradients, colori: pastello |
| lv_02_atomico | robots era, computers, hi-tech, Internet Servers, wiring cables, cromature, atomic age, high buildings, steampunk, stars & pianets, engines, colori: toni grigi e accenti rubin red |
| lv_03_foglie | plants, foglie di canapa, palm trees, sea side, beach, Birre, Jeans, Rasta culture, Fumo, colori: Jamaica inspired, with green and brown and blue sky |
| lv_04_cromo | sunset, Ganja, Visual grids, neon lights, Pin ups, videogame culture, toni verdi-blue, grunge texture, industrial, colori brillanti su sfondo scuro notturno |
| lv_05_spray | Urban Megalopoli, London inspired, Public Transport (tube or trains), Busy life in a big City with many different random people and cultures, donne nude ai lati della strada, cloudy, fog, industrial, delfini, colori: bevel grigio, red, blue, white |
| lv_06_desktop | Blue sky, green fields surroundings, wild animals, Nature, birds, folder symbols and operating system icons, long dark and coloured connecting cables, motheboard circuits, Internet Culture, meme, colori: rosa shoking, azzurro, lime green, pink |

## Sponsor finti (cartelloni lungo la strada)
Tutti inventati, mai marchi veri. Grafica in `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/sponsor/` (generatore `genera_sponsor.py`, catalogo `CATALOGO_SPONSOR.json`), una versione per stile (neon, atomico, foglie).

| Sponsor | Settore | Slogan |
|---|---|---|
| CURVE PERICOLOSE | intimo / abbigliamento | appoggiati al peggio |
| BRINIS UDINE | ricambi motorini / officina | Pezzi caldi per motorini che non si fermano mai! |
| PISTON HIGH | motoristico / gioco di parole | Erba del Pilota |
| CANNA-BIS | ironico / velocita | L'unico sponsor che ti fa andare forte... rimanendo piantato. |
| SLOW DOWN | ricambi auto / freni | Pastiglie per freni molto rilassati |
| ERBA MEDICA | ammortizzatori / doppio senso | Ammortizzatori ad alta quota |
| GREEN BOX | pit stop / servizi | Il Pit Stop della felicita |


## Regole grafiche fisse (Andrea, ottobre-novembre)
- Pixel art, acid graphic, con animazioni moderne; palette definita e condivisa, dithering: Floyd-Steinberg, Atkinson, Error Diffusion, Dot Raster, Bayer al posto di colori aggiuntivi.
- Visualizzatore solo sopra la linea dell'orizzonte; niente esplosioni o scintille nello spettro.
- L'auto del giocatore la disegna solo Andrea (pose in `sprite/giocatore_pose/`).
- Interfaccia e testi a schermo in inglese semplice.
