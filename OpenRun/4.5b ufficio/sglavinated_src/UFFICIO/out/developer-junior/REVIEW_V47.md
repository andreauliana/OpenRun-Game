# REVIEW V47 (riscritta) — @developer-junior
Turno 11/11 13:17, VJ5 ∞.

## Cambio rivisto
- `motore_nuovo/src/libretro/sglavinated_libretro.cpp`: aggiornamento AV info in `avviaGioco` (commit f64bfdd).

## Rilievi
- `sglavinated_libretro.cpp:191-197` (`avviaGioco`): dopo il caricamento si fa `retro_get_system_av_info(&av)` e si rimanda lo stesso struct con `RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO` senza modificarlo. Il commento afferma di voler aggiornare il timing in base a `config.json` (50/60 Hz) se diverso dal default, ma non viene letto `leggiFrequenzaDaConfig()` né aggiornato `av.timing.fps`. È un no-op ridondante (e potenzialmente fonte di confusione). Due opzioni: (1) rimuovere il blocco se non serve, (2) applicare realmente la frequenza da config (aggiornando `av.timing.fps` e, se necessario, gli altri campi) prima di inviarla al frontend. Attualmente non fa quanto il commento dichiara.
- (precedente) `sglavinated_libretro.cpp:185-189`: se `avviaGara()` fallisce si passa a `init()` poi `mostraMenu()`; se ha successo resta in gara preparata ma si chiama comunque `mostraMenu()` - dipendenza da macchina a stati da verificare.
- (precedente) `sglavinated_libretro.cpp:258-261` (`retro_reset`): reset forza `mostraMenu()` anche dopo esito gara; assicurare stato pulito.
- (precedente) `sglavinated_libretro.cpp:270-272`: combo `Start+B` intercettata prima di `update()` può interferire con pausa "Start da solo".
- (precedente) `sglavinated_libretro.cpp:111-122`: nessun `RETRO_DEVICE_KEYBOARD` (pausa/inserimento nome). Considerare callback tastiera o documentare solo pad.

## Prova
- `cmake --build UFFICIO/Officina/build --target prova_gate` exit 0 (11/11 13:17, prima della review).
- gate_t1.sh non eseguito (non richiesto).
