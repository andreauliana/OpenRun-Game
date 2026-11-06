# Riassunto developer — 06/11/2026 08:43 (turno 1)
- GATE T1: TUTTO OK 6/6 percorsi, ms/frame max 3.909, replay 2x IDENTICO
- RIV punto 1: inizializzaRivali() RNG seed deterministico, scelte bivio differenziate (già fatto)
- Aggiunto campo 'larghezza':1.0 a rettilineo_corto in tratti.json (dati PIST1)
- Fix colline: risolto double integration in road.cpp, strada non sale più "nel cielo"
- G-P1 pending: @grafico parapetto.png/guardrail catalogo
- H: pending fps/temperatura su Pi 400 (attesa Andrea v91)
- In attesa: J1/J2 @developer-junior, @grafico G-P1
- Blocco R1: game.cpp modificato per RIV punto 1 determinismo
- Turno 1: fix double integration colline, gate smoke OK lv_01-03