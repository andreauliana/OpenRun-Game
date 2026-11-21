#!/bin/sh
cd "$(dirname "$0")" || exit 1
sh v3.sh stop
status=$?
if [ -t 0 ]; then
  echo
  printf "Premi Invio per chiudere..."
  read _
fi
exit "$status"
