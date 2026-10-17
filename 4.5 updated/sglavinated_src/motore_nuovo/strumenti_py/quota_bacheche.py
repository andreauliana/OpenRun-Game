#!/usr/bin/env python3
"""Controlla lo spazio delle bacheche personali (default 5 MB ciascuna, limite di Andrea). Esce 1 se qualcuna supera il limite."""
import pathlib, sys
proj = pathlib.Path(__file__).resolve().parent.parent.parent
lim = float(sys.argv[1]) if len(sys.argv) > 1 else 5.0
bad = 0
for d in sorted((proj/"bacheche").glob("*")):
    if d.is_dir():
        mb = sum(f.stat().st_size for f in d.rglob("*") if f.is_file())/1048576
        flag = "  SUPERA IL LIMITE: libera spazio (comprimi o cancella bozze vecchie)" if mb > lim else ""
        if flag: bad = 1
        if mb > 1 or flag: print(f"bacheca {d.name}: {mb:.1f} MB{flag}")
print("bacheche controllate, limite", lim, "MB")
sys.exit(bad)
