#!/bin/sh
# SPDX-License-Identifier: MIT
# CMake/CTest entry point for agents and local test runs.
cd "$(dirname "$0")/.." || exit 9

BUILD_DIR="${BUILD_DIR:-UFFICIO/Officina/build}"
mkdir -p "$(dirname "$BUILD_DIR")" || exit 12
BUILD_DIR="$(cd "$(dirname "$BUILD_DIR")" && pwd -P)/$(basename "$BUILD_DIR")"
GATE_DIR="$(pwd -P)/build_gate"

case "$BUILD_DIR/" in
  "$GATE_DIR/"*)
    echo "BUILD_DIR non puo' stare dentro build_gate: usare UFFICIO/Officina/build o un percorso separato."
    exit 12
    ;;
esac
case "$GATE_DIR/" in
  "$BUILD_DIR/"*)
    echo "BUILD_DIR non puo' contenere build_gate: usare UFFICIO/Officina/build o un percorso separato."
    exit 12
    ;;
esac

cmake -S motore_nuovo -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release || exit 10
cmake --build "$BUILD_DIR" --target prova_gate || exit 11
ctest --test-dir "$BUILD_DIR" --output-on-failure
