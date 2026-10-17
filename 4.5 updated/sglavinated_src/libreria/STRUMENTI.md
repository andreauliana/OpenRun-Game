# Strumenti del progetto (generato da indice_strumenti.py: non modificare a mano)

Tutti li usano; li modifica solo il ruolo proprietario. Per un cambio, chiedilo nel tuo diario.
Prima di scrivere uno strumento nuovo controlla qui se esiste gia'.

| Strumento | Proprietario | Scopo | Uso (dalla radice sglavinated_src) |
|---|---|---|---|
| `tools/gate_t1.sh` | Claude | Gate Tappa 1 in UN SOLO comando: configura, compila, lancia prova_gate sui 3 livelli. | `sh tools/gate_t1.sh` |
| `assets_originali/genera_bloccoF.py` | grafico | Generatore asset BLOCCO F — Outrun Sglavinated Edition v3 (open) | `python3 assets_originali/genera_bloccoF.py` |
| `assets_originali/sponsor/genera_sponsor.py` | grafico | Genera sprite cartelloni sponsor per Sglavinated Edition v3 (open). | `python3 assets_originali/sponsor/genera_sponsor.py` |
| `assets_originali/sprite/auto_gt/genera_auto.py` | grafico | Genera sagoma auto GT europea anni '70-'90 per Sglavinated Edition v3. | `python3 assets_originali/sprite/auto_gt/genera_auto.py` |
| `assets_originali/ui/genera_ui_b.py` | grafico | genera_ui_b.py - rigenera gli asset UI della VARIANTE B in bianco e nero (D7). | `python3 assets_originali/ui/genera_ui_b.py` |
| `motore_nuovo/strumenti_py/check_catalogo.py` | sviluppatore | Fallisce se dati/personaggi/* e CATALOGO_PERSONAGGI.md non coincidono, o se manca il manifest. | `python3 motore_nuovo/strumenti_py/check_catalogo.py` |
| `motore_nuovo/strumenti_py/check_catalogo_grafico.py` | sviluppatore | Controlla assets_originali/CATALOGO_GRAFICO.json (lo scrive @grafico). | `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` |
| `motore_nuovo/strumenti_py/galleria.py` | sviluppatore | Genera assets_originali/galleria/galleria.html: foto di tutti i PNG per cartella (ogni team vede il lavoro degli altri). | `python3 motore_nuovo/strumenti_py/galleria.py` |
| `motore_nuovo/strumenti_py/indice_assets.py` | sviluppatore | Indice unico degli asset di assets_originali/ (libreria interna). | `python3 motore_nuovo/strumenti_py/indice_assets.py` |
| `motore_nuovo/strumenti_py/indice_strumenti.py` | sviluppatore | Registro comune degli strumenti (script) che gli agenti costruiscono. | `python3 motore_nuovo/strumenti_py/indice_strumenti.py` |
| `motore_nuovo/strumenti_py/png_a_raw.py` | sviluppatore | png_a_raw.py — converte i PNG dei cartelloni sponsor in raw per il core C++. | `python3 motore_nuovo/strumenti_py/png_a_raw.py` |
| `motore_nuovo/strumenti_py/quota_bacheche.py` | sviluppatore | Controlla lo spazio delle bacheche personali (default 5 MB ciascuna, limite di Andrea). Esce 1 se qualcuna supera il limite. | `python3 motore_nuovo/strumenti_py/quota_bacheche.py` |
| `motore_nuovo/strumenti_py/quota_open.py` | sviluppatore | Quota open source: % di byte (codice+asset+dati) con licenza aperta dichiarata. Soglia: 50%. Uso: python3 quota_open.py [soglia] | `python3 motore_nuovo/strumenti_py/quota_open.py` |
