# Esito FORK1 punto 7 — gate `tema_transizione` — 25/10/2026 07:47

## Blocco
FORK1 [ ] — bivio e transizione tragitto fluidi e graduali, transizione di tema, guida safe per bordi strada durante bivio. Punto 7 (@developer-junior): gate `tema_transizione`, da scrivere e far passare con il codice FORK1 già presente (Renderer::iniziaTransizioneTema/aggiornaTransizioneTema e Game::fineTappa, integrati 23/10; game.cpp non toccato da @developer oggi).

## Modifica
- `motore_nuovo/test/prova_gate.cpp`: nuovo modo `tema_transizione` (exit code 17). Guida la gara su `lv_01_neon` fino al primo cambio tappa (sterzo a sinistra al bivio -> `lv_02_atomico`), campiona il cielo del framebuffer (riga 8 < HORIZON_Y=112) a ogni frame e verifica:
  1. cambio tappa raggiunto;
  2. variazione per canale RGB tra frame consecutivi <= soglia (graduale, niente scatto istantaneo al nuovo tema);
  3. cielo finale diverso da quello iniziale (vero cambio di tema);
  4. transizione completata entro `transizione_tema_s` con tolleranza e in piu' di un frame.
  Salva screenshot PPM (prima/cambio/dopo/fine) in `UFFICIO/Officina/Test area/tema_transizione/`.
- `motore_nuovo/dati/tracciati/regole.json`: blocco `tema_transizione` con `delta_canale_max_per_frame: 12` (soglia letta dal JSON, niente hardcoded).
- Durata e frequenza lette da `motore_nuovo/dati/config.json` (`transizione_tema_s` default 2.5 s, `frequenza_hz` default 60).

## Verifica
- Comando: `cmake --build UFFICIO/Officina/build --target prova_gate` — exit 0.
- Comando: `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/tema_transizione" lv_01_neon tema_transizione` (lanciato da `sglavinated_src/`) — exit 0.
- Esito: `tema_transizione lv_01_neon -> lv_02_atomico: cambio frame 2232 | cielo 0x202040 -> 0xF2B8C6 | maxDeltaCanale=4.0 (soglia 12) | transizione 91 frame (1.52 s misurata, attesi ~60 frame = 1.0 s, limite 130) | OK`.
- `GATE TEMA TRANSIZIONE OK`. Screenshot PPM: `p_tema_transizione_prima2.ppm`, `prima1.ppm`, `cambio.ppm`, `dopo1.ppm`, `dopo2.ppm`, `dopo10.ppm`, `fine.ppm`.
- Nessun file `src/` toccato. `game.cpp` non modificato (teleport al blocco scelta `game.cpp:790` ancora presente).

## Residui
- FORK1 punto 5 `bivio_bordi` [ ]: bloccato, attende i punti 1-3 di @developer su `game.cpp` (teleport al blocco scelta smussato e bordi). Senza quel fix il gate `bivio_uscita` (punto 4 [x], exit 16 atteso) non puo' passare.
- T1-RUN [~]: chiusura formale D19 [APERTA] in attesa decisione Andrea su ANDREA_REVIEW.md. DECO1 [ ], RIV [ ], R2 [ ] in coda dietro FORK1.
