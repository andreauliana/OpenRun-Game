# Riepilogo mattina — OpenRun 4.6 (31/10/2026)

## Fatto per blocco / agente
- @developer: R2 passo 1d (nome 3 lettere, gate ciclo) fatto; DECO1 2e parallasse iniziata; codice test e diagnostica in motore_nuovo/.
- @developer-junior: review CP-005 positiva; gate decorazioni 6/6 livelli OK; ctest 21/21 verde gate decorazioni.
- @grafico: G1 catalogo 181 elementi, PNG non catalogati 0, CC0-1.0; DECO1 2f fatto (6 oggetti alti + manifest); asset Shared asset/ verificati e catalogo aggiornato.
- @capo: CP-006 APERTO (D19=B); STATO_CORRENTE e REGISTRO_REVISIONI aggiornati; checkpoint registrati.

## Non funziona / blocchi
- T1-RUN [~]: D19 = B, Andrea risposta B, chiusura attesa; CP-006 APERTO, DEFAULT non applicabile.
- libretro.h manca: BUILD_LIBRETRO=ON non compila per Pi 400/RetroPie.
- CP-002 (G1) e CP F60: review indipendente mancante; F60 attende review developer→developer-junior.
- runner Mac in attesa conferma Andrea (D12/D14 = A); nessun lavoro dedicato Pi 400/RetroPie prima di ok formale.
- 60 Hz opzione: da configurare dopo R1 (Andrea 19 ott).

## 3 decisioni aperte in ANDREA_REVIEW.md (più importanti)
1. D19 [APERTA]: T1-RUN chiusura dopo prova Mac 23/10 — RISPOSTA B, T1-RUN resta [~], CP-006 aperto, nessun lavoro Pi 400/RetroPie prima di ok Mac formale.
2. T1-Q3: giro guidato — rivedere parametri percorsi (varietà curve/rettilinei, soglie non abbassate, ref. lv_02_atomico); OK gate interni 19-21/10, attende conferma Andrea.
3. Ordine test post-R1 (D12/D14 = A): ri-verifica runner Mac v91 di Andrea prima di Pi 400/RetroPie e 60 Hz/R2; resta in attesa finché Andrea non conferma.

## Prossimo passo consigliato
- @developer: procedere R2 passo 1d, poi DECO1 2e (parallasse); evitare toccare src/runner/ e nuove dipendenze.
- @grafico: attendere per RIV/FIN; fornire asset DECO1 2e quando richiesto; catalogo G1 confermato.
- @capo: chiudere CP-006 solo su decisione Andrea su D19; poi aprire R2 con conferma formale.
