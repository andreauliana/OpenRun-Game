# Censimento motori OutRun open source (anche pseudo-3D) — licenza
# SPDX-License-Identifier: CC0-1.0 (solo il testo di questo rapporto di ricerca)
# Richiesta: voce 2 SCAMBIO.md (capo->@cercaasset) + PIANO_V3 BLOCCO 9 (setup e avvio).
# Nota: licenza di progetto da decidere (voce D1). Nessun motore e' stato scaricato o usato; solo analisi di riferimento.

## Metodo
Per ciascun motore: licenza, richiede asset originali?, gira su Pi 4/400 a 50 fps?, cosa sostituire.
Fonti: pagine GitHub lette (verificato dove indicato) + PIANO_V3 (BLOCCO 4, 8, 9, 11, Decisione "il meglio da piu' fonti").
Licenze distinte: "VERIFICATA" = letta sulla pagina/fonte; "PRESUNTA" = riportata da PIANO_V3/Andrea, da confermare.

## Tabella censimento

| Motore / Fonte | Licenza | Richiede asset originali? | Gira su Pi 4/400 a 50 fps? | Cosa sostituire | Giudizio per V3 |
|---|---|---|---|---|---|
| Cannonball (djyt/cannonball) | Non libera/custom, richiede ROM originali Sega (PRESUNTA da PIANO_V3; la pagina conferma "richiede le ROM originali" e build su Raspberry Pi; nessuna licenza OSI chiara in lettura) | SI' — tutto (sprite, audio, dati sono nelle ROM Sega) | SI' — build riuscita su Raspberry Pi (pagina), mira a frame-rate superiore all'originale | Tutti gli asset + la ROM stessa -> non adottabile come base "versione libera" | NO (solo riferimento tecnico; PIANO_V3 BLOCCO 8: non usare come base libera) |
| ZgzInfinity/OutRun | GPL-3.0 (VERIFICATA: pagina GitHub riporta "GPL-3.0 license") | SI' — tutti i grafici/audio/mappe del remake vanno sostituiti con asset originali nostri | DA VERIFICARE — C++14 + SFML 2.5.1 + OpenGL, eseguibili Windows + Linux; nessun test su Pi 4/400 effettuato in questo ambiente | Tutti gli asset; se si adopera il codice come base l'intera opera ricade sotto GPL-3.0 (valutazione licenza, voce D1) | Condizionata: possibile solo se Andrea accetta GPL-3.0 e sostituisce tutti gli asset |
| Lou's Pseudo 3D Page | Non dichiarata (solo riferimento tecnico) | N/A (non e' codice usabile) | N/A | N/A | Solo idee (matematica strada a segmenti, curve, colline, sprite scalati), mai copia |
| JavaScript Racer (J. Gordon) | Non dichiarata | N/A | N/A | N/A | Solo riferimento (ciclo di gioco, segmenti, collisioni, traffico semplice), mai copia |
| Genesis Plus GX / lr-genesis-plus-gx (emulatore) | Emulatore (core libretro) | N/A | SI' — esegue la hack v87 su Pi 400 (PAL) | N/A | Non e' un motore nuovo; esegue solo la hack derivata da ROM Sega (uso privato) |

## Conclusioni per la pista C (motore nuovo C++/SDL2)
- Nessuna fonte e' adottabile come-is per una "versione libera con asset originali":
  - Cannonball e' vincolata dalle ROM originali Sega e da licenza non libera/commerciale vietata.
  - ZgzInfinity e' GPL-3.0 (compatibile open source ma vincolante: rende GPL l'intera opera se usata come base) e richiede comunque la sostituzione di tutti gli asset.
- Raccomandazione (già decisa da Andrea, PIANO_V3 Decisione): pista C = motore nuovo in C++/SDL2, indipendente dalla piattaforma, render software 320x224 a 50 Hz, deterministico. Si prendono IDEE e TECNIChe da queste fonti (matematica strada, ciclo di gioco, traffico), MAI codice, asset, dati ROM o sprite.
- Per il Pi 4/400 a 50 fps: nessun motore censito e' stato testato qui; il nuovo motore C dovra' essere misurato sul Pi da @debugger (BLOCCO H).

## Note licenze
- GPL-3.0: se si decidesse di usare ZgzInfinity come base, l'intero progetto diventerebbe GPL-3.0 (conflitto possibile con D1 opzione A MIT/CC-BY). Decisione a Andrea (voce D1).
- Cannonball: richiede ROM OutRun (materiale Sega) -> assolutamente esclusa dalla versione libera.
