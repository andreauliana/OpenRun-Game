# REVIEW_MIG1 — @developer-junior (J4, riscritto 09/11 10:15)
Stato letto: `UFFICIO/out/developer/ULTIMO_TURNO.md`, `RICHIESTA_APERTA.md` MIG1, esecuzione prove locali 09/11 10:15. Gate_t1.sh non eseguito.

## Cosa va
- MIG1 D1 [x] (verifica codice/report): `abbattiPali` coord. mondo; `drawDetriti` proietta con `projs_` e clampa `baseY <= 208.0f` (mai sotto riga ruote/area visibile).
- MIG1 D2 [x]: colpo sterzo rapido affiancato (`dSterzo>0.22`, `AFFIANCATA_DX`), `Rivale::sbandataS=5s`, recupero graduale, memoria danni/KO invariata.
- MIG1 D3 [x]: `CarattereRivale` seed fisso deterministico (aggressivo/prudente/vendicativo/sbaglia-curva), guardie anti-tamponamento.
- MIG1 J3 [~] prova eseguita: `sportellata` exit 0 (09/11 10:15) — GATE SPORTELLATA OK, colpo_a_segno=SI a 0.52s (frame 31), urti_gara=8, ripresa_proxy Z 0-2s=30168 2-4s=10325 4-7s=-40493 (proxy utile ma `sbandataS` privato: recupero 5s non misurato direttamente).
- MIG1 J1 [x]. J4 [~] aggiornato.

## Cosa non va / da verificare
- **MIG1 J2 [~] FALLITO**: `pali` exit 27 (09/11 10:15), hit_rilevati=0, frame_primo_hit=-1, screenshot=0, dir `UFFICIO/Officina/Test area/mig1_j2_pali/mig1_j2_pali`. La prova non ha verificato paletti abbattuti dentro schermo/sopra riga ruote (mancano hit e screenshot). Azione: verificare setup modo `pali` in `motore_nuovo/test/prova_gate.cpp` (posizionamento/guida su paletti, condizione screenshot) vs stato runtime D1/presenza paletti in lv_01_neon.
- J3 limite: verifica sbandata ~5s piena richiede strumentazione/API dedicata; proxy Z mostra inversione 4-7s ma non è prova diretta di `sbandataS`.
- D4 [ ] nomi rivali da confermare. D5 facoltativo. G2 dopo D1-D3. Nota: rotazione estrema detrito può eccedere `baseY` di ~1px (da verificare con screenshot J2 quando la prova produrrà immagini).

## Miglioria piccola
- Affinare modo `pali` (targeting paletti, log diagnostico frame/hit, salvataggio screenshot anche su hit mancato se utile) per ottenere evidenza J2; poi rieseguire solo `pali`.