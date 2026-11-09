# Stato corrente — OpenRun 4.6

> Scheda di avvio per Capo e ruoli autorizzati. Aggiornata: 2026-11-09 (Turno 1, ora 1 di 12 @capo). Fonti: [CP-006](./CP-006-t1-run-chiusura.md) [CHIUSO], `REGISTRO_REVISIONI.md` (append-only), `UFFICIO/out/capo/STATI_PROPOSTI.md`, `ANDREA_REVIEW.md` (precedenza assoluta), `RICHIESTA_APERTA.md`, handoff grafico + RIASSUNTI developer/developer-junior/grafico.

| Campo | Stato |
|---|---|
| Fase | v4.7: **MIG1 [~] quasi chiuso** + **V47 [~] in corso** (continuo, solo aggiunte). Base legacy 08/11 approvata e **NON si tocca** (ANDREA_REVIEW sez. 2-3). v4.6 CHIUSA 08/11. |
| Ultimo checkpoint | [CP-006](./CP-006-t1-run-chiusura.md) **CHIUSO** 2026-11-08 (non riaprire). [CP-005](./CP-005-tracciati-percorsi.md), [CP-007](./CP-007-fork1-bivio-tema.md) CHIUSI. CP-002/003/004 APERTI (review indipendente). |
| Stato attività | **MIG1:** G1 **ANNULLATO** (sponsor solo Andrea). D1-D3,G2,J1,J2,J4 [x] da handoff/prove; D4 [~] discrepanza (dev D1-D5 [x] vs junior D4 [ ]); D5 [x] handoff dev (facoltativo, da review); J3 [~] colpo 0.52s ok, recupero `sbandataS` ~5s non misurato direttamente. **V47:** VG1 [x], V1 [x], V2 [x], V3 [x] (ctest 27/27 + gate_t1 exit 0 09/11 10:34, ms/frame 6 livelli 0.988-1.427 ≤ 20), V4/V5 [ ], VJ1 [~], VJ2 [ ]. |
| Sorgenti attivi | `motore_nuovo/`, `assets_originali/` (symlink Libreria), `libreria/`, `UFFICIO/Officina/TOOL/`. Scrittura solo in `UFFICIO/` e `motore_nuovo/` quando richiesto dal blocco. |
| Ruoli | @capo, @developer, @developer-junior, @grafico |
| Review | Peer review tra sviluppatori (autore ≠ revisore). MIG1/V47: review incrociate non registrate in dossier; necessarie per eventuale CP. |
| Build/test | Dev 10:34: ctest 27/27 exit 0, `gate_t1.sh` exit 0, 6/6 replay IDENTICI, screenshot P1 OK. Junior 10:45: `pali` exit 0 hit=17+screenshot; 10:15: `sportellata` exit 0 colpo 0.52s. Log/screenshot: `UFFICIO/Officina/Test area/`. |
| Limite della prova | Prove su macOS; Pi 400 non verificato da gate. |

## Handoff
- Grafico: [STATUS_GRAFICO_PER_CAPO.md](../out/grafico/STATUS_GRAFICO_PER_CAPO.md) + `UFFICIO/bacheche/grafico/RIASSUNTO.md` (G1 annullato, G2 [x], VG1 [x], in attesa).
- Developer: `UFFICIO/bacheche/developer/RIASSUNTO.md` (D1-D5 [x], V1/V3 [x], prossimo V4/V5).
- Developer-junior: `UFFICIO/bacheche/developer-junior/RIASSUNTO.md` (J1/J2/J4 [x], J3 [~], prossimo review MIG1 + VJ1/VJ2).

## Prossime azioni
1. **@developer**: V4 dissolvenza 0.5s risultati/menu/partenza, poi V5 testi EN; chiarire D4 se review lo richiede. Dopo ogni passo: ctest verde + `gate_t1.sh` exit 0, replay 2x IDENTICO.
2. **@developer-junior**: review incrociata MIG1 (focus D4, J3 recupero `sbandataS` privato — strumentare o registrare limite); VJ1 su V1/V3/V4 (`REVIEW_V47.md` riscritto); VJ2 screenshot spiagge/gabbiani/barche lv_01,04,06.
3. **@grafico**: in attesa richiesta/asset approvati da Andrea; sponsor non toccare.
4. **@capo**: registro append-only per turno; assegnare/registrare review incrociate quando prove complete; aprire dossier CP solo per revisione sostanziale. Nessun CP chiudibile ora.
5. Vietati: base legacy (auto/HUD/audio esistente/tracciati/ponte/galleria/incroci/partenza/menu-logo/premiazione/grafica Andrea/sponsor). Costanti aperte solo MIG1. `RICHIESTA.md`/`SCAMBIO.md` sola lettura.

Cronologia: [REGISTRO_REVISIONI.md](./REGISTRO_REVISIONI.md). Template: [TEMPLATE_CHECKPOINT.md](./TEMPLATE_CHECKPOINT.md).