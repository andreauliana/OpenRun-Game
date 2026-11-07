# REVIEW_AUTO1 — screenshot gate J-A1 (auto del giocatore)

**Revisore:** @grafico · **Data:** 07/11/2026 08:36 · **Blocco:** AUTO1
**Oggetto:** i 4 screenshot PPM prodotti dal gate J-A1 di @developer-junior il 07/11/2026 08:32 (dopo l'integrazione A2 dei componenti nel Renderer) in `UFFICIO/Officina/Test area/auto1/`: `auto1_dritto.ppm`, `auto1_sx2.ppm`, `auto1_dx2.ppm`, `auto1_ribaltamento.ppm`.

## Metodo

1. Conversione PPM -> PNG con il nuovo tool `UFFICIO/Dipartimento Grafico/TOOLS/ppm_a_png.py` (solo stdlib, nessuna dipendenza): `python3 "UFFICIO/Dipartimento Grafico/TOOLS/ppm_a_png.py" "out/grafico/anteprime/auto1" <4 file>` -> **exit 0**, 4 PNG 320x224, `ls` confermato.
2. Ispezione visiva dei 4 PNG (anteprime in `out/grafico/anteprime/auto1/`, artefatti generati dai PPM di Test area).

## Esito per fotogramma

| Screenshot | Verifica | Esito |
|---|---|---|
| `auto1_dritto` | fotogramma dritto centrato (cx=160), ruote posteriori visibili (componenti `ruota_*`), ombra sotto carrozzeria, nessun artefatto di trasparenza, palette coerente con la scena | OK |
| `auto1_sx2` | fotogramma sterzo pieno a sinistra + rotazione per `rollAngle`; ruote/fumo ruotano con la carrozzeria; ombra resta piatta a terra (corretto: l'ombra non ruota) | OK |
| `auto1_dx2` | specchio coerente di sx2 (inclinazione opposta, stessi componenti) | OK |
| `auto1_ribaltamento` | auto capovolta, ruote in alto, pixel chiaramente diversi dal dritto: la rotazione (A4) si vede | OK |

## Verifiche incrociate con il gate J-A1 (@developer-junior, 07/11 08:20)

- Selezione fotogramma: mismatch 0 px su dritto/sterzo pieno sx/dx (gate `urto` + `verificaPoseAuto`), ritorno a centro 0.
- Ribaltamento: 8555 px cambiati rispetto al frame (soglia 772), 6484 vs dritto (soglia 1718).
- Gate storico `urto` invariato: replay 2x IDENTICO, ms/frame 4.68/3.91 (limite 20), ctest 27/27.
- Coerenza componenti: posizioni da `auto_giocatore.json` (A2 caricato in `game.cpp:681`), `disegna_componenti_auto.py` exit 0 nel turno G-A1.

## Osservazioni (non bloccanti, per @developer)

1. A sterzo pieno l'auto inclina di circa 26-28°: viene da `rollAngle = steeringLean * 1.5` (`game.cpp:1972`) + `STERZO_IMBARDATA` 0.09 rad (`renderer.h:44`). E' un effetto **calcolato dal codice** e coerente con la Direzione di Andrea 05/11 ("inclinazione con lo sterzo... restano effetti calcolati dal motore"): nessuna azione grafica. Se Andrea la trova eccessiva, si riduce un parametro alla volta lato @developer.
2. L'ombra resta sulla carrozzeria centrale mentre la carrozzeria ruota: comportamento corretto (ombra al suolo).
3. Nessun difeso grafico su componenti, palette o trasparenze: niente da correggere nei PNG.

## Limiti della review

Review visiva su 4 screenshot statici del gate headless: non sostituisce la prova di Andrea su Mac/SDL2 e non approva il blocco; la chiusura AUTO1 resta a review @capo + decisione di Andrea.

**Provenienza/licenza:** PNG originali del @grafico, CC0-1.0, in catalogo (`auto.giocatore.componente.*`); nessun materiale Sega; `giocatore_pose/` di Andrea non toccato.
