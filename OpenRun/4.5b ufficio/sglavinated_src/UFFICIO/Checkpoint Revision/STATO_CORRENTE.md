# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-08 (Turno 1, ora 0 di 12 — riscrittura STATI_PROPOSTI e riga di registro; chiusura CP-006 gia' registrata). Fonte dettagliata: [CP-005](./CP-005-tracciati-percorsi.md) [CHIUSO], [CP-006](./CP-006-t1-run-chiusura.md) [CHIUSO], `UFFICIO/out/capo/STATI_PROPOSTI.md`, `ANDREA_REVIEW.md`, `UFFICIO/Officina/STATUS_OFFICINA.md`, `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`.

| Campo | Stato |
|---|---|
| Fase | 4.6, migrazione graduale |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) — chiusura T1-RUN (Tappa 1, Gate T1), **CHIUSO** 2026-11-08 su mandato di Andrea: D19 aggiornata e CHIUSA in `ANDREA_REVIEW.md` ("il gioco gira sul Mac di Andrea, prove 06-07/11, gate T1 TUTTO OK 08/11. T1-RUN [x]. Il @capo chiude CP-006. Non riaprirla"). Prove: gate `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0 (log `UFFICIO/Officina/Test area/gate_t1.log`) e review indipendenti POSITIVE (developer-junior → developer su `gate_t1.sh`; developer → developer-junior su `prova_gate.cpp:969`). Non riaprire. |
| | [CP-005](./CP-005-tracciati-percorsi.md) — R1 tracciati 6 piste, **CHIUSO** 2026-10-22 (review incrociata indipendente positiva, ctest 17/17). |
| | [CP-007](./CP-007-fork1-bivio-tema.md) — FORK1 bivio e transizione tema, **CHIUSO** 2026-10-27 (review incrociate positive, ctest 20/20). |
| Stato attività | **Tappa 1: Gate T1-RUN [x] (CP-006 chiuso 08/11). Tappa attiva: rifinitura del percorso — BLOCCO PERC1** (TAPPE.md 08/11), un passo per turno, senza aspettare nessuno. Stati: PERC1 [~] (@developer D1-D5; @developer-junior J1-J4; @grafico G1-G3; vincoli ctest 27/27 verde e `gate_t1.sh` exit 0 dopo ogni passo) · P1 [x] · G1-catalogo [x] · FORK1 [x] · DECO1 [~] (resta review incrociata + CP-008) · F60 [x] gate/[~] review (incrociata developer→developer-junior pendente) · R2 [~] (NON subordinato a D18/D19; prossimo 1d poi RIV a-c poi PIST1 punto 1) · RIV [~] (codice a-c fatto, grafica rivali a @grafico) · PIST1 [~]/[ ] (G-P1 [x]; P3 parapetti **annullato per sempre** da Andrea 06/11 confermato 08/11: ponte aperto col filo basso, niente `parapetto.png`/`guardrail.png` sulla pista) · FIN [ ] · H [~] (prova Pi 400 di Andrea quando vorra'; ordine di test: prima Mac poi Pi 400, D12/D14 = A) · MD0, T5, N, M sospesi [-]. HUD NON si tocca (Andrea 08/11): niente `dati/ui/hud.json`, `drawHudDati`, `assets_originali/ui/`. Auto del giocatore: regole bloccate in `REGOLE_AUTO_GIOCATORE.md`. Parapetti ponte: annullati per sempre. |
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

1. **BLOCCO PERC1 (tappa attiva), un passo per turno:** @developer **D1** (dati `dati/tracciati/tratti.json`: `curva_cieca_dx/sx` curva ±0.6, `dune` 0.12, `onde` 1, lunghezza 220; `saliscendi` dune 0.09, onde 3, lunghezza 300; nel `tracciato` di lv_02_atomico, lv_03_foglie, lv_05_spray: curve cieche in coppia dx+sx, prima degli ultimi 1200 segmenti, somma curva*lunghezza ~0, nessun tipo ripetuto due volte di fila), poi ctest 27/27 verde e `sh UFFICIO/Officina/TOOL/gate_t1.sh` exit 0. Poi D2 traffico per tratto, D3 frecce cresta, D4 galleria, D5 miglioria dalla review J4. **@developer-junior J1** (gate `tratti`: >= 7 tipi di tratto per livello, nessun tipo ripetuto due volte di fila, una riga per livello); J2 dopo D2, J3 dopo D4, J4 review breve in `UFFICIO/out/developer-junior/REVIEW_PERC1.md` dopo ogni passo D. **@grafico G1** (`freccia_cresta_dx/sx.png`, script in TOOLS, raw con `png_a_raw.py`, 2 voci `pronto`, `check_catalogo_grafico.py` exit 0); poi G2, G3. **@capo**: una riga di registro per turno e `UFFICIO/out/capo/STATI_PROPOSTI.md` allineato.
2. Vietati PERC1: HUD/varianti UI; auto del giocatore (`REGOLE_AUTO_GIOCATORE.md`); premiazione; musica/mixer.*; `src/runner/`; scenari/skyline (li rifà Andrea); parapetti sul ponte (annullati per sempre); mai togliere curve o `rampa_ponte`/`discesa_ponte` (si AGGIUNGONO tratti solo con D1/D4).
3. CP-002 aperto: G1-catalogo [x] (handoff 07/11 12:04, 220 elementi, 0 non catalogati) resta in attesa di review indipendente da assegnare a un ruolo diverso da @grafico (cand. @developer-junior).
4. DECO1 [~]: review incrociata del gate `decorazioni` da completare, poi CP-008 da aprire. F60 [~]: review incrociata developer→developer-junior da registrare, poi CP F60.
5. R2 [~] NON subordinato a CP-006/D19 (Andrea 27/10): prossimo @developer passo 1d, poi RIV punti a-c, poi PIST1 punto 1. RIV grafica rivali a @grafico.
6. Pi 400/RetroPie: nessun lavoro dedicato prima della prova di Andrea su Mac (ordine test T1-Q5, D12/D14 = A); H [~] di Andrea.

Per la cronologia e gli ID, consultare [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Per una nuova scheda, usare [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).
