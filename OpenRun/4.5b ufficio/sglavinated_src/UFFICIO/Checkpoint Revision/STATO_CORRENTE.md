# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-09 (Turno 1, ora 0 di 12 — lavoro attivo = BLOCCO MIG1 + V47). Fonte dettagliata: [CP-006](./CP-006-t1-run-chiusura.md) [CHIUSO], `UFFICIO/out/capo/STATI_PROPOSTI.md`, `ANDREA_REVIEW.md`, `RICHIESTA_APERTA.md`, `UFFICIO/Officina/STATUS_OFFICINA.md`, `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`.

| Campo | Stato |
|---|---|
| Fase | v4.7 iniziata: **BLOCCO MIG1** (Andrea 08/11 sera, `RICHIESTA_APERTA.md`), poi BLOCCO V47 continuo. Base legacy 08/11 approvata, NON si tocca (ANDREA_REVIEW sez. 2-3). v4.6 CHIUSA 08/11. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) — chiusura T1-RUN (Tappa 1, Gate T1), **CHIUSO** 2026-11-08 su mandato di Andrea: D19 aggiornata e CHIUSA in `ANDREA_REVIEW.md` ("il gioco gira sul Mac di Andrea, prove 06-07/11, gate T1 TUTTO OK 08/11. T1-RUN [x]. Il @capo chiude CP-006. Non riaprirla"). Prove: gate `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0 (log `UFFICIO/Officina/Test area/gate_t1.log`) e review indipendenti POSITIVE (developer-junior → developer su `gate_t1.sh`; developer → developer-junior su `prova_gate.cpp:969`). Non riaprire. |
| | [CP-005](./CP-005-tracciati-percorsi.md) — R1 tracciati 6 piste, **CHIUSO** 2026-10-22 (review incrociata indipendente positiva, ctest 17/17). |
| | [CP-007](./CP-007-fork1-bivio-tema.md) — FORK1 bivio e transizione tema, **CHIUSO** 2026-10-27 (review incrociate positive, ctest 20/20). |
| Stato attività | **Tappa attiva: BLOCCO MIG1** (un passo per turno, niente domande ad Andrea, si procede). Deleghe: @grafico **G1** cartelloni leggibili · @developer **D1** paletti che atterrano sull'erba · @developer-junior **J1** review in `REVIEW_MIG1.md` · @capo una riga di registro. Finiti i propri passi: BLOCCO V47 (developer V1, junior VJ1, grafico VG1). Ogni passo: ctest verde (27) + `gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 <= 12 ms/frame. Stati storici: T1-RUN [x] (CP-006 CHIUSO 08/11 su mandato Andrea, D19 con RISposta ANDREA) · P1 [x] · G1-catalogo [x] · FORK1 [x] · DECO1 [~] / F60 [~] (in attesa review incrociata) · CP-002/003/004 APERTI (review indipendente da assegnare) · H [~] (prova Pi 400 di Andrea; D12/D14 = A) · MD0, T5, N, M [-]. Base legacy intoccata: auto (`REGOLE_AUTO_GIOCATORE.md`), HUD, audio esistente, tracciati, ponte/galleria/incroci, partenza, menu e grafica di Andrea. Costanti aperte solo `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`, danni/KO/recupero rivali, volo dei paletti. |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/`, `libreria/`, `UFFICIO/Officina/TOOL/`. Dal 2026-10-19 `assets_originali/` e' un symlink a `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/` (un solo originale, nella Libreria) |
| Ruoli | @capo, @developer, @developer-junior, @grafico |
| Review | Peer review tra i due sviluppatori; autore distinto dal revisore. CP-005, CP-006 e CP-007 chiusi con review indipendenti positive. |
| Build/test | Gate verde 08/11: `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, `GATE T1: TUTTO OK`, 6/6 piste GATE PERCORSO OK, replay 2x IDENTICO, ms/frame medio 1.079-1.758 max 17.003 (<= 20), screenshot P1 completo (log `UFFICIO/Officina/Test area/gate_t1.log`). ctest 27/27 verde (report Officina). Build CMake in `UFFICIO/Officina/build/`. |
| Limite della prova | Eseguito su macOS; prestazioni e compatibilità su Raspberry Pi 400 non verificate da questo gate |

## Handoff grafico

Leggere [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) come unico riepilogo aggiornato del @grafico. Inventario dettagliato: [CATALOGO_RISORSE.md](../UFFICIO/Dipartimento%20Grafico/Libreria/CATALOGO_RISORSE.md). Ultimo handoff 07/11 12:04: catalogo 220 elementi, 0 non catalogati, CC0-1.0/OFL/MIT/CC-BY-4.0, nessun materiale Sega.

## Handoff Officina

Leggere [STATUS_OFFICINA.md](../UFFICIO/Officina/STATUS_OFFICINA.md) per l'ultimo report di sviluppo, build, test e blocchi. Nel checkpoint riportare soltanto esito, misure e riferimenti ai log pertinenti; lo status non sostituisce la peer review.

## Prossime azioni

1. **BLOCCO MIG1 (tappa attiva, Andrea 08/11 sera), un passo per turno:** @grafico **G1** (cartelloni sponsor leggibili, stesso nome/misura, `Libreria/assets_originali/sponsor/` + `genera_sponsor.py`, raw con `png_a_raw.py`, catalogo aggiornato, prima/dopo in `Test area/grafico/`); @developer **D1** (paletti abbattuti che atterrano sull'erba, posizione nel mondo, mai sotto la riga ruote: `Game::abbattiPali` + render); @developer-junior **J1** (review breve in `REVIEW_MIG1.md`); @capo una riga di registro. Ogni passo: ctest verde (27) + `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0, replay 2x IDENTICO, Pi 400 <= 12 ms/frame. Poi D2 sportellata, D3 rivali con carattere, D4 nomi, D5 audio extra (facoltativo); J2 dopo D1, J3 dopo D2, G2 dopo D1-D3. Finiti i propri passi: BLOCCO V47 (developer V1, junior VJ1, grafico VG1).
2. Vietati (base legacy): auto giocatore (`REGOLE_AUTO_GIOCATORE.md`), HUD/UI, audio esistente (solo aggiunte), tracciati/tempi, ponte/galleria/incroci, partenza, menu e logo, premiazione, musica/mixer.*, `src/runner/`, scenari/li rifà Andrea. Costanti aperte solo `AFFIANCATA_DX`, `URTO_SPINTA_RIVALE`, `RIVALE_SORPASSO_DX`, danni/KO/recupero rivali, volo dei paletti.
3. CP-002/003/004 APERTI: in attesa di review indipendente da ruolo diverso dall'autore.
4. DECO1 [~]: review incrociata del gate `decorazioni`, poi CP-008. F60 [~]: review incrociata developer→developer-junior, poi CP F60.
5. Pi 400/RetroPie: nessun lavoro dedicato prima della prova di Andrea su Mac (D12/D14 = A); H [~] di Andrea.

Per la cronologia e gli ID, consultare [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Per una nuova scheda, usare [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
