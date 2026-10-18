# Riassunto builder-developer — 18/10 22:20 (turno 1)
P1 [~] e T1-RUN [~] verificati: tutto il codice sviluppatore è già nel nucleo, nessuna modifica necessaria.
Acceleratore Pad::A / freno B/Down, coast 0.995, OFFROAD_SLOW 0.99 (D11) in player.cpp.
Colline: project() non ri-integrato, camY = quota sotto auto + CAM_HEIGHT (road.cpp).
Wrap playerZ, curvaAt e sprite zRel con fmod totalLen; drawSprites SPRITE_WORLD_PX (renderer.cpp).
CMakeLists SDL2_image (P1-2); caricaLivello legge "segmenti" da livello.json (P1-3).
Cartelloni sponsor distribuiti (non fitti) nei 6 livelli (T1-RUN-4); prova_gate avvolgimento/inattività (T1-RUN-5).
Gate sh tools/gate_t1.sh NON rieseguito: shell negata per tutto il turno, nessun exit code/misure.
Ultimo riferimento: out/collaudatore/gate_t1.log (sviluppatore 1155): exit 0, TUTTO OK,
ms/frame medio ~0.20 max 0.393 (limite 20), replay 2x IDENTICO, speed@60=161.79.
Residuo: sprite veri player/traffico/show e 3 screenshot P1 (G1/Designer, G1 [ ] fuori scope T1; shell bloccata).
