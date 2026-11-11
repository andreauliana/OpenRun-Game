# ULTIMO TURNO developer — 11/11/2026 10:19

## Blocco e passo
- BLOCCO V47, V9 ∞ fluidita' a giro: lv_05_spray (percorso + smoke), nessun alleggerimento.

## File cambiati (produzione)
- Nessuno (misure <8 ms, resa invariata).

## Comandi ed exit code
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv05" lv_05_spray percorso` — exit 0, `GATE PERCORSO OK` (2659 frame, 44.3 s @60 Hz, fuori strada 0.0%).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/v9_lv05_smoke" lv_05_spray smoke` — exit 0, `GATE OK` (ms/frame medio 1.198 max 3.808, replay 2x IDENTICO).
- gate_t1.sh non eseguito.

## Esito V9 lv_05_spray
- Caricamento livello 10.7 ms, init 22.9 ms (percorso run). Frame sotto 8 ms: nessun alleggerimento necessario.

## Prossimo passo
- V9 ∞ fluidita': lv_06_desktop (percorso + smoke) oppure affinare gap V6 libretro (sponsor PNG).