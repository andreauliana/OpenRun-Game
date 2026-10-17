# OutRun Sglavinated — build riproducibile

## Requisiti
- Linux / WSL / macOS, Python 3.10+, `pip install numpy pillow`
- **binutils m68k**: servono `m68k-linux-gnu-as` e `m68k-linux-gnu-ld` in `PATH`.
  Debian/Ubuntu: `apt install binutils-m68k-linux-gnu`.
  macOS (Homebrew): compilare binutils `--target=m68k-linux-gnu`; i binari devono
  avere **questi nomi esatti**, altrimenti `build7.py` non li trova.
- Font: **usa `tools/DejaVuSans-Bold.ttf`**, già nel repo. `gianni12.py` lo preferisce
  al path di sistema: versioni diverse di DejaVu danno larghezze diverse (1 px su una
  riga) e quindi fumetti diversi da macchina a macchina.

## ROM base (NON inclusa)
Metti il tuo dump in: `outrun_src/OutRun (USA, Europe).md` (1 MB)
SHA1 atteso: `95d2055ffd679ab19f0d4ca0af62a0d565bc258e`

## Build (dalla root della cartella)
```
sh tools/build_all.sh out.gen 81 6
```
La pipeline è **un file** (`tools/build_all.sh`) e fa, con `set -e`:
```
rm -f tools/h40y.s tools/h40z.s tools/gianni_hook.s   # niente .s vecchio
python3 tools/patch_y.py            # h40x.s -> tools/h40y.s
python3 tools/patch_z.py            # h40y.s -> tools/h40z.s
python3 tools/check_vars.py         # FA FALLIRE se un VAR tocca una zona riservata,
                                    # se un offset non e' documentato in VARMAP.md,
                                    # se rst/rst.s non copre la zona, se un id di itab
                                    # non e' esclusivo
cp tools/h40z.s tools/gianni_hook.s
python3 -I tools/build7.py out.gen 81 6
sha1sum out.gen
```
**Controllo del toolchain**: `tools/patch_z.py.v75` (v75 ricostruita) deve dare
`1cd6af0047a696c1def35ed4077973adb337db7e`. Se cambia, il toolchain non è quello giusto:
non procedere con le modifiche.
Con `tools/patch_z.py` (v77 = v76 + contenuti: `otab` solo off-road, `ctab` solo Gianni,
36 frasi di escalation in `g1t-g3t`/`j1t-j3t`) la build dà
`197866c5ff77d9d7784706472518bea8145f1dbc`.

**Perché la pipeline cancella prima i `.s`**: `patch_z.py` scrive `h40z.s` più volte
durante il suo run; se una `rep` fallisce a metà, resterebbe un `.s` **parziale**, che è
peggio di uno vecchio perché sembra aggiornato. Per questo `rep()` ora solleva un errore
con il testo cercato per intero se non compare esattamente una volta (prima era un
`assert` troncato a 50 caratteri, che sparisce anche sotto `python3 -O`), e
`build_all.sh` ha una trappola che cancella `h40z.s` e `gianni_hook.s` a ogni errore.

**Il punto di ripristino è la v78, e va ricostruita così**

| cosa | file | nota |
|---|---|---|
| **codice** | `tools/patch_z.py.bak` | è la **v78** (`a07946b7`), non la v79 |

```
cp tools/patch_z.py.bak tools/patch_z.py
sh tools/build_all.sh out.gen 81 6
# -> a07946b7e2ae49fd7ec3ce7839567ab8cfce95a3  (v78, escalation senza inattività)
```
La v78 passa `check_vars` così com'è, senza opzioni: non usa `itab` e non tocca zone
riservate (67 byte liberi, offset 180-185 non usati). Nessun `--legacy` serve: la v79
**non è un punto di ripristino** perché scriveva `VAR196`, dentro il buffer HUD, e i
controlli oggi la rifiutano correttamente.

**Zona VAR**: `tools/VARMAP.md` è la mappa degli offset 0xFF8F00-0xFF8FFF e
`tools/check_vars.py` la fa rispettare (nella pipeline, prima di `build7.py`). Regola:
**prima di usare un offset nuovo va scritto nella tabella "Occupato" con un nome**, poi
si rilancia il controllo. I candidati liberi sono 224-255; **192-223 sono riservati al
buffer di ricoloramento HUD** (`lea 0xFF8FC0`), e usarli scrive dentro l'HUD: è successo
con `VAR196`, sintomo fuorviante perché leggendo indietro quel byte sembrava un flag
armato per centinaia di frame. Ultimo offset libero utile: **180**.

**v79 = escalation r2 + messaggio di inattività** (`804b5f1332bc0298c409862d64c0e562b593e233`,
`OutRun_Sglavinated_v79.gen`). Sopra la v78: il decadimento dell'escalation sta in
`ss_nj` (una volta per show, non ogni frame) con soglie 2/3/4, la scala cambia anche
**chi parla** (livello 0 Gianni, 1 → 50% Gesù, 2 → 75%, 3 → sempre) e c'è il messaggio
di inattività: 500 frame senza tasti in gara → una delle 4 frasi già presenti (id 35-38)
tramite la nuova `itab` (4 voci). `ctab` resta codice morto. Contatore in VAR182, flag in
VAR196. Priorità crash e uscita di strada. Attenzione: lo stato del pad è `0xFFFF01A` ma
la RAM è byte-swapped, quindi il byte del pad è nel **byte alto** (maschera `0xFF00`).

**v78 = v77 + escalation collegata al codice** (`a07946b7e2ae49fd7ec3ce7839567ab8cfce95a3`,
`OutRun_Sglavinated_v78.gen`). La patch conta i **crash** in `pe_n2` (dove `VAR60` viene
consumato, una volta per crash) in `VAR186`, i show puliti di fila in `VAR188`, e il
livello 0-3 in `VAR190`. Livello 0 = tabelle normali (`otab` per Gianni off-road,
`xtab` per Gesù crash); livelli 1/2/3 = `g1t`/`g2t`/`g3t` e `j1t`/`j2t`/`j3t`.
Tre show senza errori azzerano il contatore. Reset a cambio stage.
Conta solo il crash: contare ogni uscita di strada faceva saturare il livello a 3
sempre (19/19 show) e cancellava i contenuti nuovi di `otab`.
Attenzione: `nocrash` gira **ogni frame**, non è il posto giusto per contare eventi.

## Verificare una build (ordine obbligatorio)

1. **Prima di ogni intervento**: `cp tools/patch_z.py tools/patch_z.py.bak`.
2. **Confronta prima `tools/h40z.s`, non lo SHA della ROM**: lo SHA cambia anche quando
   cambiano i dati in `shots/`, quindi da solo può ingannare. Il diff di `h40z.s` dice
   esattamente cosa è cambiato nel codice.
3. Solo dopo, lo SHA della ROM.

Esempio, controllo che una modifica ai dati tocchi solo i dati e nessun codice:

```
cp tools/patch_z.py.v75 /tmp/pz.py && cp /tmp/pz.py tools/patch_z.py   # build v75
python3 tools/patch_y.py && python3 tools/patch_z.py && cp tools/h40z.s /tmp/h40z.v75.s
cp tools/patch_z.py.bak tools/patch_z.py                              # build v76
python3 tools/patch_y.py && python3 tools/patch_z.py && cp tools/h40z.s /tmp/h40z.v76.s
diff /tmp/h40z.v75.s /tmp/h40z.v76.s        # atteso: solo ph_tab e jtab
```

Nota su `tools/patch_z.py.bak-v74`: **non è corrotto**, contiene la v74 più le modifiche
di frequenza della v75, quindi non produce lo SHA della v74 e non va usato come riferimento.

## Limiti imposti dall'hardware (non superabili)
| Limite | Valore | Perché |
|---|---|---|
| Frasi (fumetti) | **156** | `BSRC 0x3B4000` → `GSRC 0x3E0000` = 180224 byte, 1152 per frase |
| Larghezza riga fumetto | **88 px** | fumetto 96 px; `gianni12.py` fallisce l'assert sopra questo |
| Codice hook 68k | 0x3B0000-0x3B3F00 | vettore vblank + stub di reset |
| Spazio libero | 0x390000-0x39FFFF (64 KB), 0x3A8E40-0x3AFFFF (29 KB), 512 byte dopo i fumetti | |

## Modificare
| Cosa | Dove | Poi |
|---|---|---|
| Codice hook / logica personaggi | `tools/patch_z.py` (`rep(old,new)` prima dell'ULTIMO `open('tools/h40z.s','w')`) | build |
| Testo frasi | `tools/gianni12.py` liste `PHR` | `python3 tools/gianni12.py` → build |
| HUD accelerometro | `hud/mkhud.py` | `python3 hud/mkhud.py` → build |
| Musica radio | `tools/mkradio2.py` (servono gli .xm originali, non inclusi) | build |

**`shots/g_frames.bin` (arte di Gianni) è un INPUT**: non rigenerarlo.
`gianni12.py` produce solo `g_bubbles2.bin`; i file `.bin` delle altre tavole restano
quelli committati.

Mai modificare `tools/h40z*.s` a mano: viene rigenerato.

## Dati binari in `shots/` (input già pronti)
| File | Contenuto |
|---|---|
| g_frames.bin | Gianni, 8 pose statiche × 4 pezzi 4×4 tile |
| g_bubbles2.bin | 137 fumetti × 1152 B (da gianni12.py) |
| g_bubbles.bin, g_dfr0/1.bin, g_pal.bin, g_dpal.bin | fumetti vecchi, frame Gesù, palette |
| g_title.bin, g_end.bin, g_woman.bin, g_app4.bin | titolo, schermata finale, UFO/apparizioni |
| pcmr_M.bin, pcmr_A.bin, pcmn_d.bin, pcmn_f.bin | PCM radio 1, radio 2, musiche title/end |
| dots.s, horns.s, wtiles.s | include asm |

`rst/rst.bin` = stub di reset (sorgente `rst/rst.s`).

## Test harness (opzionale, Linux)
Emulatore = Genesis-Plus-GX libretro (commit `393f70cd64c7334564c19a424c383f53986c13f4`) + `gpgx/dbgshim.c`:
```
git clone https://github.com/libretro/Genesis-Plus-GX gpgx_src && cd gpgx_src
git checkout 393f70cd64c7334564c19a424c383f53986c13f4
make -f Makefile.libretro -j4
gcc -O2 -fPIC -c ../gpgx/dbgshim.c -o dbgshim.o
gcc -shared -o ../gpgx/gpgx_dbg.so $(find . -name '*.o' ! -name dbgshim.o) dbgshim.o -lm
cd ..
```
Test (dalla cartella `lr/`, PAL):
```
cd lr && python3 final.py ../out.gen 3000 left
# atteso: vsbad 0, wrong 0
```
| Script | Verifica |
|---|---|
| final.py / chk.py | vs-bad (overrun VBlank), personaggio/frase sbagliati, ripetizioni, leak |
| jerk.py, lag2.py | fluidità strada / lag |
| events.py, shows.py | timeline show (offroad→Gianni, crash→Gesù) |
| leak.py | sprite cartelloni che usano blocchi nostri |
| pause.py, demo.py | pausa e demo non toccate |

## Reference
`OPENCODE_REFERENCE_v74.json` (frasi, tabelle, VAR, label→riga asm) e `SPEC_personaggi_v74.md`.
