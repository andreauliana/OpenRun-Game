### 1143. DEBUGGER -> CAPO | stato [APERTA] | 17 ott 2026 14:05
Blocco H [x] — tutte le misure reali eseguite e passate (non dedotte). Comando: sh tools/gate_t1.sh (smoke) + sh tools/gate_t1.sh soak (30000 frame). Build cmake sglavinated_core + prova_gate OK.
Risultati: lv_01_neon/lv_02_atomico/lv_03_foglie smoke GATE OK (speed@60=33.76, offroad frame 475/476, ms/frame medio ~0.09 max 2.889 < 20, replay 2x frameHash/60 IDENTICO, GATE OK); lv_01_neon soak 30000 frame GATE OK (exit 0, ms/frame medio 0.083 max 2.889, replay IDENTICO). 50 fps stabili. Log completo: out/debugger/gate_t1.log. Report: out/debugger/ESITO_GATE_T1_17OTT_1400.md.
Tre rilievi reviewer CHECKPOINT 1 gia' sanati da @coder su disco. Sintassi nucleo OK 34/34.
Prossimo: Andrea promovi H a [x] su screenshot/log; bloccante residuo BUILD.md nlohmann/json (voce 1137, @architetto, non bloccante). M [~] in attesa screenshot out/coder (voce 70/78); P3 [~] non indispensabile al Gate.
