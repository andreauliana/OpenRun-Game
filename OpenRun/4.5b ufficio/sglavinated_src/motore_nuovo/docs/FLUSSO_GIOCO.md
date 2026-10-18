# Flusso di gioco — proposta (18 ott, da confermare con Andrea)

Struttura come in OutRun (solo la struttura, contenuti nostri). La gara a tappe
con bivi esiste già nel motore (`Game::avviaGara`, `dati/percorso_gara.json`);
le altre schermate sono da fare.

```
INTRO (attract) -> TITOLO / MENU -> GRIGLIA 3-2-1 -> TAPPA 1 -> bivio -> TAPPA 2 -> bivio -> TAPPA 3 (arrivo)
                                                                                         -> FINALE della tappa d'arrivo
                                                                                         -> RISULTATI -> TITOLO
tempo scaduto in qualsiasi tappa -> GAME OVER -> RISULTATI -> TITOLO
```

## 1. Intro (attract mode)

- Parte dopo l'avvio e dopo 30 s di inattività sul titolo.
- Una gara dimostrativa guidata dal computer (la stessa guida automatica dei test), con logo e scritta "PREMI START".
- Gianni e Gesù appaiono con una frase a caso dai fumetti.

## 2. Titolo e menu

- Titolo: `ui.titolo` (approvato) con il font `ui.font`.
- Voci: **Gioca**, **Auto** (le 3 auto approvate: argento lunare, neon cyan, rosso corsa), **Radio** (brano da `dati/audio/playlist.json`, come la radio di OutRun), **Opzioni** (schermo intero, volume), **Esci**.
- Menu: `ui.menu` è ancora in costruzione.

## 3. Gioco

- **Griglia di partenza:** 7 avversari davanti, conto alla rovescia 3-2-1 (fatto: griglia e avversari).
- **Tappa:** circa un minuto di percorso (rettilinei, curve, colline diversi per pista). Fatto.
- **Tempo limite:** ogni tappa usa `tempo_limite_s` di `livello.json`; il tempo avanza a ogni nuova tappa (checkpoint). Da fare.
- **Bivio a fine tappa:** sinistra o destra porta alla tappa dopo (piramide 1 → 2 → 3). Fatto.
- **HUD:** velocità, marcia, posizione (quadratini), tappa, tempo rimasto (da fare).
- **Show:** Gianni/Gesù come oggi (crash, uscita di strada, inattività, periodico).

## 4. Finali

Tre arrivi possibili, uno per pista d'arrivo, ognuno con la sua scena a tema:

| Arrivo | Scena di fine (proposta) |
|---|---|
| lv_04_cromo | città futurista, robot che premiano l'auto |
| lv_05_spray | festa punk tra i muri a spray |
| lv_06_desktop | il desktop si "spegne" e riparte con l'auto vincitrice |

La scena cambia anche con la posizione d'arrivo (primo, sul podio, fuori dal podio), come chiede DIREZIONE_ARTISTICA ("reward grafico per ogni casistica"). Asset da disegnare (catalogo: da aggiungere come `mancante`).

## 5. Risultati

- Posizione d'arrivo, tempo totale, tempo per tappa, percorso scelto (le tre tappe).
- Classifica dei 10 migliori tempi con 3 iniziali (salvata su file accanto ai dati).
- Poi si torna al titolo.

## Decisioni aperte per Andrea

1. Avversari in gara (fatto ora, "gara con diversi altri guidatori") oppure traffico da sorpassare come l'OutRun originale, oppure entrambi?
2. Tempo limite con game over: sì o no?
3. Tre finali (uno per arrivo) e varianti per posizione: va bene?
4. Classifica: per tempo o per posizione + tempo?
