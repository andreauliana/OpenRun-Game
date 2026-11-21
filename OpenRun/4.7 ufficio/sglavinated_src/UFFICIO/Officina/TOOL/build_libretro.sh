#!/bin/sh
# SPDX-License-Identifier: MIT
# Compila il core libretro (target sglavinated_libretro) per il blocco V6.
# Uso: sh UFFICIO/Officina/TOOL/build_libretro.sh
# Env opzionale: BUILD_DIR (predefinita UFFICIO/Officina/build, quella standard).
cd "$(dirname "$0")/../../.." || exit 9  # TOOL -> Officina -> UFFICIO -> radice

BUILD_DIR="${BUILD_DIR:-UFFICIO/Officina/build}"
mkdir -p "$(dirname "$BUILD_DIR")" || exit 12
BUILD_DIR="$(cd "$(dirname "$BUILD_DIR")" && pwd -P)/$(basename "$BUILD_DIR")"
. ./UFFICIO/Officina/TOOL/prepara_build.sh

cmake -S motore_nuovo -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DBUILD_LIBRETRO=ON || exit 10
cmake --build "$BUILD_DIR" --target sglavinated_libretro || exit 11
echo "Core libretro: $BUILD_DIR/sglavinated_libretro.*"
