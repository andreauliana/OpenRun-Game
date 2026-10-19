# STATI PROPOSTI — Capo (sostituisci a ogni turno)

Turno 1 — ora 0 di 12. Tappa attiva: **1 — Gate T1-RUN** (`sh tools/gate_t1.sh soak`).
Fonti lette (solo queste): TAPPE.md, ANDREA_REVIEW.md (precedenza assoluta), CONTROLLO.md, VISIONE.md, AGENTS.md (unica fonte di regole; aggiornamento 19/10: `assets_originali/` è symlink a `UFFICIO/Dipartimento Grafico/Libreria/assets_originali/` — unica cartella con due nomi, copia vera in Libreria), RICHIESTA_APERTA.md, SCAMBIO_RECENTE.md. Handoff reparto citati: UFFICIO/Officina/STATUS_OFFICINA.md (gate Andrea 19/10 09:39 fallito exit 13 → poi verde dopo tuning builder-developer), out/designer/STATUS_GRAFICO_PER_CAPO.md (inventario verificato 18/10, 52 elementi, 100 PNG non catalogati).

## Stati proposti — Tappa 1 (Gate T1-RUN)

| Blocco | Stato | Prova / criterio di completamento | Responsabile → revisore indipendente | Percorsi scrivibili |
|---|---|---|---|---|
| C | [x] | Baseline 4.6 chiusa (CP-001 riferimento documentale, nessuna approvazione release). | capo → — | Checkpoint Revision/ |
| T1-RUN | [x] **CHIUSO — gate verde + review indipendente positiva** | `sh tools/gate_t1.sh soak` exit 0, `GATE T1: TUTTO OK`; replay 2x IDENTICO su 6/6 livelli; ms/frame medio ~0.18 max ~0.48 (limite 20); giro guidato OK su tutti i 6 livelli (rettilineo 52–65%, curva max 0.37–0.88, 0 frame fuori strada); tuning confinato a livello.json dei 4 livelli (lv_01, lv_03, lv_04, lv_05), nessun core C++ toccato, licenza CC-BY-4.0 già presente. Gate fallito prima (Andrea 19/10 09:39 exit 13), risolto da builder-developer (DEFAULT T1-Q5 opzione A) e revisionato positivamente da developer-builder-junior (REVIEW_T1RUN_VOCE1175_2026-10-19.md). Pronto per Andrea (T1-RUN). | builder-developer (esecuzione) → developer-builder-junior (peer review, POSITIVO) | out/builder-developer/ (report VOCE_SCAMBIO_1175.md); motore_nuovo/dati/livelli/ (solo 4 livello.json) |
| P1 | [~] | Promozione T1-Q2 Andrea: "sì"; screenshot P1 registrati (SCAMBIO_RECENTE). Punto 4 sprite veri da assets_originali ancora aperto (dipende da G1/designer). | builder-developer → developer-builder-junior | out/builder-developer/ (report) |
| H | [~] | Andrea: v91 `sglavinated_runner` Mac (SDL2) + guida registrata in ANDREA_REVIEW.md; fps/temperatura Pi 400 (`vcgencmd measure_temp`, `get_throttled`). T1-Q3 Andrea: "sì", da eseguire. Gate headless verde. | Andrea → capo (solo registrazione esito) | out/capo/ |
| G1 | [~] **ATTIVATO** | T1-Q4 Andrea: "attivare". `check_catalogo_grafico.py` exit 0 e "PNG non catalogati: 0"; catalogo unico id/tipo/stato/file/raw/livelli. Attuale: 52 elementi, 100 PNG non catalogati (check exit 0, STATUS_GRAFICO 18/10). `assets_originali/` è symlink alla Libreria grafica (AGENTS.md aggiornato): non creare copie parallele. | designer → capo (review indipendente, CP-002) | out/designer/; fase 1 assets_originali/, libreria/, dati UI/testo assegnati |
| M | [~] | Tappa 3 (HUD/font variante B, elemento tempo) non avviata — fuori scope Tappa 1. | designer → capo | out/designer/ |
| MD0 | [-] | Sospeso (scelta Andrea 17 ott: prima il C++). | — | — |
| T5 | [-] | Tappa 5 non avviata — fuori scope Tappa 1. | — | — |
| J | [ ] | Escalazione a Claude solo dopo 2 tentativi falliti o fine blocco importante; non serve. Nessun PER_CLAUDE.md. | — | — |

Regole 4.6: autore ≠ revisore indipendente; il Capo non è revisore tecnico del proprio lavoro. Review ultima (SCAMBIO_RECENTE): saltata, nessun file di codice/dati/asset testuale modificato dall'ultimo checkpoint; quota open 99,5% (soglia 50%); nessuna ROM Sega. CONTROLLO.md: blocchi [x]: 17, [~]: 4, [ ]: 4; agenti aperti: developer-builder-junior, builder-developer; decisioni aperte per Andrea: 0.

## Deleghe del turno — ruoli 4.6, percorsi out/<ruolo>/

- **capo** (out/capo/): assegna task, responsabile, revisore indipendente, percorsi scrivibili e criteri; aggrega esiti; aggiorna STATI_PROPOSTI.md; verifica review indipendenti e prove. Non scrive codice/asset.
- **builder-developer** (out/builder-developer/; fase 1 anche motore_nuovo/ e tools/): task T1-RUN completato (tuning percorsi + gate soak exit 0). Non avviare P2/P3/T5/MD0 (fuori scope Tappa 1). Attendere designer/G1 per sprite veri (P1 punto 4).
- **developer-builder-junior** (out/developer-builder-junior/): peer review T1-RUN già eseguita, esito POSITIVO (report REVIEW_T1RUN_VOCE1175_2026-10-19.md). Test, gate T1, replay deterministico, diagnosi, report. Non avviare nuove tappe.
- **designer** (out/designer/): **G1 catalogo grafico attivato** (assets_originali/CATALOGO_GRAFICO.json, check_catalogo_grafico.py), classificazione 100 PNG non catalogati, sprite veri (punto 4 T1-RUN/P1), UI/font (M). È l'unico scrittore di `Shared asset/` (non in questo turno). Aggiorna out/designer/STATUS_GRAFICO_PER_CAPO.md in place.

## Domande ad Andrea (ANDREA_REVIEW.md) — DEFAULT, si prosegue senza aspettare

- T1-Q5: DEFAULT A applicato (allinea tutti i livelli al profilo lv_02_atomico, tuning un parametro alla volta con `sh tools/gate_t1.sh soak`). Review indipendente positiva registrata; nessuna attesa di Andrea per T1-RUN.
- T1-Q3/T1-Q8: Andrea ha risposto "sì"; v91 runner Mac e misure Pi 400 in attesa di Andrea (blocco H resta [~]).
- T1-Q4: Andrea: "attivare" — G1 procede.

## Esito turno

T1-RUN [x]: gate soak exit 0, 6/6 percorsi OK, review indipendente positiva → **pronto per Andrea (T1-RUN)**. Non tutti i blocchi della tappa sono [x] (P1 [~] sprite veri dipende da designer/G1; H [~] v91 Mac + Pi 400 di Andrea; G1 [~] 100 PNG non catalogati da classificare; M [~]; MD0 [-]; T5 [-]; J [ ]): **nessun FINITO**. Nessun blocco hard (decisioni aperte per Andrea 0; nessun PER_CLAUDE.md). Prossimo: designer esegue G1 (catalogo + classificazione PNG, `check_catalogo_grafico.py` exit 0, "PNG non catalogati: 0"); P1 punto 4 si sblocca con sprite veri da G1. Note junior (non bloccanti): blocco R1 (road.h/road.cpp/game.cpp, campo tracciato) recensito col perimetro R1, non imputabile a T1-RUN; lv_02 63%/0.88 vs storico 40%/0.86 per raccordo R1, soglie invariate e guidabilità verificata.
