#!/usr/bin/env python3
# SPDX-License-Identifier: CC0-1.0
# V47 VG1: quadro cosa manca (OGGETTI_LIVELLI post gruppo B) + proposte tematiche.
# Genera anteprima in UFFICIO/Officina/Test area/grafico/v47_vg1/ (solo lettura concetti).
# Lanciare da sglavinated_src: python3 "UFFICIO/Dipartimento Grafico/TOOLS/genera_v47_vg1_quadro.py"
import struct, zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "UFFICIO" / "Officina" / "Test area" / "grafico" / "v47_vg1"
BAYER = [[0,8,2,10],[12,4,14,6],[3,11,1,9],[15,7,13,5]]
FONT = {
 'A':["01110","10001","10001","11111","10001","10001","10001"],
 'B':["11110","10001","11110","10001","10001","10001","11110"],
 'C':["01110","10001","10000","10000","10000","10001","01110"],
 'D':["11110","10001","10001","10001","10001","10001","11110"],
 'E':["11111","10000","11110","10000","10000","10000","11111"],
 'F':["11111","10000","11110","10000","10000","10000","10000"],
 'G':["01110","10001","10000","10111","10001","10001","01110"],
 'H':["10001","10001","11111","10001","10001","10001","10001"],
 'I':["11111","00100","00100","00100","00100","00100","11111"],
 'J':["00111","00010","00010","00010","00010","10010","01100"],
 'K':["10001","10010","11100","10010","10001","10001","10001"],
 'L':["10000","10000","10000","10000","10000","10000","11111"],
 'M':["10001","11011","10101","10101","10001","10001","10001"],
 'N':["10001","11001","10101","10011","10001","10001","10001"],
 'O':["01110","10001","10001","10001","10001","10001","01110"],
 'P':["11110","10001","10001","11110","10000","10000","10000"],
 'Q':["01110","10001","10001","10001","10101","10010","01101"],
 'R':["11110","10001","10001","11110","10100","10010","10001"],
 'S':["01111","10000","10000","01110","00001","00001","11110"],
 'T':["11111","00100","00100","00100","00100","00100","00100"],
 'U':["10001","10001","10001","10001","10001","10001","01110"],
 'V':["10001","10001","10001","10001","10001","01010","00100"],
 'W':["10001","10001","10001","10101","10101","11011","10001"],
 'X':["10001","01010","00100","00100","00100","01010","10001"],
 'Y':["10001","01010","00100","00100","00100","00100","00100"],
 'Z':["11111","00001","00010","00100","01000","10000","11111"],
 '0':["01110","10001","10011","10101","11001","10001","01110"],
 '1':["00100","01100","00100","00100","00100","00100","01110"],
 '2':["01110","10001","00001","00010","00100","01000","11111"],
 '3':["11110","00001","00001","01110","00001","00001","11110"],
 '4':["00010","00110","01010","10010","11111","00010","00010"],
 '5':["11111","10000","11110","00001","00001","10001","01110"],
 '6':["00110","01000","10000","11110","10001","10001","01110"],
 '7':["11111","00001","00010","00100","01000","01000","01000"],
 '8':["01110","10001","10001","01110","10001","10001","01110"],
 '9':["01110","10001","10001","01111","00001","00010","01100"],
 ' ': ["00000","00000","00000","00000","00000","00000","00000"],
 '.': ["00000","00000","00000","00000","00000","00110","00110"],
 ',': ["00000","00000","00000","00000","00110","00110","01100"],
 ':': ["00000","00110","00110","00000","00110","00110","00000"],
 '-': ["00000","00000","00000","11111","00000","00000","00000"],
 '/': ["00001","00010","00100","01000","10000","00000","00000"],
 '+': ["00000","00100","00100","11111","00100","00100","00000"],
 '(': ["00010","00100","01000","01000","01000","00100","00010"],
 ')': ["01000","00100","00010","00010","00010","00100","01000"],
 '?': ["01110","10001","00010","00100","00100","00000","00100"],
}

def scrivi_png(path, w, h, rgba):
    def chunk(t,d): return struct.pack(">I",len(d))+t+d+struct.pack(">I",zlib.crc32(t+d)&0xFFFFFFFF)
    raw=bytearray()
    for y in range(h): raw.append(0); raw.extend(rgba[y*w*4:(y+1)*w*4])
    ihdr=struct.pack(">IIBBBBB",w,h,8,6,0,0,0)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(b"\x89PNG\r\n\x1a\n"+chunk(b"IHDR",ihdr)+chunk(b"IDAT",zlib.compress(bytes(raw),9))+chunk(b"IEND",b""))

def put(buf,w,h,x,y,c):
    if 0<=x<w and 0<=y<h:
        i=(y*w+x)*4; buf[i:i+4]=bytes(c)

def dither(c0,c1,x,y): return c0 if BAYER[y&3][x&3]<8 else c1

def text(buf,w,h,x,y,s,fg,scale=1):
    cx=x
    for ch in s.upper():
        g=FONT.get(ch,FONT[' '])
        for ry,row in enumerate(g):
            for rx,px in enumerate(row):
                if px=='1':
                    for sy in range(scale):
                        for sx in range(scale): put(buf,w,h,cx+rx*scale+sx,y+ry*scale+sy,fg)
        cx+=(len(g[0])+1)*scale

def box(buf,w,h,x0,y0,x1,y1,fill,edge):
    for y in range(y0,y1):
        for x in range(x0,x1):
            c=edge if x in (x0,x1-1) or y in (y0,y1-1) else dither(fill,(fill[0]-12 if fill[0]>12 else 0,fill[1]-12 if fill[1]>12 else 0,fill[2]-12 if fill[2]>12 else 0,255),x,y)
            put(buf,w,h,x,y,c)

def main():
    W,H=480,320
    buf=bytearray(W*H*4)
    BG=(18,20,28,255); FG=(220,224,232,255); DIM=(140,148,160,255); ACC=(120,180,220,255); WARN=(220,180,100,255)
    for y in range(H):
        for x in range(W): put(buf,W,H,x,y,dither(BG,(24,26,34,255),x,y))
    text(buf,W,H,8,6,"V47 VG1 - QUADRO COSA MANCA (GRUPPO B CANCELLATO)",ACC,1)
    text(buf,W,H,8,16,"PROPOSTE = ANTEPRIME CONCETTO. ANDREA RIDISEGNA PNG FINALI.",WARN,1)
    rows=[
      ("LV01 NEON", (70,40,90,255), "+INSEGNA NEON, PANCHINA, RIFIUTO GLOW"),
      ("LV02 ATOMICO",(60,55,65,255), "+CAVI/BUNDLE, MOTORE/PEZZO, CARTELLO STELLE"),
      ("LV03 FOGLIE", (45,70,50,255), "+PALMA, BOTTIGLIA NO BRAND, ASCIUGAMANO"),
      ("LV04 CROMO",  (40,55,70,255), "+ARCADE SILHOUETTE, POSTER FRAME, GRID SIGN"),
      ("LV05 SPRAY",  (65,60,60,255), "+FERMATA/BUS STOP, SEGNALE TRANSITO (NO PERSONE)"),
      ("LV06 DESKTOP",(50,60,80,255), "+UCCELLO, ICONE OS EXTRA, TILE CIRCUITO"),
    ]
    y=30
    for name,col,prop in rows:
        box(buf,W,H,6,y,474,y+44,col,(col[0]+40,min(255,col[1]+40),min(255,col[2]+40),255))
        text(buf,W,H,12,y+5,name,FG,1)
        text(buf,W,H,12,y+16,prop,FG,1)
        text(buf,W,H,12,y+28,"STESSA MISURA SE SOSTITUISCE; NUOVO=VOCE CATALOGO+RAW",DIM,1)
        y+=46
    text(buf,W,H,8,308,"GRUPPO B: NON RIGENERARE. SENSIBILI (PINUP/NUDI): SOLO ANDREA.",WARN,1)
    out=OUT/"v47_vg1_quadro_mancanze.png"
    scrivi_png(out,W,H,bytes(buf))
    print("OK",out)

if __name__=="__main__":
    main()
