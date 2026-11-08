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

## Stili per livello (richiesta di Andrea - aggiornata al 23 Oct)
Ogni livello ha UNO stile, e lo dichiara in livello.json (`stile`):
| ID livello | Note creative |
|---|---|

| lv_01_neon | USA, Neon Lights, Dark Buildings, Heroin, shining metalli, futuristic, romantic, gradients, colori: pastello |

| lv_02_atomico | robots era, computers, hi-tech, Internet Servers, wiring cables, cromature, atomic age, high buildings, steampunk, stars & pianets, engines, colori: toni grigi e accenti rubin red |

| lv_03_foglie | plants, foglie di canapa, palm trees, sea side, beach, Birre, Jeans, Rasta culture, Fumo, colori: Jamaica inspired, with green and brown and blue sky |

| lv_04_cromo | sunset, Ganja, Visual grids, neon lights, Pin usp, videogame culture, toni verdi-blue, grunge texture, industrial, London style, colori accesi su sfondi scuri |

| lv_05_spray | Urban, London, Public transport, busy City landscape, donne nude, cloudy, fog, industrial, delfini, colori: bevel grigio, red, blue, white |

| lv_06_desktop | Blue sky, green fields surroundings, wild animals, Nature, birds, system icons, cables, motheboard circuits, Internet Culture, AI agents, colori: rosa shoking, azzurro, lime green, pink |

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

## Temi dei livelli nel gioco (da `motore_nuovo/dati/livelli/*/livello.json`)
| Livello | Tema | Stile |
|---|---|---|
| lv_01_neon | autostrada di neon di notte: griglia di luce, skyline scuro e sole a righe all'orizzonte | synthwave / C-wave |
| lv_02_atomico | strada atomica anni '50: razzi di latta, UFO di stagno e colori pastello | sci-fi anni '50 |
| lv_03_foglie | viale delle foglie: colori che cambiano e fumi stilizzati, parodia dentro il messaggio di sicurezza stradale | psichedelico / cannabinoidi |
| lv_04_cromo | tangenziale sopraelevata tra megastrutture cromate: curvoni lunghi e veloci, una grande discesa | futurista |
| lv_05_spray | periferia industriale con muri a spray: incroci a 90 gradi, rettilinei corti, frenate | punk |
| lv_06_desktop | prato verde e cielo con nuvole da sfondo del desktop: colline continue, saliscendi e curve medie | Windows 95 vibe |

## Regole grafiche fisse (Andrea, ottobre-novembre)
- Pixel art anni '80 con animazioni moderne; palette piccola e condivisa, dithering (Bayer, Atkinson, halftone) al posto di altri colori.
- Visualizzatore solo sopra la linea dell'orizzonte; niente esplosioni o scintille nello spettro.
- L'auto del giocatore la disegna solo Andrea (pose in `sprite/giocatore_pose/`).
- Interfaccia e testi a schermo in inglese semplice.
