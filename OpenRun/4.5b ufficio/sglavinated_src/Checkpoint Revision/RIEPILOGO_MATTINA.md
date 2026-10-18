# RIEPILOGO_MATTINA — OpenRun 4.6 (2026-10-18)

## Fatto per blocco / agente
- Capo: baseline CP-001 aperta e chiusa come riferimento; aperti CP-002 (grafica), CP-003 (Officina/test), CP-004 (fonte Andrea + archivio v4); aggiornati STATO_CORRENTE.md e REGISTRO_REVISIONI.md.
- Designer: catalogo 52 voci (30 approvate, 16 pronte, 2 in costruzione, 4 mancanti); 100 PNG non catalogati segnalati; testi fumetti completi. Mancano sprite concorrenti/personaggi, 3 finali animati e sei percorsi. Nessun asset migrato in Shared asset/.
- Builder/Developer (Officina): STATUS_OFFICINA.md scritto; wrapper CMake/CTest e allowlist aggiornati; gate T1 OK su 3 livelli (report 1155: exit 0, media 0,19–0,20 ms/frame, max 0,393 ms/frame, replay identico, off-road/accelerazione/sterzo/curve OK); runner SDL2 compilato con PNG disabilitato.
- Developer/Builder Junior: nessuna assegnazione di task attiva registrata.

## Cosa non funziona / blocchi
- CP-002, CP-003 e CP-004 restano APERTI: manca la review indipendente (revisore da assegnare dal Capo, diverso dall'autore).
- CTest: 7 test scoperti, nessuno eseguito; CMake Tools in VS Code senza progetto attivo.
- SDL2_image non installabile (conflitto Homebrew/libtool): caricamento PNG disabilitato, prova_gate disegna rettangoli.
- Exit code del gate non registrato nel log (solo dichiarato nel report 1155).
- Grafica: sprite auto.traffico, Gianni/Gesù/UFO, 3 finali animati e sei percorsi mancanti; 100 PNG da classificare.

## Decisioni aperte in ANDREA_REVIEW.md (le 3 più importanti)
1. T1-Q1 | Chiusura gate T1: DEFAULT sì se `sh tools/gate_t1.sh soak` exit 0, replay 2x IDENTICO, ms/frame <= 20. Risposta di Andrea: non registrata.
2. T1-Q2 | Promozione P1: DEFAULT sì (screenshot bloccati, prova differita). Risposta di Andrea: non registrata.
3. T1-Q3 | Prova manuale su Mac e Pi 400: DEFAULT sì, Andrea esegue appena possibile; gate headless resta verde. Esito: non registrato.

## Prossimo passo consigliato
Assegnare revisori indipendenti per CP-002, CP-003, CP-004; per CP-003 attivare il progetto CMake in VS Code ed eseguire `sh tools/run_tests.sh` (CTest) registrando comando, exit code e misure; quindi gate soak per elementi su T1-Q1/T1-Q2 prima di proporre chiusura a Andrea.
