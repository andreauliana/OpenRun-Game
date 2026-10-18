# TOOLS — Dipartimento Grafico

Destinazione degli strumenti creati e mantenuti dal Designer. In questa migrazione graduale gli script eseguibili restano nelle loro cartelle sorgente: questa pagina è l'indice operativo, non una copia degli strumenti.

Eseguire i comandi dalla radice `sglavinated_src/`. L'agente Designer ha accesso ai quattro strumenti grafici elencati; gli strumenti che generano file vanno eseguiti solo dopo aver verificato i percorsi di output.

| Strumento | Comando | Funzione e output |
|---|---|---|
| Verifica catalogo grafico | `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` | Sola lettura; controlla ID, stati e riferimenti PNG/raw, avvisa sui PNG non catalogati. |
| Indice asset | `python3 motore_nuovo/strumenti_py/indice_assets.py` | Aggiorna `assets_originali/INDICE_ASSET.json` e `INDICE_ASSET.md` con percorso, categoria, dimensioni, hash, licenza/stato rilevati e avvisi. |
| Galleria PNG | `python3 motore_nuovo/strumenti_py/galleria.py` | Genera `assets_originali/galleria/galleria.html` per la revisione visiva delle immagini. |
| Conversione PNG in raw | `python3 motore_nuovo/strumenti_py/png_a_raw.py` | Genera `.sglraw` degli sponsor in `motore_nuovo/dati/sprite_raw/sponsor/`; output generato, non modificare a mano. |
| Generatore auto GT | `python3 assets_originali/sprite/auto_gt/genera_auto.py` | Genera le tre varianti GT 64x32; controllare il diff e il catalogo dopo l'esecuzione. Richiede Pillow. |
| Generatore blocco F | `python3 assets_originali/genera_bloccoF.py` | Script grafico esistente; leggere prima le istruzioni e verificare i file che produce, perché non è ancora indicizzato qui per singoli output. |

I primi quattro sono strumenti condivisi del progetto collocati attualmente sotto `motore_nuovo/strumenti_py/`; la loro presenza qui nell'indice non ne sposta la sorgente. `genera_bloccoF.py` e `genera_auto.py` sono già consentiti dal perimetro `assets_originali/**`.

## Coda di aggiornamento strumenti

Prima di aggiungere uno strumento, verificare se esiste già. Per ogni nuovo tool registrare: proprietario, licenza, comando, input/output, dipendenze, rischio di sovrascrittura e risultato della verifica catalogo. Il Designer non sposta tool né aggiorna percorsi di build senza task di migrazione assegnato.
