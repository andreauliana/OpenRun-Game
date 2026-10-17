# REA nel progetto V3 — regole d'uso

Data: 16 ott 2026. Fonte metodo: `libreria/REA_METODO.md` (copia della skill REA, licenza MIT del repo github.com/morluto/rea).

## Dove sta REA
- Skill installata sul Mac di Andrea in `~/.agents/skills/reverse-engineer-anything/` (fuori dalla cartella del progetto).
- Setup fatto con: agente = capo, skill CLI = sì, Hopper = no.
- Gli agenti del progetto NON devono leggere `~/.agents/`: la regola 21 limita il lavoro alla cartella del progetto.

## Cosa si può analizzare con REA
- Il nostro motore `motore_nuovo/` (codice e build) — ma per il codice nostro.
- Binari o pacchetti di uso e analisi.
- Fonti open source con licenza compatibile.

## Cosa NON si analizza
- file `.gen`, né loro derivati.
- musica o codice del gioco originale.

## Chi lo usa
- Andrea, a mano, dal Terminale o da Claude Code.
