# Esito turno developer — 20/10/2026 10:48

Ruolo: @developer. Blocco attivo: R1 (tracciati 6 piste), stato [~].

## Gate
Comando tentato: sh UFFICIO/Officina/TOOL/gate_t1.sh soak
Esito: NON eseguito — shell permission.rejected (permission denied: shell).
'gate non eseguito' (nessuna misura exit code/log disponibile in questo turno).

## Verifica sorgente (read-only, nessun blocco scrittura)
- R1-1 road.cpp: FATTO — motore_nuovo/src/road/road.cpp implementa raccordo entrata/uscita a lunghezza propria (entrata/uscita, default min(n/3,60)), curva costante al centro del tratto, correzione della direzione solo su segmenti con curve != 0 (mai rettilinei), collina quota assoluta. Coerente con RIASSUNTO precedente.
- Tutti e 6 i motore_nuovo/dati/livelli/*/livello.json hanno campo tracciato (verificati lv_01_neon, lv_02_atomico).

## Blocco scrittura (persiste)
- motore_nuovo/dati/tracciati/tratti.json e regole.json inesistenti; scrittura nuovi file in motore_nuovo/dati/ negata (edit rejected). Compiti R1-2/R1-3 bloccati. Contenuto pronto: out/developer/VOCE_SCAMBIO_1174.md.
- prova_gate.cpp e docs/FORMATI_DATI.md non scrivibili al ruolo (compiti R1-4/R1-5 in capo a @developer-junior).

## Ultimo gate noto (riferimento, 19/10 11:45, builder-developer — non eseguito in questo turno)
- sh tools/gate_t1.sh soak: exit 0, GATE T1 TUTTO OK.
- 6/6 percorsi GATE PERCORSO OK: lv_01_neon rettilineo 65%/curva 0.57; lv_02_atomico 63%/0.88; lv_03_foglie 58%/0.67; lv_04_cromo 54%/0.37; lv_05_spray 73%/1.01; lv_06_desktop 52%/0.51. Giro guidato finito su tutti, fuori strada 0%.
- Soak 30000 frame: ms/frame medio 0.187 (limite 20), replay 2x IDENTICO, speed@60=161.79. Accelerazione, ribaltamento, sterzo, curve continue, gara a tappe, screenshot P1: OK. Log: UFFICIO/Officina/Test area/gate_t1.log.
- Peer review Junior indipendente: POSITIVA (VOCE_SCAMBIO_1175; report out/developer-junior/REVIEW_T1RUN_VOCE1175_2026-10-19.md).
- T1-RUN chiuso [x] per DEFAULT T1-Q1 (ANDREA_REVIEW: risposta Andrea sì).

## Andrea 19/10 09:39 (pre-tuning, superato dal tuning 11:45)
- GATE T1 FALLITO exit 13; giro guidato KO 5/6, solo lv_02_atomico OK (rettilineo 40%, curva max 0.86). Core OK ms/frame ~0.19 max 0.94, replay IDENTICO.

## Scritture effettuate questo turno
- bacheche/developer/diario.md (1 riga aggiunta)
- bacheche/developer/RIASSUNTO.md (riscritto, <=12 righe)
- out/developer/ESITO_TURNO_20OTT_1048.md (questo file)
Zero scritture in motore_nuovo/ e tools/. Nessun REPORT_TURNO, nessuna VOCE_SCAMBIO (nessuna voce da pubblicare su SCAMBIO.md).

## Prossimo passo
Sblocco scrittura motore_nuovo/dati/ e shell; creare dati/tracciati/tratti.json e regole.json; rilanciare sh UFFICIO/Officina/TOOL/gate_t1.sh soak su tutte e 6 le piste (T1-Q5 DEFAULT A); poi CP-005 del Capo con gate verde e review incrociate.
Ordine test Andrea: prima ok su Mac (runner SDL2), poi Raspberry Pi 400/RetroPie; nessun lavoro dedicato a Pi 400 finché non c'è ok di Andrea su Mac.
