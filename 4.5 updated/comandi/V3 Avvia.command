#!/bin/sh
cd "$(dirname "$0")" || exit 1
sh v3.sh start
status=$?
if [ -t 0 ]; then
  echo
  printf "Premi Invio per chiudere..."
  read _
fi
exit "$status"
