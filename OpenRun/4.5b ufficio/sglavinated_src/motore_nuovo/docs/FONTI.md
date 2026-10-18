# FONTI — motore nuovo (pista C): cosa si prende e cosa no

# SPDX-License-Identifier: MIT

Autore: @architetto (reparto tecnico)
Data: 15 ott 2026
Stato: consegnato (BLOCCO B0 di RICHIESTA.md)
Percorso: motore_nuovo/docs/FONTI.md
Vista sul progetto: motore nuovo libero "Outrun Sglavinated Edition v3 (open)", pista C decisa da Andrea il 2026-10-15.

---

## 1. Scopo e metodo

Questo documento e' la consegna del blocco B0 di RICHIESTA.md. Fissa, per il motore nuovo (pista C), cosa si prende da ogni fonte elencata in PIANO_V3.md (BLOCCO 12, decisione "il meglio da piu' fonti") e cosa NON si prende, con la licenza di ciascuna fonte.

Regole applicabili (PIANO_V3 Decisione, BLOCCO 4, BLOCCO 8, VISIONE.md):
- Si prendono solo IDEE e TECNICHE (matematica della strada a segmenti, struttura del ciclo di gioco, numeri di gameplay come riferimento d'analisi, organizzazione a moduli).
- Non si prende mai: codice, asset, sprite, tracciati, dati ROM o musica di terzi.
- La pista C e' un motore nuovo scritto da zero in C++17, indipendente dalla piattaforma (render software 320x224, 50 Hz, audio int16 stereo, input tipo RetroPad, tempo a contatore di frame, RNG con seed).

Distinzione fatti / ipotesi:
- FATTI VERIFICATI: tratti dai file letti in questa sessione (PIANO_V3.md, RAPPORTO_FATTIBILITA.md, censimento_motori_outrun.md, ANDREA_REVIEW.md, VISIONE.md, ARCHIVIO_V87.md, FEDELTA.md, MODULI_E_REPARTI.md, CATALOGO_PERSONAGGI.md) e dalle pagine citate laddove esplicitamente indicato come verificato.
- IPOTESI: tutto cio' che non e' stato letto o verificato direttamente in questa sessione (es. altri motori pseudo-3D non elencati qui) e' marcato come "ipotesi da verificare" e non e' usato per decisioni.

---

## 2. Nota esplicita — decisione pista C (Andrea, 2026-10-15)

La pista C (motore nuovo libero, migliori idee da piu' fonti, mai codice o asset da terzi) e' DECISA da Andrea il 2026-10-15. Il confronto con la pista A2 resta un'opzione CHIUSA: non va riaperta senza una nuova decisione di Andrea.

**go/no-go per A2 (mod di Cannonball con ROM dell'utente, solo non commerciale): NO-GO.**

Motivi (PIANO_V3 BLOCCO 4 e 8, RAPPORTO_FATTIBILITA par. 1b, verifica licenza docs/license.txt di Cannonball):
1. Licenza non commerciale: non e' software libero e blocca ogni uso commerciale futuro.
2. Richiede la romset arcade OutRun Rev B dell'utente (materiale Sega): senza quella ROM il motore non ha grafica, audio né dati.
3. Il codice e' dichiarato come riscrittura in C++ del codice 68000/Z80 originale Sega: se questo lo renda un'opera derivata e' una domanda aperta per un legale (non verificata da me).
4. Distribuzione consentita solo come codice/patch, mai la ROM, e solo in ambito non commerciale.

A2 puo' rimanere come laboratorio di gameplay veloce, ma i moduli di gameplay (dati e regole) prodotti da @traduttore sono validi per entrambi i motori e vanno portati nel motore C indipendentemente (RAPPORTO_FATTIBILITA par. 1b, nota 25).

---

## 3. Fonti (PIANO_V3 BLOCCO 12)

Tabella riassuntiva. Per ogni fonte: cosa si prende come idea/tecnica, cosa NON si prende, licenza e stato di verifica.

| # | Fonte | Cosa si prende (idea / tecnica) | Cosa NON si prende | Licenza | Verifica |
|---|-------|----------------------------------|--------------------|---------|----------|
| 1 | Lou's Pseudo 3d Page | Matematica della strada a segmenti: curve, colline, sprite scalati, prospettiva pseudo-3D | Codice, testi o pagine copiate | Non dichiarata (pagina di riferimento tecnico, nessuna licenza esposta) | Fonte citata in PIANO_V3; contenuto/licenza non controllati direttamente -> non dichiarata |
| 2 | JavaScript Racer (J. Gordon) | Struttura del ciclo di gioco, segmenti di strada, collisioni, traffico semplice | Codice (licenza non dichiarata) | Non dichiarata | Fonte citata in PIANO_V3; licenza non dichiarata |
| 3 | Cannonball / OutRun originale | Solo i *numeri* di gameplay come riferimento d'analisi: velocita', sensazione di guida, 50 Hz, ritmo delle tappe | Codice, dati ROM, sprite, musica di OutRun | Cannonball: licenza custom tipo BSD con divieto d'uso commerciale (verificata, docs/license.txt); OutRun originale: proprietario SEGA, nessun diritto di riuso | Verificata per Cannonball (licenza letta); Sega: proprietario per definizione |
| 4 | ZgzInfinity/OutRun | Organizzazione di un clone in C++: moduli, menu, struttura delle tappe | Asset del remake; codice riusabile solo se il progetto accetta la GPL-3.0 (decisione a parte, voce D1) | GPL-3.0 (verificata: dichiarata nella pagina GitHub del progetto) | Verificata dal documento di ricerca; da ricontrollare in pagina prima di ogni uso del codice |
| 5 | La hack Sglavinated (v87) | Personaggi (Gianni, Gesu', UFO, donna), fumetti, ritmo degli «show» (escalation, inattivita', priorita' crash/uscita) — opere di Andrea, si portano su | I limiti Mega Drive (156 frasi, ROM 4 MB, 64 KB RAM, budget VBlank) | Non dichiarata; opera derivata dalla ROM Sega -> uso privato, verifica legale per ogni distribuzione (PIANO_V3 BLOCCO 8) | Stato noto dai file locali (ARCHIVIO_V87.md, SPEC_personaggi_v74.md); licenza non dichiarata |

Note per fonte (dettaglio):
- Lou's Pseudo 3d Page: riferimento tecnico sulla tecnica pseudo-3D (strada a segmenti, curve, colline, sprite scalati). Non e' codice riutilizzabile: se ne prendono solo le idee matematiche, mai testi o codice copiati.
- JavaScript Racer (J. Gordon): riferimento sulla struttura del ciclo di gioco (input -> aggiornamento segmenti -> collisioni traffico -> render). Licenza non dichiarata: mai copia di codice.
- Cannonball / OutRun originale: usata solo come riferimento d'analisi dei *numeri* (velocita', feeling di guida, 50 Hz, ritmo tappe). Non e' adottabile come base di una versione libera (vedi BLOCCO 8 e censura motori sotto).
- ZgzInfinity/OutRun (GPL-3.0): riferimento sull'organizzazione a moduli di un clone C++ (menu, tappe). Il codice si puo' riusare solo se Andrea sceglie la GPL-3.0 per tutto il progetto (voce D1 ancora aperta; DEFAULT raccomandato MIT/CC-BY). Gli asset vanno comunque sostituiti tutti.
- La hack Sglavinated: personaggi e fumetti sono di Andrea e si portano nel motore nuovo (dati, non codice Sega). Non si riportano i vincoli hardware della hack (tetto 4 MB, 156 fumetti, 64 KB RAM).

---

## 4. Censimento motori (PIANO_V3 BLOCCO 9)

Censimento dei motori candidati, anche pseudo-3D con licenza libera. Per ciascuno: licenza, richiede asset originali (ROM/sprite/tracciati Sega)?, gira su Raspberry Pi 4 / Pi 400 a 50 fps?, cosa si puo' riusare, cosa va sostituito per una versione libera senza materiale Sega.

| Motore / fonte | Licenza | Richiede asset originali? | Gira su Pi 4 / Pi 400 a 50 fps? | Cosa si puo' riusare | Cosa va sostituito | Giudizio per V3 |
|----------------|---------|---------------------------|----------------------------------|----------------------|--------------------|-----------------|
| Cannonball (djyt/cannonball) | Custom tipo BSD, **divieto d'uso commerciale** (verificata, docs/license.txt); pacchetti Linux la classificano "MAME"/nonfree | **Sì**: tutto (grafica, audio, dati provengono dalla romset arcade OutRun Rev B) | Sì — build riuscita su Raspberry Pi (documentata dal progetto; recalbox wiki conferma Pi 4/400), mira a fps superiori all'originale | Nulla di riutilizzabile in una versione libera; solo i numeri di gameplay come riferimento d'analisi | Tutti gli asset e la ROM stessa: non adottabile come base libera | **NO** — solo riferimento tecnico (PIANO_V3 BLOCCO 8) |
| ZgzInfinity/OutRun | **GPL-3.0** (verificata, pagina GitHub) | **Sì**: tutti i grafici, l'audio e le mappe del remake vanno sostituiti con asset originali nostri | **Da verificare**: C++14 + SFML 2.5.1 + OpenGL; eseguibili Windows + Linux; nessun test su Pi 4/400 in questa sessione | Organizzazione del codice in C++ (moduli, menu, tappe), solo se si accetta la GPL-3.0 per l'intero progetto (voce D1) | Tutti gli asset; se usato come base, l'intera opera diventa GPL-3.0 | **Condizionata**: possibile solo se Andrea accetta la GPL-3.0 e sostituisce tutti gli asset |
| Lou's Pseudo 3d Page | Non dichiarata | N/A (non e' codice eseguibile) | N/A | Idee matematiche (strada a segmenti, curve, colline, sprite scalati) | N/A | Solo riferimento, mai copia |
| JavaScript Racer (J. Gordon) | Non dichiarata | N/A (non e' codice eseguibile) | N/A | Idee (ciclo di gioco, segmenti, collisioni, traffico semplice) | N/A | Solo riferimento, mai copia |
| Genesis Plus GX / lr-genesis-plus-gx (core libretro) | Licenza di emulatore (core libretro) | N/A | Sì — esegue la hack v87 su Pi 400 (PAL) | Nulla (non e' un motore nuovo per il v3) | N/A | Non adottabile: esegue solo l'hack derivata da ROM Sega (uso privato) |
| Altri motori pseudo-3D open source (es. topic "pseudo3d" su repos.ecosyste.ms) | **Ipotesi**: da verificare | **Ipotesi**: da verificare | **Ipotesi**: da verificare | **Ipotesi**: da verificare | **Ipotesi**: da verificare | Non analizzati in questa sessione; da valutare solo se in futuro serve una scorciatoia |

Conclusioni del censimento (per il reparto tecnico e Andrea):
1. Nessun motore censito e' adottabile "come e'" per una versione libera con asset originali.
2. Cannonball e' vincolata dalla ROM originale Sega e da una licenza non commerciale: esclusa come base (solo riferimento tecnico).
3. ZgzInfinity/OutRun e' GPL-3.0 (open source ma vincolante) e richiede comunque la sostituzione totale degli asset: percorribile solo con decisione licenza (voce D1) e test su Pi 400.
4. Confermata la **pista C**: motore nuovo scritto da zero in C++17, prendendo idee e tecniche dalle fonti sopra, mai codice o asset.

---

## 5. Riferimenti consultati

| Fonte | Tipo | Stato |
|-------|------|-------|
| PIANO_V3.md (blocco 4, 8, 9, 11, Decisione) | File locale del progetto | Letto, verificato |
| RAPPORTO_FATTIBILITA.md | File locale del progetto | Letto, verificato |
| censimento_motori_outrun.md (assets_originali/ricerca/) | File locale del progetto | Letto, verificato |
| ANDREA_REVIEW.md (voci D1, D2) | Fonte canonica locale per feedback e decisioni di Andrea | Letto, verificato |
| VISIONE.md, ARCHIVIO_V87.md, FEDELTA.md, MODULI_E_REPARTI.md, CATALOGO_PERSONAGGI.md | File locali del progetto | Letti, verificati |
| https://github.com/djyt/cannonball (+ docs/license.txt) | Web | Pagina e licenza lette e citate nei documenti del progetto |
| https://wiki-next.recalbox.com/en/emulators/ports/out-run | Web | Citato da PIANO_V3 BLOCCO 11 (ROM richieste, Pi 4/400); non ricontrollato direttamente in questa sessione -> da ritenere come riportato |
| https://github.com/ZgzInfinity/OutRun | Web | GPL-3.0 dichiarata (riportata come verificata nel documento di ricerca); da ricontrollare in pagina prima di qualsiasi uso del codice |
| Lou's Pseudo 3d Page | Web | Riferimento tecnico; licenza non dichiarata |
| JavaScript Racer (J. Gordon) | Web | Riferimento tecnico; licenza non dichiarata |
| La hack Sglavinated v87 (sglavinated_src/) | File locale | Sola lettura; opere di Andrea derivate da ROM Sega, licenza non dichiarata |

---

## 6. Licenza di questo documento

Questo file (motore_nuovo/docs/FONTI.md) e' rilasciato sotto **MIT** (SPDX-License-Identifier: MIT), scelta DEFAULT della raccomandazione D1 di ANDREA_REVIEW.md (per i docs si dichiara MIT o CC-BY; qui si sceglie MIT per coerenza con il codice del progetto). La licenza del documento non estende alcuna licenza delle fonti citate, che restano di proprieta' dei rispettivi autori.

---

Fatto da: @architetto, turno 1, blocco B0. Da consegnare a @capo per aggiornamento SCAMBIO.md (voce CHIUSA) e, se necessario, a @reviewer per controllo licenze.
