# Esito turno developer — 27/10/2026 21:57 — blocco RIV punti b/c

## Blocco
RIV [~] — rivali veri e urto con sbandata (Andrea 22 ott; punti b e c anticipati prima di PIST1).

## Stato del codice (nessuna modifica in questo turno)
- Punto b (sprite rivali da catalogo): 4 sprite `dati/sprite_raw/rivali/auto_rivale_{rosso,verde,giallo,azzurro}_retro.sglraw` caricati dal runner (`src/runner/main.cpp`) e da `prova_gate` tramite `Game::setRivaliSprites`; `aggiornaRivali` assegna `r.spriteIdx` e disegna `ren_.rivaleSprite(r.spriteIdx)` quando disponibile.
- Punto c (comportamento realistico in `aggiornaRivali`): rallentamento in curva (`lentaCurva` con bravura), elastico 0.80-1.15 sul distacco, distanza minima tra rivali con sorpasso di lato, schivata traffico, passo in unità/secondo (non per frame).
- Punto a (già fatto 19:41): bump urto avversario = spinta laterale + rallentamento -15%, nessun ribaltamento, controlli sempre attivi; costanti in `dati/config.json`.

## Verifica eseguita (27/10/2026 21:57)
Comando: `sh UFFICIO/Officina/TOOL/gate_t1.sh` (solo, radice sglavinated_src)
- cmake configure: OK
- cmake --build UFFICIO/Officina/build --target prova_gate: exit 0
- lv_01_neon..lv_05_spray: `prova_gate` exit 0 + `percorso` exit 0 (6/6 percorsi avviati OK)
- Il gate si è fermato su `lv_06_desktop` per timeout dello strumento (10 min): **gate_t1.sh non completato, nessun exit code valido -> gate non eseguito**
Verifica a mano su lv_06_desktop (stesso binario già compilato):
- `prova_gate ... lv_06_desktop`: GATE OK, ms/frame medio 0.654 max 1.231 (limite 20), replay 2x IDENTICO, sprite rivali caricati 4/7
- `prova_gate ... lv_06_desktop percorso`: GATE PERCORSO OK, rettilineo 53% (limiti 25-75%), curva max 0.85 (max 1.00), fuori strada 0 frame (0.0%)

## Esito
RIV b/c codice completo e verificato (nessuna regressione dopo il bump). Gate T1 completo non eseguibile in turno per timeout strumento; misure parziali sopra. Prossimo: PIST1 [ ] (tipi tratto chicane/ponte/autostrada/citta'). T1-RUN [~] e H [~] in attesa Andrea (runner Mac v91, poi Pi 400).
