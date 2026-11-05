# REVIEW_PR44_05NOV — @developer-junior review incrociata su @developer (game.cpp PR #44: urti, rivali, arrivo)

- Blocco: PIST1 [~], passo J1 (RICHIESTA_APERTA, "Tocca a" 05/11).
- Regola: solo osservazioni, nessuna modifica al codice.
- File esaminati: `motore_nuovo/src/game/game.cpp`, `motore_nuovo/src/game/game.h` (working tree, non committati).

## Osservazioni — RIV punto 1 (ogni rivale i propri bivi)
- OK: `avviaGara` calcola `lunghezzeTappe_` con `lunghezzaNominaleTappa` per tutte le tappe; `inizializzaRivali` costruisce `percorso[]` dal seme (partenza = tappa del giocatore, scelta sinistra/destra a ogni bivio, ultima tappa = traguardo proprio), deterministico.
- OK: `aggiornaRivali` separa rivali fuori tappa (proseguono invisibili, zero interazione con traffico/bivi del giocatore) da rivali in tappa (rallentamento in curva con bravura, elastico solo fuori `RIVALE_LIBERO_S`, distanza minima, sorpasso laterale, auto solide vs giocatore).
- OK: `calcolaClassifica` usa `tempoArrivoS` dei rivali arrivati e stima dal percorso residuo per gli altri; ordinamento stabile e `posizioneArrivo_`.
- OK: `liberaRipartenza` sposta solo i rivali della tappa del giocatore, evitando botti alla ripartenza.
- Nota (rischio regressione): `lunghezzaNominaleTappa` duplica la logica di espansione di `caricaLivello` (tracciato + ripetizione a 2*DRAW_DIST segmenti): se `caricaLivello` cambia (nuovi tratti ponte/dune/onde di PIST1), il nominale dei rivali rischia di divergere. Da tenere sincronizzato.
- Nota: `RIVALE_MAX_TAPPE = 8` copre l'albero a 6 tappe (percorso max 6); se l'albero cresce, da rivedere.

## Osservazioni — urto / bump (Andrea 27/10 e 29/10)
- OK: `urto()` separa traffico (SPINTA 900, DOPO 0.70, effetto 0.55 s) da rivale (SPINTA 550, DOPO 0.95, effetto 0.35 s); nessun ribaltamento per contatto, `ribalta()` solo fuori strada prolungato/ciglio.
- OK: velocità del giocatore mai azzerata (floor 10% di maxSpeed), controlli sempre attivi durante `playerBump_` (spinta laterale che decade su `bumpDurataS_`).
- OK: `collisioneRivaleVicino` separata da `collisioneCartelloneVicino`; fianco a fianco clampa il giocatore senza attraversare.
- Nota: parametri bump da `dati/config.json` (`bump_rallenta`, `bump_durata_s`, `bump_spinta_laterale`); verificare i default se chiavi mancanti.

## Osservazioni — coesistenza PIST1 P3 (parapetti, stesso working tree)
- `caricaParapetti` difensiva: se `scenari/comuni/parapetto.sglraw` manca (idx -1) nessun crash, ponte senza ringhiera. G-P1 (@grafico: PNG + catalogo + raw) resta da consegnare.

## Esito
Review consegnata come osservazioni only. Nessuna modifica introdotta. Nessun build/test richiesto da J1: gate non eseguito in questo turno.
