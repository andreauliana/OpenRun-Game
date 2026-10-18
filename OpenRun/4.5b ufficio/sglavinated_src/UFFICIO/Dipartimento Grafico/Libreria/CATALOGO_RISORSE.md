# Inventario risorse grafiche — OpenRun 4.6

Inventario sintetico basato sul catalogo macchina `../../../assets_originali/CATALOGO_GRAFICO.json`, sui file presenti e sui dati attivi. Le cartelle indicate sono i percorsi sorgente correnti, non copie nella Libreria 4.6.

## Stato della destinazione Libreria

`UFFICIO/Dipartimento Grafico/Libreria/` contiene al momento solo documentazione; `Shared asset/` non contiene asset pubblicati. Gli asset elencati sotto sono disponibili nei percorsi v4.5 attivi indicati in ogni sezione. La migrazione fisica è sospesa fino all'aggiornamento coordinato dei riferimenti di produzione.

Il controllo `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py`, eseguito dalla radice `sglavinated_src/`, ha riportato: **52 elementi catalogati** (30 approvati, 16 pronti, 2 in costruzione, 4 mancanti) e **100 PNG non catalogati**, segnalati come varianti o doppioni da valutare.

## Temi e livelli

I dati di livello attivi sono tre, non sei percorsi distinti:

| Tema | Dati attivi | Scenario grafico selezionato | Elementi del bordo catalogati |
|---|---|---|---:|
| Neon | `../../../motore_nuovo/dati/livelli/lv_01_neon/livello.json` | `../../../assets_originali/scenari/lv_01_neon/scenario_neon_01_varianteA/` | 5 pronti |
| Atomico | `../../../motore_nuovo/dati/livelli/lv_02_atomico/livello.json` | `../../../assets_originali/scenari/lv_02_atomico/scenario_atomico_02_varianteA/` | 6 pronti |
| Foglie | `../../../motore_nuovo/dati/livelli/lv_03_foglie/livello.json` | `../../../assets_originali/scenari/lv_03_foglie/scenario_foglie_03_varianteA/` | 5 pronti |

Ogni livello ha un file dati con 20 segmenti stradali e un PNG skyline approvato. Le varianti A/B/C presenti nelle cartelle scenario non equivalgono a sei percorsi di gioco: il catalogo non definisce sei tracciati selezionabili o ramificazioni corrispondenti.

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
- **Concorrenti / sagome avversarie:** non c'è uno sprite di traffico approvato. `auto.traffico` risulta `mancante`; i modelli identici alla macchina principale menzionati in `VISIONE.md` sono una direzione di progetto, non una raccolta asset completa.

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

- **Sei percorsi:** non presenti come sei definizioni/varianti di percorso selezionabili. Sono presenti solo i tre livelli elencati sopra.
- **Tre finali con animazioni:** non risultano asset, sequenze o manifest di finale nel catalogo e nelle cartelle grafiche attive. Non confondere il file storico della hack con asset originali 4.6.
- **UI:** titolo e font variante B risultano approvati; le anteprime di HUD e menu variante B sono `in_costruzione` e non vanno pubblicate come asset definitivi.
- **Palette:** sono presenti tre varianti del livello 1 (`anni50`, `futurista`, `punk`); non sono registrate come palette specifiche approvate per i tre temi Neon, Atomico e Foglie.

## Provenienza e aggiornamento

Il catalogo `../../../assets_originali/CATALOGO_GRAFICO.json` è la fonte per ID, stato, PNG e raw; `../../../assets_originali/CREDITI.md` e i manifest sono la fonte per licenze/provenienza. Prima di spostare o pubblicare una risorsa, il Designer verifica file e licenza, aggiorna catalogo/manifest e ottiene lo stato di approvazione. Non promuovere automaticamente i 100 PNG non catalogati.
