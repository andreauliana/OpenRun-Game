# Sterzo di riferimento (OutRun Mega Drive, 1991): cosa fa, in numeri

Studio solo in lettura, 18 nov. Nessun codice, dato o grafica Sega va nel repo OpenRun: qui ci sono solo comportamenti e numeri, da reimplementare a modo nostro.

**Come è stato fatto.** rea per il 68000 richiede Ghidra, che qui non c'è. Ho letto il codice della cartuccia con un disassemblatore (capstone). Non ho potuto avviare un emulatore, quindi i numeri sono letti dal codice e non misurati giocando. Unità: "u" = unità laterali del gioco. La carreggiata tra i due limiti duri è larga 512 u. Non so se la logica gira a 60 o a 30 aggiornamenti al secondo, quindi do i valori "per aggiornamento".

## 1. Il volante (posizione dello sterzo)
- 8 scatti per lato più il centro.
- Premendo, avanza di 2 passi per aggiornamento; rilasciando, torna al centro di 1 solo passo. **Il ritorno è lento la metà della pressione** (in OpenRun è il contrario: 0,125 s per sterzare, 0,08 s per tornare).
- Un'opzione del menu cambia la rapidità: sterzo pieno in 1, 2 o 4 aggiornamenti; ritorno in 2, 4 o 8.
- Spostamento laterale per scatto (non lineare): 1→2, 2→4, 3→4, 4→8, 5→8, 6→16, 7→16, 8→24 u. I tocchi brevi muovono pochissimo, lo sterzo pieno dà uno scatto deciso.

## 2. Sterzo e velocità
- Velocità interna = km/h, punta 293.
- Tetto allo spostamento laterale totale: <20 km/h 4 u, <40: 8, <60: 12, <80: 16, <120: 20, da 120 km/h in su 32 u (24 di sterzo + spinta della curva).
- **Sopra i 120 km/h la presa non cala mai, nemmeno alla punta.** Da fermi lo sterzo torna al centro e l'auto non si sposta.

## 3. Curve (spinta verso l'esterno)
- La velocità viene divisa in fasce di 20 km/h (fascia 1..15).
- In curva la spinta verso l'esterno dipende dalla **differenza tra fascia di velocità e valore della curva**, su 17 gradini: 4, 8, 12, 16, 18, 20, 22, 24 (pareggio), 26, 26, 28, 30, 32, 34, 36, 40 u. C'è un piccolo tremolio di ±2 u che alterna i fotogrammi.
- Spinta e sterzo si sommano subito, senza inerzia, con un tetto di ±32 u.
- **Regola chiave:** al pareggio la spinta è 24, cioè proprio quanto lo sterzo pieno. Allo sterzo pieno l'auto tiene la linea; un gradino più veloce e scivola fuori anche con lo sterzo a fondo; più lento e taglia dentro.
- Dubbio aperto: dal solo codice non sono riuscito a capire se un valore di curva alto significa curva più stretta o più larga.

## 4. Stridio / sbandata
- Stridio delle gomme e "flag sbandata" quando valgono insieme tre condizioni: sterzo a fondo, velocità ≥ 240 km/h e fascia di velocità oltre il valore della curva.
- In questa routine lo stridio non toglie velocità.
- Stesso suono alla partenza col gas sotto i 90 km/h (pattinamento).

## 5. Bordi e fuori strada
- Due zone vicino al limite duro di ogni lato: a meno di 79 u (circa il 15% della carreggiata) e a meno di 47 u (circa il 9%).
- Zona esterna: la velocità scende di 2 km/h per aggiornamento finché supera 210 km/h.
- Zona profonda: la velocità scende di 3 km/h per aggiornamento finché supera 88 km/h.
- In entrambe si sente il suono del bordo, diverso a sinistra e a destra.
- Al limite duro l'auto si ferma, senza rimbalzo.
- Durante un incidente/testacoda c'è uno spostamento laterale fisso di 24 u per aggiornamento verso un lato.
- Gli urti laterali col traffico sono gestiti altrove: non li ho studiati.

## Confronto con OpenRun (main del 18 nov)
| | Riferimento | OpenRun |
|---|---|---|
| Presa piena | da 120 km/h (41% della punta) | dal 25% della punta, -5% alla punta |
| Ritorno del volante | più lento della pressione | più veloce della pressione |
| Risposta ai tocchi | non lineare (tocco = quasi niente) | lineare |
| Spinta curva | a gradini, fascia − curva, pareggio = sterzo pieno | continua, curva × v² |
| Peso/inerzia | nessuno, immediato | 0,03–0,45 s (scelta di Andrea, 11/11) |
| Tempo per attraversare la strada a sterzo pieno | ~0,35 s (60 Hz) o ~0,7 s (30 Hz) | ~1,7 s |
| Fuori strada | due zone, tetti 210 e 88 km/h | bordo (88% della punta) + erba che frena |
| Derapata | solo suono + flag | sovrasterzo, controsterzo, testacoda |

## Idee da valutare (da scrivere da zero, se Andrea vuole)
1. **Regola del pareggio:** tarare la spinta in modo che, a una certa velocità per ogni curva, lo sterzo pieno tenga esattamente la linea. È una regola chiara per chi progetta le piste.
2. Ritorno del volante più lento della pressione: aiuta a "tenere" la curva a colpetti.
3. Risposta non lineare: tocchi piccoli per correggere, sterzo pieno deciso.
4. Spostamento laterale più rapido rispetto alla larghezza della strada: il riferimento è 2,5–5 volte più rapido di OpenRun.
5. Fuori strada a due zone con tetti di velocità, invece di un freno percentuale.
