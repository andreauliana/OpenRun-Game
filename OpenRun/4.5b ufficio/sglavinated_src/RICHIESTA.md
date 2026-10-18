# FOGLIO RICHIESTA — V3 (open)

Uso: compila i blocchi che servono; poi a `capo` scrivi:
`leggi RICHIESTA.md ed esegui il prossimo blocco con Stato [ ]; aggiorna lo Stato e scrivi gli esiti in SCAMBIO.md`
Stato: [ ] da fare · [~] in corso · [x] fatto · [!] bloccato (-> /claude) · [-] in attesa della sua tappa (non iniziare)

Prima di tutto: `VISIONE.md` (idea, vincoli duri, architettura).
Nota v4.5 (17 ott): 5 ruoli (@capo, @sviluppatore, @collaudatore, @grafico, @ricercatore); i nomi dei blocchi sono stati aggiornati, stati e contenuti no. Regole in AGENTS.md (congelate).
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

## BLOCCO B0 — @collaudatore (pista C DECISA da Andrea; ora: mappa delle fonti. Il confronto con A2 e' facoltativo, resta come opzione chiusa)
| Compito | PRIMA: motore_nuovo/docs/FONTI.md = per ogni fonte in ../PIANO_V3.md (sez. «Decisione») cosa si prende come idea, cosa no, licenza verificata o «non dichiarata». POI, facoltativo: motore_nuovo/docs/CONFRONTO_PISTE.md: confronto tra A2 (mod di Cannonball) e C (motore nuovo): punti d'innesto per personaggi/fumetti/razzo/volo nel sorgente pubblico di Cannonball (solo analisi, niente copia di codice), effort, rischi, prestazioni su Pi, licenza |
| Input | ../RAPPORTO_FATTIBILITA.md sez. 1b, https://github.com/djyt/cannonball |
| Output | tabella + raccomandazione go/no-go per A2 e per C |
| Stato | [x] |
| Dopo | Andrea decide (PIANO_V3 blocco 3, decisione 1); i blocchi B-F partono dopo |

## BLOCCO B — @collaudatore
| Compito | scrivere motore_nuovo/docs/: ARCHITETTURA.md, FORMATI_DATI.md, BUILD.md (Mac e Pi 400), LICENZE.md |
| Input | ../PIANO_V3.md, ../RAPPORTO_FATTIBILITA.md, ../RISORSE_TECNICHE.md |
| Vincoli | C++17 + SDL2, strada a segmenti, dati in file, nessun limite artificiale, licenze verificate |
| Output | 4 documenti brevi (tabelle, comandi) |
| Stato | [x] | (consegnato T2, SCAMBIO voce 16)

## BLOCCO C — @sviluppatore
| Compito | prototipo in motore_nuovo/src/: nucleo senza dipendenze di piattaforma + runner SDL2 per test + core libretro; strada a segmenti con curve e salite, auto del giocatore, 50 fps |
| Input | motore_nuovo/docs/ |
| Vincoli | nessun codice copiato senza licenza libera; nessun asset Sega; interfacce per il gameplay |
| Gate | compila con cmake; @collaudatore misura fps |
| Stato | [x] | (sintassi 30/30 SPDX MIT; build e gate reali OK il 17 ott 14:00, vedi H e T1-RUN)

## BLOCCO D — @sviluppatore
| Compito | SPEC_GAMEPLAY.md; dati/frasi.json da gianni12.py; modulo gameplay (Gianni/Gesu', escalation, inattivita') |
| Input | HANDOVER.md, SPEC_personaggi_v74.md, OPENCODE_REFERENCE_v74.json |
| Vincoli | niente codice 68000 ne' grafica/audio Sega; limiti hardware della hack non si portano |
| Stato | [x] | (SPEC_GAMEPLAY.md + src/gameplay/ + 156 fumetti in dati/frasi.json; SCAMBIO 23)

## BLOCCO E — @ricercatore
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

## BLOCCO H — @collaudatore (+ Andrea sul Pi)
| Compito | build, replay deterministico (hash identico su due esecuzioni), tempo di frame, fps e temperatura sul Pi (ARCHITETTURA_CHECK.md) |
| Stato | [~] | (17 ott 14:00 gate reale OK: 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame, exit 0. Mancano fps/temperatura su Pi 400 e la v91 di Andrea sulla guida; [x] solo con T1-RUN punto 8. Dettaglio: out/collaudatore/ESITO_GATE_T1_17OTT_1400.md)

## BLOCCO I — @collaudatore
| Compito | controllo di codice, licenze, asset, regole di AGENTS.md |
| Stato | [x] | (CHECK 2 ore 4, esito in SCAMBIO.md voce 20; B/C/K/M confermati [x], correzioni segnalate a @sviluppatore prima di H)

## BLOCCO J — Escalation a Claude
Quando: 2 tentativi falliti o fine blocco importante. In OpenCode: `/claude` (crea PER_CLAUDE.md).
Poi: Claude app -> `leggi PER_CLAUDE.md e SCAMBIO.md e dai il tuo parere`. (Con `ciclo.sh` lo fa il comando `claude`.)
| Stato | [ ] |

## MODELLO — copia per un nuovo blocco
```
## BLOCCO K — @sviluppatore (dopo B0; in parallelo a C)
| Compito | progettare 3 livelli di prova: motore_nuovo/dati/livelli/<id>/livello.json + riga in motore_nuovo/docs/LIVELLI.md |
| Input | VISIONE.md, motore_nuovo/docs/ARCHIVIO_V87.md (ritmo degli show), DIREZIONE_ARTISTICA.md |
| Output | 3 livelli, ciascuno con `stile` diverso scelto da DIREZIONE_ARTISTICA.md (tema, lunghezza, seme, traffico, difficolta' facile, punti degli show, moduli fx, scenario richiesto) |
| Stato | [x] | (consegnato T2, SCAMBIO voce 18)

## BLOCCO L — @grafico (dopo K)
| Compito | per ciascuno dei 3 livelli: 3 varianti di sfondo + oggetti a bordo strada + palette, poi produzione dopo la scelta di Andrea |
| Output | assets_originali/scenari/<id>/ con manifest.json; CREDITI.md |
| Stato | [x] | (@grafico, voce SCAMBIO 34 + voce 37; 9 cartelle variante piene, manifest per livello, 3 righe CREDITI.md, licenza CC0-1.0, originale=true)

## BLOCCO FED — @collaudatore (con B)
| Compito | compilare motore_nuovo/docs/FEDELTA.md (10 tratti, prova per ciascuno) e proporre in ANDREA_REVIEW.md D3 il numero di tappe/bivi |
| Stato | [x] | (consegnato con BLOCCO B, SCAMBIO voce 16: FEDELTA.md 10 tratti, punteggio 50.0% soglia; D3 default 5 tappe/bivi, 3 livelli di prova)

## BLOCCO CONS — @sviluppatore (conservazione, subito dopo B0)
| Compito | seguire motore_nuovo/docs/ARCHIVIO_V87.md: estrarre i 156 fumetti e le tabelle show in JSON, scrivere la logica degli show in .md; aggiornare CATALOGO_PERSONAGGI.md |
| Vincolo | sola lettura sulla v87; niente va perso |
| Stato | [x] |

## BLOCCO M — @grafico (in parallelo a K)
| Compito | 3 varianti di: font pixel, lettering titolo «Sglavinated Edition», menu principale e HUD (320x224); poi produzione dopo la scelta di Andrea |
| Input | DIREZIONE_ARTISTICA.md, assets_originali/ricerca/ (font liberi da @ricercatore) |
| Output | assets_originali/ui/ + motore_nuovo/dati/ui/{menu,hud}.json + CREDITI.md |
| Stato | [~] | (consegnato T2, SCAMBIO 15; manifest in "bozza" finche' Andrea non approva i PNG B/N; D8 "tempo" da aggiungere a hud.json)

## BLOCCO N — @collaudatore (solo dopo che C gira; facoltativo)
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

## BLOCCO P1 — @sviluppatore (PRIORITA' MASSIMA, da Andrea 16 ott: primo livello giocabile)
| Nota | Claude ha compilato motore_nuovo su Linux: core e runner compilano, sono deterministici, ~0,5 ms/frame. A schermo pero' solo strada grigia dritta + rettangolo. Ora serve un livello giocabile, non documenti. |
| Compito | 1) player.cpp: accelera con Pad::A (oggi solo Up: col pad l'auto resta ferma), freno su B o Down. 2) CMakeLists.txt: find_package(SDL2_image) invece di SDL2_IMAGE (oggi i PNG non si caricano mai). 3) Game::init() legge i segmenti della pista da livello.json (dopo P2), curve visibili. 4) sprite veri da assets_originali: auto giocatore, traffico, cartelloni, bordo strada, skyline (niente rettangoli). 5) con @sviluppatore (P3): disegnare Gianni, Gesu', UFO e fumetto (oggi in game.cpp lo show e' scartato con (void)show). |
| Gate | build cmake ok; 3 screenshot in out/sviluppatore/ con pista curva, traffico, cartelloni, un personaggio con fumetto |
| Stato | [~] | (punti 1-3 e 5 fatti; punto 4 sprite veri ancora aperto = T1-RUN 4; build e 3 screenshot strada OK in out/collaudatore/)

## BLOCCO P2 — @sviluppatore (per P1)
| Compito | aggiungere in ogni livello.json la lista "segmenti" (curva, collina, lunghezza) con curve abbastanza forti da vedersi; documentare il formato in motore_nuovo/docs/FORMATI_DATI.md |
| Stato | [x] |
| Nota | Segmenti (20 per livello) aggiunti in tutti i 3 livello.json (SCAMBIO voce 75). FORMATI_DATI.md NON modificabile qui: write negato (regola 21, permission denied). Documentazione da completare appena il write è sbloccato; non blocca P2 (dati già presenti).

## BLOCCO P3 — @sviluppatore (per P1)
| Compito | interfaccia di disegno per gli show: cosa disegnare a ogni frame (personaggio, posa, posizione, fumetto, UFO) da DatiShow; aiutare @sviluppatore a collegarla al renderer |
| Stato | [x] |
| Nota | src/gameplay/disegno_show.h/.cpp + testi_frasi.h/.cpp + DISEGNO_SHOW.md consegnati (SCAMBIO voce 77, MIT). Collegamento in game.cpp (sostituisce (void)show) e nel Renderer spetta a @sviluppatore dentro P1 (voce 71/1-2).

## BLOCCO MD0 — @sviluppatore — TAPPA 1: la ROM parte e si pilota
| Compito | ROM Mega Drive minima e originale con SGDK (MIT): all'avvio va subito in gara. Strada che scorre (strisce di corsia), auto che accelera (A/Su), frena (B/Giu') e sterza (sinistra/destra). Niente menu, logo o testi: arrivano nelle tappe 2-3 (`../TAPPE.md`). Sorgenti in `motore_nuovo/export_md/`, ROM in `out/coder/sglavinated_test.md` |
| Input | `../TAPPE.md`, `motore_nuovo/docs/FATTIBILITA_ROM_MD.md`, logica di guida da `motore_nuovo/src/player/` e `src/road/` (stessi valori di accelerazione e freno) |
| Vincoli | Grafica provvisoria (rettangoli e colori pieni). Niente codice, grafica, musica o logo SEGA. Build solo sul Mac o sul Pi 400 (regola 21) |
| Gate | la ROM parte su emulatore (Genesis Plus GX) e si guida per 5 minuti senza crash; 2 screenshot in `out/coder/` |
| Stato | [-] (sospeso: prima il C++, scelta di Andrea 17 ott) |

## BLOCCO P1b — @sviluppatore — TAPPA 1: niente collisioni fantasma
| Compito | In `src/game/game.cpp` il traffico segnaposto (`spawnTraffic`) e' invisibile, sta sempre a distanza fissa davanti al giocatore (x = +-800) e la collisione scatta gia' con |dx| < 700: sterzando oltre x = 100 il gioco vede un incidente continuo (causa probabile dell'auto che sparisce, SCAMBIO voce 67-68). Per la tappa 1: disattiva traffico segnaposto e relative collisioni (flag), lascia solo strada e auto. Fixa anche il clamp di playerX (auto sparisce a playerX=125 dopo 60 frame di sterzata, voce 67). |
| Gate | prova headless o runner: sterzando a destra e sinistra per 10 s l'auto resta visibile e nessun crash viene segnalato |
| Stato | [x] | (codice SCAMBIO 86/87; gate headless 17 ott PASSATO: sterzo sx/dx, offroad f.476, 30000 frame senza crash)

## BLOCCO T1-RUN — @sviluppatore + @collaudatore — prossimo run: implementazione e test (da Claude 17 ott 14:00)
> PROTETTO: titolo e Stato di T1-RUN li gestisce Andrea.
| Campo | Valore |
|---|---|
| Premessa | Il motore COMPILA e il gate headless PASSA. **Per compilare e provare: un solo comando `sh tools/gate_t1.sh` (o `... soak`) dalla radice sglavinated_src/, senza pipe/`2>&1`/`;`/`echo`/`which`** (AGENTS.md, Build e test). Il vecchio "shell negata" era un errore di diagnosi: i comandi composti vengono negati, quelli semplici passano. Non riscrivere "build non eseguibile" se non e' negato proprio quel comando. |
| 0 Strada (fatto da Claude) | Corretti in `render/renderer.cpp`: interpolazione del trapezio invertita (lontano/vicino) che spezzava i bordi; strisce non ancorate al mondo. Ora bande alternate per indice segmento (asfalto, erba, rumble bianco/magenta, tratteggio di corsia) e `road.cpp` proietta con z continua dentro il segmento (scorrimento fluido). Sprite/cartelloni seguono la curva della strada. Verificare con gli screenshot di `out/collaudatore/<livello>/`. |
| 1 @sviluppatore | **Acceleratore** (fatto da Claude, PR #2, 17 ott): ACCEL 110, BRAKE 120, gas rilasciato COAST 0.995 in player.h; speed@60 da 33,76 a 100,83, replay 2x IDENTICO. Resta: Andrea conferma la sensazione sul runner (punto 8); eventuali ritocchi un parametro alla volta. |
| 2 @sviluppatore | **Colline**: `Road::project` integra due volte la collina (dy += hill; y += dy*len), quindi la strada sale nel cielo e la camera non ne segue la quota (`camY` fissa). Portare l'altezza a quota assoluta per segmento e camera = quota sotto l'auto + CAM_HEIGHT. Stessa cosa per l'ascissa: usare x relativa al punto sotto l'auto. |
| 3 @sviluppatore | **Sprite**: `drawSprites` usa worldZ assoluto contro playerZ avvolto sul giro: vicino al punto di wrap gli sprite spariscono. Calcolare zRel con modulo totalLen. |
| 4 @sviluppatore | **Sprite veri** (fatto da Claude, PR #2): gli sprite erano scalati per ~0,001 e larghi 0 pixel, mai visibili; ora `drawSprites` usa la proiezione della strada (`Renderer::SPRITE_WORLD_PX`) con ritaglio allo schermo, cartelloni a +/-1600. Resta: densita' dei cartelloni in `livello.json` (oggi troppo fitti, uno ogni 1-4 segmenti) e caricamento degli sprite dal catalogo grafico (blocco G1). |
| 4b @collaudatore | Fatto da Claude (PR #2): `prova_gate.cpp` carica i 21 `.sglraw` prima di `init()`; screenshot con cartelloni veri in `out/collaudatore/<livello>/`. |
| 5 @collaudatore | Aggiungere casi a prova_gate: wrap di playerZ oltre totalLen_, inattivita' 10 s -> show/fumetto visibile in screenshot. `sh tools/gate_t1.sh soak` ad ogni modifica del coder. |
| 6 Libreria asset | @collaudatore (o chi genera asset con Python) lancia `python3 motore_nuovo/strumenti_py/indice_assets.py` dopo ogni generazione; il primo indice (da elenco cartelle) e' gia' in assets_originali/INDICE_ASSET.json/.md, senza dimensioni PNG ne' duplicati veri finche' lo script non gira sui file reali: produce `assets_originali/INDICE_ASSET.json/.md` (cosa esiste, dimensioni, duplicati, segnaposto, lacune). Gli agenti grafici consultano l'indice prima di creare. Lacune note: traffico, sprite personaggi (Gianni, Gesu'), UFO. |
| 7 @collaudatore | Rivedere SOLO: renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh. |
| 8 Andrea | Provare `sglavinated_runner` sul Mac (SDL2) e registrare in `ANDREA_REVIEW.md` l'esito della guida; misurare fps/temperatura su Pi 400 (`vcgencmd measure_temp`, `get_throttled`). Questo chiude H solo dopo aver registrato la prova. |
| Fuori scope | MD0, T5 (traffico/collisioni/razzo), N, M (finche' Andrea non approva i PNG). Non promuovere H/P1 senza i punti 4 e 8. |
| Gate | `sh tools/gate_t1.sh soak` exit 0; screenshot con cartelloni veri e strada con bande allineate; v91 di Andrea. |
| Stato | [~] | (in esecuzione: @sviluppatore punti 1-4, @collaudatore punti 5-6; build e gate OK 17 ott 14:51, quota open 99,8%; v91 di Andrea pendente. Dettaglio: SCAMBIO 1144)

## BLOCCO G1 — @grafico — catalogo unico delle risorse grafiche (da Andrea e Claude 17 ott)
| Compito | Tenere `assets_originali/CATALOGO_GRAFICO.json` come unico elenco delle risorse grafiche: un elemento per riga con `id`, `tipo`, `stato` (mancante, bozza, in_costruzione, pronto, approvato, scartato), `file`, `raw` (versione per il motore) e `livelli`. Claude ha creato la prima versione (52 elementi: 21 cartelloni, 3 auto, skyline e oggetti di variante A per i 3 livelli, UI variante B, font; mancano traffico, Gianni, Gesu', UFO). 1) Catalogare i 100 PNG oggi fuori elenco (varianti B/C, scenari doppi `scenario_<n>_01_*` e `scenario_lv_*`): per ognuno `scartato` o un elemento nuovo, senza cancellare file. 2) Ogni sprite nuovo entra nel catalogo con il suo stato prima di essere usato. 3) Dopo ogni modifica: `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` (exit 0). |
| Input | `assets_originali/CATALOGO_GRAFICO.json`, `INDICE_ASSET.md`, `MAPPA_LIVELLI_SPONSOR.json`, `ANDREA_REVIEW.md` (D2, D4-D7) |
| Vincoli | Solo @grafico scrive il catalogo. Il motore e i test usano solo `pronto` e `approvato` (`in_costruzione` solo nei test). Niente sottocartelle nuove senza voce nel catalogo. |
| Gate | `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0" |
| Stato | [ ] |

## BLOCCO T5 — @sviluppatore — TAPPA 5: altri veicoli, collisioni, razzo
| Compito | 1) @sviluppatore: in ogni livello.json aggiungi `traffico_spawn` (segmento, corsia, tipo, velocita') e `rivali` (2-3 auto che corrono la gara, con velocita' e stile di guida), deterministici dal `seme`. 2) @sviluppatore: veicoli con posizione, corsia e velocita' propri (si raggiungono e si sorpassano); rivali con IA semplice (seguono la strada, cambiano corsia); auto del giocatore pseudo-3D: la telecamera segue la posizione sulla strada e lo sprite cambia posa in sterzata; collisioni con traffico e rivali (rallentamento o testacoda). 3) Razzo: tasto secondario (X sul pad, Spazio sulla tastiera; B resta il freno), usa `moduli_fx.razzo_B` del livello, con ricarica |
| Input | `../TAPPE.md`, `dati/livelli/*/livello.json` (campo `traffico`), `src/gameplay/`, `src/game/game.cpp` |
| Vincoli | Meccaniche di genere, niente codice o grafica SEGA. Determinismo: stesso seme e input = stesso hash. Rettangoli colorati finche' non ci sono sprite |
| Gate | in 2 minuti di guida: sorpassi traffico, almeno un rivale ti supera o lo superi, una collisione rallenta, il razzo spinge e si ricarica |
| Stato | [-] (tappa 5) |

---

## RIEPILOGO CORRENTE — tenuto da Andrea/Claude, un solo blocco (storico: out/capo/RICHIESTA_STORICO_17OTT.md)
Tappa 1. Build e gate reali OK (17 ott 14:00, `sh tools/gate_t1.sh`): 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame. Aperti: guida (v91 di Andrea), colline, wrap sprite, sprite veri nel gate, fps/temperatura su Pi 400.
Stati: C [x] · H [~] · P1 [~] · P1b [x] · P2 [x] · P3 [x] · M [~] · T1-RUN [~] · J [ ] · N [ ] · MD0 [-] · T5 [-].
Tocca a: @sviluppatore T1-RUN 2-3 e densita' cartelloni · @collaudatore T1-RUN 5-7 · @grafico G1 (catalogo grafico, su richiesta) · Andrea 8. Nessuna decisione aperta (D1-D9 chiuse), nessun PER_CLAUDE.md, FINITO non creabile.
