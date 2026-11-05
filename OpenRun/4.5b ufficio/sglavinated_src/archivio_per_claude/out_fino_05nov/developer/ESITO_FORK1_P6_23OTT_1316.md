# Esito FORK1 punto 6 — transizione tema graduale — 23/10/2026 13:16

Blocco: FORK1 [~] (@developer). Punto 6: transizione graduale del tema grafico tra tappe (nessuno scatto di un frame al cambio tappa).

Modifiche:
- motore_nuovo/src/render/renderer.h: Renderer::iniziaTransizioneTema(da, durataSecondi), aggiornaTransizioneTema(dt), lerpColore(); stati temaDa_, transizioneT_, transizioneDurataS_.
- motore_nuovo/src/render/renderer.cpp: interpolazione canale per canale (0xAARRGGBB) di cielo/erba/erba_alt/rumbleAlt durante la transizione; drawRoad usa tema_ interpolato.
- motore_nuovo/src/game/game.h: membro transizioneTemaS_ = 1.0f.
- motore_nuovo/src/game/game.cpp: lettura transizione_tema_s da dati/config.json in init(); aggiornaTransizioneTema(dt) in update() ogni frame (secondi, frequenza indipendente); fineTappa cattura temaPrima prima di caricaLivello e avvia la transizione verso il tema della tappa nuova.
- motore_nuovo/dati/config.json: aggiunto "transizione_tema_s": 1.0.

Verifica:
- Comando: sh UFFICIO/Officina/TOOL/gate_t1.sh
- Exit code: 0
- Esito: GATE T1: TUTTO OK. 6/6 percorsi GATE PERCORSO OK (lv_01_neon, lv_02_atomico, lv_03_foglie, lv_04_cromo, lv_05_spray, lv_06_desktop), replay 2x IDENTICO, ms/frame medio ~0.17 max 0.336 (limite 20), gara 4 combinazioni OK con replay IDENTICO, frequenza 60 Hz e 50 Hz OK, screenshot P1 OK (curva, cartelloni sponsor, personaggio+fumetto).
- Log: UFFICIO/Officina/Test area/gate_t1.log

Stato residuo: gate Junior (bivio_uscita/bivio_bordi/tema_transizione) a carico di @developer-junior; DECO1 (@grafico).
