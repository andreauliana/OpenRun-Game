# Riepilogo mattina — OpenRun 4.6 (20/10/2026)

## Fatto per blocco/agente
- Blocco R1 tracciati (developer): `road.cpp` raccordo entrata/uscita fatto; `dati/tracciati/tratti.json` (8 tratti) e `regole.json` creati; `tracciato` nei 6 `livello.json`.
- developer-junior: `prova_gate` legge `regole.json`, stampa regole violate, test espansione OK 6/6.
- Gate T1 (20/10 ~14:05, Mac): `sh UFFICIO/Officina/TOOL/gate_t1.sh soak` exit 0, `GATE T1: TUTTO OK`, 6/6 percorsi OK, ms/frame ~0.18-0.20 (max < 2), replay 2x IDENTICO. Log: `UFFICIO/Officina/Test area/gate_t1.log`.

## Cosa non funziona / blocchi
- CP-005: review incrociata indipendente NON ancora registrata (developer-junior rivede developer punti 1-3; developer rivede developer-junior punti 4-5).
- ctest: 7 test scoperti nella build, non rieseguiti né registrati.
- `prova_gate` legge `regole.json` (return riga 536): BLOCCATO, edit negato su `motore_nuovo/test/prova_gate.cpp`; serve sblocco + CP-005.
- Pi 400/RetroPie: manca `libretro.h`, `BUILD_LIBRETRO=ON` non compila; nessun lavoro dedicato fino a ok Andrea su Mac.

## Decisioni aperte in ANDREA_REVIEW.md (top 3)
1. D13 Chiusura T1-RUN [APERTA]: sì dopo review CP-005 + ctest verde; risposta Andrea ancora mancante.
2. D14 Ordine test post-R1/Pi 400 [APERTA]: Andrea ri-verifica runner su Mac prima di Pi 400; conferma richiesta.
3. D15 Attivazione G1 [APERTA]: catalogo 100 PNG, check exit 0, solo pronto/approvato; conferma richiesta.

## Prossimo passo consigliato
Registrare la review incrociata indipendente su CP-005 con prove, riconfermare gate verde (soak exit 0, 6/6 percorsi, replay 2x IDENTICO), eseguire ctest; poi chiudere T1-RUN e attivare G1 solo dopo decisione Andrea.
