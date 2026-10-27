# Review incrociata R2 passo 1d — fasi.cpp (RisultatiFase: nome a 3 lettere)

**Data:** 27/10/2026 19:06  
**Autore del codice:** @developer  
**Reviewer:** @developer-junior (me)  
**Blocco:** R2 [~] — passo 1d di 4 (ordine: 1d -> DECO1 2f -> RIV -> PIST1)  
**File esaminato:** motore_nuovo/src/game/fasi.cpp (git diff vs HEAD)  
**Gate di riferimento:** `ciclo` (motore_nuovo/test/prova_gate.cpp, mio) — non lo approvo (autore); lo verifico e @developer lo rivede.

## Requisiti (R2 1d)
Nome giocatore a 3 lettere nella schermata Risultati: SU/GIU cambia lettera (ciclico A..Z), Left/Right sposta il cursore, Start conferma e torna al Menu; timeout di sicurezza 30 s; classifica con nome del giocatore invece di "TU"; posizione calcolata su nome_.

## Esame del diff
- Costruttore: `nome_` = "AAA", `nomePos_` = 0. OK.
- `entra()`: resetta nome a "AAA" e cursore a 0 a ogni ingresso (determinismo preservato). OK.
- `classifica_.push_back({std::string(nome_), punteggio_})`: sostituisce "TU". OK.
- `posizione_`: confronta `e.nome == nome_` invece di "TU". OK.
- `aggiorna()`: Up/Down cambiano lettera, Right/Left spostano cursore, Start o timer > 30 s imposta finita_. OK.
- `disegna()`: riga "NOME: A_A_" con cursore "_", punteggio, posizione, classifica con `>` sul giocatore. OK.
- Core senza SDL, durate in secondi (Andrea 19 ott), nessun tocco a src/runner/. OK.

## Verifica eseguita (questo turno)
Comando: `UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/ciclo_review" lv_01_neon ciclo`  
Exit code: 0 — GATE CICLO OK  
- sequenza A = Splash Menu Gara FineTempo Risultati Menu (attesa rispettata)
- sequenza B = identica (determinismo 2x IDENTICO)
- stesso hash, stesse misure, speedMax = 234.40, nessun ribaltamento
- corsa singola = ok, ESCI = ok
Build: `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0 (@developer).  
ctest: `gate_ciclo` già verde (sviluppatore, Test #22 Passed, 9.19s).

## Esito
**POSITIVO.** Nessun rilievo bloccante. Nota marginale: un pareggio di punteggio tra giocatore e fittizio dipende da `std::sort` non stabile, ma su input identico il risultato è deterministico (accettabile).

## Prossimi passi (R2)
DECO1 passo 2f (@developer), poi RIV punti a/b/c (@developer), poi gate `urto` (@developer-junior quando esiste il bump).
