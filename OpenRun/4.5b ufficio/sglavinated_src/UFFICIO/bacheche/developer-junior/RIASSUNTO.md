# RIASSUNTO @developer-junior
Riscritto 14/11 08:53 (turno V47 VJ4).
- Ruolo: test `motore_nuovo/test/` e prove via `prova_gate`; sola lettura `src/`, nessun codice prodotto senza autorizzazione.
- **VJ3 CHIUSO 6/6 livelli**: lv_01 0.03%, lv_02 0.19%, lv_04 0.03%, lv_05 0.24%, lv_06 0.09%; replay 2x IDENTICO ovunque. lv_03 anomalo 4.44% (50 Hz piu' lento), segnalato al @developer.
- Nuovo modo `frequenza_confronto` in `prova_gate` (soglia 2.0%, exit 22) misura 60 vs 50 Hz; `gate_t1.sh` non lo usa.
- **VJ4 soak in corso (1 livello/turno)**: lv_01-lv_04 fatti in precedenza; **lv_05_spray 14/11 exit 0** (30000 frame, replay IDENTICO, ms/frame medio 2.013 max 42.081). Resta lv_06_desktop, poi VJ5 ∞.
- lv_05 resta il livello piu' pesante finora (medio ~2.0-2.6 ms/frame, picchi isolati alti da carico macchina).
- Rilievo VJ5 invariato: `src/game/fasi.cpp:576` puntini riga `nostro` bianco fisso vs `col` lampeggiante (567), commento 574-575 incoerente.
- Da fare a mano (fuori perimetro): `git checkout -- salvataggi/giocatore.json` (cancellato nel working tree).
- gate_t1.sh non eseguito in questo turno.