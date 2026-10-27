# Generatore oggetti alti bordo strada (blocco DECO1, passo 2f, compito @grafico).
# Produce PNG verticali (altezza > larghezza) >= 32x96, sagome piene e colorate,
# poca trasparenza (opachi), stessi nomi file dei placeholder esistenti.
# Eseguire dalla radice sglavinated_src/ con: python3 "UFFICIO/Dipartimento Grafico/TOOLS/genera_oggetti_alti.py"
import zlib
import struct
import os

PNG_SIG = b"\x89PNG\r\n\x1a\n"

def chunk(kind, data):
    c = zlib.crc32(kind + data) & 0xFFFFFFFF
    return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", c)

def ihdr(w, h):
    return chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0))

def idat(pixels, w, h):
    # pixels: list of rows, each row is bytes of RGBA (filter type 0)
    raw = b"".join(b"\x00" + row for row in pixels)
    comp = zlib.compress(raw, 6)
    return chunk(b"IDAT", comp)

def iend():
    return chunk(b"IEND", b"")

def write_png(path, w, h, pixels):
    with open(path, "wb") as f:
        f.write(PNG_SIG)
        f.write(ihdr(w, h))
        f.write(idat(pixels, w, h))
        f.write(iend())

class Canvas:
    def __init__(self, w, h):
        self.w = w
        self.h = h
        self.px = [[(0, 0, 0, 0) for _ in range(w)] for _ in range(h)]
    def rect(self, x0, y0, x1, y1, color):
        for y in range(max(0, y0), min(self.h, y1)):
            for x in range(max(0, x0), min(self.w, x1)):
                self.px[y][x] = color
    def line_v(self, x, y0, y1, color):
        for y in range(max(0, y0), min(self.h, y1)):
            if 0 <= x < self.w:
                self.px[y][x] = color
    def rows(self):
        return [bytes(c for p in row for c in p) for row in self.px]

# Colori (RGBA, opachi = poca trasparenza)
C_TRASPARENTE = (0, 0, 0, 0)
# lv_01 neon (synthwave)
NEON_CIANO = (0, 255, 255, 255)
NEON_MAGENTA = (255, 0, 255, 255)
NEON_BLU = (40, 40, 120, 255)
NEON_OSCURO = (10, 10, 40, 255)
# lv_02 atomico (sci-fi anni 50)
ATOM_VERDE = (0, 255, 120, 255)
ATOM_VERDE_SC = (0, 140, 60, 255)
ATOM_GIALLO = (255, 255, 0, 255)
ATOM_OSCURO = (20, 30, 20, 255)
# lv_04 cromo (futurista/cromo)
CROME_CHIARO = (210, 210, 220, 255)
CROME_MEDIO = (150, 150, 165, 255)
CROME_SCUR = (70, 72, 80, 255)
CROME_RIFLESSO = (240, 240, 248, 255)
# lv_06 desktop (Windows 95 vibe)
DESK_GRIGIO = (128, 128, 128, 255)
DESK_GRIGIO_CH = (192, 192, 192, 255)
DESK_SCUR = (64, 64, 64, 255)
DESK_BLU = (0, 0, 255, 255)

def lampione_neon(c):
    # lampione a stelo alto, base scura, testa luminosa ciano/magenta
    c.rect(18, 112, 30, 128, NEON_OSCURO)      # base
    c.line_v(24, 32, 112, NEON_OSCURO)         # stelo
    c.line_v(23, 32, 112, NEON_OSCURO)
    c.line_v(25, 32, 112, NEON_OSCURO)
    c.rect(14, 8, 34, 34, NEON_CIANO)          # testa lampada
    c.rect(16, 10, 32, 32, NEON_MAGENTA)       # riflesso interno
    c.rect(19, 14, 29, 22, C_TRASPARENTE)      # vetro (lascia trasparente il centro)

def palo_neon(c):
    # palo verticale sottile, testa sferica magenta
    c.rect(12, 116, 20, 128, NEON_OSCURO)      # base
    c.line_v(16, 20, 116, NEON_MAGENTA)        # stelo
    c.line_v(15, 20, 116, NEON_MAGENTA)
    c.rect(10, 4, 22, 22, NEON_MAGENTA)        # testa
    c.rect(13, 8, 19, 16, NEON_CIANO)

def torre_razzo(c):
    # torre di lancio atomica, verticale, verde con base
    c.rect(20, 128, 44, 144, ATOM_OSCURO)      # base
    c.rect(22, 96, 42, 128, ATOM_VERDE_SC)     # struttura bassa
    c.line_v(32, 40, 96, ATOM_VERDE)           # stelo centrale
    c.line_v(31, 40, 96, ATOM_VERDE)
    c.line_v(33, 40, 96, ATOM_VERDE)
    c.rect(26, 4, 38, 42, ATOM_VERDE)          # razzo
    c.rect(28, 4, 36, 14, ATOM_GIALLO)         # punta
    c.rect(29, 12, 35, 20, ATOM_GIALLO)

def lampione_lama(c):
    # lampione a lama futurista cromo, verticale
    c.rect(14, 108, 26, 128, CROME_SCUR)       # base
    c.rect(18, 96, 22, 108, CROME_MEDIO)       # attacco
    c.rect(10, 4, 30, 96, CROME_CHIARO)        # lama
    c.rect(14, 8, 26, 80, CROME_RIFLESSO)      # riflesso lama
    c.rect(20, 4, 22, 16, CROME_SCUR)          # bordo destro lama

def pilone_cromo(c):
    # pilone cromo verticale largo
    c.rect(16, 120, 32, 144, CROME_SCUR)       # base
    c.rect(18, 104, 30, 120, CROME_MEDIO)      # struttura
    c.rect(8, 0, 40, 104, CROME_CHIARO)        # pilone
    c.rect(12, 4, 36, 96, CROME_RIFLESSO)      # riflesso verticale
    c.rect(40, 0, 48, 144, CROME_MEDIO)        #ombra destra

def pali_cavi(c):
    # palo cavi desktop, verticale con traverso e cavi
    c.rect(24, 108, 32, 128, DESK_SCUR)        # base
    c.line_v(28, 28, 108, DESK_GRIGIO)         # stelo
    c.line_v(27, 28, 108, DESK_GRIGIO)
    c.line_v(29, 28, 108, DESK_GRIGIO)
    c.rect(8, 24, 48, 30, DESK_GRIGIO_CH)      # traverso cavi
    c.rect(26, 0, 30, 26, DESK_BLU)            # isolatori/testa

SPEC = [
    ("scenari/lv_01_neon/scenario_neon_01_varianteA/oggetti/obj_lampione.png", 48, 128, lampione_neon),
    ("scenari/lv_01_neon/scenario_neon_01_varianteA/oggetti/obj_palo_neon.png", 32, 128, palo_neon),
    ("scenari/lv_02_atomico/scenario_atomico_02_varianteA/oggetti/obj_torre_razzo.png", 64, 144, torre_razzo),
    ("scenari/lv_04_cromo/scenario_cromo_04_varianteA/oggetti/obj_lampione_lama.png", 40, 128, lampione_lama),
    ("scenari/lv_04_cromo/scenario_cromo_04_varianteA/oggetti/obj_pilone_cromo.png", 48, 144, pilone_cromo),
    ("scenari/lv_06_desktop/scenario_desktop_06_varianteA/oggetti/obj_pali_cavi.png", 56, 128, pali_cavi),
]

ROOT = "UFFICIO/Dipartimento Grafico/Libreria/assets_originali"
ok = True
for rel, w, h, disegna in SPEC:
    path = os.path.join(ROOT, rel)
    c = Canvas(w, h)
    disegna(c)
    write_png(path, w, h, c.rows())
    print("OK", path, w, "x", h)

print("TOTALE_OGGETTI_ALTI:", len(SPEC))
