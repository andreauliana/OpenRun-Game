# REVIEW V47 (@developer-junior)
Riscritto 15/11 10:43 (turno VJ5 ∞).

## VJ5 ∞ 15/11 10:43 — `src/game/game.cpp:1809-1814` (commit `57ac3f5`, nessun cambio src)
Stato: `git log -5 --oneline -- motore_nuovo/src` exit 0 (ultimo `57ac3f5`), `git status --short -- motore_nuovo/src` exit 0 (pulito). File test modificato: `motore_nuovo/test/prova_gate.cpp` (commento VJ5).
Verifica: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0; `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_replay_turno3" lv_01_neon replay` exit 0 (GATE REPLAY OK, cap/size 4096, playback OK). gate_t1.sh non eseguito.

Rilievo nuovo: `Game::aggiornaRivali` calcola `lentaCurva = 1.0f - std::clamp(curvaQui * RIVALE_CURVA_SCALE * r.bravura, 0.0f, RIVALE_CURVA_SLOW_MAX)` (`game.cpp:1810-1811`) con commento "attenuato dalla bravura: i rivali piu' bravi soffrono meno la curva" (`:1809`). Ma `r.bravura` e' 0.92..1.08 (`:1530-1531`) e moltiplica il termine di rallentamento: un rivale piu' bravo (bravura maggiore) produce `lentaCurva` piu' basso e quindi rallenta di piu' in curva, opposto all'intento dichiarato.
Correzione proposta: invertire il contributo della bravura (es. dividere per `r.bravura`, usare `2.0f - r.bravura`, o fattore analogo centrato su 1.0) mantenendo il clamp, poi verificare comportamento rivali/tempi.

## VJ5 ∞ 15/11 10:35 — `src/audio/mixer.cpp:91-99` (commit `57ac3f5`, nessun cambio src)
Stato: `git log -5/status -- motore_nuovo/src` exit 0. Test modificato: `motore_nuovo/test/prova_gate.cpp`.
Verifica: `cmake --build .../prova_gate` exit 0; replay `vj5_replay_turno2` exit 0 (GATE REPLAY OK). gate_t1.sh non eseguito.

Rilievo: `Mixer::caricaMotore` (`mixer.cpp:91-99`) fa `return false` se minimo/pieno mancano (`:93-94`) prima di assegnare i campioni (`:96-98`), lasciando stato motore precedente; `game.cpp:912` ignora l'esito. Reload fallito può conservare WAV stantie.
Correzione proposta: azzerare/stabilire fallback esplicito e gestire il ritorno.

## VJ5 ∞ 15/11 10:26 — `src/audio/mixer.cpp:40-76,91-99` (commit `57ac3f5`, nessun cambio src)
Stato: `git show 57ac3f5 -- motore_nuovo/src` exit 0. Nessun cambio src/test/dati in quel turno.
Verifica: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0; `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_replay_turno" lv_01_neon replay` exit 0 (GATE REPLAY OK). gate_t1.sh non eseguito.

Rilievo: `leggiWav` (`mixer.cpp:52-75`) legge `data` solo se `canali > 0 && bit == 16`; se `data` precede `fmt ` il chunk viene saltato e `caricaMotore` può fallire su WAV non canonico (fallback sintetico). Proposta: salvare/decodificare `data` dopo `fmt` o doppio passaggio.

## VJ5 ∞ 15/11 10:17 — `src/game/game.h:49`, `src/game/game.cpp:511`, `src/runner/main.cpp:344,346` (commit `57ac3f5`, nessun cambio src)
Stato: ultimo src `57ac3f5` invariato (nessun cambio src/test/dati in quel turno). Nessuna verifica eseguita; gate non eseguito.

Rilievo: `Game::AUDIO_FRAME_SAMPLES` è fisso a `44100 / 60` (`game.h:49`) e usato per buffer core (`game.cpp:511`) e coda SDL (`runner/main.cpp:344,346`). A 50 Hz servono 882 campioni/frame ma se ne producono/accodano 735: possibile sottoriempimento/desync audio a 50 Hz.
Correzione proposta: derivare i campioni per frame dalla frequenza effettiva e mantenere coerenti buffer core e runner per 50/60 Hz.

## VJ5 ∞ 15/11 10:11 — `src/audio/mixer.cpp:108-110` (commit `57ac3f5`, nessun cambio src)
Stato: `git log -1 --oneline -- motore_nuovo/src` exit 0 (`57ac3f5`), `git status --short -- motore_nuovo/src` exit 0. Nessuna verifica eseguita; gate non eseguito.

Rilievo nuovo: `Mixer::motoreCampionato` (`audio/mixer.cpp:108-110`) incrementa `motSpentoS_` solo quando `rpm >= 0.0f && rpm <= 1.0f`; con `rpm < 0` (pausa, `game.cpp:2234` passa `-1.0f`) il timer di motore spento non cresce, mentre con `rpm == 0.0f` (menu, `game.cpp:2812`) cresce. Dopo >1 s in menu, al rientro in gara con `rpm > 1.0f` la condizione `motSpentoS_ > 1.0f` (`:108`) può far ripartire `posAcc_` (suono accensione) se il campione è caricato; dopo una pausa lunga no. Asimmetria di comportamento tra pausa e menu.
Correzione proposta: uniformare il criterio (es. trattare `rpm < 0` come pausa senza azzerare la semantica di "spento da tempo", oppure usare flag esplicito) e documentare la sentinella in `mixer.h`/chiamanti.

## VJ5 ∞ 15/11 10:03 — `src/game/salvataggi.cpp:78-83` (commit `57ac3f5`, nessun cambio src)
Stato: `git log -1 --oneline -- motore_nuovo/src` exit 0 (`57ac3f5`), `git status --short -- motore_nuovo/src` exit 0.
Verifica: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0; `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_replay" lv_01_neon replay` exit 0 (GATE REPLAY OK, playback sequenza/fine 0 verificati). gate_t1.sh non eseguito.

Rilievo: `SalvataggiGiocatore::creaDirectorySeManca()` (`game/salvataggi.cpp:78-83`) fa probe con `fopen("salvataggi","rb")` (non affidabile per directory) e poi `if (SGL_MKDIR("salvataggi") != 0) return false;` ma il commento dice "errno=EEXIST se gia' presente: accettato": se la directory esiste `mkdir` fallisce e la funzione ritorna `false` invece di trattare il caso come OK. Il chiamante ignora l'esito (`:145-147`) quindi il salvataggio spesso prosegue, ma logica/commento sono incoerenti.
Correzione proposta: distinguere `errno==EEXIST`/directory già presente (ritorno `true`) dal vero errore, preferendo `stat` al probe `fopen`.

## VJ5 ∞ 15/11 09:54 — `src/core/rng.h:19` (commit `57ac3f5`, nessun cambio src)
Stato/Verifica: `git log/status` exit 0; `cmake --build ... --target prova_gate` exit 0; replay exit 0. gate_t1.sh non eseguito.
Rilievo: `Rng::next(u32 limit)` fa `return next() % limit;` senza guardia `limit==0` (UB/crash). Correzione: `if (limit == 0) return 0;` prima del modulo.

## VJ5 ∞ 15/11 09:47 — `src/game/game.cpp:2794` + `core/replay.*` (commit `57ac3f5`, nessun cambio src)
Stato: `git log -1 --oneline -- motore_nuovo/src` exit 0 (`57ac3f5`), `git status --short -- motore_nuovo/src` exit 0.
Verifica: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0; `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_replay" lv_01_neon replay` exit 0 (GATE REPLAY OK, `capacity=4096 size=4096` dopo 5000 record). gate_t1.sh non eseguito.

Rilievo: `Game::update()` chiama `replay_.record(input_.pressed | input_.held)` se `replay_.active()` (`game.cpp:2794`), ma `Replay::active()` è true anche in `Playback` e `Replay::record()` (`core/replay.cpp:20`) accoda senza verificare il modo. In playback potrebbe appendere/corrompere il buffer (entro capacità).
Correzione proposta: registrare solo in `Mode::Record` e/o far ignorare `record()` fuori da Record.

Rilievo confermato: `Replay::startRecord()` riserva 4096 (`core/replay.cpp:9`) e `record()` scarta oltre capacità senza riallocare/segnalare (`:20`); replay lunghi (>~68 s a 60 Hz) si troncano silenziosamente.

## VJ5 ∞ 15/11 09:40 — `motore_nuovo/test/prova_gate.cpp` (modo `motore`)
Guardia `piccoPausaPulita > 1500` in pausa pulita: misura 1349 (OK), replay IDENTICO.

## Rilievi aperti (precedenti)
- `src/runner/main.cpp:227-233` joystick generico: tasto 2 mappa `X` e `A` insieme.
- `src/audio/mixer.cpp:127-130` accensione sommata con `rpm < 0` (PAUSE rumorosa primi ~1.6 s); `:112-116` `std::exp` per campione; `:108` riaccensione senza guardia `posAcc_ < 0`.
- `src/audio/mixer.cpp:91-99` + `src/game/game.cpp:912` fallimento `caricaMotore` silenzioso.
- `src/game/fasi.cpp:512-520` `salva()` forza decima riga rompendo ordine punteggio; `:576` puntini `nostro` bianchi fissi.
- `src/game/show.cpp:141-145` off-road pending/timer ripetuti ogni frame sopra soglia.
- `src/game/game.h:49` + `game.cpp:511` + `runner/main.cpp:344,346` `AUDIO_FRAME_SAMPLES` fisso 44100/60: possibile desync audio a 50 Hz.

## VJ4 Soak (6/6 chiuso)
| Data | Livello | Exit | Frame | Replay | ms/frame medio | ms/frame max | Crash |
|---|---|---|---|---|---|---|---|
| precedenti | lv_01-lv_04 | 0 | 30000 | IDENTICO | — | — | no |
| 14/11 | lv_05_spray | 0 | 30000 | IDENTICO | 2.013 | 42.081 | no |
| 14/11 | lv_06_desktop | 0 | 30000 | IDENTICO | 1.536 | 8.926 | no |

## VJ3 50/60 Hz (6/6 chiuso)
Delta: lv_01 0.03%, lv_02 0.19%, lv_03 **4.44%** (anomalia @developer), lv_04 0.03%, lv_05 0.24%, lv_06 0.09%. Replay IDENTICO.

## Note
- `salvataggi/giocatore.json` da ripristinare a mano (`git checkout -- salvataggi/giocatore.json`, fuori perimetro).
- @developer: lv_03 anomalia 50/60 Hz 4.44%.
- gate_t1.sh non eseguito in questo turno.