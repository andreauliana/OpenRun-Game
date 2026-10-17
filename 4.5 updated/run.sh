#!/bin/sh
# Entra nel progetto e avvia OpenCode (agenti e comandi sono gia' nel progetto).
BASE="$(cd "$(dirname "$0")" && pwd)" || exit 1
cd "$BASE/sglavinated_src" || exit 1
exec opencode "$@"
