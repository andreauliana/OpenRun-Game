# Fedelta' al gioco originale: almeno 50% (richiesta di Andrea)

SPDX-License-Identifier: MIT

Autore: @architetto. Data: 15 ott 2026 (BLOCCO FED, con BLOCCO B); allineato a D1 chiusa (MIT codice / CC-BY 4.0 dati e grafiche, vedi LICENZE.md).
Percorso: motore_nuovo/docs/FEDELTA.md

Si misura la STRUTTURA e la SENSAZIONE di gioco, NON gli asset: sprite, tracciati, musica, nomi restano tutti originali (VISIONE.md). Somiglianza = stesso tipo di gioco, non copia.
Controllo: ogni voce ha Stato (si/no/parziale) e prova; punteggio = (si + 0.5*parziale) / voci; soglia 50%, obiettivo 70%.
Stato attuale: architettura e formati dati definiti (BLOCCO B), codice non ancora scritto (BLOCCO C in corso). Le voci sono quindi "parziale" (specificato, non implementato) tranne dove indicato.

| # | Tratto strutturale | Stato | Prova |
|---|---|---|---|
| 1 | guida vista da dietro l'auto, strada pseudo-3D che scorre | parziale | ARCHITETTURA.md moduli road/render (strada a segmenti, curve, colline, framebuffer 320x224); codice non ancora scritto (BLOCCO C) |
| 2 | tempo limite con checkpoint che danno altro tempo | parziale | FORMATI_DATI.md livello.json: campi `tempo_limite_frame` e `checkpoint`; logica da implementare in BLOCCO C/D |
| 3 | bivi a scelta: percorso ad albero di tappe (6 tappe e 3 finali) | parziale | proposta D3 in SCAMBIO.md (albero delle tappe); livello.json supporta `id`, `tema`, `stile` per tappa |
| 4 | traffico da sorpassare, collisioni con rallentamento | parziale | ARCHITETTURA.md modulo road (traffico base, collisione auto/traffico, fuori-strada, elementi decorativi inseriti); codice in BLOCCO C |
| 5 | Tema e ambiente diverso per tappa | parziale | FORMATI_DATI.md livello.json: `tema`, `stile` (da DIREZIONE_ARTISTICA.md), `scenario`; 3 livelli di prova previsti (BLOCCO K) |
| 6 | radio con scelta 2 brani prima della partenza, e gameplay | parziale | FORMATI_DATI.md playlist.json (radio -> elenco brani .xm, sostituibile, regola 8) |
| 7 | HUD: tachimetro, tempo, punteggio, indicatore tappa | parziale | FORMATI_DATI.md ui/hud.json (layout su framebuffer 320x224, grafica/font: @grafico BLOCCO M) |
| 8 | sensazione di velocita' a 50 Hz (curve, colline, fuori strada) | parziale | ARCHITETTURA.md vincoli (50 Hz, frame <=20 ms obiettivo <=12 ms); BUILD.md obiettivo Pi 400 e multipiattaforma; misura fps da fare (@debugger BLOCCO H) |
| 9 | schermata finale / scena di arrivo | parziale | VISIONE.md tabella evento "Fine corsa" (schermata finale a tema); non ancora progettata nei dati |
| 10 | marcia bassa/alta o equivalente | parziale | non definita nei dati attuali; equivalente candidato: modulo fx razzo B / sollevamento auto (FORMATI_DATI.md config.json moduli_fx), da decidere |

Punteggio attuale: 10 parziale × 0,5 = 5,0 / 10 = **50,0%**. Soglia 50% raggiunta. Obiettivo 70% da raggiungere con l'implementazione (BLOCCO C/D/H) e la misura effettiva (voce 8 -> si dopo test Pi 400).
Le novita' (razzo B, sollevamento auto, personaggi, parodia) si AGGIUNGONO sopra e non contano contro la soglia.
Responsabile: @architetto (compila), @reviewer (controlla a ogni checkpoint).

---

## D3 — proposta numero tappe/bivi e temi (per DECISIONI_ANDREA.md, voce D3)

Domanda: quante tappe/finali ha l'albero di bivi, da copiare come STRUTTURA (non come contenuto).

Proposta @architetto:
- Riferimento numerico di analisi (solo numeri, mai codice/asset/tracciati Sega): l'originale OutRun ha 15 tappe totali disposte su 5 percorsi, ogni percorso 5 tappe, 5 finali (fonte: Cannonball come analisi dei numeri di gameplay, FONTI.md; i percorsi/tematiche veri sono nostri).
- Struttura proposta: **albero di 5 percorsi, 5 tappe per percorso, 5 finali**. Ogni tappa ha `stile` diverso (DIREZIONE_ARTISTICA.md: sci-fi anni '50, futurista, punk, synthwave/C-wave, Windows 95 vibe, parodia societa', psichedelico/cannabinoidi).
- Per il prototipo (BLOCCO K): **3 livelli di prova**, ciascuno con stile diverso scelto da DIREZIONE_ARTISTICA.md, tema, lunghezza, seme, traffico, difficolta facile, punti show, moduli fx, scenario.
- DEFAULT usato nel frattempo (DECISIONI_ANDREA.md D3): 5 tappe per percorso con bivi, 3 livelli di prova.

Verifica su FEDELTA voce 3: conteggio tappe/finali definito nella proposta sopra; prova = proposta D3 + schema livello.json (ogni tappa = un livello.json con id/tema/stile).

Testo da riportare da @capo in DECISIONI_ANDREA.md (voce D3, riga RISPOSTA DI ANDREA):
"Confermo: albero di 5 percorsi x 5 tappe = 15 tappe, 5 finali; prototipo con 3 livelli di prova (stili diversi da DIREZIONE_ARTISTICA.md). Licenza e grafiche: vedesi voce D1."
