# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-09 (Turno 1, ora 0 di 12 @capo — tappa attiva = BLOCCO MIG1 [~]). Fonti: [CP-006](./CP-006-t1-run-chiusura.md) [CHIUSO], `REGISTRO_REVISIONI.md`, `UFFICIO/out/capo/STATI_PROPOSTI.md`, `ANDREA_REVIEW.md` (precedenza assoluta), `RICHIESTA_APERTA.md`, `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`.

| Campo | Stato |
|---|---|
| Fase | v4.7: **BLOCCO MIG1** [~] (Andrea 08/11 sera), poi **V47** continuo. Base legacy 08/11 approvata e **NON si tocca** (ANDREA_REVIEW sez. 2-3). v4.6 CHIUSA 08/11. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (D19 CHIUSA, gate T1 exit 0, review incrociate POSITIVE). Non riaprire. [CP-005](./CP-005-tracciati-percorsi.md) CHIUSO. [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSO. CP-002/003/004 APERTI (review indipendente). |
| Stato attività | **MIG1 [~]** (handoff, da verificare con review/prove): @grafico **G1 [x]** cartelloni sponsor (21, raw/catalogo, `pronto` in attesa ok Andrea; anteprime `Test area/grafico/mig1_g1/`; `check_catalogo_grafico.py` exit 1 preesistente non-G1). @developer **D1 [x]** paletti coord. mondo/erba (ctest 27/27, gate_t1.sh exit 0, replay IDENTICI, ms/frame medio 1.07-1.66 max 10.47 ≤ 20). @developer-junior **J1 [ ]** (`REVIEW_MIG1.md` assente). **Prossimi:** junior J1 poi J2 prova D1 con screenshot; developer D2 sportellata (solo `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`, recupero ~5 s, danni/KO invariati); grafico G2 dopo D1-D3. Finiti passi MIG1 → V47. Storici: T1-RUN [x] · P1 [x] · FORK1 [x] · DECO1 [~] · F60 [x] gate/[~] review · H [~] (Pi 400 Andrea, D12/D14=A) · MD0,T5,N,M [-]. |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/` (symlink Libreria), `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto dal blocco. |
| Ruoli | @capo, @developer, @developer-junior, @grafico |
| Review | Peer review tra sviluppatori (autore ≠ revisore). G1/D1: review indipendente ancora da registrare (J2 su D1). |
| Build/test | Ultimo gate verde noto 08/11 (CP-006): `gate_t1.sh` exit 0, 6/6 piste, replay IDENTICO, ms/frame ≤ 20. D1 handoff 09/11: ctest 27/27, gate exit 0 (da confermare nel dossier se si apre CP MIG1). Log: `UFFICIO/Officina/Test area/`. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (09/11 09:19: VG1 quadro, G1 [x] in attesa ok Andrea, G2 bloccata su D1-D3).
- Officina: [STATUS_OFFICINA.md](../UFFICIO/Officina/STATUS_OFFICINA.md) (storico lungo; per MIG1 usare RIASSUNTI bacheca + log in `Test area/`).

## Prossime azioni
1. **@developer-junior J1** → `UFFICIO/out/developer-junior/REVIEW_MIG1.md`; poi **J2** prova D1 paletti con screenshot in `UFFICIO/Officina/Test area/`.
2. **@developer D2** sportellata (costanti aperte MIG1; recupero ~5 s, danni/KO invariati). Ogni passo: ctest 27 + `gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 ≤ 12 ms/frame.
3. **@grafico G2** dopo D1-D3: 3 osservazioni in `ULTIMO_TURNO.md`/handoff.
4. @capo: registro append-only per turno; STATI_PROPOSTI allineato; niente domande ad Andrea. Nessun CP chiudibile ora.
5. Vietati: base legacy (auto/HUD/audio esistente/tracciati/ponte/galleria/incroci/partenza/menu-logo/premiazione/grafica Andrea). Costanti aperte solo MIG1. `RICHIESTA.md`/`SCAMBIO.md` sola lettura.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
