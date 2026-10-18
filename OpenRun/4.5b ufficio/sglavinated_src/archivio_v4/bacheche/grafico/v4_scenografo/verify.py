import os, json
from PIL import Image
root="assets_originali/scenari"
livelli=["lv_01_neon","lv_02_atomico","lv_03_foglie"]
varianti=["A","B","C"]
errori=[]
for lv in livelli:
    for v in varianti:
        d=os.path.join(root,lv,f"{lv.split('_')[0] if False else ''}")
    # percorso reale: sotto lv_* le cartelle si chiamano scenario_<nome>_varianteX
# mappa livello -> nome scenario
nomi={"lv_01_neon":"scenario_neon_01","lv_02_atomico":"scenario_atomico_02","lv_03_foglie":"scenario_foglie_03"}
for lv in livelli:
    nome=nomi[lv]
    for v in varianti:
        d=os.path.join(root,lv,f"{nome}_variante{v}")
        files=os.listdir(d)
        obbligati=["skyline.png","palette.png","palette.json","manifest.json","oggetti"]
        for o in obbligati:
            if o not in files:
                errori.append(f"{d}: manca {o}")
        objs=[f for f in os.listdir(os.path.join(d,"oggetti")) if f.endswith(".png")]
        if not (4 <= len(objs) <= 8):
            errori.append(f"{d}: {len(objs)} oggetti (devono essere 4-8): {objs}")
        # colori palette
        pj=json.load(open(os.path.join(d,"palette.json")))
        if len(pj) > 16:
            errori.append(f"{d}: palette ha {len(pj)} colori (>16)")
        # colori effettivi skyline
        im=Image.open(os.path.join(d,"skyline.png")).convert("RGBA")
        colori=set(im.getdata())
        if len(colori) > 17:  # 16 + trasparente
            errori.append(f"{d}/skyline.png: {len(colori)} colori RGBA (max 17 con trasparente)")
        m=json.load(open(os.path.join(d,"manifest.json")))
        if m.get("livello")!=lv or m.get("variante")!=v:
            errori.append(f"{d}: manifest livello/variante incoerenti")
        if m.get("palette_max_colori")!=16:
            errori.append(f"{d}: palette_max_colori non 16")
        print(f"[VERIF] {lv} v{v}: {len(objs)} oggetti, palette {len(pj)} colori, skyline {len(colori)} RGBA, manifest OK")
if errori:
    print("ERRORI:")
    for e in errori: print(" -",e)
else:
    print("TUTTA LA VERIFICA PASSATA: 9 varianti, 4-8 oggetti ciascuna, palette <=16, manifest OK")
