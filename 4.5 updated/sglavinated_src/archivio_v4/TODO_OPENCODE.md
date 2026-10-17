# OutRun Sglavinated — task list per OpenCode (dalla v76)

Base di partenza: `tools/patch_z.py` = **v76** (ROM `f1d1b3aca998b9e65c59d3688882dcccd8310fa7`, con `shots/g_bubbles2.bin` a 156 frasi).
Esegui i task **in ordine**. Un task = una build + un test + un backup. Non passare al successivo se il gate fallisce.

## Regole valide per OGNI task
1. Prima di toccare: `cp tools/patch_z.py tools/patch_z.py.bak`.
2. Modifiche solo con `rep(old,new)` in `tools/patch_z.py`, appese prima dell'ULTIMO `open('tools/h40z.s','w')`. Mai editare `h40z*.s`.
3. Codice nuovo che gira ogni frame: in fondo a `hk_after` o dietro `room()`, mai prima di `scan`. Preferire modifiche ai DATI (tabelle, frasi): costo zero.
4. Gate di test (dalla cartella `lr/`):
   `python3 final.py ../out.gen 16000 left` e `... 16000 wild`
   Accetta solo se: `vsbad 0`, `wrong 0`, `repeat 0`, `leak` ≤ 200 per 16k frame, `short` non peggiore della v76.
5. Gate di build: se il task non tocca il codice, `tools/h40z.s` deve restare identico (`cmp`). Lo SHA della ROM cambia anche solo per i dati.
6. Ogni tabella frasi contiene SOLO id del personaggio che la usa (Gianni: ph_tab, otab, ctab · Gesù: jtab, xtab). Verifica con `lr/phr.py`.
7. A fine task: aggiorna HANDOVER.md / SPEC (togli le righe superate, non aggiungere duplicati) e scrivi qui sotto lo SHA nuovo.
8. Contenuti: Gianni tenta con slang, Gesù risponde con messaggio anti-droga chiaro. Niente contenuti sessuali. Niente "UFO che uccide persone".
9. **Byte-swap di `lremu`**: `e.wb(indirizzo, v)` su **indirizzo pari** finisce sul **byte dispari** (misurato: `e.wb(0xFF8FBA,1)` scrive a `0xFF8FBB` e la parola letta indietro torna `0x0100`). Per i VAR usa **`e.ww` / `e.rw`** (word big-endian), mai `wb`/`rb` affiancati: altrimenti le misure sono rumore e sembrano bug della ROM. Vale per ogni misura fatta dall'harness, non per il codice asm (che scrive a word con `move.w`).

---

## P0 — Test sul Pi (Andrea)
- **Prova la v85**: `OutRun_Sglavinated_v85.gen`. Se qualcosa non torna, ripristino immediato con `OutRun_Sglavinated_v78.gen` (nessuna build necessaria).
Build v76 su RetroPie (PAL). Annotare: personaggi troppo/poco frequenti? Gianni insistente a bordo strada? glitch?
**Prompt:** nessuno, è un test manuale. Riportare l'esito prima di P3.

## P1 — ✅ FATTO — Correggere la diagnosi otab (solo test/doc, niente ROM)
**Conclusione:** la mia diagnosi "otab spenta" era **sbagliata**. `show_start` testa `VAR166`
prima di `VAR58` per Gianni (`h40z.s` 982-985), quindi `otab` viene usata. Il mio `otab.py`
campionava `VAR166` dopo lo step, quando `ph_d` l'ha già azzerato nella stessa VBlank: da qui
lo 0 falso. Ora la classificazione legge le tabelle dal sorgente.
**Caveat scoperto:** 13 id sono in `otab` **e** in `ph_tab`, quindi la frase da sola non
basta per attribuire; `otab.py` li conta come otab solo entro 60 frame da un trigger.
**Prompt:**
> otab NON è spento: in `show_start` VAR166 è testato prima di VAR58 (righe ~982-986 di h40z.s). Tracciato su v76 wild: gli show dopo uscita di strada hanno frasi 135,136,137,139,142,146,153,123,15,17 = otab in sequenza. Correggi `lr/otab.py` perché riconosca le frasi leggendo la tabella `otab` dal sorgente asm (non una lista fissa), rimisura e correggi la documentazione.

Gate: otab.py deve riportare >0 show otab in wild.

## P2 — Bug latente `ctab` (dati)
**Prompt:**
> `ctab` è letta solo nel ramo Gianni (`ph_nj`, VAR24=0 e VAR122≠0) ma contiene 16 id Gesù (33,74,77,79,83,84,110-119). Oggi VAR122 non viene mai impostato quindi è inattiva. Rimettila con soli id Gianni (32 voci) e annota in SPEC che VAR122 non ha writer (ctab inutilizzata).

Gate: `cmp` comportamento invariato (stesse metriche della v76), `phr.py` coerente.

## P3 — Uscita di strada davvero contestuale (frasi + dati)
Oggi otab ha 32 voci ma solo 134/135/136 parlano della strada; il resto sono frasi generiche.
**Prompt:**
> Aggiungi le frasi off-road approvate da Andrea (lista sotto) a `PHR` in `tools/gianni12.py` con `CHAR='G'`. Se si supera 156 frasi, implementa il secondo blocco: id ≥156 letti da `0x390000` (64 KB liberi, +56 frasi) — modifica minima nel calcolo indirizzo della bubble, poi verifica che `build7.py` scriva i dati lì. Riempi `otab` (32 voci) SOLO con frasi off-road (134,135,136 + nuove, ripetute a riempire). Rigenera `g_bubbles2.bin` con il font in `tools/`.

Gate: regole 4 e 6; tutte le frasi ≤88 px; nel test wild ≥80% degli show off-road hanno frase off-road.

Bozza frasi off-road di Gianni (Andrea approva/modifica prima):
| Riga 1 | Riga 2 |
|---|---|
| ERBA A BORDO? | LA MIA E' MEGLIO |
| SEI NEL PRATO, | COME ME, FRA |
| SULL'ERBA VAI | PIU' LEGGERO |
| PERSO LA STRADA? | TE LA RULLO IO |
| BORDO? RILASSATI, | FATTI UNA DIERA |
| IN MEZZO AI CAMPI | CRESCO IO, FRA |
| SBANDI? NORMALE, | SEI GIA' IN VOLO |
| SBANDATA BELLA! | UN ALTRO GIRO? |
| RUOTE NEL VERDE: | CASA MIA, BRO |
| GHIAIA E FUMO, | COPPIA PERFETTA |
| FRENI? MA NO, | ACCENDI E VAI |
| CHE CURVA STORTA, | COME LA CANNA |

## P4 — Cooldown uscita di strada
**Prompt:**
> Lascia `cmpi.w #24,(120)` salvo diverso esito di P0. Se Andrea lo trova insistente, porta a 40 e rimisura la quota di show off-road.

## P5 — Stabilità UFO (codice)
**Prompt:**
> La x dell'UFO è `128+340-(frame&0x7FF)`: legata al contatore globale. Sostituisci con un timer proprio in un VAR libero (verifica che sia libero con grep, es. VAR+186), azzerato a ogni partenza e da `ufo_cancel`. Il codice va dietro `room()`. Misura spegnimenti UFO (oggi 7-8/run) e leak.

Gate: regola 4; leak non superiore alla v76 (169/32k left).

## P6 — Filtro velocità sugli show periodici (codice, piccolo)
**Prompt:**
> Gli show interrotti hanno velocità d'ingresso mediana 308 vs 93. Nel percorso periodico (`pe_nb`/`periodic`, NON crash né off-road) non avviare lo show se `0xFFF03E` ≥ 250: rimanda (lascia (28) a un valore breve). Misura `short` e numero show prima/dopo.

Gate: `short` in calo, show totali non sotto il 90% della v76.

## P7 — Diagnosi retry `salloc` (solo misura)
**Prompt:**
> Strumenta in `lr/` (senza toccare la ROM) i ritorni di `salloc`: 0 ok, 1 riprova, 2 rinuncia, e il motivo (budget `room()` insufficiente vs nessun blocco libero). Report in tabella. Se il motivo è il budget CPU, NON forzare.

## P8 — Gesù: tutti i crash o solo i gravi (decisione di Andrea)
Opzioni:
- A) com'è ora: ogni crash → Gesù.
- B) solo crash gravi (ribaltamento/velocità alta + durata) → Gesù; testacoda/colpetti → nessuno o battuta di Gianni.
- C) via di mezzo: uscita di strada breve → Gianni; lunga o ripetuta → Gesù.
**Prompt (dopo la scelta):**
> Implementa l'opzione scelta classificando il crash con `0xFFF039`, velocità all'impatto e durata. Il crash grave deve restare prioritario su tutto.

## P9 — Escalation errori (codice, dopo P5-P8 stabili)
**Prompt:**
> VAR+180 errori nello stage, VAR+182 livello 0-3, VAR+184 frame dall'ultimo errore (decay). Verifica che siano liberi. Reset a cambio stage. Livello alto → frasi più dure (sottoinsiemi di xtab/otab) e pr_tab più corto. Nessun costo prima di `scan`.

## P10 — Opzionali (in fondo)
- Testacoda di un'auto del traffico al passaggio dell'UFO (effetto arcade, riusa sprite traffico; codice dietro `room()`).
- Logo SEGA → splash Sglavinated.
- Reazione a collisione auto-auto.
- Personaggi nella demo (regola attuale: demo intatta — solo se Andrea lo chiede).

---
## Registro build
| Task | SHA1 ROM | Esito test |
|---|---|---|
| v76 base | f1d1b3aca998b9e65c59d3688882dcccd8310fa7 | 32k left: 80 show, wrong 0, leak 169 |
| P1 ✅ | invariata (nessuna modifica alla ROM) | `otab.py` wild: 27 trigger, 15 show con frase in otab, **13 attribuiti**, consecutivi max 2, distanza min 121 frame · left: 10 attribuiti, consecutivi max 2, distanza min 115 · gate superato (>0 in wild) |
| P3 ⚠️ → v77 | `509bf101c867bc7e1e17b4a4a1c9fed5e0d70a10` | **Secondo blocco ABANDONATO**: con id ≥156 a 0x390000 il leak sale a 196/16k left anche calcolando l'indirizzo una volta sola per show (cache BADDR, criterio 123±15 non rispettato). Strada 2: **156 frasi, sostituzione di testi**. left 36 show / wrong 0 / repeat 0 / vsbad 0 / **leak 123** / short 4 / ufo 5 · wild 41/0/0/0/**129**/5/6 — metriche identiche alla v76, costo runtime zero |
| audit VRAM ✅ | strumento | `lr/vramaudit.py`, left/right/wild 16k su v76 e P3: **corrotto 0 e piani 0 in tutte e 6 le run** (il gioco non riscrive i tile nostri durante uno show, i piani non puntano a blocchi presi). Il leak è solo sovrapposizione di tile su sprite. Blocchi più colpiti: 44 (1568 frame, 0 hit DANGER), 42, 34, 40, 68 |
| **v77 finale** | `197866c5ff77d9d7784706472518bea8145f1dbc` | **solo dati, zero codice**. 156 frasi. left 36 show / wrong 0 / repeat 0 / vsbad 0 / leak 123 / short 4 / ufo 5 · right 36/0/0/0/157/2/4 · wild 41/0/0/0/129/5/6 — **identiche alla v76**. Titolo+demo 6000 frame senza input: vsbad 0, 0 personaggi, 0 UFO. Contenuti: `otab` = solo frasi off-road (16 id riscritti), `ctab` = solo Gianni (chiude il bug P2), 36 frasi di escalation in `g1t-g3t`/`j1t-j3t` (dati pronti, **codice non ancora collegato**). Attenzione: ricomponendo ph_tab/jtab/xtab, riempire **ciclando** i valori rimasti, mai ripetendone uno solo (causa `repeat`). |
| v78 step 2 (dati audit) ⏪ **ritirato** | — | ord_tab con 44 in fondo + blacklist 34/40/52/64/68 → leak 123→109 / 157→**237** / 129→195. Solo ord_tab → 123→**280** / 157→188 / 129→78. **Conclusione: `leak` non è una funzione della bontà dei singoli blocchi**, cambia di ±100 frame con quali blocchi vengono allocati (è fase: quale blocco tocca a quale sprite in quel frame). Le stime per-blocco di `vramaudit.py` non sono traducibili in una riduzione del leak |
| v78 step 3 (escalation A) ⏪ **ritirato** | (patch salvata in `/tmp/patch_z.escalation.py`) | leak entro ±15 in 2 modalità su 3 ma **`wrong` 3/3/8** e `leak` wild **217** > 200. Codice solo agli eventi (nessun costo per frame): contatore in VAR186, ultimo errore VAR188, livello VAR190, stage VAR184. Tre bug trovati: (1) tabelle indicizzate a passo 2 invece di 6 (poi anche a passo 6 ma con slot non limitato: leggevano dentro la tabella successiva); (2) stavo scrivendo su VAR192+ che è il **buffer ricoloramento HUD** (0xFF8FC0); (3) ~~**bug latente v76**: il retry in `pend` salta `pe_g`~~ → **RITIRATO, NON ERA REALE**: era un errore di `lr/desync.py`, che leggeva la frase a `0xFF8F0C` (VAR12) invece di `0xFF8F12` (VAR18). Con l'offset giusto la v77 dà 0 disallineamenti su 15 run. Le patch provate su quella diagnosi (dispatch su VAR24, retry allineato, guardia VAR52) sono state tutte rimosse |
| v78 F3 (audio) ⚠️ | `lr/audio_audit.py` | Il core **emette solo zeri**: 0 campioni non nulli su 2.1M dal callback batch. Strumento pronto (picco/RMS su blocchi da 2 s + WAV), ma la misura non è possibile finché l'audio non esce. Da verificare sul Pi prima di darlo per un bug della ROM |
| v78 step 1 (documenti) ✅ | — | `OPENCODE_REFERENCE_v74.json` (156 frasi + tabelle + escalation), `SPEC` (v77 + 2 sezioni), `BUILD` (hash v77), `HANDOVER` (stato + 3 regole), `grafica/` rigenerata con `fumetti_137.png` → **`fumetti_156.png`** |

| v78b punto 1 — **misura stabile del leak** ✅ | strumento | `lr/leakstats.py`: 5 seed × 3 modalità, leak **normalizzato a 1000 frame di show**, media ± dev.std. Base **v77** (`197866c5`): left **24.51 ± 11.73** (15.5-44.7) · right **14.99 ± 8.07** · wild **18.37 ± 8.67** (5.9-30.6), wrong 0 e vsbad 0 su tutti i 15 run. **Soglie "peggiora" = media + 2·sd: left ≈ 48, right ≈ 31, wild ≈ 36.** Attenzione: `sglavinated_src/out.gen` è la build **v76**, non la v77 — le misure vanno fatte su `/tmp/out.v77.gen` (copia di `OutRun_Sglavinated_v77.gen`) |
| v78b rimisura ord_tab/blacklist ⏪ **definitivamente ritirato** | `17701a750e157cafc799ac7ae6c0078c25aacc26` | Rimesurato con il metodo stabile: left **21.53 ± 7.67**, right 15.19 ± 7.78, wild 19.87 ± 5.08 contro la base v76 24.51 ± 11.73 / 16.19 ± 7.56 / 18.37 ± 8.67. **Le differenze stanno dentro il rumore**: l'escursione fra seed di una stessa modalità (left da 15.5 a 44.7, fattore 2.9) è molto più grande di qualunque "miglioria" di −14/1000. Nessuna ottimizzazione del leak dentro il rumore |
| v78b punto 2 — **test di desync** ⚠️ strumento incoerente | `lr/desync.py` | Registra VAR24/VAR58/VAR166/VAR168/frase/VAR84 a ogni show e segnala il primo disallineamento. **Ma non concorda con `final.py` sulla stessa identica run** (stesso ROM, seed 0, wild, 6000 frame, 16 show): `final.py` dà wrong 0, `desync.py` dà 7 eventi. Inline con la stessa logica di `final.py` la frase letta agli stessi frame è 135/136/142/144/146 (id di Gianni), mentre `desync.py` legge 128/129/130 (id di Gesù). **Finché i due non concordano il test non è affidabile** e non si può fixing sul desync |

| **v78 ESCALATION** ✅ | `a07946b7e2ae49fd7ec3ce7839567ab8cfce95a3` (`OutRun_Sglavinated_v78.gen`) | La patch salvata era rotta: indicizzava `g1t`/`j1t` come un blocco da 32 e leggeva dentro la tabella successiva (`wrong` 3/3/8), e usava `%a0@` che è l'indirizzo, non il frame. Riscritta: livello = `(liv-1)*6` **sommato all'indirizzo** con slot interno limitato a 0-5, decadimento per show puliti (3) invece che per frame. Conteggio solo dei **crash** in `pe_n2` (in `nocrash`, che gira ogni frame, il contatore saliva 7672 volte in 8000 frame). **Funziona**: con livello forzato 2 in asm Gianni prende 6/10/12 (g2t) e Gesù 66/67/70 (j2t). Misura 5 seed, leak per 1000 frame di show: left **22.15 ± 7.25** · right **22.78 ± 10.21** · wild **16.15 ± 5.95**, contro v77 24.51 ± 11.73 / 14.99 ± 8.07 / 18.37 ± 8.67 — tutte entro la soglia (media + 2·sd: 48/31/36), `wrong` e `vsbad` 0 su tutti i 15 run, **desync 0 su tutti i 15 run**. `short` (somma 5 seed): left 20→**10**, right 12→**16**, wild 18→**7**: nel complesso 50→33, nel criterion «non peggio». Gate classici seed 0: left leak 114 · right 189 · wild 194, `wrong`/`repeat`/`vsbad` 0 |

| **v79 = escalation r2 + task D** ⏪ **RITIRATA** | `804b5f1332bc0298c409862d64c0e562b593e233` (`OutRun_Sglavinated_v79.gen`) | **Escalation r2**: il decay era nel blocco di scelta frase, che gira **ogni frame** → azzerava VAR186 ogni 4 frame e il livello restava **0** (misurato: 36 crash in wild, livello max 0). Spostato in `ss_nj` (una volta per show), soglie 2/3/4. Aggiunta la **rampa del personaggio** (0→Gianni, 1→50% Gesù, 2→75%, 3→sempre): `v78_fg` scrive VAR58=2 **e** VAR24=2, saltare a `pe_g` non bastava (pe_g imposta VAR24=2 solo se VAR58 è già 2). Misura wild 5 seed: **livello max 3 in tutti gli stage**, show su tutti i livelli, **Gesù 89-97% dal 4° errore** (era 0% prima). **Task D inattività**: nessun testo nuovo, si riusano gli id 35-38 già in `ctab` dalla v77 (≤88 px, ≤16 car, verificati). `ctab` però è codice morto (ph_nj si raggiunge solo con VAR58=0, che pend non lascia mai) → nuova `itab` a 4 voci con ramo dedicato in testa al dispatch. Timer in VAR182, flag in VAR196 azzerato in `ss_nj` (uno solo per pausa), forza Gianni con VAR58/VAR24, priorità crash e uscita di strada. Test: messaggio a 515-543 frame (500 + show residuo, è il comportamento voluto), **riarmo OK**, rotazione delle 4 frasi OK, **0 messaggi in 4000 frame di titolo/demo** (stati 0/24/28/32), VAR182=0 in guida. **Bug trovato e corretto**: la priorità del crash cambiava il personaggio in Gesù *dopo* la scelta della frase → frase di Gianni con faccia di Gesù (`wrong` 6/7/7, 15 desync su 15); corretto con due guardie (non spara se VAR60/VAR166, e il ramo itab scarta se VAR24≠0). **Motivo del ritiro**: il flag di inattività stava in `VAR196`, dentro il buffer HUD (offset 192-223): scriveva nell'HUD e, leggendo indietro quel byte, sembrava armato per centinaia di frame in run dove il timer non era mai salito. La v79 non è più un punto di ripristino: il ripristino è la **v78**. Gate finali: **wrong 0, vsbad 0, repeat 0, desync 0 su 15+15 run**; leak 5 seed left 39.96 ± 9.77 (max 49.5) · right 15.08 ± 3.27 · wild 23.82 ± 11.74 — medie entro soglia (48/31/36). Gate classici seed 0: left leak 369 · right 140 · wild 203 |

| **v85 = escalation col decay a tempo + inattività** ✅ (consegnata) | `c7276aa46f94594bac86450dd9feb7b37753ea99` (`OutRun_Sglavinated_v85.gen`, da `tools/patch_z.py` = `2dffd465`) | **Gate a 5 seed, 16000 frame** — leak per 1000 frame di show: left **24.55 ± 9.50** (12.9-37.1) · right **19.31 ± 10.07** (9.0-31.1) · wild **26.10 ± 6.34** (19.2-34.4). `wrong` **0** e `vsbad` **0** su tutti i 15 run, `short` 14/9/16. Anti-deadlock: nessuno stage senza show, `VAR180` armato al massimo **0** frame (limite 600). **Desync 0 su 15 run** (5 seed × 3 modalità). **Inattività**: `pad_test` TUTTI OK sulle 3 modalità (0 messaggi con B tenuto 30 s, 1 messaggio a 517 frame senza input) e `idle_test` TUTTI OK; **`itab` selezionata 0 volte nei 15 run dei gate**. **Escalation**: livello max **3** in ogni seed di left e wild, show distribuiti su tutti i livelli, **Gesù al 100% dal 4° errore** su tutti i 10 run. Decay **a tempo**: 1000 frame (~20 s) senza errori → livello −1, con l'istante dell'ultimo errore aggiornato sia al crash sia all'uscita di strada. Oltre ai gate: `tools/VARMAP.md` + `tools/check_vars.py` (nella pipeline, prima di `build7.py`), `tools/build_all.sh` con scrittura **atomica** di `h40z.s` e `rep()` severo, id di `itab` **esclusivi** (controllo automatico), gate anti-deadlock in `leakstats.py` e `final.py` |

| **v86 task 1 = UFO_FUGA_BREVE** ✅ (intermedia, non ancora consegnata) | `9fead172b225f54d90d676c0b67d135e2d3df63a` (`OutRun_Sglavinated_v86_ufo1.gen`, da `tools/patch_z.py` = `0ac07c36`, backup `patch_z.py.bak-v85c` = `708ba1dc`) | Quando il gioco si riprende un blocco dell'UFO ma la grafica è **ancora integra** (ramo `VAR74` di `sat_scan`), l'UFO non sparisce più di colpo: 12 frame di fuga e poi l'abort differito. `ucheck` riletto in sola lettura (word 66 salvata su stack: `VAR67` è un byte all'offset dispari). `ufochk.py` 16000 frame: **sparizioni 0** (era 1) su left/right/wild, `fuga_breve 1`, passaggi e fuga da show **agli stessi frame** della v85c. **Regola trovata**: non aggiungere istruzioni ad `ap_x`, che gira ogni frame anche in attract → spostato il check su `ap_fle` e la divergenza della RAM è passata da frame 129 a frame 8402. Gate 5 seed × 3 modalità: leak left **33,2 ± 17,3**, right **19,0 ± 4,1**, wild **18,0 ± 6,1**, tutte entro soglia 48/31/36; `wrong` 0, `vsbad` 0, `desync` 0 su 15 run; `short` 3/2/4 → **2/2/2**. `jerk` 4/840 e `lag2` 22,8% **identici** alla v85c. Costo: `leakwho` 220 → 232 frame (+5%, dentro il rumore fra seed). VAR nuovi 242/244 in VARMAP; `check_vars.py` 179 byte, exit 0 |
| **v87 task 2 = BOTTA (auto colpita)** ✅ (consegnata) | `3fddbeb6c82c30c8bb2c0c3633248d2b62689a0d` (`OutRun_Sglavinated_v87.gen`, da `tools/patch_z.py` = `34f329b4`, snapshot `patch_z_history/patch_z.v87.py`, ripristino `patch_z.py.bak-v86-ufo1` o `patch_z_history/patch_z.v86_ufo1.py` → `9fead172`) | L'auto colpita **scivola di lato** (testacoda di 24 frame, X verso il margine opposto ≥ 64 dal giocatore, mai dentro la zona di collisione 240+) e poi **sparisce**: si scrive solo `Z = 256` nel suo record e il gioco la libera al prossimo aggiornamento (+1000 come un sorpasso). Nessuna grafica nuova, nessuna `clr.w` sul record, nessuna scrittura sulla lista, mai persone: solo record tipo 2, `bt_pick` con finestra Z 110-186, un'auto per passaggio e cooldown 90 frame. **Trigger corretto in questa build**: la v86 armava solo con `VAR238 != 0`, cioè **durante la fuga** dell'UFO (46 frame su 16000); ora arma con `VAR238 == 0` (volo normale, mai in fuga) e il `clr.w` del flag di passaggio è stato tolto da `ap_dr` (azzerava a ogni frame). Test `lr/botta100.py` (100 colpi per mode) **PASS**: `0xFFF6B8` mai acceso per colpa di BOTTA, nessun record non tipo 2 toccato, X (+10) identica al 100% per gli stessi oggetti, bersaglio **0 frame** dentro il bucket 240-255, `|dx| < 48` mai. Gate 5 seed × 3 modalità, leak per 1000 frame di show: left **18,3 ± 5,6** · right **16,2 ± 5,4** · wild **24,4 ± 7,0**, tutte entro soglia 48/31/36 (v85c 30,5/19,9/16,3 · v86_ufo1 33,2/19,0/18,0); `wrong` 0, `vsbad` 0, `repeat` 0, **`desync` 0 su 15 run**, `ufochk` identico alla v86_ufo1 (sparizioni 0, passaggi 3, fuga breve 1), `short` 4/4/6 (v85c 4/2/4), show 180/175/174. `check_vars.py` exit 0, liberi 224-225 e 252-255. **In gara BOTTA non parte praticamente** (0 colpi in 16000 frame × 3 modalità): l'UFO si arma solo a strada libera e nei suoi ~545 frame di volo ci sono **0** auto tipo 2 collidibili → **task 6 UFO_RAPITORE sospeso**, le auto colpite le farà il razzo (task 3) con lo stesso codice |

### Note aperte (v85)
- **Escalation A/B/C/D, P5, P6, P7, S1-S5: non implementati.** Motivo comune: ogni istruzione aggiunta nel percorso hook sposta `leak` di ±50-100 frame (misurato più volte), quindi vanno fatti uno alla volta con backup e rollback. I dati delle frasi sono già pronti e verificati.
- **F1 (schermate finali frame per frame) non fatto**: manca un modo per arrivare al finale in emulatore (servirebbe forzare lo stato in RAM o un save state). Fatto solo il controllo statico del render (`grafica/finale_schermata.png`): nessun glitch visibile. La bozza con siringhe e sacchetti è in `grafica/finale_bozza.png`, non ancora applicata.
### Storico (v78) — problemi chiusi o superati
- **Desync (v78b): RISOLTO, era un bug del test, non della ROM**. `desync.py` leggeva la frase a `0xFF8F0C` (VAR12) invece che a `0xFF8F12` (VAR18): il valore 128-130 apparteneva a un'altra variabile e sembrava una frase di Gesù. Con l'offset giusto la v77 dà **0 disallineamenti su 15 run** (5 seed × 3 modalità), e i gate concordano. Le tre patch provate prima ((a) dispatch su VAR24, (b) retry di `pend` che allinea VAR24, (c) guardia VAR52 su `pe_nb`) erano basate su quella lettura falsa: **tutte ritirate**, rollback verificato con rebuild = SHA v77
- **`out.gen` = build corrente (vedi registro)**: il file non è una versione fissa, è l'ultima build generata. Se le misure divergono dai numeri qui, controllare questo file e usare la ROM giusta: le versioni consegnate stanno nella cartella progetto, i gate in `lr/gate/`.
- **Mappa memoria dell'harness per i VAR**: ora è una **regola valida** (vedi "Regole valide per OGNI task", punto 9): `wb` su indirizzo pari finisce sul byte dispari, per i VAR si usa `ww`/`rw`.
- **F3 (audio) non fatto**: `lremu.py` raccoglie i campioni ma manca lo strumento per misurare peak/RMS per brano.

### Note aperte (v87)
- **task 6 UFO_RAPITORE: SOSPESO** (decisione Andrea 15/10). L'UFO si arma solo a **strada libera** (sprite e VRAM liberi, regole anti-glitch): misurato in 16000 frame, il suo volo normale dura 210-545 frame e in **tutti** il numero di auto tipo 2 collidibili era **0** (con `bt_have` che chiede Z 110-186 e anche con Z 60-200 non cambia: non ci sono auto, punto). Quindi l'UFO **non ha auto da colpire** e l'UFO_RAPITORE com'è pensato perderebbe senso. Sul Pi della v87 si vedrà la **fuga dell'UFO** (task 1), non auto colpite.
- **Le auto colpite arriveranno dal razzo (task 3)**: il codice BOTTA è già pronto e serve a quello (finestra Z 110-186, testacoda di 24 frame verso il margine opposto ≥ 64 dal giocatore, poi `Z = 256` e il gioco libera il record, +1000 come un sorpasso). Il razzo si spara quando c'è traffico davanti.
- **Il trigger era invertito** e l'ho corretto nella v87: prima armava solo con `VAR238 != 0`, cioè **durante la fuga** dell'UFO (46 frame in 16000); ora arma con `VAR238 == 0` (volo normale, mai in fuga). Tolto anche il `clr.w %a0@(250)` che stava in `ap_dr`: azzerava a ogni frame e annullava il limite "1 auto per passaggio" (lo resettano `ap_nf` e `ap_ab`).
