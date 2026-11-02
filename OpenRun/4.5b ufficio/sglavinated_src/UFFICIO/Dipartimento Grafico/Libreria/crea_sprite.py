import json
import os
from PIL import Image, ImageDraw

BASE = "/Users/andreauliana/Documents/GitHub/OpenRun-Game/OpenRun/4.5b ufficio/sglavinated_src/UFFICIO/Dipartimento Grafico/Libreria/assets_originali/sprite"

# Palette: 16 colors, OutRun style
PAL = {
    "nero":       (0, 0, 0),
    "bianco":     (240, 240, 240),
    "rosso":      (255, 0, 0),
    "verde":      (0, 255, 0),
    "blu":        (0, 0, 255),
    "giallo":     (255, 255, 0),
    "magenta":    (255, 0, 255),
    "ciano":      (0, 255, 255),
    "arancio":    (255, 165, 0),
    "viola":      (128, 0, 128),
    "rosa":       (255, 192, 203),
    "grigio":     (128, 128, 128),
    "grigio_chiaro": (200, 200, 200),
    "bronzo":     (212, 175, 55),
    "nero_soft":  (40, 40, 40),
    "argento":    (192, 192, 192),
}

def salva(img, percorso):
    os.makedirs(os.path.dirname(percorso), exist_ok=True)
    img.save(percorso)
    print(f"  [OK] {percorso}")

def scrivi_manifest(cartella, nome, descrizione):
    m = {
        "autore": "@grafico (progetto Sglavinated)",
        "licenza": "CC0-1.0",
        "descrizione": descrizione,
        "files": [nome],
        "originale": True,
        "riferimenti_sega": False,
    }
    path = os.path.join(cartella, "manifest.json")
    with open(path, "w", encoding="utf-8") as f:
        json.dump(m, f, ensure_ascii=False, indent=2)
    print(f"  [OK] {path}")

def crea_sprite(nome_cartella, filename, disegna_func):
    cartella = os.path.join(BASE, nome_cartella)
    os.makedirs(cartella, exist_ok=True)
    img = disegna_func(cartella)
    salva(img, os.path.join(cartella, filename))
    scrivi_manifest(cartella, filename, f"Sprite {filename} per progetto Sglavinated")
    return cartella

# Disegna Gianni (automobilista style, anni '70)
def disegna_gianni(cartella):
    W, H = 64, 32
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    
    # Corpo rosso auto
    d.polygon([(4, 20), (8, 14), (20, 10), (44, 10), (56, 14), (60, 20), (58, 24), (6, 24)],
              fill=PAL["rosso"], outline=PAL["nero"], width=1)
    # Cabina
    d.ellipse([24, 6, 40, 14], fill=PAL["bianco"], outline=PAL["nero"], width=1)
    # Finestrini
    d.ellipse([28, 8, 36, 14], fill=PAL["grigio_chiaro"])
    # Ruote
    d.ellipse([10, 22, 22, 30], fill=PAL["nero"], outline=PAL["grigio"], width=1)
    d.ellipse([42, 22, 54, 30], fill=PAL["nero"], outline=PAL["grigio"], width=1)
    # dettagli
    d.line([(16, 18), (16, 20)], fill=PAL["grigio"], width=1)
    d.line([(48, 18), (48, 20)], fill=PAL["grigio"], width=1)
    
    return img

# Disegna Gesu (spiritual style)
def disegna_gesu(cartella):
    W, H = 64, 32
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    
    # Corpo blu cielo
    d.polygon([(4, 20), (8, 14), (20, 10), (44, 10), (56, 14), (60, 20), (58, 24), (6, 24)],
              fill=PAL["blu"], outline=PAL["nero"], width=1)
    # aureola gialla
    d.ellipse([20, 4, 44, 12], fill=PAL["giallo"], outline=PAL["nero"], width=1)
    # occhi bianchi
    d.ellipse([24, 8, 28, 10], fill=PAL["bianco"])
    d.ellipse([38, 8, 42, 10], fill=PAL["bianco"])
    # mani alzate (semplificate)
    d.line([(8, 16), (12, 20)], fill=PAL["bianco"], width=2)
    d.line([(56, 16), (52, 20)], fill=PAL["bianco"], width=2)
    # ruote
    d.ellipse([10, 22, 22, 30], fill=PAL["nero"], outline=PAL["grigio"], width=1)
    d.ellipse([42, 22, 54, 30], fill=PAL["nero"], outline=PAL["grigio"], width=1)
    
    return img

# Disegna UFO
def disegna_ufo(cartella):
    W, H = 64, 32
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    d = ImageDraw.Draw(img)
    
    # Corpo UFO verde
    d.ellipse([8, 8, 56, 24], fill=PAL["verde"], outline=PAL["nero"], width=1)
    # occhi rossi
    d.ellipse([16, 12, 24, 20], fill=PAL["rosso"])
    d.ellipse([40, 12, 48, 20], fill=PAL["rosso"])
    # luce superiore
    d.rectangle([28, 4, 36, 8], fill=PAL["giallo"])
    # base
    d.polygon([(24, 24), (30, 32), (34, 24), (40, 32), (44, 24), (50, 32), (54, 24)], 
              fill=PAL["grigio"], outline=PAL["nero"], width=1)
    # dettagli laterali
    d.line([(12, 16), (12, 20)], fill=PAL["ciano"], width=1)
    d.line([(52, 16), (52, 20)], fill=PAL["ciano"], width=1)
    
    return img

print("=== Creazione sprite mancanti: Gianni, Gesu, UFO ===")

# Gianni
print("\n--- Gianni ---")
cartella_gianni = crea_sprite("gianni", "gianni.png", disegna_gianni)
scrivi_manifest(cartella_gianni, "gianni.png",
    "Sprite personaggio Gianni - automobilista stile anni '70, rosso e bianco, con dettagli")

# Gesu
print("\n--- Gesu' ---")
cartella_gesu = crea_sprite("gesu", "gesu.png", disegna_gesu)
scrivi_manifest(cartella_gesu, "gesu.png",
    "Sprite personaggio Gesu' - spirituale, blu cielo con aureola gialla")

# UFO
print("\n--- UFO ---")
cartella_ufo = crea_sprite("ufo", "ufo.png", disegna_ufo)
scrivi_manifest(cartella_ufo, "ufo.png",
    "Sprite UFO - veicolo alieno verde con occhi rossi e luce superiore")

print("\n=== Sprite creati con successo ===")