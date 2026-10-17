# OutRun Sglavinated v85c — Characters spec (Gianni, Gesù, UFO)

Reference for debugging (e.g. OpenCode). Target: Mega Drive 68000, **PAL**, tested with Genesis Plus GX (libretro).
Valori verificati sul sorgente generato **`tools/h40z.s`** della build v85c
(`OutRun_Sglavinated_v85c.gen` = `a4422f784d95c1f6b044c77911a66bc8337a6999`,
sorgente `tools/patch_z_history/patch_z.v85c_claude.py`). Differenze rispetto alla v85b: §13.
Le note marcate "v74"/"v77" sono misure vecchie, non valori di codice.

## 1. Where the code is

The hook is **generated** Assembly. It is not a single hand-written file.

| Step | File | Role |
|---|---|---|
| 1 | `tools/h40x.s` | base hook source (old) |
| 2 | `tools/patch_y.py` → `tools/h40y.s` | first patch layer |
| 3 | `tools/patch_z.py` → `tools/h40z.s` | **all recent changes** (`rep(old,new)`, applied in order; scrittura atomica via `.tmp`) |
| 4 | `cp tools/h40z.s tools/gianni_hook.s` | file the build reads |
| 5 | `build7.py` | links hook at 0x3B0000, appends data, writes ROM |

- Pipeline completa: `sh tools/build_all.sh out.gen 81 6` (compila tutti i .py, patch_y, patch_z, `check_vars.py`, build7, SHA).
- Codice fuori da `h40z.s`: `rst/rst.s` (stub di reset), `smoke/`, `hud/`, `gstatic/gmask.s`, `shots/*.s`.
- Toolchain: `m68k-linux-gnu-as -m68000` / `m68k-linux-gnu-ld -Ttext=0x3B0000`.
- Base ROM (not included): `outrun_src/OutRun (USA, Europe).md`.
- Syntax: GNU m68k (`%a0@(8)` = `8(a0)`), commenti con `|` (non `#`). `a0` = `VAR` (0xFF8F00) almost everywhere.

## 2. Entry points

| Item | Value |
|---|---|
| VBlank vector (0x78) | → `_start` @ 0x3B0000 |
| `_start` | calls original handler `jmp 0x17B8`, then `hk_after` |
| Skip condition | RAM byte 0xFFF00C == 0 → hook does nothing |
| Reset vector (0x4) | → stub @ 0x3B3F00 (`rst/rst.s`): clears 0xFF5300–0xFF5BFF and 0xFF8F00–0xFF8FFF, then `jmp 0x200` |

Coda di `hk_after` (in ordine): … `scan` → `palguard` → `chkroom` → `pend` → controllo inattività (solo con `0xFFF000`=8) → `id_out`: `ddec` (scadenza DANGER) → `rte`.

## 3. Memory map

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

## 8. VRAM "library"

Characters **borrow** 16-tile blocks:

1. `alloc`/`salloc`: `ord_tab`, poi ricerca 28–87 (salta 62–63, 74–77). Mai 0–27, mai ≥ 88, mai blocchi con DANGER ≠ 0.
2. Contenuto salvato in `BAKT`, poi DMA della grafica.
3. Controlli ogni frame: `sat_scan`, `plane_scan`, `gcheck`/`bcheck`, `hidemine`.
4. Su ripresa: `(64)` → `show_end` → `rststep` ripristina da `BAKT`; il blocco riceve DANGER = `DANGER_T` (16).
5. **Scadenza (v85b)**: `ddec` gira a fine hook, 4 blocchi per frame (giro di 128 blocchi = 32 frame), decrementa i contatori ≠0. Un blocco torna utilizzabile dopo 16 giri ≈ 512 frame ≈ 8,5 s. Indice in `VAR234`, azzerato in `zstage`.

Motivo (misura OpenCode, 64k frame): senza scadenza nello stage 1 i DANGER arrivavano a 52 sul pool reale di ~54 blocchi, `salloc` falliva 135 volte, 3,2 show/min. Con scadenza 16: 7,3 show/min, 7 fallimenti.

## 9. Timing / CPU budget

| Rule | Detail |
|---|---|
| `room` | min(linee a fine vblank, `PBUDGET`(58) − linee usate) |
| PAL line counter | 00–FF, 00–02, CA–FF |
| Gates | `SHR`=18, `ALR`=26, `MGA`/`MGB`/`MGF`=17/30/28, `CKR`=17, `SSR`=8, `WBR`=14; `hidemine` ≥ 20 |
| Codice nuovo per frame | solo in fondo a `hk_after` (dopo `pend`), mai prima di `scan`/`hidemine` |

## 10. Debug checklist

| Watch | Meaning |
|---|---|
| 0xFF8F08 | ≠ 0 → show on screen |
| 0xFF8F18 | 0 = Gianni, 2 = Gesù |
| 0xFF8F12 | phrase index |
| 0xFF8F1E | UFO state |
| 0xFF8F40 | hit mask |
| 0xFF8F1C | countdown to the next show |
| 0xFF8F3C | crash pending |
| 0xFF8FBA / 0xFF8FBE | errori escalation / livello |
| 0xFFF039 / 0xFFF05A / 0xFFF03E / 0xFFF01A | crash / off-road / speed / pad |
| 0xFF5330 + block | MINE |
| 0xFF8D00 + block | DANGER (contatore) |

Test harness in `lr/` (Genesis Plus GX, PAL): `final.py`, `leakstats.py` (5 seed, default 16000 frame, metodo stampato), `showpool.py`, `showwhy.py`, `desync.py`, `pad_test.py`, `idle_test.py`, `esc_natural.py`, `itab_count.py`, `otab.py`, `jerk.py`, `lag2.py`, `verify.py`.

## 11. Known limits / bug aperti

- Bug escalation della v85b (0 errori = livello 3): **corretto nella v85c** (livello = errori/2).
- Characters cannot have their own permanent VRAM: alcuni show finiscono presto quando il gioco riprende un blocco (`VAR64`) o `bcheck` trova la grafica sovrascritta (`VAR128`).
- La scadenza DANGER aumenta un po' il leak (misura OpenCode, 16000 frame, 5 seed: left 24,6→31,8, right 19,3→31,5, wild 26,1→30,2; entro media v85 + 2 sd). Ripieghi pronti: DANGER_T 24 e 32.
- No characters in the attract demo. Tested only on Genesis Plus GX.

## 12. Stato v85b (misure)

| Test | Risultato |
|---|---|
| Quick test Claude (final.py, 16000 frame, seed 0) | left: 33 show, wrong 0, repeat 0, vsbad 0, short 1 · wild: 35 show, wrong 0, repeat 0, vsbad 0, short 3 |
| Gate 5 seed × 3 modalità sulla v85b | **fatto sulla v85c** (vedi §13) |

Ripristino: `OutRun_Sglavinated_v85.gen` (`c7276aa4`) / codice `tools/patch_z_history/patch_z.v85_pulito.py`. La v79 è ritirata.

## 13. v85c (Claude, 2026-10-14)

| Nome | Cosa | Dove |
|---|---|---|
| ESC0/ESC2 | livello = errori/2, decadimento 600 frame (corregge 0 errori = livello 3) | `v78_ck`, `v78_dl` |
| SBORDATA | evento uscita di strada in attesa 250 frame; sostituisce la risposta di Gesù in attesa; uno show **periodico** (VAR248=0) già durato 100 frame (fumetto letto) fa la sua uscita (Gianni 24, Gesù 60 frame); gli show reattivi non vengono mai tagliati | `offr`, VAR236, VAR248 |
| SBORDATA2 | una uscita di strada su 2 conta come errore per l'escalation | `offr`, VAR240 |
| UFO_FUGA | l'UFO scappa in 24 frame invece di sparire quando parte uno show | `apps`, `ap_dr`, VAR238 |
| FINALE | stato 44 (immagine finale): dal frame 24 la tabella sprite viene svuotata a ogni frame | `endshow` |
| BOLLA | se `bcheck` trova il fumetto sovrascritto: fumetto nascosto (VAR246), i 3 blocchi del fumetto restituiti (`brel`: restore da BAKT se ancora nostri, altrimenti DANGER; bit in VAR67, MINE azzerato), il personaggio fa subito la sua uscita (Gianni 24 frame, Gesù 48). Prima finiva tutto lo show: era la causa di **tutte** le sparizioni di Gesù | `bcheck`, `brel`, `sb` |

Misure (Claude, harness `lr/`). Il run di `final.py` è deterministico: per avere più run si usa
`lr/fseed.py rom N mode seed` (seed = frame di attesa prima della partenza, sposta la fase).

5 seed × 16000 frame, leak per 1000 frame di show:

| Modalità | v85b leak | v85c leak | v85b show corti (<60 frame) | v85c show corti | wrong / vsbad |
|---|---|---|---|---|---|
| left | 27,4 ± 7,2 | 27,9 ± 16,5 | 14 | **3** | 0 / 0 |
| right | 11,9 ± 6,7 | 19,0 ± 4,5 | 20 | **2** | 0 / 0 |

Un run (seed 0, 16000 frame):

| Test | v85b | v85c |
|---|---|---|
| show finiti di colpo (`showend.py`) left / wild | 9 / 8 | 1 / 2 (cambio stage; 1 blocco ripreso) |
| uscite di strada con reazione (`offreact.py`) left / wild s137 | 34% / 48% | 64% / 66% |
| UFO spariti di colpo (`ufochk.py`) | 4 su 5 | 1 su 3 (abort `ucheck`) |
| `desync.py` / `pad_test.py` | — | 0 disallineamenti / TUTTI OK |
| `pause.py` / `demo.py` | — | vsbad 0 / vsbad 0 |
| `lag2.py` cadenza strada | 23,0% | 23,0% (identica) |
| `jerk.py` pose miste | — | 4 frame su 749 |
| titolo + demo 8000 frame | — | vsbad 0, 0 personaggi, 0 UFO |
| rebuild da zero / `check_vars.py` | — | SHA identico / exit 0 |

Il leak di right sale di ~7 per 1000 (entro la soglia OpenCode media v85 + 2 sd ≈ 39,5).

Provato e scartato: tenere il personaggio a schermo per tutta la durata dopo la perdita del
fumetto. Il leak in wild saliva 93 → 475 (`leakwho.py`: la zona contesa dal gioco sporca
anche i blocchi del corpo). Per questo l'uscita è immediata.

Nota: con il bot (molti crash) il livello arriva spesso a 3, quindi la reazione all'uscita di strada
è quasi sempre Gesù nei test. Con guida normale prevale Gianni ai livelli 0–1.
Non verificato: il finale reale (stato 44) non è stato raggiunto giocando; la correzione è
stata provata forzando lo stato 44 (sprite visibili: v85b 20–53, v85c 0).

### Gate a 5 seed × 3 modalità (OpenCode, 2026-10-15, su `a4422f78`)

`lr/fseed.py out.gen 16000 <mode> <seed 0..4>`, leak per 1000 frame di show:

| Modalità | leak/1000 per seed | media ± sd | max | soglia (media v77 + 2 sd) | wrong | vsbad | repeat | short |
|---|---|---|---|---|---|---|---|---|
| left | 26,2 · 11,2 · 44,2 · 42,6 · 30,1 | **30,9 ± 13,5** | 44,2 | 48 | 0 | 0 | 0 | 2 |
| right | 21,6 · 24,0 · 13,4 · 25,2 · 8,9 | **18,6 ± 7,1** | 25,2 | 31 | 0 | 0 | 0 | 7 |
| wild | 11,2 · 37,9 · 23,9 · 20,0 · 18,8 | **22,4 ± 9,8** | 37,9 | 36 | 0 | 0 | 1 | 8 |

**Tutte e tre le medie entro soglia.** `wrong` 0 e `vsbad` 0 su tutti i 15 run.
L'unico `repeat` è wild seed 0 (1 su 15); il max singolo in wild (37,9 a seed 1) supera
di poco la soglia 36, ma la soglia è sulla media dei 5 seed (22,4), come per le altre modalita'.

Controlli eseguiti sulla stessa build:
- **rebuild da zero** (`build_all.sh` con `patch_z.v85c_claude.py`) → `a4422f78…`, **byte-identica** alla ROM consegnata.
- `check_test_build.sh --consegna` → OK, 0 marcatori diagnostici.
- `check_vars.py` → exit 0 (nella rebuild: 175 byte VAR usati, 0..249).
- `desync.py` 8000 frame → 0 disallineamenti su left/right/wild.
- `pause.py` → vs-bad 0 · `demo.py` → vs-bad 0, stati 0/32/28/24 intatti.
- `final.py` 8000 frame (titolo+demo) → vsbad 0, 0 personaggi fuori show.

## 14. v86 task 1 (OpenCode, 2026-10-15) — UFO_FUGA_BREVE

| Nome | Cosa | Dove |
|---|---|---|
| UFO_FUGA_BREVE | quando il gioco si riprende un blocco dell'UFO ma la grafica è ancora integra, l'UFO scappa in 12 frame e poi fa l'abort differito, invece di sparire di colpo | `ap_ab`, `ap_fle`, `ap_ab2`, VAR242, VAR244 |

`ap_ab` era raggiungibile in due modi e i due casi vanno trattati diversamente:
1. da `ucheck` (VRAM **già** sovrascritta: la grafica NON è integra) → abort identico a v85c, nessuna lettura in più;
2. da `VAR74` (sat_scan: il gioco rivendica il blocco, ma i nostri tile sono ancora lì → integra) → 12 frame di fuga.

Nel caso 2 si rilegge `ucheck` **in sola lettura**: `VAR67` è un byte all'offset dispari 67, quindi si
salva/ripristina la **word 66** (byte 66+67, entrambi pari, `move.w` resta allineato) e l'abort finale
(`ap_ab2`) resta byte a byte quello di v85c. Il dispatch è `tst.w VAR74`: se `VAR74==0` si è arrivati
da `ucheck` e non serve alcuna lettura.

### NON toccare `ap_x` (regola misurata)
`ap_x` è la destinazione di `cmpi.b #8,%d0; bne.w ap_x`: viene eseguito **ogni frame anche in attract
mode**. La prima versione della patch aggiungeva il check del flag di chiusura proprio lì e la RAM
divergeva dalla v85c **dal frame 129** (misurata con `lr/divrun.py`, che confronta l'hash della RAM
frame per frame fra due run con gli stessi input). Spostando il check su `ap_fle` (subito dopo
`bra.w ap_dr`, che in attract non viene mai raggiunto) la divergenza del core `0000-FBFF` è partita
**dal frame 8402**, cioè dal primo istante in cui la nuova logica gira davvero in gara: i seed
137/313/911 danno numeri **identici** fra v85c e v86.

### Gate (5 seed × 16000 frame, leak per 1000 frame di show)

| Modalità | v85c leak | v86 leak | soglia | short (somma 5 seed) | wrong / vsbad |
|---|---|---|---|---|---|
| left | 27,9 ± 16,5 | **33,2 ± 17,3** (max 51,1) | 48 | 3 → **2** | 0 / 0 |
| right | 19,0 ± 4,5 | **19,0 ± 4,1** | 31 | 2 → 2 | 0 / 0 |
| wild | 16,4 ± 6,1 | **18,0 ± 6,1** | 36 | 4 → **2** | 0 / 0 |

`repeat` 1 su wild seed 0: **presente anche nella v85c** (stesso seed), non è una regressione.
`desync.py` 0 disallineamenti su left e wild (seed 0).

Un run (seed 0 / seed 137, 16000 frame):

| Test | v85c | v86 task 1 |
|---|---|---|
| `ufochk.py` left / right / wild | 3 passaggi, **1 sparizione** (`t400`) | 3 passaggi, **0 sparizioni**, `fuga_breve 1` |
| `showend.py` left | 35 normali + 1 cambio stage | **identico** (stessi frame) |
| `showend.py` wild | 35 + 1 cambio stage + 1 blocco ripreso | 32 + 1 cambio stage, **nessun blocco ripreso** |
| `offreact.py` left / wild s137 | 23 reazioni su 39 / 19 su 29 | **identico** |
| `jerk.py` pose miste | 4 frame su 840 | **4 su 840 (identico)** |
| `lag2.py` cadenza strada | 975 redraw, 22,8%, istogramma X | **identico** |
| `pause.py` / `demo.py` | vs-bad 0 / vs-bad 0 | vs-bad 0 / vs-bad 0 |
| `check_vars.py` | 175 byte VAR | **179 byte VAR**, exit 0 |

Costo: `leakwho.py` totale 220 → 232 (+12 frame su 16000, +5%), tutto dentro il rumore fra seed
(il leak di una stessa modalità varia fra 15,5 e 44,7 a seconda del seed). È il prezzo atteso di
12 frame in più in cui l'UFO tiene i blocchi, ed è il motivo per cui la durata è 12 e non di più.
