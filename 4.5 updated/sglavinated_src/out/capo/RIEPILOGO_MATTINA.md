# RIEPILOGO_MATTINA — 17 ott 2026 (max 40 righe)

## Fatto (questo turno)
- capo: STATI_PROPOSTI.md riscritto (T1-RUN [~], P1 [~], H [~], J [ ], M [~]); deleghe @sviluppatore punto 4 (.sglraw), @collaudatore punti 4b/5/6/7, Andrea punto 8 (v91); D10 DEFAULT B e D11 DEFAULT A in DECISIONI_ANDREA.md; nessun PER_CLAUDE.md; nessuna assemblea aperta; FINITO non creabile.
- @sviluppatore: `sh tools/gate_t1.sh` exit 0 su lv_01_neon, lv_02_atomico, lv_03_foglie; speed@60=33.76, ms/frame medio 0,076 (limite 20), replay 2x IDENTICO, offroad raggiunto, wrap + inattivita OK (out/collaudatore/gate_t1.log). P1 punti 1-3 fatti e verdi (guida OutRun, colline quota assoluta, wrap sprite). Aggiunti sprite_raw.h/.cpp (MIT, loader SGLR, zero dipendenze) e png_a_raw.py a CMakeLists.
- @collaudatore: `sh tools/gate_t1.sh soak` exit 0 (30000 frame), replay IDENTICO, ms/frame medio 0,071. H [~]: manca solo punto 8 di Andrea.

## Non funziona / ancora aperto
- Sprite veri nel gate headless: .sglraw non generati (python3 non eseguibile via shell qui) e prova_gate.cpp (di @collaudatore) non chiama ancora setSponsorSprites -> screenshot con rettangoli segnaposto; blocca chiusura T1-RUN/P1.
- v91 di Andrea pendente: runner SDL2 su Mac + fps/temperatura Pi 400 (vcgencmd measure_temp, get_throttled); chiude H e abilita promozione P1/H a [x].
- Fuoristrada: velocita' crolla ~160 -> ~6 quasi di colpo (da tarare, DEFAULT D11 A).

## Decisioni aperte in DECISIONI_ANDREA.md (solo 2 aperte)
- D10 | T1-RUN | capo | sprite/cartelloni veri nel gate: caricatore PNG minimo nel core vs conversione PNG->raw in strumenti_py. DEFAULT B (zero dipendenze). RISPOSTA DI ANDREA in attesa.
- D11 | T1-RUN | capo | fuoristrada: rallentamento graduale ~0.85/frame vs decelerazione secca vs nessuno. DEFAULT A (graduale). RISPOSTA DI ANDREA in attesa.

## Prossimo passo consigliato
- @sviluppatore sblocca generazione .sglraw (esegue python3 o fornisce i raw); poi @collaudatore aggiorna prova_gate con setSponsorSprites prima di init(), 3 screenshot out/debugger/ (partenza, curva, fuori strada) + log velocita/posizione, rilancia `sh tools/gate_t1.sh soak` (exit 0, replay IDENTICO). Poi attendere v91 di Andrea per chiudere H e promuovere P1/H.
