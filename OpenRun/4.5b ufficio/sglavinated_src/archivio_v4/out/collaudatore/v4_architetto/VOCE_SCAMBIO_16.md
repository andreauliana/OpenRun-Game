# Record voce SCAMBIO (da trascrivere in SCAMBIO.md quando i permessi lo permettono)

### 16. ARCHITETTO -> CAPO | stato [CHIUSA] | 16 ott 2026
Chiude voce 10 (CAPO -> ARCHITETTO, [APERTA]).

BLOCCO B consegnato: motore_nuovo/docs/ARCHITETTURA.md (moduli core/road/render/audio/input/personaggi/ui/fx, interfacce, ciclo di gioco a frame, determinismo replay + hash frame ogni 60 frame, piano blocchi C-N), FORMATI_DATI.md (livello.json, frasi.json, personaggi manifest/fumetti, playlist.json radio->.xm, ui menu/hud, config.json moduli on/off; nessun limite artificiale), BUILD.md (CMake: sglavinated_core static senza SDL, runner SDL2, core libretro stub; comandi Mac brew e Pi 400 apt; obiettivo Pi 400 <=20 ms/frame, 50 fps), LICENZE.md (D1 aperta, DEFAULT nessuna licenza dichiarata, raccomandazione MIT codice / CC-BY grafiche, tabelle tool di build e fonti tecniche, obblighi per nuovo file).
Coerenza col codice esistente in motore_nuovo/src/ verificata: framebuffer 320x224, mixer int16 stereo, Pad tipo RetroPad, FrameClock, Rng xorshift32 seme fisso, target CMake core/runner/libretro stub — OK.

BLOCCO FED consegnato: FEDELTA.md (10 tratti strutturali, ciascuno con Stato e prova; punteggio 50.0%, soglia raggiunta) e proposta D3: albero 5 percorsi x 5 tappe = 15 tappe, 5 finali; prototipo 3 livelli di prova con stili diversi da DIREZIONE_ARTISTICA.md (testo pronto per RISPOSTA DI ANDREA in DECISIONI_ANDREA.md voce D3).

Controlli eseguiti da sglavinated_src:
- python3 motore_nuovo/strumenti_py/check_catalogo.py -> OK (catalogo: 11 righe, 2 cartelle)
- python3 motore_nuovo/strumenti_py/quota_open.py -> quota open 73.4% (soglia 50%)

Aggiornamenti stati RICHIESTA.md (non eseguibili: modifica file esistenti negata dal sistema): BLOCCO B [~]->[x], BLOCCO FED [ ]->[x].

Risultati condivisi in out/architetto/ (copie dei 5 documenti + RIEPILOGO_CONSEGNA_B.md).
Prossimi: @reviewer controlla licenze al checkpoint; @developer/@developer-junior inizia BLOCCO C usando questi documenti come spec. Decisioni ancora attese da Andrea: D1 licenza, D3 numero tappe/bivi.
