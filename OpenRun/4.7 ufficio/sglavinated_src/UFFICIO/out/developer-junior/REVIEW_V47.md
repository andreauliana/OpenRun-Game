# REVIEW V47 — VJ4 soak `lv_02_atomico` (23/11/2026 08:11)

- **Ruolo**: @developer-junior (prova dedicata col gate esistente, nessuna modifica a src).
- **Blocco**: V47 VJ4 (soak su tutti e 6 i livelli, uno per turno). Secondo della lista: `lv_02_atomico` (diverso da `lv_01_neon` del turno prima). Ricompilazione prima della prova con `run_tests.sh`.

## VJ4 — soak `lv_02_atomico` (2/6)
- **Comando**: `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj4_lv02_atomico_soak_2311_t1" lv_02_atomico soak` — **exit 0** (`GATE OK`).
- **Replay 2x**: **IDENTICO** (30000 frame x2, nessuna divergenza). Nessun crash.
- **ms/frame**: medio **1.051**, max **90.682** (frame 22150), limite 20. Media ben sotto soglia; il max e' uno spike isolato (un solo frame su 30000, non ripetuto nel replay identico), non un peggioramento medio. Confronto Pi 400 (<= 12 ms): da tenere d'occhio lo spike, non la media. Stesso fenomeno gia' visto su `lv_01_neon` (61.573 ms, frame 10343, 07:51).
- **Altro**: accelerazione reattiva (speed@1=8.43, speed@60=154.11), offroad raggiunto (frame 156), X[min,max]=[-1560.2,2100.0], zFinale=408676.4. CARICAMENTO livello 6.5/5.1 ms, init 15.8/12.1 ms (due run).

## Stato
- VJ4: 2/6 fatti (`lv_01_neon`, `lv_02_atomico`). Prossimo turno: `lv_03_foglie` soak (livello diverso dal turno prima).
- `git status --short` — **exit 0** (M src renderer.cpp + road.cpp, non toccati: solo prova).
- `sh UFFICIO/Officina/TOOL/run_tests.sh` — **exit 0** (ctest **27/27**, 205.30 s, prova_gate ricompilato).
- `gate_t1.sh` non eseguito in questo turno. Nessun fail aperto noto.
