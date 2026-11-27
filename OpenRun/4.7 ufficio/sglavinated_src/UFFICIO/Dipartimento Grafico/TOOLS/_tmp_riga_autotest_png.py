#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Trasformazione una tantum: estrae le 34 righe di esito autotest inline di
vg3_png_non_catalogati.py nella funzione pura riga_autotest(nome, ok).

Input : UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py (letto)
Output: lo STESSO file (sovrascrittura intenzionale, una sola volta).
Formato preservato: 'autotest <nome>: OK/FALLITO' (minuscolo, come nel tool).
Verifica: il numero di sostituzioni deve essere esattamente 34.
"""
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[3]
TARGET = ROOT / "UFFICIO/Dipartimento Grafico/TOOLS/vg3_png_non_catalogati.py"

testo = TARGET.read_text(encoding="utf-8")

# 1) nota in testa, dopo l'ultima aggiunta (turno 9)
ancora_nota = """globale. La chiamata in esito_noti() lascia il default, output invariato.
\"\"\"
"""
nota_nuova = """globale. La chiamata in esito_noti() lascia il default, output invariato.

Aggiunta 27/11 (turno 10): le 34 righe di esito dell'autotest (prima concatenate
inline con 'autotest <nome>: ' + ('OK'/'FALLITO')) sono ora la funzione pura
riga_autotest(nome, ok), provata in autotest: il formato minuscolo di questo tool
resta identico e sta in un punto solo. L'output di main() non cambia.
\"\"\"
"""
assert testo.count(ancora_nota) == 1, "nota in testa: ancoraggio non unico"
testo = testo.replace(ancora_nota, nota_nuova, 1)

# 2) definizione della funzione pura, subito prima di def autotest()
ancora_def = "def autotest():\n"
funzione = '''def riga_autotest(nome, ok):
    """Riga di esito di una prova di autotest (funzione pura).

    Erano 34 concatenazioni inline nel corpo di autotest(): qui si prova in
    isolamento il formato ('OK' / 'FALLITO'), identico a prima (minuscolo
    'autotest', come in questo tool). Nessun disco, nessuna stampa.
    """
    return f"autotest {nome}: {'OK' if ok else 'FALLITO'}"


'''
assert testo.count(ancora_def) == 1, "def autotest: ancoraggio non unico"
testo = testo.replace(ancora_def, funzione + ancora_def, 1)

# 3) sostituisce le 34 print inline con la funzione pura
pattern = re.compile(r'print\("autotest ([^"]+): " \+ \("OK" if (\w+) else "FALLITO"\)\)')
testo, n = pattern.subn(lambda m: f'print(riga_autotest("{m.group(1)}", {m.group(2)}))', testo)
assert n == 34, f"attese 34 sostituzioni, trovate {n}"

# 4) prova dedicata, prima del return finale di autotest()
ancora_return = "    print(riga_autotest(\"carica_se_esiste\", ok_carica_se))\n    return 0 if ok else 1\n"
prova = '''    print(riga_autotest("carica_se_esiste", ok_carica_se))
    # riga_autotest: formato 'autotest <nome>: OK/FALLITO' identico a prima delle
    # 34 righe inline; provato in isolamento (funzione pura, nessun disco).
    ok_riga_autotest = (
        riga_autotest("x", True) == "autotest x: OK"
        and riga_autotest("x", False) == "autotest x: FALLITO"
    )
    ok = ok and ok_riga_autotest
    print(riga_autotest("riga_autotest", ok_riga_autotest))
    return 0 if ok else 1
'''
assert testo.count(ancora_return) == 1, "return autotest: ancoraggio non unico"
testo = testo.replace(ancora_return, prova, 1)

TARGET.write_text(testo, encoding="utf-8")
print(f"OK: 34 print inline -> riga_autotest; scritto {TARGET}")
