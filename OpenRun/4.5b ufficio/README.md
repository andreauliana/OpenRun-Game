# OpenRun v4.6 — Sglavinated Edition

Progetto in elaborazione v4.6 con schema concettuale 4.6 Ufficio con la struttura semplificata: 4 ruoli, meno file.

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
I nomi interni `V3` (`comandi/v3.sh`, `V3_BASE`) restano per compatibilita': i comandi funzionano come nella v4.
Il motore va ricompilato al primo avvio (le cartelle `build*` non sono state copiate): `cd sglavinated_src && sh UFFICIO/Officina/tools/gate_t1.sh`.
Sul Pi 400: `sh "<da aggiornare>/pi/compila_e_installa.sh"`.

### Le 3 regole d'oro
1. Un foglio, uno scrittore.
2. Solo Andrea e Claude promuovono a [x]; H e P1 vogliono l'ok di Andrea.
3. Uno script rosso rimanda il passo allo sviluppatore, senza chiamare altri modelli.

## Dove sono le cose
- `sglavinated_src/` — progetto degli agenti: `AGENTS.md`, `RICHIESTA.md`, `SCAMBIO.md`, `VISIONE.md`, `opencode.json`.
- `sglavinated_src/.opencode/agents/` — le 4 definizioni dei ruoli (modelli e permessi).
- `sglavinated_src/motore_nuovo/` — motore C++17 (src, dati, docs, test, strumenti_py).
- `sglavinated_src/UFFICIO/Dipartimento Grafico/Libreria/assets_originali/` — asset originali e materiale usabile.
- `sglavinated_src/bacheche/<ruolo>/` e `out/<ruolo>/` — bacheche (diario) e consegne di ogni ruolo. Niente LEGGIMI: le regole sono in `AGENTS.md` e in `.opencode/agents/<ruolo>.md`.
- `sglavinated_src/out/collaudatore/` — log del gate e screenshot (prima in `out/debugger/`).

## Cosa non e' stato copiato
- Filone Mega Drive (MD0 sospeso): `lr/`, `shots/`, `gpgx/`, `gstatic/`, `hud/`, `rst/`, `smoke/` e gli strumenti della hack.
- `outrun_src/`: ROM Sega.
- Cartelle `build*`, `log_agenti/`, log in radice, la copia duplicata `.opencode/agent/`, `__pycache__`.
- Dalle uscite del debugger: i file `.ppm` e le cartelle del soak (si rigenerano col gate).

## Decisioni v4.5 (default scelti, cambiabili)
1. Il lavoro dei agesnti ruoli precedenti e' in `archivio_v4/bacheche/` e `archivio_v4/out/`.
2. La panchina modelli e' in `strumenti/panchina.txt`.
3. Il @capo lavora al primo giro e poi ogni terzo giro (3, 6, 9...).
4. Grafico e ricercatore sono spenti nel ciclo base; il giro parallelo `strumenti/giro.sh` usa i 4 ruoli operativi.
5. Un file, un ruolo: `frasi.json` e `CREDITI.md` li scrive solo il grafico; il ricercatore propone in `libreria/ricerca/`.
6. `RICHIESTA.md` e' aggiornato solo nei nomi dei ruoli (@coder -> @sviluppatore, @debugger -> @developer-junior) e nei percorsi `out/collaudatore/`.

## Stato al passaggio (17 ott 2026)
Tappa 1. Gate reale OK (3 livelli, replay 2x identico, 0,09 ms/frame, soak 30000 frame). Aperti: H, P1, T1-RUN (punto 4 sprite veri nel gate, punto 8 test di Andrea su Mac e Pi 400), M, J. MD0 e T5 in attesa.
