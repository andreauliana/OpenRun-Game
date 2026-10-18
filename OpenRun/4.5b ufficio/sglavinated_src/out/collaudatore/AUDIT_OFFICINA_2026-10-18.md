# Audit Officina / build C++ — 18 ottobre 2026

## Esito sintetico

- **Configurazione CMake e compilazione del target `prova_gate`: OK** in ambiente Linux pulito dopo aver isolato la cache CMake non portabile.
- **Test singoli:** `lv_01_neon` e `lv_02_atomico` hanno terminato con exit 0 in esecuzioni isolate; output include `replay 2x: IDENTICO` e `GATE OK`.
- **Gate completo:** NON CONFERMATO. Due tentativi del comando completo sono scaduti al limite dell'ambiente durante il secondo livello, senza un esito finale completo. Non dichiarare il gate T1 verde sulla base di questi tentativi.
- **Runner SDL2:** non verificato; in questo ambiente CMake non trova SDL2 e quindi non genera `sglavinated_runner`. La build del core e del gate non dipende da SDL2, come previsto.
- **Raspberry Pi 400 / macOS:** non verificati su hardware/OS reali.

## Problema P1 — cache CMake con percorsi assoluti obsoleti

Nel pacchetto sono presenti più directory `build*` con `CMakeCache.txt` riferite a `/Users/andreauliana/Documents/GitHub/OpenRun-Game/4.5 updated/...`. Quando il progetto viene estratto in un percorso diverso, `sh tools/gate_t1.sh` fallisce in configurazione con exit 10 prima della compilazione.

### Mitigazione introdotta

`tools/gate_t1.sh` controlla ora `CMAKE_HOME_DIRECTORY`. Se la cache di `build_gate` punta a un percorso diverso, usa una directory `build_gate_relocated_<hash>` invece di cancellare o sovrascrivere la build precedente. La build resta riproducibile e i vecchi artefatti sono preservati.

## Altri rilievi

1. Il README dichiara che le directory `build*` non sono state copiate, ma il pacchetto contiene build e cache CMake in più posizioni. Questo contraddice le istruzioni e crea falsi errori dopo spostamenti/estrazioni.
2. `tools/gate_t1.sh` lancia 3 smoke test e poi 4 prove aggiuntive. Ogni smoke test include anche il controllo di avvolgimento a 60.000 frame e inattività; il tempo complessivo è quindi molto superiore a un semplice smoke test. Separare in futuro smoke veloce e test estesi, senza ridurre i controlli, e misurare il tempo per fase.
3. CMake esclude il runner quando SDL2 non è disponibile. È un comportamento dichiarato, ma il report deve distinguere sempre core/gate da runner grafico.
4. La cartella `motore_nuovo/build*` non va considerata una fonte di verità: gli artefatti compilati dipendono da OS, percorso, compilatore e configurazione. La fonte di verità deve essere il codice e il log del comando eseguito nell'ambiente corrente.
5. Il file `tools/run_tests.sh` usa CTest, mentre `tools/gate_t1.sh` esegue direttamente `prova_gate`: i due percorsi non sono equivalenti e vanno mantenuti distinti finché non sono verificati entrambi.

## Prossimi passi consigliati

1. Eseguire `sh tools/gate_t1.sh` sul Mac dell'utente e conservare exit code + `out/collaudatore/gate_t1.log`.
2. Eseguire separatamente `sh tools/gate_t1.sh soak` solo dopo il gate standard verde.
3. Installare/verificare SDL2 e SDL2_image per validare il runner; il core-only verde non dimostra che la finestra/input funzionino.
4. Eseguire la build sul Pi 400 con `pi/compila_e_installa.sh` e annotare modello OS, compilatore, tempo e risultato.
5. In un turno separato, riprogettare il gate in profili `smoke` e `extended`, con timeout esplicito e log per fase. Non cambiare ora logica di gioco o asset.

## Evidenze osservate

- Primo tentativo con cache inclusa: configurazione fallita per mismatch tra percorso estratto e percorso registrato in CMakeCache.
- Dopo configurazione in directory di build pulita: `sglavinated_core` e `prova_gate` compilati al 100%.
- Esecuzioni isolate dei livelli Neon e Atomico: exit 0; replay identico e `GATE OK`.
- Gate completo: due tentativi interrotti dal timeout dell'ambiente; risultato complessivo lasciato **non verificato**.
