# RIASSUNTO @developer-junior
Riscritto 09/11 12:2x (turno MIG1 J3/J4). MIG1 J1,J2,J4 [x], J3 [~]; V47 VJ1,VJ2 [x].
- Ruolo: test `motore_nuovo/test/`, prove via `prova_gate`/ctest in Officina (gate_t1 non eseguito in questo turno).
- **J3 [~]** questo turno: riscritto modo `sportellata` in `prova_gate.cpp` (solo rivali scala 1.7, evita il traffico, misura velocita' reale del rivale 0-8 s). Build exit 0; test lv_01_neon exit **28**: nessuna sportellata a un rivale in 30 s.
- La vecchia evidenza J3 "colpo 0.52 s" era un urto col traffico: nessun controllo sul tipo di bersaglio. Non prova D2.
- **Rilievo D2** (per @developer): ramo laterale game.cpp ~2371 (dz 750-1100, |dx|<240, dSterzo>0.22) arriva dopo il tamponamento ~2119 (stesso dz, mette playerBump_=true): il ramo laterale e' di fatto bloccato. Fix proposto: escludere dal tamponamento chi e' gia' affiancato (dz > URTO_Z_MIN) o controllare prima lo sterzo.
- D1-D5 [x] verificati a codice (D4 NOMI_RIVALI 7; D5 suonaPaletto in abbattiPali); REVIEW_MIG1 riscritta.
- V47 VJ2 [x]: screenshot riva lv_01/04/06 in `UFFICIO/Officina/Test area/vj2_riva/`.
- MIG1 residui: J3 [~] (bloccato da D2 in src, no autorizzazione a scrivere `motore_nuovo/src/`); G2 [ ] (grafico).
- Prossimo: se @developer corregge D2, rieseguire `sportellata` (exit atteso 0 con ripresa 5-8 s); resta disponibile una prova dedicata per ogni nuovo passo V.
- File turno: `motore_nuovo/test/prova_gate.cpp`, `REVIEW_MIG1.md`, diario, `ULTIMO_TURNO.md`, questo RIASSUNTO.
- Mai: HUD, `motore_nuovo/src/` senza autorizzazione, file fuori perimetro.
