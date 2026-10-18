# OpenRun 4.6 — regole operative

Queste regole si applicano agli agenti OpenCode avviati da `sglavinated_src/`. Le decisioni riservate ad Andrea restano ad Andrea. Gli agenti non leggono né modificano `../ANDREA/`, l'area personale `../` o i comandi dell'utente senza autorizzazione esplicita.

## Gerarchia

```text
sglavinated_src/
├── UFFICIO/                         # coordinamento del Capo
│   ├── Dipartimento Grafico/        # Designer / Art Director
│   │   ├── TOOLS/                  # strumenti grafici
│   │   └── Libreria/
│   │       └── Shared asset/       # asset approvati per la produzione
│   └── Officina/                    # Builder/Developer + Developer/Builder Junior
│       ├── TOOL/                    # strumenti di sviluppo
│       ├── ENGINE COMPILER/         # destinazione dei sorgenti di prodotto
│       ├── build/                   # output generato, mai modificato a mano
│       └── Test area/               # test, log e prove
└── Checkpoint Revision/             # revisioni aggregate, scrittura del Capo
```

`4.5b ufficio/` e i suoi `comandi/` restano l'area di Claude e degli strumenti dell'utente, fuori dal perimetro operativo degli agenti. `../ANDREA/` è privata.

## Migrazione graduale

La struttura 4.6 è attiva per ruoli, review, checkpoint e output di build/test. In questa prima fase i sorgenti non vengono spostati: CMake e gli strumenti esistenti usano ancora `motore_nuovo/`, `assets_originali/`, `libreria/` e `tools/`. Questi sono gli unici percorsi sorgente attivi fino alla migrazione dei riferimenti. Non creare copie parallele nelle cartelle di destinazione e non considerare vuote le nuove cartelle come fonti di produzione. I log e gli artefatti generati dal gate vanno invece in `UFFICIO/Officina/build/` e `UFFICIO/Officina/Test area/`.

## Ruoli

| Ruolo | Responsabilità | Scrittura |
|---|---|---|
| Capo | Assegna task, responsabile, revisore, percorsi e criteri; aggrega esiti; gestisce checkpoint e documenti per Andrea. Non scrive codice/asset e non sostituisce la review tecnica. | `Checkpoint Revision/`, documenti di coordinamento assegnati |
| Designer / Art Director | Produce, cura e cataloga asset; verifica provenienza/licenza; integra la ricerca di risorse libere. È l'unico ruolo che approva e aggiorna `Shared asset/`. | Percorsi grafici assegnati; in fase 1 `assets_originali/`, `libreria/` e dati UI/testo assegnati |
| Builder/Developer | Guida codice C++, integrazione, CMake e build multipiattaforma; rivede il lavoro del Junior quando designato. | Codice e configurazione assegnati; in fase 1 i percorsi attivi sotto `motore_nuovo/` e `tools/` |
| Developer/Builder Junior | Contribuisce a codice, test, strumenti e diagnosi entro task delimitati; rivede il lavoro del Builder quando designato. | Solo percorsi del task; in fase 1 test in `motore_nuovo/test/` e codice esplicitamente assegnato |

Agenti e modelli sono intercambiabili: le responsabilità seguono il ruolo assegnato, non il modello o l'identità tecnica dell'agente. Il perimetro statico OpenCode è una allowlist di base; il Capo specifica nel task i singoli percorsi scrivibili. Nessun agente amplia autonomamente il proprio accesso.

## Regole di scrittura e permessi

- Default: scrittura negata. Ogni task indica responsabile, revisore indipendente, percorsi scrivibili e criteri di completamento.
- Capo scrive in `Checkpoint Revision/`; gli altri ruoli non modificano né cancellano checkpoint o revisioni aggregate.
- Designer è l'unico scrittore di `Shared asset/`. Gli sviluppatori lo leggono e lo usano senza copiarlo o alterarlo; le richieste di modifica tornano al Designer.
- Gli sviluppatori scrivono solo il codice, i test e gli strumenti assegnati. Il Designer e il Capo leggono i materiali pertinenti, ma non scrivono codice di prodotto.
- `build/` e gli output in `Test area/` sono generati: nessuno li modifica manualmente. Gli sviluppatori possono eseguire build e test consentiti; il revisore legge diff, log ed esiti.
- Nessun ruolo cancella file o modifica aree fuori dal proprio incarico. Le allowlist OpenCode negano le scritture fuori dai percorsi del ruolo.
- L'accesso web del Designer richiede approvazione della richiesta. Non scaricare né pubblicare risorse non verificate o non approvate.

## Review e checkpoint

1. Ogni consegna registra file modificati, scopo, test e comando eseguito, esito misurabile, problemi residui e provenienza/licenza per materiale esterno.
2. La review tecnica è peer review: Builder/Developer rivede il Junior; il Junior rivede il Builder se assegnato. L'autore non è il proprio unico revisore e non approva la release.
3. Il Designer verifica formato, catalogo, provenienza e licenza prima della pubblicazione in `Shared asset/`.
4. Il Capo verifica completezza delle review, prove, esiti e decisioni di Andrea; non si autovaluta come revisore tecnico né dichiara una build verificata senza prova registrata.
5. Review negativa o incompleta: il Capo registra i rilievi nel checkpoint, assegna la correzione all'autore e richiede una nuova review indipendente. Se il revisore autorizzato non è disponibile, il checkpoint resta in attesa.

## Vincoli di progetto

- Motore C++17 deterministico: tempo a contatore di frame, RNG con seed, core indipendente da SDL; obiettivo Raspberry Pi 400 a 50 fps e 320x224.
- Nessun materiale Sega (ROM, sprite, musica, tracciati, palette o codice 68000). Codice/risorse esterni solo con licenza compatibile, fonte e attribuzione registrate.
- La musica segue `motore_nuovo/dati/audio/playlist.json`; MD0 è sospeso.
- Quota open almeno 50%. Per verificare il catalogo e la quota, usare gli strumenti esistenti sotto `motore_nuovo/strumenti_py/` solo se assegnati.
- Non cancellare file, non creare copie di sorgenti e non modificare documenti storici in `archivio_v4/`.

## Build e test

Dalla radice `sglavinated_src/`, usare un comando semplice:

- `sh tools/gate_t1.sh` — smoke test e replay deterministico sui tre livelli.
- `sh tools/gate_t1.sh soak` — smoke test più 30.000 frame.

Il gate configura e compila il sorgente ancora in `motore_nuovo/`; build, log, screenshot e altri output sono collocati in `UFFICIO/Officina/build/` e `UFFICIO/Officina/Test area/`. Riportare l'exit code e le righe misurate del log; non dedurre il successo dal codice o dal solo avvio della build. Un errore torna all'autore per la correzione.

## Memoria operativa

I materiali v4.5 in `bacheche/collaudatore/`, `bacheche/ricercatore/`, `out/collaudatore/` e `out/ricercatore/` sono storici e non assegnano permessi attuali. I nuovi ruoli usano i propri percorsi di consegna. `RICHIESTA.md` e `SCAMBIO.md` restano documenti gestiti da Andrea e Claude; gli agenti non li modificano.
