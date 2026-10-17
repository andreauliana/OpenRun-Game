# Catalogo personaggi e asset (da tenere aggiornato)
Stato: `hack` = esiste solo nella v87 Mega Drive, da rifare originale; `nuovo` = in dati/personaggi/; `idea` = da creare.
Fonte per la hack: ../SPEC_personaggi_v74.md (v85c), BUILD.md, shots/.

| id | Nome | Ruolo | Come e' nella hack v87 | Stato v3 |
|---|---|---|---|---|
| gianni | Gianni (Spinellato/Spinellutti) | amico che tenta, fumetti dopo errori off-road | 8 pose x 4 pezzi 4x4 tile (g_frames.bin); fumetti | nuovo (dati/personaggi/gianni/ + manifest.json CC-BY-4.0, crediti Andrea Uliana); fumetti: testi portati = nuovo, grafica da rifare = idea |
| gianni_figlia | frame alternativi (DS0/DS1) | variante animazione | g_dfr0/1.bin, palette DPAL | hack |
| gesu | Gesu' (Sacro Cuore) | corregge, apparizione rispettosa dopo crash | 4 frame (APP+0x900) | nuovo (dati/personaggi/gesu/ + manifest.json CC-BY-4.0, crediti Andrea Uliana); fumetti: testi portati = nuovo, grafica da rifare = idea |
| ufo | UFO | apparizione | 2 frame (g_app4.bin) | hack |
| donna | figura «woman» | apparizione | g_woman.bin | hack (da chiarire) |
| fumetti | 156 frasi | testi di Gianni/Gesu' (sicurezza stradale) | 1152 B l'una, 88 px di riga | nuovo: testi portati = nuovo (dati/frasi.json CC-BY-4.0 + dati/personaggi/*/fumetti.json); grafica da rifare = idea |
| show_regole | logica show (trigger, cooldown, escalation, inattivita') | quando parla chi, tabelle frase | otab/xtab/ph_tab/jtab/itab, g1t-g3t, j1t-j3t, pr_tab | nuovo (dati/show_logica.md) |
| titolo/finale | schermate | titolo SGLAVINATED, fine | g_title.bin, g_end.bin | hack |
| radio | 3 radio tracker | musica propria dell'autore | pcmr_M/A.bin, pcmn_d/f.bin | hack: musica di riferimento, sostituibile |
| font_ui | font pixel + lettering titolo | grafica UI | titolo SGLAVINATED (TSRC) | idea |
| menu_hud | menu principale, HUD | UI come dati | HUD accelerometro (hud/mkhud.py) | idea |
| razzo_B | razzo sul tasto B | modulo fx | non esiste | idea |
| auto_sollevata | auto che si alza ad alta velocita' | modulo fx | non esiste (v88 in lavorazione) | idea |
| pubblico/scenari | scenari fantascienza lofi | sfondi per tappa | non esiste | idea |

Regola: ogni riga nuova ha cartella, manifest e crediti. Se un personaggio non e' qui, per il progetto non esiste.
