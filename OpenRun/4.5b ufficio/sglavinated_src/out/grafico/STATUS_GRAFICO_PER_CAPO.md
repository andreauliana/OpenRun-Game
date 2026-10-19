# Status grafico per il Capo — OpenRun 4.6

**Aggiornato:** 2026-10-18  
**Stato:** INVENTARIO VERIFICATO; asset 4.6 NON ancora migrati; review indipendente richiesta.  
**Checkpoint:** [CP-002 — inventario grafico](../../Checkpoint%20Revision/CP-002-inventario-grafico.md)  
**Inventario:** [CATALOGO_RISORSE.md](../../UFFICIO/Dipartimento%20Grafico/Libreria/CATALOGO_RISORSE.md)

Questo è l'unico handoff corrente del Designer per il Capo. Aggiornarlo in place a ogni task grafico; non creare riepiloghi di stato paralleli.

## Sintesi per review/checkpoint

- **Destinazione Libreria 4.6:** non contiene ancora gli asset di produzione. Gli asset attivi restano in `assets_originali/` e i dati sorgente in `motore_nuovo/dati/`; `Shared asset/` contiene solo README.
- **Catalogo:** 52 voci: 30 approvate, 16 pronte, 2 in costruzione, 4 mancanti. Il check ha segnalato 100 PNG non catalogati da classificare, senza promuoverli.
- **Temi/livelli:** presenti 3 livelli, Neon, Atomico e Foglie. Sei percorsi distinti/selezionabili: non trovati.
- **Auto principale:** 3 sprite originali 64x32 approvati (`auto_argento_lunare`, `auto_neon_cyan`, `auto_rosso_corsa`); non sono modelli 3D. **Concorrenti:** sprite `auto.traffico` mancante.
- **Sponsor:** 21 banner approvati: 7 sponsor (`brinis_udine`, `canna_bis`, `curve_pericolose`, `erba_medica`, `green_box`, `piston_high`, `slow_down`) x 3 temi (Neon, Atomico, Foglie).
- **Fumetti:** TESTI COMPLETI: Gianni 97 + Gesù 59 = 156. Fonte di riferimento: [SPEC_personaggi_v74.md](../../SPEC_personaggi_v74.md), confermata da Andrea. Grafica dei personaggi/balloon da verificare separatamente.
- **Personaggi grafici:** Gianni, Gesù e UFO risultano mancanti nel catalogo come sprite/pose approvati.
- **Finali:** non risultano i 3 finali animati richiesti tra gli asset attivi.
- **Elementi livelli:** 16 oggetti di bordo `pronto` (Neon 5, Atomico 6, Foglie 5) e 3 skyline `approvato`.
- **UI/palette:** titolo e font variante B approvati; preview HUD/menu in costruzione. Palette presenti per varianti anni '50/futurista/punk, non tre palette definitive associate ai temi livello.

## Tool disponibili al Designer

Le sorgenti restano nei percorsi attuali, elencati in [TOOLS/README.md](../../UFFICIO/Dipartimento%20Grafico/TOOLS/README.md). Sono autorizzati il check catalogo, l'indice asset, la galleria PNG e la conversione sponsor PNG->raw; i generatori in `assets_originali/` erano già nel perimetro. In questo audit è stato eseguito solo il check catalogo.

## Prove e richieste al Capo

- `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` dalla radice `sglavinated_src/`: **exit 0**, 52 elementi; 100 PNG non catalogati segnalati come varianti/doppioni.
- Non sono stati spostati o generati asset; nessuna licenza è stata promossa o modificata.
- **Azione Capo:** assegnare un Builder/Developer o Developer/Builder Junior diverso dall'autore per la review dell'inventario e dei riferimenti; poi registrare esito e prossima priorità in CP-002.
- **Azione futura:** pianificare con responsabile e criteri la migrazione fisica della libreria e le priorità per sei percorsi, finali animati, sprite dei concorrenti/personaggi e classificazione dei PNG non catalogati. Non considerarli completati sulla base di questo report.
