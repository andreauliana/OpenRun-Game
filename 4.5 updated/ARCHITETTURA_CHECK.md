# Verifica architettura + runtime su Raspberry Pi 400

Pi 400: 4 core Cortex-A72 a 1,8 GHz, 4 GB RAM, uscita 1080p. Target: 50 Hz (PAL) fluido, zero rallentamenti.

## Quando serve un "emulatore"
| Pista | Runtime su Pi 400 | Emulatore? |
|---|---|---|
| Hack attuale (v87, ROM Mega Drive) | `lr-genesis-plus-gx` (baseline dei test) | si, gia' in uso: NON cambiarlo, i test `lr/` misurano quel core |
| A: motore nativo + asset nuovi | app nativa (SDL) o core libretro | no |
| B: ROM Mega Drive riscritta | core Mega Drive | si: provarla su piu' core |

Se la Pista A: il problema non e' l'emulatore ma il motore. 
Se la Pista B: confronta i core, ma resta la v87 come riferimento.

## Criteri per scegliere il motore (compila con @architetto (licenze, motori) e @cercaasset (risorse))
| Criterio | Soglia |
|---|---|
| Licenza | libera (MIT/BSD/GPL), nessun asset originale richiesto |
| Gira su Pi 4/400 | 50 fps stabili a 1080p o 720p |
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
2. Se nessuno li passa -> restare sulla hack Mega Drive e chiudere alla v87/v88.
3. Il cambio di core vale solo se il nuovo core e' piu' fluido E i test `lr/` danno gli stessi esiti sulla v87.
