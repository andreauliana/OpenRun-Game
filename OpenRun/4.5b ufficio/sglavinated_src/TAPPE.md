# Tappe del progetto (una alla volta, ognuna con il suo controllo)

Regola: si lavora solo sulla tappa del percorso di gara. Una tappa si chiude quando passa il suo **gate**; poi review, poi interventi di Andrea, poi la tappa dopo.

| # | Tappa | Gate (quando è chiusa) | Debug |
|---|---|---|---|
| **1** | **Il gioco parte, si visualizza il payer che si muove, si pilota** | strada che scorre, auto che accelera (A), frena (B), sterza (sinistra/destra) | build pulita, nessun crash in 5 minuti |
| 2 | Input completi | pad e tastiera rispondono a tutti i tasti; Start mette in pausa | test di ogni tasto |
| 3 | Menu e interfaccia | logo → splash → menu (Start, Opzioni) → gara; HUD con velocità e tempo | navigazione avanti e indietro senza blocchi |
| 4 | Auto e strada pseudo-3D | sprite dell'auto con 3 pose (dritta, sinistra, destra); curve e salite | fps stabili, nessun sprite che sparisce |
| 5 | Gara con altri veicoli | traffico da sorpassare e 2-3 rivali che corrono la gara; collisioni; oggetti a bordo strada | collisioni, esperienza di corsa automobilistica |
| 6 | Contenuti | statistiche giocatore, foglia "Gianni" con fumetti, radio | Ganja, testi e tempi di comparsa |
| 7 | Rifinitura | grafica di Andrea (`ANDREA/`), musica, bilanciamento | test completo su Pi 400 e architettura multipiattaforma |

**Tappa attiva: 1.**

Dopo ogni gate:
1. Review (@developer-junior o Claude, breve).
2. Andrea prova e corregge a mano, se serve.
3. Si passa alla tappa successiva in `RICHIESTA.md`.

## Funzioni del genere: dove sono le due versioni
Stato: **ok** fatto · **logica** calcolata ma non a schermo · **parziale** · **—** da fare

| Funzione | Tappa | Really Open (C++) | *Enhanced ROM (.md) — sospesa*|
|---|---|---|---|
| Guida: accelera, frena, sterza | 1 | ok | — |
| Strada che scorre, strisce, curve | 1 / 4 | parziale (dritta) | — |
| Pausa (Start), tutti i tasti | 2 | — | — |
| Titolo, splash, menu, opzioni | 3 | — | — |
| Scelta della radio, cambio brano | 3 | parziale (tasto R) | — |
| HUD: velocità, tempo, punteggio | 3 | — | — |
| Auto pseudo-3D con pose di sterzata | 4 | — | — |
| Cartelloni e oggetti a bordo strada | 4 / 5 | parziale (solo cartelloni) | — |
| Traffico e rivali, collisioni, testacoda | 5 | — | — |
| Razzo (X / Spazio) | 5 | — | — |
| Tempo con checkpoint e tempo extra | 5 | — | — |
| Bivi e percorso ad albero (più tappe) | 5 | — | — |
| Gianni, Gesù, fumetti, Diere, UFO | 6 | logica | — |
| Finali diversi a seconda del percorso | 6 | — | — |
| Classifica e inserimento nome | 6 | — | — |
| Grafica e musica di Andrea | 7 | — | — |

Regola: una funzione nasce nel C++, Andrea la approva, poi passa alla ROM con gli stessi dati (`livello.json`, frasi, PNG convertiti).
