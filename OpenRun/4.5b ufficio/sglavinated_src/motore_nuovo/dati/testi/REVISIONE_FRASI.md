# REVISIONE FRASI — BLOCCO G (@testi, turno 1)

## Regole applicate (motore nuovo)
- MAIUSCOLO, max 2 righe, riga <= 16 caratteri, senza accenti, nessun contenuto sessuale.
- Nessun limite di numero di frasi (il limite 156 della v87 non si porta).
- Coerenza: Gianni dice solo frasi del suo range, Gesu' solo frasi del suo range.

## Verifica sulle 156 frasi esistenti (dati/frasi.json versione 2)
- Totale: 156 (Gianni 97, Gesu' 59, id 0-155).
- Tutte maiuscole, 2 righe ciascuna, 0 caratteri accentati, 0 righe oltre 16 caratteri, nessun contenuto sessuale rilevato.
- Coerenza personaggio <-> frase: OK (Gianni solo id Gianni, Gesu' solo id Gesu').

## Righe corrette (16 righe in 15 id)
| id | personaggio | riga1 | riga2 |
|---|---|---|---|
| 3 | gianni | TI PORTO GIAN | GRATIS, PROVA |
| 6 | gianni | FIRMA IL DEAL, | POI CIMITERO |
| 8 | gianni | SEI SOLO SENZA | UNA DIERA |
| 15 | gianni | L'ANIMA E' MIA | TE L'HO PRESA |
| 17 | gianni | SCENDI, AMICO, | IL DEAL E FATTO |
| 20 | gianni | FUMO TI ASPETTA | STAI QUI |
| 50 | gianni | HO ROBA PIU | FORTE, VUOI? |
| 56 | gianni | NESSUNO LO | SAPRA MAI |
| 63 | gianni | CHI PROVA NON | SI FERMA PIU |
| 74 | gesu | DALL'ERBA A AGO | IL PASSO E BREVE |
| 81 | gesu | A CHI HA SETE | DARO ACQUA VIVA |
| 120 | gesu | ASCIUGHERO | OGNI TUA LACRIMA |
| 123 | gianni | SULL'ERBA VAI | PIU TRA |
| 134 | gianni | FUORI STRADA? | UN TIRO, RIENTRI |
| 144 | gianni | SBANDI? TRANQUI, | SEI GIA IN VOLO |
| 150 | gesu | CHI BEVE DA ME | NON MORRA MAI |

Motivo correzioni: id 3, 6, 8, 17, 20, 134 = riga oltre 16 caratteri (accorciata); id 15 = oltre 16 caratteri + accento (GIA'); id 50, 56, 63, 123, 144 = accento-sostituto (PIU', GIA'); id 74 = oltre 16 caratteri; id 81, 120, 150 = accento-sostituto (DARO', ASCIUGHERO', MORRA').

## File aggiornati
- motore_nuovo/dati/frasi.json (versione 2, aggiunto blocco regole_testo_nuovo: maiuscole, max_righe 2, max_caratteri_riga 16, no_accenti, no_contenuto_sessuale, limite_numero_frasi false)
- motore_nuovo/dati/personaggi/gianni/fumetti.json (12 righe)
- motore_nuovo/dati/personaggi/gesu/fumetti.json (4 righe)

Consegna: out/testi/RIEPILOGO_CONSEGNA_G.md
