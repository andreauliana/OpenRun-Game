# FATTIBILITA_ROM_MD — possibile ROM Mega Drive originale (.md) dal v3 (blocco N, facoltativo)

SPDX-License-Identifier: MIT

Autore: @architetto. Data: 16 ott 2026. Stato: documento consegnato; BLOCCO N resta [ ] in RICHIESTA.md in attesa che C giri su Pi 400 con toolchain (regola 9). Questo documento e' solo uno studio di fattibilita': non contiene codice, asset, tracciati o musica Sega, e non impegna nessuno a realizzare la ROM.
Percorso: motore_nuovo/docs/FATTIBILITA_ROM_MD.md

---

## 1. Domanda (da RICHIESTA.md, Blocco N)

Si puo' produrre una ROM Mega Drive originale (.md) dal v3, tutta nuova (nessun byte Sega, nessun codice Sega), a PAL 50 Hz, testabile su Genesis Plus GX? Output richiesto: tabella go/no-go + stima in blocchi (se go, piano di una v9x «.md»).

Vincoli duri del progetto (VISIONE.md, PIANO_V3.md): render software 320x224, 50 Hz, audio int16 stereo, input RetroPad, tempo a contatore di frame, RNG con seed, determinismo, zero alloc nel ciclo di gioco, dati in file, niente Sega.

---

## 2. Cosa si riusa dal v3 e cosa va riscritto

| Componente | Riusabile dal C (v3) | Da riscrivere/adattare per Mega Drive |
|---|---|---|
| Strada a segmenti (curve, salite, proiezione) | Logica e matematica (virgola fissa, seme, determinismo) | Resa su VDP (tile/sprite) invece di framebuffer software; 320x224 coincide con modalita' H40 del Mega Drive |
| Auto giocatore, traffico base, collisioni | Logica | Sprite VDP, gestione priorita' e limite sprite per scanline |
| Audio mixer int16 stereo | Struttura (mix, volume, radio da playlist.json) | Driver sonoro MD (Z80 + YM2612 + SN76489 via SGDK o custom); PCM .xm non direttamente suonabile, da riconvertire o sostituire |
| Input RetroPad | Mappatura logica | Lettura joypad Mega Drive (3/6 tasti) |
| Frame counter, RNG xorshift32 seme, replay input | Portabili (stesso comportamento) | Hash frame ogni 60 da calcolare su dati del core (costo VDP da valutare) |
| Show Gianni/Gesu'/UFO, escalation, fumetti | Logica + testi (dati/frasi.json, show_logica.md) | Visualizzazione fumetti su VDP (tile di testo, palette) |
| Livelli JSON (tema, seme, traffico, punti show) | Dati generati da seme | Convertitore JSON -> binari inclusi nella ROM (niente lettura file su cartuccia) |

Riferimento di fattibilita' (non asset del progetto): OutRun su Mega Drive (versione commerciale) girava su questo hardware a 50 Hz con pseudo-3D -> un motore pseudo-3D a 50 fps su 68000 e' in principio possibile. Il budget frame del nostro motore su 68000 va stimato e misurato.

---

## 3. Toolchain (licenze)

**SGDK** — https://github.com/Stephane-D/SGDK
- «SGDK - A free and open development kit for the Sega Mega Drive» (letto dal readme ufficiale, 16 ott 2026).
- Libreria e tool custom distribuiti sotto **licenza MIT** (vedi license.txt nel repo).
- Compilatore GCC e libgcc sotto **GNU GPL3**; software compilato con esso ricade sotto l'**eccezione runtime GCC** (COPYING.RUNTIME) — compatibile con distribuzione.
- Target: m68k-elf-gcc. Binari GCC 13.2 forniti solo per Windows; su Mac/Linux va installato/compilato a parte (readme cita: gendev-macos, sgdk-helper, marsdev, docker-sgdk). I tool custom richiedono Java.
- Stato: **[C]** da verificare (installazione su Mac e, se possibile, su Pi 400/Raspberry Pi OS). Nessun compilatore C++ in questo ambiente (regola 21), build reale non eseguibile qui.

Generatore di ROM: SGDK produce immagini `.md`/`.bin` pronte per emulatore e flashcart. Genesis Plus GX (emulatore di test, RetroArch) è lo strumento indicato da RICHIESTA.md Blocco N.

---

## 4. Limiti hardware Mega Drive a 50 Hz PAL (da verificare su documentazione hardware)

Valori di riferimento generali (non misurati su questo progetto, marca [C]): CPU Motorola 68000 ~7,6 MHz; RAM 64 KB; VRAM 64 KB; sprite max 64 totali, 20 per scanline, dimensioni fino a 32x32; palette 64 colori (4 x 16); tile 8x8; risoluzione 320x224 (H40) = coincide col render v3. Fonti da consultare: documentazione VDP linkata nel readme SGDK (rasterscroll.com/mdgraphics/, copetti.org Mega Drive architecture, segaretro Genesis Software Manual).

---

## 5. Niente materiale Sega

- Zero byte della ROM originale Sega, zero codice Sega, zero asset/tracciati/musica Sega.
- SGDK e' una libreria di sviluppo (MIT), non contiene ROM Sega; usarla non inietta codice Sega nella ROM prodotta.
- Tutti i dati v3 (livelli, frasi, show, scenari, sprite originali) sono già originali o con licenza dichiarata (vedi LICENZE.md, CREDITI.md).

---

## 6. Tabella go/no-go e stima blocchi (v9x)

| Aspetto | Esito provvisorio | Nota |
|---|---|---|
| Toolchain disponibile e compilabile | [C] da verificare | SGDK MIT, ma m68k-elf-gcc su Mac/Pi da installare; Java richiesto |
| 50 fps pseudo-3D su 68000 | [C] da stimare/misurare | OutRun MD lo dimostra in generale; il nostro budget frame no |
| VRAM/sprite/colori entro limiti | [C] da verificare | 320x224 OK; limite sprite per scanline da budgetare |
| Audio su MD | [C] da verificare | Driver sonoro + riconversione musica da playlist |
| Determinismo/replay | OK per logica | Frame counter e RNG portabili; hash frame costo da valutare |
| Licenza distribuibile | OK (MIT + eccezione GCC) | Nessun vincolo Sega |

**Go/No-Go:** go solo se toolchain installa e stima frame <= 20 ms (obiettivo <= 12 ms su 68000); altrimenti no-go e si resta su Pi 400/PC.

**Stima blocchi (se go, numerazione v90+):**
- N1: toolchain m68k-elf + SGDK, ROM vuota che gira su Genesis Plus GX (Hello World + VBlank).
- N2: resa strada a segmenti su VDP da framebuffer del core C (tilemap + sprite).
- N3: convertitore dati livelli/frasi/show JSON -> binari inclusi nella ROM.
- N4: driver sonoro MD + musica convertita; silenzio se driver non pronto (come runner v3).
- N5: gameplay completo (auto, traffico, collisioni, show, input joypad MD).
- N6: test determinismo (hash ogni 60 frame), fps e temperatura su emulatore e Pi 400; eventuale ottimizzazione.

Totale stimato: circa 6 blocchi (v90-v95), solo dopo che C gira su Pi 400 (regola 9).

---

## 7. Fatti verificati vs ipotesi

**Fatti verificati:**
- SGDK: licenza MIT per libreria e tool custom; GCC/libgcc GPL3 + eccezione runtime (letto dal readme ufficiale GitHub Stephane-D/SGDK, 16 ott 2026).
- Pista N: resta bersaglio finale FACOLTATIVO, solo dopo che il motore C gira (PIANO_V3.md Blocco 12, regola 9).
- v3 è già pensato per aiutare il port: virgola fissa, 320x224, 50 Hz, determinismo, dati separati dal codice (PIANO_V3.md riga 149).
- Questo ambiente: nessun compilatore C++ e shell ristretta (regola 21); build e misura fps non eseguibili qui.

**Ipotesi (da verificare, non usate per decisioni):**
- m68k-elf-gcc installabile e funzionante su Mac e su Pi 400/Raspberry Pi OS.
- Frame del motore pseudo-3D <= 20 ms su 68000 a 7,6 MHz.
- Numeri esatti di limite sprite/VRAM del Mega Drive per il nostro caso d'uso.
- Costo del calcolo hash frame ogni 60 frame sul VDP.

---

## 8. Raccomandazione

Consegnare questo documento come base per la decisione di Andrea. Non attivare N finche': (a) C non gira su Pi 400 con toolchain (blocco H), e (b) toolchain SGDK verificata installabile. Nel frattempo il lavoro prioritario resta il motore C su Pi 400 (blocco C/H).