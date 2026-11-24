# STATI PROPOSTI — @capo (Turno 2, ora 3 di 12, 2026-11-24)

## Contesto letto
- TAPPE, ANDREA_REVIEW (precedenza assoluta; nessuna RISPOSTA DI ANDREA; G1 ANNULLATO/sponsor solo Andrea; legacy 08/11+vocabolario intoccati; decisioni aperte 0), DIREZIONE_ARTISTICA, CONTROLLO, AGENTS 4.7, RICHIESTA_APERTA, SCAMBIO_RECENTE, STATO_CORRENTE, REGISTRO_REVISIONI (coda), handoff grafico/dev/jr, STATUS_OFFICINA (arretrato).

## Tappa attiva / gate
- **MIG1 [~] + V47 [~] continuo** (passi ∞). Gate: ctest verde, gate_t1 exit 0, replay IDENTICO, Pi 400 <=12 ms/frame. CP-006 CHIUSO (no riaprire). Nessun CP con **gate+review indipendenti** registrati in dossier → nessuna chiusura. SCAMBIO_RECENTE CHK auto [APERTE]=controlli automatici, non peer review. Gate non rieseguito dal capo.

## Stati proposti e deleghe

| Task | Responsabile | Revisore indipendente | Criteri completamento | Esito verifiche (handoff/log citati) | Stato proposto |
|---|---|---|---|---|---|
| V9 ∞ lv_01_neon 36° | @developer (`UFFICIO/out/developer/ULTIMO_TURNO.md`) | @developer-junior | gate_t1 exit 0; percorso/smoke replay IDENTICO; ms/frame riportati; se >8 ms Mac alleggerire senza cambiare resa/replay | Handoff dev 24/11 12:52: lv_06_desktop 35° ok, smoke/percorso pre/post exit 0, replay IDENTICO frameHash 3627137594, medio 0.675 max 2.961 ms; gate_t1 ref 24/11 10:03 exit 0 (capo non riesegue) | IN CORSO |
| VJ5 ∞ review prossimo diff src (≠ roadside-markers) | @developer-junior (`UFFICIO/out/developer-junior/ULTIMO_TURNO.md`, `REVIEW_V47.md`) | @developer se fix / @capo verifica handoff | `run_tests.sh` exit 0 prima prova; review file/riga in `REVIEW_V47.md`; nessuna modifica src | Handoff jr 24/11 12:55: roadside-markers ok, run_tests 27/27 exit 0, prova_gate lv_01 percorso exit 0, no src (capo non riesegue) | IN CORSO |
| VG3 ∞ PNG vs `.sglraw` scenari | @grafico (`UFFICIO/out/grafico/STATUS_GRAFICO_PER_CAPO.md`, `GRAFICA/scenari/`) | @capo verifica catalogo/licenza/provenienza | Solo script catalogo; PNG più nuovo→converti; no ritocco area Andrea; aggiornare STATUS; no risorse esterne non verificate | Handoff gf 24/11 13:20 giro 183: 0 conversioni; vg3_date exit 1 (solo flag Andrea noto), vg3_raw_senza_catalogo exit 0, vg3_png_non_catalogati exit 0, autotest 18 OK (capo non riesegue) | IN CORSO |
| Coordinamento CP/registro | @capo (`UFFICIO/Checkpoint Revision/`, `UFFICIO/out/capo/`) | — | REGISTRO append-only; STATO/STATI/RIASS/diario allineati; chiusura CP solo con gate+review indie in dossier | Nessun CP chiudibile; CP-002/003/004 APERTI; DECO1/F60 citati APERTI (dossier assenti) | APERTO |

## Blocchi / decisioni necessarie
- DEFAULT Andrea (non richiedere, nessuna RISPOSTA DI ANDREA): `traffico_incrocio/lato_furgone_bianco.png.png` doppia est. — **NON** rinominare/convertire senza RISPOSTA.
- Pi 400 non verificato (blocco H sospeso; gate richiede <=12 ms/frame su Pi 400).
- Peer review indie non registrate in dossier per step V47 recenti → nessuna chiusura CP.
- STATUS_OFFICINA arretrato (18/11); usati handoff dev/jr.
- Nessun PER_CLAUDE. Nessun FINITO (∞). Nessuna domanda nuova ad Andrea.