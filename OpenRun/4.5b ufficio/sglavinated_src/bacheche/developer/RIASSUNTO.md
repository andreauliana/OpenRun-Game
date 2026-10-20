# Riassunto developer — 20/10 ~10:48 (turno 1)
Blocco attivo: R1 (tracciati 6 piste), stato [~].
R1-1 road.cpp: FATTO (raccordo entrata/uscita default min(n/3,60), correzione deriva solo segmenti curve != 0; verificato in lettura motore_nuovo/src/road/road.cpp).
Tutti e 6 i livello.json hanno campo tracciato (carattere tema invariato, lv_02_atomico riferimento per il giro guidato).
R1-2/R1-3: dati/tracciati/tratti.json e dati/tracciati/regole.json inesistenti; scrittura nuovi file in motore_nuovo/dati/ negata — bloccati, contenuto pronto in out/developer/VOCE_SCAMBIO_1174.md.
Gate di questo turno: NON eseguito (shell permission.rejected, 'gate non eseguito').
Ultimo gate noto (riferimento, non eseguito oggi): 19/10 11:45 exit 0, GATE T1 TUTTO OK, 6/6 GATE PERCORSO OK (rettilineo 52-65%, curva max 0.37-0.88, fuori strada 0%), ms/frame medio 0.187 soak 30000 frame, replay 2x IDENTICO; peer review Junior positiva (VOCE_SCAMBIO_1175).
T1-RUN [x] DEFAULT T1-Q1 (ANDREA_REVIEW: soak exit 0, replay 2x IDENTICO, ms/frame <= 20; risposta Andrea: sì).
Andrea 19/10 09:39 (pre-tuning): exit 13, giro guidato KO 5/6, solo lv_02_atomico OK (40%/0.86) — superato dal tuning 11:45.
Scritture questo turno: solo bacheche/developer/ e out/developer/; zero scritture in motore_nuovo/ e tools/.
In attesa: sblocco scrittura dati/ e shell per creare tratti.json/regole.json e rilanciare soak; CP-005 al Capo a gate verde + review registrate.
