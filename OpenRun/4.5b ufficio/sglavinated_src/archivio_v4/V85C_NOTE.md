# v85c → v87 (task 1: UFO_FUGA_BREVE · task 2: BOTTA)

## Quale ROM usare
| File (cartella progetto) | SHA1 | Stato |
|---|---|---|
| `OutRun_Sglavinated_v87.gen` | `3fddbeb6c82c30c8bb2c0c3633248d2b62689a0d` | **corrente** (v87 task 2 = BOTTA: il colpo alle auto, armato dall'UFO) |
| `OutRun_Sglavinated_v86_ufo1.gen` | `9fead172b225f54d90d676c0b67d135e2d3df63a` | ripristino del task 2 (`patch_z_history/patch_z.v86_ufo1.py`) |
| `OutRun_Sglavinated_v85c.gen` | `a4422f784d95c1f6b044c77911a66bc8337a6999` | ripristino del task 1 |
| `OutRun_Sglavinated_v85b.gen` | `10defeb56c63e8c578f1e5ab2e3a641853f19616` | ripristino |
| `OutRun_Sglavinated_v85.gen` | `c7276aa46f94594bac86450dd9feb7b37753ea99` | ripristino (OpenCode) |
| v79 | — | ritirata, non usare |

## Linea dei sorgenti
`v85 (OpenCode, patch_z.v85_pulito.py)`
→ `v85b` = + DANGER a scadenza (16) + SHDUR 260 (blocchi di OpenCode, senza diagnostica) — `patch_z.v85b_claude.py`
→ `v85c` = + 7 blocchi "v85c" in fondo a patch_z — `patch_z.v85c_claude.py`
→ **v86 task 1** = + 2 blocchi `UFO_FUGA_BREVE` (OpenCode, 15/10) — `patch_z_history/patch_z.v86_ufo1.py`
→ **v87 task 2 = BOTTA** = + colpo alle auto (testacoda + `Z = 256`), armato dall'UFO in `ap_dr` (volo normale, mai in fuga) — `patch_z_history/patch_z.v87.py`

Tutto il lavoro di OpenCode è dentro (pipeline, check_vars, VARMAP, escalation, inattività, itab,
tabelle frasi, DANGER a scadenza, durata fissa). Il `tools/patch_z.py` di OpenCode NON è stato
toccato fino al 15/10: era uno stato intermedio (v85_pulito + SHDUR, rebuild b244cf8c).
T1 (tempo lento) **non esiste nel codice**: VAR224 era solo prenotata in VARMAP, ora è di nuovo libera.

## Per riprendere dalla v87 (OpenCode o altri)
```
cp tools/patch_z_history/patch_z.v87.py tools/patch_z.py
sh tools/build_all.sh out.gen 81 6        # deve dare 3fddbeb6...
```
Ripristino a v86 (solo task 1, senza BOTTA): `cp tools/patch_z_history/patch_z.v86_ufo1.py tools/patch_z.py` → `9fead172`.
Ripristino del solo task 1 sulla v85c: `cp tools/patch_z.py.bak-v85c tools/patch_z.py` → `a4422f78`.
`sglavinated_src/out.gen` = v87 (build corrente). I test `lr/*.py ../out.gen` misurano quindi quella.

## Regola trovata il 15/10: NON toccare `ap_x`
`ap_x` è la destinazione di `cmpi.b #8,%d0; bne.w ap_x`, quindi viene eseguito **ogni frame anche in
attract mode**. Aggiungervi anche una sola istruzione sposta il budget di cicli del frame e con esso
la sequenza pseudocasual del gioco: misurato con `lr/divrun.py` (due run + hash della RAM per frame),
la prima versione della patch divergeva dalla v85c **dal frame 129**. Spostando il check della fuga
breve su `ap_fle` (subito dopo `bra.w in ap_dr`, mai raggiunto in attract) la divergenza del core
è partita **dal frame 8402**, cioè dal primo istante in cui la nuova logica gara effettivamente
gira: i seed 137/313/911 danno numeri **identici** fra v85c e v86.

## I blocchi v85c (dettagli e misure: SPEC §13)
| Nome | Problema segnalato da Andrea | Soluzione |
|---|---|---|
| ESC0/ESC2 | Gesù sempre, anche guidando pulito | bug 0 errori = livello 3; ora livello = errori/2, calo ogni 600 frame |
| SBORDATA | "sbando più volte e non esce nessun messaggio" | evento uscita di strada in attesa 250 frame; fa uscire uno show periodico già letto (≥100 frame); sostituisce la risposta di Gesù in attesa; mai su show reattivi (VAR248) |
| SBORDATA2 | più esci di strada, più interviene Gesù | 1 uscita di strada su 2 conta come errore |
| UFO_FUGA | "l'UFO scompare, non finisce l'animazione" | se parte uno show, l'UFO scappa in 24 frame invece di sparire |
| FINALE | "verso la premiazione sprite sbagliati" | stato 44: tabella sprite svuotata ogni frame sull'immagine finale |
| BOLLA | "qualcuno appare, azione agitata e scompare" | fumetto sovrascritto → si nasconde solo il fumetto, blocchi restituiti, uscita animata immediata |

## Il blocco v86 task 1 — UFO_FUGA_BREVE (dettagli: SPEC §14)
| Nome | Problema segnalato da Andrea | Soluzione |
|---|---|---|
| UFO_FUGA_BREVE | "quando l'UFO perde la grafica sparisce di colpo, senza finire l'animazione" | `ap_ab` arriva in due modi: da `ucheck` (tile **già** sovrascritti → abort come in v85c) oppure da `VAR74` (sat_scan: il gioco rivendica il blocco ma i nostri tile sono ancora lì → **grafica integra**). Nel secondo caso l'UFO fa 12 frame di fuga (`VAR242=12`, contatore `VAR238`) e poi l'abort differito (`ap_fle` → `ap_ab2`) |

Codice (2 blocchi `rep()` in fondo a `tools/patch_z.py`):
- `ap_ab`: se `VAR74≠0` si rilegge `ucheck` **in sola lettura** (la word 66, cioè byte 66+67 pari, viene salvata su stack e ripristinata: `VAR67` è un byte all'offset **dispari** e `check_vars.py` vieta gli accessi `.w` dispari). Grafica integra → fuga; altrimenti → `ap_ab2`, che è **byte a byte** l'abort di v85c.
- Durata della fuga letta da `VAR242` (24 da show, 12 da `ap_ab`) invece che 24 fisso; il `cmpi` diventa `move`+`cmp` perché su 68000 non esiste cmp memoria-memoria.
- `ap_fle` (dopo `bra.w ap_dr`, mai raggiunto in attract): `VAR244≠0` → abort differito, altrimenti cade su `ap_x` invariato.
- `VAR242` e `VAR244` documentati in `tools/VARMAP.md` (occupano l'ex zona libera 242-245); `check_vars.py` 179 byte VAR, exit 0.

Misure (`ufochk.py`, 16000 frame, uguali su left/right/wild):

| | v85c | v86 task 1 |
|---|---|---|
| passaggi UFO | 3 | 3 |
| uscita normale / fuga da show | 1 / 1 | 1 / 1 (frame **identici**: 2610 e 8425) |
| fuga breve 12 frame | — | **1** (frame 10798) |
| **sparizioni di colpo** | **1** (`t400`, frame 10787) | **0** |

## Problemi ancora aperti (in ordine di importanza)
1. ~~**UFO**: sparisce ancora di colpo quando il gioco gli riprende un blocco (`ucheck` → `ap_ab`).~~ **Risolto nel task 1 v86** (vedi sotto). Le sparizioni `ucheck`/`VAR74` sono **0** invece di 1 in `ufochk.py` su left/right/wild.
2. **Reazione all'uscita di strada ~64%**: il resto è bloccato da show già a schermo nei primi 40 frame o da caricamenti. Con il bot l'escalation va spesso al livello 3 (molti crash), quindi nei test reagisce quasi sempre Gesù.
3. **Finale**: correzione verificata solo forzando lo stato 44, non arrivandoci giocando (`lr/goal.py` arriva a mappa/classifica, non al finale).
4. ~~Gate a 5 seed fatti solo per left e right (`fseed.py`), non per wild.~~ **Fatto su tutte e 3 le modalità (OpenCode, 15/10)**: left 30,9 ± 13,5 · right 18,6 ± 7,1 · wild 22,4 ± 9,8, tutte entro soglia, `wrong` 0 e `vsbad` 0 su 15 run. Tabella in `SPEC_personaggi_v74.md` §13. Nota: `final.py` senza 4° argomento è deterministico (seed 0).
5. ~~car_hit (BOTTA)~~ **fatto nella v87**: l'auto colpita scivola di lato (testacoda di 24 frame, X verso il margine opposto, mai nella zona di collisione 240+) e poi sparisce con `Z = 256` (il gioco la libera al prossimo aggiornamento, +1000 come un sorpasso). Nessuna grafica nuova, nessuna scrittura fuori dal record dell'auto, mai persone.
6. **task 6 UFO_RAPITORE: SOSPESO**. L'UFO parte solo a strada libera (sprite e VRAM liberi, regole anti-glitch): in 16000 frame il suo volo normale dura ~210-545 frame e in **tutti** il numero di auto tipo 2 collidibili era **0**. Quindi l'UFO, da solo, non ha auto da colpire e l'UFO_RAPITORE com'è pensato non avrebbe senso. Le auto colpite arriveranno dal **razzo (task 3)**, che spara quando c'è traffico davanti e userà il codice BOTTA già pronto (stessa finestra Z 110-186, stesso testacoda, stessa sparizione).
7. Restano da fare: razzo di ganja (tasto X), tempo lento, macchina che si alza, schermate titolo/radio/mappa.

## Script nuovi in lr/
| Script | Misura |
|---|---|
| `offreact.py rom N mode seed` | % uscite di strada con reazione, chi reagisce e a che livello |
| `showend.py rom N mode` | show finiti di colpo e motivo (dall'esterno) |
| `ufochk.py rom N` | passaggi UFO: uscita normale / fuga / sparizione |
| `leakwho.py rom` | quali pezzi (TGR) sono coinvolti nei leak e se il fumetto era nascosto |
| `fseed.py rom N mode seed` | come final.py ma con seed (fase di partenza) e frame di show per normalizzare il leak |
| `goal.py rom N` | corsa lunga con screenshot di mappa/classifica in /tmp/goal |
| `ufoev.py rom N` | tutti gli eventi UFO (comparsa/sparizione, ramo preso, `pf`/`tg`/`VAR74`/`VAR67`) |
| `ufotrace2.py rom N` | traccia dei cambi di stato dell'UFO, un ROM alla volta |
| `divrun.py rec\|rep\|dump` | **trova il primo frame in cui due ROM divergono**: registra input+hash della RAM (due processi separati, il core libretro è un singleton) |
| `bytehist.py rom N in.npz off...` | storico di uno o più byte RAM con input registrati |

## Regole imparate oggi (valgono per chi continua)
- Modifiche solo con `rep()` ancorata a testo unico, in fondo a `patch_z.py` (prima della scrittura finale di `h40z.s.tmp`).
- Ogni VAR nuovo prima in `VARMAP.md`, poi `check_vars.py` (exit 0). Niente offset 192-223 (HUD), niente ≥256.
- Il controllo dimensioni di `check_vars.py` tratta `bchg/btst` come word: usare `eori.w`/`tst.w` su word pari.
- Tenere un personaggio più a lungo in una zona VRAM contesa fa salire il leak: meglio uscire subito con l'animazione.
- Misurare prima/dopo con lo stesso script e la stessa lunghezza di run.
