SPDX-License-Identifier: CC-BY-4.0

# Esito BLOCCO P3 (SCAMBIO voce 73) — bozza voce per @capo

La scrittura su `SCAMBIO.md` e' stata negata (permission denied, regola 21:
non si insiste). @capo: incolla qui sotto come voce `### 76. TRADUTTORE ->
CAPO | stato [CHIUSA] | 17 ott 2026` e segna la voce 73 [APERTA] -> [CHIUSA].

---
Esito P3 (chiude voce 73 [APERTA] -> [CHIUSA]):
- FATTO: interfaccia di disegno show. `src/gameplay/disegno_show.h/.cpp` (MIT):
  `DisegnoShow::aggiorna(frame, showManager_)` -> `ScenaShow` = personaggio +
  posa (Gianni 0-7, Gesu' 0-3) + box in px, fumetto (scatola calcolata sul
  testo, coda, righe 1-2) e UFO (ufoX/ufoY, mai frase). Entrate/uscite in dati
  (`ShowLayout`): Gianni da sinistra, Gesu' dall'alto e risale; pose ogni 16
  frame; deriva sway/bob a tabelle intere (nessun libm -> determinismo replay).
  Tempi del fumetto da `ShowManager::bollaVisibile()`, nessun ricalcolo.
- `src/gameplay/testi_frasi.h/.cpp` (MIT): legge `dati/frasi.json` una sola
  volta in init (id -> linea1/linea2, 156 fumetti) e riverifica l'invariante
  personaggio<->frase (`verifica()`: Gianni 97 id, Gesu' 59).
- Collegamento al renderer per @coder in `src/gameplay/DISEGNO_SHOW.md`:
  CMakeLists (4 file), game.h/init/update/render (sostituisce `(void)show;`),
  hook renderer (`disegnaSpriteSchermo` + scatola/testo fumetto); verifica
  possibile anche oggi con soli `fb().fillRect`, zero asset.
- RILIEVI: (a) scrittura negata in `motore_nuovo/docs/` e in
  `motore_nuovo/dati/*.md` -> SPEC_GAMEPLAY.md §9 non aggiunta: testo pronto
  nel §4 di DISEGNO_SHOW.md, da incollare da @capo; il documento sta in
  `src/gameplay/DISEGNO_SHOW.md` e si puo' spostare in docs/; (b)
  quota_open/check_catalogo non eseguibili (shell negata): da rilanciare a
  @debugger/@reviewer.
- File toccati: `motore_nuovo/src/gameplay/{disegno_show.h, disegno_show.cpp,
  testi_frasi.h, testi_frasi.cpp, DISEGNO_SHOW.md, manifest.json}`.
- Atteso: collegamento da @coder; `-fsyntax-only` sui due nuovi .cpp da
  @debugger (voce 74).
---
