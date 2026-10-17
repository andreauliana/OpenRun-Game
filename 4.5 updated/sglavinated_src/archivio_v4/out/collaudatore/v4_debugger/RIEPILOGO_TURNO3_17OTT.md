# Riepiilo Turno 3 @debugger — 17 ott 2026 08:05

## Blocco H (Stato: [~])
Compito: build cmake, replay deterministico (hash identico su 2 esecuzioni), tempo di frame, fps, temperatura sul Pi (ARCHITETTURA_CHECK.md).

### Sintassi nucleo (già verificata, Turno 3)
- Voce 76: `clang++ -std=c++17 -fsyntax-only -Isrc` sui 31 file del nucleo (core/6 road/3 render/7 input/1 audio/2 player/2 game/2 gameplay/7), esclusi runner/sponsor_loader/libretro → OK, exit 0, 0 errori. 5 warning non bloccanti.
- Voce 78: 34/34 file inclusi 3 nuovi gameplay (disegno_show.h, testi_frias.h/cpp), `clang++ -std=c++17 -fsyntax-only -Isrc` exit 0, 0 errori.
- Controllo negativo: `motore_nuovo/test/neg_control_sintassi.cpp` → exit 1 (gate non vuoto).

### Misure runtime NON eseguibili qui (regola 21)
- Build cmake: nessun cmake/compilatore eseguibile; shell ristretta.
- Replay deterministico 2x stesso input → stesso hash ogni 60 frame.
- ms/frame <= 20 ms (obiettivo <= 12 ms).
- fps 50.
- Temperatura Pi 400.
→ Rimandate a Pi 400 con toolchain Andrea (regole 9/21). Piano: ARCHITETTURA_CHECK.md §5.

### Voce SCAMBIO
- Voce 79 (DEBUGGER -> CAPO, [CHIUSA]): sintassi OK, shell negata per i 2 nuovi .cpp gameplay, H resta [~], nessuna misura runtime eseguibile qui.

### File toccati
- Nessun file di codice sorgente toccato.
- Solo: SCAMBIO.md (voce 79), bacheche/debugger/diario.md, bacheche/debugger/RIASSUNTO.md, out/debugger/RIEPILOGO_TURNO3_17OTT.md.

### Prossimo passo
Eseguire build cmake, replay deterministico, ms/frame, fps su Pi 400 con Andrea; poi H puo' passare a [x].