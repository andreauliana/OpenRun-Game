# CP-001 — Baseline iniziale OpenRun 4.6

| Campo | Valore |
|---|---|
| Stato | CHIUSO — fotografia iniziale, non approvazione di release |
| Aperto / ultima verifica | 2026-10-18 / 2026-10-18 |
| Ambito | Registrare lo stato di partenza operativo 4.6 e le prove di build/test disponibili |
| Responsabile | Capo |
| Revisore | Non applicabile alla registrazione della baseline; la review tecnica resta indipendente per i checkpoint di modifica |
| Decisioni richieste ad Andrea | NON RICHIESTA per la sola registrazione della baseline |

## Sintesi

- La squadra prevista è composta da Capo, Designer, Builder/Developer e Developer/Builder Junior.
- La migrazione è graduale: le cartelle 4.6 definiscono ruoli e destinazioni, mentre le fonti attive restano per ora in `motore_nuovo/`, `assets_originali/`, `libreria/` e `tools/`.
- Il Capo gestisce questo registro e le revisioni aggregate; la review tecnica è peer review tra sviluppatori. Il Designer è l'unico ruolo che aggiorna gli asset condivisi.
- La gerarchia e le regole operative sono descritte in [AGENTS.md](../AGENTS.md); la policy checkpoint è nel [README dell'area](./README.md).
- Questa baseline fotografa il punto di partenza, non certifica una release né sostituisce l'approvazione di Andrea.

## Prova build/test disponibile

| Data | Comando | Esito misurato | Evidenza |
|---|---|---|---|
| 2026-10-18 | `sh tools/gate_t1.sh` | Exit 0; `GATE OK` su lv_01_neon, lv_02_atomico e lv_03_foglie; replay identico sui tre livelli; accelerazione, ribaltamento, sterzo e continuità curve OK | [Log gate T1](../UFFICIO/Officina/Test%20area/gate_t1.log) |

Il log riporta tempi medi di 0,200–0,280 ms/frame e un massimo misurato di 1,492 ms/frame, entro il limite del gate di 20 ms/frame. Il test è stato eseguito su macOS; non dimostra le prestazioni sul Raspberry Pi 400.

## Perimetro e limiti della baseline

- La prova di gate non costituisce peer review delle modifiche né approvazione finale.
- Le attività successive devono avere un checkpoint dedicato con responsabile, revisore diverso dall'autore, percorsi autorizzati e criteri misurabili.
- Log, screenshot e altre prove restano nella loro posizione operativa; il checkpoint conserva riferimenti e misure, non copie.
- Al momento della fotografia iniziale, il ramo `main` aveva 8 modifiche già in stage relative alla transizione 4.6 (ignore, quattro profili/regole, CMake, configurazione OpenCode e gate); nessun commit era stato creato. Questo dossier non ha alterato né registrato tali modifiche.

## Eventi

- 2026-10-18 — registrata la baseline di avvio 4.6 usando lo stato e il log di gate presenti nell'area di lavoro.
- 2026-10-18 — baseline chiusa come riferimento documentale; non richiede decisione di Andrea e non approva una release.
