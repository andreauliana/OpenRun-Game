#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# Completa DECO1 per lv_01_neon, lv_02_atomico, lv_03_foglie:
# converte i PNG esistenti in .sglraw e aggiunge i campi "raw" a CATALOGO_GRAFICO.json.
# Formato output: "SGLR" + u32 LE larghezza + u32 LE altezza + byte RGBA8888.
# Lanciare da sglavinated_src/: python3 "UFFICIO/Dipartimento Grafico/TOOLS/completa_deco1_lv123.py"
import struct, zlib, json
from pathlib import Path

SRC = Path(__file__).resolve().parents[3]  # sglavinated_src, qualunque sia la cartella da cui si lancia
LIBRERIA = SRC / "UFFICIO/Dipartimento Grafico/Libreria/assets_originali"
RAW_ROOT = SRC / "motore_nuovo/dati/sprite_raw"
CATALOGO = LIBRERIA / "CATALOGO_GRAFICO.json"
MAGIC = b"SGLR"

def write_sglraw(png_path, out_path):
    data = Path(png_path).read_bytes()
    assert data[:8] == b"\x89PNG\r\n\x1a\n", f"{png_path}: non PNG"
    pos = 8
    idat = bytearray(); ihdr = None
    while pos < len(data):
        (length,) = struct.unpack_from(">I", data, pos); pos += 4
        ctype = data[pos:pos+4]; pos += 4
        cdata = data[pos:pos+length]; pos += length; pos += 4
        if ctype == b"IHDR": ihdr = cdata
        elif ctype == b"IDAT": idat += cdata
        elif ctype == b"IEND": break
    w, h, bd, ct, comp, filt, inter = struct.unpack(">IIBBBBB", ihdr)
    raw = zlib.decompress(bytes(idat))
    bpp = 4 if ct == 6 else 3
    stride = w * bpp
    out = bytearray(w * h * 4)
    prev = bytearray(stride); i = 0
    for y in range(h):
        f = raw[i]; i += 1
        line = bytearray(raw[i:i+stride]); i += stride
        if f == 1:
            for x in range(bpp, stride): line[x] = (line[x] + line[x-bpp]) & 0xFF
        elif f == 2:
            for x in range(stride): line[x] = (line[x] + prev[x]) & 0xFF
        elif f == 3:
            for x in range(stride):
                a = line[x-bpp] if x >= bpp else 0
                line[x] = (line[x] + ((a + prev[x]) >> 1)) & 0xFF
        elif f == 4:
            for x in range(stride):
                a = line[x-bpp] if x >= bpp else 0
                b = prev[x]; c = prev[x-bpp] if x >= bpp else 0
                p = a + b - c
                pa, pb, pc = abs(p-a), abs(p-b), abs(p-c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                line[x] = (line[x] + pr) & 0xFF
        prev = line
        base = y * w * 4
        if ct == 6:
            out[base:base+stride] = line
        else:
            for x in range(w):
                out[base+4*x] = line[3*x]; out[base+4*x+1] = line[3*x+1]
                out[base+4*x+2] = line[3*x+2]; out[base+4*x+3] = 255
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "wb") as f:
        f.write(MAGIC); f.write(struct.pack("<II", w, h)); f.write(bytes(out))
    return w, h

# (id_catalogo, src_rel_png, dest_raw_rel_sotto_sprite_raw)
CONVERSIONI = [
    ("scenario.lv_01_neon.skyline",
     "scenari/lv_01_neon/scenario_neon_01_varianteA/skyline.png",
     "scenari/lv_01_neon/skyline.sglraw"),
    ("oggetto.lv_01_neon.bidone",
     "scenari/lv_01_neon/scenario_neon_01_varianteA/oggetti/obj_bidone.png",
     "scenari/lv_01_neon/bidone.sglraw"),
    ("oggetto.lv_01_neon.cassetta",
     "scenari/lv_01_neon/scenario_neon_01_varianteA/oggetti/obj_cassetta.png",
     "scenari/lv_01_neon/cassetta.sglraw"),
    ("oggetto.lv_01_neon.lampione",
     "scenari/lv_01_neon/scenario_neon_01_varianteA/oggetti/obj_lampione.png",
     "scenari/lv_01_neon/lampione.sglraw"),
    ("oggetto.lv_01_neon.palina",
     "scenari/lv_01_neon/scenario_neon_01_varianteA/oggetti/obj_palina.png",
     "scenari/lv_01_neon/palina.sglraw"),
    ("oggetto.lv_01_neon.palo_neon",
     "scenari/lv_01_neon/scenario_neon_01_varianteA/oggetti/obj_palo_neon.png",
     "scenari/lv_01_neon/palo_neon.sglraw"),
    ("scenario.lv_02_atomico.skyline",
     "scenari/lv_02_atomico/scenario_atomico_02_varianteA/skyline.png",
     "scenari/lv_02_atomico/skyline.sglraw"),
    ("oggetto.lv_02_atomico.bancone_drive_in",
     "scenari/lv_02_atomico/scenario_atomico_02_varianteA/oggetti/obj_bancone_drive_in.png",
     "scenari/lv_02_atomico/bancone_drive_in.sglraw"),
    ("oggetto.lv_02_atomico.cupola",
     "scenari/lv_02_atomico/scenario_atomico_02_varianteA/oggetti/obj_cupola.png",
     "scenari/lv_02_atomico/cupola.sglraw"),
    ("oggetto.lv_02_atomico.freccia",
     "scenari/lv_02_atomico/scenario_atomico_02_varianteA/oggetti/obj_freccia.png",
     "scenari/lv_02_atomico/freccia.sglraw"),
    ("oggetto.lv_02_atomico.panchina_cromata",
     "scenari/lv_02_atomico/scenario_atomico_02_varianteA/oggetti/obj_panchina_cromata.png",
     "scenari/lv_02_atomico/panchina_cromata.sglraw"),
    ("oggetto.lv_02_atomico.segnale_atomico",
     "scenari/lv_02_atomico/scenario_atomico_02_varianteA/oggetti/obj_segnale_atomico.png",
     "scenari/lv_02_atomico/segnale_atomico.sglraw"),
    ("oggetto.lv_02_atomico.torre_razzo",
     "scenari/lv_02_atomico/scenario_atomico_02_varianteA/oggetti/obj_torre_razzo.png",
     "scenari/lv_02_atomico/torre_razzo.sglraw"),
    ("scenario.lv_03_foglie.skyline",
     "scenari/lv_03_foglie/scenario_foglie_03_varianteA/skyline.png",
     "scenari/lv_03_foglie/skyline.sglraw"),
    ("oggetto.lv_03_foglie.foglia_gigante",
     "scenari/lv_03_foglie/scenario_foglie_03_varianteA/oggetti/obj_foglia_gigante.png",
     "scenari/lv_03_foglie/foglia_gigante.sglraw"),
    ("oggetto.lv_03_foglie.fungo_psico",
     "scenari/lv_03_foglie/scenario_foglie_03_varianteA/oggetti/obj_fungo_psico.png",
     "scenari/lv_03_foglie/fungo_psico.sglraw"),
    ("oggetto.lv_03_foglie.insegna_fumosa",
     "scenari/lv_03_foglie/scenario_foglie_03_varianteA/oggetti/obj_insegna_fumosa.png",
     "scenari/lv_03_foglie/insegna_fumosa.sglraw"),
    ("oggetto.lv_03_foglie.panchina_curva",
     "scenari/lv_03_foglie/scenario_foglie_03_varianteA/oggetti/obj_panchina_curva.png",
     "scenari/lv_03_foglie/panchina_curva.sglraw"),
    ("oggetto.lv_03_foglie.panchina_fumo",
     "scenari/lv_03_foglie/scenario_foglie_03_varianteA/oggetti/obj_panchina_fumo.png",
     "scenari/lv_03_foglie/panchina_fumo.sglraw"),
]

# 1. Converti PNG -> raw
convertiti = []
for cat_id, src_rel, dest_rel in CONVERSIONI:
    src = LIBRERIA / src_rel
    if not src.exists():
        print(f"SALTA (manca sorgente): {src_rel}")
        continue
    dst = RAW_ROOT / dest_rel
    w, h = write_sglraw(src, dst)
    convertiti.append((cat_id, dest_rel, w, h))
    print(f"RAW {dst} ({w}x{h}) da {src_rel}")

# 2. Aggiorna catalogo: aggiungi "raw" alle voci corrispondenti che non lo hanno
cat = json.loads(CATALOGO.read_text(encoding="utf-8"))
aggiornati = 0
for e in cat["elementi"]:
    for cat_id, dest_rel, w, h in convertiti:
        if e["id"] == cat_id and "raw" not in e:
            e["raw"] = f"motore_nuovo/dati/sprite_raw/{dest_rel}"
            aggiornati += 1
            break

CATALOGO.write_text(json.dumps(cat, indent=3, ensure_ascii=False) + "\n", encoding="utf-8")
print(f"\nConvertiti {len(convertiti)} raw, aggiornate {aggiornati} voci catalogo.")
