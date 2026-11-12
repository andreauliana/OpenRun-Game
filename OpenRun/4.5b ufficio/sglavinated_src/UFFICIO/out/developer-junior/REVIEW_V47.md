# REVIEW V47 (riscritta) — @developer-junior
Turno 12/11 09:55, VJ5 ∞.

## Cambio rivisto
- File: `motore_nuovo/src/road/road.cpp` (+`motore_nuovo/src/road/road_segment.h`) — diverso da renderer/mixer/framebuffer/paesaggio/road.h/partenza/salvataggi/fasi/libretro/game/player/replay. Review statica, nessun cambio src locale.

## Rilievi
1. **road.cpp:179-188,200,203-204 — chiusura direzione e rettilineo bivio:** il residuo di curvatura è calcolato sui segmenti con `curve != 0` (passata 1) e la correzione è sottratta solo dove `seg.curve != 0` (200), mantenendo i dritti perfettamente dritti come da commento. Nel tratto bivio (164-175,203-204) le curve vengono però forzate a 0 dopo la correzione (`pesoDritto>=1 => 0`, altrimenti scala): l'azzeramento/scala post-correzione può reintrodurre un piccolo scarto di direzione locale nella zona bivio (il bivio resta speciale e a fine tappa il ramo è ricentrato), da verificare solo se si osserva deriva.
2. **road.cpp:74-81 — raccordi entrata/uscita:** se `ee+uu > n` i raccordi vengono scalati proporzionalmente per non sovrapporsi (ok); con valori molto piccoli l'arrotondamento intero può ridurre leggermente una delle rampe, comportamento accettabile.
3. **road.cpp:164-175,227-235 — rettilineo/piano del bivio:** attivato solo se `conRettilineo` lascia margine per i raccordi (`aDritto-BIVIO_RACCORDO>0` e `bDritto+BIVIO_RACCORDO<nSeg`), altrimenti nessuna modifica (fallback sicuro). Le quote sono raccordate verso `quotaDritto` con `pesoDritto` (229-234).
4. **road_segment.h:** campi e commenti coerenti con l'uso in road.cpp (portali 1/4/5-7, riva/luci/incrocio/galleria, traffico/larghezza).

## Prova
- `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0 (12/11 09:55).
- `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_road_0955" lv_02_atomico caricamento` exit 0, `GATE CARICAMENTO OK` (init 11.0 ms, avviaGara 10.4/7.8 ms, soglia 1000 ms).
- gate_t1.sh: non eseguito.
