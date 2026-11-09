# UFFICIO — OpenRun 4.7 (base legacy 4.6 dell'08/11)

Area operativa coordinata da @capo. Capo assegna task e revisori, raccoglie prove e prepara le revisioni per Andrea; non scrive codice né asset.

```text
UFFICIO/
├── Dipartimento Grafico/
│   ├── TOOLS/
│   └── Libreria/
│       └── Shared asset/
├── Officina/
│   ├── TOOL/
│   ├── build/
│   └── Test area/
├── bacheche/<ruolo>/        diario e riassunto di ogni agente
├── out/<ruolo>/             ultimo turno e review
├── Checkpoint Revision/     dossier del capo
└── archivio_per_claude/     note vecchie e file fuori posto
```

Il motore (la "ENGINE COMPILER") e' `../motore_nuovo/`. Gli agenti scrivono solo qui dentro e in `motore_nuovo/`: nessuna cartella nuova altrove.

La migrazione graduale: sorgenti e asset attivi restano nei percorsi v4.6 descritti in `../AGENTS.md`. Non creare copie. 
