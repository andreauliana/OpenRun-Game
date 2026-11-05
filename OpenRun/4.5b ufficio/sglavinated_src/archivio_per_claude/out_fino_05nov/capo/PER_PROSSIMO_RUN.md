# Per il prossimo run — 17 ott 2026 

## Cosa è cambiato (verificato eseguendo, non solo leggendo)
- `cmake --build build --target prova_gate` **compila e passa** (g++/cmake).
- Il pacchetto del mattino diceva "mai compilato": non è vero. I 3 rilievi del reviewer sono applicati e la build ha trovato 2 errori VERI, ora corretti:
  1. `src/game/game.h`: mancava `using gameplay::ShowLayout;` (game.cpp:99 non compilava).
  2. `test/prova_gate.cpp`: mancava `using namespace sgl;` (u32/i32/Framebuffer non risolti). Il file è stato riscritto.
- Applicati i 3 rilievi: include `"game/game.h"` e `"render/framebuffer.h"`; guardia `if (segs_.empty() || totalLen_ <= 0) return;` in `Road::project()`; header SPDX MIT.
- `prova_gate` ora: esegue la simulazione 2 volte (replay), confronta il frameHash di ogni frame, misura ms/frame, scrive log + 3 PPM, ha il modo `soak` (30000 frame = 10 min), exit code 0-4. Esce con 4 se lanciato da cartella sbagliata (altrimenti il gioco ripiega in silenzio sul tracciato di emergenza e il test passerebbe sul tracciato sbagliato).
- BUILD.md: nlohmann/json rimosso (il codice usa `json_mini.h`), aggiunta sez.5 con comandi e codici di uscita.

## Risultati misurati
| Prova | Esito |
|---|---|
| lv_01_neon / lv_02_atomico / lv_03_foglie, smoke 500 frame | exit 0, replay 2x IDENTICO |
| soak 30000 frame (lv_01 e lv_02) | exit 0, nessun crash, replay IDENTICO |
| ASan + UBSan (Debug) | nessun errore |
| ms/frame | medio 0,05 (limite 20); max 3,5 nel soak |
| Guida | v=33,8@f60, 82@f170, 160@f450; sterzo sx X=-165, dx fino a offroad al frame ~476 |

## Aggiunte dopo il feedback di Andrea (14:00)
- **"Permessi negati"**: causa trovata nei log. `cmake` e il compilatore sono permessi, ma gli agenti provavano comandi composti (`which cmake; echo`, `cmake ... | head`, `ls a; ls b`) che non sono nella lista; dopo il primo rifiuto concludevano "shell negata, regola 21" e smettevano. Soluzione: un solo comando `sh tools/gate_t1.sh [soak]` permesso a coder e debugger (opencode.json + schede agente), regola 26 in AGENTS.md, regola 21 precisata. Provato in cloud: exit 0 su 3 livelli.
- **Bordi strada disallineati**: bug reale in `drawSegmentTrapezoid` (interpolazione lontano/vicino invertita; strisce con fase a parte, non ancorate ai segmenti). Riscritto con bande per indice segmento; `road.cpp` ora proietta con z continua dentro il segmento. Replay 2x ancora IDENTICO, 0,07 ms/frame.
- **Libreria asset**: `motore_nuovo/strumenti_py/indice_assets.py` genera `assets_originali/INDICE_ASSET.json/.md`. Gli asset trovati sono per lo piu' minuscoli (oggetti bordo strada 100-180 byte) e il motore non li usa nel gate headless.
- **Bar**: non toccato. AGENTS.md regola 25 lo vieta; BAR.md invariato dal 17 ott 10:12.

## Cosa NON è verificato
- fps e temperatura sul Pi 400/RetroPie.
- Il runner SDL2 (SDL2 assente in cloud: il target è escluso). Nessuno ha ancora guardato la guida in finestra dopo queste modifiche.
- Giudizio di Andrea sulla guida: v90 diceva NON OK.
- Screenshot: la strada con strisce e curve è grezza, sprite e cartelloni mancano, niente traffico, niente personaggio/fumetto rifiniti, solo 'box' rettangolari di blocchi Blu e Verdi che si muovono. Il personaggio Gianni (foglia) dovrebbe essere associato al box verde, Gesu dovrebbe essere associato al box azzurro. Affinare animazioni dei personaggi ed finalizzare esperienza di gameplay della gara e sviluppo del percorso di gara con tratti collegati.

## Problemi trovati da sistemare
1. Fuoristrada: velocità da ~160 a ~6 quasi di colpo. Probabile eccesso, da tarare.
2. Sprite veri nel test headless: il caricamento PNG esiste solo nel runner SDL2.
3. HUD e "tempo" (D8) non compaiono nel motore: Tappa 3, non adesso.
4. Colline: doppia integrazione in `Road::project` + camera a quota fissa, la strada sale nel cielo (T1-RUN punto 2).
5. Sprite: worldZ assoluto vs playerZ avvolto sul giro (T1-RUN punto 3).

## Stati
C [x], P1b [x] (gate headless passato), H [~], P1 [~], P2 [x], P3 [x], M [~], J [ ], N [ ], MD0 [-], T5 [-]. Nuovo blocco **T1-RUN** in RICHIESTA.md con compiti per @developer, @debugger, @reviewer e Andrea.

## Nota operativa
Gli agenti coder e debugger risultavano attivi (`.agenti_attivi`) mentre questi file venivano scritti. Se un file è stato sovrascritto da loro, rifare l'edit dai punti sopra: sono tutti piccoli.

## Aggiunta 14:25 — ordine nei fogli (Claude)
- Regola 27 (AGENTS.md): a fine turno UNA riga di diario `[GG/MM hh:mm] blocco | stato | esito misurato (exit code) | prossimo`. Lo stato e' una proposta: lo promuove @capo, e H/P1 anche con il via di Andrea. Sostituisce le righe "Pausa".
- Nel run di oggi (dalle 13:51) nessun agente ha aperto BAR.md: la regola 25 funziona. Ora dice esplicitamente di ignorare l'invito in cima a BAR.md. BAR.md e .registro_bar non sono stati toccati.
- RICHIESTA.md compattato (26 -> 16 KB): celle Stato a una riga, i RIEPILOGO TURNO sostituiti da un solo `RIEPILOGO CORRENTE`. Testo originale identico in out/capo/RICHIESTA_STORICO_17OTT.md. capo.md ora manda il capo su RICHIESTA_APERTA.md e SCAMBIO_RECENTE.md (non su SCAMBIO.md intero, 148 KB).
- Il debugger aveva scritto H [x] e "50 fps stabili" nella sua memoria: riportato a [~]; i fps reali non sono misurati.
- out/capo/RIEPILOGO_MATTINA.md (generato dal capo alle 14:13) e' vecchio sul punto build ("nessuna build cmake qui"): vale la regola 26, la build funziona.
