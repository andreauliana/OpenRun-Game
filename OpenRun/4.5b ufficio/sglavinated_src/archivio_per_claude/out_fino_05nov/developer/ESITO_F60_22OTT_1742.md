# Esito blocco F60 — 22/10/2026 17:42 — @developer

## Stato
F60 (60 Hz, dt in secondi) avviato. Non completato: gate non eseguibile per build error preesistente.

## Modifiche apportate (4 file + 1 nuovo)
1. `motore_nuovo/dati/config.json` — NUOVO: `{"frequenza_hz": 60}`.
2. `motore_nuovo/src/game/game.h` — `tempoResiduoS_` (f32, secondi), `frequenzaHz_` (i32), `FLIP_ANIMATION_S` (1.5f), `OFFROAD_CRASH_S` (2.0f), `AUDIO_FRAME_SAMPLES` = 44100/60.
3. `motore_nuovo/src/game/game.cpp` — `dt = 1.0f / frequenzaHz_` in `update()`; lettura `config.json` in `init()`; `tempoResiduoS_` in secondi (inizializzazione, checkpoint, game-over, HUD); `FLIP_ANIMATION_S * frequenzaHz_` e `OFFROAD_CRASH_S * frequenzaHz_` per i contatori frame; dichiarazioni `leggiFrequenzaDaConfig()` / `impostaFrequenzaDaConfig()` in spazio globale.
4. `motore_nuovo/src/player/player.h` — costanti per secondo: MAX_SPEED 250, ACCEL_BASE 56, BRAKE 120, STEER_INC 320, COAST_PER_SEC 0.7782, OFFROAD_SLOW_PER_SEC 0.6050, ACCEL_BOOST_SPEED 100.
5. `motore_nuovo/src/player/player.cpp` — `std::pow(COAST_PER_SEC, dt)` e `std::pow(OFFROAD_SLOW_PER_SEC, dt)`; sterzo/accelerazione/freno con dt in secondi.

## Gate
- `sh UFFICIO/Officina/TOOL/gate_t1.sh` — BUILD FALLITA (exit 11).
- Causa: errori preesistenti in `motore_nuovo/src/gameplay/disegno_show.cpp` (membri mancanti `posePasso`/`posePassoS`, `entraFrame`/`entraS`, `frameInizio`/`tempoShowS`, mismatch firma `calcolaPersonaggio`/`aggiorna`) — blocco D/P3, NON correlati a F60. Emersi dopo ricompilazione totale di `sglavinated_core` scatenata dalla modifica a `game.h`.
- Non eseguibile: nessuna misura ms/frame, replay, percorso disponibile in questo turno.

## Ultimo gate noto OK
22/10 ~10:00, exit 0: GATE T1 TUTTO OK, 6/6 GATE PERCORSO OK, ms/frame medio 0.171-0.175 max 0.318, replay 2x IDENTICO, gara e screenshot P1 OK; ctest CP-005 22/10 09:40: 17/17 passati.

## Blocchi correlati
- T1-RUN [~]: aperto (Andrea D13=B, non chiudere, non toccare src/runner/).
- P1 [~]: punto 4 sprite veri bloccato da @grafico G1.
- H [~]: bloccato fino a ok runner Mac Andrea.
- F60 [ ]: in corso, bloccato dal build error sopra.

## Scritture effettuate
- `bacheche/developer/diario.md` (1 riga in cima)
- `bacheche/developer/RIASSUNTO.md` (riscritto, 11 righe)
- `out/developer/ESITO_F60_22OTT_1742.md` (questo file)
