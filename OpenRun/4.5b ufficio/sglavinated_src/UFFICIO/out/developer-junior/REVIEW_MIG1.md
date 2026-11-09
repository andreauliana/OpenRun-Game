# REVIEW_MIG1 — @developer-junior (J4, riscritto 09/11 12:2x)
Fonti: `RICHIESTA_APERTA.md` MIG1, `UFFICIO/out/developer/ULTIMO_TURNO.md`, `UFFICIO/bacheche/developer/diario.md`, codice in `motore_nuovo/src/game/`, prove locali 09/11 12:2x. Gate_t1.sh non eseguito in questo turno.

## Cosa va
- **D1 [x]** (letto): detriti in coord. mondo, proiezione strada, base clampata <= 208 px. **J2 [x]**: prova `pali` in `motore_nuovo/test/prova_gate.cpp`, exit 0 (09/11 10:45), hit_rilevati=17.
- **D3 [x]** (letto): `CarattereRivale` deterministico, guardie anti-sovrapposizione.
- **D4 [x]** (letto): `NOMI_RIVALI` (game.cpp:134) 7 nomi max 6 maiuscole, temi diversi; coerente col blocco.
- **D5 [x]** (letto): `Mixer::suonaPaletto` (mixer.h:65) chiamato in `abbattiPali` (game.cpp:2946), solo effetto nuovo in aggiunta.
- **J1 [x], J4 [x]**: review aggiornata a D1-D5.

## Cosa non va (J3 — sportellata a un RIVALE)
- **J3 [~]**: prova riscritta in questo turno (`sportellata` in `prova_gate.cpp`): bersaglio solo rivali (scala 1.7, il traffico 1.4), schiva il traffico piu' vicino, misura la velocita' reale del rivale colpito in finestre da 1 s (0-1..7-8 s) per vedere sbandata e ripresa.
  - Build `prova_gate` exit 0; `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj3_rivale" lv_01_neon sportellata` **exit 28**: nessuna sportellata a un rivale in 30 s (urti=1, `rivali_visibili_max=7`, `min_dz_rivale=1`: i rivali passano accanto, ma nessun contatto valido).
  - La vecchia evidenza J3 ("colpo 0.52 s") era un urto col **traffico** (nessun controllo sul tipo di bersaglio): non prova D2.
- **Blocco probabile in D2 (game.cpp)**: la sportellata laterale (riga ~2371-2392: dz > `URTO_Z_MIN`=750 e dz < 1100, |dx| < `URTO_LARGHEZZA`=240, `dSterzo` > 0.22, richiede `!playerBump_`) arriva **dopo** il tamponamento (righe ~2119-2138: dz < 1100 e |dx| < 240 -> `urto(..., true)`). Lo stesso intervallo dz e' coperto prima dal tamponamento, che mette `playerBump_ = true` e fa scattare il `continue`/skip del ramo laterale. Il ramo laterale risulta quindi raggiungibile solo se `|dx|` passa sotto 240 senza che il tamponamento lo veda nello stesso frame (differenza solo dalla deriva laterale del rivale, pochi px). Da verificare/fixare in `src/` (@developer): escludere dal tamponamento chi e' gia' affiancato (dz > `URTO_Z_MIN`) o controllare il colpo di sterzo prima del tamponamento.

## Miglioria piccola
- `prova_gate` modo `sportellata`: esporre in `game.h` un getter pubblico di sola lettura per lo stato rivale (es. `sbandataS`/velocita') renderebbe J3 una misura diretta; richiede autorizzazione temporanea su `motore_nuovo/src/`.
