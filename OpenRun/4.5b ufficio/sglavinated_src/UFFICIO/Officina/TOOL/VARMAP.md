# Mappa della zona VAR (0xFF8F00-**0xFF8FFF**)

La zona finisce a `0xFF8FFF`: **oltre non è RAM nostra, è del gioco.** `check_vars.py`
fallisce su qualunque accesso con base VAR e offset ≥ 256 (offset 300 → 0xFF902C,
respinto).

Zona dei variabili di stato usate dall'hook. Il base register è `%a0`, impostato da
`lea VAR,%a0` all'inizio di `hk_after`. **L'offset nel nome del VAR è già l'offset in
byte dalla base**: `VAR8` = `%a0@(8)` = 0xFF8F08, la frase è `VAR18` = `%a0@(18)` =
0xFF8F12. Le tabelle in assembly (`ph_tab`, `jtab`, `otab`, `ctab`, `itab`, `xtab`,
`g1t`/`g2t`/`g3t`, `j1t`/`j2t`/`j3t`) NON stanno qui: sono etichette a parte, con
`lea <etichetta>,%aN`.

`tools/check_vars.py` fa la scansione di `tools/h40z.s` e **fallisce la build** se:
1. un uso di VAR cade dentro una zona riservata;
2. un uso di VAR (byte singolo, `.w` = 2 byte, `.l` = 4 byte) esce dalla zona documentata;
3. un offset usato nel codice non è coperto da una riga di questa tabella;
4. due righe documentate si sovrappongono.

Regola d'uso: **prima di usare un offset nuovo, scriverlo qui**, poi passare
`python3 tools/check_vars.py`.

## Occupato

| offset | byte | nome / uso | stato |
|---|---|---|---|
| 0-179 | 180 | codice originale del gioco e dell'hook (nomi in `SPEC_personaggi_v74.md`) | occupato |
| 184 | 2 | contatore di rampa dell'escalation (v79) | occupato |
| 186 | 2 | crash accumulati nello stage (v79) | occupato |
| 188 | 2 | istante dell'ultimo errore (v79) | occupato |
| 190 | 2 | livello di escalation 0-3 (v79) | occupato |
| 180 | 2 | flag richiamo inattività (v79) | occupato |
| 182 | 2 | contatore frame senza input (v79) | occupato |

| 234 | 2 | indice del giro di scadenza dei DANGER (0,4,...124) | occupato |
| 236 | 2 | v85c SBORDATA: evento uscita di strada in attesa (frame rimasti, parte da 250) | occupato |
| 238 | 2 | v85c UFO_FUGA: frame di fuga dell'UFO (0 .. VAR242) | occupato |
| 242 | 2 | v86 UFO_FUGA: durata della fuga in frame (24 da show, 12 da `ap_ab`) | occupato |
| 244 | 2 | v86 UFO_FUGA: 1 = la fuga in corso si chiude con l'abort di `ap_ab` | occupato |
| 226 | 2 | v87 BOTTA: richiesta `BBREQ` (1 testacoda, 2 sparisce subito, 3 testacoda e sparisce); 0 = inattiva. Nella ROM consegnata è armata solo da `ap_dr` (l'UFO) | occupato |
| 228 | 2 | v87 BOTTA: `BBACT` frame rimanenti dello scivolamento (24 → 0) | occupato |
| 230 | 2 | v87 BOTTA: `BBIDX` indice dell'auto bersaglio + 1 (0 = nessuna) | occupato |
| 232 | 2 | v87 BOTTA: `BBCLD` cooldown, frame di volo dell'UFO tra un colpo e l'altro (parte da 90) | occupato |
| 250 | 2 | v87 BOTTA: `BBPPS` 1 = un'auto è già stata colpita in questo passaggio (reset in `ap_nf` e a ogni nuovo passaggio); offset pari: 233 era dispari | occupato |
| 224 | 2 | v88 VOLO: `VOLB` base del volo in pixel (0 a terra, sale a 4 sopra `VOLOMIN`) | occupato |
| 252 | 2 | v88 VOLO: `VOLP` fase dell'onda (contatore proprio, non quello del gioco) | occupato |
| 254 | 2 | v88 VOLO: `VOLO` quanto viene alzata davvero la macchina questo frame (0 = la SAT non viene toccata) | occupato |
| 248 | 2 | v85c REATTIVO: 1 = show corrente nato da uscita di strada o crash | occupato |
| 246 | 2 | v85c BOLLA: fumetto nascosto perche' il gioco ne ha ripreso un blocco | occupato |
| 240 | 2 | v85c SBORDATA2: alterna 0/1: una uscita di strada su 2 conta come errore | occupato |
## Riservato (non VAR)

| offset | byte | uso | stato |
|---|---|---|---|
| 192-223 | 32 | **buffer di ricoloramento HUD**, `lea 0xFF8FC0,%a4`, 8 long letti/scritti | RISERVATO |

Fuori dalla zona, per completezza: mappa per-stage dei blocchi VRAM a `0xFF8D00`
(`lea 0xFF8D00,%a1`, 512 byte, 0xFF8D00-0xFF8EFF). Non è un VAR e non viene controllata
qui: `check_vars.py` la ignora perché il base register è impostato da un `lea` diverso
da `VAR`.

**Da cui la regola: niente offset 192-223.** Ci era finito `VAR196`, che non era libero:
scriveva dentro il buffer HUD. Il sintomo era fuorviante — leggendo indietro `VAR196` si
vedeva "1" per centinaia di frame in un run in cui il timer di inattività non era mai
salito, e sembrava un flag armato di continuo.

## Liberi

| offset | byte | stato |
|---|---|---|
| 224-225 | — | **non più liberi**: v88 VOLO ha preso 224 (`VOLB`) |
| 252-255 | — | **non più liberi**: v88 VOLO ha preso 252 (`VOLP`) e 254 (`VOLO`) |
| 7, 26-27, 32-33, 38-45, 50-51, 130-131, 134-135, 138-139, 146-149, 154-157 | — | liberi (li stampa `check_vars.py`; quasi tutti sono byte, nessuno è una word comoda) |
| 256+ | - | **fuori zona**: non è libero, è RAM del gioco. Il controllo lo rifiuta |

Nota: 191 non è libero, è il secondo byte della word all'offset 190 (livello di
escalation). Per questo il primo candidato utile è 224, non 192.

Nota sui word: gli offset pari sono allineati. Usare 225 significherebbe una word
disallineata (niente penalità sul 68000, ma rende il debug più fragile).

## Reset (fuori da h40z.s)

Lo stub di reset **esiste** e sta in `rst/rst.s`, fuori da `h40z.s`. `build7.py` lo
installa al vettore di reset e rimette `ROM[4:8] = 0x003B3F00`. Verificato sul ROM
consegnato: vettore `0x003B3F00`, `lea 0xFF5300,%a0` con 576 `clr.l` e `lea 0xFF8F00,%a0`
con 64 `clr.l`, poi `jmp 0x200`.

| da | byte | file | cosa copre |
|---|---|---|---|
| 0xFF5300 | 2304 | `rst/rst.s` | tabelle del fumetti, 0xFF5300-0xFF5BFF |
| 0xFF8F00 | 256 | `rst/rst.s` | **tutta la zona VAR, 0xFF8F00-0xFF8FFF** |

Quindi **le variabili nuove 180-190 e 224+ partono già pulite dopo un reset**: non serve
un secondo azzeramento nell'hook, e `check_vars.py` fallisce se lo stub smette di
coprire esattamente queste due zone (o se la mappa le documenta diversamente).

Attenzione: `rststep` in `h40z.s` **non** c'entra, è una routine di verifica con checksum.

Oltre allo stub, l'hook azzera i propri flag anche qui:

| punto | quando | azzera |
|---|---|---|
| `zstage` | cambio stage | 180, 182, 184, 186, 188, 190 |
| `id_nr` | ogni frame fuori gara: game over, reset, continue, titolo, demo, menu | 180, 182 |
| `ph_f2` | show rinunciato dopo i retry | 180 |
| `ss_nj` | lo show parte davvero (messaggio consumato) | 180 |

`id_nr` copre il game over perché al continue lo stage non cambia e `zstage` non basta;
riguarda solo il flag e il contatore dell'inattività, non l'escalation (di quella si
occupano `zstage` e lo stub).

## Come viene mantenuta

`python3 tools/check_vars.py` scandisce `tools/h40z.s`, risolve i base register
(`lea VAR,%aN` = zona VAR, qualunque altro `lea` = tabella separata, ignorata) e
ricava gli usi reali con la loro dimensione. Va eseguito **prima di `build7.py`** nella
pipeline di build.