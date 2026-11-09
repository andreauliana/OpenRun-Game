# REVIEW_MIG1 — @developer-junior (J4, riscritto 09/11 10:02)
Stato letto da: `UFFICIO/out/developer/ULTIMO_TURNO.md` (dev 09/11 09:39 D3), `RICHIESTA_APERTA.md` MIG1, grep/read `motore_nuovo/src/game/game.cpp`/`.h` (sola lettura). Gate non eseguito (shell permission.rejected).

## Cosa va
- MIG1 D1 [x] (verifica codice): `abbattiPali` coord. mondo (`z`,`x0`,`x1` erba); `drawDetriti` proietta con `projs_` e clampa `baseY <= 208.0f` (riga ruote, `renderer.cpp`) => mai sotto riga ruote/fuori area visibile.
- MIG1 D2 [x] (report dev 09:22 + grep): colpo sterzo rapido affiancato (`dSterzo>0.22`, `AFFIANCATA_DX`), urto laterale; `Rivale::sbandataS=5s` in `urto(...,false)`, recupero graduale `passo *= 1-0.12*k` (`k=sbandataS/5`), memoria danni/KO invariata.
- MIG1 D3 [x] (report dev 09:39 + grep): `CarattereRivale` + shuffle seed `^0x43415241u` (aggressivo/prudente/vendicativo 8s/sbaglia-curva + normali), guardia anti-tamponamento, no teletrasporti/sovrapposizioni dichiarati.
- MIG1 J1 [x]. J2 [~] codice: modo `pali` in `prova_gate.cpp`. J3 [~] codice: modo `sportellata` aggiunto (misura t colpo via `urtiGara()`, proxy ripresa Z 0-2/2-4/4-7s, screenshot `mig1_j3_sportellata/`). J4 [~] aggiornato.

## Cosa non va / da verificare
- **J2/J3 esecuzione bloccata**: shell permission.rejected su build. Cmd esatti negati: `cmake --build "UFFICIO/Officina/build" --target prova_gate` (anche 09:55). Binario `UFFICIO/Officina/build/prova_gate` è 09:44 (obsoleto: non contiene `pali`/`sportellata`; eseguito con `pali` ha fatto smoke/avvolgimento/inattività). Serve build+run: `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/mig1_j2_pali" lv_01_neon pali` e `.../mig1_j3_sportellata lv_01_neon sportellata`.
- J3 limite: `sbandataS`/sorgente rivale privati; la prova usa `urtiGara()` e progressione Z sprite come proxy (verifica sbandata ~5s pienamente misurabile richiede strumentazione/API dedicata o accesso in test).
- D4 [ ] nomi rivali (da confermare), D5 facoltativo, G1/G2 fuori perimetro. Nota: `drawDetrito` è rettangolo ruotato centrato; rotazione estrema può eccedere `baseY` di ~1px (verificare screenshot J2).

## Miglioria piccola
- Quando shell ok: build unico, eseguire `pali` poi `sportellata`, allegare esiti/screenshot a REVIEW; se J3 non aggancia colpo, affinare targeting (finestra Z/dSterzo) entro `prova_gate.cpp`.
