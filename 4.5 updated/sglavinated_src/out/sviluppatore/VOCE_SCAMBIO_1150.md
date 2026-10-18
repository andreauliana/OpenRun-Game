# Voce scambio @sviluppatore — 18/10 09:15

Stato blocchi: P1 [~], T1-RUN [~].

## Eseguito questo turno
Lettura stato: bacheche/sviluppatore/{RIASSUNTO,DIARIO_RECENTE}.md, FEEDBACK_ANDREA.md (precedenza assoluta), AGENTS.md, RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md.
Eseguito `sh tools/gate_t1.sh` (exit 0): `GATE T1: TUTTO OK` su lv_01_neon, lv_02_atomico, lv_03_foglie.
- speed@60=161.79; accelerazione 0-10s progressiva fino a 246.66; frenata OK; sterzo OK (sin=-218.3 dx=218.3); curve OK (salto pendenza max 0.50); ribaltamento OK; avvolgimento playerZ OK; inattivita show OK (bolla 4655 px, soglia 100); replay 2x IDENTICO; ms/frame medio ~0.36 max 0.651 (limite 20); offroad raggiunto.
Log: out/collaudatore/gate_t1.log. Build cmake OK (target prova_gate compilato in build_gate/).

## Guida (feedback Andrea punti 1-3)
Coerente con i sorgenti: accelerazione che cala verso MAX_SPEED (ACCEL_BASE + ACCEL_BOOST*exp(-speed/ACCEL_BOOST_SPEED) * (1-speed/MAX_SPEED)), freno secondario B/Down, coast 0.995 al rilascio, sterzo dipendente velocita', spinta esterna in curva, rallentamento offroad OFFROAD_SLOW 0.99, ribaltamento per uscita/collisione, drawHud pannello info velocita/marcia/offroad.

## Blocco in corso (PASSAGGIO)
PASSAGGIO: @sviluppatore -> @sviluppatore (prossimo turno) | screenshot P1 in out/sviluppatore/ | shell negato 6 volte con errore esatto `Permission denied: shell` su `./build_gate/prova_gate out/sviluppatore/gate lv_01_neon smoke` (comando semplice, nessun pipe/composto); nessun file copiato, nessun screenshot generato. Il binary prova_gate e' gia' costruito in build_gate/ e accetta outDir come argv[1]: basta shell per lanciare 3 run (lv_01/lv_02/lv_03, modo smoke) e copiare partenza/curva/fuoristrada.

## Mancante per chiudere P1
3 screenshot in out/sviluppatore/ (pista curva, cartelloni sponsor, personaggio con fumetto). Nota: prova_gate non salva PPM in modo inattivita; il gate log verifica gia' bolla+personaggio sopra soglia (modo inattivita OK).

## Pendente
v91 di Andrea: prova sglavinated_runner su Mac + giudizio guida (punto 8); fps/temperatura Pi 400.

## Prossimo
Riprovare shell -> 3 screenshot in out/sviluppatore/ -> RIASSUNTO.md e diario.md con esito misurato.
