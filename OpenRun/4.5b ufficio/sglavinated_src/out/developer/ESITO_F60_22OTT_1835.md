# Esito F60 — 22/10/2026 18:35 — @developer

## Blocco
F60 [ ] — gioco a 60 Hz, tempi in secondi con dt (ANDREA_REVIEW, dopo R1, prima di R2; non tocca src/runner/).

## Cosa è stato fatto
Conversione del module show e relative dipendenze da contatore di frame a secondi (dt = 1/frequenzaHz_), così il comportamento è identico a 50 e 60 Hz:
- `motore_nuovo/src/gameplay/show.h` / `show.cpp`: ShowConfig, DatiShow, StatoGameplayInput (campo `dt`), ShowManager e contatori interni in secondi (durataShow, inattivitaSoglia, cooldownShow, offRoadSoglia/Attesa/TagliaPeriodico, decadimentoErrori, minGaraS, minStageS, bollaAppare, prTab).
- `motore_nuovo/src/gameplay/ufo.h`: UfoConfig e aggiorna in secondi (periodo, finestra, durata, fuga); tabella TW in secondi.
- `motore_nuovo/src/gameplay/disegno_show.h` / `disegno_show.cpp`: ScenaShow (tempoShowS/tempoRimastoS), PersonaggioLayout (posePassoS/entraS/esceS/swayPassoS/bobPassoS), aggiorna(tempoS, ...).
- `motore_nuovo/src/game/game.h` / `game.cpp`: membro `tempoGiocoS_`, `gi.dt = dt`, `disegnoShow_.aggiorna(tempoGiocoS_, ...)`, reset in init().

Il core di gioco (game.cpp/player.cpp) era già in secondi (turno precedente); con questa conversione l'intero nucleo F60 è coerente.

## Verifica
- `cmake --build UFFICIO/Officina/build --target sglavinated_core` -> exit 0 (100% Built target sglavinated_core).
- `prova_gate` / `ctest`: NON eseguibili. Conflitto preesistente (non introdotto in questo turno) su `impostaFrequenzaDaConfig`: definita `static` in `motore_nuovo/test/prova_gate.cpp:274` e dichiarata `scl::impostaFrequenzaDaConfig` in `game.h:200` -> chiamata ambigua (4 errori), build di prova_gate fallita. Dominio test @developer-junior; file prova_gate.cpp non modificabile per questo task (solo compito 3 di R1).
=> gate non eseguito in questo turno.

## Blocchi residui
- T1-RUN [~]: Andrea D13=B (non chiudere), prova runner Mac v91 pendente, non si tocca src/runner/.
- P1 [~]: punto 4 sprite veri in attesa di @grafico G1.
- H [~]: fps/temperatura Pi 400 dopo ok runner Mac di Andrea.
