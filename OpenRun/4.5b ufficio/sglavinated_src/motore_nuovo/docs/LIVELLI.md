# LIVELLI — Percorsi e test di prova

Progetto dati per livelli del motore nuovo. Ogni livello vive in
`motore_nuovo/dati/livelli/<id>/livello.json` (dati leggibili da Python 3, nessun
codice C++). Ogni livello ha il suo tema, curato da @grafico dopo questo
blocco (regola 7: ogni livello ha il suo livello.json).

## Schema di livello.json

| Campo | Tipo | Descrizione |
|---|---|---|
| id | string | Identificativo del livello = nome cartella |
| versione | int | Versione dello schema dati |
| licenza | string | SPDX della licenza del file |
| autore | string | Chi ha firmato il disegno del livello |
| tema | string | Descrizione breve dell'ambientazione |
| stile | string | Uno stile da DIREZIONE_ARTISTICA.md, diverso per ogni livello |
| parte_di_albero | bool | Il livello appartiene all'albero di tappe/bivi |
| variante_albero | string | Variante dell'albero a cui appartiene |
| lunghezza | oggetto | segmenti (num), durata_stimata_s (stima a velocita' media) |
| seme | int | Seme per generare il tracciato da zero (mai copiato da Sega) |
| tempo_limite_s | int | Tempo di gara con checkpoint (FEDELTA tr. 2) |
| checkpoint_ogni_segmenti | int | Ogni quanti segmenti si rinnova il tempo |
| difficolta | string | "facile" per questi 3 livelli di prova |
| note_difficolta | string | Ritmo: respiro sui rettilinei, intensita' in curva |
| traffico | oggetto | densita, veicoli_per_km, tipi[] con id/percentuale/descrizione (veicoli originali) |
| show | oggetto | durata_frame, cooldown_frame, guardia_avvio_frame, intervallo_periodico_frame_stage0, soglia_inattivita_frame, ufo, trigger_map (trigger da dati/show_logica.md) |
| moduli_fx | oggetto | razzo_B (tasto B), sollevamento_auto_alta_velocita (on/off) |
| scenario_richiesto | oggetto | nome scenario per @scenografo, elementi, oggetti bordo strada, palette, note |

Trigger show (da dati/show_logica.md): crash -> Gesu' (priorita'), uscita di strada -> Gianni (cooldown 24 frame, non conta errore), periodico -> Gianni/Gesu' in base all'escalation, inattivita 10 s -> Gianni, UFO -> passaggio senza frase.

## Tabella riassuntiva (6 piste di prova)

| id | tema | stile | carattere del percorso | segmenti (giro a gas pieno) | seme | traffico | scenario |
|---|---|---|---|---|---|---|---|
| lv_01_neon | autostrada con luci a neon tramonto/nottura tra megastrutture | synthwave | rettilinei lunghi, curve larghe e dolci | 7650 (~53 s) | 4211 | bassa, 5/km | scenario_neon_01 |
| lv_02_atomico | strada atomica anni '50 | sci-fi anni '50 | dossi forti, chicane sinistra-destra-sinistra, un tornante | 7250 (~51 s) | 7731 | bassa, 6/km | scenario_atomico_02 |
| lv_03_foglie | viale delle foglie, parodia | psichedelico / cannabinoidi | tortuoso: percorso con tratti ad esse continue, tornanti, rettilinei corti | 5900 (~42 s) | 2098 | media, 7/km | scenario_foglie_03 |
| lv_04_cromo | tangenziale stradale sopraelevata tra megastrutture | futurista | sopraelevato: tragitto in salita, percorso sopraelevato, scenario urbano percepito dall'alto, discesa graduale | 7500 (~52 s) | 5309 | media, 7/km | scenario_cromo_04 (da fare) |
| lv_05_spray | periferia industriale con muri a spray | punk | urbano: grigio, curve piu strette, maggiore traffico, popolazione, rettilinei corti | 4950 (~35 s) | 8812 | alta, 9/km | scenario_spray_05 (da fare) |
| lv_06_desktop | prati ed animali ai lati, sfondo cielo azzurro e delicate nuvole bianche sparse | Windows 95 vibe | variazione con colline continue con curve medie | 5450 (~39 s) | 9595 | bassa, 5/km | scenario_desktop_06 (da fare) |

Il formato di `segmenti` (curva, collina, lunghezza in segmenti da 200 unita') e' in
`FORMATI_DATI.md`. Ogni pista passa il gate `prova_gate <out> <id> percorso`: rettilinei
fra 25% e 75% del giro, almeno un rettilineo di 300 segmenti, curve forti (>= 0.3) a
destra e a sinistra, e un giro intero guidato in automatico senza uscire di strada.
Per provarla a mano: `sglavinated_runner --livello <id>`.

**Gara a tappe.** Il runner senza opzioni avvia la gara: lv_01 in partenza, poi un bivio
(sinistra lv_02, destra lv_03), poi un secondo bivio verso l'arrivo (lv_04, lv_05 o lv_06).
L'albero e' in `dati/percorso_gara.json`; il bivio occupa gli ultimi 800 segmenti di ogni
tappa con rami, quindi quelle tappe devono finire con un rettilineo. Il gate
`prova_gate <out> lv_01_neon gara` percorre le 4 combinazioni di scelte. Il flusso completo
(intro, menu, finali, risultati) e' proposto in `FLUSSO_GIOCO.md`.

## Note di conformita

- Regola 16: i 3 livelli usano 3 stili diversi (synthwave/C-wave, sci-fi anni '50, psichedelico/cannabinoidi); tutte le varianti dell'albero di tappe vanno considerate, ciascuna con stile diverso (l'albero completo, default 5 tappe/bivi, e' definito nel blocco FED/D3; questi 3 sono i livelli di prova).
- Regola 16 / FEDELTA.md: >= 50% di somiglianza di struttura e sensazione con l'originale (mai asset): tratti 2 (tempo limite + checkpoint), 3 (bivi/albero di tappe), 4 (traffico da sorpassare), 5 (ambiente diverso per tappa), 8 (sensazione di velocita' a 50 Hz), 9 (schermata finale a tema, a carico di @grafico).
- Tracciati generati da seme, da zero, mai tracciati Sega. Nessun asset, musica o nome Sega.
- Determinismo: seme fisso, tempo = contatore di frame, nessun orologio di sistema.
- Ogni file: licenza dichiarata (CC-BY-4.0), autore indicato.
- Passaggio a @scenografo (BLOCCO L): 3 varianti di sfondo + oggetti bordo strada + palette per scenario_neon_01, scenario_atomico_02, scenario_foglie_03.


---

description: Level designer. Progetta i livelli/tappe (tracciato, ritmo, difficolta', traffico, dove appaiono Gianni/Gesu' UFO e gli extra) come dati, non come codice.
mode: subagent
model: opencode/longcat-2.5-preview-free
temperature: 0.5
permission:
  execute: deny
  edit:
    "*": deny
    "bacheche/livelli/**": allow
    "BACHECA_PUBBLICA.md": allow
    "out/livelli/**": allow
    "libreria/livelli/**": allow
    "motore_nuovo/dati/livelli/**": allow
    "motore_nuovo/docs/LIVELLI.md": allow
  webfetch: deny
---
Progetti i livelli in motore_nuovo/dati/livelli/<id>/livello.json (+ riga in motore_nuovo/docs/LIVELLI.md).
Ogni livello: tema (sci-fi lofi / foglie / ...), lunghezza, sequenza di segmenti (curve, colline) generata da seme, densita' traffico, difficolta' (piu' facili dell'originale, voluto da Andrea), punti degli show di Gianni/Gesu'/UFO, moduli fx abilitati (razzo, sollevamento), musica/radio, scenario richiesto (nome per @scenografo).
Percorsi NUOVI da zero, mai copiati dal gioco Sega. Ritmo: respiro sui rettilinei, intensita' in curva. Niente elementi che distraggono senza scopo.
Output sempre dati leggibili da Python 3 (JSON); nessun codice C++. Chiedi i grafici a @scenografo via SCAMBIO.md.
