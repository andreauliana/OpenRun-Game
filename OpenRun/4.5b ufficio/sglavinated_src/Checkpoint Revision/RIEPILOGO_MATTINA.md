# Riepilogo mattina — OpenRun 4.6 (29/10/2026)

## Fatto per blocco / agente
- @developer: R1 tracciati concluso (road.h/road.cpp, tratti.json 8 tratti, regole.json, campo tracciato in 6 livello.json); gate soak 6/6 OK (rettilineo 52-71%, curva max <=1.0, fuori strada 0%, ms/frame <=20, replay 2x IDENTICO). FORK1 gate bivio_uscita/bordi/tema_transizione OK (CP-007 CHIUSO). DECO1 passi 2a-2d fatto (skyline lv_01, gate decorazioni OK). R2 passi 1/1b/1c fatti, ctest 22/22. F60 gate verde 60/50 Hz (22/10).
- @developer-junior: review CP-005 punti 4-5 positiva; review FORK1 gate 4/5/7 positiva; gate decorazioni OK 27/10; gate F60 fatto (manca review incrociata developer→junior e CP F60).
- @grafico: G1 chiuso (catalogo 181, PNG non catalogati 0, CC0-1.0); M UI/HUD variante B completo; DECO1 2f fatto (6 oggetti alti, manifest, raw); review screenshot DECO1 fatto. RIV e FIN in attesa integrazione motore + ok Andrea.
- @capo: CP-005 e CP-007 chiusi; CP-006 APERTO; STATO_CURRENTE aggiornato.

## Non funziona / blocchi
- T1-RUN resta [~] (D19 = B): CP-006 APERTO, attende nuova prova/decisione Andrea su runner Mac; nessun lavoro Pi 400/RetroPie dedicato prima di ok Mac.
- 60 Hz (dt in secondi, default 60, opzione 50): da fare dopo R1, prima di R2 (Andrea 19 ott).
- libretro.h manca: BUILD_LIBRETRO=ON non compila; runner RetroPie (OpenRun.sh, guida Pi) da preparare.
- CP-002 in attesa review indipendente; CP F60 da aprire.

## 3 decisioni aperte in ANDREA_REVIEW.md
1. D19 [APERTA]: chiusura T1-RUN — Andrea ha risposto B; T1-RUN [~], R2 non parte, CP-006 aperto.
2. T1-Q3: giro guidato — @developer rivede parametri percorsi (varietà curve/rettilinei, soglie non abbassate, ref. lv_02_atomico); OK nei gate interni 19-21/10, attende conferma Andrea.
3. Ordine test post-R1 (D12/D14 = A): ri-verifica runner Mac v91 di Andrea prima di Pi 400/RetroPie e 60 Hz/R2; resta in attesa finché Andrea non conferma (D16).

## Prossimo passo consigliato
@developer procede con R2 passo 1d, poi DECO1 2e (parallasse), senza toccare src/runner/ e senza nuove dipendenze. @grafico attende per RIV/FIN. @capo chiude CP-006 e apre R2 solo su decisione Andrea su D19.
