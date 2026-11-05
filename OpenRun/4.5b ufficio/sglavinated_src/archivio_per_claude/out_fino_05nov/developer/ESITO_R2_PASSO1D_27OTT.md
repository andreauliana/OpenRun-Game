# Esito R2 passo 1d — nome a 3 lettere nei Risultati (27/10/2026 18:44)

## Blocchi toccati
R2 [~] — passo 1d di 4 (ordine: 1d -> DECO1 2f -> RIV -> PIST1).

## Modifica
File: `motore_nuovo/src/game/fasi.cpp` (classe `RisultatiFase`)
- Aggiunto `char nome_[4]` (inizializzato "AAA") e `i32 nomePos_` (cursore 0..2).
- `entra()`: resetta nome a "AAA" e cursore a 0; inserisce `{nome_, punteggio_}` in classifica al posto di `"TU"`.
- `aggiorna()`: SU/GIU cambia la lettera al cursore (A..Z ciclico), Left/Right sposta il cursore, Start conferma e imposta `finita_` (torna al Menu); timeout 30 s.
- `disegna()`: mostra "NOME: A_A_" con cursore trattino basso, punteggio e posizione, classifica con `>` sul giocatore; aiuto "START TORNA AL MENU".
- `posizione_` calcolato confrontando con `nome_` invece di `"TU"`.

## Gate eseguiti
- `cmake --build UFFICIO/Officina/build --target prova_gate` — exit 0 (fasi.cpp compila, core senza SDL).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/ciclo_1d" lv_01_neon ciclo` — GATE CICLO OK:
  - sequenza A=B=Splash Menu Gara FineTempo Risultati Menu (attesa rispettata)
  - determinismo: stessa sequenza, stesse misure, stesso hash (run A == run B)
  - corsa singola OK, ESCI OK, nessun ribaltamento, speedMax > 0.
- `ctest --test-dir UFFICIO/Officina/build -R ciclo` — Test #22 gate_ciclo Passed (9.19s).

## Esito
PASSED. Il gate ciclo resta verde dopo l'inserimento del nome a 3 lettere.

## Prossimi passi (R2)
- DECO1 passo 2f: niente cartelloni/oggetti negli ultimi BIVIO_SEG segmenti, scala oggetti da manifest, oggetto ogni 2000 unita', run_tests.sh verde, screenshot lv_01/lv_03 con oggetti visibili.
- RIV punti a,b,c: urto = bump (no ribaltamento, -15/-20% velocita', controlli attivi), sprite rivali dal catalogo, comportamento realistico (rallenta in curva, cambio corsia morbido, distanza minima).
- PIST1 punto 1: nuovi tipi tratto (chicane, dosso/valle, ponte, autostrada, citta') in tratti.json/regole.json, larghezza per segmento.

## Vincoli rispettati
- Core indipendente da SDL (nessun tocca a src/runner/).
- Determinismo invariato (replay 2x IDENTICO, dt in secondi).
- Tutte le misure in secondi (decisione Andrea 19 ott).
- Nessun nuovo documento oltre a diario.md (una riga), RIASSUNTO.md (max 12 righe) e questo file esito.
