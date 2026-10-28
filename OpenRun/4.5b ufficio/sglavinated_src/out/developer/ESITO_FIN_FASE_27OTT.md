# Esito FIN compito 1 — fase Finale (27/10/2026 22:38, @developer)

## Blocca
FIN [ ] — dopo R2 e RIV (entrambi [x] per RIASSUNTO 27/10 22:24). Compito 1 @developer: fase Finale tra arrivo e Risultati nella macchina a stati di R2. Compito 2 (@grafico: scene per arrivo) e 3 (@developer-junior: gate) posticipati.

## Modifiche
- motore_nuovo/src/game/fasi.h: `FaseId` -> `{ Splash, Menu, Gara, FineTempo, Finale, Risultati }`.
- motore_nuovo/src/game/fasi.cpp:
  - `FINALE_DURATA_S = 5.0f` (secondi, frequenza indipendente, decisione Andrea 19 ott).
  - Nuova classe `FinaleFase`: entra acquista pista d'arrivo (`Game::tappaCorrente()`) e posizione (`Game::posizioneArrivo()`), scena da `dati/finali.json` (segnaposto "FINALE" se mancante), disegna testo, durata in secondi, `prossima()` = Risultati.
  - `FineTempoFase::prossima()` -> `arrivo_ ? FaseId::Finale : FaseId::Risultati` (tempo scaduto salta il finale).
  - `GestoreFasi::crea()`: aggiunto `case FaseId::Finale`.
- motore_nuovo/src/game/game.h: accessore pubblico `i32 posizioneArrivo() const` (ritorna `posizioneArrivo_` se arrivato, altrimenti `posizioneGiocatore()`).
- motore_nuovo/dati/finali.json (nuovo): scene per lv_04_cromo, lv_05_spray, lv_06_desktop con varianti primo/podio/altro.

## Comandi ed esiti
- `cmake --build UFFICIO/Officina/build --target prova_gate` -> exit 0 (build pulita; unico warning `-Wswitch` su prova_gate.cpp per `Finale` non gestita in `nomeFase`, file non modificabile dal developer salvo R1-3, inoffensivo).
- `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/fin_ciclo" lv_01_neon ciclo` -> exit 0, `GATE CICLO OK`:
  - sequenza A e B = `Splash Menu Gara FineTempo Risultati Menu` (preservata; il ciclo termina in tempo scaduto, quindi salta il finale).
  - sequenza giusta=sì, stessa sequenza=sì, stesse misure=sì, stesso hash=sì, nessun ribaltamento, speedMax>0.
  - corsa singola termina in ARRIVO (attraversa Finale) e raggiunge Risultati; `corsa singola=ok`, `esci=ok`.

## Note
- L'inserimento di `Finale` non rompe il gate `ciclo` (che non può essere modificato, salvo R1 compito 3) perché si attiva solo su arrivo; il percorso gate termina in tempo scaduto.
- Core resta senza SDL, deterministico (dt in secondi, replay 2x IDENTICO).
- Bloccati in attesa Andrea: T1-RUN [~] (D19=B, ok runner Mac v91 mancante) e H [~] (fps/temperatura Pi 400); nessun lavoro dedicato a Pi 400/RetroPie prima di ok Mac. FORK1 [~] attende CP-007 di @capo. P1 [~] punto 4 sprite veri in attesa @grafico.

## Scritture
motore_nuovo/src/game/fasi.{h,cpp}, motore_nuovo/src/game/game.h, motore_nuovo/dati/finali.json, bacheche/developer/diario.md, bacheche/developer/RIASSUNTO.md, out/developer/ESITO_FIN_FASE_27OTT.md.
