# ULTIMO TURNO — @developer-junior (23/11/2026 08:26)
- **Blocco/pass**: V47 VJ4 (terzo passo, 3/6, davanti a VJ5 ∞) — soak `lv_03_foglie`.
- **File cambiati (solo documenti di ruolo)**: `UFFICIO/bacheche/developer-junior/diario.md` (una riga), `UFFICIO/bacheche/developer-junior/RIASSUNTO.md` (riscritto), `UFFICIO/out/developer-junior/ULTIMO_TURNO.md` (riscritto). Nessuna modifica a src/test/dati di prodotto; REVIEW_V47 non toccato (VJ4 non chiede review), STATUS_OFFICINA non toccato (nessun altro .md).
- **Comando 1**: `git status --short` — **exit 0** (M src renderer.cpp + road.cpp, non toccati: solo prova).
- **Comando 2**: `sh UFFICIO/Officina/TOOL/run_tests.sh` — **exit 0** (ctest **27/27**, 174.85 s, prova_gate ricompilato, gate_decorazioni passa).
- **Comando 3**: `./UFFICIO/Officina/build/prova_gate "UFFICIO/Officina/Test area/vj4_lv03_foglie_soak_2311_t1" lv_03_foglie soak` — **exit 0** (`GATE OK`).
- **Misure**: 30000 frame x2, replay **IDENTICO**, nessun crash; ms/frame medio **0.845**, max **79.872** (frame 16637, limite 20); accelerazione reattiva, offroad frame 156, X[-1070.7,2100.0], zFinale 737468.6, car 6.5/4.4 ms, init 14.7/10.9 ms.
- **Problemi residui**: spike 79.872 ms/frame isolato (1 frame su 30000, non ripetuto nel replay; media 0.845): sopra limite 20 e target Pi 400 12 ms, come 61.573 (lv_01) e 90.682 (lv_02); da tenere d'occhio. Non e' un fail.
- **`gate_t1.sh`**: non eseguito in questo turno.
- **Prossimo**: VJ4 soak `lv_04_cromo` (4/6, livello diverso), poi VJ5 ∞.
