# TOOL — Officina

Destinazione degli strumenti di sviluppo. Gli script esistenti restano in `../../../tools/` finché build e automazioni non vengono migrate insieme; non duplicarli qui.

## Toolchain e prove attive

La configurazione CMake attiva è `../../../motore_nuovo/CMakeLists.txt`; tutti i comandi vanno lanciati dalla radice `sglavinated_src/`.

| Componente/strumento | Comando o target | Funzione |
|---|---|---|
| Core C++17 | `cmake --build UFFICIO/Officina/build` | `sglavinated_core`, libreria indipendente da SDL |
| Runner desktop | `cmake --build UFFICIO/Officina/build --target sglavinated_runner` | Runner SDL2, se SDL2 è disponibile in configurazione |
| Core libretro stub | Configurare con `-DBUILD_LIBRETRO=ON`, poi `cmake --build UFFICIO/Officina/build --target sglavinated_libretro` | Modulo opzionale; richiede che l'opzione sia attivata |
| Prove CTest | `sh tools/run_tests.sh` | Configura Release, compila `prova_gate` e lancia i sette test registrati quando `motore_nuovo/test/prova_gate.cpp` è presente |
| Gate integrato | `sh tools/gate_t1.sh` | Configura/compila in Release e verifica i tre livelli, replay, accelerazione, crash, sterzo e curve; log in `UFFICIO/Officina/Test area/gate_t1.log` |
| Soak gate | `sh tools/gate_t1.sh soak` | Aggiunge la prova da 30.000 frame |
| Inizializzazione SDL | `sh tools/check_sdl_init.sh` | Controlla `SDL_Init` del runner già compilato |

Builder/Developer e Developer/Builder Junior possono usare i wrapper di test autorizzati e aggiornare lo status condiviso dell'Officina. Non modificare a mano build, log o altri output generati; non eseguire direttamente binari fuori dai wrapper assegnati.
