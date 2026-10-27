# Esito R2 — DECO1 passo 2f (sviluppatore) — 27/10/2026 19:11

## Blocco
R2 [~], ordine esecutivo: 1d fatto -> DECO1 passo 2f -> RIV -> PIST1. DECO1 2a-2e gia' completate (developer), passo 2f = questo turno.

## Modifica
File: motore_nuovo/src/game/game.cpp, `Game::caricaOggettiStrada()` (DECO1 2c estesa a 2f, punti 1-3).
- Passo oggetto 4000 -> 2000 unita' mondo (una istanza ogni 2000 invece di 4000).
- Scala oggetto 1.0 -> 4.0 default (oggetti bordo strada visibili a bordo strada; senza campo altezza_mondo/scala nel manifest della variante si usa 4.0).
- Nessun oggetto negli ultimi `Road::BIVIO_SEG` segmenti quando il bivio e' attivo (stop a `bivioInizioZ()`), bivio pulito, niente elementi in mezzo alla carreggiata.
Cartelloni: gia' coperti da `preparaBivio()` (gara) e densita' ~26-27% (target D2b 20-30%).

## Build
`cmake --build UFFICIO/Officina/build --target prova_gate` exit 0.

## Gate
`sh UFFICIO/Officina/TOOL/gate_t1.sh` (modo smoke) exit 0, 27/10/2026 19:11.
- GATE T1: TUTTO OK
- 6/6 livelli: smoke exit 0, percorso GATE PERCORSO OK (rettilineo 55-71%, curva max 0.72-0.98, fuori strada 0%)
- replay 2x IDENTICO
- ms/frame medio <= 0.87, max 1.16 (limite 20)
- cartelloni densita' 26-27% (target 20-30%)
- gara 4 combinazioni OK, replay IDENTICO
- frequenza 60 Hz OK, 50 Hz OK
- screenshot P1 OK
- Screenshot lv_01 (UFFICIO/Officina/Test area/screenshot_p1/) e lv_03 (UFFICIO/Officina/Test area/screenshot_lv_03/) prodotti.

## Riferimenti
- ANDREA_REVIEW.md: D18 chiusa (27/10), R2/DECO1 2f/RIV/PIST1/FIN non aspettano D18 né D19 — si lavora senza altri ok.
- RICHIESTA_APERTA.md BLOCCO R2: passo per turno; dopo 1d -> DECO1 2f -> RIV -> PIST1.
- T1-RUN [~], H [~] in attesa Andrea (ok runner Mac v91, poi fps/temperatura Pi 400); nessun lavoro Pi 400/RetroPie prima di ok Mac.

## Prossimo
RIV (bump urto rivale, sprite rivali dal catalogo, comportamento realistico), poi PIST1 punto 1.
