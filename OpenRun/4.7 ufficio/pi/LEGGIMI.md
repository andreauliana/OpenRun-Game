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

## Seconda strada: core libretro dentro RetroArch

Build separata (cartella `build_libretro`, niente SDL). Stessi passi 1, 2 e 4; al passo 3 usa:

`sh "$HOME/Outrun Sglavinated Edition v4 (OpenWay)/pi/compila_core_retroarch.sh"`

- Nel menu **Ports** compare "Sglavinated RetroArch".
- Pad: croce o stick per sterzare, tasto in basso (B) o grilletto destro per accelerare, grilletto sinistro per frenare, Start + tasto destro (A) per il brano successivo.
- Uscita: Hotkey + Start (di solito Select + Start).
- Prova a mano: `/opt/retropie/emulators/retroarch/bin/retroarch -L sglavinated_src/motore_nuovo/build_libretro/sglavinated_libretro.so sglavinated_src/motore_nuovo/dati/percorso_gara.json`
- Non ci sono salvataggi di stato; i cartelloni sponsor PNG ci sono solo nella versione Ports normale.

## Build di misura (prestazioni sul Pi 400)

Serve a sapere se il gioco regge i 60 fps sul Pi vero. Non installa nulla in **Ports** e non cambia il gioco.

| Passo | Comando |
|---|---|
| 1. Pacchetti (una volta) | `sudo apt update && sudo apt install -y build-essential cmake pkg-config libsdl2-dev libsdl2-image-dev` |
| 2. Dal **terminale del desktop** del Pi (non da SSH) | `sh "<cartella del repo>/OpenRun/4.7 ufficio/pi/misura_pi400.sh"` |
| Prova veloce (20 s per pista invece di 60) | `sh ".../pi/misura_pi400.sh" 20` |

- Dura circa 10 minuti con 60 s per pista: 7 prove senza finestra (6 piste + gara a tappe) e 2 gare a schermo intero (CRT spento e acceso). Il gioco guida da solo: non toccare niente. Esc interrompe la gara a schermo.
- Risultato: `misure_pi400/<data_ora>/riepilogo.txt` (temperatura, frequenza, limitazioni di alimentazione, ms per frame). Mandalo a Claude oppure fai commit della cartella con GitHub Desktop (i file grandi `.csv`/`.log` restano fuori da git).
- Da SSH lo script fa solo le prove senza finestra. Da console senza desktop: `SDL_VIDEODRIVER=kmsdrm sh .../pi/misura_pi400.sh`.
- A mano: `./motore_nuovo/build_misura/misura_pi out/misura 60` (dalla cartella `sglavinated_src`) e `./motore_nuovo/build_misura/sglavinated_runner --subito --pilota-auto --misura-secondi 60`.
- Come leggerlo: nel nucleo media e p99 sotto ~10 ms vanno bene; nel runner "tick logici al s" vicino a 60 e "giri lenti" vicino allo 0% vogliono dire niente scatti. `get_throttled` diverso da `0x0` = alimentatore debole o Pi troppo caldo, e la misura va ripetuta.
