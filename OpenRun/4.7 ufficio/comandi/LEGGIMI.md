# Comandi personali v4.5 (il nome interno `V3` resta per compatibilita')
Doppio clic sui launcher `.command`, oppure da Terminale dalla cartella `comandi/`:
- V3 Avvia.command: avvia ciclo + pannello (no duplicati)
- V3 Stato.command: cosa sta succedendo adesso
- V3 Ferma.command: stop a fine giro
- V3 Aggiorna e riavvia.command: ferma il ciclo in sicurezza e lo riavvia con gli script installati

Comandi da Terminale:

```sh
sh v3.sh start        # avvia il ciclo degli agenti
sh v3.sh stato        # stato del ciclo e pannello
sh v3.sh stop         # ferma a fine turno
sh v3.sh stop-subito  # richiede lo stop e termina il ciclo corrente
sh v3.sh aggiorna     # ferma e riavvia con la configurazione corrente
sh v3.sh vivo         # apre il cruscotto nel Terminale
sh v3.sh pannello     # apre il pannello web
```

Da un'altra cartella, usare il percorso completo, ad esempio:
`sh "/percorso/alla/v4.5/comandi/v3.sh" stato`.
I wrapper `.command` determinano la propria cartella, inoltrano l'exit code
del comando e mantengono il messaggio finale quando sono aperti da Terminale.
Gli script risolvono `sglavinated_src/` e `strumenti/` a partire dalla radice
v4.5; il pannello usa la porta 8090.
