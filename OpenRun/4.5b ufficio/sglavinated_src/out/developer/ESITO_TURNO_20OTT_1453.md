# Esito turno developer — 20/10/2026 14:53 (blocco R1)

Comando eseguito: sh UFFICIO/Officina/TOOL/gate_t1.sh (comando singolo, radice sglavinated_src)
Exit code: 0
Risultato: GATE T1: TUTTO OK

- Build cmake --build target prova_gate: OK
- Percorso 6/6 GATE PERCORSO OK:
  lv_01_neon 63%/659/0.85-0.85, lv_02_atomico 65%/1440/0.72-0.98,
  lv_03_foglie 57%/590/0.75-0.95, lv_04_cromo 71%/1457/0.85-0.85,
  lv_05_spray 55%/607/0.86-0.84, lv_06_desktop 68%/813/0.85-0.85
- Giro guidato: finito su tutti e 6, fuori strada 0%
- ms/frame medio ~0.17, max 0.26 (limite 20); replay 2x IDENTICO su tutti i livelli
- accelerazione, ribaltamento, sterzo, curve continue, gara a tappe (4 combinazioni, replay IDENTICO), screenshot P1: OK
- Confronto Andrea Mac 19/10 (pre-fix, exit 13, giro guidato KO 5/6): ora post-fix rettilineo 55-71% (>= soglia minima Andrea >=35-40%, T1-Q5) e curva 0.72-0.98 (coerente 0.6-1.0); Andrea ri-verifica su Mac dopo peer review + CP-005 (D12 = A)

Blocco R1: compiti 1-3 conclusi (sviluppatore). Resta: compito 4 (Junior, dettaglio tratto+segmento iniziale nelle violazioni), compito 5 (Junior, rapporto test), review incrociata Builder/Junior, CP-005 del Capo a gate verde + entrambe le review registrate.

Log: UFFICIO/Officina/Test area/gate_t1.log
Scritture: bacheche/developer/, out/developer/.
