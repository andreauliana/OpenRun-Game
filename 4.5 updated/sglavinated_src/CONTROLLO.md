# Controllo automatico — 2026-10-17 18:17 (generato dallo script, non modificare)

## Modelli assegnati
- capo: opencode/nemotron-3-ultra-free
- collaudatore: opencode/longcat-2.5-preview-free
- grafico: opencode/mimo-v2.6-flash-free
- ricercatore: opencode/nemotron-3.5-lightning-free
- sviluppatore: opencode/longcat-2.5-preview-free

Distribuzione:
     2 opencode/longcat-2.5-preview-free
     1 opencode/nemotron-3.5-lightning-free
     1 opencode/nemotron-3-ultra-free
     1 opencode/mimo-v2.6-flash-free

## Errori ricorrenti (ultime 300 righe di ogni log agente)
- architetto: rate limit 51, permessi negati 2, file/cartella inesistente 6
  -> architetto: controlla il percorso con 'ls' prima di leggere; niente percorsi assoluti.
- coder: rate limit 0, permessi negati 3, file/cartella inesistente 0
  -> coder: usa solo percorsi relativi e solo le tue cartelle (regola 15); non provare shell/cd.
- debugger: rate limit 0, permessi negati 4, file/cartella inesistente 0
  -> debugger: usa solo percorsi relativi e solo le tue cartelle (regola 15); non provare shell/cd.
- livelli: rate limit 27, permessi negati 0, file/cartella inesistente 0
- scenografo: rate limit 0, permessi negati 6, file/cartella inesistente 0
  -> scenografo: usa solo percorsi relativi e solo le tue cartelle (regola 15); non provare shell/cd.
- sviluppatore: rate limit 0, permessi negati 15, file/cartella inesistente 0
  -> sviluppatore: usa solo percorsi relativi e solo le tue cartelle (regola 15); non provare shell/cd.
- testi: rate limit 0, permessi negati 3, file/cartella inesistente 0
  -> testi: usa solo percorsi relativi e solo le tue cartelle (regola 15); non provare shell/cd.
- traduttore: rate limit 27, permessi negati 0, file/cartella inesistente 0

## Memoria agenti (compattazione automatica)

## Workflow
- blocchi chiusi [x]: 17; in corso [~]: 4; da fare [ ]: 3
- agenti con lavoro aperto: collaudatore sviluppatore 
- decisioni aperte per Andrea: 2
- istruzione: in pausa non servono nuove richieste; riprendi dai blocchi [~] e chiudi i file lasciati a meta.
