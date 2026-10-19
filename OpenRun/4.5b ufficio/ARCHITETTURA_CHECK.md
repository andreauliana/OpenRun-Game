# Verifica architettura + runtime su Raspberry Pi 400
Pi 400: 4 core Cortex-A72 a 1,8 GHz, 4 GB RAM, uscita 1080p. Target: 60 Hz fluido.

## Criteri per scegliere il motore (compila con @architetto (licenze, motori) e @cercaasset (risorse))
| Criterio | Soglia |
|---|---|
| Licenza | libera (MIT/BSD/GPL), nessun asset originale richiesto |
| Gira su Pi 4/400 | 60 fps stabili a 1080p o 720p |
| Dipendenze | disponibili su RetroPie/Linux |
| Asset | formato semplice (PNG/.mx/tracker), sostituibile da Andrea |
| Codice | piccolo e leggibile, build con `make`/`cmake` senza fatica |
| Manutenzione | commit recenti, issue aperte gestibili |

## Prove su Pi 400 (da fare a mano, poi riportare in SCAMBIO.md)
```
# frequenza e temperatura durante il gioco
vcgencmd measure_clock arm ; vcgencmd measure_temp ; vcgencmd get_throttled
# per ROM Mega Drive: provare lo stesso .gen su piu' core
#   lr-genesis-plus-gx (baseline) e lr-picodrive
# per motore nativo: fps a schermo (counter del motore o RetroArch: Impostazioni > Mostra fps)
```
`get_throttled` deve dare `0x0` (nessun throttling).

## Regola di decisione
1. Se un motore libero passa tutti i criteri -> Pista A, con quello.
2. Se nessuno li passa -> verificare attraverso feedback Andrea e procedere con sviluppo.