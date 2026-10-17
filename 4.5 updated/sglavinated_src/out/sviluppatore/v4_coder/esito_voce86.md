### 86. CAPO -> CODER | stato [CHIUSA] | Turno 3 (ora 4 di 12), 17 ott 2026

Ricevo CHECKPOINT 2 reviewer (righe 839-912). 3 rilievi:

1) **offroad** (player.cpp/player.h): verificato gia' applicato. player.h:33 = OFFROAD_LIMIT = 700.0f (< X_LIMIT=900). player.cpp:25 calcola offroad PRIMA del clamp (righe 30-31). Nessun duplicato presente. File non modificati.

2) **wiring disegnoShow_** (game.cpp): verificato parzialmente applicato. game.cpp:305 = `disegnoShow_.aggiorna(clock_.frame, showManager_)`. init() contiene testiFrasi_.carica/verifica/setTesti/layout. Aggiunto in render() dopo drawPlayerCar:
   ```cpp
   const gameplay::ScenaShow& sc = disegnoShow_.scena();
   if (sc.ufoVisibile) fb_.fillRect(sc.ufoX, sc.ufoY, sc.ufoX+64, sc.ufoY+24, 0xFFFF0000u);
   if (sc.personaggioVisibile) fb_.fillRect(sc.persX, sc.persY, sc.persX+sc.persW, sc.persY+sc.persH, 0xFF00FF00u);
   if (sc.bollaVisibile) fb_.fillRect(sc.bollaX, sc.bollaY, sc.bollaX+sc.bollaW, sc.bollaY+sc.bollaH, 0xFF0000FFu);
   ```
   Placeholder con rettangoli colorati; sprite veri quando @asset avra' creato gli originali.

3) **frameHash** (game.cpp:349-362): aggiunti in coda all'hash:
   ```cpp
   h ^= (u32)state_.playerX;
   h ^= (u32)state_.playerZ;
   h ^= state_.playerOffroad ? 1u : 0u;
   h ^= rng_.state();
   h *= 0x01000193u;
   ```
   Getter `u32 state() const { return state_; }` aggiunto a `core/rng.h` (riga 20, prima di private).

**File toccati:**
- `motore_nuovo/src/core/rng.h` (getter stato RNG)
- `motore_nuovo/src/game/game.cpp` (render() + frameHash())
- `motore_nuovo/src/player/player.cpp` (verifica: gia' corretto, non toccato)
- `motore_nuovo/src/player/player.h` (verifica: gia' corretto, non toccato)

**SPDX MIT:** presente riga 1 su tutti i file toccati (gia' presente, nessuna aggiunta necessaria).

**Build cmake:** NON eseguibile qui (regola 21). `-fsyntax-only` su `game.cpp` e `player.cpp` da eseguire da @debugger (controllo separato da voce 74/76).

**Vincoli rispettati:** solo motore_nuovo/src/; determinismo preservato (frame counter, RNG seme, nessun orologio, niente SDL nel core); zero alloc nel ciclo di gioco; niente asset Sega.

@capo: edit su SCAMBIO.md negato (regola 21). Copiare questa voce in fondo a SCAMBIO.md come voce 86 [CHIUSA].
