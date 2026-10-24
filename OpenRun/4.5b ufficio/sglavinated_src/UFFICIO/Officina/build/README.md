# build

Output generato da CMake. Non modificare manualmente né usare come fonte. Il gate crea qui la build; questa cartella è esclusa dal versionamento salvo questo README.

È l'unica cartella di build: `run_tests.sh` e `gate_t1.sh` passano da `TOOL/prepara_build.sh`. Se la `CMakeCache.txt` è stata creata in un altro percorso (cartella spostata, ZIP, altra macchina), la build vecchia viene spostata in `build_vecchia_<data>` e se ne crea una nuova qui. Le `build_vecchia_*` si possono cancellare a mano quando non servono.
