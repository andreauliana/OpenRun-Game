# Cosa si CONSERVA dalla hack v87 (non si butta niente)
Regola: la v87 in questa cartella e' sola lettura. Si porta il lavoro nel motore nuovo, non lo si rifa a caso.
| Cosa | Dove (v87) | Dove va nel v3 | Come |
|---|---|---|---|
| Testi: 156 fumetti, tabelle show (otab, xtab, g1t-j3t) | tools/gianni12.py liste PHR, OPENCODE_REFERENCE_v74.json | dati/personaggi/*/fumetti.json | @traduttore estrae in JSON con script Python |
| Logica degli show (quando parla chi, escalation, inattivita', priorita' crash/uscita) | SPEC_personaggi_v74.md (par. 5-13), patch_z.py | src/personaggi/ | @grafico descrive in .md, @developer/@developer-junior riscrive in C++ |
| Arte Gianni, Gesu', UFO, donna, titolo, finale | shots/*.bin | dati/personaggi/*/ | convertire in PNG con script Python; il disegno resta di Andrea |
| Musica propria (radio tracker) | pcmr_*.bin, tools/mkradio2.py, file .xm di Andrea | dati/audio/ | musica = RIFERIMENTO, puo' cambiare: si conserva ma playlist.json la rende sostituibile |
| Strumenti di build e controllo | tools/*.py, build_all.sh, check_vars.py | motore_nuovo/strumenti_py/ | riusare lo schema: pipeline unica, controlli che fanno fallire la build |
| Test (vsbad/wrong/repeat/leak, replay) | lr/*.py | tests del nuovo motore | stessa idea: replay deterministico |
| Lezioni e limiti | BUILD.md, HANDOVER.md, V85C_NOTE.md | docs/ | leggerli prima di decidere |
Ogni elemento portato diventa riga in CATALOGO_PERSONAGGI.md. Cio' che non e' qui sopra o nel catalogo va segnalato a Andrea prima di scartarlo.
