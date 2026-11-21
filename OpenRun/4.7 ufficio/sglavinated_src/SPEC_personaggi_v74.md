# OutRun Sglavinated v85c — Characters spec (Gianni, Gesù, UFO)

Reference for debugging.
Target: Mega Drive 68000, **PAL**, tested with Genesis Plus GX (libretro).(`OutRun_Sglavinated_v85c.gen` = `a4422f784d95c1f6b044c77911a66bc8337a6999`,
sorgente `tools/patch_z_history/patch_z.v85c_claude.py`).

### ROM

| Address | Content |
|---|---|
| 0x100000–0x39FFFF | PCM music banks |
| 0x3A0000 | ending image (`ENDIMG`), palette 0x3A8C00 |
| 0x3B0000 | hook code |
| 0x3B3F00 | reset stub |
| 0x3B4000 | `BSRC` bubbles: **156** phrases (id 0-155) × 1152 B |
| 0x3E0000 | `GSRC` Gianni frames: 8 poses × 4 pieces × 16 tiles = 16384 B |
| 0x3E4000 / 0x3FF500 | `DS0`/`DS1` alternate (daughter) frames |
| 0x3E4800 / 0x3E4820 | `GPAL` / `DPAL` |
| 0x3E5000 | `TSRC` title text tiles (SGLAVINATED) |
| 0x3FE000 | `APP`: UFO 2 frames × 1152 B; Gesù at `APP+0x900`, 4 frames × 0x300 |

### RAM (hack only)

| Address | Name | Use |
|---|---|---|
| 0xFF4000 | `BAKT` | backup of borrowed VRAM blocks |
| 0xFF4E00 | `BAKP` | backup of palette line 1 |
| 0xFF5300 | `TGR` | tile base per piece |
| 0xFF5320 | `UTR` | tile base for UFO |
| 0xFF5330 | `MINE` | 128 B, 1 per 16-tile VRAM block: owner code, 0 = game |
| 0xFF53B0 | `MAPN` | 3 epochs × 128 |
| 0xFF5530 / 0xFF5590 / 0xFF5650 | `AGE` / `SUM` / `EXP` | block age / checksum / expected checksum |
| 0xFF8D00 | DANGER | 128 B, **conto alla rovescia** per blocco (v85b): `dmark` scrive `DANGER_T`=16, `ddec` lo decrementa a rotazione; ≠0 = blocco escluso. Azzerata al cambio stage |
| 0xFF8E00 | `ROTBUF` | rotated palette (colour effect) |
| 0xFF8F00 | `VAR` | state, see §4 (zona 0xFF8F00–0xFF8FFF, offset 0–255) |
| 0xFF8FC0 | — | offset 192–223: buffer HUD recolour, **riservato** |

## 4. VAR offsets (0xFF8F00 + n, words)

Mappa completa e controllo automatico: `tools/VARMAP.md`, `tools/check_vars.py` (accessi .w/.l solo a offset pari).

| n | Meaning |
|---|---|
| 0 | frame counter |
| 8 | show counter (0 = no character; frames since start) |
| 10 | current pose |
| 18 | phrase index (0–155) |
| 24 | character: 0 = Gianni, 2 = Gesù |
| 28 | countdown before the next show starts (pend) |
| 30 | UFO state: 0 off, >0 active, −1 aborted |
| 46 | pose-load step 0–8 |
| 48 | restore-in-progress |
| 52 | loading phase |
| 58 | 1 = Gianni, 2 = Gesù (catena / rampa escalation) |
| 60 | crash pending |
| 64 | hit mask: the game reclaimed one of our blocks → show ends |
| 68 / 70 | plane_scan position / epoch |
| 84 | show_start retry flag |
| 104 | heavy frame |
| 120 | frames since the last show |
| 122 | mai impostato (solo azzerato) → `ctab` non raggiunta |
| 128 | fine show richiesta: scritto al cambio stage con show attivo (in v85c `bcheck` non lo usa più, vedi §13 BOLLA) |
| 140 / 142 / 144 | accelerometer |
| 150 / 152 | PAL line-counter wrap |
| 158 / 160 / 162 | HUD recolour |
| 164 | off-road frame counter |
| 166 | off-road → Gianni flag (otab) |
| 168 | crash → Gesù flag (xtab) |
| 170 / 172 | last show was Gesù / Gesù retry counter |
| 174 | frames left in the current show |
| 176 | VDP line at hook start |
| 178 | rotating phrase counter |
| 180 | flag inattività (1 = prossimo show usa `itab`) |
| 182 | contatore inattività (frame senza input in gara, scatta a 500) |
| 184 | contatore rampa escalation (alternanza Gianni/Gesù ai livelli 1–2) |
| 186 | errori nello stage: +1 a ogni crash (`pe_n2`) e +1 ogni 2 uscite di strada (v85c), decade col tempo |
| 188 | istante dell'ultimo errore (`0xFFF10C`): aggiornato al crash **e** all'uscita di strada |
| 190 | livello escalation 0–3 (calcolato a ogni `show_start`) |
| 234 | indice del giro di scadenza DANGER (multipli di 4, 0–124) |
| 236 | v85c: evento uscita di strada in attesa (frame rimasti, parte da 250) |
| 238 | v85c: frame di fuga dell'UFO (0–24) |
| 240 | v85c: alterna 0/1, una uscita di strada su 2 conta come errore |
| 248 | v85c: 1 = show corrente nato da uscita di strada o crash (non viene tagliato da SBORDATA) |
| 246 | v85c: fumetto nascosto (il gioco ne ha ripreso un blocco); azzerato a inizio show (`ss_nj`) |

## 5. Characters

### Gianni (cannabis leaf, tempter)

| Item | Spec |
|---|---|
| Art | 64×64 px = 4 pieces 4×4 tiles (column-major), 8 poses @ `GSRC` |
| Art source | `shots/g_frames.bin` — **INPUT, non rigenerarlo**. `tools/gianni12.py` genera solo i fumetti |
| Poses | 0–3 idle, 4–5 inhale, 6–7 exhale |
| Pose sequence | `sq` (32 entries), 1 step every 16 frames |
| Pose update | 8 half-steps of 256 B; identici saltati via `gmask`; loop finché `room ≥ 10` |
| Palette | line 1 (`GPAL`), originale in `BAKP`, `palguard` durante lo show |
| Sprites | SAT 0, 70, 71, 72 (pieces) + 73–76 (bubble) |
| Entry / Idle / Exit | `ez_tab` 24 frame / `bob` + `sw_tab` / `xo_tab` ultimi 24 frame; bolla nascosta con <30 frame |
| Bubble | 3 sprites 4×3 tiles, appare al frame 30 (`bz_tab`) |
| Length | **`SHDUR` = 260 frame fissi** (≈5,2 s PAL), uguale in tutti gli stage (v85 e prima: 220+30·stage) |

### Gesù (corrector)

| Item | Spec |
|---|---|
| Art | `APP+0x900`, 4 frames × 0x300 B: testa 4×4 + corpo 4×2 tile |
| Entry / Hover / Exit | cade dall'alto 64 frame / `bob` / risale ultimi 60 frame; bolla nascosta con <55 |
| Palette | `jpal` (in code) |
| Length | `SHDUR` = 260 |

### UFO

| Item | Spec |
|---|---|
| Routine | `apps` (+ `ualloc`, `ubk`, `udma`, `ucheck`, `ufo_cancel`) |
| Art | `APP`, 2 frames × 1152 B |
| Schedule | `(frame & 0x7FF) < 420` e nessuno show/pending/loading |
| Motion | x = 128 + 340 − t, y wobble `tw` |
| Abort | `ucheck`/VRAM hit → `(30) = −1`, `j_rest` |
| Fuga (v85c) | se uno show sta per partire `(28)` mentre l'UFO è a schermo: per 24 frame sale e accelera a sinistra di 8 px/frame (`(238)`), poi `ufo_cancel` |

## 6. Triggers (cause → effect)

| Event | Detection | Effect |
|---|---|---|
| Crash | 0xFFF039 0→1 or bit 6 | `(60)=1` → `pe_n2`: errori `(186)+1`, istante `(188)`, **Gesù** con `xtab` (o `j1t-j3t` per livello) |
| Ruote fuori strada | `offr`: in gara, 0xFFF05A & 0xF0 ≠ 0 per ≥ 8 frame → evento. v85c: l'evento resta in attesa 250 frame (`(236)`); uno show periodico a schermo già durato 100 frame fa la sua uscita; se c'è una risposta di Gesù in attesa (`(58)`=2) viene sostituita; con evento in attesa il cooldown `(120)≥24` non vale. Ignorata solo con crash pendente `(60)` | **Gianni** con `otab`; cooldown `(120) ≥ 24`; aggiorna l'istante `(188)` ma **non** incrementa gli errori |
| Tempo | `periodic`: `(120)` ≥ `pr_tab[stage]` = 200/180/165/150/140 | show periodico; chi parla dipende dal livello (sotto) |
| Inattività | in gara, byte alto di `0xFFF01A` = 0 per 500 frame, nessun crash/off-road/show/caricamento | Gianni con `itab` (id 35-38); una volta per pausa |
| UFO | frame counter | UFO pass |

**Escalation** (codice `v78_*` in `show_start`/`pend`):
- livello `(190)` = errori `(186)` / 2: 0–1 → 0, 2–3 → 1, 4–5 → 2, ≥6 → 3 (v85c; prima errori−1, con il bug 0 errori → 3).
- decadimento: ogni 600 frame (`0xFFF10C`) senza errori → errori −1 (v85c; prima 1000).
- chi parla negli show periodici: livello 0 → Gianni; 1 → Gesù 1 volta su 2; 2 → Gesù 3 volte su 4; 3 → sempre Gesù.
- frasi: livello 1/2/3 → `g1t/g2t/g3t` (Gianni), `j1t/j2t/j3t` (Gesù).
- tutto azzerato al cambio stage (`zstage`).

Start guards (`show_start`): 0xFFF10C ≥ 250; `(70)≥1` or `(68)≥128`; nessun UFO/caricamento/restore.

## 7. Phrase tables

156 frasi, id 0-155. Limite: `BSRC 0x3B4000` → `GSRC 0x3E0000` = 180224 byte.
Fonte unica del personaggio: `G_IDS` in `tools/gianni12.py` (letta da `lr/phr.py`).

| Range | Who |
|---|---|
| 0–26, 34–65, 85–104, 121–124, 134–140, 142–146, 152–153 | Gianni |
| 27–33, 66–84, 105–120, 125–133, 141, 147–151, 154–155 | Gesù |

| Table | Size | Uso | Id contenuti (v85b) |
|---|---|---|---|
| `ph_tab` | 4×32 | Gianni periodico, un gruppo per stage | 11,13,14,16,19,20,44–65,85–104,121–124,137–140,142–146,152,153 |
| `jtab` | 32 | Gesù periodico | 73,76,80,82,106–109,114,115,120,125–133,141,147–151,154,155 |
| `xtab` | 32 | Gesù dopo crash | 73,76,80,82,106–109,126,130,141,147–151,154,155 |
| `otab` | 32 | Gianni uscita di strada | 19,44,54,64,65,121–124,134–136,142–146,152,153 |
| `ctab` | 32 | **non raggiunta** (VAR122 mai impostato) | 88–91 |
| `itab` | 4 | inattività, slot `VAR178 & 3`, id **esclusivi** (check_vars) | 35–38 |
| `g1t/g2t/g3t` | 6 | escalation Gianni L1/L2/L3 | 0–5 / 6–10,12 / 14,15,17,18,20,21 |
| `j1t/j2t/j3t` | 6 | escalation Gesù L1/L2/L3 | 27–32 / 66–71 / 72,105,110–113 |

Nota: 16 id di `otab` sono anche in `ph_tab` (19,44,54,64,65,121–124,142–146,152,153): la frase da sola non distingue la provenienza.
Testo: `tools/gianni12.py` → `shots/g_bubbles2.bin`. Font `tools/DejaVuSans-Bold.ttf`, riga ≤ 88 px, niente accenti.
`ph_b1`/`ph_b2` restano nel sorgente, irraggiungibili.
