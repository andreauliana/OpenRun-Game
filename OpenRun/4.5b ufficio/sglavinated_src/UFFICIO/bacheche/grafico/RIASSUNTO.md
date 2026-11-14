# RIEPILOGO — @grafico (14/11/2026 08:27)
1. V47 VG3 ∞ giro 28: tutto allineato, 0 PNG di Andrea da convertire.
2. `vg3_date_png_raw.py` exit 0: 188 PNG Libreria (142 confrontati, 46 non motore, 0 fuori copertura), 87 raw scenari (81 a byte, 6 skyline a data) + 54 altri gruppi allineati, gruppi raw noti 8 / sconosciuti 0.
3. `converti_oggetti_raw.py` exit 0: raw aggiornati 0, errori 0 (81 oggetti controllati).
4. Novita' del turno: `converti_oggetti_raw.py` (lo script che scrive i raw) ha radice da `__file__`, salta i PNG illeggibili da PIL con `PNG-ILLEGGIBILE` senza fermarsi (exit 1 finale), scrive atomico (tmp + `os.replace`) e rilegge ogni raw con `VERIFICA-FALLITA`; `--autotest` exit 0.
5. Residuo noto invariato: `sprite/rivali/auto_giocatore_sbandata.png` senza raw e non usato dal motore; non cancellato.
6. File turno: `TOOLS/converti_oggetti_raw.py`, `UFFICIO/bacheche/grafico/diario.md` (+1), `RIASSUNTO.md`, `UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md` (`ULTIMO_TURNO.md` negato dai permessi).
7. Gate: non eseguito (14/11 08:27).
8. Mai: HUD/UI, auto giocatore, logo/menu, premiazione, sponsor (solo Andrea), scenari/skyline Andrea. Licenze: zero esterne, zero Sega.
9. Chiusi: MIG1 G1 [ANNULLATO], MIG1 G2 [x], V47 VG1 [x], V47 V5 frasi [x], VG2 [x], PERC1, AUTO1, G-A1, G-P1, PIST1.
10. Prossimo: VG3 ∞ — ricontrollare PNG/sglraw al prossimo turno e convertire solo i nuovi.
