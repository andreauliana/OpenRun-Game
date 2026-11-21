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
