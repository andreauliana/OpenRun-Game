# Riassunto developer — 23/10/2026 turno 6

- Gate T1 componenti: build cmake exit 0; 6/6 livelli prova_gate+percorso exit 0 (GATE PERCORSO OK, rettilineo 55-71%, giro guidato OK, 0% fuori strada); gara 4 combinazioni OK; frequenza 60/50 Hz OK (30.40 s identici); replay 2x IDENTICO; ms/frame max 0.57 (limite 20).
- FORK1 [ ] punti @developer 1-3 fatti: fattore ramo -> 1.0 in ~1 s (road.cpp carreggiate + secondiDaSceltaBivio), raccordo curva/collina tra tappe (game.cpp fineTappa + road.cpp build curva/collina inizio tappa), spinta curva attenuata offroad (CURVA_SPINTA_OFFROAD_FACTOR 0.25) per rientro; ribaltamento a 2 s offroad (OFFROAD_CRASH_S, gia' presente).
- Punto 2 centroCorsia continuita' gia' fatto (turno 5). Punto 3 bordi: ribaltamento solo se offroad prolungato; spinta attenuata agevola rientro.
- Blocco attivo: FORK1 (gate bivio_uscita/bivio_bordi/tema_transizione sono Junior); poi DECO1.
- Scritture: motore_nuovo/src/road/{road.cpp,road.h}, motore_nuovo/src/game/{game.cpp,game.h}, diario.md, RIASSUNTO.md, out/developer/.
