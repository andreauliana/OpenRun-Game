# RIEPILOGO_MATTINA — OpenRun 4.6 (2026-10-20)

## Fatto per blocco/agente
- Sviluppo/Builder-developer (R1 + T1-RUN): road.h/road.cpp (raccordo entrata/uscita, curva costante), game.cpp campo `tracciato` su 6 livello.json. `sh tools/gate_t1.sh soak` exit 0, 6/6 GATE PERCORSO OK (rettilineo 52–65%, curva max 0.37–0.88, fuori strada 0), ms/frame medio 0.185 max 0.518 (limite 20), replay 2x IDENTICO. Peer review Junior POSITIVA (out/developer-junior/REVIEW_T1RUN_VOCE1175_2026-10-19.md).
- Junior: peer review indipendente su VOCE_SCAMBIO_1175; nessun blocco assegnato; in attesa task Capo.
- Grafico: check catalogo exit 0 (52 voci: 30 approvate, 16 pronte, 2 costruzione, 4 mancanti; 100 PNG non catalogati). CP-002 aperto, review indipendente da assegnare. Mancano personaggi/concorrenti, 6 percorsi, 3 finali animati.
- Officina: CMake Release configurato, runner compila (PNG off), 7 test CTest scoperti non eseguiti; SDL2_image non installabile (brew bloccato libtool); VS Code CMake senza progetto attivo.

## Non funziona / blocchi
- Blocco H: v91 Andrea runner Mac SDL2 + fps/temp Pi 400 non fatti (T1-Q3/T1-Q8); Pi 400 dopo ok Mac (ordine Andrea).
- libretro.h manca → BUILD_LIBRETRO=ON non compila.
- P1 punto 4 (sprite veri) e G1 [ ] di Designer, fuori scope T1.
- dati/tracciati/tratti.json e regole.json non ancora scritti (contenuto pronto in out/developer/VOCE_SCAMBIO_1174.md).
- CP-002/CP-003/CP-004 in attesa di review indipendente.

## Decisioni aperte ANDREA_REVIEW.md (3 più importanti)
1. D12 [APERTA, risposta A = sì]: dopo peer review R1 e gate verde 6/6, Andrea ri-verifica runner su Mac (v91) prima di Pi 400.
2. Frequenza di gioco (19 ott): 60 Hz default, opzione 50 Hz; tutte le misure in secondi (non frame) — da checkpoint dopo R1, prima di R2.
3. SDL/multipiattaforma (19 ott): include `<SDL.h>` portabile, SDL2main condizionale, find_package(SDL2 2.0.5), libretro.h, script RetroPie Ports; ordine Mac → Linux → Pi 400 → Windows XP.

## Prossimo passo consigliato
Scrivere dati/tracciati/tratti.json e regole.json (contenuto in out/developer/VOCE_SCAMBIO_1174.md), completare peer review incrociata Junior su R1, poi attivare ri-verifica Mac di Andrea (D12 = A) prima di ogni lavoro Pi 400.
