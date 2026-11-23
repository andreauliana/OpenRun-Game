# RIASSUNTO developer (max 12 righe)
- Data/ora: 23/11/2026 08:25 (Turno 2).
- Blocco: V9 ∞ fluidita' lv_05_spray (trentaduesimo giro).
- File codice: `motore_nuovo/src/road/road.cpp` (Road::project: `idx = (startIdx+i)%nSeg` e l'indice `prev = (idx+nSeg-1)%nSeg` facevano due divisioni intere per segmento, centinaia al frame; sostituiti con indice incrementale nell'incremento del `for` (`idx = (idx+1>=nSeg)?0:idx+1`, scatta anche sui `continue`) e `prev = idx==0?nSeg-1:idx-1`. Stessi idx/prev, stessi float di x/dx/curve e screenX/screenY, resa e replay IDENTICI, frameHash 0 4250821778 invariante).
- Build: `cmake --build "UFFICIO/Officina/build" --target prova_gate --parallel 4` exit 0.
- Baseline pre: lv_05_spray smoke exit 0 "GATE OK" medio 0.751 max 6.881 ms (frame 195), replay IDENTICO, frameHash 0 4250821778; percorso exit 0 "GATE PERCORSO OK" (2920 frame, 48.7 s, 0% fuori).
- Post smoke: lv_05_spray medio 0.722 max 3.534 ms (frame 172), 10/10 frameHash identici al baseline, replay IDENTICO, frameHash 0 4250821778.
- Percorso lv_05_spray: exit 0 "GATE PERCORSO OK" (2920 frame, 48.7 s, 0% fuori).
- Gate: `sh UFFICIO/Officina/TOOL/gate_t1.sh` eseguito 23/11/2026 08:25 -> exit 0 "GATE T1: TUTTO OK", 6/6 "GATE PERCORSO OK" (conteggi giro identici: 1989/2476/2392/2125/2920/2125; smoke lv_05 medio 0.722 max 3.865 ms frame 103; nessuna riga FALLITO).
- Esito: frameHash invariato, replay IDENTICO, nessuna regressione; ms/frame ben sotto 8. Prossimo V9 ∞ lv_06_desktop (trentaduesimo giro).
