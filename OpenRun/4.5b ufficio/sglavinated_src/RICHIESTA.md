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
| Stato | [x] | (Claude 23/10: `check_catalogo_grafico.py` exit 0, 159 elementi, PNG non catalogati: 0; @grafico 22/10) |

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
| Chi tocca adesso (Claude 27/10 pomeriggio, leggere per primo) | **Nessuno aspetta D18 o D19 per R2 e per i blocchi dopo** (Andrea 27/10: D18 e' chiusa, R2, DECO1 2f, RIV, PIST1 vanno avanti senza altri ok). Scrivere "R2 attende Andrea" o "D18 = B" e' sbagliato e conta come turno a vuoto. Fatto da Claude 27/10: passi 1b e 1c. `GestoreFasi::cambia()` chiamata da dentro una fase (menu che avvia la gara) viene eseguita a fine `aggiorna()`, la fase in corso non viene piu' distrutta a meta'; `avviaGara()` e `init()` partono in Gara (gate e `--subito`); `Game::mostraMenu()` apre Splash -> Menu; il runner parte da Splash (salvo `--livello` o `--subito`), ESCI chiude il runner; FineTempo scrive ARRIVO o TEMPO SCADUTO; testi in maiuscolo (il font non ha minuscole). Gate `ciclo` riscritto e in ctest: Splash -> Menu -> Gara -> FineTempo -> Risultati -> Menu, piu' CORSA SINGOLA ed ESCI; **ctest 22/22 verde**. Prima di questa correzione `gara`, `bivio_uscita`, `bivio_bordi`, `tema_transizione` fallivano: lanciare sempre **tutto** `ctest`, non una parte. **Prossimo, @developer, un passo per turno:** 1d) nome a 3 lettere nei Risultati (su/giu cambia lettera, Start conferma), gate `ciclo` ancora verde; poi DECO1 passo 2f; poi RIV punti a, b, c (bump, sprite rivali dal catalogo, rivali piu' realistici); poi PIST1 punto 1. @developer-junior: review incrociata di `fasi.cpp` e del gate `ciclo`, poi gate `urto` quando c'e' il bump. @grafico: schermate splash, menu e risultati (punto 3) con gli asset della Libreria. @capo: apre il CP di R2 quando c'e' 1d. |
| Stato | [~] | (Andrea 25/10: R2 parte subito DOPO DECO1, PRIMA di PIST1 e RIV. Ordine: FORK1 review -> DECO1 -> R2 -> PIST1 -> RIV -> FIN. Struttura voluta: intro (caricamento), selezione, partita, risultati, ripartenza. Per R2 si tocca `src/runner/` solo per chiamare la fase corrente.) |

---

## BLOCCO F60 — @developer + @developer-junior — gioco a 60 Hz, tempi in secondi (Andrea 19 ott; dopo R1, prima di R2)
| Campo | Valore |
|---|---|
| Compito | Parte 1 della nota di Andrea "frequenza di gioco" in `ANDREA_REVIEW.md` (punti 1-3): tutte le misure in secondi con `dt`, 60 Hz predefinito, opzione 50 Hz in `dati/config.json` (`"frequenza_hz"`), core libretro che dichiara la frequenza scelta. @developer scrive, @developer-junior rivede e aggiunge al gate la prova a 50 e a 60 Hz. |
| Vincoli | Non toccare `src/runner/` finche' Andrea non chiude la sua prova del runner su Mac: l'opzione `--hz` e il contatore dei frame ripetuti nel runner vengono dopo. Soglie del gate invariate, determinismo invariato. |
| Gate | `ctest` verde a 60 Hz e a 50 Hz; giro guidato con gli stessi secondi (+/- 2%) alle due frequenze; replay 2x IDENTICO a ciascuna. @capo apre il checkpoint. |
| Stato | [x] | (Claude 23/10: gate verde 22/10 20:06, `gate_t1.sh` exit 0, GATE FREQUENZA 60 Hz OK e 50 Hz OK, replay IDENTICO, 6/6 percorsi OK, `out/developer-junior/ESITO_F60_22OTT_2006.md`. A @capo resta solo il checkpoint CP-006. Prossimo: FORK1) |

---

## BLOCCO FORK1 — @developer + @developer-junior + @grafico — bivio e cambio tappa fluidi, transizione di tema, bordi strada (Andrea 22 ott, dopo prove su Mac; dopo F60, prima di RIV)
| Campo | Valore |
|---|---|
| Segnalazione di Andrea | Nei test su Mac: all'uscita e all'entrata di un percorso (cambio tappa) si vede per un attimo una prospettiva sbagliata; il bivio deve essere fluido; troppo facilmente l'auto resta intrappolata contro i bordi della strada e va in crash. Precisazione di Andrea (22/10): l'effetto al bivio e' legato al Tema grafico; il passaggio da una tappa all'altra deve essere una transizione graduale di tema, un momento di passaggio e non uno stacco. |
| Cause probabili (lette nel codice da Claude 22/10, NON ancora riprodotte: verificare prima di correggere) | a) `Road::carreggiate()`: a bivio aperto ogni ramo ha `fattore` 0.6; in `Game::fineTappa()` il livello nuovo non ha bivio e la strada torna a 1.0 in un solo frame (scatto di larghezza); `player_.centroCorsia` e `larghezzaCorsia` restano quelli del vecchio ramo per un frame. b) `fineTappa()` non raccorda curva e collina: il nuovo tracciato parte col suo valore; il tema (`Game::leggiTemaDaFile` -> `ren_.impostaTema`: colori `cielo`, `erba`, `erba_alt`, `rumble` piu' lo stile dei cartelloni) cambia in un solo frame. c) Prima della scelta (`latoBivio_ == 0`) `Game::update` assegna `centroCorsia` al ramo piu' vicino: quando l'auto passa da x<0 a x>0 il centro salta di circa 2d, `vincolaAllaCorsia()` lo ricolloca con un clamp a `X_LIMIT` e l'auto scatta di lato. d) Bordi: sul bivio la soglia offroad e' `OFFROAD_LIMIT * larghezzaCorsia` (700 x 0.6 = 420) mentre la spinta della curva (`CURVA_SPINTA`) porta verso l'esterno; dopo `OFFROAD_CRASH_FRAMES` (100 frame) c'e' il ribaltamento, e `OFFROAD_SLOW` 0.99 per frame blocca la velocita' (~160 -> ~6 in pochi frame, gia' visto in D11). |
| Compito | 1) @developer, cambio tappa: nessuno scatto di prospettiva. Il `fattore` finale del ramo scelto torna a 1.0 in circa 1 s (in secondi, compatibile con F60); `centroCorsia`, `larghezzaCorsia` e `road_.playerX` ricalcolati nello stesso frame del cambio; curva e collina dei primi segmenti raccordate con quelle finali della tappa precedente; il tema non cambia piu' di colpo (vedi punto 6). 2) @developer, prima della scelta: niente salto di `centroCorsia` quando l'auto passa da un ramo all'altro (continuita' o isteresi); il ramo non scelto non deve spingere l'auto. 3) @developer, bordi: l'auto non deve restare intrappolata. Con sterzo attivo verso la strada deve riuscire a rientrare anche con la spinta di curva (attenuare la spinta quando e' offroad o usare una larghezza minima della corsia sul bivio); il ribaltamento per fuori strada scatta solo se l'auto resta fuori davvero. Un parametro alla volta, soglie e costanti in `dati/` se possibile; documentare il valore scelto. 4) @developer-junior: nuovo gate `bivio_uscita` per le 4 combinazioni di bivi: registra per ogni frame (±60 attorno a ogni cambio tappa) larghezza strada, centro corsia, `x` dell'auto, curva e collina, e fallisce se lo scarto da un frame all'altro supera una soglia (da proporre e scrivere in `dati/tracciati/regole.json`); screenshot PPM di 5 frame prima e 5 dopo il cambio. 5) @developer-junior: gate `bivio_bordi`: guida scriptata che sterza normalmente dentro il bivio e fuori dal bivio sul livello piu' curvo; deve arrivare al cambio tappa senza ribaltamento e senza piu' di N frame consecutivi offroad (N proposto da @developer-junior). 6) Transizione di tema graduale. @developer: il Renderer interpola `TemaColori` (`cielo`, `erba`, `erba_alt`, `rumble`) dal tema della tappa che finisce a quello della tappa che comincia. Il tema di arrivo e' quello del ramo scelto, quindi parte dopo la scelta (da `bivioBloccoZ`) e finisce nei primi secondi della tappa nuova; durata in secondi, letta da `dati/` (es. `dati/config.json` `transizione_tema_s`, proposta 3-4 s), non in frame. Lo stile dei cartelloni cambia solo quando i vecchi cartelloni sono fuori vista. Deterministico (replay 2x IDENTICO). @grafico: per le 6 coppie di tappe (01->02, 01->03, 02->04, 02->05, 03->05, 03->06) controlla che i colori si fondano in modo gradevole e, se serve, ritocca i colori in `livello.json` (suo ambito); facoltativo, proposta: un elemento di passaggio al bivio (cartello o portale col nome del nuovo tema), segnalato nel catalogo. 7) @developer-junior: gate `tema_transizione`: per ogni coppia, i 4 colori variano in modo monotono nell'intervallo, finiscono esattamente sul tema nuovo, e screenshot PPM a inizio, meta' e fine transizione. |
| Input | `motore_nuovo/src/game/game.cpp` (`fineTappa`, `preparaBivio`, update del giocatore), `src/road/road.cpp` / `road.h` (`carreggiate`, bivio), `src/player/player.cpp` / `player.h` (`vincolaAllaCorsia`), `dati/percorso_gara.json`, `test/prova_gate.cpp` |
| Vincoli | Struttura delle tappe invariata (piramide a 6 tappe, 3 bivi, 3 arrivi; lv_05 raggiunta da lv_02 e lv_03, stesso finale: confermato da Andrea 22/10). Non abbassare le soglie del gate per farlo passare. Non toccare `src/runner/`. Determinismo invariato (replay 2x IDENTICO). Stesso file `game.cpp` toccato da F60: il @developer coordina, F60 per primo. |
| Gia' fatto da Claude (23/10 pomeriggio, PR su richiesta di Andrea dopo la prova su Mac) | a) Transizione di tema: `Renderer::aggiornaTransizioneTema` interpolava verso `tema_` sovrascritto a ogni frame, quindi ogni tappa restava con i colori della prima; ora c'e' `temaA_` (tema di arrivo fisso) e la tappa nuova finisce esattamente sul suo tema. b) Passaggio assistito al bivio (richiesta di Andrea: il bivio e' un premio, guida facilitata, poi si riprende normale): da `bivioInizioZ` `Game::assistenzaBivio_` sale a 1 in 0.5 s; niente spinta di curva, niente ribaltamento per fuori strada o contatti, richiamo morbido (max 250 unita'/s, sotto lo sterzo) verso il centro del ramo piu' vicino; a inizio tappa nuova si spegne in 2.5 s. c) Paletti bordo strada nascosti mentre le carreggiate sono due. d) Auto incastrata in mezzo ai rami che si ribaltava per sempre (prova di Andrea 23/10): dopo il ribaltamento si ripartiva da `centroCorsia` = 0, cioe' dallo spartitraffico; ora si riparte sul ramo piu' vicino (`playerXIncidente_`). Resta da fare (@developer): raggio piu' ampio nell'apertura del bivio (rami che si separano piu' dolcemente, `raccordo` in `Road::carreggiate`, `BIVIO_SEG`), senza scatti di prospettiva. Il gate `tema_transizione` (punto 7) resta da scrivere e deve passare con questo codice; non reintrodurre l'interpolazione verso `tema_`. Punti ancora aperti: 4, 5, 7 e la parte @grafico del 6 (elemento di passaggio al bivio, anche come effetto 'mistico' del premio). |
| Gate | `ctest` verde con `bivio_uscita`, `bivio_bordi` e `tema_transizione`; replay 2x IDENTICO; gate `gara` 4 combinazioni OK; screenshot in `UFFICIO/Officina/Test area/`. Review incrociata: @developer rivede 4-5 e 7, @developer-junior rivede 1-3 e 6; @grafico rivede gli screenshot della transizione. @capo apre il checkpoint. La prova su Mac resta di Andrea: gli agenti non la segnano come superata. |
| Fatto da Claude (25/10, su richiesta di Andrea: stallo developer/junior della mattina) | Causa vera del FALLITO `bivio_uscita` (x_rel 1800): a scelta aperta `centroCorsia` era fisso a 0 e il clamp `X_LIMIT` (900) teneva l'auto sullo spartitraffico, fuori strada, mentre i rami si aprono a `BIVIO_DIST` 1900; al blocco l'auto scattava di 100 unita'. Ora in `game.cpp` a scelta aperta la corsia e' il ramo piu' vicino (`carreggiate().centro`, fattore del ramo). Gate corretti in `prova_gate.cpp`: `bivio_uscita` misura x rispetto al ramo piu' vicino e il centro nel sistema prima del ricentramento; `bivio_bordi` guidava verso lo spartitraffico (obiettivo 0 a scelta aperta, `lato*BIVIO_DIST` dopo il blocco) e ignorava la seconda scelta (DS e DD finivano entrambi in lv_06): ora punta al centro del ramo scelto. I 3 gate FORK1 sono in ctest (`CMakeLists.txt`). `dati/config.json`: `transizione_tema_s` 2.0 (Andrea 25/10); il gate frequenza riscriveva `config.json` da zero e cancellava la chiave: ora cambia solo `frequenza_hz`. Esito: `run_tests.sh` 20/20 verde, `gate_t1.sh` TUTTO OK. |
| Stato | [~] | (Claude 26/10: le due review incrociate sono FATTE e POSITIVE: `out/developer-junior/REVIEW_FORK1_26OTT.md` (07:14) e `out/developer/REVIEW_FORK1_26OTT.md` (07:20). Resta SOLO @capo: nel prossimo turno apre CP-007 per FORK1, lo chiude (gate ctest verde + 2 review positive), registra in REGISTRO_REVISIONI e aggiorna STATO_CORRENTE e STATI_PROPOSTI con FORK1 e DECO1; poi propone FORK1 [x]. Developer e developer-junior NON aspettano CP-007: passano a DECO1.) (Claude 25/10: codice e gate FORK1 FATTI, ctest 20/20. Restano SOLO: a) review incrociata, UN turno ciascuno: @developer rivede i gate 4, 5, 7 di `prova_gate.cpp`; @developer-junior rivede la modifica di `game.cpp` (corsia a scelta aperta) e la transizione tema; esito in `out/<ruolo>/REVIEW_FORK1_<data>.md`. b) @capo apre CP-007 per FORK1 e lo chiude quando le due review sono positive. c) @grafico elemento di passaggio al bivio: FACOLTATIVO, non blocca. Nessuno aspetta piu' `game.cpp`: chi trova FORK1 senza lavoro suo passa a DECO1.) |

---

## BLOCCO DECO1 — @grafico + @developer + @developer-junior — elementi decorativi di ogni livello (Andrea 22 ott; dopo FORK1, prima di RIV e prima di ogni prova su Pi 400/RetroPie)
| Campo | Valore |
|---|---|
| Richiesta di Andrea | Gli elementi decorativi di ogni livello (skyline e oggetti a bordo strada) devono essere nel gioco prima della prova su Raspberry Pi 400/RetroPie: senza, per chi testa su dispositivi esterni non e' un'esperienza di gioco completa. |
| Oggi (Claude 22/10, letto in `game.cpp`, `renderer.cpp`, `road.cpp`, `player.cpp`, `sponsor_loader.cpp`) | Il motore disegna solo i cartelloni sponsor (7 sponsor x 3 stili = 21 sprite raw: neon, atomico, foglie). `tema_grafico.skyline` e `tema_grafico.oggetti_bordo_strada` dei 6 `livello.json` sono id di catalogo che il motore non legge: il cielo e' un colore piatto. lv_05 e lv_06 usano oggetti presi in prestito da altri livelli, e `scenario_richiesto` e' "da fare a cura del grafico"; per cromo, spray e desktop non esiste uno stile di cartellone proprio (lv_05 usa atomico, lv_06 foglie). I cartelloni sono troppo fitti (uno ogni 1-4 segmenti) rispetto a D2b (~20-30% dei segmenti). G1 (catalogo, 100 PNG fuori elenco) non chiuso. |
| Compito | 1) @grafico: chiudere G1; per ognuno dei 6 livelli uno skyline e almeno 4 oggetti a bordo strada propri (nessun prestito), partendo da `scenario_richiesto` di ogni `livello.json` e dalla variante A (D6); stile di cartellone proprio per lv_04, lv_05, lv_06 (o riduzione degli sponsor per livello in `MAPPA_LIVELLI_SPONSOR.json`, a scelta del @grafico); conversione PNG->raw con `png_a_raw.py` (D10: B); ogni elemento nel catalogo con stato `pronto` o `approvato`. La lista degli oggetti la decide il @grafico; Andrea la vede nei risultati e puo' cambiarla. 2) @developer: il motore legge da `tema_grafico` di ogni livello: skyline disegnato sopra l'orizzonte (scorrimento lento in curva, parallasse leggero) e `oggetti_bordo_strada` piazzati a bordo strada, con posizioni da `livello.json` come i cartelloni (seme fisso, nessuna allocazione nel ciclo di gioco); densita' dei cartelloni riportata a ~20-30% dei segmenti. Collisione con gli oggetti decorativi: decide e documenta il @developer (solo oggetti marcati solidi, oppure nessuna). 3) @developer-junior: gate `decorazioni`: per ognuno dei 6 livelli skyline e oggetti risolti nel catalogo (nessun id mancante o in prestito), densita' cartelloni nell'intervallo, ms/frame entro la soglia con le decorazioni, screenshot PPM per livello (strada con oggetti e skyline); `check_catalogo_grafico.py` exit 0. |
| Input | `assets_originali/CATALOGO_GRAFICO.json`, `MAPPA_LIVELLI_SPONSOR.json`, `dati/livelli/*/livello.json` (`scenario_richiesto`, `tema_grafico`, `cartelloni_sponsor`), `src/render/renderer.cpp`, `src/game/game.cpp`, `src/runner/sponsor_loader.cpp`, `strumenti_py/png_a_raw.py` |
| Vincoli | Solo @grafico scrive catalogo e Libreria. Solo asset originali o con licenza libera, niente materiale Sega (quota open >= 50%). Core senza SDL; determinismo invariato (replay 2x IDENTICO). Il budget di frame non peggiora: obiettivo Pi 400 invariato (<= 12 ms/frame, massimo 20). |
| Prova di Andrea su Mac (23/10) | Gli elementi grafici del @grafico non si vedono per niente: solo i cartelloni. Da una tappa all'altra il gioco sembra uguale. I paletti bianchi/rossi ai lati (`Renderer::drawRoadsideMarkers`, colori fissi) non sono belli tutto il tempo: vanno sostituiti spesso con oggetti del tema del livello che abbiano senso (es. lampioni neon in lv_01, foglie giganti in lv_03), lasciando i paletti solo dove servono (curve). Al bivio i paletti sono gia' nascosti (Claude 23/10). Anche asfalto e striscia centrale hanno colori fissi (`pal_[2]`, `pal_[3]`, `pal_[1]`): aggiungere a `colori` di `livello.json` `asfalto`, `asfalto_alt`, `striscia`, letti da `leggiTemaDaFile` e inclusi nella transizione di tema. lv_01_neon non ha il campo `colori` (usa i default): il @grafico lo aggiunge. |
| Passaggio di consegne @grafico -> @developer (Andrea 23/10: la resa migliora solo se sviluppatore e grafico lavorano bene insieme) | Gli asset ci sono gia': `CATALOGO_GRAFICO.json` risolve ogni id di `tema_grafico` in un PNG (es. `scenario.lv_01_neon.skyline` -> `scenari/lv_01_neon/scenario_neon_01_varianteA/skyline.png`, `oggetto.lv_03_foglie.foglia_gigante` -> `.../oggetti/obj_foglia_gigante.png`). Accordo: 1) @grafico tiene ogni id di `tema_grafico` dei 6 livelli nel catalogo con `file` esistente e stato `pronto`/`approvato`, misure e punto di appoggio (base dell'oggetto) nel `manifest.json` della variante; ogni cambio di file o misura si annuncia nel diario del @developer. 2) @developer carica nel runner, come fa `sponsor_loader.cpp` con i cartelloni, gli sprite risolti dal catalogo per gli id di `tema_grafico` di ogni tappa (skyline + oggetti) e li passa al core prima di `init()`/`fineTappa()`; un id mancante si segnala nel log e al @grafico, mai un crash. 3) Primo passo piccolo e visibile: skyline e 2 oggetti per lv_01, lv_02, lv_03 nel gioco; screenshot in `Test area/` rivisti dal @grafico prima di passare agli altri 3 livelli. 4) In `src/runner/` si tocca solo il caricamento degli sprite del tema (accanto a `sponsor_loader.cpp`, gia' negli Input di questo blocco). |
| Passi piccoli (Claude 25/10: il 25/10 il @developer ha rinviato DECO1 per 6 turni come "multi-file non completabile in un turno"; un passo = un turno, ognuno compila e lascia ctest verde) | 2a) @developer: `Renderer` disegna uno skyline (sprite raw gia' in `sprite_raw/scenari/<livello>/skyline.sglraw`) sopra l'orizzonte, solo lv_01_neon, passato al core come gli sprite sponsor; nessun parallasse ancora. 2b) skyline per tutti i 6 livelli letto da `tema_grafico.skyline`, cambio con la tappa. 2c) 2 oggetti bordo strada per lv_01..lv_03 al posto di parte dei paletti (seme fisso). 2d) tutti gli oggetti dei 6 livelli + densita' cartelloni 20-30%. 2e) parallasse leggero in curva. Dopo ogni passo: `run_tests.sh` verde, una riga nel diario, screenshot in `Test area/`. Se un passo non entra nel turno, consegnarne meta' che compila, non rinviare tutto. @developer-junior: in attesa del passo 2a scrive il gate `decorazioni` (punto 3), che non dipende dal codice del developer. |
| Chi tocca adesso (Claude 26/10, superato: 2a-2e fatti; il 27/10 vale il passo 2f qui sotto) | Il prossimo passo era del @developer: passo 2a. Il @grafico NON deve consegnare altro per 2a: gli skyline raw ci sono gia' per tutti e 6 i livelli (`motore_nuovo/dati/sprite_raw/scenari/<livello>/skyline.sglraw`) e `game.cpp` legge gia' `skylineId_` da `tema_grafico`. Scrivere "DECO1 in attesa @grafico" e' sbagliato e conta come turno a vuoto. Gate `decorazioni` del @developer-junior FATTO (26/10 07:35) e ora in ctest (21 test); per ora controlla solo il catalogo: il @developer-junior lo estende con gli screenshot PPM per livello quando il passo 2a disegna lo skyline. @grafico: dopo il passo 2a rivede lo screenshot di lv_01_neon in `Test area/`. |
| Gate | `ctest` verde con `decorazioni`; screenshot dei 6 livelli in `UFFICIO/Officina/Test area/`; review incrociata (@developer rivede 3, @developer-junior rivede 2), @grafico rivede gli screenshot. @capo apre il checkpoint. DECO1 deve essere [x] prima della prova su Pi 400/RetroPie; la prova su Mac resta di Andrea. |
| Prova di Andrea su Mac (27/10, leggere per primo) | Gli skyline FUNZIONANO e migliorano molto il gioco, e la transizione di tema al bivio da' un bel feeling: non toccarli. Tanti cartelloni (vanno bene), ma **zero oggetti decorativi visibili**, e al bivio qualche elemento sembra sparso **in mezzo al percorso**. Cause trovate da Claude nel codice: a) gli sprite degli oggetti sono minuscoli (PNG 32x32 o 48x32, in gran parte trasparenti) e `caricaOggettiStrada` li disegna con `scale = 1.0`: un lampione e' un quarto di un cartellone 128x64, quindi quasi invisibile; b) cartelloni e oggetti sono a `worldX = +/-1600` su tutto il giro, anche negli ultimi `Road::BIVIO_SEG` segmenti: li' i due rami hanno centro a +/-`BIVIO_DIST` (1900) e larghezza 2400, quindi +/-1600 cade SOPRA la carreggiata di un ramo (e `collisioneOggettiVicini` conta i cartelloni con larghezza 760). I paletti al bivio sono gia' nascosti (`p.carr.doppia`), cartelloni e oggetti no. |
| Passo 2f (Claude 27/10, @developer, prima di PIST1) | 1) Nessun cartellone e nessun oggetto negli ultimi `Road::BIVIO_SEG` segmenti quando il bivio e' attivo (stessa regola dei paletti), oppure posizionati fuori dai rami (abs(x) >= `BIVIO_DIST` + `ROAD_WIDTH`/2 + 400); nessuna collisione con elementi che non si vedono. 2) Scala degli oggetti: campo `altezza_mondo` (o `scala`) per oggetto nel `manifest.json` della variante, default tale che un lampione sia alto circa 2 volte l'auto; senza campo usare `scala` 4.0. 3) Un oggetto ogni 2000 unita' invece di 4000, sempre fuori dalla carreggiata. 4) `run_tests.sh` verde, screenshot lv_01 e lv_03 in `Test area/` con oggetti visibili e un bivio pulito. @grafico: ridisegna gli oggetti alti (lampioni, pali, torri, piloni) in verticale, almeno 32x96, sagome piene e colorate (poca trasparenza); stessi nomi file, aggiorna misure nel manifest e annuncialo nel diario del @developer. |
| Stato | [~] | (Claude 27/10: codice @developer passi 2a-2e FATTO, `gate_t1.sh` 07:55 exit 0, lv_06 smoke OK 08:42. Restano solo: review incrociata (un turno ciascuno, esito in `out/<ruolo>/REVIEW_DECO1_<data>.md`), @grafico rivede gli screenshot in `out/developer-junior/` e `Test area/`, @capo apre e chiude il checkpoint. Nessuno aspetta altro codice DECO1: il @developer passa a R2.) |

---

## BLOCCO PIST1 — @developer + @developer-junior + @grafico — percorsi piu' vari: ponti, colline, chicane, autostrada, citta' (Andrea 23 ott, dopo la prova su Mac; dopo DECO1, prima di RIV)
| Campo | Valore |
|---|---|
| Richiesta di Andrea | Il lavoro e' decente; per renderlo interessante i percorsi devono variare come descritto nei documenti (ANDREA_REVIEW, priorita' 1): ponti, colline, curve e chicane come un vero circuito, tratti di autostrada e tratti cittadini. Oggi un livello dopo l'altro sembra uguale. |
| Oggi | `livello.json` `segmenti` ha solo `curva`, `collina`, `lunghezza`; `tracciato` usa i tratti di `dati/tracciati/tratti.json` (rettilinei e curve). Non esistono tipi di tratto con aspetto diverso (ponte, galleria, citta'), ne' larghezza strada variabile. |
| Compito | 1) @developer: nuovi tipi di tratto in `tratti.json`/`regole.json`: `chicane` (curva-controcurva stretta), `dosso`/`valle` (collina marcata), `ponte` (nessuna erba ai lati: acqua o vuoto del colore del tema, parapetti come oggetti a bordo strada), `autostrada` (strada piu' larga, 2 strisce, guardrail), `citta'` (strada piu' stretta, palazzi del tema come oggetti fitti ai lati). Larghezza strada per segmento (campo opzionale `larghezza`, default 1.0) raccordata senza scatti. 2) @developer: ogni livello usa almeno 3 tipi diversi coerenti col suo tema (es. lv_04_cromo autostrada + ponte, lv_06_desktop citta'); la durata stimata resta quella di oggi. 3) @grafico: oggetti per ponte (parapetto), autostrada (guardrail, cartelli di uscita inventati), citta' (facciate per tema), nel catalogo, convertiti in raw. 4) @developer-junior: gate `tratti`: ogni livello contiene i tipi dichiarati, nessun salto di larghezza/curva/collina oltre le soglie di `regole.json`, ms/frame entro la soglia, screenshot PPM di ogni tipo di tratto. |
| Input | `dati/tracciati/tratti.json`, `dati/tracciati/regole.json`, `dati/livelli/*/livello.json`, `src/road/road.cpp`/`road.h`, `src/render/renderer.cpp`, `assets_originali/DIREZIONE_ARTISTICA.md` |
| Vincoli | Core senza SDL; determinismo invariato (replay 2x IDENTICO). Gate `gara` e `bivio_*` restano verdi. Budget Pi 400 invariato (<= 12 ms/frame). Niente materiale Sega. |
| Gate | `ctest` verde con `tratti`; screenshot in `UFFICIO/Officina/Test area/`; review incrociata, @grafico rivede gli screenshot. @capo apre il checkpoint. La prova su Mac resta di Andrea. |
| Stato | [ ] |

---

## BLOCCO RIV — @developer + @developer-junior + @grafico — rivali veri e urto con sbandata (Andrea 22 ott; dopo F60, prima di R2)
| Campo | Valore |
|---|---|
| Decisione di Andrea | Scelta B (variazione rispetto all'OutRun originale): gli stessi 7 rivali corrono tutta la gara, ognuno sceglie i propri bivi e arriva al proprio finale; la classifica finale e' sul tempo totale di tutti. |
| Oggi | `game.cpp`: i rivali hanno un `progresso` globale e, dopo il bivio, ricompaiono tutti sulla tappa scelta dal giocatore (`ramo` serve solo a disegnarli sul bivio); la posizione e' per distanza. Sono rettangoli 26x18 (`renderer.cpp:177`). Un urto con un rivale fa come un incidente: ribaltamento e `speed = 0` (`game.cpp:743`). |
| Compito | 1) @developer: ogni rivale ha la sua sequenza di tappe da `percorso_gara.json` (scelta deterministica dal `seme`); nella tappa del giocatore si vedono solo i rivali che sono sulla stessa tappa; ogni rivale registra il tempo d'arrivo; la classifica finale ordina tutti per tempo totale (chi non arriva prima del tempo limite esce dalla classifica). 2) @developer: urto con un rivale = sbandata, non incidente: perdita di velocita' (es. -40%), spinta laterale e breve rotazione/scodata dell'auto (circa 1 s, in secondi), poi i controlli tornano al giocatore; il ribaltamento resta solo per ostacoli fissi e fuori strada prolungato. Anche il rivale urtato perde un po' di velocita'. 3) @grafico: sprite delle auto rivali (almeno 4 modelli/colori, viste da dietro e di lato per le curve, piu' un fotogramma di sbandata per l'auto del giocatore), registrate nel catalogo; il motore le usa al posto dei rettangoli. 4) @developer-junior: gate `gara` con classifica a tempo deterministica e gate `urto` (dopo l'urto: niente ribaltamento, velocita' > 0, controlli di nuovo attivi entro 1,5 s). |
| Gia' fatto da Claude (23/10) | Andrea: i rivali scappano come saette, impossibile riprenderli. Causa: `passo`/`passoMax` erano unita' per frame tarate a 50 Hz (540..594), mentre il giocatore a 60 Hz fa ~495 unita'/frame in punta: i rivali erano piu' veloci del 10-20%. Ora `passo` e' in unita'/secondo, punta 94-104% di quella del giocatore, accelerazione con costante di tempo 2 s, elastico continuo sul distacco (0.80..1.15, `RIVALE_ELASTICO_*`): il gruppo resta a pochi secondi dal giocatore e dopo un incidente si riprende in ~15 s. Seconda nota di Andrea (23/10): l'urto tra concorrenti (anche rivale contro rivale) deve avere una reazione credibile (punto 2); le auto dei rivali sembrano segnaposto e vanno sostituite (punto 3, priorita' alta per il @grafico); anche l'auto del giocatore va migliorata con un finto 3D alla OutRun: piu' fotogrammi (dritta, sterzo leggero/forte a sinistra e destra, salita/discesa) scelti in base a sterzo, curva e posizione sulla strada, fatti dal @grafico e scelti dal @developer in `Renderer::drawPlayerCar` al posto dei rettangoli. Restano da fare i punti 1-4; tarare con questi valori come base. |
| Prova di Andrea su Mac (27/10, leggere per primo) | Oggi se l'auto tocca un rivale si ribalta del tutto: va cambiato. Andrea vuole un **'bump' simpatico**: piccola spinta laterale e **rallentamento leggero** (circa -15/-20%, non -40%), nessun ribaltamento, controlli sempre attivi. I rivali sono migliorati rispetto a quando scappavano come saette (i valori di Claude del 23/10 restano la base), ma il loro comportamento deve essere piu' realistico. Codice di oggi: `Game::update` chiama `collisioneOggettiVicini()` e su qualsiasi contatto (anche `sprites_`, cioe' i rivali) mette `playerFlipping_ = true` e `speed = 0`. Grafica dei rivali ancora grezza: Andrea approva di usare sprite di altri modelli di auto al posto dei rettangoli (il @grafico ha gia' 7 sprite rivali nel catalogo). Piace l'auto che si alza a velocita' massima: non toccarla. Guida in generale decente. |
| Chi tocca adesso (Claude 27/10) | Ordine: questi due passi piccoli vengono PRIMA di PIST1 (dopo R2 1b-1c e DECO1 2f). a) @developer: urto con rivale = bump come sopra (separare il contatto con `sprites_` da quello con i cartelloni; ribaltamento solo per ostacoli fissi e fuori strada prolungato), costanti in secondi in `config.json`; @developer-junior: gate `urto` (nessun ribaltamento, velocita' > 0, controlli attivi). b) @developer: i rivali usano gli sprite del catalogo (almeno 4 modelli/colori diversi) invece dei rettangoli; il @grafico controlla che gli sprite siano `pronto` e ne aggiunge se mancano. c) @developer, comportamento piu' realistico in `aggiornaRivali` (oggi: velocita' costante anche in curva, solo 3 corsie fisse -450/0/450 scelte a caso ogni ~4 s, i rivali si sovrappongono tra loro e non reagiscono all'urto): rallentano in curva in base alla curvatura del segmento, ognuno con la sua bravura dal `seme`; cambio corsia morbido e solo per sorpassare o evitare; distanza minima tra rivali (chi raggiunge un altro rivale lo sorpassa di lato o rallenta); il rivale urtato perde anche lui un po' di velocita'. Tenere l'elastico e le punte di oggi. @developer-junior: nel gate `gara` controlla che nessun rivale si sovrapponga a un altro e che in curva rallentino. Il punto 1 (ogni rivale sceglie i propri bivi) resta dopo PIST1. |
| Vincoli | Non toccare `src/runner/`. Determinismo invariato (replay 2x IDENTICO). Niente grafica SEGA. |
| Gate | `ctest` verde con `gara` e `urto`; screenshot dei rivali e della sbandata in `UFFICIO/Officina/Test area/`; review incrociata, @grafico rivede gli sprite. @capo apre il checkpoint. |
| Stato | [~] | (Andrea 27/10: punti 2 e 3 anticipati prima di PIST1, vedi 'Chi tocca adesso') |

---

## BLOCCO FIN — @developer + @developer-junior + @grafico — finale animato a fine corsa (Andrea 22 ott; dopo R2 e RIV)
| Campo | Valore |
|---|---|
| Richiesta di Andrea | Corsa a bivi: partenza, bivio 1, scelta A o B, e cosi' via fino alla fine corsa, poi animazione di reward. I bivi ci sono gia' (`dati/percorso_gara.json`, `Game::fineTappa`, gate `gara` 4 combinazioni); oggi all'arrivo compare solo la bandiera a scacchi e l'auto va per inerzia. |
| Compito | 1) @developer: fase `Finale` tra arrivo e `Risultati` (nella macchina a stati di R2, in `src/game/`), durata in secondi, scena scelta dalla pista d'arrivo e dalla posizione nella classifica a tempo di RIV (primo / podio / fuori podio) letta da nuovo `dati/finali.json`; con tempo scaduto si salta il finale. 2) @grafico: una scena per arrivo (lv_04_cromo, lv_05_spray, lv_06_desktop, proposte in `docs/FLUSSO_GIOCO.md` sezione 4) con le varianti per posizione; finche' mancano, voci `mancante` nel catalogo e il motore usa un segnaposto. 3) @developer-junior: il gate `gara` arriva fino al finale per tutte le combinazioni di bivi e salva uno screenshot per arrivo. |
| Vincoli | Non toccare `src/runner/` (revisione di Andrea in corso). Determinismo invariato. Aggiungere tappe alla piramide resta possibile solo modificando `percorso_gara.json`. |
| Gate | `ctest` verde; screenshot dei 3 finali in `UFFICIO/Officina/Test area/`; review incrociata, @grafico rivede le scene. @capo apre il checkpoint. |
| Stato | [ ] |

---

## RIEPILOGO CORRENTE — tenuto da Andrea/Claude, un solo blocco (storico: out/capo/RICHIESTA_STORICO_17OTT.md)
Tappa 1. Build e gate reali OK (17 ott 14:00, `sh tools/gate_t1.sh`): 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame. Aperti: guida (v91 di Andrea), colline, wrap sprite, sprite veri nel gate, fps/temperatura su Pi 400.
Stati: C [x] · H [~] · P1 [~] · P1b [x] · P2 [x] · P3 [x] · M [~] · T1-RUN [~] · J [ ] · N [ ] · MD0 [-] · T5 [-].
Tocca a (Claude 25/10: FORK1 codice e gate fatti, ctest 20/20; restano solo le 2 review incrociate e CP-007 di @capo. Poi DECO1 a passi piccoli 2a-2e: @developer passo 2a, @developer-junior gate `decorazioni`. Dopo DECO1: R2 (intro, selezione, partita, risultati, ripartenza; Andrea 25/10), poi PIST1, RIV, FIN. T1-RUN resta [~]: Andrea ha risposto D19 = B, CP-006 riaperto. Regola anti-stallo: chi trova "nessun blocco eseguibile" per 2 turni di fila scrive UNA riga in `PER_CLAUDE.md` con cosa aspetta e da chi, poi solo la riga di diario.) · (storico Claude 23/10: F60 [x], G1 [x]; nessuno di questi blocchi aspetta il runner Mac di Andrea) · @capo checkpoint CP-006 per F60 · @developer + @developer-junior FORK1 ora (bivio fluido e transizione di tema), poi DECO1 (decorazioni dei 6 livelli, prima di ogni prova Pi), poi RIV, poi R2 (ciclo di gioco) (ordine test: prima Mac di Andrea, poi RetroPie/Pi 400; v. ANDREA_REVIEW T1-Q5) · @developer T1-RUN 2-3 e densita' cartelloni · @developer-junior T1-RUN 5-7 · @grafico G1 (catalogo grafico, su richiesta) · Andrea 8. Nessuna decisione aperta (D1-D9 chiuse), nessun PER_CLAUDE.md, FINITO non creabile.
