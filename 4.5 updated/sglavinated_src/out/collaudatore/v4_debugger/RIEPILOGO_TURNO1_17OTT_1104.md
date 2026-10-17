# Riepiologo Blocco H — @debugger — Turno 1, 17/10/2026 11:04

## Stato blocco H
[~] in corso — attribuito a @debugger (+ Andrea sul Pi 400).

## Compito (da RICHIESTA.md blocco H)
Build cmake, replay deterministico (hash identico su due esecuzioni), tempo di frame, fps e temperatura sul Pi (ARCHITETTURA_CHECK.md).

## Esito questo turno
- Shell negata (regola 21): `clang++ -std=c++17 -fsyntax-only` su motore_nuovo/src/ (game.cpp, core/rng.h modificate da @coder voce 87) NON eseguibile; `cmake -S motore_nuovo -B build` NON eseguibile. Non si insiste (regola 21).
- Build cmake, replay 2x stesso input (frameHash ogni 60 frame identico), ms/frame (<=20 ms, obiettivo <=12 ms), fps 50 stabili, temperatura, test prova_gate 10s headless: NON misurabili qui -> Pi 400 con toolchain Andrea (regole 9/21, ARCHITETTURA_CHECK.md §5).
- Sintassi nucleo ultima verifica OK: 34/34 file, 0 errori (voce 76/78, 17 ott). Modifiche voce 87 da ricontrollare con -fsyntax-only appena shell sbloccata.
- Nessun file di sorgente toccato (sola lettura).

## Prossimo
- Su Pi 400 (Andrea): `cmake -S motore_nuovo -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --target prova_gate`; replay deterministico 2x stesso input; ms/frame, fps, temperatura; 3 screenshot in out/debugger/ (partenza, curva, fuori strada) + log velocita'/posizione.
- Esito da riportare in SCAMBIO.md e bacheche/debugger/diario.md.

## Riferimenti
- Voci SCAMBIO aperte a @debugger: 79, 89 (entrambe chiuse con voce 90).
- DECISIONI_ANDREA.md D9 [APERTA]: DEFAULT solo `cmake --build`.
