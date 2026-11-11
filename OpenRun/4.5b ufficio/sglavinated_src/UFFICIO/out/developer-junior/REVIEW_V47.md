# REVIEW V47 (riscritta) — @developer-junior
Turno 11/11 12:55, VJ5 ∞.

## Cambio rivisto
- `motore_nuovo/src/game/salvataggi.cpp` / `salvataggi.h`: persistenza nome e Top-10 in `salvataggi/giocatore.json` (carica/salva atomica).

## Rilievi
- `salvataggi.cpp:77-86`: `creaDirectorySeManca()` fa probe `fopen("salvataggi","rb")` (su POSIX una directory di norma non si apre come file, probe poco affidabile per distinguere file omonimo) poi `SGL_MKDIR` e ritorna `mkdir != 0`. Se la directory esiste già, `mkdir` fallisce con `EEXIST` e la funzione ritorna `false` anche in caso nominale; `salva()` ignora il ritorno (`:131-133`) ma procede a scrivere `.tmp` nella cartella: se la cartella manca davvero la scrittura fallirà dopo, se esiste il falso negativo è solo fuorviante. Semplificazione: trattare `EEXIST` come successo e propagare/gestire errori reali.
- `salvataggi.cpp:127-160`: `salva()` normalizza solo `nome` in `normale` ma serializza la classifica da `g.classifica` (`:147`) scrivendo `e.nome`/`e.data` così come sono; `carica()` invece tronca/padda i nomi a 6 caratteri (`:113-116`). Un chiamante che passi nomi più lunghi o non normalizzati produce JSON incoerente rispetto a quanto letto dopo; normalizzazione simmetrica in salvataggio eviterebbe drift.
- `salvataggi.cpp:134-159`: nessuna creazione esplicita della directory `salvataggi/` prima di aprire il `.tmp` quando manca: dipende dal fatto che sia già presente. Coerente col commento "si ricrea al primo salvataggio" solo se la directory viene creata davvero.

## Prova
- `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0 (11/11 12:55).
- `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj5_salvataggi_1255" lv_01_neon caricamento` exit 0, GATE CARICAMENTO OK (init 8.3 ms, avviaGara 6.7 ms, avviaGara singola 7.9 ms, sotto 1000 ms).
- Nota aggiunta in `motore_nuovo/test/prova_gate.cpp:102`.
- gate_t1.sh non eseguito.
