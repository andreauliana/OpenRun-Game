# RIASSUNTO @developer-junior
Riscritto 14/11 08:35 (turno V47 VJ3 chiuso).
- Ruolo: test `motore_nuovo/test/` e prove via `prova_gate`; sola lettura `src/`, nessun codice prodotto senza autorizzazione.
- **VJ3 CHIUSO 6/6 livelli**: lv_01 0.03%, lv_02 0.19%, lv_04 0.03%, lv_05 0.24%, lv_06 0.09%; replay 2x IDENTICO ovunque, ms/frame medi 1.4-2.6.
- **lv_03_foglie resta anomalo**: 60 Hz 39.73 s vs 50 Hz 41.58 s (4.44%). Ipotesi "guida automatica" gia' falsificata il 14/11: e' il gioco a 50 Hz su quella pista. Segnalato al @developer.
- Buco del gate chiuso da me: nuovo modo `prova_gate <out> <livello> frequenza_confronto` (helper `misuraGiroFrequenza`, soglia `MAX_DIFF_FREQUENZA_PCT = 2.0`, exit 22) confronta 60 e 50 Hz nello stesso comando. lv_03 -> exit 22 atteso, lv_06 -> exit 0.
- `gate_t1.sh` non usa il nuovo modo (chiama solo `frequenza` su lv_01): il gate resta verde.
- lv_05 e' il livello piu' pesante finora: 2.44-2.60 ms/frame medi (citta' con incroci e traffico) contro 1.7-1.8 di lv_06; utile per V3/V9 del @developer.
- Prossimi passi miei: **VJ4 soak** (1 livello per turno, fatti lv_01-lv_04) e **VJ5 ∞** review dell'ultimo cambio in `src/`.
- Rilievo VJ5 invariato: `src/game/fasi.cpp:576` puntini riga `nostro` bianco fisso vs `col` lampeggiante (567), commento 574-575 incoerente.
- Da fare a mano (fuori dal mio perimetro): `git checkout -- salvataggi/giocatore.json` (ancora cancellato nel working tree).
- gate_t1.sh non eseguito oggi (turno senza modifiche a `src/`).
