# Indice dei tracciati (08/11)

Andrea 08/11: i tracciati sono vari e vanno bene. **Legacy 08/11: non si toccano.** Solo aggiunte non distruttive (per esempio gli incroci a 90 gradi in citta', che non cambiano curve, colline ne' lunghezze).

- Pezzi di strada (tratti): `motore_nuovo/dati/tracciati/tratti.json` (curva, collina, lunghezza in segmenti).
- Ordine dei tratti per livello: campo `tracciato` in `motore_nuovo/dati/livelli/<livello>/livello.json`. Il motore ripete la sequenza finche' la tappa non e' lunga abbastanza.
- Paesaggio per tratto: campo `paesaggio` dello stesso file.
- Albero delle tappe (bivi): `motore_nuovo/dati/percorso_gara.json`.

## Albero delle tappe

```
lv_01_neon      -> sinistra lv_02_atomico, destra lv_03_foglie
lv_02_atomico   -> sinistra lv_04_cromo, destra lv_05_spray
lv_03_foglie    -> sinistra lv_05_spray, destra lv_06_desktop
lv_04_cromo     -> ARRIVO
lv_05_spray     -> ARRIVO
lv_06_desktop   -> ARRIVO
```

## lv_01_neon

autostrada di neon di notte: griglia di luce, skyline scuro e sole a righe all'orizzonte. Tempo limite 50 s, 14 tratti, 4540 segmenti per giro della sequenza.

| # | tratto | curva | collina | segmenti |
|---|---|---|---|---|
| 1 | rettilineo_lungo | 0.0 | 0.0 | 480 |
| 2 | curva_dolce_dx | 0.5 | 0.0 | 300 |
| 3 | autostrada | 0.0 | 0.0 | 520 |
| 4 | rettilineo_largo | 0.0 | 0.0 | 480 |
| 5 | curva_larga_sx | -0.45 | 0.0 | 300 |
| 6 | curva_larga_dx | 0.45 | 0.0 | 300 |
| 7 | rampa_ponte | 0.45 | 1.0 | 200 |
| 8 | ponte | 0.0 | 1.0 | 360 |
| 9 | discesa_ponte | -0.45 | 0.0 | 200 |
| 10 | dosso | 0.0 | 0.0 | 160 |
| 11 | curva_stretta_dx | 0.85 | 0.0 | 230 |
| 12 | curva_stretta_sx | -0.85 | 0.0 | 230 |
| 13 | curva_dolce_sx | -0.5 | 0.0 | 300 |
| 14 | rettilineo_lungo | 0.0 | 0.0 | 480 |

## lv_02_atomico

strada atomica anni '50: razzi di latta, UFO di stagno e colori pastello. Tempo limite 58 s, 18 tratti, 5200 segmenti per giro della sequenza.

| # | tratto | curva | collina | segmenti |
|---|---|---|---|---|
| 1 | rettilineo_lungo | 0.0 | 0.0 | 480 |
| 2 | collina_su | 0.0 | 0.2 | 300 |
| 3 | chicane_dx | 0.85 | 0.0 | 140 |
| 4 | chicane_sx | -0.85 | 0.0 | 140 |
| 5 | dune | 0.0 | 0.0 | 360 |
| 6 | curva_dolce_sx | -0.5 | 0.0 | 300 |
| 7 | rettilineo_largo | 0.0 | 0.0 | 480 |
| 8 | curva_larga_dx | 0.45 | 0.0 | 300 |
| 9 | curva_larga_sx | -0.45 | 0.0 | 300 |
| 10 | rettilineo_corto | 0.0 | 0.0 | 340 |
| 11 | curva_stretta_sx | -0.85 | 0.0 | 230 |
| 12 | curva_dolce_dx | 0.5 | 0.0 | 300 |
| 13 | curva_stretta_dx | 0.85 | 0.0 | 230 |
| 14 | galleria | 0.3 | 0.0 | 260 |
| 15 | uscita_galleria | -0.26 | 0.0 | 300 |
| 16 | curva_cieca_dx | 0.6 | ? | 220 |
| 17 | curva_cieca_sx | -0.6 | ? | 220 |
| 18 | saliscendi | ? | ? | 300 |

## lv_03_foglie

viale delle foglie: colori che cambiano e fumi stilizzati, parodia dentro il messaggio di sicurezza stradale. Tempo limite 59 s, 18 tratti, 5240 segmenti per giro della sequenza.

| # | tratto | curva | collina | segmenti |
|---|---|---|---|---|
| 1 | rettilineo_lungo | 0.0 | 0.0 | 480 |
| 2 | curva_dolce_dx | 0.5 | 0.0 | 300 |
| 3 | valle | 0.0 | -0.35 | 220 |
| 4 | chicane_dx | 0.85 | 0.0 | 140 |
| 5 | chicane_sx | -0.85 | 0.0 | 140 |
| 6 | dune | 0.0 | 0.0 | 360 |
| 7 | curva_dolce_sx | -0.5 | 0.0 | 300 |
| 8 | rettilineo_corto | 0.0 | 0.0 | 340 |
| 9 | curva_stretta_dx | 0.85 | 0.0 | 230 |
| 10 | rettilineo_largo | 0.0 | 0.0 | 480 |
| 11 | curva_larga_sx | -0.45 | 0.0 | 300 |
| 12 | curva_larga_dx | 0.45 | 0.0 | 300 |
| 13 | rettilineo_corto | 0.0 | 0.0 | 340 |
| 14 | curva_stretta_sx | -0.85 | 0.0 | 230 |
| 15 | rettilineo_corto | 0.0 | 0.0 | 340 |
| 16 | curva_cieca_dx | 0.6 | ? | 220 |
| 17 | curva_cieca_sx | -0.6 | ? | 220 |
| 18 | saliscendi | ? | ? | 300 |

## lv_04_cromo

tangenziale sopraelevata tra megastrutture cromate: curvoni lunghi e veloci, una grande discesa. Tempo limite 48 s, 15 tratti, 4840 segmenti per giro della sequenza.

| # | tratto | curva | collina | segmenti |
|---|---|---|---|---|
| 1 | rettilineo_lungo | 0.0 | 0.0 | 480 |
| 2 | curva_dolce_dx | 0.5 | 0.0 | 300 |
| 3 | autostrada | 0.0 | 0.0 | 520 |
| 4 | rettilineo_largo | 0.0 | 0.0 | 480 |
| 5 | curva_larga_sx | -0.45 | 0.0 | 300 |
| 6 | curva_larga_dx | 0.45 | 0.0 | 300 |
| 7 | rampa_ponte | 0.45 | 1.0 | 200 |
| 8 | ponte | 0.0 | 1.0 | 360 |
| 9 | discesa_ponte | -0.45 | 0.0 | 200 |
| 10 | rettilineo_largo | 0.0 | 0.0 | 480 |
| 11 | curva_stretta_dx | 0.85 | 0.0 | 230 |
| 12 | collina_giu | 0.0 | -0.2 | 300 |
| 13 | dosso | 0.0 | 0.0 | 160 |
| 14 | curva_stretta_sx | -0.85 | 0.0 | 230 |
| 15 | curva_dolce_sx | -0.5 | 0.0 | 300 |

## lv_05_spray

periferia industriale con muri a spray: incroci a 90 gradi, rettilinei corti, frenate. Tempo limite 61 s, 20 tratti, 5640 segmenti per giro della sequenza.

| # | tratto | curva | collina | segmenti |
|---|---|---|---|---|
| 1 | rettilineo_corto | 0.0 | 0.0 | 340 |
| 2 | curva_stretta_dx | 0.85 | 0.0 | 230 |
| 3 | citta | 0.0 | 0.0 | 200 |
| 4 | chicane_dx | 0.85 | 0.0 | 140 |
| 5 | chicane_sx | -0.85 | 0.0 | 140 |
| 6 | rettilineo_corto | 0.0 | 0.0 | 340 |
| 7 | curva_stretta_sx | -0.85 | 0.0 | 230 |
| 8 | dune | 0.0 | 0.0 | 360 |
| 9 | curva_stretta_dx | 0.85 | 0.0 | 230 |
| 10 | rettilineo_largo | 0.0 | 0.0 | 480 |
| 11 | curva_larga_sx | -0.45 | 0.0 | 300 |
| 12 | rettilineo_lungo | 0.0 | 0.0 | 480 |
| 13 | curva_stretta_sx | -0.85 | 0.0 | 230 |
| 14 | rettilineo_corto | 0.0 | 0.0 | 340 |
| 15 | curva_larga_dx | 0.45 | 0.0 | 300 |
| 16 | galleria | 0.3 | 0.0 | 260 |
| 17 | uscita_galleria | -0.26 | 0.0 | 300 |
| 18 | curva_cieca_dx | 0.6 | ? | 220 |
| 19 | curva_cieca_sx | -0.6 | ? | 220 |
| 20 | saliscendi | ? | ? | 300 |

## lv_06_desktop

prato verde e cielo con nuvole da sfondo del desktop: colline continue, saliscendi e curve medie. Tempo limite 47 s, 16 tratti, 4840 segmenti per giro della sequenza.

| # | tratto | curva | collina | segmenti |
|---|---|---|---|---|
| 1 | rettilineo_lungo | 0.0 | 0.0 | 480 |
| 2 | curva_dolce_dx | 0.5 | 0.0 | 300 |
| 3 | valle | 0.0 | -0.35 | 220 |
| 4 | collina_su | 0.0 | 0.2 | 300 |
| 5 | rettilineo_largo | 0.0 | 0.0 | 480 |
| 6 | curva_larga_sx | -0.45 | 0.0 | 300 |
| 7 | curva_larga_dx | 0.45 | 0.0 | 300 |
| 8 | rampa_ponte | 0.45 | 1.0 | 200 |
| 9 | ponte | 0.0 | 1.0 | 360 |
| 10 | discesa_ponte | -0.45 | 0.0 | 200 |
| 11 | collina_giu | 0.0 | -0.2 | 300 |
| 12 | dosso | 0.0 | 0.0 | 160 |
| 13 | curva_stretta_dx | 0.85 | 0.0 | 230 |
| 14 | rettilineo_lungo | 0.0 | 0.0 | 480 |
| 15 | curva_stretta_sx | -0.85 | 0.0 | 230 |
| 16 | curva_dolce_sx | -0.5 | 0.0 | 300 |
