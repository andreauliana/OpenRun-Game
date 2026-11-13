# Open Run nel browser (pagina 404 di andreauliana.com)

Lo stesso motore del desktop, compilato con Emscripten (WebAssembly). Il nucleo non cambia:
le differenze stanno solo in `src/runner/main.cpp`, dentro `#ifdef __EMSCRIPTEN__`.

## Cosa c'è qui
- `build_web.sh` — compila e prepara `web/sito/` (non va nel repo, si rigenera).
- `blocco.html` — il blocco del gioco (schermo, "click or press any key", barra home/tasti/fullscreen).
- `openrun.css`, `avvio.js` — stile e avvio del blocco, uguali per tutte le pagine.
- `wordpress/404.php` — il 404 del tema `au2026` con il gioco al posto dell'immagine.
- `404.html` — la stessa cosa come pagina a sé, per un sito senza WordPress.

## Compilare (Mac)
1. Una volta sola: `git clone https://github.com/emscripten-core/emsdk ~/emsdk && cd ~/emsdk && ./emsdk install latest && ./emsdk activate latest`
2. Ogni volta: `source ~/emsdk/emsdk_env.sh` e poi `sh motore_nuovo/web/build_web.sh` (dalla cartella `sglavinated_src`).
3. Prova in locale: `cd motore_nuovo/web/sito && python3 -m http.server 8000` e apri http://localhost:8000/404.html

## Mettere sul sito (WordPress, tema au2026)
- `sito/openrun/` va dentro il tema: `wp-content/themes/au2026/openrun/`.
- `sito/404.php` sostituisce `404.php` del tema (tieni una copia del vecchio).
- Il gioco si carica solo dopo il clic: chi non gioca scarica pochi KB.

## Come si comporta nel browser
- Si parte con un clic o un tasto (i browser fanno partire l'audio solo così).
- EXIT del menu porta alla home del sito; Esc esce dallo schermo intero, non chiude il gioco.
- Classifica e nome restano salvati nel browser (IndexedDB), anche ricaricando la pagina.
- Pad: SDL usa la Gamepad API del browser (da provare su un pad vero). Niente comandi touch.
- Dati: `openrun.data` contiene `motore_nuovo/dati` senza i `.md` e senza il brano non usato.
