# Riepilogo mattina — OpenRun 4.6 (06/11/2026, Turno 6, ora 10 di 12)

## Fatto per blocco / agente
- @developer: R1 tracciati chiuso (CP-005 CHIUSO, review incrociata positiva, ctest 17/17 verde). FORK1 chiuso (CP-007 CHIUSO, ctest 20/20 verde). DECO1 passi 2a-2d fatti (gate decorazioni 6/6 livelli OK, ctest 21/21 verde); 2e parallasse prossimo. R2 passi 1/1b/1c fatti (ctest 22/22 verde gate ciclo); 1d in corso. F60 gate verde 60/50 Hz registrato (manca review incrociata). PIST1: G-P1 [x] (handoff grafico 05/11 20:46, parapetto.png 32x48 + guardrail.png 48x32, raw, 2 voci pronto); prossimo P3 parapetti su segmenti acqua.
- @developer-junior: review incrociata CP-005 e FORK1 positive; gate decorazioni OK; gate F60 fatto (manca review developer→junior e CP F60); PIST1 J2/J1 pendenti (gate tratti); candidato review indipendente CP-002.
- @grafico: G1 [x] (189 elementi, 0 non catalogati, CC0-1.0, check_catalogo_grafico.py exit 0); DECO1 2f FATTO (6 oggetti alti, manifest aggiornati); RIV e FIN in attesa integrazione motore.
- @capo: STATO_CORRENTE.md, STATI_PROPOSTI.md e REGISTRO_REVISIONI.md aggiornati (Turno 6). Nessun checkpoint chiuso questo turno; CP-006 APERTO (D19 = B).

## Non funziona / blocchi
- T1-RUN [~] (D19 = B): CP-006 APERTO, attende nuova prova/decisione Andrea su runner Mac; nessun lavoro Pi 400/RetroPie prima di ok Mac formale.
- DECO1 [~]: passo 2e (parallasse) prossimo; manca review incrociata + CP DECO1 (CP-008 da aprire).
- F60: gate verde ma manca review incrociata developer→developer-junior e apertura CP F60.
- CP-002: G1 [x] per handoff ma review indipendente da assegnare (revisore diverso da @grafico).
- libretro.h manca: BUILD_LIBRETRO=ON non compila; runner RetroPie da preparare.

## 3 decisioni aperte in ANDREA_REVIEW.md
1. D19 [APERTA]: chiusura T1-RUN — Andrea ha risposto B; T1-RUN [~], CP-006 aperto, attende decisione Andrea (DEFAULT non applicabile).
2. T1-Q3 [APERTA]: giro guidato fallito su 5/6 livelli (solo lv_02_atomico OK); @developer rivede parametri percorsi (varietà curve/rettilinei, soglie non abbassate); attende conferma Andrea.
3. Ordine test post-R1 (D12/D14 = A): ri-verifica runner Mac v91 prima di Pi 400/RetroPie e 60 Hz/R2; in attesa di conferma Andrea.

## Prossimo passo consigliato
@developer procede con PIST1 P3 (parapetti oggetti ai due lati su segmenti `acqua`), poi `sh UFFICIO/Officina/TOOL/gate_t1.sh`; poi @developer-junior J2 e J1 (gate `tratti`). DECO1: review incrociata + CP-008. @capo: CP-002 review indipendente G1; chiude CP-006 solo su decisione Andrea su D19.
