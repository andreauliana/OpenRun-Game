# Status grafico per il Capo — OpenRun 4.6

**Aggiornato:** 2026-10-23 10:41  
**Stato:** G1 [x] check_catalogo_grafico.py eseguito in turno 22/10/2026 21:33: exit 0, 159 elementi (approvato 30, pronto 19, in_costruzione 0, mancante 9, scartato 101), PNG non catalogati: 0; nessun asset modificato. Errore residuo sanato: elemento scartato `obj_insegna_fumosa` lv_03_foglie variante B ora ha `file: []` (file inesistente, storico conservato). M [~] UI/HUD D8 "tempo" in hud.json variante B (risposta A); produzione completa: 4 PNG variante B rigenerati in bianco e nero (font.png, title.png, menu_preview.png, hud_preview.png con elemento "tempo"), audit OK 4 colori acromatici; catalogo ui.hud e ui.menu passate a "pronto" (verifica exit 0, PNG non catalogati 0); CREDITI.md aggiornato con i 4 asset UI (CC-BY-4.0); manifest resta "bozza" in attesa ok Andrea PNG B/N, nessun blocco in attesa (D8). Gate T1 non eseguito (compito sviluppatore).  
**Checkpoint:** [CP-002 — inventario grafico](../../Checkpoint%20Revision/CP-002-inventario-grafico.md)  
**Inventario:** [CATALOGO_RISORSE.md](../../UFFICIO/Dipartimento%20Grafico/Libreria/CATALOGO_RISORSE.md)

Turno 23/10/2026 10:41: nessun asset creato/modificato (M [~] in attesa ok Andrea PNG B/N manifest bozza; DECO1 [ ] punto 1 non eseguibile senza asset grafici PNG da disegnare; G1 [x] chiuso). Questo è l'unico handoff corrente del Designer per il Capo. Aggiornarlo in place a ogni task grafico; non creare riepiloghi di stato paralleli.

## Sintesi per review/checkpoint

- **Destinazione Libreria 4.6:** non contiene ancora gli asset di produzione. Gli asset attivi restano in `assets_originali/` e i dati sorgente in `motore_nuovo/dati/`; `Shared asset/` contiene solo README.
- **Catalogo:** 159 voci (58 originali + 101 `scartato`): 30 approvate, 17 pronte, 2 in costruzione, 9 mancanti, 101 scartato. I PNG fuori elenco (varianti B/C, scenari doppi e doppioni oggetto) sono classificati `scartato`, file conservati, non cancellati; atteso `PNG non catalogati: 0` (raggiunto). Errore residuo sanato (exit 0): l'unico riferimento scartato a file inesistente (`obj_insegna_fumosa`, lv_03_foglie variante B) ha ora `file: []`.
- **Temi/livelli:** presenti 3 livelli, Neon, Atomico e Foglie. Sei percorsi distinti/selezionabili: non trovati.
- **Auto principale:** 3 sprite originali 64x32 approvati (`auto_argento_lunare`, `auto_neon_cyan`, `auto_rosso_corsa`); non sono modelli 3D. **Concorrenti:** sprite `auto.traffico` mancante.
- **Sponsor:** 21 banner approvati: 7 sponsor x 3 temi (Neon, Atomico, Foglie).
- **Fumetti:** TESTI COMPLETI: Gianni 97 + Gesù 59 = 156. Fonte di riferimento: SPEC_personaggi_v74.md, confermata da Andrea. Grafica dei personaggi/balloon da verificare separatamente.
- **Personaggi grafici:** Gianni, Gesù e UFO risultano mancanti nel catalogo come sprite/pose approvati.
- **Finali:** non risultano i 3 finali animati richiesti tra gli asset attivi.
- **Elementi livelli:** 16 oggetti di bordo `pronto` (Neon 5, Atomico 6, Foglie 5) e 3 skyline `approvato`.
- **UI/palette:** titolo e font variante B approvati; preview HUD/menu in costruzione. Palette presenti per varianti anni '50/futurista/punk, non tre palette definitive associate ai temi livello.

## Tool disponibili al Designer

Le sorgenti restano nei percorsi attuali, elencati in [TOOLS/README.md](../../UFFICIO/Dipartimento%20Grafico/TOOLS/README.md). Sono autorizzati il check catalogo, l'indice asset, la galleria PNG e la conversione sponsor PNG->raw; i generatori in `assets_originali/` erano già nel perimetro. In questo turno è stato eseguito solo il check catalogo.

## Prove e richieste al Capo

- `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` dalla radice `sglavinated_src/`: eseguito 2026-10-22 10:56, **exit 0** (159 elementi, PNG non catalogati: 0). Errore precedente (`obj_insegna_fumosa` variante B, file inesistente) sanato con `file: []`. Ultimo esito noto prima di oggi: exit 0, 52 elementi, 100 PNG non catalogati.
- Non sono stati spostati o generati asset; nessuna licenza è stata promossa o modificata.
- **Azione Capo:** assegnare un reviewer diverso dall'autore per l'inventario e i riferimenti; poi registrare esito e prossima priorità in CP-002.
- **Azione futura:** pianificare con responsabile e criteri la migrazione fisica della libreria e le priorità per sei percorsi, finali animati, sprite dei concorrenti/personaggi e classificazione dei PNG non catalogati. Non considerarli completati sulla base di questo report.
