# FOGLIO RICHIESTA — V3 (open)

Uso: compila i blocchi che servono; poi a `capo` scrivi:
`leggi RICHIESTA.md ed esegui il prossimo blocco con Stato [ ]; aggiorna lo Stato e scrivi gli esiti in SCAMBIO.md`
Stato: [ ] da fare · [~] in corso · [x] fatto · [!] bloccato (-> /claude) · [-] in attesa della sua tappa (non iniziare)

Prima di tutto: `VISIONE.md` (idea, vincoli duri, architettura).
Nota v4.5 (17 ott): 5 ruoli (@capo, @sviluppatore, @collaudatore, @grafico, @ricercatore); i nomi dei blocchi sono stati aggiornati, stati e contenuti no. Regole in AGENTS.md (congelate).
Nota 19 ott (aggiornata 20 ott): ruoli 4.6 = @capo, @developer, @developer-junior, @grafico. Tutti i blocchi usano questi nomi; i vecchi (@sviluppatore, @collaudatore, @builder-developer, @designer) sono tradotti dagli script solo per compatibilita'. @ricercatore non ha agente.
Nota 17 ott: questo foglio lo modificano SOLO Andrea e Claude (il ciclo lo legge per scegliere gli agenti). @capo e gli altri propongono gli stati in `out/capo/STATI_PROPOSTI.md`.

## BLOCCO 0 — Testata
| Campo | Valore |
|---|---|
| Data | 15 ott 2026 |
| Obiettivo | prototipo del motore nuovo (strada pseudo-3D) su Pi 400 a 50 fps |
| Ordine | B0 -> CONS -> B -> C (+K -> L, +M) -> (D, E, F in parallelo) -> H -> I |

## BLOCCO A — @capo
| Compito | leggere i blocchi, assegnare, tenere aggiornati i fogli, preparare PER_CLAUDE.md |
| Stato | [x] |

## BLOCCO B0 — @developer (pista C DECISA da Andrea; ora: mappa delle fonti. Il confronto con A2 e' facoltativo, resta come opzione chiusa)
| Compito | PRIMA: motore_nuovo/docs/FONTI.md = per ogni fonte in ../PIANO_V3.md (sez. «Decisione») cosa si prende come idea, cosa no, licenza verificata o «non dichiarata». POI, facoltativo: motore_nuovo/docs/CONFRONTO_PISTE.md: confronto tra A2 (mod di Cannonball) e C (motore nuovo): punti d'innesto per personaggi/fumetti/razzo/volo nel sorgente pubblico di Cannonball (solo analisi, niente copia di codice), effort, rischi, prestazioni su Pi, licenza |
| Input | ../RAPPORTO_FATTIBILITA.md sez. 1b, https://github.com/djyt/cannonball |
| Output | tabella + raccomandazione go/no-go per A2 e per C |
| Stato | [x] |
| Dopo | Andrea decide (PIANO_V3 blocco 3, decisione 1); i blocchi B-F partono dopo |

## BLOCCO B — @developer
| Compito | scrivere motore_nuovo/docs/: ARCHITETTURA.md, FORMATI_DATI.md, BUILD.md (Mac e Pi 400), LICENZE.md |
| Input | ../PIANO_V3.md, ../RAPPORTO_FATTIBILITA.md, ../RISORSE_TECNICHE.md |
| Vincoli | C++17 + SDL2, strada a segmenti, dati in file, nessun limite artificiale, licenze verificate |
| Output | 4 documenti brevi (tabelle, comandi) |
| Stato | [x] | (consegnato T2, SCAMBIO voce 16)

## BLOCCO C — @developer
| Compito | prototipo in motore_nuovo/src/: nucleo senza dipendenze di piattaforma + runner SDL2 per test + core libretro; strada a segmenti con curve e salite, auto del giocatore, 50 fps |
| Input | motore_nuovo/docs/ |
| Vincoli | nessun codice copiato senza licenza libera; nessun asset Sega; interfacce per il gameplay |
| Gate | compila con cmake; @developer-junior misura fps |
| Stato | [x] | (sintassi 30/30 SPDX MIT; build e gate reali OK il 17 ott 14:00, vedi H e T1-RUN)

## BLOCCO D — @developer
| Compito | SPEC_GAMEPLAY.md; dati/frasi.json da gianni12.py; modulo gameplay (Gianni/Gesu', escalation, inattivita') |
| Input | HANDOVER.md, SPEC_personaggi_v74.md, OPENCODE_REFERENCE_v74.json |
| Vincoli | niente codice 68000 ne' grafica/audio Sega; limiti hardware della hack non si portano |
| Stato | [x] | (SPEC_GAMEPLAY.md + src/gameplay/ + 156 fumetti in dati/frasi.json; SCAMBIO 23)

## BLOCCO E — @developer
| Compito | cercare risorse libere per: skyline sci-fi notturno, font pixel, effetti sonori motore, texture strada |
| Output | tabelle in assets_originali/ricerca/ (nome, autore, licenza, URL, adatta si/no) |
| Stato | [x] |

## BLOCCO F — @grafico
| Compito | 3 varianti per: sagoma auto del giocatore, skyline notturno, palette livello 1 |
| Input | assets_originali/DIREZIONE_ARTISTICA.md |
| Vincoli | nessun riferimento a asset Sega; max 16 colori; CREDITI.md |
| Stato | [x] | (@grafico, voce SCAMBIO 24, chiusa con voce 30)

## BLOCCO G — @grafico (opzionale)
| Compito | nuove frasi o revisione delle 156 esistenti |
| Stato | [x] | (156 frasi verificate, 16 righe corrette: out/testi/RIEPILOGO_CONSEGNA_G.md)

## BLOCCO H — @developer (+ Andrea sul Pi)
| Compito | build, replay deterministico (hash identico su due esecuzioni), tempo di frame, fps e temperatura sul Pi (ARCHITETTURA_CHECK.md) |
| Stato | [~] | (17 ott 14:00 gate reale OK: 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame, exit 0. Mancano fps/temperatura su Pi 400 e la v91 di Andrea sulla guida; [x] solo con T1-RUN punto 8. Dettaglio: out/collaudatore/ESITO_GATE_T1_17OTT_1400.md)

## BLOCCO I — @developer
| Compito | controllo di codice, licenze, asset, regole di AGENTS.md |
| Stato | [x] | (CHECK 2 ore 4, esito in SCAMBIO.md voce 20; B/C/K/M confermati [x], correzioni segnalate a @developer prima di H)

## BLOCCO J — Escalation a Claude
Quando: 2 tentativi falliti o fine blocco importante. In OpenCode: `/claude` (crea PER_CLAUDE.md).
Poi: Claude app -> `leggi PER_CLAUDE.md e SCAMBIO.md e dai il tuo parere`. (Con `ciclo.sh` lo fa il comando `claude`.)
| Stato | [ ] |

## MODELLO — copia per un nuovo blocco
```
## BLOCCO K — @developer (dopo B0; in parallelo a C)
| Compito | progettare 3 livelli di prova: motore_nuovo/dati/livelli/<id>/livello.json + riga in motore_nuovo/docs/LIVELLI.md |
| Input | VISIONE.md, motore_nuovo/docs/ARCHIVIO_V87.md (ritmo degli show), DIREZIONE_ARTISTICA.md |
| Output | 3 livelli, ciascuno con `stile` diverso scelto da DIREZIONE_ARTISTICA.md (tema, lunghezza, seme, traffico, difficolta' facile, punti degli show, moduli fx, scenario richiesto) |
| Stato | [x] | (consegnato T2, SCAMBIO voce 18)

## BLOCCO L — @grafico (dopo K)
| Compito | per ciascuno dei 3 livelli: 3 varianti di sfondo + oggetti a bordo strada + palette, poi produzione dopo la scelta di Andrea |
| Output | assets_originali/scenari/<id>/ con manifest.json; CREDITI.md |
| Stato | [x] | (@grafico, voce SCAMBIO 34 + voce 37; 9 cartelle variante piene, manifest per livello, 3 righe CREDITI.md, licenza CC0-1.0, originale=true)

## BLOCCO FED — @developer-junior (con B)
| Compito | compilare motore_nuovo/docs/FEDELTA.md (10 tratti, prova per ciascuno) e proporre in ANDREA_REVIEW.md D3 il numero di tappe/bivi |
| Stato | [x] | (consegnato con BLOCCO B, SCAMBIO voce 16: FEDELTA.md 10 tratti, punteggio 50.0% soglia; D3 default 5 tappe/bivi, 3 livelli di prova)

## BLOCCO CONS — @developer (conservazione, subito dopo B0)
| Compito | seguire motore_nuovo/docs/ARCHIVIO_V87.md: estrarre i 156 fumetti e le tabelle show in JSON, scrivere la logica degli show in .md; aggiornare CATALOGO_PERSONAGGI.md |
| Vincolo | sola lettura sulla v87; niente va perso |
| Stato | [x] |

## BLOCCO M — @grafico (in parallelo a K)
| Compito | 3 varianti di: font pixel, lettering titolo «Sglavinated Edition», menu principale e HUD (320x224); poi produzione dopo la scelta di Andrea |
| Input | DIREZIONE_ARTISTICA.md, assets_originali/ricerca/ (font liberi da @ricercatore) |
| Output | assets_originali/ui/ + motore_nuovo/dati/ui/{menu,hud}.json + CREDITI.md |
| Stato | [~] | (consegnato T2, SCAMBIO 15; manifest in "bozza" finche' Andrea non approva i PNG B/N; D8 "tempo" da aggiungere a hud.json)

## BLOCCO N — @developer (solo dopo che C gira; facoltativo)
| Compito | motore_nuovo/docs/FATTIBILITA_ROM_MD.md: si puo' produrre una ROM Mega Drive originale (.md) dal v3? Toolchain e licenze (es. SGDK: verificare), budget 68000 a 50 fps, limiti VRAM/sprite/colori, cosa del motore C si riusa (dati, livelli, fumetti, regole), cosa va riscritto |
| Output | tabella go/no-go + stima in blocchi; se go, piano di una v9x «.md» |
| Vincoli | nessun byte della ROM Sega, ne' codice Sega; PAL 50 Hz; test su Genesis Plus GX |
| Stato | [ ] |

## BLOCCO X — @agente
| Compito | |
| Input | |
| Vincoli | |
| Gate | |
| Stato | [ ] |
```

## BLOCCO P1 — @developer (PRIORITA' MASSIMA, da Andrea 16 ott: primo livello giocabile)
| Nota | Claude ha compilato motore_nuovo su Linux: core e runner compilano, sono deterministici, ~0,5 ms/frame. A schermo pero' solo strada grigia dritta + rettangolo. Ora serve un livello giocabile, non documenti. |
| Compito | 1) player.cpp: accelera con Pad::A (oggi solo Up: col pad l'auto resta ferma), freno su B o Down. 2) CMakeLists.txt: find_package(SDL2_image) invece di SDL2_IMAGE (oggi i PNG non si caricano mai). 3) Game::init() legge i segmenti della pista da livello.json (dopo P2), curve visibili. 4) sprite veri da assets_originali: auto giocatore, traffico, cartelloni, bordo strada, skyline (niente rettangoli). 5) con @developer (P3): disegnare Gianni, Gesu', UFO e fumetto (oggi in game.cpp lo show e' scartato con (void)show). |
| Gate | build cmake ok; 3 screenshot in out/sviluppatore/ con pista curva, traffico, cartelloni, un personaggio con fumetto |
| Stato | [~] | (punti 1-3 e 5 fatti; punto 4 sprite veri ancora aperto = T1-RUN 4; build e 3 screenshot strada OK in out/collaudatore/)

## BLOCCO P2 — @developer (per P1)
| Compito | aggiungere in ogni livello.json la lista "segmenti" (curva, collina, lunghezza) con curve abbastanza forti da vedersi; documentare il formato in motore_nuovo/docs/FORMATI_DATI.md |
| Stato | [x] |
| Nota | Segmenti (20 per livello) aggiunti in tutti i 3 livello.json (SCAMBIO voce 75). FORMATI_DATI.md NON modificabile qui: write negato (regola 21, permission denied). Documentazione da completare appena il write è sbloccato; non blocca P2 (dati già presenti).

## BLOCCO P3 — @developer (per P1)
| Compito | interfaccia di disegno per gli show: cosa disegnare a ogni frame (personaggio, posa, posizione, fumetto, UFO) da DatiShow; aiutare @developer a collegarla al renderer |
| Stato | [x] |
| Nota | src/gameplay/disegno_show.h/.cpp + testi_frasi.h/.cpp + DISEGNO_SHOW.md consegnati (SCAMBIO voce 77, MIT). Collegamento in game.cpp (sostituisce (void)show) e nel Renderer spetta a @developer dentro P1 (voce 71/1-2).

## BLOCCO MD0 — @developer — TAPPA 1: la ROM parte e si pilota
| Compito | ROM Mega Drive minima e originale con SGDK (MIT): all'avvio va subito in gara. Strada che scorre (strisce di corsia), auto che accelera (A/Su), frena (B/Giu') e sterza (sinistra/destra). Niente menu, logo o testi: arrivano nelle tappe 2-3 (`../TAPPE.md`). Sorgenti in `motore_nuovo/export_md/`, ROM in `out/coder/sglavinated_test.md` |
| Input | `../TAPPE.md`, `motore_nuovo/docs/FATTIBILITA_ROM_MD.md`, logica di guida da `motore_nuovo/src/player/` e `src/road/` (stessi valori di accelerazione e freno) |
| Vincoli | Grafica provvisoria (rettangoli e colori pieni). Niente codice, grafica, musica o logo SEGA. Build solo sul Mac o sul Pi 400 (regola 21) |
| Gate | la ROM parte su emulatore (Genesis Plus GX) e si guida per 5 minuti senza crash; 2 screenshot in `out/coder/` |
| Stato | [-] (sospeso: prima il C++, scelta di Andrea 17 ott) |

## BLOCCO P1b — @developer — TAPPA 1: niente collisioni fantasma
| Compito | In `src/game/game.cpp` il traffico segnaposto (`spawnTraffic`) e' invisibile, sta sempre a distanza fissa davanti al giocatore (x = +-800) e la collisione scatta gia' con |dx| < 700: sterzando oltre x = 100 il gioco vede un incidente continuo (causa probabile dell'auto che sparisce, SCAMBIO voce 67-68). Per la tappa 1: disattiva traffico segnaposto e relative collisioni (flag), lascia solo strada e auto. Fixa anche il clamp di playerX (auto sparisce a playerX=125 dopo 60 frame di sterzata, voce 67). |
| Gate | prova headless o runner: sterzando a destra e sinistra per 10 s l'auto resta visibile e nessun crash viene segnalato |
| Stato | [x] | (codice SCAMBIO 86/87; gate headless 17 ott PASSATO: sterzo sx/dx, offroad f.476, 30000 frame senza crash)

## BLOCCO T1-RUN — @developer + @developer-junior — prossimo run: implementazione e test (17 ott 14:00)
> PROTETTO: titolo e Stato di T1-RUN li gestisce Andrea.
| Campo | Valore |
|---|---|
| Premessa | Il motore COMPILA e il gate headless PASSA. **Per compilare e provare: un solo comando `sh tools/gate_t1.sh` (o `... soak`) dalla radice sglavinated_src/, senza pipe/`2>&1`/`;`/`echo`/`which`** (AGENTS.md, Build e test). Il vecchio "shell negata" era un errore di diagnosi: i comandi composti vengono negati, quelli semplici passano. Non riscrivere "build non eseguibile" se non e' negato proprio quel comando. |
| 0 Strada (fatto da Claude) | Corretti in `render/renderer.cpp`: interpolazione del trapezio invertita (lontano/vicino) che spezzava i bordi; strisce non ancorate al mondo. Ora bande alternate per indice segmento (asfalto, erba, rumble bianco/magenta, tratteggio di corsia) e `road.cpp` proietta con z continua dentro il segmento (scorrimento fluido). Sprite/cartelloni seguono la curva della strada. Verificare con gli screenshot di `out/collaudatore/<livello>/`. |
| 1 @developer | **Acceleratore** (fatto da Claude, PR #2, 17 ott): ACCEL 110, BRAKE 120, gas rilasciato COAST 0.995 in player.h; speed@60 da 33,76 a 100,83, replay 2x IDENTICO. Resta: Andrea conferma la sensazione sul runner (punto 8); eventuali ritocchi un parametro alla volta. |
| 2 @developer | **Colline**: `Road::project` integra due volte la collina (dy += hill; y += dy*len), quindi la strada sale nel cielo e la camera non ne segue la quota (`camY` fissa). Portare l'altezza a quota assoluta per segmento e camera = quota sotto l'auto + CAM_HEIGHT. Stessa cosa per l'ascissa: usare x relativa al punto sotto l'auto. |
| 3 @developer | **Sprite**: `drawSprites` usa worldZ assoluto contro playerZ avvolto sul giro: vicino al punto di wrap gli sprite spariscono. Calcolare zRel con modulo totalLen. |
| 4 @developer | **Sprite veri** (fatto da Claude, PR #2): gli sprite erano scalati per ~0,001 e larghi 0 pixel, mai visibili; ora `drawSprites` usa la proiezione della strada (`Renderer::SPRITE_WORLD_PX`) con ritaglio allo schermo, cartelloni a +/-1600. Resta: densita' dei cartelloni in `livello.json` (oggi troppo fitti, uno ogni 1-4 segmenti) e caricamento degli sprite dal catalogo grafico (blocco G1). |
| 4b @developer-junior | Fatto da Claude (PR #2): `prova_gate.cpp` carica i 21 `.sglraw` prima di `init()`; screenshot con cartelloni veri in `out/collaudatore/<livello>/`. |
| 5 @developer-junior | Aggiungere casi a prova_gate: wrap di playerZ oltre totalLen_, inattivita' 10 s -> show/fumetto visibile in screenshot. `sh tools/gate_t1.sh soak` ad ogni modifica del coder. |
| 6 Libreria asset | @developer-junior (o chi genera asset con Python) lancia `python3 motore_nuovo/strumenti_py/indice_assets.py` dopo ogni generazione; il primo indice (da elenco cartelle) e' gia' in assets_originali/INDICE_ASSET.json/.md, senza dimensioni PNG ne' duplicati veri finche' lo script non gira sui file reali: produce `assets_originali/INDICE_ASSET.json/.md` (cosa esiste, dimensioni, duplicati, segnaposto, lacune). Gli agenti grafici consultano l'indice prima di creare. Lacune note: traffico, sprite personaggi (Gianni, Gesu'), UFO. |
| 7 @developer-junior | Rivedere SOLO: renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh. |
| 8 Andrea | Provare `sglavinated_runner` sul Mac (SDL2) e registrare in `ANDREA_REVIEW.md` l'esito della guida; misurare fps/temperatura su Pi 400 (`vcgencmd measure_temp`, `get_throttled`). Questo chiude H solo dopo aver registrato la prova. |
| Fuori scope | MD0, T5 (traffico/collisioni/razzo), N, M (finche' Andrea non approva i PNG). Non promuovere H/P1 senza i punti 4 e 8. |
| Gate | `sh tools/gate_t1.sh soak` exit 0; screenshot con cartelloni veri e strada con bande allineate. |
| Stato | [~] | (gate FALLITO 19/10 09:39 da Andrea su Mac, exit 13: giro guidato FALLITO su 5/6 livelli — "percorso poco vario o non guidabile"; solo lv_02_atomico OK. Gate core OK: soak, accelerazione, ribaltamento, sterzo, curve, gara, screenshot P1, ms/frame ~0.19 max 0.94. Percorsi risolti con R1 (gate 6/6 verde 21/10 10:44). Resta solo la v91 di Andrea sul runner SDL2 (revisione in corso: non toccare `src/runner/`). Dettaglio: ANDREA_REVIEW.md T1-Q3, STATUS_OFFICINA.md report 09:39)

## BLOCCO G1 — @grafico — catalogo unico delle risorse grafiche (da Andrea e Claude 17 ott)
| Compito | Tenere `assets_originali/CATALOGO_GRAFICO.json` come unico elenco delle risorse grafiche: un elemento per riga con `id`, `tipo`, `stato` (mancante, bozza, in_costruzione, pronto, approvato, scartato), `file`, `raw` (versione per il motore) e `livelli`. Claude ha creato la prima versione (52 elementi: 21 cartelloni, 3 auto, skyline e oggetti di variante A per i 3 livelli, UI variante B, font; mancano traffico, Gianni, Gesu', UFO). 1) Catalogare i 100 PNG oggi fuori elenco (varianti B/C, scenari doppi `scenario_<n>_01_*` e `scenario_lv_*`): per ognuno `scartato` o un elemento nuovo, senza cancellare file. 2) Ogni sprite nuovo entra nel catalogo con il suo stato prima di essere usato. 3) Dopo ogni modifica: `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` (exit 0). |
| Input | `assets_originali/CATALOGO_GRAFICO.json`, `INDICE_ASSET.md`, `MAPPA_LIVELLI_SPONSOR.json`, `ANDREA_REVIEW.md` (D2, D4-D7) |
| Vincoli | Solo @grafico scrive il catalogo. Il motore e i test usano solo `pronto` e `approvato` (`in_costruzione` solo nei test). Niente sottocartelle nuove senza voce nel catalogo. |
| Gate | `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0" |
| Stato | [ ] |

## BLOCCO T5 — @developer — TAPPA 5: altri veicoli, collisioni, razzo
| Compito | 1) @developer: in ogni livello.json aggiungi `traffico_spawn` (segmento, corsia, tipo, velocita') e `rivali` (2-3 auto che corrono la gara, con velocita' e stile di guida), deterministici dal `seme`. 2) @developer: veicoli con posizione, corsia e velocita' propri (si raggiungono e si sorpassano); rivali con IA semplice (seguono la strada, cambiano corsia); auto del giocatore pseudo-3D: la telecamera segue la posizione sulla strada e lo sprite cambia posa in sterzata; collisioni con traffico e rivali (rallentamento o testacoda). 3) Razzo: tasto secondario (X sul pad, Spazio sulla tastiera; B resta il freno), usa `moduli_fx.razzo_B` del livello, con ricarica |
| Input | `../TAPPE.md`, `dati/livelli/*/livello.json` (campo `traffico`), `src/gameplay/`, `src/game/game.cpp` |
| Vincoli | Meccaniche di genere, niente codice o grafica SEGA. Determinismo: stesso seme e input = stesso hash. Rettangoli colorati finche' non ci sono sprite |
| Gate | in 2 minuti di guida: sorpassi traffico, almeno un rivale ti supera o lo superi, una collisione rallenta, il razzo spinge e si ricarica |
| Stato | [-] (tappa 5) |

---

## BLOCCO R1 — @developer + @developer-junior — tracciati modificabili per le 6 piste (da Andrea e Claude 19 ott)
| Campo | Valore |
|---|---|
| Problema | Il gate `percorso` fallisce su 5 piste su 6 (rettilinei 12-23%, minimo 25%). Causa: commit `a8b5292` in `src/road/road.cpp`: il raccordo della curva ora dura TUTTO il tratto (un rettilineo dopo una curva torna a 0 solo all'ultimo segmento) e `seg.curve -= correzione` sposta anche i segmenti dritti. Andrea: la curvatura non era ottimale, la dinamica dei percorsi va ottimizzata pista per pista e tratto per tratto. |
| Compito | 1) @developer, `road.cpp`: raccordo di entrata/uscita a lunghezza propria (campi nuovi `entrata` e `uscita` in segmenti, default min(n/3, 60)), poi curva costante; correzione della deriva distribuita solo sui segmenti con curva diversa da 0, mai sui rettilinei. 2) @developer: tracciato modificabile a mano. Nuovo `dati/tracciati/tratti.json` = libreria di tratti con nome (es. `rettilineo_corto`, `rettilineo_lungo`, `curva_dolce_dx/sx`, `curva_stretta_dx/sx`, `esse`, `tornante`, `collina_su/giu`), ognuno con curva, collina, lunghezza, entrata, uscita. In ogni `livello.json` nuovo campo `tracciato`: lista di nomi di tratto, con override opzionali (es. `{"tratto":"curva_dolce_dx","lunghezza":500}`). Se c'e' `tracciato`, il loader lo espande in `segmenti`; se manca, si usa `segmenti` come oggi (compatibile). 3) @developer: regole per pista in `dati/tracciati/regole.json` (quota rettilinei min/max, rettilineo piu' lungo, curva massima, quota fuori strada massima), lette da `prova_gate` al posto delle soglie scritte nel codice (`test/prova_gate.cpp:503` e riga del return). 4) @developer-junior: il gate `percorso` stampa, per ogni pista, quale TRATTO viola quale regola (nome tratto + segmento iniziale), cosi' chi corregge sa dove mettere mano; aggiungi un test che carica ogni `tracciato` e verifica che l'espansione dia lo stesso numero di segmenti della somma delle lunghezze. 5) @developer-junior: riscrivi i 6 `livello.json` con `tracciato` (stesso carattere di ogni tema: lv_02 resta come riferimento perche' passa) e segnala nel rapporto di test ogni pista in cui la curvatura e' ancora poco guidabile. |
| Input | `motore_nuovo/src/road/road.cpp`, `motore_nuovo/src/game/game.cpp` (loader livello), `motore_nuovo/dati/livelli/*/livello.json`, `motore_nuovo/test/prova_gate.cpp`, `motore_nuovo/docs/FORMATI_DATI.md` |
| Vincoli | Non abbassare le soglie per far passare il gate: si correggono i tracciati. Determinismo invariato (replay 2x IDENTICO). Formato documentato in `docs/FORMATI_DATI.md` con un esempio, perche' Andrea deve poter modificare una pista solo editando il JSON. |
| Gate | `sh tools/gate_t1.sh soak` verde su tutte e 6 le piste, `ctest` 17/17. Review incrociata: @developer rivede 4-5, @developer-junior rivede 1-3. @capo apre CP-005 in `Checkpoint Revision/` (template, registro, STATO_CORRENTE) e lo chiude solo con gate verde e entrambe le review registrate. |
| Stato | [x] | (Claude 22/10: gate 6/6 verde 21/10 10:44 `out/developer/ESITO_GATE_21OTT_1044.md`, `ctest` 17/17 ricontrollato su main ae16132; review registrate: `out/developer-junior/REVIEW_CP005_PUNTI1-3_DEVELOPER_2026-10-20_1714.md` POSITIVA, `out/developer/REVIEW_CP005_PUNTI45_JUNIOR_20OTT.md` POSITIVA. Resta solo a @capo: chiudere CP-005 nel registro. Prossimo: F60) |

---

## BLOCCO R2 — @developer + @developer-junior + @grafico — ciclo di gioco completo (da Andrea 19 ott)
| Campo | Valore |
|---|---|
| Richiesta di Andrea | La guida e la gara devono stare dentro un gioco a fasi: intro/splash (caricamento), menu gara (scelta e conferma corsa con Start/Invio), gameplay, fine gioco quando finisce il tempo, risultati con punteggio (score) insieme a risultati fittizi di altri giocatori, conclusione e ripartenza dal menu. |
| Compito | 1) @developer: macchina a stati in `src/game/` (nuovo `fasi.h/.cpp`): `Splash -> Menu -> Gara -> FineTempo -> Risultati -> Menu`. Ogni fase ha entra/aggiorna/disegna/esce; il runner chiama solo la fase corrente. Splash: logo e caricamento asset, poi menu da solo o con un tasto. Menu: scelta corsa (pista di partenza o gara a tappe da `percorso_gara.json`), conferma con Start/Invio, Indietro con B/Esc. FineTempo: quando `tempo_limite_s` arriva a 0 l'auto rallenta e compare "GAME OVER/TEMPO SCADUTO" per qualche secondo. Risultati: score = distanza + bonus checkpoint + bonus tempo residuo (formula in `dati/punteggio.json`, modificabile); classifica con 9 giocatori fittizi da `dati/classifica_finta.json` (nomi e punteggi), tuo punteggio inserito al suo posto e nome a 3 lettere; Start torna al menu. 2) @developer-junior: gate headless `prova_gate ... ciclo` che attraversa tutte le fasi con input scriptati (Start, guida, tempo scaduto, risultati, ritorno al menu) e controlla l'ordine delle fasi e lo score deterministico; screenshot PPM di ogni fase. 3) @grafico: grafica di splash, menu e schermata risultati con gli asset della Libreria (testi, font, HUD variante B); segnala nel catalogo cosa manca. |
| Input | `motore_nuovo/docs/FLUSSO_GIOCO.md` (proposta del 18 ott: seguire quella dove non contraddice questa richiesta), `src/game/`, `src/runner/main.cpp`, `dati/percorso_gara.json`, `dati/ui/hud.json` |
| Vincoli | Stessi controlli su tastiera e pad. Nessuna fase blocca il caricamento su Pi 400 (splash corto). Determinismo invariato. Arrivi e finali animati restano per dopo: per ora la gara finisce solo per tempo o all'ultima tappa, poi Risultati. |
| Gate | `ctest` verde incluso il gate `ciclo`; screenshot delle 5 fasi in `UFFICIO/Officina/Test area/`. Review incrociata @developer/@developer-junior, @grafico rivede le schermate. @capo apre CP-006 in `Checkpoint Revision/`. Dopo R1. |
| Stato | [ ] | (dopo F60) |

---

## BLOCCO F60 — @developer + @developer-junior — gioco a 60 Hz, tempi in secondi (Andrea 19 ott; dopo R1, prima di R2)
| Campo | Valore |
|---|---|
| Compito | Parte 1 della nota di Andrea "frequenza di gioco" in `ANDREA_REVIEW.md` (punti 1-3): tutte le misure in secondi con `dt`, 60 Hz predefinito, opzione 50 Hz in `dati/config.json` (`"frequenza_hz"`), core libretro che dichiara la frequenza scelta. @developer scrive, @developer-junior rivede e aggiunge al gate la prova a 50 e a 60 Hz. |
| Vincoli | Non toccare `src/runner/` finche' Andrea non chiude la sua prova del runner su Mac: l'opzione `--hz` e il contatore dei frame ripetuti nel runner vengono dopo. Soglie del gate invariate, determinismo invariato. |
| Gate | `ctest` verde a 60 Hz e a 50 Hz; giro guidato con gli stessi secondi (+/- 2%) alle due frequenze; replay 2x IDENTICO a ciascuna. @capo apre il checkpoint. |
| Stato | [ ] |

---

## RIEPILOGO CORRENTE — tenuto da Andrea/Claude, un solo blocco (storico: out/capo/RICHIESTA_STORICO_17OTT.md)
Tappa 1. Build e gate reali OK (17 ott 14:00, `sh tools/gate_t1.sh`): 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame. Aperti: guida (v91 di Andrea), colline, wrap sprite, sprite veri nel gate, fps/temperatura su Pi 400.
Stati: C [x] · H [~] · P1 [~] · P1b [x] · P2 [x] · P3 [x] · M [~] · T1-RUN [~] · J [ ] · N [ ] · MD0 [-] · T5 [-].
Tocca a: @capo chiusura CP-005 (R1 fatto) · @developer + @developer-junior F60 (60 Hz, priorita') poi R2 (ciclo di gioco) (ordine test: prima Mac di Andrea, poi RetroPie/Pi 400; v. ANDREA_REVIEW T1-Q5) · @developer T1-RUN 2-3 e densita' cartelloni · @developer-junior T1-RUN 5-7 · @grafico G1 (catalogo grafico, su richiesta) · Andrea 8. Nessuna decisione aperta (D1-D9 chiuse), nessun PER_CLAUDE.md, FINITO non creabile.
