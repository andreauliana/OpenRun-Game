# OpenRun v4.6 — Alpha Edition

Progetto di ricerca grafica e sviluppo tramite AI di andreauliana.com in progresso v4.6 con nuova architettura 'Ufficio' con 4 ruoli.

```text
UFFICIO/
├── Dipartimento Grafico/
│   ├── TOOLS/
│   └── Libreria/
│       └── Shared asset/
└── Officina/
    ├── TOOL/
    ├── ENGINE COMPILER/
    ├── build/
    └── Test area/
```

## Avvio
```
cd "$HOME/<percorso folder>"
sh comandi/v3.sh start
```

Stato e stop: `sh comandi/v3.sh stato` e `sh comandi/v3.sh stop`. Pannello su http://localhost:8090 (la v4 usa la 8089).
I nomi interni `V3` (`comandi/v3.sh`, `V3_BASE`) restano per compatibilita': i comandi come v4.
Il motore va ricompilato al primo avvio (le cartelle `build*` non sono state copiate): `cd sglavinated_src && sh UFFICIO/Officina/tools/gate_t1.sh`.
Sul Pi 400: `sh "<da aggiornare>/pi/compila_e_installa.sh"`.

### Le 3 regole d'oro
1. Un foglio, uno scrittore.
2. Solo Andrea e Claude promuovono a [x]; H e P1 vogliono l'ok di Andrea.
3. Uno script rosso rimanda il passo al @developer, senza chiamare altri modelli.

## Dove sono le cose
- `sglavinated_src/` — progetto degli agenti: `AGENTS.md`, `RICHIESTA.md`, `SCAMBIO.md`, `VISIONE.md`, `opencode.json`.
- `sglavinated_src/.opencode/agents/` — definizioni ruoli.
- `sglavinated_src/motore_nuovo/` — motore C++17 (src, dati, docs, test, strumenti_py).
- `sglavinated_src/UFFICIO/Dipartimento Grafico/Libreria/assets_originali/` — asset originali in uso.
- `sglavinated_src/bacheche/<ruolo>/` e `out/<ruolo>/` — bacheche (diario) e consegne di ogni ruolo. Le regole sono in `AGENTS.md` e in `.opencode/agents/<ruolo>.md`.
- `sglavinated_src/UFFICIO/Officina/Test area/` — log del gate e screenshot (prima in `out/collaudatore/` e `out/debugger/`).

## Cosa non e' stato copiato
- `outrun_src/`: ROM Sega.
- Cartelle `build*`, `log_agenti/`, log in radice, la copia duplicata `.opencode/agent/`, `__pycache__`.
- Dalle uscite del vecchio debugger: i file `.ppm` e le cartelle del soak (si rigenerano col gate).

## Decisioni v4.5 (default scelti, cambiabili)
1. Il lavoro dei agesnti ruoli precedenti e' in `archivio_v4/bacheche/` e `archivio_v4/out/`.
2. La panchina modelli e' in `strumenti/panchina.txt`.
3. Il @capo lavora al primo giro e poi ogni terzo giro (3, 6, 9...).
4. Ruoli 4.6: @capo, @developer, @developer-junior, @grafico (stessi nomi in AGENTS.md, `.opencode/agents/`, `bacheche/`, `out/` e negli script). Il ciclo base fa girare @developer, @developer-junior e @grafico (per escludere il grafico: `AGENTI_ATTIVI="developer developer-junior"`). Il giro parallelo `strumenti/giro.sh` usa @developer, @developer-junior e @grafico. Il @ si usa solo davanti al nome di un ruolo: negli header `## BLOCCO` di RICHIESTA.md e' il tag che gli script leggono per scegliere chi lavora.
5. Un file, un ruolo: `frasi.json` e `CREDITI.md` li scrive solo il @grafico.
6. `RICHIESTA.md` e' aggiornato solo nei nomi dei ruoli e nei percorsi.

## Riserva locale con Ollama o llama.cpp (lavoro minimo, zero crediti)
Quando i crediti gratuiti di OpenCode finiscono su tutta la panchina, il ciclo non si ferma: tutti i ruoli passano al modello locale Qwen3 4B Instruct (contesto 16k, 4 thread). E' la versione senza "ragionamento": su CPU Intel Qwen3.5 pensava per minuti anche per rispondere una parola. In locale gira **un agente alla volta** e dopo ogni turno c'e' una **pausa di 10 minuti**, cosi' il Mac non va a mille. Dopo 6 ore si riprova con i modelli free e il server locale si spegne.

Motore: **Ollama** se e' installato (modello `ollama/openrun-locale`), altrimenti **llama.cpp** (`locale/openrun-locale`). Per scegliere: `LOCALE_MOTORE=ollama` oppure `LOCALE_MOTORE=llamacpp`.

Preparazione (una volta sola):
1. Ollama: `curl -fsSL https://ollama.com/install.sh | sh` (funziona anche su macOS 12 Intel, senza l'app). Oppure llama.cpp: `brew install llama.cpp`, o compilato a mano (sotto).
2. `sh strumenti/modo_locale.sh prepara` (scarica circa 2,5 GB; accende da solo `ollama serve` se non e' gia' acceso; log in `sglavinated_src/locale.log`).
3. `sh strumenti/modo_locale.sh stato` deve dire "riserva locale: pronta".

Comandi utili:
- `sh strumenti/modo_locale.sh entra fisso`: solo locale, finche' non lanci `esci`.
- `sh strumenti/modo_locale.sh esci`: torna ai modelli free di prima e spegne il server (solo se l'ha acceso lo script).
- Variabili: `RIPOSO_LOCALE` (secondi di pausa, default 600), `RISERVA_ORE` (default 6), `RISERVA_LOCALE=0` per disattivare la riserva, `LOCALE_CTX` (default 16384), `LOCALE_THREAD` (default 4).
- Piu' qualita' (e piu' calore): `LOCALE_OLLAMA=qwen3.5:9b LOCALE_CTX=32768 sh strumenti/modo_locale.sh prepara` (con llama.cpp: `LOCALE_HF=unsloth/Qwen3.5-9B-GGUF:Q4_K_M`).

llama.cpp compilato a mano (Mac vecchi dove brew fallisce; servono cmake e gli strumenti Xcode, gli stessi del gioco):
```
cd ~/Documents && git clone --depth 1 https://github.com/ggml-org/llama.cpp
cd llama.cpp && cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j 4 --target llama-server
```
Lo script trova da solo `~/Documents/llama.cpp/build/bin/llama-server` (altrimenti `LLAMA_SERVER=/percorso/llama-server`).

Su un Mac Intel il modello gira sul processore: e' lento, per questo restano la pausa e un solo agente alla volta. Il modello locale e' molto piu' debole dei free: va bene per passi piccoli, diario, test e documenti, non per modifiche C++ grandi.

## Stato al passaggio (17 ott 2026)
Tappa 1. Gate reale OK (3 livelli, replay 2x identico, 0,09 ms/frame, soak 30000 frame). Aperti: H, P1, T1-RUN (punto 4 sprite veri nel gate, punto 8 test di Andrea su Mac e Pi 400), M, J. MD0 e T5 in attesa.
