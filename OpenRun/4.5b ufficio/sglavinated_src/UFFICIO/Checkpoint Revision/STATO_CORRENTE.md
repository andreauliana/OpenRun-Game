# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-09 (Turno 1, ora 0 di 12 @capo — riallineamento MIG1). Fonti: [CP-006](./CP-006-t1-run-chiusura.md) [CHIUSO], `REGISTRO_REVISIONI.md` (append-only), `UFFICIO/out/capo/STATI_PROPOSTI.md`, `ANDREA_REVIEW.md` (precedenza assoluta), `RICHIESTA_APERTA.md`, `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`, RIASSUNTI bacheca.

| Campo | Stato |
|---|---|
| Fase | v4.7: **BLOCCO MIG1** [~], poi **V47** continuo. Base legacy 08/11 approvata e **NON si tocca** (ANDREA_REVIEW sez. 2-3). v4.6 CHIUSA 08/11. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004 APERTI (review indipendente). |
| Stato attività | **MIG1 [~]** (handoff): **G1 ANNULLATO** da Andrea 09/11 (cartelloni solo Andrea). **G2 [x]** (3 osservazioni). @developer **D1-D4 [x]** handoff (paletti, sportellata, carattere rivali, nomi; costanti aperte solo MIG1); gate verde noto 09/11 09:22; `prova_gate.cpp:2136` pre-esistente blocca ctest/gate da build dev (09/11 10:00). @developer-junior **J1 [x]**, **J2/J3 [~]** codice in `prova_gate.cpp` ma esecuzione bloccata (shell permission, binario 09:44 obsoleto), **J4 [~]** `REVIEW_MIG1.md` da chiudere con prove. **D5 [ ]** facoltativo. V47: **VG1 [x]**, V1/V3-V5 [ ], V2 [x]. |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/` (symlink Libreria), `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto dal blocco. |
| Ruoli | @capo, @developer, @developer-junior, @grafico |
| Review | Peer review tra sviluppatori (autore ≠ revisore). MIG1 D1-D4: prove/review indipendenti ancora da registrare. |
| Build/test | Gate verde noto 09/11 09:22 (dev D2); 08/11 CP-006 exit 0. Blocchi: shell permission junior; `prova_gate.cpp:2136` pre-esistente. Log: `UFFICIO/Officina/Test area/`. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) (09/11 10:08: G1 annullato, G2 [x], VG1 [x], in attesa).
- Officina: [STATUS_OFFICINA.md](../Officina/STATUS_OFFICINA.md) (storico lungo; per MIG1 usare `UFFICIO/bacheche/developer*/RIASSUNTO.md` e log in `Test area/`).

## Prossime azioni
1. **@developer-junior** (quando shell ok): build `prova_gate`, run `pali` + `sportellata`, registrare exit code/misure/screenshot, chiudere `REVIEW_MIG1.md` (J2/J3/J4).
2. **@developer**: correggere `prova_gate.cpp:2136` pre-esistente se assegnato, poi ctest 27 + `gate_t1.sh` exit 0; quindi D5 (facoltativo) o V47 V1.
3. **@grafico**: in attesa richiesta/asset approvati da Andrea; sponsor non toccare.
4. @capo: registro append-only per turno; STATI_PROPOSTI/STATO_CORRENTE allineati; review indipendente quando prove disponibili. Nessun CP chiudibile ora.
5. Vietati: base legacy (auto/HUD/audio esistente/tracciati/ponte/galleria/incroci/partenza/menu-logo/premiazione/grafica Andrea/sponsor). Costanti aperte solo MIG1. `RICHIESTA.md`/`SCAMBIO.md` sola lettura.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).