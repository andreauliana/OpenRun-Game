# STATI PROPOSTI — out/capo/ (Turno 1, ora 0 di 12)

Fonte: TAPPE.md (tappa attiva: 1), ANDREA_REVIEW.md (Gate T1 / blocco T1-RUN, T1-Q1..T1-Q5), RICHIESTA_APERTA.md, CONTROLLO.md, VISIONE.md, AGENTS.md.
Regole 4.6: scrittura solo nei percorsi assegnati; review indipendente (autore != revisore); prove = comando, exit code, misure; percorsi relativi, mai cd. RICHIESTA.md e SCAMBIO.md non si modificano (sola lettura).

## Stato tappa attiva — Tappa 1, Gate T1 (blocco T1-RUN) — [~]

Tappa: "Il gioco parte, si visualizza il player che si muove, si pilota".
Gate: strada che scorre, auto accelera (A), frena (B), sterza sx/dx. Debug: build pulita, nessun crash in 5 min.

Esito ANDREA_REVIEW T1-Q3 (Andrea su Mac, 19/10, exit 13): gate headless OK (soak 30000 frame, replay 2x IDENTICO, ms/frame medio ~0.19 max 0.94, screenshot P1 OK), ma GIRO GUIDATO FALLITO su 5/6 livelli ("percorso poco vario o non guidabile"); solo lv_02_atomico OK (rettilineo 40%, curva max 0.86).
Next: @developer rivede generazione/parametri percorsi (blocco R1, priorita' assoluta).

T1-Q1 DEFAULT: sì — chiudi T1-RUN a [x] se `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0, replay 2x IDENTICO, ms/frame <= 20. (Risposta Andrea: sì — in attesa di gate verde.)

## Deleghe proposte (ruoli 4.6; consegne in out/<ruolo>/)

### 1. @developer — responsabile R1 punti 1-2 e T1-RUN punti 2-3
Percorsi scrivibili: motore_nuovo/src/road/road.cpp, motore_nuovo/src/game/*.{cpp,h}, motore_nuovo/dati/livelli/*/livello.json, nuovi motore_nuovo/dati/tracciati/*.json

Task:
- road.cpp: raccordo curva con campi entrata/uscita (default min(n/3, 60)), poi curva costante; correzione della deriva solo su segmenti con curva diversa da 0, mai sui rettilinei.
- dati/tracciati/tratti.json: libreria tratti (rettilineo_corto, rettilineo_lungo, curva_dolce_dx/sx, curva_stretta_dx/sx, esse, tornante, collina_su/giu) con curva, collina, lunghezza, entrata, uscita; livello.json campo tracciato (se assente, si usano i segmenti come oggi).
- T1-RUN 2: Road::project quota assoluta per segmento; camera = quota sotto l'auto + CAM_HEIGHT; ascissa x relativa al punto sotto l'auto.
- T1-RUN 3: drawSprites con zRel = worldZ % totalLen (niente sparizioni al wrap).
- T1-RUN 4b: ridurre densita' cartelloni in livello.json (oggi 1 ogni 1-4 segmenti).

Revisore: @developer-junior (peer review incrociata; l'autore non rivede se stesso).
Criteri di completamento: `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0 su tutte e 6 le piste; replay 2x IDENTICO; ms/frame <= 20; ctest 17/17; giro guidato passa su tutti i 6 livelli (rettilineo ~>=35-40%, curva max coerente 0.6-1.0, riferimento lv_02_atomico).

### 2. @developer-junior — responsabile R1 punti 3-5 e T1-RUN punti 5-7
Percorsi scrivibili: motore_nuovo/test/prova_gate.cpp, motore_nuovo/dati/tracciati/regole.json, motore_nuovo/dati/livelli/*/livello.json

Task:
- dati/tracciati/regole.json: regole per pista (quota rettilinei min/max, rettilineo piu' lungo, curva massima, quota fuori strada massima), lette da prova_gate al posto delle soglie hardcoded.
- prova_gate stampa, per ogni pista, quale TRATTO viola quale regola (nome tratto + segmento iniziale); test che carica ogni tracciato e verifica espansione == somma lunghezze.
- riscrivi i 6 livello.json con tracciato (lv_02_atomico resta riferimento perché passa); rapporto test per piste ancora poco guidabili.
- T1-RUN 5: casi prova_gate per wrap di playerZ oltre totalLen e inattivita' 10 s -> show/fumetto visibile in screenshot.

Revisore: @developer.
Criteri di completamento: gate percorso verde su tutte e 6 le piste; ctest 17/17; review incrociata registrata (autore non revisore del proprio lavoro).

### 3. @designer — responsabile G1 (catalogo grafico)
Percorsi scrivibili: assets_originali/CATALOGO_GRAFICO.json, assets_originali/INDICE_ASSET.json/.md, MAPPA_LIVELLI_SPONSOR.json

Task:
- Catalogo unico: un elemento per risorsa (id, tipo, stato mancante/bozza/in_costruzione/pronto/approvato/scartato, file, raw, livelli).
- Cataloga i 100 PNG fuori elenco (varianti B/C, scenari doppi scenario_<n>_01_* e scenario_lv_*) come scartato o nuovo elemento, senza cancellare file.
- Ogni sprite nuovo entra nel catalogo con il suo stato prima di essere usato; motore e test usano solo pronto e approvato.
- Verifica risorse temi lv_01_neon..lv_06_desktop, cartelloni sponsor ("Curve Pericolose" ~20% segmenti, "Brinis Udine" 10-15%), HUD variante B con elemento "tempo" residuo in alto a sinistra sotto velocità (D8-A).

Revisore: @capo (completezza catalogo, provenienza/licenze; non è peer review tecnica né approvazione).
Criteri di completamento: `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0"; manifest resta bozza fino ad approvazione PNG.

## Stati blocchi RICHIESTA_APERTA proposti (RICHIESTA.md/SCAMBIO.md non toccati)

T1-RUN [~] · R1 [~] da avviare (priorita') · R2 [-] (dopo R1) · G1 [ ] (attivato per T1-Q4) · H [~] (chiude con T1-RUN punto 8: v91 Andrea su Mac + fps/temperatura Pi 400) · P1 [~] (punto 4 sprite veri ancora aperto) · J [ ] · MD0 [-] · T5 [-].

## Domande ad Andrea (sempre con DEFAULT, procedo senza aspettare)

- T1-Q3 v91 runner Mac (SDL2): DEFAULT = sì, Andrea ri-verifica il runner su Mac dopo gate verde R1 e peer review, prima di Pi 400/RetroPie (ordine test ANDREA_REVIEW T1-Q5; T1-Q12 risposta A registrata: sì).
- Fino a ok Mac su R1+R2: nessun lavoro dedicato a Pi 400/RetroPie oltre a non rompere la build Linux (libretro.h manca, BUILD_LIBRETRO=ON non compila oggi).

## Chiusura turno

Non tutti i blocchi della tappa sono [x] (T1-RUN [~], R1 [~], G1 [ ], H [~], P1 [~]) -> non creo FINITO.
Non sono bloccato -> non scrivo PER_CLAUDE.md.
Prossimo: apertura dossier CP-005 per R1 in Checkpoint Revision/ (template, registro append-only, STATO_CORRENTE) dopo assegnazione formale.
