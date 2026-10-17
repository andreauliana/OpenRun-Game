# Riepilogo Blocco H — @debugger — 17 ott 2026 07:10

Stato: [~] (in corso, non dichiarabile "fatto": misure runtime mancanti).

## Cosa è verificato (sola lettura/ispezione, nessun file sorgente toccato)
- Sintassi nucleo `clang++ -std=c++17 -fsyntax-only`: OK, 31/31 file (core/, road/, render/, input/, audio/, player/, game/, gameplay/; esclusi runner/, libretro/). Vedi motore_nuovo/test/ESITO_VERIFICA_SINTASSI.md §6-9.
- Build cmake con artefatti reali (libsglavinated_core.a, sglavinated_runner) già riportata nei turni precedenti.
- Determinismo per ispezione: tempo = contatore frame (FrameClock), RNG xorshift32 con seme fisso, nessun orologio di sistema nel core, dt = 1/50, frameHash ogni 60 frame. Vedi motore_nuovo/test/ARCHITETTURA_CHECK.md §3.
- 0 riferimenti Sega in src/; SPDX MIT sui file del nucleo; quota open sopra soglia (verifiche precedenti).

## Cosa NON è eseguibile in questo ambiente (regola 21, shell negata)
- `clang++ -fsyntax-only` a mano (2 tentativi negati, non si insiste).
- `cmake -S motore_nuovo -B build && cmake --build build` (shell negata).
- Esecuzione binari runner per: replay deterministico (2x stesso input -> stesso hash /60), ms/frame, fps, temperatura.

## Cosa resta da fare (su Pi 400 con toolchain, Andrea, regole 9/21)
Piano in motore_nuovo/test/ARCHITETTURA_CHECK.md §5:
1. Build Release: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j$(nproc)`.
2. Termico prima: `vcgencmd measure_temp`, `vcgencmd get_throttled`.
3. Test deterministico: stesso file di input due volte, confrontare frameHash ogni 60 frame (identici).
4. Misura frame: ms/frame di update()+render() su >=600 frame; report media, p99, massimo, frame > 20 ms.
5. Termico durante/dopo. Criteri: 50 fps stabili, frame <= 20 ms, obiettivo <= 12 ms, zero alloc nel ciclo di gioco.
6. Riportare tutti i numeri in SCAMBIO.md.

## Voce SCAMBIO
- Voce 70 (DEBUGGER -> CAPO, [CHIUSA]) chiude voce 59 CAPO -> DEBUGGER [APERTA]. Blocco H resta [~] in RICHIESTA.md.
