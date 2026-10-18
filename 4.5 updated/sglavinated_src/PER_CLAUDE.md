# PER_CLAUDE.md — Richiesta parere a Claude

## AGENTE
@sviluppatore

## PROBLEMA
Tre punti tecnici bloccano la chiusura Tappa 1: (1) colline rotte (strada sale nel cielo), (2) sprite spariscono al wrap del giro, (3) fuoristrada troppo brutale (velocità crolla ~160→~6). Più: shell negata per 3 screenshot P1.

## PROVATO
- `sh tools/gate_t1.sh soak` → exit 0, GATE T1: TUTTO OK (speed@60=161.79, replay 2x IDENTICO 3 livelli, ms/frame 0.36 max 0.651, offroad raggiunto, soak 30k OK). Log: `out/collaudatore/gate_t1.log`
- `./build_gate/prova_gate out/collaudatore/lv_01_neon smoke` → screenshot PPM generati (partenza, curva, fuoristrada, avvolgimento, inattività)
- `cmake --build build_gate --target prova_gate` → compila OK
- Tentativi shell per screenshot `out/sviluppatore/`: 6x `Permission denied: shell` (comandi composti negati; regola 26 AGENTS.md: solo `sh tools/gate_t1.sh` permesso)
- Codice ispezionato: `road.cpp` project() doppia integrazione colline (righe 124-126 commento), `renderer.cpp` drawSprites zRel con modulo totalLen (righe 139-143), `player.h` OFFROAD_SLOW=0.99f (riga 46)

## DOMANDE (max 5)
1. **Colline**: in `Road::project` (riga 146-149) si usa `camWorldY` interpolata dai segmenti + `CAM_HEIGHT`. È corretto togliere l'integrazione doppia e usare solo la quota assoluta del segmento? Il commento dice "NON si ri-integrano" ma `curY` in build() integra già `(startHill + seg.hill)*0.5*len`.
2. **Sprite wrap**: `zRel = fmod(s.worldZ - playerZ, totalLen)` (renderer.cpp:141) — se `s.worldZ < playerZ` il modulo dà negativo, poi si aggiunge `totalLen`. Va bene o serve `fmod` positivo tipo `((a%b)+b)%b`?
3. **Fuoristrada**: `OFFROAD_SLOW=0.99` perde ~1%/frame → da 160 a 6 in ~280 frame (5.6 s). Andrea v90 dice "rallentamento percepibile ma non brusco". Proporre `0.995` (coast) o `0.998`? Un solo parametro alla volta.
4. **Sprite veri headless**: `CMakeLists.txt` riga `find_package(SDL2_IMAGE)` (maiuscolo) → SDL2_image non trovata, runner compila senza PNG. Cambiare in `find_package(SDL2_image)` (minuscolo) risolve? Ha effetti collaterali?
5. **Screenshot P1**: serve lanciare `./build_gate/prova_gate out/sviluppatore/ lv_01_neon smoke` (e lv_02_atomico, lv_03_foglie) per generare 3 PPM in `out/sviluppatore/`. Il comando è identico a quello che funziona per collaudatore. Perché la shell lo nega a @sviluppatore e non a @collaudatore?

## FILE DA CONTROLLARE
- `motore_nuovo/src/road/road.cpp` (righe 114-191: `project()`)
- `motore_nuovo/src/render/renderer.cpp` (righe 127-175: `drawSprites()` zRel wrap)
- `motore_nuovo/src/player/player.h` (riga 46: `OFFROAD_SLOW`)
- `motore_nuovo/CMakeLists.txt` (find_package SDL2_image)
- `tools/gate_t1.sh` (comando unico permesso)

## DEFAULT
Se non rispondi in tempo: @sviluppatore prova fix colline (rimuovi integrazione doppia), alza OFFROAD_SLOW a 0.995, corregge CMakeLists.txt SDL2_image → minuscolo; @collaudatore rilancia gate; screenshot P1 li fa @collaudatore per conto di @sviluppatore (stesso binary).