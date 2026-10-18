#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
# Genera sprite cartelloni sponsor per Sglavinated Edition v3 (open).
# Disegnati da zero con Pillow, nessun asset Sega. Tutti fittizi.
import os, json
from PIL import Image, ImageDraw

BASE = os.path.dirname(os.path.abspath(__file__))

# Palette per stile livello
PAL = {
    'neon':    {'bg': (20, 5, 40),    'fg': (255, 0, 200),   'txt': (0, 255, 255),   'accent': (255, 255, 0)},
    'atomico': {'bg': (40, 50, 45),   'fg': (255, 180, 100),  'txt': (255, 255, 220),  'accent': (200, 60, 60)},
    'foglie':  {'bg': (15, 40, 15),   'fg': (180, 255, 60),   'txt': (255, 200, 255),  'accent': (255, 100, 200)},
}

# --- Tutti gli sponsor ---
SPONSORS = {
    'curve_pericolose': {
        'nome_display': 'CURVE PERICOLOSE',
        'slogan': 'appoggiati al peggio',
        'desc': 'Sponsor fittizio di intimo, doppio senso guida+corpo',
        'icon_fn': 'icon_curva',  # tipo di icona
    },
    'brinis_udine': {
        'nome_display': 'BRINIS UDINE',
        'slogan': 'Pezzi caldi per motorini che non si fermano mai!',
        'desc': 'Ricambi per motorini, insegna officina italiana',
        'slogan_alt': [
            'Diamo una spinta alle tue curve.',
            'Dal 50ino al maxiscooter, abbiamo il pezzo che lo accende.',
        ],
        'icon_fn': 'icon_motorino',
    },
    'piston_high': {
        'nome_display': 'PISTON HIGH',
        'slogan': 'Erba del Pilota',
        'desc': 'Gioco di parole motoristico: pistone + high. Livrea da corsa.',
        'icon_fn': 'icon_pistone',
    },
    'canna_bis': {
        'nome_display': 'CANNA-BIS',
        'slogan': "L'unico sponsor che ti fa andare forte... rimanendo piantato.",
        'desc': 'Ironia: sponsor velocita con prodotto che rallenta',
        'icon_fn': 'icon_foglia',
    },
    'slow_down': {
        'nome_display': 'SLOW DOWN',
        'slogan': 'Pastiglie per freni molto rilassati',
        'desc': 'Pastiglie freno ironiche',
        'icon_fn': 'icon_freno',
    },
    'erba_medica': {
        'nome_display': 'ERBA MEDICA',
        'slogan': 'Ammortizzatori ad alta quota',
        'desc': 'Ammortizzatori con doppio senso erba/altitudine',
        'icon_fn': 'icon_molla',
    },
    'green_box': {
        'nome_display': 'GREEN BOX',
        'slogan': 'Il Pit Stop della felicita',
        'desc': 'Pit stop ironico, box verde',
        'icon_fn': 'icon_box',
    },
}

def draw_icon(d, icon_type, x, y, pal):
    """Disegna icona stilizzata 20x20 nell'angolo del cartellone."""
    c = pal['accent']
    if icon_type == 'icon_curva':
        d.arc([x, y, x+18, y+18], 180, 360, fill=c, width=2)
    elif icon_type == 'icon_motorino':
        d.ellipse([x, y+10, x+8, y+18], outline=c, width=1)
        d.ellipse([x+10, y+10, x+18, y+18], outline=c, width=1)
        d.polygon([(x+2,y+10),(x+6,y+2),(x+14,y+2),(x+16,y+10)], outline=c)
    elif icon_type == 'icon_pistone':
        d.rectangle([x+4, y, x+14, y+12], outline=c, width=1)
        d.line([(x+9, y+12), (x+9, y+18)], fill=c, width=2)
    elif icon_type == 'icon_foglia':
        d.ellipse([x+2, y, x+16, y+14], outline=c, width=1)
        d.line([(x+9, y+14), (x+9, y+18)], fill=c, width=1)
        d.line([(x+5, y+5), (x+13, y+10)], fill=c)
    elif icon_type == 'icon_freno':
        d.ellipse([x+2, y+2, x+16, y+16], outline=c, width=2)
        d.rectangle([x+6, y+6, x+12, y+12], fill=c)
    elif icon_type == 'icon_molla':
        for i in range(4):
            yy = y + i*4
            d.line([(x+3, yy), (x+15, yy+2)], fill=c, width=1)
    elif icon_type == 'icon_box':
        d.rectangle([x+2, y+2, x+16, y+16], outline=(0,180,0), width=2)
        d.rectangle([x+6, y+6, x+12, y+12], fill=(0,220,0))

def draw_billboard(sponsor, style, pal):
    """Cartellone 128x64 per uno sponsor in uno stile."""
    img = Image.new('RGBA', (128, 64), pal['bg'])
    d = ImageDraw.Draw(img)
    # Bordo
    d.rectangle([1, 1, 126, 62], outline=pal['fg'], width=2)
    # Nome
    name = sponsor['nome_display']
    d.text((6, 4), name, fill=pal['txt'])
    # Slogan (troncato se lungo)
    slogan = sponsor['slogan']
    if len(slogan) > 28:
        # Spezza in 2 righe
        mid = len(slogan)//2
        sp = slogan.rfind(' ', 0, mid+5)
        if sp < 0: sp = mid
        d.text((6, 28), slogan[:sp], fill=pal['accent'])
        d.text((6, 40), slogan[sp+1:], fill=pal['accent'])
    else:
        d.text((6, 34), slogan, fill=pal['accent'])
    # Icona
    draw_icon(d, sponsor['icon_fn'], 100, 6, pal)
    # Linea decorativa
    d.line([(4, 24), (124, 24)], fill=pal['fg'], width=1)
    return img

def main():
    for sname, sdata in SPONSORS.items():
        sdir = os.path.join(BASE, sname)
        os.makedirs(sdir, exist_ok=True)

        files = []
        for style, pal in PAL.items():
            img = draw_billboard(sdata, style, pal)
            fname = f'cartellone_{style}.png'
            img.save(os.path.join(sdir, fname))
            files.append(fname)
            print(f'  {sname}/{fname}')

        manifest = {
            "nome": sname,
            "nome_display": sdata['nome_display'],
            "slogan": sdata['slogan'],
            "descrizione": sdata['desc'],
            "autore": "genera_sponsor.py",
            "licenza": "CC0-1.0",
            "originale": True,
            "riferimenti_sega": False,
            "file": files,
        }
        if 'slogan_alt' in sdata:
            manifest['slogan_alt'] = sdata['slogan_alt']

        with open(os.path.join(sdir, 'manifest.json'), 'w') as f:
            json.dump(manifest, f, indent=2, ensure_ascii=False)

    print(f'\nTotale: {len(SPONSORS)} sponsor x {len(PAL)} stili = {len(SPONSORS)*len(PAL)} cartelloni')

if __name__ == '__main__':
    main()
