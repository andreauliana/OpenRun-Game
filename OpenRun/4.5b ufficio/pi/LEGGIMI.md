# Edizione "Really Open" sul Raspberry Pi (RetroPie)

| Passo | Comando |
|---|---|
| 1. Copia la cartella del progetto sul Pi (dal Mac) | `scp -r "$HOME/Desktop/Outrun Sglavinated Edition v4 (OpenWay)" pi@retropie.local:~/` |
| 2. Sul Pi, apri il terminale (F4 dal menu, oppure `ssh pi@retropie.local`) | |
| 3. Compila e installa | `sh "$HOME/Outrun Sglavinated Edition v4 (OpenWay)/pi/compila_e_installa.sh"` |
| 4. Gioca | riavvia EmulationStation, menu **Ports** → "Sglavinated Really Open" |

- Dopo un aggiornamento: ripeti i passi 1 e 3.
- Uscita dal gioco: ESC sulla tastiera.
- Utente e indirizzo predefiniti di RetroPie: `pi@retropie.local` (cambiali se li hai modificati).
