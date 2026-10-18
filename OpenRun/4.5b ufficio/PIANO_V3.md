# PIANO V3 (open) — Outrun Sglavinated Edition

> Piano originario V3: la copia di test corrente e' `Outrun Sglavinated Edition v4 (OpenWay)`; i nomi interni V3 restano invariati. Visione e vincoli duri: `sglavinated_src/VISIONE.md`. Squadra attuale: 12 agenti in `sglavinated_src/.opencode/agents/`; scaletta e fattibilita' delle novita': vedi `SCALETTA.md`. Istruzioni per Andrea: `COSA_FA_L_UMANO.md`.

Aggiornato: 16 ott 2026. Ogni blocco e' indipendente: leggi solo quello che ti serve.

---
## BLOCCO 1 — Obiettivo e stato
| Voce | Stato |
|---|---|
| Hack Mega Drive v87 (v88 in corso nella v2) | funziona sul Pi 400 (lr-genesis-plus-gx, PAL) |
| Problema | tetto hardware: ROM 4 MB, 156 fumetti, budget VBlank, 64 KB RAM -> nuove funzioni (auto che vola, cannone) fanno sbandare i test |
| Obiettivo V3 (open) | valutare una versione libera (senza materiale Sega), con asset nuovi, su Raspberry Pi 400 |
| Natura di V3 | progetto nuovo; la v2 resta degli altri operatori |

---
## BLOCCO 2 — Decisioni prese
1. Cartella di lavoro per questa build di test: `Desktop/Outrun Sglavinated Edition v4 (OpenWay)`; la v2 non si tocca. La cartella v3 precedente resta separata.
2. Dentro c'e' una copia dell'ultima versione (v87) in `sglavinated_src/`, senza ROM Sega e senza `.gen`.
3. Sistema multi-agente su OpenCode con modelli gratuiti; Claude (abbonamento) a mano per i casi difficili.
4. Canale di scambio: `sglavinated_src/SCAMBIO.md`.
5. Asset della versione libera: solo originali o con licenza libera (niente cloni di materiale Sega, nemmeno "simili").
6. Stile: sci-fi + lofi hip hop + jungle (`assets_originali/DIREZIONE_ARTISTICA.md`).
7. PS1 e console 3D: scartate come bersaglio.

---
## BLOCCO 3 — Decisioni aperte (tocca ad Andrea)
| # | Domanda | Quando |
|---|---|---|
| 1 | **DECISO da Andrea il 2026-10-15: pista C** (motore nuovo libero, migliori idee da piu' fonti, vedi sotto). Opzioni valutate: Pista C o A2 (mod di Cannonball: rapida, ma non libera, con ROM dell'utente e solo non commerciale) o restare sulla hack Mega Drive? | dopo il confronto del blocco B0 |
| 2 | Controllare `Resources` di ZgzInfinity/OutRun per la scorciatoia B | prima della fase 1 |
| 3 | Chiudere l'hack alla v87/v88 o continuare a estenderla? | dopo i test della v88 |
| 4 | Modelli gratuiti: i nomi funzionano? (`opencode models`) | primo avvio |
| 5 | Consultare un legale prima di distribuire la hack (anche allo sponsor) | prima di ogni distribuzione oltre l'uso privato |

---
## BLOCCO 4 — Le due piste
| | A: motore nativo su Pi | B: ROM Mega Drive riscritta (.md) |
|---|---|---|
| Idea | motore C/C++ su Pi 400, asset sostituiti | portare un motore C++ su ROM con un compilatore |
| Limiti | nessun tetto 4 MB / 64 KB | RAM 64 KB, 7,6 MHz: un motore arcade non ci sta |
| Strumenti | gcc, SDL/libretro | gcc m68k (es. SGDK); un compilatore Python non basta |
| Licenza | Cannonball: custom, vieta uso commerciale, richiede ROM arcade -> non libero | idem |
| Parere | piu' realistica | da scartare per il motore completo |
Criteri e prove su Pi 400: `ARCHITETTURA_CHECK.md`.
**Esito della verifica (15 ott): vedi `RAPPORTO_FATTIBILITA.md`.** Raccomandata la pista C (motore nuovo C++/SDL2); alternativa B (base GPL ZgzInfinity/OutRun con tutti gli asset sostituiti); Cannonball solo come riferimento tecnico; traduzione in ROM Mega Drive scartata.

---
## BLOCCO 5 — Squadra agenti (configurazione iniziale, riferimento storico)
| Agente | Modello (iniziale) | Scrive | Compito |
|---|---|---|---|
| capo | opencode/kimi-k2.5-free | solo *.md | coordina, delega, aggiorna HANDOVER/TODO/SCAMBIO |
| coder | opencode/nemotron-3.5-super-vl-free | tools/patch_z.py | patch 68000 |
| testi | opencode/nemotron-3.5-super-vl-free | tools/gianni12.py | fumetti Gianni/Gesu' |
| asset | opencode/big-pickle | assets_originali/ | asset originali + risorse a licenza libera |
| debugger | opencode/big-pickle | nessuno | gate di test, cause dei bug |
| reviewer | opencode/minimax-m2.1-free | nessuno | controllo prima di dare buona una build |
| opensource | opencode/nemotron-3.5-super-vl-free | nessuno | motori, licenze, elenco asset da sostituire |
Assegnazione a tentativi: cambia la riga `model:` nel file dell'agente se rende poco. Reviewer e coder usano modelli diversi di proposito.

---
## BLOCCO 6 — Flusso di lavoro
1. Un task = una build + un test + un backup (`tools/patch_z.py.bak`).
2. Gate: `cd lr && python3 final.py ../out.gen 16000 left|right|wild` -> `vsbad 0`, `wrong 0`, `repeat 0`.
3. Passaggi tra agenti in `SCAMBIO.md` (voce APERTA/CHIUSA).
4. Escalation: dopo 2 tentativi falliti, voce `-> CLAUDE [APERTA]`; Andrea apre Claude app: "leggi SCAMBIO.md e risolvi l'ultima voce aperta per te".
5. ChatGPT web (opzionale): Andrea incolla la voce, riporta la risposta nel file.

---
## BLOCCO 7 — Asset e specifiche
- Specifiche tecniche del motore (dimensioni tile, formato dati, colori, struttura segmenti di strada): si possono usare.
- Contenuto (sprite, scenari, tracciati, musiche, nomi, loghi): nuovo. Fonti ammesse: direzione di Andrea, generazione procedurale (Python/Pillow), risorse CC0/CC-BY con crediti in `assets_originali/CREDITI.md`, tracciati generati da zero (seed).
- Musica: solo tracker di Andrea (.xm).
- Gianni, Gesu', UFO: di Andrea, restano.
- I modelli di testo non disegnano bene in pixel art: per gli sprite importanti il lavoro migliore e' di Andrea.

---
## BLOCCO 8 — Copyright e rischi
| Rischio | Cosa fare |
|---|---|
| La hack v87 e' derivata dalla ROM Sega: anche con musica propria resta un derivato | uso privato ok; per ogni distribuzione far verificare da un legale |
| Cannonball: licenza non commerciale + richiede ROM arcade | non usarlo come base della versione libera; solo riferimento tecnico |
| Cloni "simili" di sprite/percorsi | vietati nel progetto: restano opere derivate |
| Attribuzioni mancanti | ogni risorsa esterna in `CREDITI.md` (autore, licenza, URL) |
| Nessuna certezza legale da ChatGPT/agenti | decisione finale a un legale |

---
## BLOCCO 9 — Setup e avvio della build di test v4
```
cd "$HOME/Desktop/Outrun Sglavinated Edition v4 (OpenWay)"
opencode auth login        # per modelli a pagamento; i gratuiti Zen non richiedono chiave
opencode models            # verifica nomi
sh run.sh
```
Per avvio/stop del ciclo agenti usare `sh comandi/v3.sh start|stato|stop` dalla cartella del progetto; per il comando manuale `opencode`, `sh run.sh` apre la configurazione della build corrente.
Prima richiesta da dare a `@architetto (licenze, motori) e @cercaasset (risorse)`: censimento motori (anche pseudo-3D con licenza libera): licenza, richiede asset originali?, gira su Pi 4/400?, cosa sostituire.
Accesso Claude: l'abbonamento Pro NON vale dentro OpenCode (divieto Anthropic di feb 2026); Claude si usa da Claude app/Claude Code. Un'API key e' a pagamento a parte.

---
## BLOCCO 10 — Roadmap
| Fase | Cosa | Chi | Uscita |
|---|---|---|---|
| 0 | avvio, verifica modelli | Andrea | agenti rispondono |
| 1 | prototipo strada pseudo-3D su Pi 400 (pista C) e controllo asset di ZgzInfinity/OutRun | coder + @architetto (licenze, motori) e @cercaasset (risorse) | video/fps + tabella in SCAMBIO.md |
| 2 | scelta pista dopo il prototipo | Andrea (+ legale) | decisione |
| 3 | prova runtime su Pi 400 | Andrea + @debugger | numeri fps/temperatura |
| 4 | elenco asset da sostituire | @architetto (licenze, motori) e @cercaasset (risorse) | lista per categoria |
| 5 | asset originali (3 varianti -> scelta -> produzione) | Andrea + @asset | `assets_originali/` + `CREDITI.md` |
| 6 | build senza file Sega | capo + coder + @debugger | repo pulito |
| 7 | audit file/licenze | @reviewer + legale | checklist firmata |

---
## BLOCCO 11 — Risorse
| Tema | Link |
|---|---|
| OpenCode: agenti e modelli per agente | https://opencode.ai/v2/docs/agents/ |
| Esempio multi-agente (reviewer su modelli diversi) | https://github.com/abivan-tech/opencode-agentic-workflows |
| Esempio agenti + modello gratuito Big Pickle | https://github.com/TraftG/opencode-game-studio |
| Cambiare modello dei subagent | https://github.com/Yivas/opencode-subagent-models |
| ChatGPT (abbonamento) dentro OpenCode | https://aiengineerguide.com/til/chatgpt-subscription-with-opencode/ |
| Divieto login abbonamento Claude in app terze | https://alternativeto.net/news/2026/2/anthropic-officially-bans-using-subscription-authentication-for-third-party-claude-use |
| Divieto (altra fonte) | https://winbuzzer.com/2026/02/19/anthropic-bans-claude-subscription-oauth-in-third-party-apps-xcxwbn/ |
| Cannonball: ROM richieste, licenza, Pi 4/400 | https://wiki-next.recalbox.com/en/emulators/ports/out-run |
| Cannonball sorgente e licenza | https://github.com/djyt/cannonball |
| Remake GPL-3.0 (C++/SFML) | https://github.com/ZgzInfinity/OutRun |

---
## BLOCCO 12 — Non verificato / da controllare
- Nomi dei modelli gratuiti e disponibilita' (cambiano spesso).
- Sintassi dei permessi per file nei `.md` degli agenti (`edit: {"tools/patch_z.py": allow}`).
- Se il login ChatGPT in OpenCode vale per il tuo piano.
- Specifiche Pi 400 e nomi dei core (dalla mia conoscenza, non ricontrollati).
- Il primo avvio OpenCode con questa configurazione v4 e i relativi permessi va ancora verificato.
- Non cancellare la cartella v3 precedente: e' separata da questa copia di test e puo' contenere dati di lavoro.

## Decisione: pista C, «il meglio da piu' fonti»
Si prendono IDEE e tecniche, mai codice o asset (vale solo per C; le licenze di alcune fonti non sono dichiarate).
| Fonte | Cosa se ne prende | Cosa NON si prende |
|---|---|---|
| Lou's Pseudo 3d Page | matematica della strada a segmenti, curve, colline, sprite scalati | codice/testi copiati |
| JavaScript Racer (J. Gordon) | struttura del ciclo di gioco, segmenti, collisioni, traffico semplice | codice (licenza non dichiarata) |
| Cannonball / OutRun originale | solo i *numeri* di gameplay (velocita', sensazione di guida, 50 Hz, ritmo delle tappe) come riferimento d'analisi | codice, dati ROM, sprite, musica |
| ZgzInfinity/OutRun (GPL) | organizzazione di un clone in C++ (moduli, menu, tappe) | asset; codice solo se si accetta la GPL per tutto il progetto (decisione a parte) |
| La hack Sglavinated | personaggi, fumetti, ritmo degli «show», UFO: sono tuoi, si portano su | limiti Mega Drive (156 frasi ecc.) |
Il blocco B0 diventa: documento `motore_nuovo/docs/FONTI.md` con cosa si prende da ciascuna e perche', senza piu' il confronto con A2 (resta come opzione chiusa).

## Numerazione e uscita finale (decisione di Andrea, 2026-10-15)
- La numerazione riparte dalla **v90**: primo build del v3 = v90, poi v91, v92... (la hack Mega Drive resta v87/v88 nella cartella v2).
- Obiettivo finale: oltre alla versione per Pi/PC/XP, una **nuova ROM Mega Drive (.md)** nata dal v3, tutta originale (nessun byte Sega).
- Attenzione: e' la vecchia «pista D» (gioco nuovo su 68000), scartata prima per difficolta'. Resta bersaglio FINALE e FACOLTATIVO, da valutare solo quando il motore C gira (blocco N). Il motore e' gia' pensato per aiutare: numeri a virgola fissa, 320x224, 50 Hz, determinismo, dati separati dal codice.
- Non verificato: toolchain adatta (es. SGDK, licenza da controllare), quanta parte della strada pseudo-3D regge a 50 fps sul 68000, limiti di VRAM/sprite. Li studia @architetto nel blocco N.

## NOTA MODELLI (15 ott, vale su tutto)
Unico modello verificato: opencode/nemotron-3.5-super-vl-free. Nessun agente deve passare un parametro model ai subagent: ogni agente usa quello scritto nel suo file .opencode/agents/<nome>.md. Le tabelle modelli sopra sono solo indicative.
