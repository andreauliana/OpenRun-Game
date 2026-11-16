# REVIEW V47 (@developer-junior)
Riscritto 16/11 13:31 (turno VJ5 ∞).

## VJ5 ∞ 16/11 13:31 — review `audio/mixer.cpp` caricaMotore + test motore
File letti: `motore_nuovo/src/audio/mixer.h:88`, `motore_nuovo/src/audio/mixer.cpp:91-99` (src invariato, ultimo `57ac3f5`).
- **Possibile errore/contratto `Mixer::caricaMotore` (`mixer.cpp:91-99`, `mixer.h:88`):** se `leggiWav` di minimo o pieno fallisce, ritorna `false` senza azzerare/sostituire `motAcc_`, `motMin_`, `motPieno_` (e posizioni/filtri restano nello stato corrente). Il commento API dice "false = resta il sintetico", ma un fallimento dopo un caricamento riuscito lascia campioni già caricati/stato residuo invece di ricadere pulitamente al sintetico; `game.cpp` ignora l'esito (rilievo aperto). Misurato: ricarica su cartella inesistente dopo load OK -> ritorno `false`, campione successivo ancora udibile (rms@800 ~901.8).

## Test/prova (questo turno)
- `motore_nuovo/test/prova_gate.cpp`: `verificaMotoreCampionato()` esteso con probe ricarica fallita post-load OK (ritorno false + rms dopo fail).
- `sh UFFICIO/Officina/TOOL/run_tests.sh` exit 0 (ctest 27/27).
- `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0.
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_motore_1611d" lv_01_neon motore` exit 0 (`GATE MOTORE OK`, "ricarica fallita dopo OK: ritorno false=si rms@800 dopo fail=901.8"). gate_t1.sh non eseguito.

## Rilievi aperti (cumulativi)
- `mixer.cpp:91-99` (questo turno, confermato); `salvataggi.cpp:78-83/:145-147/:108-109`.
- `replay.cpp:20-21` troncamento silenzioso; `paesaggio.cpp:113-114/:238-239`; `fasi.cpp:512-520/:576`; `game.cpp:1809-1814`.
- `runner/sponsor_loader.cpp:101-103`; `render/crt.cpp` w/h negativi; `core/json_mini.h:215-221/:187-206/:109`.
- VJ3/VJ4 chiusi. gate_t1.sh non eseguito.
