# Riassunto developer — 11/11/2026 09:20 (Turno 3)
- Stato: MIG1 D1-D5 [x], V47 V1-V8 [x]. Prossimo: passo ∞ V9 fluidita' a giro (parte da lv_01).
- V8 Caricamento fatto: motore_nuovo/src/game/game.cpp — <chrono> in caricaLivello, init e avviaGara con righe log "CARICAMENTO livello/init/gara <id> <ms>" catturate da gate_t1.sh nel log del gate.
- gate_t1.sh (ufficio/TOOL): "CARICAMENTO" aggiunto al riepilogo a video, i ms di caricamento escono con gli altri esiti.
- Misure 6 livelli (gate percorso+smoke, Mac Release): livello 3.1-11.2 ms; init (anche base del menu) 5.8-18.1 ms; avviaGara 8.2-12.1 ms. Nessuno supera 1 s: nessun alleggerimento, dati non toccati.
- Prove 11/11: run_tests.sh exit 0 ctest 27/27 (~08:55); gate_t1.sh exit 0 "GATE T1: TUTTO OK" 6/6 percorso, replay IDENTICI (~09:15).
- Log completo: UFFICIO/Officina/Test area/gate_t1.log.
- Prossimo: ∞ V9 un livello per turno (lv_01 -> lv_06): gate percorso, frame piu' lento; sotto 8 ms si scrive e si passa al livello dopo.
- Nota per @developer-junior: VJ1 resta (prova dopo ogni passo V + riga in REVIEW_V47.md).
