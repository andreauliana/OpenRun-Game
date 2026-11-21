# Riepilogo mattina — 21/11/2026 (@capo)

## Cosa è stato fatto (per blocco/agente)
- **V47/MIG1 [~] (continuo, ∞)** — base legacy 08/11 e vocabolario intoccati; nessun CP chiudibile (mancano gate+review indipendenti in dossier).
- **@developer (V9 ∞, fluidità a giro):** ultimo handoff 21/11 10:24, lv_01_neon 27° giro. Ottimizzazione `drawTerraSospesa` in `renderer.cpp` (hash riga precalcolato, `cellaF` riusato), resa/replay identici. `prova_gate` smoke exit 0 (medio 0.648 max 1.338 ms), percorso exit 0 (1989f, 33.2s, 0% fuori), `gate_t1.sh` exit 0 6/6 (`GATE T1: TUTTO OK`). Fonte: `UFFICIO/Officina/STATUS_OFFICINA.md`, `UFFICIO/out/developer/ULTIMO_TURNO.md`.
- **@developer-junior (VJ5 ∞, review):** 21/11 10:29 review hunk `hshFila` `drawTerraSospesa` (`renderer.cpp`:319-331), nessun errore; rilievi minori doc/commento. `run_tests.sh` exit 0 (ctest 27/27, 146.77s), `prova_gate ... lv_01_neon percorso` exit 0. VJ3 4/6 (mancano lv_05/lv_06 50/60 Hz), VJ4 6/6 chiuso. Fonte: `UFFICIO/Officina/STATUS_OFFICINA.md`, `UFFICIO/out/developer-junior/ULTIMO_TURNO.md`.
- **@grafico (VG3 ∞):** giro 130, 21/11 10:35. Nessun PNG nuovo da convertire; skyline 6/6, scenari/oggetti allineati, 0 da convertire. Catalogo invariato, zero esterni/Sega. Fonte: `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`.
- **@capo:** STATO_CORRENTE/STATI_PROPOSTI allineati al 21/11 T1; SCAMBIO_RECENTE checkpoint automatici verdi ma non peer review in dossier. CP-006 CHIUSO, non riaprire.

## Cosa non funziona / blocchi
- **Review indipendenti non registrate in dossier:** nessun CP chiudibile; CHK SCAMBIO_RECENTE sono controlli automatici, non peer review.
- **Asset grafico bloccato (decisione Andrea):** `GRAFICA/sprite/traffico_incrocio/lato_furgone_bianco.png.png` (20/11 10:42) senza raw/voce catalogo; grafico non rinomina/converte. DEFAULT: invariato.
- **Allineamento handoff capo arretrato:** `STATO_CORRENTE.md`/`STATI_PROPOSTI.md` riportano V9 lv_02 25° (20/11) mentre `STATUS_OFFICINA.md` riporta V9 lv_01 27° (21/11) — capo deve riallineare nel prossimo turno.
- **Pi 400 non verificato:** limite 12 ms/frame non misurato (blocco H sospeso).
- **`ULTIMO_TURNO.md` grafico non riscrivibile:** permesso negato; handoff solo via `STATUS_GRAFICO_PER_CAPO.md`.

## Decisioni aperte in ANDREA_REVIEW.md (3 più importanti)
- **Nessuna decisione aperta.** `ANDREA_REVIEW.md` §7 riporta esplicitamente: "Nessuna". Le 3 questioni operative più importanti che richiedono attenzione/decisione (non in ANDREA_REVIEW):
  1. **`lato_furgone_bianco.png.png`**: rinominare/cancellare/convertire? Attende Andrea.
  2. **Chiusura checkpoint V47/MIG1**: servono gate+review indipendenti autore≠revisore registrati in dossier; oggi nessuna.
  3. **Verifica Pi 400**: sospesa fino a disponibilità hardware Andrea (blocco H).

## Prossimo passo consigliato
- **@capo (prossimo turno):** riallineare `STATO_CORRENTE.md`/`STATI_PROPOSTI.md` agli handoff 21/11 (dev lv_01 27°, junior VJ5 hshFila, grafico giro 130), poi assegnare esplicitamente una review indipendente autore≠revisore su un diff V9 recente e registrarla in dossier CP per sbloccare la prima chiusura verificabile. Non rieseguire gate se non serve; citare exit code/log esistenti.