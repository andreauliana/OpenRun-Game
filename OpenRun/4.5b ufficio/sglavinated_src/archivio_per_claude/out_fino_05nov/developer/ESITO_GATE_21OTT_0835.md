# Esito gate T1 — 21/10 08:35 (@developer)

Comando eseguito: `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` (unico comando, dalla radice sglavinated_src/, nessun pipe/`;`/`2>&1`).
Ambiente: Mac darwin, build esistente riusata in UFFICIO/Officina/build/.

Risultato:
- Tutti e 6 i livelli: `GATE PERCORSO OK` (prova_gate + percorso).
  - lv_01_neon: rettilineo 63%, curva max 0.85/0.85
  - lv_02_atomico: rettilineo 65%, curva max 0.72/0.98
  - lv_03_foglie: rettilineo 57%, curva max 0.75/0.95
  - lv_04_cromo: rettilineo 71%, curva max 0.85/0.85
  - lv_05_spray: rettilineo 55%, curva max 0.86/0.84
  - lv_06_desktop: rettilineo 68%, curva max 0.85/0.85
  - Limiti regole.json: rettilineo [25%-75%], curva max [0.3-1.0], rettilineo piu' lungo >= 300 segmenti. Giro guidato finito su tutti e 6, fuori strada 0 frame (0.0%).
- Smoke ms/frame medio < 20 su tutti i livelli (max spike lv_03 40.247, lv_06 13.180; medio ben sotto il limite 20).
- Soak 30000 frame: lv_01_neon, lv_02_atomico, lv_03_foglie exit 0.
- Il comando e' stato interrotto dal timeout dello strumento (300s) durante `soak lv_04_cromo`, PRIMA del riepilogo finale: nessun esito "GATE T1: TUTTO OK" raggiunto oggi e nessun exit code pulito del gate completo. Stesso pattern del 20/10 13:00 (timeout durante lv_04_cromo, non errore di codice). Log: UFFICIO/Officina/Test area/gate_t1.log.

Stato:
- R1 compiti @developer 1-3 conclusi e verificati su file (road.cpp, tratti.json, regole.json, 6 livello.json con tracciato).
- T1-RUN resta APERTO: attende la ri-verifica runner Mac di Andrea (D12=A, D13=B, D14=A) prima di 60 Hz/R2; G1 e P1 punto 4 sprite in capo a @grafico; nessun lavoro su Pi prima dell'ok Mac di Andrea.
- Prossimo dopo R1 prima di R2: 60 Hz (dt in secondi, default 60 Hz, opzione 50 Hz, ctest verde a 60 Hz; ANDREA_REVIEW 19 ott Parte 1-2).

Scritture: bacheche/developer/diario.md (1 riga), bacheche/developer/RIASSUNTO.md, out/developer/ESITO_GATE_21OTT_0835.md.
