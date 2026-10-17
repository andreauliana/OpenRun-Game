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

## BLOCCO H — @collaudatore (+ Andrea sul Pi)
| Compito | build, replay deterministico (hash identico su due esecuzioni), tempo di frame, fps e temperatura sul Pi (ARCHITETTURA_CHECK.md) |
| Stato | [~] | (17 ott 14:00 gate reale OK: 3 livelli, replay 2x IDENTICO, 0,09 ms/frame, soak 30000 frame, exit 0. Mancano fps/temperatura su Pi 400 e la v91 di Andrea sulla guida; [x] solo con T1-RUN punto 8. Dettaglio: out/collaudatore/ESITO_GATE_T1_17OTT_1400.md)

## BLOCCO J — Escalation a Claude
Quando: 2 tentativi falliti o fine blocco importante. In OpenCode: `/claude` (crea PER_CLAUDE.md).
Poi: Claude app -> `leggi PER_CLAUDE.md e SCAMBIO.md e dai il tuo parere`. (Con `ciclo.sh` lo fa il comando `claude`.)
| Stato | [ ] |


## BLOCCO P1 — @sviluppatore (PRIORITA' MASSIMA, da Andrea 16 ott: primo livello giocabile)
| Nota | Claude ha compilato motore_nuovo su Linux: core e runner compilano, sono deterministici, ~0,5 ms/frame. A schermo pero' solo strada grigia dritta + rettangolo. Ora serve un livello giocabile, non documenti. |
| Compito | 1) player.cpp: accelera con Pad::A (oggi solo Up: col pad l'auto resta ferma), freno su B o Down. 2) CMakeLists.txt: find_package(SDL2_image) invece di SDL2_IMAGE (oggi i PNG non si caricano mai). 3) Game::init() legge i segmenti della pista da livello.json (dopo P2), curve visibili. 4) sprite veri da assets_originali: auto giocatore, traffico, cartelloni, bordo strada, skyline (niente rettangoli). 5) con @sviluppatore (P3): disegnare Gianni, Gesu', UFO e fumetto (oggi in game.cpp lo show e' scartato con (void)show). |
| Gate | build cmake ok; 3 screenshot in out/coder/ con pista curva, traffico, cartelloni, un personaggio con fumetto |
| Stato | [~] | (punti 1-3 e 5 fatti; punto 4 sprite veri ancora aperto = T1-RUN 4; build e 3 screenshot strada OK in out/collaudatore/)

## BLOCCO MD0 — @sviluppatore — TAPPA 1: la ROM parte e si pilota
| Compito | ROM Mega Drive minima e originale con SGDK (MIT): all'avvio va subito in gara. Strada che scorre (strisce di corsia), auto che accelera (A/Su), frena (B/Giu') e sterza (sinistra/destra). Niente menu, logo o testi: arrivano nelle tappe 2-3 (`../TAPPE.md`). Sorgenti in `motore_nuovo/export_md/`, ROM in `out/coder/sglavinated_test.md` |
| Input | `../TAPPE.md`, `motore_nuovo/docs/FATTIBILITA_ROM_MD.md`, logica di guida da `motore_nuovo/src/player/` e `src/road/` (stessi valori di accelerazione e freno) |
| Vincoli | Grafica provvisoria (rettangoli e colori pieni). Niente codice, grafica, musica o logo SEGA. Build solo sul Mac o sul Pi 400 (regola 21) |
| Gate | la ROM parte su emulatore (Genesis Plus GX) e si guida per 5 minuti senza crash; 2 screenshot in `out/coder/` |
| Stato | [-] (sospeso: prima il C++, scelta di Andrea 17 ott) |

## BLOCCO T1-RUN — @sviluppatore + @collaudatore — prossimo run: implementazione e test (da Claude 17 ott 14:00)
> PROTETTO: titolo e Stato di T1-RUN li gestisce Andrea.
| Campo | Valore |
|---|---|
| Premessa | Il motore COMPILA e il gate headless PASSA. **Per compilare e provare: un solo comando `sh tools/gate_t1.sh` (o `... soak`) dalla radice sglavinated_src/, senza pipe/`2>&1`/`;`/`echo`/`which`** (AGENTS.md regola 26). Il vecchio "shell negata" era un errore di diagnosi: i comandi composti vengono negati, quelli semplici passano. Non riscrivere "build non eseguibile" se non e' negato proprio quel comando. |
| 0 Strada (fatto da Claude) | Corretti in `render/renderer.cpp`: interpolazione del trapezio invertita (lontano/vicino) che spezzava i bordi; strisce non ancorate al mondo. Ora bande alternate per indice segmento (asfalto, erba, rumble bianco/magenta, tratteggio di corsia) e `road.cpp` proietta con z continua dentro il segmento (scorrimento fluido). Sprite/cartelloni seguono la curva della strada. Verificare con gli screenshot di `out/collaudatore/<livello>/`. |
| 1 @sviluppatore | **Guida (v90 punti 1-3)**: cifre misurate: v=33.8@f60, 82@f170, 160@f450 (MAX_SPEED 220); fuoristrada: la velocita' crolla da ~160 a ~6 in pochi frame (troppo brusco -> tarare). Tuning in player.cpp, un parametro alla volta, poi `sh tools/gate_t1.sh` (replay 2x deve restare IDENTICO). |
| 2 @sviluppatore | **Colline**: `Road::project` integra due volte la collina (dy += hill; y += dy*len), quindi la strada sale nel cielo e la camera non ne segue la quota (`camY` fissa). Portare l'altezza a quota assoluta per segmento e camera = quota sotto l'auto + CAM_HEIGHT. Stessa cosa per l'ascissa: usare x relativa al punto sotto l'auto. |
| 3 @sviluppatore | **Sprite**: `drawSprites` usa worldZ assoluto contro playerZ avvolto sul giro: vicino al punto di wrap gli sprite spariscono. Calcolare zRel con modulo totalLen. |
| 4 @sviluppatore | **Sprite veri nel gate headless**: `setSponsorSprites` e' chiamato solo dal runner SDL2_image, quindi prova_gate disegna rettangoli. Proposta: caricatore PNG minimo interno al core o conversione PNG->raw in `strumenti_py` (no nuove dipendenze, licenza MIT/CC0). Poi 3 screenshot con cartelloni veri. |
| 5 @collaudatore | Aggiungere casi a prova_gate: wrap di playerZ oltre totalLen_, inattivita' 10 s -> show/fumetto visibile in screenshot. `sh tools/gate_t1.sh soak` ad ogni modifica del coder. |
| 6 Libreria asset | @collaudatore (o chi genera asset con Python) lancia `python3 motore_nuovo/strumenti_py/indice_assets.py` dopo ogni generazione; il primo indice (da elenco cartelle) e' gia' in assets_originali/INDICE_ASSET.json/.md, senza dimensioni PNG ne' duplicati veri finche' lo script non gira sui file reali: produce `assets_originali/INDICE_ASSET.json/.md` (cosa esiste, dimensioni, duplicati, segnaposto, lacune). Gli agenti grafici consultano l'indice prima di creare. Lacune note: traffico, sprite personaggi (Gianni, Gesu'), UFO. |
| 7 @collaudatore | Rivedere SOLO: renderer.cpp/.h, road.cpp, prova_gate.cpp, game.h, tools/gate_t1.sh, regola 26 di AGENTS.md. |
| 8 Andrea | Provare `sglavinated_runner` sul Mac (SDL2) e scrivere in FEEDBACK_ANDREA.md v91 se la guida ora e' OK; misurare fps/temperatura su Pi 400 (`vcgencmd measure_temp`, `get_throttled`). Questo chiude H. |
| Fuori scope | MD0, T5 (traffico/collisioni/razzo), N, M (finche' Andrea non approva i PNG). Non promuovere H/P1 senza i punti 4 e 8. |
| Gate | `sh tools/gate_t1.sh soak` exit 0; screenshot con cartelloni veri e strada con bande allineate; v91 di Andrea. |
| Stato | [~] | (in esecuzione: @sviluppatore punti 1-4, @collaudatore punti 5-6; build e gate OK 17 ott 14:51, quota open 99,8%; v91 di Andrea pendente. Dettaglio: SCAMBIO 1144)

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
Tocca a: @sviluppatore T1-RUN 1-4 · @collaudatore T1-RUN 5-6 · @collaudatore 7 · Andrea 8. Nessuna decisione aperta (D1-D9 chiuse), nessun PER_CLAUDE.md, FINITO non creabile.
