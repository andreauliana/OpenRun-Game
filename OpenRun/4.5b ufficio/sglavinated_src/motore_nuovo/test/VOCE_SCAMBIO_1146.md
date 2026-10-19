### 1146. @developer -> CAPO | stato [~] | 17 ott 2026
Eseguito da @developer (solo build/test; nessun file di prodotto toccato, regola debugger).

- `sh tools/gate_t1.sh` (smoke 3 livelli + replay 2x): exit 0 su lv_01_neon, lv_02_atomico, lv_03_foglie (out/debugger/gate_t1.log).
  speed@60=33.76 su tutti e tre; offroad raggiunto frame 476 / 476 / 475; ms/frame medio 0,071, max 0,109 / 0,139 / 0,157 (limite 20, obiettivo 12); replay 2x IDENTICO su ogni livello.
  Casi estesi T1-RUN punto 5 già presenti in prova_gate.cpp e verificati: avvolgimento playerZ oltre totalLen_ -> wrapCount=4 (stimaTotalLen 55496-57599, replay IDENTICO); inattivita 900 frame senza input -> maxBollaDopoIdle=4655 (soglia 100), maxPersonaggioDopoIdle=6497, replay IDENTICO.
- `sh tools/gate_t1.sh soak` (30000 frame lv_01_neon): exit 0; speed@60=33.76; offroad frame 476; ms/frame medio 0,071 max 0,713 (limite 20); replay 2x IDENTICO.
- Esito complessivo: `GATE T1: TUTTO OK`, exit 0.
- `python3 motore_nuovo/strumenti_py/indice_assets.py` (punto 6): 293 file indicizzati, lacune 4/11 già note (traffico/altri veicoli, personaggio Gianni, personaggio Gesu', UFO) — non difetti, da coprire con asset grafici.
- H [~]: chiuso salvo T1-RUN punto 8 di Andrea (v91 sulla guida + fps/temperatura Pi 400, `vcgencmd`; non eseguibili qui, regole 9/21). Non promuovere H a [x] senza v91.
- Stati: H [~] · T1-RUN [~] (punti 1-4 in carico a @developer e @developer-junior; punto 5 verificato, punto 6 eseguito). Rilievi reviewer gestiti in precedenza; quota open 99.8% >= 50% (out/debugger/manifest.json MIT).
- Prossimo: @capo incolla VOCE_SCAMBIO_1146.md in SCAMBIO.md e promuove stati; Andrea esegue punto 8 (sglavinated_runner su Mac, v91 in FEEDBACK_ANDREA.md, fps/temp su Pi 400/RetroPie).
