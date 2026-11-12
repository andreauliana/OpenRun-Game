# REVIEW V47 (riscritta) — @developer-junior
Turno 12/11 10:03, VJ4 soak (1/6).

## VJ4 Soak
| Livello | Comando | Exit | Replay | ms/frame medio | ms/frame max | Note |
|---|---|---:|---|---:|---:|---|
| lv_01_neon | `prova_gate "UFFICIO/Officina/Test area/vj4_soak_lv01_1003" lv_01_neon soak` (dopo `cmake --build ... --target prova_gate` exit 0) | 0 | IDENTICO (2x), GATE OK | 1.881 | 51.219 (limite 20) | 30000 frame, nessun crash, speed@60 154.11, offroad frame 157 |

## VJ5 ultimo (09:55, riferimento)
- `src/road/road.cpp`+`road_segment.h`: chiusura direzione solo su curve!=0; azzeramento/scala curve nel rettilineo bivio post-correzione può reintrodurre piccolo residuo locale. Raccordi scalati se sovrapposti ok. Nessun cambio src.

gate_t1.sh non eseguito.
