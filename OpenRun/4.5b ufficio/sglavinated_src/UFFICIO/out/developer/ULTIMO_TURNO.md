Blocco/passo: V47 V6 Core libretro (RetroPie) — allineamento con runner SDL2 (un passo).
File cambiati:
- motore_nuovo/src/libretro/sglavinated_libretro.cpp (notifica SET_SYSTEM_AV_INFO con timing 50/60 Hz da config dopo avvio)
- UFFICIO/bacheche/developer/diario.md (riga turno)
- UFFICIO/bacheche/developer/RIASSUNTO.md (riscritto)
- UFFICIO/out/developer/ULTIMO_TURNO.md (riscritto)

Comandi ed exit code:
- cmake -S motore_nuovo -B UFFICIO/Officina/build -DBUILD_LIBRETRO=ON -> exit 0
- cmake --build UFFICIO/Officina/build --target sglavinated_libretro -> exit 0

Esito: V6 parita' presente in codice: griglia/3-2-1, pausa Start, 50/60 Hz da config+timing frontend aggiornato, musica .xm/radio/reazioni, salvataggi nome/classifica (cwd=radice), flusso Splash->Menu->Gara->FineTempo->Risultati.
Gate: non eseguito.
Prossimo: V47 V9 ∞ fluidita' lv_02_atomico.
