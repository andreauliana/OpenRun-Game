# Riepilogo consegna BLOCCO N (studio di fattibilita ROM Mega Drive .md) — @architetto

Progetto: Outrun Sglavinated Edition v3 (open), motore nuovo (pista C).
Data: 16 ott 2026. Agente: @architetto.
Stato RICHIESTA.md: BLOCCO N rimane [ ] — bloccato in attesa che C giri su Pi 400 con toolchain (regola 9); nessun compilatore C++ in questo ambiente (regola 21).

## File prodotto

| File | Percorso | Licenza | Compito |
|---|---|---|---|
| FATTIBILITA_ROM_MD.md | motore_nuovo/docs/FATTIBILITA_ROM_MD.md | MIT (SPDX) | Studio fattibilita ROM Mega Drive originale (.md) dal v3: tabella go/no-go, stima blocchi N1-N6 (v90-v95), toolchain SGDK, limiti hardware 68000 a 50 Hz PAL, nulla di Sega |

## Contenuto del documento

- Cosa si riusa dal v3 (logica strada, auto, traffico, mixer, frame counter/RNG, show, dati livelli) e cosa va riscritto (res VDP, driver sonoro MD, joypad, convertitore JSON->binari, visual fumetti).
- Toolchain: SGDK licenza MIT (fatto verificato: readme ufficiale GitHub Stephane-D/SGDK, 16 ott 2026); GCC/libgcc GPL3 + eccezione runtime; m68k-elf-gcc binari solo Windows, su Mac/Pi da installare [C]; Java richiesto per i tool custom.
- Go/No-Go provvisorio: go solo se toolchain installa e stima frame <= 20 ms (obiettivo <= 12 ms) su 68000; altrimenti no-go, si resta su Pi 400/PC.
- Stima blocchi: N1 toolchain + ROM vuota su Genesis Plus GX; N2 strada su VDP; N3 convertitore dati; N4 driver sonoro; N5 gameplay completo; N6 test determinismo/fps. Totale stimato ~6 blocchi.

## Fatti verificati vs ipotesi

Fatti: licenza SGDK MIT; N facoltativo solo dopo C su Pi 400; v3 gia' pensato per agevolare il port (virgola fissa, 320x224, 50 Hz, determinismo, dati separati); nessun compilatore qui (regola 21).
Ipotesi [C]: toolchain su Mac/Pi 400, budget frame su 68000, limiti sprite/VRAM, costo hash frame.

## Strumenti

Non rieseguibili in questo ambiente (shell negata, regola 21): check_catalogo.py (ultimo esito: OK, 11 righe/2 cartelle), quota_open.py (ultimo esito: 91.8% > 50%). Da rieseguire quando i permessi lo consentono.

Consegna condivisa: questo file in out/architetto/. Voce SCAMBIO: 46.