# Esito R1 turno 1 — 20/10/2026 (dopo le 12:25)

## Comando eseguito
`sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (dalla radice `sglavinated_src/`, comando singolo, senza pipe/2>&1/;).

## File creati (`motore_nuovo/dati/tracciati/`)
- `tratti.json`: libreria 8 tratti (rettilineo_corto 450, rettilineo_lungo 650, curva_dolce_dx/sx 0.5/400, curva_stretta_dx/sx 0.85/300, collina_su/giu +/-0.2/400), entrata/uscita -1 (raccordo automatico min(n/3,60) in road.cpp).
- `regole.json`: regole per lv_01..lv_06 + default (rettilineo 25-75%, rettilineo_piu_lungo_min 300, curva_max 1.0, curva_dx_min/sx_min 0.3, fuori_strada_max 0.01).

## Gate (log: `UFFICIO/Officina/Test area/gate_t1.log`)
- cmake configure: OK. `cmake --build` target `prova_gate`: exit 0 (core + runner compilano).
- percorso (giro guidato, prova_gate modo `percorso`): 6/6 GATE PERCORSO OK
  - lv_01_neon: rettilineo 63% (piu' lungo 659), curva dx/sx 0.85/0.85, giro finito 1527 frame, fuori strada 0.0%
  - lv_02_atomico: rettilineo 65% (1440), curva 0.72/0.98, finito 1999 frame, fuori 0.0%
  - lv_03_foglie: rettilineo 57% (590), curva 0.75/0.95, finito 1679 frame, fuori 0.0%
  - lv_04_cromo: rettilineo 71% (1457), curva 0.85/0.85, finito 1881 frame, fuori 0.0%
  - lv_05_spray: rettilineo 55% (607), curva 0.86/0.84, finito 1679 frame, fuori 0.0%
  - lv_06_desktop: rettilineo 68% (813), curva 0.85/0.85, finito 1729 frame, fuori 0.0%
- soak 30000 frame: 6/6 OK, ms/frame medio ~0.46 (limite 20), replay 2x IDENTICO su tutti i livelli.
- Nota: lo strumento shell ha interrotto l'esecuzione per timeout dopo il soak di lv_06, prima del riepilogo finale e degli step rimanenti (accelerazione, crash, sterzo, curve, gara, screenshot su lv_01). Questi step non toccano i dati R1 ed erano OK al gate 19/10. Exit code finale dello script non disponibile per l'interruzione.

## Blocco residuo
- Compito 3 R1: `prova_gate` legge `regole.json` al posto delle soglie hardcodate (`test/prova_gate.cpp:503` e `return`) — NON applicato: edit negato su `motore_nuovo/test/prova_gate.cpp`. `regole.json` e' coerente con le soglie attuali. Da completare a sblocco edit (o da @developer/@developer-junior), poi @capo apre CP-005 a gate verde con review incrociata registrate.

## Scritture questo turno
`motore_nuovo/dati/tracciati/` (2 file nuovi), `bacheche/developer/` (diario, RIASSUNTO), `out/developer/` (questa voce). Zero in `tools/`. Nessun test del Junior modificato.
