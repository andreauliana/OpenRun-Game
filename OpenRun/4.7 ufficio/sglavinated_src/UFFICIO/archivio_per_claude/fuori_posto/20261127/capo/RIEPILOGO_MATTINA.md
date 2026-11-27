# Riepilogo mattina — 2026-11-26 (@capo, T2/2 2/12)

> Scritto in `UFFICIO/bacheche/capo/` (root vietata da AGENTS). Fonti: STATO_CORRENTE, REGISTRO (ultime righe), CP-002/003/004, ANDREA_REVIEW §1-7, STATI_PROPOSTI, handoff dev/jr/gf.

## Cosa è stato fatto (per blocco/agente)
| Blocco/Agente | Fatto | Evidenza |
|---|---|---|
| V47/V9 @developer (26/11 17:47, g40 lv02) | Nessuna mod codice; prova percorso lv02 | `prova_gate ... percorso` exit 0 (2476f,41.3s,0% fuori); smoke exit 0 medio 0.811 max 61.088ms (spike f248, gate OK); replay IDENTICO 2961894142 |
| V47/VJ5+VJ1 @developer-junior (26/11 17:46) | Regressione+VJ1+gate pali lv06 | `run_tests.sh` 27/27 exit 0; VJ1 lv01 smoke exit 0 hash 3594390208; gate pali lv06 exit 27 (soglia tarata lv01, non regressione) |
| VG3 @grafico (26/11 09:27, g212) | Verifica PNG/raw | 3 script exit 0, 0 conversioni; nessuna modifica GRAFICA/catalogo; blocchi auto/sponsor = area Andrea |
| @capo | Coordinamento, checkpoint | STATO/STATI/REGISTRO/RIASS/diario aggiornati; 0 CP chiudibili; gate non rieseguito dal capo (rif 25/11 13:04 exit 0) |

## Cosa non funziona
- **D20 BLOCCANTE (REGOLA BLOCCATA §2):** 5 hunk in `renderer.cpp` `drawPlayerCar` (2798-2816,2875-2889,2890-2905,2910-2922,2932-2952). Modifica vietata senza autorizzazione Andrea; blocca merge/chiusura V9.
- **VJ3 lv_05 50/60Hz:** FAIL deterministico 2.40% (altri 5 livelli OK). Causa non isolata; serve indagine dev/jr con log/ripetizione.
- **Gate pali lv06:** exit 27 per soglia tarata su lv01 (segnalato da jr come non regressione) — portabilità gate da valutare.
- **Pi 400:** non verificato (limite <=12 ms/frame). Nessuna dichiarazione di portabilità.
- **CP aperti senza review indipendente:** CP-002/003/004 APERTI; DECO1/F60 citati ma dossier assenti. CHK automatici SCAMBIO_RECENTE = non peer review.

## Decisioni aperte in ANDREA_REVIEW.md (le 3 più importanti)
ANDREA_REVIEW §7 riporta: **"Nessuna."** Non ci sono voci D<n> formali aperte. Le 3 decisioni/blocchi più importanti che richiedono attenzione di Andrea:
1. **D20 — autorizzare o revert hunk drawPlayerCar**: REGOLA BLOCCATA §2 violata da 5 hunk. Decisione Andrea necessaria: autorizzazione esplicita oppure conferma revert. DEFAULT: no merge/chiusura, dev prepara revert pulito.
2. **`traffico_incrocio/lato_furgone_bianco.png.png`**: filename anomalo; DEFAULT non rinominare/convertire senza RISPOSTA Andrea (segnalato in STATI_PROPOSTI/grafico).
3. **Pi 400 / prova hardware**: limite 12 ms/frame non verificato; decisione/priorità di prova su hardware reale resta ad Andrea (nessuna evidenza macOS sufficiente).

## Prossimo passo consigliato
1. **@developer (priorità alta):** ottenere RISPOSTA Andrea su D20; se nessuna autorizzazione, revert pulito dei 5 hunk drawPlayerCar, poi `run_tests.sh` exit 0 + smoke/percorso lv02 replay IDENTICO. Solo dopo: V9 ∞ lv03_foglie e indagine VJ3 lv05.
2. **@developer-junior:** pronta verifica post-revert D20; in parallelo raccogliere log/ripetizione VJ3 lv05 50/60Hz e file/riga in `REVIEW_V47.md`; valutare soglia gate pali per lv06.
3. **@capo:** non chiudere CP; registrare decisione Andrea su D20 appena arriva; mantenere registro append-only.