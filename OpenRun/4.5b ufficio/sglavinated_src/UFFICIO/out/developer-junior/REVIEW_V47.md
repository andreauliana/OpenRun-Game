# REVIEW V47 (@developer-junior)
Riscritto 17/11 10:09 (turno VJ5).

## VJ4 soak (uno per turno) — completato 17/11
- 17/11 09:28 `lv_01_neon`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 1.926 max 60.243.
- 17/11 09:23 `lv_02_atomico`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay IDENTICO, ms/frame medio 2.136 max 96.164.
- 17/11 09:26 `lv_03_foglie`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 2.008 max 107.658.
- 17/11 09:35 `lv_04_cromo`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 1.869 max 125.090.
- 17/11 09:42 `lv_05_spray`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 2.047 max 126.451.
- 17/11 09:49 `lv_06_desktop`: build exit 0; soak exit 0 — GATE OK, 30000 frame, replay 2x IDENTICO, ms/frame medio 1.761 max 72.711.
- gate_t1.sh non eseguito. VJ4 chiuso (tutti lv_01-lv_06, nessun crash).

## VJ5 ∞ review src (ultimo cambio `86445cd`, merge grafica/via auto agenti)
- 17/11 09:08: `game.cpp:1026-1032,1047-1048` caricamento solo pose Andrea; warning dritta poco esplicito sul set richiesto.
- 17/11 09:58: `motore_nuovo/src/game/game.cpp:1032` costruzione nome stop con `(POSE[k] + 5)` (pointer arithmetic per togliere `auto_`) funziona coi nomi attuali ma e' fragile/opaca; semplificazione: derivare il suffisso in modo esplicito (es. `std::string(POSE[k]).substr(5)` o costante) per evitare errori se cambia la convenzione.
- 17/11 10:04: `motore_nuovo/src/game/game.cpp:1026-1048` carica 11 pose (`POSE`) e 11 stop ma avvisa solo se manca `auto_dritta` (`:1047-1048`); se manca un'altra posa/stop il gioco prosegue silenzioso col placeholder, rendendo la diagnosi asset piu' difficile. Semplificazione: loggare il nome del raw quando `caricaSpriteRaw`/sprite risultante e' vuoto per ogni `k` (o almeno per le pose base).
- 17/11 10:09: `motore_nuovo/src/game/game.cpp:3201-3204` (`caricaSpriteFolla`) e `:3254-3256` (`preparaPartenza`, gruppi supporto) caricano piu' sprite ma avvisano solo se manca l'indice 0; se 0 e' presente ma altri raw del gruppo mancano restano silenziosi e si usano sprite vuoti (gruppi/pose invisibili). Semplificazione: verificare e loggare ogni sprite caricato vuoto.
- 17/11 10:21: `motore_nuovo/src/game/game.cpp:1037-1046` carica componenti auto (ruota_1-3, luci_stop, fumo_1-3) e `auto_giocatore.json` ma non segnala se raw/json mancano o risultano vuoti; diagnostica asset piu' difficile. Semplificazione: loggare il nome file quando `caricaSpriteRaw`/caricamento punti fallisce.
- 17/11 10:27: `motore_nuovo/src/game/game.cpp:734-736` carica skyline del livello ma se `caricaSpriteRaw` fallisce/ritorna vuoto imposta sky piatto senza warning/log; diagnostica asset difficile. Semplificazione: loggare `livelloId`/percorso quando lo sprite skyline e' vuoto.

## Rilievi aperti (cumulativi, sintesi)
- `game.cpp:3719,3693` divisione per `a.passo` senza guardia; `:1645,1673,1678,1679` `trafficoAt` non clampato; `:1809-1814` bravura/curva invertita.
- `mixer.cpp:91-99` fail post-load stato residuo; `salvataggi.cpp:78-83/:145-147/:108-109`; `fasi.cpp:512-536`.
- `gameplay/escalation.h:24,35,62` `errori_` non saturato. `core/json_mini.h` BOM/`0x`/dup; `replay.cpp:20-21`; `paesaggio.cpp:113-114/:238-239`. VJ3 chiuso.
