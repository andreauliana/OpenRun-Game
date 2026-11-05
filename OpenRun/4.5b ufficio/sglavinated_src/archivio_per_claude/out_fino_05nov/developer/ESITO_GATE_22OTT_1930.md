# Esito gate T1 — 22/10/2026 19:30 — @developer

Comando: `sh UFFICIO/Officina/TOOL/gate_t1.sh`
Exit code: 0
Esito: GATE T1: TUTTO OK

Misure:
- build sglavinated_core + prova_gate: OK (compilazione pulita)
- 6/6 percorsi: GATE PERCORSO OK (rettilineo 55-71%, curva max 0.72-0.98, fuori strada 0%, giro guidato 30-40 s)
- frequenza 60 Hz: GATE OK; frequenza 50 Hz: GATE OK (stessi secondi, replay IDENTICO)
- ms/frame: medio 0.16-0.275, max < 20
- replay 2x: IDENTICO a 60 Hz e 50 Hz
- accelerazione OK, ribaltamento OK, sterzo OK, curve continue OK, gara 4 combinazioni OK, screenshot P1 OK

Codice modificato (questa turno):
- motore_nuovo/src/render/renderer.h: aggiunto clearSponsorSprites()
- motore_nuovo/src/game/game.cpp: setSponsorSprites() chiama clearSponsorSprites() prima di registrare
  (risolve puntatori SpriteInstance.sprite appesi al vettore sponsorSprites_ del renderer:
  contaminazione tra test eseguiti nello stesso processo)

Blocchi aperti: T1-RUN [~] (Andrea D13=B runner Mac v91 pendente, non si tocca src/runner/),
P1 [~] sprite veri (@grafico G1), H [~] Pi 400 dopo ok Mac.
