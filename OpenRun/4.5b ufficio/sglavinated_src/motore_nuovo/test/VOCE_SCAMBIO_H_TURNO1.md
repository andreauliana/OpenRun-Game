# Voce pronta per SCAMBIO.md (bloccata: SCAMBIO.md write-protetto per @debugger, regola 21)
@capo: incollare la voce seguente in fondo a SCAMBIO.md e chiudere la voce 22 ([APERTA]->[CHIUSA]). Nessuna modifica necessaria.

---
### 35. DEBUGGER -> CAPO | stato [CHIUSA] | 16 ott 2026
Richiesta/Esito: BLOCCO H (voce 22) — verifica statica conclusa, build/fps NON eseguibili (shell negata regola 21: nessun cmake, nessun compilatore C++, comandi python3 non raggiungibili).
Numeri statici: SPDX MIT 25/25 file motore_nuovo/src/ (manifest.json "licenza": MIT); 0 include chiusi con `>`; frameHash ogni 60 frame (runner/main.cpp:149); core senza SDL, senza orologio di sistema, senza std::chrono/time/rand (grep 0 hit su src/core); niente asset/tracciato/musica/codice Sega (solo commenti negativi e citazioni idee Lou's/Jake Gordon).
Strumenti (ultima esecuzione, non ripetibile qui): check_catalogo.py exit 0 "catalogo: 11 righe, 2 cartelle"; quota_open.py 72.8% (107738 di 148033 byte, soglia 50.0%) -> sopra soglia, da riconfermare appena shell/python3 tornano disponibili.
Determinismo: OK statico (FrameClock contatore frame, xorshift32 seme 20261015, dt=1/50; runner Game(20261015u)).
Mancanti (richiedono toolchain + harness playback): replay "stesso file di input due volte -> stesso hash ogni 60 frame"; misura ms/frame (media/p99/max, target <=20 ms, obiettivo <=12 ms, 50 fps) e temperatura Pi 400. Piano: motore_nuovo/test/ARCHITETTURA_CHECK.md §5.
BLOCCO H resta [~]: conferma @capo e build/fps reali spettano ad Andrea sul Pi 400. Nessun file di @developer modificato; output in motore_nuovo/test/ + out/debugger/.
---
