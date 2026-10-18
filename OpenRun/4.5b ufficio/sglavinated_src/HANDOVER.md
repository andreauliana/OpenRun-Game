# OutRun Sglavinated Edition — handover

Read order for an AI agent: **this file → BUILD.md → OPENCODE_REFERENCE_v74.json → SPEC_personaggi_v74.md → tools/patch_z.py**.
`tools/h40z_v74.s` is the v74 reference snapshot (read it, never edit it).

Build di lavoro corrente: **v77**, `OutRun_Sglavinated_v77.gen` =
`197866c5ff77d9d7784706472518bea8145f1dbc`. **Solo dati**: le metriche sono identiche alla
v76 (left 36 show/leak 123, right 36/157, wild 41/129; wrong 0, repeat 0, vsbad 0).

## 0. Stato v77 — fatto / da fare

### Fatto e verificato
| Intervento | File | Esito misurato |
|---|---|---|
| Frequenza personaggi | `patch_z.py` | `pr_tab` 500/440/400/360/320 → 200/180/165/150/140; attesa inizio gara 400→250; attesa inizio stage 256→128; cooldown uscita di strada 60→24; countdown fisso 90→8. Show su 16000 frame: **99 → 118**. Sotto `pr_tab`=200 non si guadagna altro e `leak` raddoppia |
| `ph_tab` usata davvero | `patch_z.py` | era codice morto: ogni show periodico passava da `pe_nb` (VAR58≠0) e finiva in `ph_b1`/`ph_b2`. Ora Gianni = `ph_tab[stage*32+(VAR178&31)]`, Gesù = `jtab[VAR178&31]`, alternanza VAR58 invariata |
| Tabelle reattive | `patch_z.py` | `otab` 4→32, `xtab` 8→32, `ctab` 16→32, tutte con maschera `#31`. Prima: 8 uscite di id 121 e 6 di id 31 in 16000 frame |
| Frasi | `gianni12.py` | 137 → **156** (limite hardware, vedi BUILD.md). Nuove id 137-155 |
| Font deterministico | `tools/DejaVuSans-Bold.ttf` | `gianni12.py` lo usa per primo |
| Dati protetti | `shots/MANIFEST.sha256`, `shots/CHECK_DATA.sh` | rileva modifiche accidentali a `g_frames.bin`, palette, tile, PCM |
| Contenuti v77 (solo dati) | `tools/gianni12.py`, `tools/patch_z.py` | `otab` = **solo frasi off-road** (16 id riscritti); `ctab` = **solo id Gianni** (chiude il bug P2); **36 frasi di escalation** in `g1t-g3t`/`j1t-j3t` (6 per livello, verificate ≤88 px, niente accenti). Dati pronti, **codice non ancora collegato** |
| `otab`/`xtab` usate davvero | `lr/otab.py` (riconosce la frase leggendo le tabelle da `tools/h40z.s`) | 16k frame: **13 show otab** in wild, 10 in left; consecutivi max **2**, distanza minima 121/115 frame. Il cooldown 24 **non** rende Gianni insistente a bordo strada: si può tenere |

### Regola emersa dai test (importante)
- **Il codice nuovo va in fondo a `hk_after`, dopo `palguard`/`pend`**: prima di `scan`/
  `hidemine` consuma budget e sposta il `leak` di ±50-100 frame. Misurato più volte.
- **Ricomponendo una tabella, riempi ciclando i valori rimasti**: ripetere un solo id
  faceva dare al contatore rotante due show identici di fila (`repeat` 2-3, gate fallito).
- **Nessun secondo blocco di fumetti**: con id ≥156 a 0x390000 il leak sale a 196 (fix per
  chiamata) o 172 (indirizzo in cache) contro il criterio 123±15. Strada 2: sostituire.

### Due trappole emerse dai test di v78b (importanti)
- **Il byte del pad è a `0xFFFF01A` ma si legge nel byte ALTO.** La RAM del 68000 è
  byte-swapped: la parola a quell'indirizzo vale `0x1000` con l'acceleratore premuto,
  `0x1100` con B+SU, `0x0000` a zero input. In Python lo stesso byte è `rb(0xF01B)`,
  **non** `rb(0xF01A)`. Con la maschera sbagliata (`0x00FF`) il contatore di inattività
  saliva anche col pulsante premuto.
- **Un blocco di codice dentro `show_start` gira OGNI FRAME**, non una volta per show:
  il decay dell'escalation stava lì e azzerava il contatore errori ogni 4 frame
  (livello sempre 0 misurato, con 36 crash in wild). Le tabelle di escalation sono
  indicizzate a `(liv-1)*6` **sommato all'indirizzo**: indicizzando come un blocco unico
  da 32 si legge dentro la tabella successiva (era il `wrong` 3/3/8).

### Consegnata: **v85** (`c7276aa46f94594bac86450dd9feb7b37753ea99`)

- **Escalation** con decadimento **a tempo** (20 s puliti → livello −1) e rampa del personaggio:
  livello 0 Gianni, 1 → 50% Gesù, 2 → 75%, 3 → sempre. Misurato: livello 3 in ogni seed, **Gesù 100% dal 4° errore**.
- **Inattività** (task D): 10 s senza tasti in gara → Gianni dice una delle 4 frasi già esistenti (id 35-38,
  `itab` dedicata). Nessun testo nuovo, si resta a 156.
- Gate a 5 seed: `wrong`/`vsbad` 0 su 15 run, desync 0 su 15 run, leak entro soglia, `itab` 0 nei gate.

**Ripristino: la v78** (`OutRun_Sglavinated_v78.gen`, `a07946b7`), non la v79 — la v79 scriveva
`VAR196` dentro il buffer HUD e i controlli oggi la rifiutano. Il codice di ripristino è
`tools/patch_z.py.bak` = v78, e ricostruisce senza opzioni speciali.

### Fatto in v78b/v85
- **Escalation**: crash → livello → frasi più dure **e** personaggio più aggressivo.
  wild 5 seed: livello max 3, Gesù 89-97% dal 4° errore.
- **Inattività (task D)**: 10 s senza tasti in gara → Gianni dice una delle 4 frasi
  (id 35-38, già scritte nella v77). Nessun testo nuovo, si resta a 156.

### File fuori da `h40z.s` (l'assembly non è tutto in un posto)
| cartella / file | contenuto | ruolo |
|---|---|---|
| `rst/` | `rst.s` + `rst.bin` | **stub di reset**, installato da `build7.py` al vettore 0x3B3F00: azzera 0xFF5300-0xFF5BFF (576 `clr.l`) e **0xFF8F00-0xFF8FFF (64 `clr.l`)**, poi `jmp 0x200`. È lui a far partire pulite le variabili nuove 180-190 e 224+ |
| `smoke/` | fumetti di fumo e di schermata | dati grafici |
| `hud/` | ricolorazione HUD e tile del punteggio | scrive in VRAM |
| `gstatic/` | tabelle statiche (`gmask.s`, maschere) | dati |
| `shots/*.s` | blob dei fumetti già convertiti | dati |

`rst/` finisce nelle prove ROM ma **non** passa per `patch_z.py`: se lo modifichi va
rigenerato `rst.bin`. Attenzione: `rststep` in `h40z.s` non c'entra niente, è la routine
di verifica con checksum.

### Zona VAR: la mappa e il controllo
`tools/VARMAP.md` mappa 0xFF8F00-**0xFF8FFF**; `tools/check_vars.py` (nella pipeline
prima di `build7.py`) fallisce se un accesso VAR entra in zona riservata, se un offset
non è documentato in "Occupato", se un offset è ≥ 256 (oltre `0xFF8FFF` non è RAM
nostra), o se `rst/rst.s` smette di coprire esattamente 0xFF5300+2304 e 0xFF8F00+256.
Scansiona `h40z.s` **e** `rst/rst.s`. Libero utile: **224-255**; 192-223 è il buffer
HUD (`lea 0xFF8FC0`), e `VAR196` ci era finito per errore.

### Provato e **riportato indietro**
| Intervento | Perché |
|---|---|
| UFO annullato su crash/uscita di strada (`j_rest` su VAR168/VAR166) | i vincoli passavano ma gli show non cambiavano (35/36/44 vs 36/36/41); il caso da servire è rarissimo (3 eventi su 44) e per quelli il ritardo era **minore** senza UFO; `leak` saliva a 396 su wild. Codice commentato in `patch_z.py` con le misure |

### Da fare, in quest'ordine
1. **Test su Pi** della build corrente.
2. **Frasi**: rivedere i testi esistenti (137 originali, alcuni banali) e usare le ~34 frasi già scritte ma non inserite per il limite di 156.
3. **Stabilità UFO**: la posizione è legata al contatore globale (`x = 128+340-(frame&0x7FF)`) invece di un tempo proprio. Serve un VAR dedicato, azzerato da `ufo_cancel`. Spegnimenti misurati: 7-8 per run (57-75% blocco VRAM rubato, 25-43% frame senza budget CPU).
4. **Filtro velocità sugli show periodici**: gli show interrotti hanno velocità d'ingresso mediana 308 contro 93 dei normali. Sopra ~250 non partire. Misurare `short` prima/dopo.
5. **Codici di ritorno di `salloc`** (0 ok, 1 riprova, 2 rinuncia): misurare se fallisce per budget CPU o per ricerca blocchi. Se è budget, non forzare.
6. **Punto 3, da decidere**: oggi Gesù esce a **ogni** crash (`hk_after`: "any crash, even a spin or a bump") e l'uscita di strada fa uscire **Gianni**. Nel test: 21 Gesù / 16 Foglia.
7. **Escalation**: dopo 3, 4 e 5 stabili. Slot liberi `VAR+180/182/184` (0xFF8FB4/B6/B8). Più errori → messaggio più forte e personaggi più frequenti.

### Idee accantonate
- **UFO che "uccide"**: richiesta respinta nella cornice "uccide i tossici/le persone". Un effetto arcade sul traffico (un'auto che sparisce o va in testacoda) è una meccanica diversa ed è una decisione del progetto. Va in fondo alla lista.
- Macchina che "vola" oltre una velocità estrema, bump con altre macchine, musica `.xm` distribuita nel gameplay: noti, non implementati.

## 1. What the hack is
Mega Drive OutRun (USA/Europe, 1 MB) + a 68000 VBlank hook at `0x3B0000` that adds characters, effects and new HUD, plus a Z80 PCM radio player. Output ROM 4 MB. Target: **PAL**, RetroPie Pi 400, lr-genesis-plus-gx. NTSC not a goal.

## 2. Characters (cause → effect)
| Character | Image | Trigger | Phrase source | Behaviour |
|---|---|---|---|---|
| **Gianni** (cannabis leaf, the tempter) | `grafica/gianni_8_pose.png` | off-road / road edge ≥8 frames (cooldown 24) → `otab`; also periodic (`pr_tab` interval) → `ph_tab` | Gianni ids | slides in from right, sway `sw_tab`, pose every 16 frames, smoking anim; exits with `xo_tab` (24 fr) |
| **Gesù** (the corrector) | `grafica/gesu_4_frame.png` | **any crash / rollover** (always wins, cuts Gianni) → `xtab`; also periodic → `jtab` | Jesus ids | drops from top (64 fr), `bob` hover, rises out in last 60 fr |
| **UFO** | `grafica/ufo_2_frame.png` | random/periodic, `(frame&0x7FF)<420`, only if no show | none | crosses right→left, wobble `tw` |
| Gianni alt frames ("daughter", pink) | `grafica/ds_frames.png` | alternate pose set (DS0/DS1, palette DPAL) | — | used by `gfr0` when VAR62 set |

Phrase bubble: 96×24 px = 3 sprites 4×3 tiles, appears ~frame 30 (`bz_tab` bounce). 156 bubbles. Text list with id/character: `tools/gianni12.py` (`PHR` + `CHAR`) and JSON `phrases`.

**Phrase id → character ranges (hard-coded in asm, keep consistent):**
Gianni 0–26, 34–65, 85–104, 121–124, 134–136, 137–140, 142–146, 152–153 · Gesù 27–33, 66–84, 105–120, 125–133, 141, 147–151, 154–155.
Fonte unica: `CHAR` in `tools/gianni12.py`, letta da `lr/phr.py` (usata da `final.py` e `chk.py`). Non modificare più gli insiemi a mano nei test.

## 3. Customized elements (all non-original content)
| Element | Where in code | Data / generator | Preview |
|---|---|---|---|
| Gianni sprite (8 static poses, 4 pieces 4×4) | `show_update`, `gm_do`, `gmask` | `shots/g_frames.bin` (from `tools/gianni12.py` art code) | `grafica/gianni_8_pose.png` |
| Gesù sprite (4 frames, head 4×4 + body 4×2) | `jp_a`, palette `jpal` | `shots/g_app4.bin` @ +0x900 | `grafica/gesu_4_frame.png` |
| UFO (2 frames: 4×4 + 4×3 + 4×2) | `apps`, `ufo_off`/`ufw`/`ufs` | `shots/g_app4.bin` @ 0 | `grafica/ufo_2_frame.png` |
| Speech bubbles (156) | `sb` | `shots/g_bubbles2.bin` ← `gianni12.py` PHR | `grafica/fumetti_156.png` |
| Title "SGLAVINATED" (y=158, x=236) | `title_update` | `shots/g_title.bin` (TSRC) | `grafica/titolo_tiles.png` |
| Title smoke from exhausts (sprites 73–76) | `smoke_upd` | `smoke/mksmoke.py` | `grafica/anteprime/smoke_preview.png` |
| HUD accelerometer (replaces original dot) | `nobar` | `hud/mkhud.py` → `hud/hud_data.s` | `grafica/anteprime/hud_accelerometro.png` |
| HUD recolour (TIME/SCORE/stage) | `hudrc` | LUTs in `hud_data.s` | — |
| Ending screen "SEQUESTRATO ALLE MAFIE" | `endshow` (ENDIMG/ENDPAL) | `shots/g_end.bin` | `grafica/finale_schermata.png` |
| Ending sponsor sprite | `wspr`/`wload` | `shots/g_woman.bin` (4×4 + palette) | `grafica/finale_sponsor_sprite.png` |
| Radio PCM (2 stations, per-stage pieces) | Z80 code in `build7.py` | `shots/pcmr_M.bin`, `pcmr_A.bin` ← `tools/mkradio2.py` | — |
| Text renames | `build7.py` `rep()` | BEST OUTRUNNERS→TOP SGLAVINATI, YOUR SCORE→TORNA VIVO, CONGRATULATIONS→ARRIVATO LUCIDO, ranking names (GIANNI, S.GLAV, BRO…), PASSING BREEZE→ATMOSPHERIC, SPLASH WAVE→IMPORTANT | — |
| Reset stub (clean RAM on soft reset) | `rst/rst.s` @ 0x3B3F00 | `rst/rst.bin` | — |

Regenerate all previews: `python3 grafica/render_assets.py` (run from root; writes to `grafica/`, palettes for title/UFO approximate).

## 4. Rules (do not break)
1. **Zero glitches**: VBlank hook must finish inside budget. Every VRAM/DMA write goes through `room()` (PAL budget `PBUDGET=58` lines). Test result must be `vsbad 0`.
2. **VRAM**: characters borrow 16-tile blocks only from **28–87**, never 62–63, 74–77, never 0–27 (sky/HUD) or ≥88. Borrowed tiles are backed up in `BAKT` and restored. `hidemine` hides game sprites using borrowed blocks in the same frame.
3. **Sprites**: our slots are 70–76 (characters/UFO/bubble), 73–76 title smoke. `sat_scan` chain 0→70..76→78.
4. **Palette**: line 1 is ours during shows (`palguard`). Don't change other lines.
5. **Character ↔ phrase**: Gianni only says Gianni ids, Gesù only Gesù ids. Crash → always Gesù. Off-road → Gianni.
6. **Readable**: show length `220 + 30·stage` frames; bubble hidden before exit (Gianni <30, Gesù <55 frames left).
7. **Animations at game frame rate**, no jitter: pose changes on fixed frame counts, sway/bob tables only.
8. Pause, menus, demo (attract mode) must stay untouched — no characters there.
9. Edit only `tools/patch_z.py` (append `rep(old,new)` before the LAST `open('tools/h40z.s','w')`). Never hand-edit generated `.s`.
10. Content: Gianni tempts with drug slang, Gesù answers with an anti-drug message. Bubble text: UPPERCASE, max 2 lines, each ≤88 px (~16 chars). No sexual content.
11. Delivery: one `.gen` file for testing on the Pi.

## 5. Free resources
ROM free: 0x0EDCC2–0x0EFFF7 (8 KB), 0x0FE080–0x0FFFFF (7 KB), 0x390000–0x39FFFF (64 KB), 0x3A8E40–0x3AFFFF (28 KB), 0x3DA880–0x3DFFFF (21 KB).
RAM: hook vars at `VAR` 0xFF8F00 (offsets in JSON), work buffers 0xFF4000–0xFF5BFF, DANGER map 0xFF8D00.

## 6. Why things are the way they are (regression traps)
| Symptom seen before | Cause | Fix kept in v74 |
|---|---|---|
| Characters rare in PAL | `room()` assumed NTSC | PAL VBLANK + line-wrap logic |
| Blank bubbles | game rewrote palette mid-show | `palguard` |
| Garbage in sky | plane scan covered half plane, blocks 0–27 | full 64-row scan + blacklist |
| Same phrase repeated | reselection during loading | counter VAR178 increments at `ss_go` |
| Jesus with Gianni phrase / vice versa | table picked without char check | guards in `ph_nx`, xtab needs VAR24 |
| No Jesus after crash | alternation rule | removed; crash overrides retries |
| Gianni vanishing abruptly / garbled | no exit anim; overwritten tiles | `xo_tab` exit; `gcheck` every frame when VAR46≥8 |
| Billboards sharing character tiles | `sat_scan` never reached game sprites | jump 70→78 |
| Hook overrun (stutter) | `hidemine` too long | re-check room every 8 entries |
| Choppy Gianni | anim + reload every pose | static body art + `gmask` skip identical pieces |

## 7. Ideas not done
SEGA logo → Sglavinated splash · reaction to car-vs-car collision · characters in demo.

## 8. Test
`cd lr && python3 final.py ../out.gen 16000 left` (and `right`, `wild`). Accept: vsbad 0, wrong 0. v74 reference: ~30 shows/16000 frames, short 1–4, leak frames <200.
