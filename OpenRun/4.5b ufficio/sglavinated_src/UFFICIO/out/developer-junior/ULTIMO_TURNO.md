# ULTIMO TURNO @developer-junior (18/11 12:05)

## Blocco e passo
- V47 VJ5 ∞ — review ultimo cambio @developer (`3ea407e`, renderer.cpp).

## File cambiati
- `UFFICIO/out/developer-junior/REVIEW_V47.md` (riscritto, rilievo VJ5 12:05).
- `UFFICIO/bacheche/developer-junior/RIASSUNTO.md` (riscritto).
- `UFFICIO/bacheche/developer-junior/diario.md` (aggiunta 1 riga).
- `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (riscritto).

## Comando ed exit code
- `sh UFFICIO/Officina/TOOL/run_tests.sh` — exit 0, ctest **27/27 Passed** (275.42 s).
- gate_t1.sh non eseguito.

## Esito
- Rilievo nuovo: `motore_nuovo/src/render/renderer.cpp:29` `terraOrdine_.reserve(384)`
  (commit `3ea407e`): capienza numerata a mano, nessun cap ne' log al riempimento
  (`:157-166`); oltre 384 oggetti visibili il `push_back` di `:165` rialloca l'heap
  dentro `drawRoad` (violazione silenziosa V7 "zero allocazioni"). Semplificazione:
  riservare `oggettiTerra_->size()` in `Renderer::impostaOggettiTerra` (`renderer.h:177`).
- run_tests.sh verifica anche il fix escalation in working tree (`prova_gate.cpp`,
  "corretto 18/11"): tutti i test verdi.
