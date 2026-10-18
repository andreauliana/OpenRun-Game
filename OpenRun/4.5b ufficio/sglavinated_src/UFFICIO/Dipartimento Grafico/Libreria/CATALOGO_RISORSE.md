# Inventario risorse grafiche — OpenRun 4.6

Inventario sintetico basato sul catalogo macchina `../../../assets_originali/CATALOGO_GRAFICO.json`, sui file presenti e sui dati attivi. Le cartelle indicate sono i percorsi sorgente correnti, non copie nella Libreria 4.6.

## Stato della destinazione Libreria

`UFFICIO/Dipartimento Grafico/Libreria/` contiene al momento solo documentazione; `Shared asset/` non contiene asset pubblicati. Gli asset elencati sotto sono disponibili nei percorsi v4.5 attivi indicati in ogni sezione. La migrazione fisica è sospesa fino all'aggiornamento coordinato dei riferimenti di produzione.

Il controllo `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py`, eseguito dalla radice `sglavinated_src/`, ha riportato (18 ott, dopo le 6 piste): **58 elementi catalogati** (30 approvati, 17 pronti, 2 in costruzione, 9 mancanti) e **100 PNG non catalogati**, segnalati come varianti o doppioni da valutare.

## Temi e livelli

Le piste sono sei, collegate da bivi come in OutRun (albero in `../../../motore_nuovo/dati/percorso_gara.json`): lv_01 alla partenza, poi lv_02 o lv_03, poi una fra lv_04, lv_05 e lv_06 come arrivo. Ogni `livello.json` dichiara in `tema_grafico` la skyline, gli oggetti di bordo strada (id del catalogo) e lo stile dei cartelloni, e in `colori` cielo, erba e cordoli.

| Pista | Stile | Skyline (id catalogo) | Oggetti bordo strada | Cartelloni |
|---|---|---|---|---|
| lv_01_neon | synthwave | `scenario.lv_01_neon.skyline` (approvato) | 5 propri (pronti) | neon |
| lv_02_atomico | sci-fi anni '50 | `scenario.lv_02_atomico.skyline` (approvato) | 6 propri (pronti) | atomico |
| lv_03_foglie | psichedelico | `scenario.lv_03_foglie.skyline` (approvato) | 5 propri (pronti) | foglie |
| lv_04_cromo | futurista | `skyline.futurista` (pronto, da approvare) | in prestito: lampione, palo neon, torre razzo, cupola | neon |
| lv_05_spray | punk | `scenario.lv_05_spray.skyline` (mancante) | in prestito: bidone, cassetta, freccia, segnale atomico | atomico |
| lv_06_desktop | Windows 95 vibe | `scenario.lv_06_desktop.skyline` (mancante) | in prestito: palina, panchina cromata, panchina curva | foglie |

Per lv_04, lv_05 e lv_06 gli oggetti propri sono segnati `mancante` nel catalogo (`scenario.<pista>.oggetti`): finché il Designer non li disegna si usano quelli in prestito, scelti per somiglianza di tema. Il motore oggi usa i colori e lo stile dei cartelloni; skyline e oggetti di bordo strada non sono ancora disegnati in gioco.

Elementi di bordo (tutti nello stato `pronto`, non `approvato`):

- **Neon:** bidone, cassetta, lampione, palina, palo neon.
- **Atomico:** bancone drive-in, cupola, freccia, panchina cromata, segnale atomico, torre razzo.
- **Foglie:** foglia gigante, fungo psico, insegna fumosa, panchina curva, panchina fumo.

Percorsi delle skyline approvate:

- `../../../assets_originali/scenari/lv_01_neon/scenario_neon_01_varianteA/skyline.png`
- `../../../assets_originali/scenari/lv_02_atomico/scenario_atomico_02_varianteA/skyline.png`
- `../../../assets_originali/scenari/lv_03_foglie/scenario_foglie_03_varianteA/skyline.png`

## Auto e concorrenti

- **Auto principale / sprite giocatore:** tre file 64x32, catalogati `approvato`: `../../../assets_originali/sprite/auto_gt/auto_argento_lunare.png`, `auto_neon_cyan.png`, `auto_rosso_corsa.png`. Sono sprite originali a sagoma/visuale arcade; il catalogo non li classifica come modelli 3D.
- **Varianti auto a tema:** `../../../assets_originali/sprite/auto/varianteA_anni50/auto.png`, `varianteB_futurista/auto.png` e `varianteC_punk/auto.png`; verificare catalogo e stato prima di usarle come alternative approvate.
- **Concorrenti / sagome avversarie:** in gara corrono 7 avversari, disegnati per ora come sagome colorate. Non c'è uno sprite di traffico approvato. `auto.traffico` risulta `mancante`; i modelli identici alla macchina principale menzionati in `VISIONE.md` sono una direzione di progetto, non una raccolta asset completa.

## Cartelloni e banner sponsor

Il catalogo contiene **21 cartelloni approvati**: 7 sponsor per 3 temi. Ogni cartellone è in `../../../assets_originali/sponsor/<sponsor>/cartellone_<tema>.png`; il corrispondente raw generato è `../../../motore_nuovo/dati/sprite_raw/sponsor/<sponsor>_<tema>.sglraw`.

| Cartella sponsor | Neon | Atomico | Foglie |
|---|---|---|---|
| `brinis_udine` | `cartellone_neon.png` | `cartellone_atomico.png` | `cartellone_foglie.png` |
| `canna_bis` | `cartellone_neon.png` | `cartellone_atomico.png` | `cartellone_foglie.png` |
| `curve_pericolose` | `cartellone_neon.png` | `cartellone_atomico.png` | `cartellone_foglie.png` |
| `erba_medica` | `cartellone_neon.png` | `cartellone_atomico.png` | `cartellone_foglie.png` |
| `green_box` | `cartellone_neon.png` | `cartellone_atomico.png` | `cartellone_foglie.png` |
| `piston_high` | `cartellone_neon.png` | `cartellone_atomico.png` | `cartellone_foglie.png` |
| `slow_down` | `cartellone_neon.png` | `cartellone_atomico.png` | `cartellone_foglie.png` |

Indice fonti: `../../../assets_originali/sponsor/CATALOGO_SPONSOR.json` e `../../../assets_originali/MAPPA_LIVELLI_SPONSOR.json`.

## Fumetti e personaggi

- **Testi dei fumetti:** COMPLETI secondo lo stato fornito da Andrea. Il foglio `../../../SPEC_personaggi_v74.md` è la specifica di riferimento; i testi sono presenti nei JSON attivi: `../../../motore_nuovo/dati/personaggi/gianni/fumetti.json` contiene 97 frasi e `../../../motore_nuovo/dati/personaggi/gesu/fumetti.json` 59, per 156 voci complessive.
- **Grafica dei fumetti/personaggi:** i testi completi non significano che siano pronti sprite, pose o balloon approvati; gli ID personaggio restano `mancante` nel catalogo grafico.
- **Grafica/personaggi:** gli ID `personaggio.gianni`, `personaggio.gesu` e `personaggio.ufo` sono segnati `mancante` nel catalogo grafico. I manifest o i testi non equivalgono a sprite/pose approvati.
- **UFO di scenario:** alcune varianti Atomico contengono oggetti di distanza; non sono un set di animazioni del personaggio UFO.

## Finali, animazioni e UI

- **Sei percorsi:** presenti dal 18 ott (vedi "Temi e livelli"); le tre piste nuove hanno solo il percorso, la grafica propria è da fare.
- **Tre finali con animazioni:** non risultano asset, sequenze o manifest di finale nel catalogo e nelle cartelle grafiche attive. Non confondere il file storico della hack con asset originali 4.6.
- **UI:** titolo e font variante B risultano approvati; le anteprime di HUD e menu variante B sono `in_costruzione` e non vanno pubblicate come asset definitivi.
- **Palette:** sono presenti tre varianti del livello 1 (`anni50`, `futurista`, `punk`); non sono registrate come palette specifiche approvate per i tre temi Neon, Atomico e Foglie.

## Provenienza e aggiornamento

Il catalogo `../../../assets_originali/CATALOGO_GRAFICO.json` è la fonte per ID, stato, PNG e raw; `../../../assets_originali/CREDITI.md` e i manifest sono la fonte per licenze/provenienza. Prima di spostare o pubblicare una risorsa, il Designer verifica file e licenza, aggiorna catalogo/manifest e ottiene lo stato di approvazione. Non promuovere automaticamente i 100 PNG non catalogati.
