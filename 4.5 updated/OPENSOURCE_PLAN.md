# Versione libera (beta) — piano

Obiettivo: gioco di guida arcade rilasciabile senza materiale Sega.
Il progetto attuale (ROM hack Mega Drive) resta uso privato: è derivato dalla ROM originale.

## Fasi
| # | Fase | Output | Agente |
|---|---|---|---|
| 1 | Censimento motori open source (OutRun in C++) | tabella: licenza, richiede asset originali?, stato | @architetto (licenze, motori) e @cercaasset (risorse) |
| 2 | Scelta motore + verifica licenza con un legale | decisione di Andrea | Andrea |
| 3 | Elenco asset da sostituire | sprite, tracciati, musica, testi, nome/logo | @architetto (licenze, motori) e @cercaasset (risorse) -> capo |
| 4 | Asset ORIGINALI (non cloni di quelli Sega) | tue musiche (.xm), Gianni/Gesù/UFO, sprite e scenari nuovi, percorsi generati da zero, risorse CC0/CC-BY con crediti | Andrea + @asset |
| 5 | Build senza file Sega | repo pulito, test di avvio senza ROM | capo + @debugger |
| 6 | Audit finale file/licenze | checklist firmata | @reviewer + legale |

## Punti da chiarire (fase 1-2)
- Il motore richiede dati/ROM arcade per girare? Se sì, non è "libero" finché non sostituisci tutto.
- Il codice del motore deriva da disassemblaggio del codice Sega? Se sì, la licenza del repo non risolve il diritto d'autore sul codice.
- Nome, logo e tracciati riconoscibili (Coconut Beach ecc.) = da cambiare.

## Fatti verificati sul motore Cannonball (15 ott 2026)
| Fatto | Fonte |
|---|---|
| Per girare richiede la romset arcade OutRun Rev B (31 file `epr-/mpr-/opr-`) | Recalbox wiki |
| Licenza personalizzata, **uso non commerciale** (non e' una licenza libera) | Recalbox wiki; in RPM Fusion e' nel repo *nonfree* |
| Gira nativo su Raspberry Pi 4/400 (libretro) | Recalbox wiki |

## Due piste (decide Andrea dopo il censimento)
| | Pista A: motore su Pi | Pista B: traduzione in ROM Mega Drive (.md) |
|---|---|---|
| Idea | Motore C++ su Pi 400, asset sostituiti | C++ -> ROM `.md` con un compilatore Python |
| Limiti | nessun tetto da 4 MB / 64 KB RAM | RAM 64 KB, 7,6 MHz: il motore arcade non ci sta |
| Strumenti | gcc, SDL/libretro | serve gcc m68k (es. SGDK), non un compilatore Python; e' una riscrittura, non una traduzione |
| Licenza | Cannonball NON e' libero: serve altro motore o riscrittura pulita | idem |
| Parere | piu' realistica | da scartare per il motore completo |

## Specifiche vs copia (regola per @asset)
- Si possono usare le **specifiche tecniche** (dimensioni tile, formato dati, numero colori, struttura dei segmenti di strada).
- Il **contenuto** (aspetto di sprite, scenari, tracciati, musiche) deve essere nuovo: non rifatto "uguale" a partire da quello Sega.

## Obiettivo: almeno 50% open source (richiesta di Andrea, 2026-10-15)
- Misura: `python3 sglavinated_src/motore_nuovo/strumenti_py/quota_open.py` = % di byte di codice, dati e asset con licenza aperta dichiarata (MIT, GPL-3.0, CC0, CC-BY, OFL...). Soglia 50%, sale con la versione. Il numero va in SCAMBIO.md a ogni consegna.
- Dichiarazione: codice del motore con file `motore_nuovo/LICENSE` (prima riga `SPDX-License-Identifier: ...`); ogni cartella di asset/personaggio con `manifest.json` che ha `licenza`.
- Cio' che non conta come aperto: asset senza licenza, materiale Sega, tutto cio' che Andrea tiene privato.
- **Decisione di Andrea (aperta):** quale licenza per il proprio codice (MIT = massima liberta'; GPL-3.0 = chi lo modifica deve restare aperto) e per le proprie grafiche/testi (CC0 / CC-BY / CC-BY-SA, o riservate). Finche' non decide, la quota resta bassa e lo strumento lo segnala. Per una distribuzione pubblica (sponsor incluso) consigliabile un parere legale.
