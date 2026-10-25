import json
d = json.load(open("UFFICIO/Dipartimento Grafico/Libreria/assets_originali/CATALOGO_GRAFICO.json"))
for e in d["elementi"]:
    r = e.get("raw")
    f = e.get("file")
    if isinstance(r, list) or (isinstance(f, list) and any(isinstance(x, list) for x in f)):
        print(e["id"], "raw=", type(r), "file=", type(f))
