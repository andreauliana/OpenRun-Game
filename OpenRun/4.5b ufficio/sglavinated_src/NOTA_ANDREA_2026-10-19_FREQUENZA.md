# Nota di Andrea per sviluppatori — frequenza di gioco (19 ott 2026)

Decisione di Andrea (registrata da Claude). Vale come voce di ANDREA_REVIEW: precedenza sulle altre fonti.
Destinatari: @builder-developer (autore), @developer-builder-junior (revisore). @capo apre un checkpoint dedicato.
Ordine: dopo R1, prima di R2.

## Decisione
1. **Tutte le misure del gioco in secondi, non in frame.** Tempo limite, durata show (oggi 260 frame), soglia di inattivita' (500 frame), timer di checkpoint, animazioni, accelerazioni e attriti devono usare `dt` in secondi.
2. **Frequenza logica predefinita: 60 Hz.** Oggi e' 50 Hz (`src/game/game.cpp:678` `dt = 1/50`, `game.cpp:831` `/50`, `UNITA_PER_KM` in `game.cpp:17`, `src/libretro/sglavinated_libretro.cpp:37` `fps = 50` e `:78` `RETRO_REGION_PAL`).
3. **Opzione 50 Hz** in configurazione (per TV a tubo PAL o RetroPie impostato in PAL), es. `dati/config.json` `"frequenza_hz": 60` oppure opzione del runner `--hz 50`. Il core libretro dichiara la frequenza scelta (60 -> NTSC, 50 -> PAL).

## Perche'
Mac, Pi 400 (solo HDMI), monitor e TV moderni, anche in Europa, lavorano a 60 Hz; anche Windows XP gira su monitor da 60 Hz in su. A 50 Hz logici su uno schermo a 60 il runner ripete un'immagine ogni 6: la strada che scorre fa un piccolo scatto continuo. Una possibile versione per Windows XP (dopo Mac e Pi) non cambia questa scelta.

## Gate
- `ctest` verde a 60 Hz e a 50 Hz; il giro guidato dura gli stessi secondi (+/- 1%) con le due frequenze.
- Replay 2x IDENTICO a ciascuna frequenza (i replay registrati a una frequenza si riproducono a quella).
- Il runner su Mac con vsync a 60 Hz: nessun frame ripetuto in 60 s di guida (contatore nel log del runner).
