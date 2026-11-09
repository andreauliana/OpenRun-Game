# REVIEW_PERC1 — @developer-junior (J4, riscritto 08/11 13:23)
Riferimento: passi D2/D3/D4 presenti in dati/codice (traffico per tratto, frecce/paesaggio, galleria lv_02/lv_05).
## Cosa va
- D2: gate `traffico` passa (J2 verificato): densità autostrada > ponte in tappa 1, deterministico.
- D4 galleria: tratti `galleria` presenti in lv_02_atomico e lv_05_spray (260 segmenti ciascuno), screenshot `UFFICIO/Officina/Test area/perc1_j3_galleria_lv_02_atomico.ppm` e `lv_05_spray.ppm`.
## Cosa non va / rilievi
- Placement oggetti decorativi dentro galleria: 187 (lv_02), 162 (lv_05) con `worldZ` in `galleriaAt`. D4 richiede “niente oggetti/cartelloni/alberi dentro”: se il Renderer li omette va bene visivamente, ma il placement resta sporco; cartelloni non verificabili da API test pubblica.
- `gate_tratti` fallisce per `espansione=NO` pre-esistente su lv_02_atomico e lv_05_spray (non introdotto da J3): ctest 26/27, test 27 KO.
## Miglioria piccola proposta per D5
- Filtrare in `caricaOggettiStrada`/paesaggio la generazione o il posizionamento degli oggetti quando `road_.galleriaAt(worldZ)` è true (e analogamente per cartelloni se applicabile), così il dato è coerente col rendering D4 e J3 può diventare un check bloccante.