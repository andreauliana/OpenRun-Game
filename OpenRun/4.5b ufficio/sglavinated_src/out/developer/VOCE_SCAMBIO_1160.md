# Voce scambio @developer — 18/10 22:56 (turno 1)

Stato blocchi: P1 [~], T1-RUN [x] (DEFAULT T1-Q1 ANDREA_REVIEW: sh tools/gate_t1.sh soak exit 0, replay 2x IDENTICO, ms/frame <= 20; risposta Andrea non registrata).

Fatto questo turno:
- Eseguito `sh tools/gate_t1.sh` (smoke): exit 0, GATE T1: TUTTO OK. 6 livelli lv_01_neon..lv_06_desktop: prova_gate e percorso GATE PERCORSO OK; ms/frame medio 0.177-0.184 max 0.263-0.366 (limite 20); replay 2x IDENTICO; speed@60=161.79.
- Eseguito `sh tools/gate_t1.sh soak`: exit 0, GATE T1: TUTTO OK; soak 30000 frame lv_01_neon ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO.
- Verifiche: accelerazione 0-10 s OK (primo frame 9.90, 1.2 s 160.83, 10 s 246.66); ribaltamento e ripartenza OK; sterzo OK (sinistra -218.3, destra +218.3); continuita' curve OK (salto pendenza max 0.02 su 1199 campioni); gara 4 bivi OK (arrivo su tutte le combinazioni, replay IDENTICO); screenshot P1 OK (curva frame 2596, cartelloni sponsor frame 884, personaggio+fumetto frame 372) in UFFICIO/Officina/Test area/screenshot_p1/.
- Aggiornato UFFICIO/Officina/STATUS_OFFICINA.md con report turno 22:56 (comando, exit code, misure, link al log).
- Nessun file di prodotto modificato (zero scritture in motore_nuovo/ e tools/); nessun test del Junior toccato.

Consegna: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md (<=12 righe), questa voce in out/developer/, STATUS_OFFICINA.md aggiornato.
Log originale (non duplicato qui): UFFICIO/Officina/Test area/gate_t1.log.

Prossimo: attendere v91 Andrea (runner sglavinated_runner su Mac SDL2) e misure fps/temperatura Pi 400 per chiudere P1 e H (ANDREA_REVIEW T1-Q3/T1-Q8); G1 [ ] resta fuori scope T1 (DEFAULT T1-Q4: non attivare).
