#!/bin/sh
BASE="$(cd "$(dirname "$0")/.." && pwd)" || exit 1
exec sh "$BASE/run.sh" "$@"
