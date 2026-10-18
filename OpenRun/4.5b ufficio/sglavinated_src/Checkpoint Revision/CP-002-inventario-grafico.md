# CP-002 — Inventario grafico e handoff al Capo

| Campo | Valore |
|---|---|
| Stato | APERTO — in attesa di review indipendente |
| Aperto / ultima attività | 2026-10-18 / 2026-10-18 |
| Ambito | Verificare contenuti grafici, tool del Designer, lacune richieste e handoff univoco |
| Responsabile | Audit documentale per il Dipartimento Grafico (Copilot) |
| Revisore indipendente | Da assegnare dal Capo; deve essere diverso dall'autore |
| Percorsi aggiornati | `UFFICIO/Dipartimento Grafico/Libreria/`, `UFFICIO/Dipartimento Grafico/TOOLS/`, `out/designer/`, definizioni agenti e checkpoint |
| Handoff Designer | [STATUS_GRAFICO_PER_CAPO.md](../out/designer/STATUS_GRAFICO_PER_CAPO.md) |
| Inventario di dettaglio | [CATALOGO_RISORSE.md](../UFFICIO/Dipartimento%20Grafico/Libreria/CATALOGO_RISORSE.md) |
| Decisioni richieste ad Andrea | Nessuna approvazione richiesta per questo audit; completamento delle lacune resta da pianificare |

## Sintesi

- L'indice della Libreria 4.6 e la guida strumenti ora descrivono le fonti v4.5 attive senza duplicarle. Gli asset non sono ancora stati spostati nella nuova Libreria o Shared asset.
- Catalogo: 52 elementi (30 approvati, 16 pronti, 2 in costruzione, 4 mancanti). Il controllo segnala 100 PNG non catalogati come varianti/doppioni da valutare.
- Esistono tre livelli configurati (Neon, Atomico, Foglie), non sei percorsi autonomi; le tre auto 64x32 e i 21 banner sponsor sono approvati; i 16 elementi bordo sono pronti.
- Le frasi di Gianni e Gesù sono complete: 97 + 59 = 156, secondo la specifica [SPEC_personaggi_v74.md](../SPEC_personaggi_v74.md). La grafica dei personaggi/balloon non è per questo approvata.
- Sei percorsi selezionabili, tre finali con animazioni e sprite avversari non risultano disponibili come asset attivi approvati.
- Sono stati documentati i tool già esistenti e concessi al Designer i comandi galleria e conversione PNG->raw. Nessun asset generato o spostato in questo checkpoint.

## Evidenze

| Comando/verifica | Esito | Evidenza |
|---|---|---|
| `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` dalla radice `sglavinated_src/` | Exit 0; 52 elementi, stati coerenti; avviso per 100 PNG non catalogati | Output verificato durante il task |
| Controllo riferimenti di file e sprite | Tre auto 64x32 esistenti; i record `auto.traffico`, `personaggio.gianni`, `personaggio.gesu`, `personaggio.ufo` sono `mancante` | Catalogo e inventario collegato |
| Conteggio testi | 97 frasi Gianni e 59 Gesù; totale 156 | JSON personaggi e specifica Andrea |
| Inventario di destinazione | `Libreria/` contiene README, indice; `Shared asset/` contiene solo README; `TOOLS/` contiene indice operativo, non script duplicati | File presenti nell'area 4.6 |

## Review indipendente

- Revisore:
- File/diff esaminati:
- Esito: IN ATTESA
- Verificare conteggi, link relativi, tool e che le lacune siano riportate senza dichiarare mancanti i testi dei fumetti.
- Nuova review dopo correzione:

## Eventi

- 2026-10-18 — aperto dopo audit inventario/tool; assegnazione del revisore indipendente demandata al Capo.
