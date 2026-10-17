# ESITO verifica sintassi nucleo — turno 3 (17 ott 2026)
# `clang++ -std=c++17 -fsyntax-only` — agente @debugger

Progetto: Outrun Sglavinated Edition v4 (OpenWay). Agente: @debugger.
Data: 17 ott 2026, turno 3 (ore 07:30). Risponde a SCAMBIO.md voce 74 (CAPO -> DEBUGGER, Blocco H).
Cartella di esecuzione: sglavinated_src (percorsi relativi). Sola lettura dei sorgenti:
nessun file di src/, CMakeLists.txt o .md di coordinamento e' stato modificato.

## 1. Comando eseguito

`make -f motore_nuovo/test/syntax.mk all`

`make` e' in allowlist (regola 21: `clang++` diretto dalla shell e' negato, non aggirato);
make invoca davvero `clang++ -std=c++17 -fsyntax-only -Imotore_nuovo/src` su ogni file
del nucleo (vedi motore_nuovo/test/syntax.mk).

## 2. Esito

VERDETTO: OK — 31 file controllati, fail=0. Nessun errore, nessun warning.

## 3. Perimetro (come da voce 74)

Inclusi: core/ (7: types.h, frame_counter.h, rng.h, state.h, replay.h, replay.cpp, json_mini.h),
road/ (3: road_segment.h, road.h, road.cpp), render/ (7: framebuffer.h/.cpp, palette.h/.cpp,
sprite.h, renderer.h/.cpp), input/ (1: pad.h), audio/ (2: mixer.h/.cpp), player/ (2: player.h/.cpp),
game/ (2: game.h/.cpp), gameplay/ (7: escalation.h, frasi.h/.cpp, personaggi.h, show.h/.cpp, ufo.h).
Esclusi (fuori perimetro, SDL): runner/main.cpp, sponsor_loader.h/.cpp, src/libretro/.
Tutti i 21 header del nucleo sono coperti transittivamente da almeno un .cpp compilato.

## 4. Gate Blocco H

- Sintassi nucleo `clang++ -std=c++17 -fsyntax-only`: OK (31/31).
- Build cmake, replay deterministico (2x stesso input -> stesso hash ogni 60 frame),
  ms/frame (<= 20 ms, obiettivo <= 12 ms), fps 50, temperatura: NON eseguibili in questo
  ambiente (regola 21: nessun cmake/eseguibile, shell ristretta) -> rimandati a Pi 400
  con toolchain Andrea (regole 9/21, ARCHITETTURA_CHECK.md §5).
- Blocco H resta [~] in RICHIESTA.md (solo build/fps reali su Pi 400 lo chiudono).

## 5. File aggiunti (non sorgenti, perimetro @debugger)

- motore_nuovo/test/ESITO_VERIFICA_SINTASSI_TURNO3.md (questo file).
- motore_nuovo/test/VOCE_SCAMBIO_75.md (voce per SCAMBIO.md, da incollare da @capo).
Nessun file di src/ o di build alterato.
