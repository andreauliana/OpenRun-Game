import os, shutil
coppie = [
    ("scenario_neon_01_varianteA","lv_01_neon","scenario_neon_01_varianteA"),
    ("scenario_neon_01_varianteB","lv_01_neon","scenario_neon_01_varianteB"),
    ("scenario_neon_01_varianteC","lv_01_neon","scenario_neon_01_varianteC"),
    ("scenario_atomico_02_varianteA","lv_02_atomico","scenario_atomico_02_varianteA"),
    ("scenario_atomico_02_varianteB","lv_02_atomico","scenario_atomico_02_varianteB"),
    ("scenario_atomico_02_varianteC","lv_02_atomico","scenario_atomico_02_varianteC"),
    ("scenario_foglie_03_varianteA","lv_03_foglie","scenario_foglie_03_varianteA"),
    ("scenario_foglie_03_varianteB","lv_03_foglie","scenario_foglie_03_varianteB"),
    ("scenario_foglie_03_varianteC","lv_03_foglie","scenario_foglie_03_varianteC"),
]
root="assets_originali/scenari"
for flat,lv,var in coppie:
    src=os.path.join(root,flat)
    dst=os.path.join(root,lv,var)
    os.makedirs(dst,exist_ok=True)
    for f in os.listdir(src):
        shutil.move(os.path.join(src,f), os.path.join(dst,f))
    print("spostato", flat, "->", dst)
print("FATTO")
