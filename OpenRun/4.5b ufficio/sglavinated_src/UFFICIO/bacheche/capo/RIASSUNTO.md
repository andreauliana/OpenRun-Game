# Riassunto turno — @capo

- 2026-11-09, Turno 1 (ora 0 di 12): lette TAPPE, ANDREA_REVIEW (prec. ass.: legacy bloccata, MIG1→V47), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS, RICHIESTA_APERTA (MIG1), SCAMBIO_RECENTE, checkpoint e handoff grafico/officina/bacheche.
- Nessun CP chiudibile: CP-005/006/007 CHIUSI; CP-002/003/004 senza review indipendente; DECO1/F60 review incrociata; MIG1 senza gate+review indipendente registrati.
- MIG1 [~]: G1 ANNULLATO da Andrea (cartelloni solo Andrea); G2 [x]. @developer D1-D4 [x] handoff, D5 [ ] (costanti aperte solo `AFFIANCATA_DX`,`URTO_SPINTA_RIVALE`,`RIVALE_SORPASSO_DX`; danni/KO/recupero/paletti). @developer-junior J1 [x], J2/J3 [~] codice in `prova_gate.cpp` ma prove bloccate (shell permission, binario 09:44 obsoleto), J4 [~]. V47 VG1 [x].
- Blocchi: shell junior; `prova_gate.cpp:2136` pre-esistente (`!game.init()` su init void) blocca ctest/gate da build dev; gate verde noto 09/11 09:22.
- Deleghe (UFFICIO/out/<ruolo>/): @developer-junior build+run `pali`/`sportellata` + REVIEW_MIG1.md; @developer fix prova_gate:2136 se assegnato poi ctest+gate_t1.sh, quindi D5/V47 V1; @grafico in attesa.
- Aggiornati: `UFFICIO/out/capo/STATI_PROPOSTI.md`, `UFFICIO/Checkpoint Revision/STATO_CORRENTE.md`, `REGISTRO_REVISIONI.md` (append), diario. Nessuna domanda ad Andrea; nessun PER_CLAUDE.md. FINITO non creato.