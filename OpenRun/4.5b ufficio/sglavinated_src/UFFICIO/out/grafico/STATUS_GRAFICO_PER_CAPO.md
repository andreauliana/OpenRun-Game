# STATUS GRAFICO PER CAPO — @grafico (09/11/2026 09:15)
Blocco: **MIG1 [~]** — passo **G1 [x] COMPLETATO** (cartelloni sponsor leggibili, Andrea 08/11 sera). PERC1 chiuso [x] (08/11).
Cosa fatto (09/11 08:41-08:46, turno matino):
- Script nuovo `UFFICIO/Dipartimento Grafico/TOOLS/ridisegna_sponsor.py`; 21 cartelloni 128x64 ridisegnati, STESSI nomi e STESSA misura: parola corta e grande (font pixel 5x7 a scala, contorno scuro 1 px), resto del nome piccolo in basso, 2-3 colori per stile (nessun saturo puro), dithering Bayer 4x4 SOLO sullo sfondo, cornice 2 px con filo scuro interno.
- Raw 21 sponsor con `png_a_raw.py`; `CATALOGO_SPONSOR.json` e 7 manifest aggiornati; prima/dopo in `UFFICIO/Officina/Test area/grafico/mig1_g1/{prima,dopo}/` (42 PNG, verifica visiva ok: CURVE, SLOW DOWN, CANNA-BIS scala 2).
Aggiunta di questo turno (09:11): completamento G1 lato catalogo —
- Script `UFFICIO/Dipartimento Grafico/TOOLS/aggiorna_catalogo_g1.py` → exit 0: le 21 voci `sponsor.*` di `CATALOGO_GRAFICO.json` passate da `approvato`/`D2/D2b` a `pronto` con nota "MIG1 G1 ridisegno, in attesa ok Andrea" (id e percorsi invariati; l'arte nuova attende l'ok).
- `png_a_raw.py` rieseguito → exit 0 (43 sprite, idempotente).
- Verifica visiva su neon/atomico/foglie (ERBA/MEDICA, GREEN/BOX) leggibili.
Verifiche:
- `python3 motore_nuovo/strumenti_py/check_catalogo_grafico.py` → **exit 1 PREESISTENTE**: 5 voci `segnaposto` non riconosciute (ui.logo.openrun di Andrea; auto.incrocio.lato_* di Claude 08/11) — non voci G1; + 6 PNG non catalogati. Da sistemare a chi competono.
Gate: `sh UFFICIO/Officina/TOOL/gate_t1.sh` e ctest → permission denied per @grafico: **gate non eseguito** in nessuno dei turni di oggi. Raw stesso formato/misura; conferma visiva in gioco da chiedere a @developer/Officina.
Note di canale: scrittura di `UFFICIO/out/grafico/ULTIMO_TURNO.md` negata per allowlist (usata STATUS); registrazione in `Ufficio/bacheche/grafico/diario.md` e `RIASSUNTO.md` ok.
Licenze: tutto originale, CC0-1.0, marchi inventati, zero materiale Sega; nessun asset Andrea/legacy toccato.
Prossimi: **G2** quando D1-D3 (@developer) sara' pronto (screenshot + 3 osservazioni); poi V47 **VG1**. Richiesta review Andrea: revisione visiva anteprime `mig1_g1/`.
Vincoli: no HUD/UI, no auto giocatore, no premiazione, no scenari Andrea, no parapetti ponte. Blocco M sospeso.
