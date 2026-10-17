# Comandi V3 (nomi interni della build v4)
Doppio clic (o `sh v3.sh <comando>`):
- V3 Avvia.command: avvia ciclo + pannello (no duplicati)
- V3 Stato.command: cosa sta succedendo adesso
- V3 Ferma.command: stop a fine giro
- V3 Aggiorna e riavvia.command: ferma il ciclo in sicurezza e lo riavvia con gli script installati
Se macOS non apre i file .command: `chmod u+x *.command`
I wrapper `.command` entrano nella propria cartella e invocano `sh v3.sh`, che risolve la root della build. Il pannello della build v4 usa la porta 8089 per non confondersi con la v3.
Alias facoltativo: alias v3='sh "$HOME/Desktop/Outrun Sglavinated Edition v4 (OpenWay)/comandi/v3.sh"'
