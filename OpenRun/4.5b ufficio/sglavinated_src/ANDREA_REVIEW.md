# Andrea — Feedback, decisioni e Review

Fonte canonica unica per feedback e decisioni di Andrea. Il @capo cita nel checkpoint soltanto gli ID e le direttive pertinenti; log, status di reparto e risultati di review restano nei rispettivi documenti. Solo una risposta esplicita di Andrea vale come sua approvazione.

## AUTO DEL GIOCATORE — REGOLE BLOCCATE (Andrea 06/11)

Le regole dell'auto del giocatore (grafica, grandezza, posizione, sollevamento, salti, inclinazione, arrivo) sono in `REGOLE_AUTO_GIOCATORE.md`. Nessun agente le modifica: niente cambi a quei valori, a quel codice o ai file in `giocatore_pose/`. Andrea ridisegna l'auto a mano in Pixelorama.

## HUD NON SI TOCCA, SI LAVORA SUL PERCORSO (Andrea 08/11)

Per adesso l'HUD non va toccato da nessuno: niente modifiche a `dati/ui/hud.json`, `Renderer::drawHudDati`, contagiri, spettro radio, ne' alle varianti UI in `assets_originali/ui/`. Gli agenti lavorano sul percorso (tracciati, curve, salite, bivi, strada, traffico e rivali sul percorso) e su tutto il resto, sempre a passi piccoli con ctest verde. Restano fuori: l'auto del giocatore (la disegna Andrea), premiazione e musica degli urti (bloccate qui sotto). **Parapetti sul ponte: P3 annullato per sempre** (Andrea 06/11, confermato 08/11): il ponte resta aperto, con il filo basso grigio/bianco al posto del rumble; niente `parapetto.png`/`guardrail.png` sulla pista. Il lavoro e' nel BLOCCO PERC1 di RICHIESTA.md.

**ANNULLATO da Claude 08/11 alle 11:40 (turno delle 11:32 fatto su una copia vecchia di RICHIESTA):** curve tolte e rampa/discesa del ponte tolte in lv_01, lv_03, lv_04, lv_05, lv_06 (facevano FALLIRE 4 gate percorso); `colpiMemoria_` azzerato per i rivali in `Game::urto` e `bumpRallentaFattore_` 0.80 (regole bloccate in `REGOLE_AUTO_GIOCATORE.md`, Andrea non l'ha chiesto); TIME spostato a sinistra e `colore_bordo` in `hud.json`; anteprime della variante UI B; cartella `help/` con log e PPM dei test. **Tracciati: si AGGIUNGONO tratti solo con i passi PERC1 (D1, D4); mai togliere curve, mai togliere `rampa_ponte`/`discesa_ponte`, mai cambiare curve per far passare un gate (i 6 gate percorso passano gia').** I log e gli screenshot dei test vanno solo in `UFFICIO/Officina/Test area/`.

**Guida e tempi (Andrea 08/11, Claude):** le curve spingono verso l'esterno e si sterza DENTRO la curva; sterzo un po' piu' forte alla punta e pieno verso un'auto affiancata (sportellata); i `tempo_limite_s` dei livelli sono stati stretti (circa 1.45 volte il tempo del bot pulito) perche' col solo acceleratore si arrivava in fondo. Regole e numeri in `REGOLE_AUTO_GIOCATORE.md`: nessun agente alza i tempi o cambia sterzo/spinta delle curve. **08/11 pomeriggio (prova di Andrea):** sterzo piu' forte alla punta e derapata nelle curve forti (presa, fumo, stridio delle gomme). Anche questi bloccati: nessun agente cambia `STEER_*`, `CURVA_SPINTA_V2`, `DERAPATA_*`.

**Gate gara FALLITO dopo PERC1 D2 (Claude 08/11 alle 11:45):** col traffico per tratto, in lv_05 due rivali arrivati da rami diversi si disegnavano uno sopra l'altro (96 sovrapposizioni, massimo 20). Causa vecchia: la distanza fra rivali usava il progresso totale invece di quello dentro la tappa. Sistemato da Claude in `Game::aggiornaRivali` (`progressoTappa`); D2 resta [x]. Nessun agente alza il limite delle sovrapposizioni del gate.

**Galleria PERC1 D4 rifatta da Claude (08/11 alle 13:45):** sul main la galleria di lv_02 e lv_05 si vedeva come uno spazio piatto di un solo colore (soffitto disegnato sopra la strada, cielo visibile; Andrea: "sembra un'area malvagia"). Ora ha pareti, soffitto a fasce, luci ogni 10 segmenti, imbocco e uscita, e fuori dall'uscita si vede solo la bocca. Codice in `Renderer::drawRoad`/`drawSegmentTrapezoid`/`drawSprite` (`GALLERIA_LARGHEZZA`, `GALLERIA_ALTEZZA`): nessun agente lo ritocca finche' Andrea non l'ha vista. Il gate tratti falliva per la curva dopo la galleria scritta come oggetto in linea: ora e' il tratto con nome `uscita_galleria` in `tratti.json` (in `tracciato` solo nomi di tratti, mai oggetti in linea). lv_02 e lv_05 sono piu' lunghi: `tempo_limite_s` 58 e 61. `REVIEW_PERC1.md` del junior e' vecchio su questi punti.

**Partenza cinematografica e consulto finale (Andrea 08/11 sera, fatto da Claude):** in gara (runner e core libretro) le auto stanno ferme in griglia, ai lati ci sono i gruppi di supporto (le stesse persone del pubblico dell'arrivo, `supporto_<a|b>_<relax|parla|esulta>.png` da `TOOLS/disegna_pubblico.py`), poi GET READY, 3, 2, 1, START! al centro in alto (sezione aurea, y 86) con semaforo, bip e la traccia che parte allo START; cronometro, rivali e traffico partono tutti insieme. Ogni rivale ha il suo spunto (1.5..2.5 s) nei primi 15 s. All'arrivo la classifica fa prima un "FINAL CHECK" (tempi che girano), poi le righe si fermano, colonna ROUTE (L/R ai bivi, in azzurro chi ha fatto la tua strada) e verdetto: WINNER! col trofeo o YOU FINISHED Nth. Codice in `render/partenza.*`, `Game::preparaPartenza/aggiornaPartenza`, `RisultatiFase::disegnaArrivo`. Il conto alla rovescia NON e' HUD (l'HUD resta bloccato). I gate non accendono la partenza (`abilitaPartenza`), quindi partono subito come prima. Nessun agente lo ritocca finche' Andrea non l'ha provato.

## FATTO E BLOCCATO (Andrea 07/11 sera, PR #66-#72)

Non si rifanno e non si ritoccano: HUD compatto senza box (`hud.json`, `drawHudDati`), premiazione all'arrivo (`premiazione.*`), musica che reagisce agli urti (`mixer.*`), ponte con mare e transizioni, alberi lv_06 stile Windows 98, auto 50 px con animazioni. Dettagli e prossimi passi: RIEPILOGO in `RICHIESTA.md`.

## Uso nel ciclo v4.6

- Il Capo consulta questa fonte quando un task o una review dipende da feedback o decisioni di Andrea.
- Assegnazioni, criteri, prove e review indipendenti restano nel dossier `Checkpoint Revision/CP-NNN`; gli status Designer/Officina restano i rispettivi handoff.
- Le voci legacy sotto sono contesto storico: il @capo ne conferma la pertinenza nel checkpoint prima di riaprirle.
- Una risposta registrata non equivale a verifica tecnica o chiusura del checkpoint. L'autore non approva il proprio lavoro.

Andrea aggiunge qui feedback, correzioni richieste o decisioni nuove con data e ambito/ID del checkpoint. Il @capo non riscrive le risposte di Andrea; registra nel checkpoint solo l'ID, l'esito e il collegamento a questa fonte. Le richieste che richiedono una decisione esplicita usano il modello seguente:

Formato (copiare per ogni voce):
```
## D<numero> | <blocco> | <agente> | [APERTA]
Domanda:
Opzioni: A) ... B) ...
DEFAULT usato nel frattempo:
RISPOSTA DI ANDREA:
```
