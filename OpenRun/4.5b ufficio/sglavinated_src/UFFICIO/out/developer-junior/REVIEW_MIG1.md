# REVIEW_MIG1 — @developer-junior (J1, riscritto 09/11 09:34)
Stato letto da: `UFFICIO/out/developer/ULTIMO_TURNO.md`, `UFFICIO/bacheche/developer/RIASSUNTO.md`, `RICHIESTA_APERTA.md` MIG1, grep/read su `motore_nuovo/src/game/*` (solo lettura). Gate non eseguito in questo turno.

## Cosa va
- MIG1 D2 [x] dichiarato dal @developer (09/11 09:22): fianco-a-fianco con colpo sterzo rapido (`sterzoPrec_`, soglia `dSterzo>0.22`, uso `spritesFonte_`/dx coerenti) chiama urto laterale; `Rivale::sbandataS=5s` in `urto(..., false)` e recupero graduale in `aggiornaRivali` (`passo *= 1 - 0.12*k`, `k=sbandataS/5`), memoria danni/KO invariata. Test riportati: `run_tests.sh` exit 0 (27/27), `gate_t1.sh` exit 0, 6/6 replay IDENTICI.
- Base legacy/vocabolario invariati a livello di dichiarazione (HUD, percorsi, audio esistente, screenshot P1 non toccato).

## Cosa non va / da verificare
- MIG1 D1 [ ] non completato: `abbattiPali` imposta detriti con coordinate mondo (`z`, `x0`, `x1`, MIG1 D1 commento), ma il comportamento visivo richiesto (atterraggio sull'erba, mai sotto riga ruote, scorrimento col mondo) va provato con screenshot/frame (J2).
- MIG1 D3, D4, G1 [ ] pendenti. `NOMI_RIVALI` ancora placeholder tematici generici (`KARVEN`...) — da allineare a D4 (7 nomi, max 6, culture/temi livelli).
- Rilievo pre-esistente noto: `gate_tratti`/espansione=NO lv_02/lv_05 e oggetti in galleria (PERC1) restano fuori perimetro MIG1.

## Miglioria piccola
- Per J2/J3: aggiungere in `prova_gate` un modo `pali`/`sportellata` che stampi metriche misurabili (es. y schermo detrito vs riga ruote dopo N frame; tempo fino a primo urto laterale riuscito in bot affiancato a punta) così la verifica MIG1 diventa ripetibile e non solo screenshot manuali. (Richiede autorizzazione/percorso `motore_nuovo/test/*.cpp` — già nel mio perimetro.)