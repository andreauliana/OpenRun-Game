# Esito FORK1 (turno @developer, 23/10/2026)

Blocco: FORK1 [ ] — punti @developer 1-3 (bivio/cambio tappa fluidi, bordi strada).
Comando eseguito: sh UFFICIO/Officina/TOOL/gate_t1.sh (componenti verificati singolarmente; timeout shell 180s per durata complessiva, nessun errore).
Exit code: build cmake 0; ogni prova_gate/percorso/verifica 0.

Esito misurato:
- 6/6 livelli prova_gate + percorso: GATE PERCORSO OK (rettilineo 55-71%, curva max 0.84-0.86, fuori strada 0%).
- Gara a tappe (4 combinazioni bivio): GATE GARA OK, replay IDENTICO (determinismo preservato).
- Frequenza 60 Hz e 50 Hz: giro guidato 30.40 s identici, replay IDENTICO.
- accelerazione / crash (ribaltamento) / sterzo / curve continue / screenshot P1: GATE OK.
- ms/frame medio ~0.17, max 0.57 (limite 20).

Modifiche:
- motore_nuovo/src/road/road.h: secondiDaSceltaBivio_ + setter; curvaInizioTappa_/collinaInizioTappa_ + setter (raccordo inizio tappa).
- motore_nuovo/src/road/road.cpp: carreggiate() fattore ramo 0.6 -> 1.0 in ~1 s dopo la scelta; build() usa curva/collina inizio tappa per il raccordo; flag consumati una tantum.
- motore_nuovo/src/game/game.h: membro secondiDaSceltaBivio_.
- motore_nuovo/src/game/game.cpp: fineTappa raccorda curva/collina con la tappa precedente; update() traccia secondi da scelta e chiama setSecondiDaSceltaBivio; spinta di curva attenuata offroad (CURVA_SPINTA_OFFROAD_FACTOR 0.25); ribaltamento invariato a 2 s offroad.

Punto 2 (centroCorsia continuita' scelta) gia' fatto turno 5. Punto 3 bordi: ribaltamento solo se offroad prolungato; spinta attenuata agevola il rientro.
Restano (Junior): gate bivio_uscita, bivio_bordi, tema_transizione. Prossimo blocco: DECO1.
