# PIANO V3 (open) — Outrun Sglavinated Edition

> Piano originario V3: la copia di test corrente e' `Outrun Sglavinated Edition v4 (OpenWay)`; i nomi interni V3 restano invariati. Visione e vincoli duri: `sglavinated_src/VISIONE.md`. Squadra attuale: 12 agenti in `sglavinated_src/.opencode/agents/`; scaletta e fattibilita' delle novita': vedi `SCALETTA.md`. Istruzioni per Andrea: `COSA_FA_L_UMANO.md`.

Aggiornato: 16 ott 2026. Ogni blocco e' indipendente: leggi solo quello che ti serve.

---
## BLOCCO 1 — Obiettivo e stato
| Voce | Stato |
|---|---|
| Hack Mega Drive v87 | funziona sul Pi 400 (lr-genesis-plus-gx, PAL) |
| Problema | budget VBlank, 64 KB RAM -> nuove funzioni fanno sbandare i test |
| Obiettivo v4.6 | creare nuovo gioco ispirato libero da materiale Sega, con asset nuovi,multipiattaforma |

---
## BLOCCO 2 — Decisioni prese
1. Cartella di lavoro per questa build di test: `/Documents/GitHub/OpenRun-Game/OpenRun`.
2. Canale di scambio: `sglavinated_src/SCAMBIO.md`.
3. Asset solo originali o con licenza libera (no cloni Sega).
4. Stile: `retro, pixel art, urban, lo-fi, hip hop, jungle` (`assets_originali/DIREZIONE_ARTISTICA.md`).

---
## BLOCCO 3 — Decisioni aperte (tocca ad Andrea)
| # | Domanda | Quando |
|---|---|---|
| 1 | **DECISO da Andrea: pista C++**.  |

---
## BLOCCO 4 — Le due piste
| | A: motore nativo su Pi | B: ROM Mega Drive riscritta (.md) |
|---|---|---|
| Idea | motore C/C++ su Pi 400, asset sostituiti | portare un motore C++ su ROM con un compilatore |
| Parere | piu' realistica | da scartare per il motore completo |
Criteri e prove su Pi 400: `ARCHITETTURA_CHECK.md`.
**Esito della verifica (15 ott): vedi `RAPPORTO_FATTIBILITA.md`.** Raccomandata la pista C (motore nuovo C++/SDL2); alternativa B (base GPL ZgzInfinity/OutRun con tutti gli asset sostituiti); Cannonball solo come riferimento tecnico; traduzione in ROM Mega Drive scartata.

---
## BLOCCO 5 — Copyright e rischi
| Rischio | Cosa fare |
|---|---|
| La hack v87 e' derivata dalla ROM Sega: anche con musica propria resta un derivato | uso privato ok; per ogni distribuzione far verificare da un legale |
| Cannonball: licenza non commerciale + richiede ROM arcade | non usarlo come base della versione libera; solo riferimento tecnico |
| Cloni "simili" di sprite/percorsi | vietati nel progetto: restano opere derivate |
| Attribuzioni mancanti | ogni risorsa esterna in `CREDITI.md` (autore, licenza, URL) |
| Nessuna certezza legale da ChatGPT/agenti | decisione finale a un legale |

---
## BLOCCO 6 — Roadmap
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
## BLOCCO 7 — Risorse
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
## BLOCCO 8 — Non verificato / da controllare
- Sintassi dei permessi per file nei `.md` degli agenti (`edit: {"tools/patch_z.py": allow}`).

## Decisione: pista C, «il meglio da piu' fonti»
Si prendono IDEE e tecniche, mai codice o asset (vale solo per C++; le licenze di alcune fonti non sono dichiarate).
| Fonte | Cosa se ne prende | Cosa NON si prende |
|---|---|---|
| Lou's Pseudo 3d Page | matematica della strada a segmenti, curve, colline, sprite scalati | codice/testi copiati |
| JavaScript Racer (J. Gordon) | struttura del ciclo di gioco, segmenti, collisioni, traffico semplice | codice (licenza non dichiarata) |
| Cannonball / OutRun originale | solo i *numeri* di gameplay (velocita', sensazione di guida, 50 Hz, ritmo delle tappe) come riferimento d'analisi | codice, dati ROM, sprite, musica |
| ZgzInfinity/OutRun (GPL) | organizzazione di un clone in C++ (moduli, menu, tappe) | asset; codice solo se si accetta la GPL per tutto il progetto (decisione a parte) |
Il blocco B0 diventa: documento `motore_nuovo/docs/FONTI.md` con cosa si prende da ciascuna e perche', senza piu' il confronto con A2 (resta come opzione chiusa).

## Numerazione e uscita finale (decisione di Andrea, 2026-10-15)
- Non verificato: toolchain adatta (es. SGDK, licenza da controllare), quanta parte della strada pseudo-3D regge a 50 fps sul 68000, limiti di VRAM/sprite. Li studia @architetto nel blocco N.
