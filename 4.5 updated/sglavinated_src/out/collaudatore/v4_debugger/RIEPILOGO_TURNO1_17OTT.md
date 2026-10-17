# Riepiologo Turno 1 (17/10/2026 09:18) — @debugger

## Causa probabile
Il gate determinismo/replay (regola 5b) e' debole: `Game::frameHash()` (motore_nuovo/src/game/game.cpp:349-362) hashs framebuffer + frame + playerSpeed, ma non include playerX, playerZ, playerOffroad ne' lo stato RNG (`state_.seed`/`rng_`). Due esecuzioni con stessi input che divergano di posizione possono collidere sullo stesso hash -> falsi negativi/positivi nel test "stesso file di input due volte -> stesso hash ogni 60 frame".
Conferma statica: reviewer rilievo #3 (voce 86 SCAMBIO.md) ancora aperto. Rilievo #1 offroad (OFFROAD_LIMIT=700 < X_LIMIT=900, un calcolo prima del clamp, player.h:33/player.cpp:25) e #2 wiring show (init+update gia' fatti, game.cpp:96-102/305; manca solo disegno scena in render, non indispensabile al Gate) gia' in codice. zero-alloc OK per Gate T1 (trafficoAttivo_=false, replay.record capped a 4096, replay.cpp:20).
Shell negata (regola 21): build cmake, replay 2x hash, ms/frame, fps, temperatura NON eseguibili qui -> Pi 400 con toolchain Andrea (voce 79 SCAMBIO).

## Prova (comando + numero)
Comando da eseguire su Pi 400/Mac con toolchain (D9 DEFAULT: solo cmake --build):
```
cd motore_nuovo && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j$(nproc)
./build/sglavinated_runner --record input.vec --frames 600   # esecuzione 1
./build/sglavinated_runner --playback input.vec              # esecuzione 2 (stesso input)
# confrontare frameHash ogni 60 frame: identici su entrambe
# ms/frame su >=600 frame: media, p99, max; conta frame > 20 ms
```
Numero atteso: hash identici su entrambe le esecuzioni ogni 60 frame; ms/frame <= 20 ms (obiettivo <=12 ms); fps 50 stabili; zero frame > 20 ms.

## Correzione proposta (diff) — NON applicata, spetta a @coder
```
diff --git a/motore_nuovo/src/game/game.cpp b/motore_nuovo/src/game/game.cpp
--- a/motore_nuovo/src/game/game.cpp
+++ b/motore_nuovo/src/game/game.cpp
@@ -349,11 +349,19 @@ u32 Game::frameHash() const {
     u32 h = 0x811c9dc5u; // FNV offset
     const u32* d = fb_.data();
     i32 n = Framebuffer::WIDTH * Framebuffer::HEIGHT;
     for (i32 i = 0; i < n; ++i) {
         h ^= d[i];
         h *= 0x01000193u;
     }
     h ^= state_.frame;
     h ^= (u32)state_.playerSpeed;
+    h ^= (u32)state_.playerX;
+    h ^= (u32)state_.playerZ;
+    h ^= state_.playerOffroad ? 1u : 0u;
+    h ^= state_.seed;
     h *= 0x01000193u;
     return h;
 }
```
Nota: allineare il log del runner a ogni 60 frame (ARCHITETTURA_CHECK.md §3 segnala 300) da verificare su Pi 400.

## Stato
Blocco H [~] in attesa Pi 400 (Andrea). Nessun file di sorgente toccato.
