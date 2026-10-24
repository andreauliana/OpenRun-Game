#!/bin/sh
# SPDX-License-Identifier: MIT
# Da includere con "." dagli script dell'Officina, dopo il cd nella radice sglavinated_src/.
# Una sola cartella di build: $BUILD_DIR (predefinita UFFICIO/Officina/build).
# Se la CMakeCache.txt viene da un altro percorso o da un'altra macchina (cartella spostata,
# ZIP, sync) la build vecchia viene messa da parte, mai cancellata, e se ne crea una nuova
# nello stesso posto: runner, test e gate usano sempre lo stesso percorso.
BUILD_DIR="${BUILD_DIR:-UFFICIO/Officina/build}"
if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
  src_l=$(cd motore_nuovo && pwd); src_p=$(cd motore_nuovo && pwd -P)
  bld_l=$(cd "$BUILD_DIR" && pwd); bld_p=$(cd "$BUILD_DIR" && pwd -P)
  src_c=$(sed -n 's/^CMAKE_HOME_DIRECTORY:INTERNAL=//p' "$BUILD_DIR/CMakeCache.txt" | head -n 1)
  bld_c=$(sed -n 's/^CMAKE_CACHEFILE_DIR:INTERNAL=//p' "$BUILD_DIR/CMakeCache.txt" | head -n 1)
  cache_altrove=0
  if [ -n "$src_c" ] && [ "$src_c" != "$src_l" ] && [ "$src_c" != "$src_p" ]; then cache_altrove=1; fi
  if [ -n "$bld_c" ] && [ "$bld_c" != "$bld_l" ] && [ "$bld_c" != "$bld_p" ]; then cache_altrove=1; fi
  if [ "$cache_altrove" -eq 1 ]; then
    vecchia="${BUILD_DIR}_vecchia_$(date +%Y%m%d_%H%M%S)"
    mv "$BUILD_DIR" "$vecchia" || { echo "ERRORE: non riesco a spostare la build vecchia $BUILD_DIR." >&2; exit 12; }
    echo "Cache CMake creata in un altro percorso ($src_c): build vecchia spostata in $vecchia, ne creo una nuova in $BUILD_DIR." >&2
  fi
fi
