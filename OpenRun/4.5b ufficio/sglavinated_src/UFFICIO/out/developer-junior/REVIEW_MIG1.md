# REVIEW_MIG1 — @developer-junior (J4, riscritto 10/11 ~01:52)
Fonti: `RICHIESTA_APERTA.md` MIG1, RIASSUNTI developer/capo, codice src (sola lettura), prove di questo turno.

## Cosa va
- **D1 [x]** J2: prova `pali` exit 0 (09/11 10:45), hit_rilevati=17. Detriti in coord. mondo/clampati.
- **D3 [x]** carattere rivali deterministico, anti-sovrapposizione. **D4 [x]** `NOMI_RIVALI` 7 nomi max 6 maiuscole. **D5 [x]** `Mixer::suonaPaletto`, solo effetto nuovo.
- **J1 [x]** review iniziale. **VJ1 [x]** percorso lv_01-lv_06 tutti exit 0, 0% fuori strada.
- **Gate 10/11 ~01:52:** `sh UFFICIO/Officina/TOOL/gate_t1.sh` **exit 0** "GATE T1: TUTTO OK" (replay 2x IDENTICO, ms/frame medio 0.94-1.34).

## Cosa non va (J3 — sportellata RIVALE)
- 10/11 ~01:50: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0; `prova_gate "UFFICIO/Officina/Test area/j3_d2_1011b" lv_01_neon sportellata` **exit 28**. urti=3, frame_finestra=30, frame_stretto=0, min_dx_finestra=242, bassa_vel=29, tagli=26.
- **Causa 1 (invariata):** trigger `game.cpp:2386` `|dSterzo| > 0.22f`; bound DERIVATO da `player.h` a 60 Hz = 0.200 (ritorno 12/s) e 0.133 (rampa 8/s) -> **IRRAGGIUNGIBILE**. Sbandata/ripresa 5 s di D2 (1579-1582, 2884-2885) implementata ma mai innescata.
- **Causa 2 (nuova, 10/11):** il clamp laterale `game.cpp:2390` tiene `|dx| >= URTO_LARGHEZZA` (240, `game.h:166`) da affiancati -> la finestra `|dx| < 240` del trigger si valuta solo in avvicinamento (min_dx_finestra=242 >= 240). Riga nuova nel test: `J3 clamp: ... finestra stretta irraggiungibile`.
- **J3 resta [~]:** fix in `motore_nuovo/src/` = fuori perimetro @developer-junior, bloccato @developer (D2 non ancora efficace a gioco).

## Miglioria piccola
- Adeguare trigger/soglia D2 al profilo sterzo reale (o trigger su avvicinamento, non su delta per-frame) e poi J3 misura secondi di colpo + recupero ~5 s; getter sola lettura su `sbandataS` per la misura precisa.
