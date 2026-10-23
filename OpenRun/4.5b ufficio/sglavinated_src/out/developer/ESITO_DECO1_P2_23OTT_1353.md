# Esito DECO1 punto 2 (sviluppo) — 23/10/2026 13:53

Blocco: DECO1 [~], compito @developer 2: il motore legge da tema_grafico di ogni livello lo skyline e gli oggetti_bordo_strada.

File modificati: motore_nuovo/src/game/game.h, motore_nuovo/src/game/game.cpp.
- game.h: membri skylineId_ e oggettiBordoStrada_ con accessori skylineId() / oggettiBordoStrada().
- game.cpp: leggiTemaDaFile() legge tema_grafico.skyline e tema_grafico.oggetti_bordo_strada da livello.json (lettura una volta in caricaLivello, nessuna allocazione nel ciclo di gioco).

Comando eseguito: sh UFFICIO/Officina/TOOL/gate_t1.sh
Exit code: 0 (tutti i passaggi exit 0). Log: UFFICIO/Officina/Test area/gate_t1.log.
Esito: GATE T1: TUTTO OK; 6/6 percorsi GATE PERCORSO OK (lv_01..lv_06, rettilineo 55-71%, curva max 0.72-0.98, fuori strada 0%); replay 2x IDENTICO; ms/frame max 0.321 (limite 20); gara 4 combinazioni OK; frequenza 60 Hz e 50 Hz OK; screenshot P1 OK.

Nota: il disegno dello skyline e degli oggetti a bordo strada resta in attesa che @grafico consegni PNG e conversioni raw e aggiorni CATALOGO_GRAFICO.json (DECO1 punto 1); solo @grafico scrive catalogo e Libreria. I file raw attesi in motore_nuovo/dati/sprite_raw/ (skyline/, oggetti/).
