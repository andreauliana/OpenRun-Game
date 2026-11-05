# Esito gate T1 — 20/10 ~14:05

## Comando eseguito
`sh UFFICIO/Officina/TOOL/gate_t1.sh` (dalla radice sglavinated_src/, comando singolo)

## Exit code
0

## Risultato
GATE T1: TUTTO OK

## Dettaglio per livello (percorso)
| Livello | Segmenti | Rettilineo | Più lungo | Curva max DX | Curva max SX | Giro guidato | Fuori strada |
|---|---|---|---|---|---|---|---|
| lv_01_neon | 4250 | 63% | 659 | 0.85 | 0.85 | finito 1527 fr (30.5 s) | 0 frame (0.0%) |
| lv_02_atomico | 5650 | 65% | 1440 | 0.72 | 0.98 | finito 1999 fr (40.0 s) | 0 frame (0.0%) |
| lv_03_foglie | 4700 | 57% | 590 | 0.75 | 0.95 | finito 1679 fr (33.6 s) | 0 frame (0.0%) |
| lv_04_cromo | 5300 | 71% | 1457 | 0.85 | 0.85 | finito 1881 fr (37.6 s) | 0 frame (0.0%) |
| lv_05_spray | 4700 | 55% | 607 | 0.86 | 0.84 | finito 1679 fr (33.6 s) | 0 frame (0.0%) |
| lv_06_desktop | 4850 | 68% | 813 | 0.85 | 0.85 | finito 1729 fr (34.6 s) | 0 frame (0.0%) |

## Misure
- ms/frame medio: ~0.18-0.20 (limite 20), max < 2
- replay 2x: IDENTICO su tutti i 6 livelli
- accelerazione: OK (speed@1=9.90, speed@60=161.79)
- ribaltamento: OK
- sterzo: OK (sinistra=-218.3, destra=218.3)
- curve continue: OK (salto pendenza max=0.02)
- gara a tappe: 4 combinazioni OK, replay IDENTICO
- screenshot P1: OK (curva, sponsor, personaggio+fumetto)

## Blocco R1
- R1-1 road.cpp: FATTO
- R1-2/R1-3 tratti.json + regole.json: CREATI
- 6 livello.json con campo tracciato: presenti
- Compito 3 (prova_gate legge regole.json): BLOCCATO — edit negato su motore_nuovo/test/prova_gate.cpp, serve sblocco edit + CP-005 al Capo

## Log
UFFICIO/Officina/Test area/gate_t1.log