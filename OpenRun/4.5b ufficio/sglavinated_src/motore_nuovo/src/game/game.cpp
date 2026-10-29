// SPDX-License-Identifier: MIT
#include "game/game.h"
#include "core/json_mini.h" // lettura livello.json (campo cartelloni_sponsor)
#include "render/sprite_raw.h" // caricaSpriteRaw: skyline raw del livello, core senza SDL
#include <algorithm> // std::min/max
#include <cctype> // std::isdigit
#include <cmath> // std::fmod
#include <cstdio> // std::fopen/fread/fclose
#include <map> // std::map per la libreria di tratti
#include <utility> // std::move

// ---------------------------------------------------------------------------
// Funzioni nello spazio globale usate da prova_gate.cpp (dichiarate in game.h
// prima di namespace sgl). Evitano ambiguità con le definizioni locali statiche
// di prova_gate.cpp (usando namespace sgl + definizione in sgl creerebbe
// simboli duplicati visibili).
// ---------------------------------------------------------------------------
sgl::i32 leggiFrequenzaDaConfig() {
    std::FILE* f = std::fopen("motore_nuovo/dati/config.json", "rb");
    if (!f) return 60;
    char buf[4096];
    std::string testo;
    size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0) testo.append(buf, n);
    std::fclose(f);
    sgl::JsonReader cfg(testo);
    if (cfg.parse() && sgl::jsonIsObject(cfg.valore())) {
        sgl::JsonValue v;
        if (sgl::jsonGet(sgl::jsonAsObject(cfg.valore()), "frequenza_hz", v) && sgl::jsonIsNumber(v)) {
            sgl::i32 hz = (sgl::i32)sgl::jsonAsNumber(v);
            if (hz > 0) return hz;
        }
    }
    return 60;
}

sgl::i32 impostaFrequenzaDaConfig(sgl::i32 hz) {
    sgl::i32 precedente = leggiFrequenzaDaConfig();
    // Cambia solo il valore di frequenza_hz: prima il file veniva riscritto
    // da zero e il gate frequenza cancellava le altre chiavi (es.
    // transizione_tema_s, persa a ogni gate_t1.sh).
    std::string testo;
    if (std::FILE* in = std::fopen("motore_nuovo/dati/config.json", "rb")) {
        char buf[4096];
        size_t n;
        while ((n = std::fread(buf, 1, sizeof(buf), in)) > 0) testo.append(buf, n);
        std::fclose(in);
    }
    const std::string valore = std::to_string(hz);
    const size_t chiave = testo.find("\"frequenza_hz\"");
    const size_t duepunti = chiave == std::string::npos ? chiave : testo.find(':', chiave);
    if (duepunti != std::string::npos) {
        size_t inizio = duepunti + 1;
        while (inizio < testo.size() && (testo[inizio] == ' ' || testo[inizio] == '\t')) ++inizio;
        size_t fine = inizio;
        while (fine < testo.size() && (std::isdigit((unsigned char)testo[fine]) || testo[fine] == '.')) ++fine;
        testo.replace(inizio, fine - inizio, valore);
    } else {
        testo = "{\n  \"frequenza_hz\": " + valore + "\n}\n";
    }
    std::FILE* f = std::fopen("motore_nuovo/dati/config.json", "wb");
    if (f) {
        std::fwrite(testo.data(), 1, testo.size(), f);
        std::fclose(f);
    }
    return precedente;
}

namespace sgl {
namespace {
constexpr f32 ROAD_SCROLL_SCALE = 72.0f;
constexpr f32 ROAD_SCROLL_SPEED_BOOST = 0.65f;
// Forza centrifuga in curva (dimensionless, si applica a speed*dt = unità/frame).
constexpr f32 CURVA_SPINTA = 0.015f;
// Quando l'auto e' fuori strada la spinta di curva si moltiplica per questo
// fattore (<1): lo sterzo verso la strada deve poter far rientrare l'auto
// senza essere contrastato dalla spinta centrifuga.
constexpr f32 CURVA_SPINTA_OFFROAD_FACTOR = 0.25f;
// Avversari: velocita' di punta in frazione di quella del giocatore (unita' di
// strada al secondo). Prima il passo era in unita' per frame tarato a 50 Hz:
// a 60 Hz superavano il giocatore. Punta 0.94..1.04: i piu' veloci restano
// davanti a chi guida perfetto. Elastico continuo sul distacco (Andrea 23/10:
// "tutti scappano"): chi e' davanti rallenta, chi e' dietro recupera, cosi' il
// gruppo resta a pochi secondi dal giocatore e si riprende dopo un incidente.
constexpr f32 RIVALE_PUNTA_MIN = 0.94f;
constexpr f32 RIVALE_PUNTA_PASSO = 0.017f; // + 0..6 passi: 0.94..1.04
constexpr f32 RIVALE_ACCEL_TAU_S = 2.0f;   // ~63% della punta in 2 s
constexpr f32 RIVALE_ELASTICO_DISTANZA = 40000.0f; // a questo distacco l'elastico vale -/+25%
constexpr f32 RIVALE_ELASTICO_MIN = 0.80f;
constexpr f32 RIVALE_ELASTICO_MAX = 1.15f;
constexpr f32 RIVALE_LATERALE_PER_S = 400.0f;
constexpr f32 RIVALE_CURVA_SCALE = 0.3f;     // rallentamento in curva: scala |curva| del segmento
constexpr f32 RIVALE_CURVA_SLOW_MAX = 0.25f; // max rallentamento in curva estrema (25%; 29/10: prima 50%, restavano a un minuto)
constexpr f32 RIVALE_DISTANZA_MIN = 1500.0f; // distanza minima di seguito tra due rivali
constexpr f32 RIVALE_LATERALE_MIN = 250.0f;  // soglia laterale per intervenire sull'altro rivale
constexpr f32 RIVALE_SPOSTAMENTO = 200.0f;   // spostamento laterale per sorpassare di lato
// Rivali e traffico grandi come l'auto del giocatore (Andrea 27/10: "piccolini"):
// 40 px x SPRITE_WORLD_PX x 1.4 = 280 unita', l'auto del giocatore ne copre ~270.
constexpr f32 SCALA_AUTO = 1.4f;
// Rivali (Andrea 29/10, "piu' evidenti"): un poco piu' grandi del traffico,
// con freccia colorata e nome del pilota sopra (renderer drawEtichetta).
constexpr f32 SCALA_RIVALE = 1.6f;
const char* const NOMI_RIVALI[] = {"VIPER", "NOVA", "BLITZ", "RAZOR", "LUNA", "TURBO", "ZETA"};
constexpr u32 COLORI_RIVALI[] = {0xFFFF4040u, 0xFF40D0FFu, 0xFFFFE040u, 0xFF50F070u,
                                             0xFFFF70D0u, 0xFFFF9030u, 0xFFE0E0FFu};
// Arrivo (Andrea 29/10): pubblico dietro le transenne ai lati della strada,
// da FOLLA_PRIMA unita' prima del traguardo a FOLLA_DOPO dopo (l'auto si
// ferma li' in mezzo), un gruppo ogni FOLLA_PASSO per lato. Le persone sono
// alte come l'auto (40x26 px x SPRITE_WORLD_PX x 3.3).
constexpr f32 FOLLA_SCALA = 3.3f;
constexpr f32 FOLLA_PASSO = 600.0f;
constexpr f32 FOLLA_PRIMA = 9000.0f, FOLLA_DOPO = 26000.0f;
constexpr f32 FRENATA_ARRIVO_S = 1.6f; // da tutta velocita' a fermo dopo il traguardo
// Urti (Andrea 29/10): le auto sono solide (finestra di contatto in game.h).
// Il controllo e' "a spazzata": a 290 km/h il giocatore fa ~500 unita' per
// frame e senza spazzata salterebbe oltre l'auto davanti.
constexpr f32 URTO_Z_DAVANTI = Game::URTO_Z_MAX - 100.0f; // il giocatore si ferma qui dietro a chi ha davanti
constexpr f32 URTO_LARGHEZZA = Game::URTO_LARGHEZZA;
constexpr f32 URTO_SPINTA_TRAFFICO = 900.0f;  // unita'/s: il traffico respinge forte
constexpr f32 URTO_SPINTA_RIVALE = 550.0f;    // sportellata tra piloti
constexpr f32 URTO_TRAFFICO_DOPO = 0.70f;     // tamponamento: velocita' = 70% di quella del veicolo
constexpr f32 URTO_RIVALE_DOPO = 0.95f;       // tamponamento di un rivale: quasi la sua velocita'
constexpr f32 URTO_EFFETTO_TRAFFICO_S = 0.55f, URTO_EFFETTO_RIVALE_S = 0.35f;
// Ciglio (Andrea 29/10): sotto questa frazione della punta uscire di strada
// non e' una botta; la scritta CIGLIO! resta per CIGLIO_AVVISO_S secondi.
constexpr f32 CIGLIO_V_MIN = 0.2f;
constexpr f32 CIGLIO_AVVISO_S = 1.5f;
// Botte da cui l'auto fuma dal cofano.
constexpr i32 DANNI_FUMO = 5;
// Rivali (Andrea 29/10): nessuna fuga impossibile e nessuno "appiccicato".
// Entro +-RIVALE_LIBERO_S secondi di distacco ognuno va del suo ritmo; oltre,
// chi e' davanti cala fino a -15% e chi e' dietro spinge fino a +25%.
constexpr f32 RIVALE_LIBERO_S = 3.0f;
constexpr f32 RIVALE_ELASTICO_RAMPA_S = 5.0f;
constexpr f32 RIVALE_RITMO = 0.035f;          // +-3.5% di ritmo che cambia piano nel tempo
// Tempo medio sul giro rispetto alla punta: serve a stimare il loro arrivo.
constexpr f32 RIVALE_MEDIA_PUNTA = 0.88f;
// Rivale che raggiunge un veicolo del traffico: cambia corsia per tempo e,
// se e' gia' attaccato, si accoda.
constexpr f32 RIVALE_SCHIVA_DIST = 12000.0f;   // traffico entro questa distanza davanti
constexpr f32 RIVALE_SCHIVA_LATERALE = 330.0f; // sovrapposizione laterale (unita' mondo)
constexpr f32 RIVALE_SCHIVA_MARGINE = 480.0f;  // passa a questa distanza laterale dal veicolo
constexpr f32 RIVALE_SCHIVA_PER_S = 1500.0f;   // cambio corsia rapido per schivare
constexpr f32 RIVALE_X_MAX = 650.0f;           // laterale massimo di un rivale (unita' di corsia)
// Traffico comune (Andrea 27/10): pochi veicoli lenti, uno ogni 3-4 s a tutta
// velocita'. Velocita' 35-55% della punta del giocatore.
constexpr f32 TRAFFICO_VEL_MIN = 0.35f;
constexpr f32 TRAFFICO_VEL_PASSO = 0.05f;        // + 0..4 passi
constexpr f32 TRAFFICO_PRIMO = 30000.0f;         // primo veicolo, davanti alla griglia
constexpr f32 TRAFFICO_DISTANZA_MIN = 45000.0f;  // fra un veicolo e il successivo
constexpr u32 TRAFFICO_DISTANZA_VAR = 25000u;    // + 0..25000
constexpr f32 TRAFFICO_DIETRO = 3000.0f;         // superato di tanto: torna davanti
constexpr f32 TRAFFICO_MARGINE_BIVIO = 2000.0f;  // nessun veicolo nuovo nella zona del bivio
constexpr f32 TRAFFICO_CODA = 3000.0f;           // stessa corsia: si accoda a chi e' davanti
constexpr f32 TRAFFICO_LATERALE_PER_S = 900.0f;
constexpr f32 TRAFFICO_CORSIA = 1200.0f;         // larghezza corsia = mezza strada normale
// Sollevamento: parte sopra il 98% della velocita' massima con curva quasi
// nulla, sale in ~0.8 s fino a SOLLEVAMENTO_MAX_PX, ondeggia piano e ricade
// in ~0.3 s appena si frena, si curva o si esce di strada.
constexpr f32 SOLLEVAMENTO_SOGLIA_V = 0.98f; // ~287 km/h, ~8 s di gas pieno in rettilineo
constexpr f32 SOLLEVAMENTO_CURVA_MAX = 0.15f;
constexpr f32 SOLLEVAMENTO_MAX_PX = 7.0f;
constexpr f32 SOLLEVAMENTO_SALITA_S = 0.8f;
constexpr f32 SOLLEVAMENTO_DISCESA_S = 0.3f;
// Passaggio assistito al bivio (feedback Andrea 23/10): niente spinta delle
// curve, niente crash, l'auto e' accompagnata verso il centro del ramo piu'
// vicino (lo sterzo vince sempre, cosi' si sceglie ancora il ramo). Entra in
// mezzo secondo e a inizio tappa nuova si spegne in 2.5 s.
constexpr f32 ASSIST_ENTRATA_S = 0.5f;
constexpr f32 ASSIST_USCITA_S = 2.5f;
constexpr f32 ASSIST_RICHIAMO_TAU_S = 0.6f;
constexpr u32 CARTELLONI_OGNI_SEG = 10;     // in media un cartellone ogni 10 segmenti (~10%)
// Oggetti decorativi (Andrea 27/10 sera): mai sul tracciato, a bordo strada o
// sparsi nel paesaggio. Distanze dal bordo della carreggiata, unita' mondo.
constexpr f32 OGGETTI_PASSO_Z = 1600.0f;     // una fila ogni 1600 unita' di strada
constexpr f32 OGGETTI_BORDO_MIN = 250.0f;    // fascia bordo strada: da 250 ...
constexpr f32 OGGETTI_BORDO_MAX = 900.0f;    // ... a 900 dal bordo
constexpr f32 OGGETTI_PAESAGGIO_MAX = 5000.0f; // fascia paesaggio: fino a 5000 dal bordo
constexpr f32 OGGETTO_ALTEZZA_MONDO = 1280.0f; // oggetti alti: altezza a schermo come ~2 auto
constexpr f32 BIVIO_ISTERESI = 300.0f;     // unita' mondo: cambio di ramo seguito solo se piu' vicini all'altro di tanto
constexpr f32 ASSIST_RICHIAMO_MAX = 250.0f; // unita'/s, sotto lo sterzo (>= 510 in alto)
constexpr f32 VELOCITA_MAX_KMH = 293.0f; // velocita' massima mostrata (come l'originale)
constexpr f32 TEMPO_EXTRA_AVVISO_S = 2.5f; // "TEMPO EXTRA" a schermo dopo un checkpoint
// Salto sui dossi (Andrea 29/10): se la strada scende piu' in fretta di quanto
// la gravita' tiri giu' l'auto, l'auto si stacca e vola per inerzia.
constexpr f32 SALTO_GRAVITA = 7000.0f;     // unita' mondo / s^2 (piu' bassa = salti piu' frequenti)
constexpr f32 SALTO_SOGLIA_V = 0.6f;       // sotto il 60% della velocita' massima resta incollata
constexpr f32 SALTO_PX_PER_UNITA = 0.04f;  // quota sopra la strada -> pixel a schermo
constexpr f32 SALTO_MAX_PX = 28.0f;
constexpr f32 SALTO_V_DISCONTINUITA = 60000.0f; // oltre: salto di quota del tracciato, non un dosso

// Legge un file intero (vuoto se manca): solo in init, mai nel ciclo di gioco.
std::string leggiFileTesto(const std::string& path) {
    std::string testo;
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return testo;
    char buf[4096];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0) testo.append(buf, n);
    std::fclose(f);
    return testo;
}

// Espande il campo "tracciato" di livello.json in descrittori di strada.
// Ogni voce e' una stringa (nome tratto) oppure un oggetto
// {"tratto": "nome", "lunghezza": 500, ...override...}; le definizioni base
// sono in motore_nuovo/dati/tracciati/tratti.json.
// Restituisce true se almeno un tratto e' espanso. Se manca il file, la
// libreria e' illeggibile o una voce e' sconosciuta, quella voce e' saltata;
// quando out resta vuoto il chiamante usa il campo "segmenti" (compatibile
// con i tracciati gia' esistenti).
// Campi di un tratto (libreria tratti.json o override nel livello): solo quelli
// presenti sovrascrivono d. "larghezza": 1 = 2 corsie, 2 = 4 corsie.
// "dune" + "onde": dossi sopra la quota del tratto (v. Road::SegmentDesc).
void leggiCampiTratto(const JsonObject& o, Road::SegmentDesc& d) {
    JsonValue cv, hv, lv, ev, uv, wv, dv, ov, av;
    if (jsonGet(o, "curva", cv) && jsonIsNumber(cv)) d.curva = (f32)jsonAsNumber(cv);
    if (jsonGet(o, "collina", hv) && jsonIsNumber(hv)) d.collina = (f32)jsonAsNumber(hv);
    if (jsonGet(o, "lunghezza", lv) && jsonIsNumber(lv)) {
        i32 l = (i32)jsonAsNumber(lv);
        if (l > 0) d.lunghezza = l;
    }
    if (jsonGet(o, "entrata", ev) && jsonIsNumber(ev)) {
        i32 e = (i32)jsonAsNumber(ev);
        if (e >= 0) d.entrata = e;
    }
    if (jsonGet(o, "uscita", uv) && jsonIsNumber(uv)) {
        i32 u = (i32)jsonAsNumber(uv);
        if (u >= 0) d.uscita = u;
    }
    if (jsonGet(o, "larghezza", wv) && jsonIsNumber(wv)) {
        d.larghezza = std::clamp((f32)jsonAsNumber(wv), 1.0f, 2.0f);
    }
    if (jsonGet(o, "dune", dv) && jsonIsNumber(dv)) d.dune = std::clamp((f32)jsonAsNumber(dv), -0.2f, 0.2f);
    if (jsonGet(o, "onde", ov) && jsonIsNumber(ov)) d.onde = std::max(0, (i32)jsonAsNumber(ov));
    if (jsonGet(o, "acqua", av) && jsonIsNumber(av)) d.acqua = (f32)jsonAsNumber(av);
}

bool leggiTracciato(const JsonValue& campoTracciato, std::vector<Road::SegmentDesc>& out) {
    if (!jsonIsArray(campoTracciato)) return false;
    const JsonArray& arr = jsonAsArray(campoTracciato);
    if (arr.empty()) return false;

    JsonReader tratti(leggiFileTesto("motore_nuovo/dati/tracciati/tratti.json"));
    if (!tratti.parse()) return false;
    const JsonValue* radice = &tratti.valore();
    if (!radice || !jsonIsObject(*radice)) return false;
    JsonValue campoLib;
    if (!jsonGet(jsonAsObject(*radice), "tratti", campoLib) || !jsonIsObject(campoLib)) return false;
    const JsonObject& lib = jsonAsObject(campoLib);

    std::map<std::string, Road::SegmentDesc> libreria;
    for (const auto& kv : lib) {
        if (!jsonIsObject(kv.second)) continue;
        const JsonObject& o = jsonAsObject(kv.second);
        Road::SegmentDesc d;
        leggiCampiTratto(o, d);
        libreria.emplace(kv.first, d);
    }

    out.clear();
    for (const JsonValue& v : arr) {
        Road::SegmentDesc d;
        const JsonObject* o = nullptr;
        std::string nome;
        if (jsonIsString(v)) {
            nome = jsonAsString(v);
        } else if (jsonIsObject(v)) {
            o = &jsonAsObject(v);
            JsonValue idv;
            if (!jsonGet(*o, "tratto", idv) || !jsonIsString(idv)) continue;
            nome = jsonAsString(idv);
        } else {
            continue;
        }
        auto it = libreria.find(nome);
        if (it == libreria.end()) continue;
        d = it->second;
        if (o) leggiCampiTratto(*o, d); // override del livello
        if (d.lunghezza > 0) out.push_back(d);
    }
    return !out.empty();
}
}

std::string Game::percorsoDati(const std::string& relativo) const {
    return radiceDati_.empty() ? relativo : radiceDati_ + "/" + relativo;
}

Game::Game() : Game(12345u) {}

Game::Game(u32 seed) : seed_(seed), rng_(seed_), fb_(), pal_(), ren_(fb_, pal_), showManager_(seed_) {
    audioBuf_.resize(AUDIO_FRAME_SAMPLES * 2, 0);
    sprites_.reserve(6); // niente allocazioni per traffico dopo il primo frame
    scenaSprites_.reserve(16); // cresce a init() con i cartelloni
}

void Game::caricaLivello(const std::string& livelloId) {
    // Tracciato da dati: legge il campo "segmenti" (array di {curva, collina,
    // lunghezza}) da motore_nuovo/dati/livelli/<livelloId>/livello.json.
    // Se il campo manca o il file e' illeggibile, usa il fallback deterministico
    // hardcoded sotto. Stesso seme = stesso tracciato (determinismo).
    // livello.json letto e analizzato una volta sola: ne usano i dati
    // sia i segmenti sia i cartelloni (prima il file veniva letto due volte).
    JsonReader livello(leggiFileTesto(percorsoDati("motore_nuovo/dati/livelli/" + livelloId + "/livello.json")));
    const JsonValue* radiceLivello = livello.parse() ? &livello.valore() : nullptr;
    leggiTemaDaFile(radiceLivello); // colori di ambiente e stile dei cartelloni

    std::vector<Road::SegmentDesc> descs;
    bool daFile = false;
    if (radiceLivello && jsonIsObject(*radiceLivello)) {
        const JsonObject& rootLivello = jsonAsObject(*radiceLivello);
        JsonValue campoTracciato;
        if (jsonGet(rootLivello, "tracciato", campoTracciato) && jsonIsArray(campoTracciato)) {
            daFile = leggiTracciato(campoTracciato, descs) && !descs.empty();
        }
    }
    if (!daFile) {
        daFile = leggiSegmentiDaFile(radiceLivello, descs) && !descs.empty();
    }
    if (daFile) {
        // Ripeti il pattern finche' il giro ha almeno 2*DRAW_DIST segmenti
        // ("lunghezza" conta segmenti): il tracciato e' un circuito; con troppo
        // pochi segmenti la proiezione con wrap mostrerebbe segmenti troppo lontani.
        std::vector<Road::SegmentDesc> pattern = std::move(descs);
        descs.clear();
        i32 segTotali = 0;
        const i32 minSeg = Road::DRAW_DIST * 2;
        while (segTotali < minSeg) {
            for (const Road::SegmentDesc& d : pattern) {
                descs.push_back(d);
                segTotali += d.lunghezza > 0 ? d.lunghezza : 1;
            }
        }
        road_.build(descs, seed_);
    } else {
        // Fallback deterministico (hardcoded): 400 segmenti con pattern inventato.
        std::vector<f32> curves, hills;
        for (i32 i = 0; i < 400; ++i) {
            f32 c = 0.0f, h = 0.0f;
            if (i % 60 < 20) c = 0.6f;        // curva a destra
            if (i % 60 < 20 && i % 120 >= 60) c = -0.6f; // curva a sinistra
            if (i % 90 < 30) h = 0.15f;       // salita
            if (i % 90 < 30 && i % 180 >= 90) h = -0.15f; // discesa
            curves.push_back(c);
            hills.push_back(h);
        }
        road_.build(curves, hills, seed_);
    }

    // Cartelloni sponsor bordo strada.
    // Fonte dati: campo "cartelloni_sponsor" di
    // motore_nuovo/dati/livelli/<livelloId>/livello.json (scritto da @livelli,
    // voce 48): per ogni voce si usano segmento, lato e sponsor_id.
    // Se il campo manca (o il file e' illeggibile), si usa il posizionamento
    // casuale deterministico storico (Rng con seed_, seme di partita): scelta a
    // caso solo se il dato manca nel file. Campi mancanti dentro una voce
    // presente (lato, sponsor_id sconosciuto) si scelgono anch'essi con Rng(seed_).
    // Tutte le allocazioni avvengono qui (init): zero alloc nel ciclo di gioco.
    cartelloni_.clear();
    if (!sponsorSpriteIdx_.empty()) {
        const f32 latoSx = -1600.0f; // bordo strada sinistro (unita' mondo)
        const f32 latoDx = 1600.0f;  // bordo strada destro
        const i32 nSeg = (i32)road_.segments().size();
        std::vector<bool> segmentoOccupato(nSeg, false);
        Rng rndCartelloni(seed_);

        // 1) Posizioni esplicite da livello.json (campo cartelloni_sponsor).
        //    leggiCartelloniDaFile segna i segmenti usati in segmentoOccupato.
        const bool daFile = leggiCartelloniDaFile(radiceLivello, segmentoOccupato);

        // 2) Densita' totale cartelloni ~8-12% dei segmenti (Andrea 27/10 sera:
        //    erano troppi al 25%, ora circa un terzo: 1 segmento su 10). I segmenti senza
        //    cartellone esplicito vengono riempiti in modo deterministico
        //    (seed di tappa), senza sovrapporre le posizioni esplicite del file.
        //    Tutte le allocazioni avvengono qui (init): zero alloc per frame.
        i32 k = (i32)cartelloni_.size();
        for (i32 i = 0; i < nSeg; ++i) {
            if (segmentoOccupato[(size_t)i]) continue;
            if (rndCartelloni.next(CARTELLONI_OGNI_SEG) != 0) continue; // ~1 segmento su 10 con cartellone
            SpriteInstance b;
            b.worldZ = (f32)(i * Road::SEG_LEN);
            b.worldX = (k & 1) ? latoDx : latoSx; // lato alternato: distribuzione bilanciata
            b.scale = 1.0f;
            b.sprite = ren_.sponsorSprite(sponsorSpriteIdx_[rndCartelloni.next((u32)sponsorSpriteIdx_.size())]);
            b.paletteBase = 0;
            cartelloni_.push_back(b);
            segmentoOccupato[(size_t)i] = true;
            ++k;
        }
        const double densita = nSeg > 0 ? (double)cartelloni_.size() / (double)nSeg : 0.0;
        std::printf("cartelloni sulla pista: %zu (da file %s, densita ~%.1f%% del percorso)\n",
                    cartelloni_.size(), daFile ? "sì" : "no", densita * 100.0);
    }
    scenaSprites_.reserve(sprites_.capacity() + cartelloni_.size()); // zero alloc per frame a regime

    // Skyline della tappa (DECO1 2b): raw del livello da
    // motore_nuovo/dati/sprite_raw/scenari/<livello>/skyline.sglraw (id del
    // catalogo tema_grafico.skyline, letto in leggiTemaDaFile). Caricato in
    // caricaLivello, quindi si rinnova a ogni cambio tappa (init e fineTappa):
    // cielo piatto se il raw manca o e' illeggibile. Zero alloc nel ciclo di gioco.
    Sprite skyline = caricaSpriteRaw(percorsoDati("motore_nuovo/dati/sprite_raw/scenari/" + livelloId + "/skyline.sglraw"));
    if (skyline.width > 0) ren_.impostaSkyline(std::move(skyline));
    else ren_.impostaSkyline(Sprite{});

    // Oggetti bordo strada (DECO1 2c): sprite da tema_grafico.oggetti_bordo_strada.
    caricaOggettiStrada(livelloId);
    // Strada larga (piu' corsie): cartelloni e oggetti si spostano con il
    // bordo, cosi' restano fuori dalla carreggiata. Il cartello del bivio
    // (worldX 0) non si sposta; al bivio la larghezza e' sempre 1.
    for (std::vector<SpriteInstance>* fissi : {&cartelloni_, &oggettiStrada_}) {
        for (SpriteInstance& f : *fissi) {
            const f32 extra = (road_.larghezzaAt(f.worldZ) - 1.0f) * Road::ROAD_HALF_WORLD;
            if (f.worldX < 0.0f) f.worldX -= extra;
            else if (f.worldX > 0.0f) f.worldX += extra;
        }
    }
    ordinaSpriteFissi();
}

namespace {
std::string nomeOggettoDaId(const std::string& id);
}

void Game::caricaOggettiStrada(const std::string& livelloId) {
    // DECO1 2c: carica gli sprite raw degli oggetti bordo strada dichiarati in
    // tema_grafico.oggetti_bordo_strada e ne fissa le posizioni a bordo strada
    // (seme fisso, zero alloc nel ciclo di gioco).
    // Sono puramente decorativi: NON generano collisione (oggetti non solidi).
    oggettiStrada_.clear();
    ren_.clearOggettoSprites();
    if (oggettiBordoStrada_.empty()) return;
    const i32 nOggetti = (i32)oggettiBordoStrada_.size();
    const f32 giro = (f32)road_.totalLength();
    if (giro <= 0.0f) return;
    // Indice sprite per ogni id: registra prima tutti gli sprite, cosi' i
    // puntatori restituiti da oggettoSprite() restano validi (nessuna rialloc
    // dopo la creazione delle istanze).
    std::vector<i32> idxPerId(nOggetti, -1);
    for (i32 j = 0; j < nOggetti; ++j) {
        std::string nome = nomeOggettoDaId(oggettiBordoStrada_[j]);
        Sprite s = caricaSpriteRaw(percorsoDati("motore_nuovo/dati/sprite_raw/scenari/" + livelloId + "/" + nome + ".sglraw"));
        if (s.width > 0) idxPerId[j] = ren_.aggiungiOggettoSprite(std::move(s));
    }
    // Posizioni (Andrea 27/10 sera): mai sul tracciato. Per ogni fila, un
    // oggetto a bordo strada (lato alternato) e uno nel paesaggio dal lato
    // opposto, a distanza variabile dal seme (deterministico). La distanza si
    // misura dal bordo della carreggiata al bordo dello sprite, quindi anche gli
    // oggetti larghi restano fuori strada. Nella zona del bivio la carreggiata e'
    // larga quanto i due rami: lo stesso margine parte dal bordo esterno dei rami.
    const bool conBivio = tappaConBivio(livelloId);
    const f32 zBivioInizio = conBivio ? road_.bivioInizioZ() : giro;
    const f32 bordo = Road::ROAD_WIDTH * 0.5f;
    const f32 bordoBivio = Road::BIVIO_DIST + bordo;
    Rng rnd(seed_ ^ 0x4F474745u);
    i32 k = 0;
    for (f32 z = OGGETTI_PASSO_Z; z < giro - OGGETTI_PASSO_Z; z += OGGETTI_PASSO_Z, ++k) {
        const f32 bordoQui = z >= zBivioInizio ? bordoBivio : bordo;
        for (i32 fascia = 0; fascia < 2; ++fascia) {
            const i32 j = (i32)rnd.next((u32)nOggetti);
            const Sprite* sprite = idxPerId[j] >= 0 ? ren_.oggettoSprite(idxPerId[j]) : nullptr;
            if (!sprite) continue;
            // Scala: gli oggetti alti (ridisegnati 32x96 e oltre) arrivano a
            // OGGETTO_ALTEZZA_MONDO; quelli piccoli restano a 4x come prima.
            const f32 scala = std::clamp(OGGETTO_ALTEZZA_MONDO / ((f32)sprite->height * Renderer::SPRITE_WORLD_PX), 1.0f, 4.0f);
            const f32 metaLarghezza = 0.5f * (f32)sprite->width * Renderer::SPRITE_WORLD_PX * scala;
            const f32 da = fascia == 0 ? OGGETTI_BORDO_MIN : OGGETTI_BORDO_MAX;
            const f32 a = fascia == 0 ? OGGETTI_BORDO_MAX : OGGETTI_PAESAGGIO_MAX;
            const f32 distanza = da + (a - da) * (f32)rnd.next(1000) / 1000.0f;
            const f32 lato = ((k + fascia) & 1) ? 1.0f : -1.0f;
            SpriteInstance o;
            o.worldZ = z + (f32)rnd.next((u32)(OGGETTI_PASSO_Z * 0.5f)); // file non allineate
            o.worldX = lato * (bordoQui + distanza + metaLarghezza);
            o.scale = scala;
            o.sprite = sprite;
            o.paletteBase = 0;
            oggettiStrada_.push_back(o);
        }
    }
}

void Game::caricaMusica() {
    // dati/audio/playlist.json: "menu": {"file": "musica/x.xm", "titolo": "..."}
    // e "gara": [{...}, ...]. Senza file o brani si gioca con il solo motore (nessun crash).
    if (musicaCaricata_) return;
    musicaCaricata_ = true;
    JsonReader r(leggiFileTesto(percorsoDati("motore_nuovo/dati/audio/playlist.json")));
    if (!r.parse() || !jsonIsObject(r.valore())) return;
    auto aggiungi = [this](const JsonValue& b) {
        JsonValue fv, tv;
        if (!jsonIsObject(b) || !jsonGet(jsonAsObject(b), "file", fv) || !jsonIsString(fv)) return false;
        const std::string titolo = jsonGet(jsonAsObject(b), "titolo", tv) && jsonIsString(tv) ? jsonAsString(tv) : "";
        if (mixer_.aggiungiBrano(percorsoDati("motore_nuovo/dati/audio/" + jsonAsString(fv)), titolo)) return true;
        std::printf("radio: brano non leggibile %s\n", jsonAsString(fv).c_str());
        return false;
    };
    JsonValue menu, gara;
    if (jsonGet(jsonAsObject(r.valore()), "menu", menu) && aggiungi(menu)) branoMenu_ = 0;
    primoBranoGara_ = mixer_.numTracks();
    if (jsonGet(jsonAsObject(r.valore()), "gara", gara) && jsonIsArray(gara))
        for (const JsonValue& b : jsonAsArray(gara)) aggiungi(b);
}

void Game::aggiornaRadio() {
    // Menu: il suo brano. Dalla partenza ai risultati: i brani della gara,
    // uno per tappa a giro; ogni nuova gara parte dal brano successivo.
    const FaseId f = fasi_.attuale();
    const bool inGara = f != FaseId::Splash && f != FaseId::Menu;
    if (inGara && !radioInGara_) ++radioGara_;
    radioInGara_ = inGara;
    const i32 braniGara = mixer_.numTracks() - primoBranoGara_;
    i32 voluto = branoMenu_;
    if (inGara && braniGara > 0)
        voluto = primoBranoGara_ + (radioGara_ + std::max(0, numeroTappa_ - 1)) % braniGara;
    if (voluto >= 0 && voluto != mixer_.currentTrack()) mixer_.selezionaBrano(voluto);
}

void Game::init(const std::string& livelloId) {
    // Legge la frequenza logica da dati/config.json (default 60 Hz).
    // Tutto il core usa dt in secondi: stesso comportamento a 50 e 60 Hz.
    frequenzaHz_ = 60;
    JsonReader cfg(leggiFileTesto("motore_nuovo/dati/config.json"));
    if (cfg.parse() && jsonIsObject(cfg.valore())) {
        JsonValue v;
        if (jsonGet(jsonAsObject(cfg.valore()), "frequenza_hz", v) && jsonIsNumber(v)) {
            i32 hz = (i32)jsonAsNumber(v);
            if (hz > 0) frequenzaHz_ = hz;
        }
        JsonValue vt;
        if (jsonGet(jsonAsObject(cfg.valore()), "transizione_tema_s", vt) && jsonIsNumber(vt)) {
            f32 val = (f32)jsonAsNumber(vt);
            if (val >= 0.0f) transizioneTemaS_ = val;
        }
        JsonValue vb;
        if (jsonGet(jsonAsObject(cfg.valore()), "bump_rallenta", vb) && jsonIsNumber(vb)) {
            f32 val = (f32)jsonAsNumber(vb);
            if (val > 0.0f && val <= 1.0f) bumpRallentaFattore_ = val;
        }
        JsonValue vd;
        if (jsonGet(jsonAsObject(cfg.valore()), "bump_durata_s", vd) && jsonIsNumber(vd)) {
            f32 val = (f32)jsonAsNumber(vd);
            if (val >= 0.0f) bumpDurataS_ = val;
        }
        JsonValue vs;
        if (jsonGet(jsonAsObject(cfg.valore()), "bump_spinta_laterale", vs) && jsonIsNumber(vs)) {
            f32 val = (f32)jsonAsNumber(vs);
            if (val >= 0.0f) bumpSpintaLaterale_ = val;
        }
        // Ciglio (Andrea 29/10): quanto rallenta il primo tocco, per quanti
        // secondi lo si ricorda, da che velocita' il tocco seguente e' un incidente.
        auto leggi = [&cfg](const char* chiave, f32& dove, f32 minimo, f32 massimo) {
            JsonValue vc;
            if (jsonGet(jsonAsObject(cfg.valore()), chiave, vc) && jsonIsNumber(vc)) {
                const f32 val = (f32)jsonAsNumber(vc);
                if (val >= minimo && val <= massimo) dove = val;
            }
        };
        leggi("ciglio_rallenta", ciglioRallenta_, 0.1f, 1.0f);
        leggi("ciglio_memoria_s", ciglioMemoriaDurataS_, 0.0f, 60.0f);
        leggi("ciglio_crash_velocita", ciglioCrashVelocita_, 0.0f, 2.0f); // > 1 = mai incidente
    }

    gara_ = false;
    road_.impostaBivio(false);
    latoBivio_ = 0;
    tempoScaduto_ = false;
    km_ = 0.0f;
    leggiHud();
    caricaLivello(livelloId);

    road_.playerZ = 0.0f;
    road_.playerX = 0.0f;
    player_.z = 0.0f;
    player_.x = 0.0f;
    player_.speed = 0.0f;
    tempoGiocoS_ = 0; // secondo contatore di gioco riparte con la partita
    playerFlipping_ = false;
    sollevamento_ = 0.0f;
    sollevamentoFaseS_ = 0.0f;
    saltoAltezza_ = 0.0f;
    saltoVY_ = 0.0f;
    saltoQuotaPrec_ = -1e9f;
    saltoVStradaPrec_ = 0.0f;
    tempoExtraS_ = 0.0f;
    urtoEffettoS_ = 0.0f;
    arrivoS_ = 0.0f;
    fermoS_ = -1.0f;
    folla_.clear();
    ren_.setLineaArrivo(false);
    assistenzaBivio_ = 0.0f;
    ramoProvvisorio_ = 0;
    centroRamoPrec_ = 0.0f;
    playerFlipFrame_ = 0;
    offroadFrames_ = 0;
    playerBump_ = false;
    playerBumpTimer_ = 0.0f;
    playerBumpSpintaX_ = 0.0f;
    urtiGara_ = 0;
    toccateCiglio_ = 0;
    dallUrtoS_ = -1.0f;
    latoUrto_ = 0;
    offroadPrec_ = false;
    ciglioMemoriaS_ = 0.0f;
    avvisoCiglioS_ = 0.0f;
    showManager_.reset(seed_); // stesso seed = stessi show (determinismo)

    // Testi fumetti da frasi.json (dati, CC-BY-4.0 Andrea): carica una volta,
    // verifica l'invariante personaggio<->frase, collega a DisegnoShow per il
    // layout del fumetto. Percorso relativo come livello.json (regola: niente
    // path assoluti). Tutte le allocazioni stanno qui (init).
    testiFrasi_.carica(percorsoDati("motore_nuovo/dati/frasi.json").c_str());
    testiFrasi_.verifica(); // log su false: dati incoerenti
    disegnoShow_.setTesti(&testiFrasi_);
    ShowLayout lay = disegnoShow_.layout();
    lay.schermoW = Framebuffer::WIDTH;   // 320x224
    lay.schermoH = Framebuffer::HEIGHT;
    disegnoShow_.setLayout(lay);

    state_.seed = seed_;
    state_.frame = 0;

    caricaMusica();
    // Campioni per tick: 735 a 60 Hz, 882 a 50 Hz (la musica non resta a corto).
    audioBuf_.assign((size_t)(44100 / frequenzaHz_) * 2, 0);
    fasi_.cambia(FaseId::Gara, *this); // blocco R2: free-play (init senza gara) guida subito
}

namespace {
// "#RRGGBB" -> 0xFFRRGGBB; false se il testo non e' un colore valido.
bool leggiColore(const JsonObject& o, const char* chiave, u32& out) {
    JsonValue v;
    if (!jsonGet(o, chiave, v) || !jsonIsString(v)) return false;
    const std::string& t = jsonAsString(v);
    if (t.size() != 7 || t[0] != '#') return false;
    u32 rgb = 0;
    for (size_t i = 1; i < 7; ++i) {
        const char c = t[i];
        u32 n;
        if (c >= '0' && c <= '9') n = (u32)(c - '0');
        else if (c >= 'a' && c <= 'f') n = (u32)(c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') n = (u32)(c - 'A' + 10);
        else return false;
        rgb = (rgb << 4) | n;
    }
    out = 0xFF000000u | rgb;
    return true;
}
} // namespace

namespace {
i32 numero(const JsonObject& o, const char* chiave, i32 predefinito) {
    JsonValue v;
    return jsonGet(o, chiave, v) && jsonIsNumber(v) ? (i32)jsonAsNumber(v) : predefinito;
}
bool coppia(const JsonObject& o, const char* chiave, i32& a, i32& b) {
    JsonValue v;
    if (!jsonGet(o, chiave, v) || !jsonIsArray(v)) return false;
    const JsonArray& arr = jsonAsArray(v);
    if (arr.size() < 2 || !jsonIsNumber(arr[0]) || !jsonIsNumber(arr[1])) return false;
    a = (i32)jsonAsNumber(arr[0]);
    b = (i32)jsonAsNumber(arr[1]);
    return true;
}
bool rgb(const JsonObject& o, const char* chiave, u32& out) {
    JsonValue v;
    if (!jsonGet(o, chiave, v) || !jsonIsArray(v)) return false;
    const JsonArray& arr = jsonAsArray(v);
    if (arr.size() < 3) return false;
    u32 c = 0xFF000000u;
    for (i32 i = 0; i < 3; ++i) {
        if (!jsonIsNumber(arr[i])) return false;
        c |= ((u32)jsonAsNumber(arr[i]) & 0xFFu) << (16 - 8 * i);
    }
    out = c;
    return true;
}
std::string stringa(const JsonObject& o, const char* chiave) {
    JsonValue v;
    return jsonGet(o, chiave, v) && jsonIsString(v) ? jsonAsString(v) : std::string();
}
// Estrae il nome dello sprite da un id di catalogo tipo "oggetto.lv_01_neon.palo_neon"
// (tutto dopo l'ultimo punto): usato per risolvere il file raw per livello.
std::string nomeOggettoDaId(const std::string& id) {
    const size_t pos = id.rfind('.');
    if (pos == std::string::npos || pos + 1 >= id.size()) return id;
    return id.substr(pos + 1);
}
} // namespace

void Game::leggiHud() {
    // dati/ui/hud.json (variante B): letto una volta; senza file resta l'HUD a barre.
    if (ren_.haHud()) return;
    JsonReader file(leggiFileTesto(percorsoDati("motore_nuovo/dati/ui/hud.json")));
    if (!file.parse() || !jsonIsObject(file.valore())) return;
    JsonValue elenco;
    if (!jsonGet(jsonAsObject(file.valore()), "elementi", elenco) || !jsonIsArray(elenco)) return;
    std::vector<HudElemento> elementi;
    for (const JsonValue& v : jsonAsArray(elenco)) {
        if (!jsonIsObject(v)) continue;
        const JsonObject& o = jsonAsObject(v);
        HudElemento e;
        e.id = stringa(o, "id");
        e.tipo = stringa(o, "tipo");
        e.label = stringa(o, "label");
        e.testo = stringa(o, "testo");
        e.x = numero(o, "x", 0); e.y = numero(o, "y", 0);
        e.w = numero(o, "w", 0); e.h = numero(o, "h", 0);
        e.spessore = numero(o, "spessore", numero(o, "spessore_bordo", 2));
        coppia(o, "barra_pos", e.x, e.y);
        e.w = numero(o, "barra_w", e.w); e.h = numero(o, "barra_h", e.h);
        coppia(o, "label_pos", e.lx, e.ly);
        coppia(o, "valore_pos", e.vx, e.vy);
        coppia(o, "pos", e.tx, e.ty);
        e.scalaValore = numero(o, "scala_valore", 1);
        e.scala = numero(o, "scala", 1);
        e.scalaLabel = numero(o, "scala_label", 0);
        e.oscuraSfondo = numero(o, "oscura_sfondo", 0);
        rgb(o, "colore", e.colore);
        rgb(o, "colore_label", e.coloreLabel);
        rgb(o, "colore_bordo", e.coloreBordo);
        rgb(o, "colore_riempimento", e.coloreRiempimento);
        if (e.tipo == "cornice" || e.tipo == "testo" || e.tipo == "barra" || e.tipo == "linea") elementi.push_back(std::move(e));
    }
    ren_.impostaHud(std::move(elementi));
}

void Game::leggiTemaDaFile(const JsonValue* radice) {
    // Default storici: livelli senza "colori" o "tema_grafico" restano come prima.
    TemaColori tema;
    stileCartellone_.clear();
    if (radice && jsonIsObject(*radice)) {
        const JsonObject& root = jsonAsObject(*radice);
        JsonValue colori;
        if (jsonGet(root, "colori", colori) && jsonIsObject(colori)) {
            const JsonObject& c = jsonAsObject(colori);
            leggiColore(c, "cielo", tema.cielo);
            leggiColore(c, "erba", tema.erba);
            leggiColore(c, "erba_alt", tema.erbaAlt);
            leggiColore(c, "rumble", tema.rumbleAlt);
        }
        tempoLimiteTappaS_ = numero(root, "tempo_limite_s", 60);
        sollevamentoAttivo_ = false;
        JsonValue fx, solleva;
        if (jsonGet(root, "moduli_fx", fx) && jsonIsObject(fx) &&
            jsonGet(jsonAsObject(fx), "sollevamento_auto_alta_velocita", solleva) &&
            std::holds_alternative<bool>(solleva.v))
            sollevamentoAttivo_ = std::get<bool>(solleva.v);
        JsonValue grafica, stile, skyline, oggetti;
        if (jsonGet(root, "tema_grafico", grafica) && jsonIsObject(grafica) &&
            jsonGet(jsonAsObject(grafica), "stile_cartellone", stile) && jsonIsString(stile))
            stileCartellone_ = jsonAsString(stile);
        if (jsonGet(jsonAsObject(grafica), "skyline", skyline) && jsonIsString(skyline))
            skylineId_ = jsonAsString(skyline);
        if (jsonGet(jsonAsObject(grafica), "oggetti_bordo_strada", oggetti) && jsonIsArray(oggetti)) {
            oggettiBordoStrada_.clear();
            for (const JsonValue& o : jsonAsArray(oggetti))
                if (jsonIsString(o)) oggettiBordoStrada_.push_back(jsonAsString(o));
        }
    }
    ren_.impostaTema(tema);
}

bool Game::avviaGara(const std::string& fileTappe, bool soloPrimaTappa) {
    // Albero delle tappe (dati/percorso_gara.json): ogni tappa indica la tappa
    // a sinistra e a destra del bivio finale; senza rami e' un arrivo.
    tappe_.clear();
    JsonReader file(leggiFileTesto(percorsoDati(fileTappe)));
    std::string partenza;
    if (file.parse() && jsonIsObject(file.valore())) {
        const JsonObject& root = jsonAsObject(file.valore());
        JsonValue v, elenco;
        if (jsonGet(root, "partenza", v) && jsonIsString(v)) partenza = jsonAsString(v);
        if (jsonGet(root, "tappe", elenco) && jsonIsArray(elenco)) {
            for (const JsonValue& t : jsonAsArray(elenco)) {
                if (!jsonIsObject(t)) continue;
                const JsonObject& o = jsonAsObject(t);
                Tappa tappa;
                JsonValue campo;
                if (!jsonGet(o, "id", campo) || !jsonIsString(campo)) continue;
                tappa.id = jsonAsString(campo);
                if (jsonGet(o, "sinistra", campo) && jsonIsString(campo)) tappa.sinistra = jsonAsString(campo);
                if (jsonGet(o, "destra", campo) && jsonIsString(campo)) tappa.destra = jsonAsString(campo);
                tappe_.push_back(tappa);
            }
        }
    }
    if (partenza.empty() || !trovaTappa(partenza)) return false;

    corsaSingola_ = soloPrimaTappa; // prima di init(): caricaOggettiStrada guarda il bivio
    init(partenza);
    gara_ = true;
    tappaId_ = partenza;
    numeroTappa_ = 1;
    progressoTappePrecedenti_ = 0.0f;
    frameGara_ = 0;
    arrivo_ = false;
    tempoResiduoS_ = (f32)tempoLimiteTappaS_; // tempo della prima tappa (secondi)
    preparaBivio();
    preparaArrivo();
    inizializzaRivali();
    aggiornaStatoGara();
    fasi_.cambia(FaseId::Gara, *this); // blocco R2: avviaGara() parte guidando (gate, runner --subito)
    return true;
}

bool Game::tappaConBivio(const std::string& id) const {
    const Tappa* t = trovaTappa(id);
    return !corsaSingola_ && t && !t->sinistra.empty() && !t->destra.empty();
}

const Game::Tappa* Game::trovaTappa(const std::string& id) const {
    for (const Tappa& t : tappe_)
        if (t.id == id) return &t;
    return nullptr;
}

void Game::preparaBivio() {
    // Bivio solo se la tappa ha due rami; il cartello sta nello spartitraffico
    // poco prima del punto in cui la scelta si blocca.
    const bool rami = tappaConBivio(tappaId_);
    road_.impostaBivio(rami);
    latoBivio_ = 0;
    if (!rami) return;
    const f32 inizio = road_.bivioInizioZ() - 4000.0f;
    std::vector<SpriteInstance> tenuti;
    tenuti.reserve(cartelloni_.size() + 1);
    const Sprite* cartello = nullptr;
    for (const SpriteInstance& c : cartelloni_) {
        if (!cartello) cartello = c.sprite;
        if (c.worldZ < inizio) tenuti.push_back(c);
    }
    const i32 idx = risolviSponsorSprite("curve_pericolose");
    if (idx >= 0) cartello = ren_.sponsorSprite(sponsorSpriteIdx_[idx]);
    if (cartello) {
        SpriteInstance b;
        b.worldZ = road_.bivioBloccoZ() - 1500.0f;
        b.worldX = 0.0f;
        b.sprite = cartello;
        tenuti.push_back(b);
    }
    cartelloni_ = std::move(tenuti);
    ordinaSpriteFissi();
}

void Game::inizializzaRivali() {
    // Griglia di partenza: gli avversari partono davanti, in due file, e hanno
    // ognuno una velocita' di punta un po' sotto quella del giocatore.
    Rng rnd(seed_ ^ 0x52495641u);
    static_assert(sizeof(NOMI_RIVALI) / sizeof(NOMI_RIVALI[0]) == RIVALI &&
                  sizeof(COLORI_RIVALI) / sizeof(COLORI_RIVALI[0]) == RIVALI, "un nome e un colore per rivale");
    for (i32 i = 0; i < RIVALI; ++i) {
        Rivale& r = rivali_[i];
        r.progresso = 900.0f + (f32)(i / 2) * 1200.0f + (f32)(i % 2) * 400.0f;
        r.x = (i % 2 == 0) ? -450.0f : 450.0f;
        r.xObiettivo = r.x;
        r.passo = 0.0f;
        const f32 puntaGiocatore = player_.maxSpeed() * ROAD_SCROLL_SCALE * (1.0f + ROAD_SCROLL_SPEED_BOOST);
        r.passoMax = puntaGiocatore * (RIVALE_PUNTA_MIN + RIVALE_PUNTA_PASSO * (f32)rnd.next(7));
        r.ramo = (i % 2 == 0) ? -1 : 1;
        r.colore = 9 + (i % 4); // arancio, ciano, giallo, rosso
        r.bravura = 0.92f + 0.04f * (f32)rnd.next(5); // bravura propria (0.92..1.08), seme fisso
        r.passoMax *= r.bravura; // la bravura scala la velocita' di punta (punte e elastico invariati)
        // Mai piu' veloci del giocatore a tutto gas (29/10): una guida pulita puo' vincere.
        r.passoMax = std::min(r.passoMax, puntaGiocatore * 0.99f);
        r.spriteIdx = nSpriteRivali_ > 0 ? i % nSpriteRivali_ : -1; // sprite rivale (RIV punto b)
        r.ritmoFase = 0.9f * (f32)i;
        r.ritmoVel = 0.15f + 0.04f * (f32)rnd.next(5);
    }
    classifica_.clear();
    rndRivali_ = Rng(seed_ ^ 0x4C414E45u);
    sprites_.clear();
    spritesFonte_.clear();
    spritesFonte_.reserve(RIVALI + TRAFFICO);
    sprites_.reserve(RIVALI + TRAFFICO);
    inizializzaTraffico();
}

void Game::setTrafficoSprites(std::vector<Sprite> sprites) {
    ren_.clearTrafficoSprites();
    nSpriteTraffico_ = 0;
    for (auto& s : sprites) {
        if (s.width > 0) {
            ren_.aggiungiTrafficoSprite(std::move(s));
            ++nSpriteTraffico_;
        }
    }
}

f32 Game::xCorsia(i32 corsia, f32 z) const {
    // Corsie larghe TRAFFICO_CORSIA: +-600 sulla strada normale, +-1800 per
    // le esterne quando la strada e' larga 2 (4 corsie). Le esterne seguono il
    // bordo mentre la strada si allarga o si stringe.
    const f32 meta = Road::ROAD_HALF_WORLD * road_.larghezzaAt(z);
    const f32 lato = corsia < 0 ? -1.0f : 1.0f;
    const f32 centro = lato * ((f32)std::abs(corsia) - 0.5f) * TRAFFICO_CORSIA;
    const f32 limite = std::max(0.5f * TRAFFICO_CORSIA, meta - 0.5f * TRAFFICO_CORSIA);
    return std::clamp(centro, -limite, limite);
}

void Game::inizializzaTraffico() {
    // Seme fisso per tappa: stessa partita = stesso traffico.
    rndTraffico_ = Rng(seed_ ^ 0x54524146u);
    for (Veicolo& v : traffico_) v.attivo = false;
    f32 z = TRAFFICO_PRIMO;
    for (Veicolo& v : traffico_) {
        piazzaVeicolo(v, z);
        if (!v.attivo) break;
        z = v.z + TRAFFICO_DISTANZA_MIN + (f32)rndTraffico_.next(TRAFFICO_DISTANZA_VAR);
    }
}

void Game::piazzaVeicolo(Veicolo& v, f32 z) {
    if (z > road_.bivioInizioZ() - TRAFFICO_MARGINE_BIVIO) { // bivio e arrivo liberi
        v.attivo = false;
        return;
    }
    v.attivo = true;
    v.z = z;
    const i32 lato = rndTraffico_.next(2) == 0 ? -1 : 1;
    const bool largo = road_.larghezzaAt(z) >= 1.75f; // 4 corsie: anche le esterne
    v.corsia = lato * (largo ? 1 + (i32)rndTraffico_.next(2) : 1);
    v.x = xCorsia(v.corsia, z);
    const f32 punta = player_.maxSpeed() * ROAD_SCROLL_SCALE * (1.0f + ROAD_SCROLL_SPEED_BOOST);
    v.passo = punta * (TRAFFICO_VEL_MIN + TRAFFICO_VEL_PASSO * (f32)rndTraffico_.next(5));
    v.ramo = rndTraffico_.next(2) == 0 ? -1 : 1;
    v.spriteIdx = nSpriteTraffico_ > 0 ? (i32)rndTraffico_.next((u32)nSpriteTraffico_) : -1;
}

void Game::aggiornaTraffico(f32 dt) {
    if (arrivo_) return;
    // Chi e' stato superato torna davanti, oltre la vista (nessuna comparsa
    // improvvisa) e dopo il veicolo piu' avanti.
    const f32 vista = (f32)((Road::DRAW_DIST + 2) * Road::SEG_LEN);
    f32 zLibero = road_.playerZ + vista;
    for (const Veicolo& v : traffico_)
        if (v.attivo) zLibero = std::max(zLibero, v.z + TRAFFICO_DISTANZA_MIN);
    for (Veicolo& v : traffico_) {
        if (!v.attivo || v.z < road_.playerZ - TRAFFICO_DIETRO) {
            piazzaVeicolo(v, zLibero + (f32)rndTraffico_.next(TRAFFICO_DISTANZA_VAR));
            if (v.attivo) zLibero = v.z + TRAFFICO_DISTANZA_MIN;
            continue;
        }
        f32 passo = v.passo;
        for (const Veicolo& a : traffico_) { // stessa corsia, piu' lento davanti: si accoda
            if (&a == &v || !a.attivo || std::abs(a.x - v.x) > 300.0f) continue;
            const f32 dz = a.z - v.z;
            if (dz > 0.0f && dz < TRAFFICO_CODA) passo = std::min(passo, a.passo);
        }
        v.z += passo * dt;
        // Dove la strada si stringe la corsia esterna finisce: il veicolo
        // rientra guardando 1 s avanti, prima che il bordo lo raggiunga.
        const f32 qui = xCorsia(v.corsia, v.z), avanti = xCorsia(v.corsia, v.z + passo);
        const f32 obiettivo = std::abs(avanti) < std::abs(qui) ? avanti : qui;
        const f32 lat = TRAFFICO_LATERALE_PER_S * dt;
        v.x = v.x < obiettivo ? std::min(obiettivo, v.x + lat) : std::max(obiettivo, v.x - lat);
    }
}

void Game::aggiungiSpriteTraffico() {
    if (arrivo_) return;
    for (i32 k = 0; k < TRAFFICO; ++k) {
        const Veicolo& v = traffico_[k];
        if (!v.attivo) continue;
        const f32 zRel = v.z - road_.playerZ;
        if (zRel <= 0.0f || zRel > 60000.0f) continue;
        // Al bivio ogni veicolo segue il suo ramo, come gli avversari.
        const Road::Carreggiate c = road_.carreggiate(v.z, latoBivio_);
        const i32 indice = latoBivio_ == 0 ? (v.ramo < 0 ? 0 : 1) : (v.ramo == latoBivio_ ? 0 : 1);
        SpriteInstance s;
        s.worldZ = road_.playerZ + zRel;
        s.worldX = c.centro[indice] + v.x * (c.doppia ? c.fattore : 1.0f);
        s.scale = SCALA_AUTO;
        s.sprite = ren_.trafficoSprite(v.spriteIdx);
        s.paletteBase = 10; // sagoma sintetica se manca lo sprite
        sprites_.push_back(s);
        spritesFonte_.push_back(-1 - k);
    }
}

void Game::aggiornaRivali() {
    const f32 giro = (f32)road_.totalLength();
    const f32 dt = 1.0f / (f32)frequenzaHz_;
    const f32 avvicina = 1.0f - std::exp(-dt / RIVALE_ACCEL_TAU_S);
    const f32 velocitaGiocatore = passoGiocatore_ / dt; // unita' al secondo
    const f32 puntaGiocatore = player_.maxSpeed() * ROAD_SCROLL_SCALE * (1.0f + ROAD_SCROLL_SPEED_BOOST);
    const f32 mio = progressoTappePrecedenti_ + player_.z;
    sprites_.clear();
    spritesFonte_.clear();
    for (i32 i = 0; i < RIVALI; ++i) {
        Rivale& r = rivali_[i];
        const f32 zLocale = r.progresso - progressoTappePrecedenti_;
        // Rallenta in curva in base alla curvatura del segmento (piu' curva = piu' lento),
        // attenuato dalla bravura: i rivali piu' bravi soffrono meno la curva.
        const f32 curvaQui = std::abs(road_.curvaAt(zLocale));
        const f32 lentaCurva = 1.0f - std::clamp(curvaQui * RIVALE_CURVA_SCALE * r.bravura,
                                                  0.0f, RIVALE_CURVA_SLOW_MAX);
        // Elastico solo lontano dal giocatore: vicino ognuno va del suo ritmo
        // (sorpassi veri, niente gruppo incollato); lontano davanti rallenta
        // quanto basta perche' una guida pulita lo riprenda, lontano dietro spinge.
        const f32 distacco = r.progresso - mio;
        const f32 distaccoS = distacco / puntaGiocatore;
        f32 elastico = 1.0f;
        if (distaccoS > RIVALE_LIBERO_S)
            elastico = 1.0f - 0.15f * std::min(1.0f, (distaccoS - RIVALE_LIBERO_S) / RIVALE_ELASTICO_RAMPA_S);
        else if (distaccoS < -RIVALE_LIBERO_S)
            elastico = 1.0f + 0.25f * std::min(1.0f, (-distaccoS - RIVALE_LIBERO_S) / RIVALE_ELASTICO_RAMPA_S);
        r.ritmoFase += r.ritmoVel * dt;
        const f32 ritmo = 1.0f + RIVALE_RITMO * std::sin(r.ritmoFase);
        r.passo += (r.passoMax * elastico * ritmo * lentaCurva - r.passo) * avvicina;
        // Chi arriva da dietro non tampona il giocatore: cambia corsia e resta
        // alla sua velocita' solo finche' e' ancora dritto dietro di lui.
        const f32 dxGiocatore = r.x - (player_.x - player_.centroCorsia);
        if (distacco < Game::URTO_Z_MIN && distacco > Game::URTO_Z_MIN - 4000.0f && std::abs(dxGiocatore) < 400.0f) {
            r.xObiettivo = (player_.x - player_.centroCorsia) > 0.0f ? -450.0f : 450.0f;
            if (distacco > Game::URTO_Z_MIN - 800.0f && std::abs(dxGiocatore) < URTO_LARGHEZZA)
                r.passo = std::min(r.passo, velocitaGiocatore);
        }
        // Distanza minima tra rivali: chi raggiunge un altro rivale rallenta e,
        // se molto vicino, si sposta di lato per superarlo (corsia libera).
        for (i32 j = 0; j < RIVALI; ++j) {
            if (j == i) continue;
            const Rivale& q = rivali_[j];
            // q davanti a r (29/10: prima il segno era invertito e a rallentare
            // era chi stava davanti: i rivali si incollavano in fila a un terzo della velocita').
            const f32 dz = q.progresso - r.progresso;
            if (dz > 0.0f && dz < RIVALE_DISTANZA_MIN && std::abs(r.x - q.x) < RIVALE_LATERALE_MIN) {
                r.passo = std::min(r.passo, q.passo * 0.95f);
                if (dz < RIVALE_DISTANZA_MIN * 0.5f) {
                    // Sorpasso dal lato libero; al bordo si passa dall'altra parte.
                    f32 dir = r.x > q.x ? 1.0f : (r.x < q.x ? -1.0f : (r.x <= 0.0f ? 1.0f : -1.0f));
                    if (std::abs(r.xObiettivo) >= RIVALE_X_MAX - 1.0f && r.xObiettivo * dir > 0.0f) dir = -dir;
                    r.xObiettivo = std::clamp(r.xObiettivo + dir * RIVALE_SPOSTAMENTO, -RIVALE_X_MAX, RIVALE_X_MAX);
                }
            }
        }
        // Traffico davanti nella stessa corsia: cambia corsia per tempo e, se
        // e' gia' attaccato, si accoda finche' non si e' spostato.
        bool schiva = false;
        if (zLocale > 0.0f && zLocale < giro) {
            const f32 f = std::max(0.5f, road_.carreggiate(zLocale, latoBivio_).fattore);
            const f32 xMondo = r.x * f;
            for (const Veicolo& v : traffico_) {
                if (!v.attivo) continue;
                const f32 dz = v.z - zLocale;
                if (dz <= 0.0f || dz > RIVALE_SCHIVA_DIST) continue;
                if (road_.bivioAttivo() && v.z >= road_.bivioInizioZ() && v.ramo != r.ramo) continue;
                const f32 dx = xMondo - v.x;
                if (std::abs(dx) >= RIVALE_SCHIVA_LATERALE) continue;
                schiva = true;
                f32 obiettivo = v.x + (dx >= 0.0f ? 1.0f : -1.0f) * RIVALE_SCHIVA_MARGINE;
                if (std::abs(obiettivo) > RIVALE_X_MAX * f) // non c'e' spazio: dall'altra parte
                    obiettivo = v.x - (dx >= 0.0f ? 1.0f : -1.0f) * RIVALE_SCHIVA_MARGINE;
                r.xObiettivo = std::clamp(obiettivo / f, -RIVALE_X_MAX, RIVALE_X_MAX);
                if (dz < 900.0f && std::abs(dx) < 250.0f) r.passo = std::min(r.passo, v.passo);
            }
        }
        r.progresso += r.passo * dt;
        // Le auto sono solide: chi arriva da dietro non attraversa il giocatore.
        const bool nelBivio = road_.bivioAttivo() && zLocale >= road_.bivioInizioZ();
        if (!nelBivio && distacco <= Game::URTO_Z_MIN && r.progresso > mio + Game::URTO_Z_MIN &&
            std::abs(dxGiocatore) < URTO_LARGHEZZA && !arrivo_) {
            r.progresso = mio + Game::URTO_Z_MIN;
            r.passo = std::min(r.passo, velocitaGiocatore);
        }
        const f32 laterale = (schiva ? RIVALE_SCHIVA_PER_S : RIVALE_LATERALE_PER_S) * dt;
        if (r.x < r.xObiettivo) r.x = std::min(r.xObiettivo, r.x + laterale);
        if (r.x > r.xObiettivo) r.x = std::max(r.xObiettivo, r.x - laterale);

        const f32 zRel = zLocale - road_.playerZ;
        if (zRel <= 0.0f || zRel > 60000.0f || zLocale >= giro + 60000.0f) continue;
        // Sul bivio ogni avversario segue il suo ramo.
        const Road::Carreggiate c = road_.carreggiate(zLocale, latoBivio_);
        const i32 indice = latoBivio_ == 0 ? (r.ramo < 0 ? 0 : 1) : (r.ramo == latoBivio_ ? 0 : 1);
        SpriteInstance s;
        s.worldZ = road_.playerZ + zRel;
        s.worldX = c.centro[indice] + r.x * c.fattore;
        s.scale = SCALA_RIVALE;
        s.sprite = r.spriteIdx >= 0 ? ren_.rivaleSprite(r.spriteIdx) : nullptr; // RIV punto b: sprite vero se disponibile
        s.paletteBase = r.colore;
        s.etichetta = NOMI_RIVALI[i];
        s.coloreEtichetta = COLORI_RIVALI[i];
        sprites_.push_back(s);
        spritesFonte_.push_back(i);
    }
}

void Game::fineTappa(f32 avanzo) {
    // Fine del tracciato in gara: il ramo scelto porta alla tappa seguente;
    // una tappa senza rami e' l'arrivo.
    const Tappa* t = corsaSingola_ ? nullptr : trovaTappa(tappaId_); // corsa singola: una tappa e arrivo
    const std::string prossima = !t ? std::string() : (latoBivio_ < 0 ? t->sinistra : t->destra);
    if (prossima.empty() || !trovaTappa(prossima)) {
        if (!arrivo_) {
            arrivo_ = true;
            arrivoS_ = 0.0f;
            fermoS_ = -1.0f;
            velocitaArrivo_ = player_.speed;
            posizioneArrivo_ = posizioneGiocatore();
            calcolaClassifica(); // posizione d'arrivo dal tempo di ognuno
        }
        return;
    }
    // Raccordo curva/collina con la tappa che finisce: il nuovo tracciato
    // parte dalla quota/curvatura dell'ultimo segmento della tappa precedente
    // (cambio tappa senza scatto di prospettiva).
    const auto& segsFine = road_.segments();
    if (!segsFine.empty()) {
        const RoadSegment& last = segsFine.back();
        road_.setCurvaInizioTappa(last.curve);
        road_.setCollinaInizioTappa(last.hill);
    }
    const TemaColori temaPrima = ren_.tema(); // tema della tappa che finisce (ancora corrente)
    const Sprite skylinePrima = ren_.skylineCorrente(); // skyline prima del cambio tappa (foto)
    const f32 giroFinito = (f32)road_.totalLength();
    progressoTappePrecedenti_ += giroFinito;
    // Traffico: resta solo chi e' davanti sul ramo scelto (continua nella
    // tappa nuova); gli altri tornano davanti quando serve.
    for (Veicolo& v : traffico_) {
        if (v.attivo && v.ramo == latoBivio_ && v.z > giroFinito) v.z -= giroFinito;
        else v.attivo = false;
    }
    caricaLivello(prossima); // imposta subito il tema e lo skyline della tappa nuova (impostaTema)
    // Transizione graduale verso il tema di arrivo (FORK1 punto 6): i colori di
    // ambiente e lo skyline si interpolano in transizioneTemaS_ secondi, niente scatto visivo.
    ren_.iniziaTransizioneTema(temaPrima, skylinePrima, transizioneTemaS_);
    tappaId_ = prossima;
    ++numeroTappa_;
    if (!tempoScaduto_) {
        tempoResiduoS_ += (f32)tempoLimiteTappaS_; // checkpoint: tempo extra (secondi)
        tempoExtraS_ = TEMPO_EXTRA_AVVISO_S;
    }
    saltoQuotaPrec_ = -1e9f; // tracciato nuovo: la quota riparte
    preparaBivio();
    preparaArrivo();
    road_.playerZ = avanzo;
    player_.z = avanzo;
    road_.playerX = player_.x;
}

i32 Game::posizioneGiocatore() const {
    const f32 mio = progressoTappePrecedenti_ + player_.z;
    i32 pos = 1;
    for (const Rivale& r : rivali_)
        if (r.progresso > mio) ++pos;
    return pos;
}

void Game::aggiornaStatoGara() {
    state_.gara = gara_;
    state_.tappa = numeroTappa_;
    state_.latoBivio = latoBivio_;
    state_.concorrenti = RIVALI + 1;
    state_.posizione = arrivo_ ? posizioneArrivo_ : posizioneGiocatore();
    state_.arrivo = arrivo_;
    state_.frameGara = frameGara_;
    state_.tempoResiduoFrame = (u32)tempoResiduoS_;
    state_.tempoScaduto = tempoScaduto_;
}

void Game::setInput(const Pad& pad) {
    input_ = pad;
}

i32 Game::risolviSponsorSprite(const std::string& sponsorId) const {
    if (sponsorId.empty()) return -1;
    // Prima lo stile del cartellone della tappa ("id/stile"), poi qualsiasi stile.
    if (!stileCartellone_.empty()) {
        const std::string conStile = sponsorId + "/" + stileCartellone_;
        for (size_t i = 0; i < sponsorId_.size(); ++i)
            if (sponsorId_[i] == conStile) return (i32)i;
    }
    for (size_t i = 0; i < sponsorId_.size(); ++i) {
        const std::string& id = sponsorId_[i];
        if (id == sponsorId || (id.size() > sponsorId.size() && id.compare(0, sponsorId.size(), sponsorId) == 0 &&
                                id[sponsorId.size()] == '/'))
            return (i32)i;
    }
    return -1; // sconosciuto: il chiamante sceglie a caso (seeded RNG)
}

void Game::setSponsorSprites(std::vector<std::pair<std::string, Sprite>> sprites) {
    sponsorSpriteIdx_.clear();
    ren_.clearSponsorSprites(); // non accumulare sprite sponsor tra test/esegui
    sponsorId_.clear();
    sponsorSpriteIdx_.reserve(sprites.size());
    sponsorId_.reserve(sprites.size());
    for (auto& p : sprites) {
        sponsorId_.push_back(std::move(p.first));
        sponsorSpriteIdx_.push_back(ren_.aggiungiSponsorSprite(std::move(p.second)));
    }
    // Possesso esclusivo del Renderer; puntatori stabili dopo questa chiamata.
}

void Game::setSkylineSprite(Sprite sprite) {
    // Consegnato dal runner (o dal test) prima di init(); applicato in
    // caricaLivello() alla tappa che viene caricata, poi svuotato.
    skylineSprite_ = std::move(sprite);
}

void Game::setRivaliSprites(std::vector<Sprite> sprites) {
    // RIV punto b: consegna gli sprite delle auto rivali (decodati dal runner o
    // dal test dai raw prodotti da png_a_raw.py) prima di avviaGara()/init().
    // Senza sprite si mantiene la sagoma sintetica del renderer (retrocompatibile).
    ren_.clearRivaliSprites();
    nSpriteRivali_ = 0;
    for (auto& s : sprites) {
        if (s.width > 0) {
            ren_.aggiungiRivaleSprite(std::move(s));
            ++nSpriteRivali_;
        }
    }
    std::printf("sprite rivali caricati: %d/%d\n", nSpriteRivali_, RIVALI);
}

bool Game::leggiSegmentiDaFile(const JsonValue* radice, std::vector<Road::SegmentDesc>& out) const {
    if (!radice) return false; // file assente o non valido: init() usa il fallback hardcoded
    const JsonValue& root = *radice;
    JsonValue campo;
    if (!jsonIsObject(root) ||
        !jsonGet(jsonAsObject(root), "segmenti", campo) ||
        !jsonIsArray(campo)) {
        return false; // campo mancante: init() usa il fallback hardcoded
    }

    const JsonArray& arr = jsonAsArray(campo);
    out.clear();
    out.reserve(arr.size()); // tutte le alloc in init

    for (const JsonValue& v : arr) {
        if (!jsonIsObject(v)) continue;
        const JsonObject& o = jsonAsObject(v);

        JsonValue cv, hv, lv;
        if (!jsonGet(o, "curva", cv) || !jsonIsNumber(cv)) continue; // curva obbligatoria
        if (!jsonGet(o, "collina", hv) || !jsonIsNumber(hv)) continue; // collina obbligatoria
        Road::SegmentDesc d;
        d.curva = (f32)jsonAsNumber(cv);
        d.collina = (f32)jsonAsNumber(hv);
        if (jsonGet(o, "lunghezza", lv) && jsonIsNumber(lv)) {
            i32 len = (i32)jsonAsNumber(lv);
            if (len > 0) d.lunghezza = len;
        }
        out.push_back(d);
    }
    return !out.empty();
}

bool Game::leggiCartelloniDaFile(const JsonValue* radice, std::vector<bool>& segmentoOccupato) {
    if (!radice) return false; // file assente o non valido: init() usa il fallback casuale deterministico
    const JsonValue& root = *radice;
    JsonValue campo;
    if (!jsonIsObject(root) ||
        !jsonGet(jsonAsObject(root), "cartelloni_sponsor", campo) ||
        !jsonIsArray(campo)) {
        return false; // campo mancante: init() usa il fallback casuale deterministico
    }

    const JsonArray& arr = jsonAsArray(campo);
    cartelloni_.reserve(cartelloni_.size() + arr.size()); // tutte le alloc in init
    Rng rndCampi(seed_); // campi mancanti dentro una voce presente

    // Guardia: senza sprite sponsor caricati l'indice sotto e' fuori range
    // (o modulo per zero). init() chiama questa funzione solo se non vuota,
    // ma la guardia rende la funzione sicura anche da altre chiamate.
    if (sponsorSpriteIdx_.empty()) return false;

    for (const JsonValue& v : arr) {
        if (!jsonIsObject(v)) continue;
        const JsonObject& o = jsonAsObject(v);

        JsonValue segV;
        if (!jsonGet(o, "segmento", segV) || !jsonIsNumber(segV)) continue; // segmento obbligatorio
        const i32 segmento = (i32)jsonAsNumber(segV);
        if (segmento >= 0 && segmento < (i32)road_.segments().size())
            segmentoOccupato[(size_t)segmento] = true; // posizionamento esplicito del file

        f32 worldX;
        JsonValue latoV;
        if (jsonGet(o, "lato", latoV) && jsonIsString(latoV)) {
            const std::string& lato = jsonAsString(latoV);
            if (lato == "destro") worldX = 1600.0f;
            else if (lato == "sinistro") worldX = -1600.0f;
            else worldX = (rndCampi.next(2) == 0) ? -1600.0f : 1600.0f; // lato non valido: a caso (seed)
        } else {
            worldX = (rndCampi.next(2) == 0) ? -1600.0f : 1600.0f; // lato mancante: a caso (seed)
        }

        std::string sponsorId;
        JsonValue idV;
        if (jsonGet(o, "sponsor_id", idV) && jsonIsString(idV)) sponsorId = jsonAsString(idV);
        i32 spriteIdx = risolviSponsorSprite(sponsorId);
        if (spriteIdx < 0) {
            // sponsor a caso (seed).
            // Guardia: senza sprite registrati (setSponsorSprites non chiamato)
            // l'accesso sotto andrebbe fuori range (modulo per zero). init()
            // usa allora il posizionamento cartelloni di fallback.
            if (sponsorSpriteIdx_.empty()) return false;
            spriteIdx = sponsorSpriteIdx_[rndCampi.next((u32)sponsorSpriteIdx_.size())];
        }

        SpriteInstance b;
        b.worldZ = (f32)segmento * (f32)Road::SEG_LEN;
        b.worldX = worldX;
        b.scale = 1.0f;
        b.sprite = ren_.sponsorSprite(spriteIdx);
        b.paletteBase = 0;
        cartelloni_.push_back(b);
    }
    return true;
}

void Game::update() {
    // Logica deterministica, nessun orologio di sistema.
    f32 dt = 1.0f / (f32)frequenzaHz_; // un frame di gioco, in secondi (frequenza da config.json)
    ren_.aggiornaTransizioneTema(dt); // transizione graduale tema tra tappe (FORK1 punto 6)
    fasi_.aggiorna(*this, dt);        // blocco R2: macchina a stati (menu, risultati, ...)
    aggiornaRadio();
    if (scenaStrada()) {
        aggiornaGuida(dt);
    } else {
        aggiornaMenu(dt);
    }
    // Aggiorna stato esportato (comune a tutte le fasi).
    state_.frame = clock_.frame;
    state_.playerSpeed = player_.speed;
    state_.playerX = player_.x;
    state_.playerZ = player_.z;
    state_.playerOffroad = player_.offroad;
    state_.playerFlipping = playerFlipping_;
    state_.playerFlipFrame = playerFlipFrame_;
    if (gara_) {
        if (!arrivo_ && !tempoScaduto_) {
            ++frameGara_;
            if (tempoResiduoS_ > 0.0f) tempoResiduoS_ -= dt;
            if (tempoResiduoS_ <= 0.0f) tempoScaduto_ = true; // game over
        }
        aggiornaStatoGara();
        // Tempo scaduto o arrivo: FineTempo, poi Risultati (blocco R2).
        if (fasi_.attuale() == FaseId::Gara && (tempoScaduto_ || arrivo_))
            fasi_.cambia(FaseId::FineTempo, *this);
    }
    clock_.tick();
}

void Game::aggiornaGuida(f32 dt) {
    if (playerFlipping_) {
        passoGiocatore_ = 0.0f;
        ++playerFlipFrame_;
        const u32 frameFlip = (u32)(FLIP_ANIMATION_S * (f32)frequenzaHz_);
        // Si riparte dal punto dell'incidente, al centro della propria corsia.
        // Al bivio con la scelta ancora aperta centroCorsia e' 0, cioe' lo
        // spartitraffico: si ripartiva in mezzo ai rami e si ribaltava per
        // sempre (prova di Andrea 23/10). Si riparte sul ramo piu' vicino.
        f32 xRipartenza = player_.centroCorsia;
        const Road::Carreggiate cr = road_.carreggiate(player_.z, latoBivio_);
        if (cr.doppia && latoBivio_ == 0)
            xRipartenza = playerXIncidente_ < 0.0f ? cr.centro[0] : cr.centro[1];
        // Rientro animato: nella seconda meta' del ribaltamento l'auto scivola
        // dolcemente verso il centro invece di comparirci di colpo.
        const f32 t = std::clamp(2.0f * (f32)playerFlipFrame_ / (f32)frameFlip - 1.0f, 0.0f, 1.0f);
        const f32 morbido = t * t * (3.0f - 2.0f * t);
        player_.x = playerXIncidente_ + (xRipartenza - playerXIncidente_) * morbido;
        road_.playerX = player_.x;
        if (playerFlipFrame_ >= frameFlip) {
            playerFlipping_ = false;
            playerFlipFrame_ = 0;
            offroadFrames_ = 0;
            player_.x = xRipartenza;
            player_.speed = 0.0f;
            player_.offroad = false;
            road_.playerX = player_.x;
        }
        if (gara_) { // gli avversari e il traffico non aspettano
            aggiornaTraffico(dt);
            aggiornaRivali();
            aggiungiSpriteTraffico();
        }
    } else {
        // Corsia dell'auto: al bivio e' il ramo piu' vicino.
        const Road::Carreggiate carr = road_.carreggiate(player_.z, latoBivio_);
        if (latoBivio_ == 0 && !carr.doppia) {
            player_.centroCorsia = 0.0f;
            player_.larghezzaCorsia = carr.fattore; // > 1 dove la strada si allarga
        } else if (latoBivio_ == 0) {
            // Scelta ancora aperta: corsia = ramo seguito, cosi' l'auto
            // segue il ramo mentre si apre fino a BIVIO_DIST. Con il centro
            // fisso a 0 il clamp X_LIMIT (900) la teneva sullo spartitraffico,
            // fuori strada, e al blocco scattava di 100 unita' (gate
            // bivio_uscita, 25/10). Il ramo si sceglie una volta (lato dell'auto,
            // a parita' lo sterzo) e cambia solo se l'auto e' chiaramente piu'
            // vicina all'altro: prima la scelta "piu' vicino" e il richiamo
            // usavano due regole diverse a x = 0 e l'auto restava in mezzo.
            if (ramoProvvisorio_ == 0) {
                // Lato dello sterzo se il giocatore sterza, altrimenti lato dell'auto.
                const i32 sterzo = sterzoBivio();
                ramoProvvisorio_ = sterzo != 0 ? sterzo : (player_.x < 0.0f ? -1 : 1);
                centroRamoPrec_ = carr.centro[ramoProvvisorio_ < 0 ? 0 : 1];
            }
            const i32 attuale = ramoProvvisorio_ < 0 ? 0 : 1;
            const f32 dAttuale = std::abs(player_.x - carr.centro[attuale]);
            const f32 dAltro = std::abs(player_.x - carr.centro[1 - attuale]);
            if (dAltro + BIVIO_ISTERESI < dAttuale) {
                ramoProvvisorio_ = -ramoProvvisorio_;
                centroRamoPrec_ = carr.centro[1 - attuale];
            }
            player_.centroCorsia = carr.centro[ramoProvvisorio_ < 0 ? 0 : 1];
            player_.larghezzaCorsia = carr.fattore;
        } else {
            const i32 vicino = std::abs(player_.x - carr.centro[0]) <= std::abs(player_.x - carr.centro[1]) ? 0 : 1;
            player_.centroCorsia = carr.centro[vicino];
            player_.larghezzaCorsia = carr.fattore;
        }
        Pad comandi = input_;
        if (arrivo_ || tempoScaduto_) comandi = Pad{}; // dopo arrivo o tempo scaduto l'auto va per inerzia
        player_.input(comandi, dt);
        player_.update(dt);
        if (arrivo_) frenataArrivo(dt);

        road_.playerX = player_.x;
        const f32 roadScrollScale = ROAD_SCROLL_SCALE *
            (1.0f + ROAD_SCROLL_SPEED_BOOST * player_.speed / player_.maxSpeed());
        const f32 zPrima = road_.playerZ;
        passoGiocatore_ = player_.speed * dt * roadScrollScale;
        // Auto solide: il giocatore non passa attraverso chi ha davanti; si
        // ferma sulla sua coda (anche se in un frame percorrerebbe di piu').
        bool tampona = false;
        i32 tamponato = 0;
        f32 dxTamponato = 0.0f;
        if (gara_ && !playerFlipping_) {
            for (size_t k = 0; k < sprites_.size() && k < spritesFonte_.size(); ++k) {
                const f32 dz = sprites_[k].worldZ - road_.playerZ;
                const f32 dx = sprites_[k].worldX - player_.x;
                // Solo chi e' ancora davanti alla carrozzeria; chi e' gia' a fianco e' un urto laterale.
                if (dz < URTO_Z_DAVANTI - 50.0f || std::abs(dx) >= URTO_LARGHEZZA) continue;
                const f32 libero = std::max(0.0f, dz - URTO_Z_DAVANTI + passoFonte(spritesFonte_[k]) * dt);
                if (passoGiocatore_ > libero) {
                    passoGiocatore_ = libero;
                    tampona = true;
                    tamponato = spritesFonte_[k];
                    dxTamponato = dx;
                }
            }
            if (tampona) {
                if (!playerBump_ && assistenzaBivio_ <= 0.0f && !arrivo_) urto(tamponato, dxTamponato, true);
                // Spinge sulla coda: la velocita' resta quella di chi sta davanti.
                const f32 vDavanti = passoFonte(tamponato) / roadScrollScale;
                player_.speed = std::max(0.05f * player_.maxSpeed(), std::min(player_.speed, vDavanti));
            }
        }
        // Contachilometri coerente con il tachimetro: a 290 km/h ~80 m al secondo.
        km_ += player_.speed / player_.maxSpeed() * VELOCITA_MAX_KMH / 3600.0f * dt;
        road_.advance(passoGiocatore_);
        if (gara_ && road_.playerZ < zPrima) fineTappa(road_.playerZ); // fine del tracciato
        player_.z = road_.playerZ;
        // DECO1 2e: parallasse leggero in curva dello skyline, opposto alla curva
        // e proporzionale allo scorrere avanti della strada (zero alloc, deterministico).
        const f32 curva = road_.curvaAt(player_.z);
        ren_.scorrereSkyline(-curva * passoGiocatore_ * 0.10f);
        // Bivio: oltre il punto di blocco la scelta e' fatta; l'auto viene
        // ricentrata sul ramo scelto (stessa immagine, nessuno scatto).
        bool sceltaFattaOra = false;
        if (road_.bivioAttivo() && latoBivio_ == 0 && player_.z >= road_.bivioBloccoZ()) {
            latoBivio_ = player_.x < 0.0f ? -1 : 1;
            ramoProvvisorio_ = 0;
            player_.x -= (f32)latoBivio_ * Road::BIVIO_DIST;
            player_.centroCorsia = 0.0f;
            road_.playerX = player_.x;
            secondiDaSceltaBivio_ = 0.0f;
            sceltaFattaOra = true;
        }
        if (!sceltaFattaOra && latoBivio_ != 0) {
            secondiDaSceltaBivio_ += dt;
        }
        road_.setSecondiDaSceltaBivio(secondiDaSceltaBivio_);

        // Le curve spingono l'auto verso l'esterno, con intensita' crescente alla velocita'.
        // Le curve ora durano centinaia di metri: la spinta resta sotto lo sterzo
        // (a curva 0.8 e velocita' massima circa 7 unita'/frame contro 11 di sterzo),
        // cosi' la curva si tiene sterzando.
        const bool nelBivio = road_.bivioAttivo() && player_.z >= road_.bivioInizioZ();
        assistenzaBivio_ = nelBivio ? std::min(1.0f, assistenzaBivio_ + dt / ASSIST_ENTRATA_S)
                                    : std::max(0.0f, assistenzaBivio_ - dt / ASSIST_USCITA_S);
        const f32 curvaQui = road_.curvaAt(player_.z);
        const f32 spinta = player_.offroad ? CURVA_SPINTA * CURVA_SPINTA_OFFROAD_FACTOR : CURVA_SPINTA;
        player_.x += curvaQui * player_.speed * spinta * dt * roadScrollScale * (1.0f - assistenzaBivio_);
        if (assistenzaBivio_ > 0.0f) {
            // Richiamo morbido verso il centro del ramo piu' vicino.
            const Road::Carreggiate c = road_.carreggiate(player_.z, latoBivio_);
            f32 centro = player_.centroCorsia;
            // Il giocatore sterza verso l'altro ramo: nessun aiuto finche' non ci
            // arriva (prima l'aiuto tirava dalla parte opposta e l'auto restava in mezzo).
            bool aiuto = true;
            if (c.doppia && latoBivio_ == 0 && ramoProvvisorio_ != 0) {
                // L'auto si sposta insieme al ramo che si apre (prima lo inseguiva
                // a 250 unita'/s mentre il ramo si allontanava piu' in fretta:
                // ~4 s nell'erba dello spartitraffico e -40% di velocita').
                centro = c.centro[ramoProvvisorio_ < 0 ? 0 : 1];
                const i32 sterzo = sterzoBivio();
                aiuto = sterzo == 0 || sterzo == ramoProvvisorio_;
                if (aiuto) player_.x += (centro - centroRamoPrec_) * assistenzaBivio_;
                centroRamoPrec_ = centro;
            }
            f32 richiamo = (centro - player_.x) * (1.0f - std::exp(-dt / ASSIST_RICHIAMO_TAU_S));
            const f32 maxPasso = ASSIST_RICHIAMO_MAX * dt;
            richiamo = std::clamp(richiamo, -maxPasso, maxPasso);
            if (aiuto) player_.x += richiamo * assistenzaBivio_;
        }
        player_.vincolaAllaCorsia();

        const bool inVolo = sollevamentoAttivo_ && !player_.offroad &&
            player_.speed >= SOLLEVAMENTO_SOGLIA_V * player_.maxSpeed() &&
            std::abs(curvaQui) <= SOLLEVAMENTO_CURVA_MAX;
        const f32 obiettivo = inVolo ? 1.0f : 0.0f;
        const f32 passoSoll = dt / (inVolo ? SOLLEVAMENTO_SALITA_S : SOLLEVAMENTO_DISCESA_S);
        sollevamento_ = sollevamento_ < obiettivo ? std::min(obiettivo, sollevamento_ + passoSoll)
                                                  : std::max(obiettivo, sollevamento_ - passoSoll);
        sollevamentoFaseS_ = sollevamento_ > 0.0f ? sollevamentoFaseS_ + dt : 0.0f;
        aggiornaSalto(dt);
        if (tempoExtraS_ > 0.0f) tempoExtraS_ -= dt;
        if (urtoEffettoS_ > 0.0f) urtoEffettoS_ = std::max(0.0f, urtoEffettoS_ - dt);
        if (dallUrtoS_ >= 0.0f) dallUrtoS_ = dallUrtoS_ + dt > 5.0f ? -1.0f : dallUrtoS_ + dt;
        if (avvisoCiglioS_ > 0.0f) avvisoCiglioS_ = std::max(0.0f, avvisoCiglioS_ - dt);
        // Il tocco del ciglio si dimentica solo guidando in strada.
        if (ciglioMemoriaS_ > 0.0f && !player_.offroad) ciglioMemoriaS_ = std::max(0.0f, ciglioMemoriaS_ - dt);

        if (player_.offroad && assistenzaBivio_ <= 0.0f) ++offroadFrames_;
        else offroadFrames_ = 0;

        if (gara_) {
            aggiornaTraffico(dt);
            aggiornaRivali();
            aggiungiSpriteTraffico();
        } else if (trafficoAttivo_) spawnTraffic();
        // RIV (Andrea 27/10): separare il contatto con gli avversari (sprites_) da
        // quello con i cartelloni. Urto con un avversario = bump: spinta laterale
        // leggera + rallentamento (~ -15%), nessun ribaltamento, controlli sempre
        // attivi. Contatto con un cartellone = nessun ribaltamento (ostacolo
        // decorativo a bordo strada). Il ribaltamento resta solo per fuori strada
        // prolungato.
        const bool toccaRivale = assistenzaBivio_ <= 0.0f && collisioneRivaleVicino();
        const bool toccaCartellone = assistenzaBivio_ <= 0.0f && collisioneCartelloneVicino();
        const bool toccaQualcosa = toccaRivale || toccaCartellone;

        if (toccaRivale && !inCollision_ && !playerFlipping_ && !playerBump_ && !arrivo_) {
            // Contatto di fianco (o rimasto dopo un tamponamento): urto con
            // l'auto piu' vicina, avversario o traffico.
            const f32 giro = (f32)road_.totalLength();
            f32 dxMin = 1e9f;
            i32 fonte = 0;
            bool trovata = false;
            for (size_t k = 0; k < sprites_.size() && k < spritesFonte_.size(); ++k) {
                f32 dz = std::fmod(sprites_[k].worldZ - road_.playerZ, giro);
                if (dz < 0.0f) dz += giro;
                const f32 dx = sprites_[k].worldX - player_.x;
                if (dz > Game::URTO_Z_MIN && dz < Game::URTO_Z_MAX && std::abs(dx) < URTO_LARGHEZZA &&
                    std::abs(dx) < std::abs(dxMin)) {
                    dxMin = dx;
                    fonte = spritesFonte_[k];
                    trovata = true;
                }
            }
            if (trovata) urto(fonte, dxMin, false);
            inCollision_ = true;
        } else if (!toccaQualcosa) {
            inCollision_ = false;
        }

        // Durante il bump: spinta laterale che decade, controlli sempre attivi
        // (l'auto non si ferma e puo' continuare a sterzare/accelerare).
        if (playerBump_) {
            playerBumpTimer_ -= dt;
            const f32 t = std::clamp(playerBumpTimer_ / bumpDurataS_, 0.0f, 1.0f);
            player_.x += playerBumpSpintaX_ * t * dt;
            if (playerBumpTimer_ <= 0.0f) {
                playerBump_ = false;
                playerBumpSpintaX_ = 0.0f;
            }
        }

        // Fianco a fianco (auto tra URTO_Z_MIN e il punto di tamponamento):
        // non si sterza dentro la sua carrozzeria, si resta appoggiati di lato.
        if (!playerFlipping_) {
            const f32 giro = (f32)road_.totalLength();
            for (const auto& s : sprites_) {
                f32 dz = std::fmod(s.worldZ - road_.playerZ, giro);
                if (dz < 0.0f) dz += giro;
                const f32 dx = s.worldX - player_.x;
                if (dz > Game::URTO_Z_MIN && dz < URTO_Z_DAVANTI - 50.0f && std::abs(dx) < URTO_LARGHEZZA)
                    player_.x = s.worldX + (dx > 0.0f ? -URTO_LARGHEZZA : URTO_LARGHEZZA);
            }
        }

        // Ciglio (Andrea 29/10): uscire di strada e' un tocco del ciglio. Il
        // primo rallenta e resta in memoria; il seguente, se si va ancora forte,
        // e' un incidente. Al bivio l'aiuto tiene l'auto: nessun tocco.
        const bool entraFuori = player_.offroad && !offroadPrec_;
        offroadPrec_ = player_.offroad;
        if (entraFuori && assistenzaBivio_ <= 0.0f && !playerFlipping_ && !arrivo_ &&
            player_.speed >= CIGLIO_V_MIN * player_.maxSpeed())
            toccaCiglio();

        // Ribaltamento per fuori strada prolungato (mai per contatto con
        // rivali o cartelloni).
        if (offroadFrames_ >= (u32)(OFFROAD_CRASH_S * (f32)frequenzaHz_) && !playerFlipping_ && !playerBump_ && !arrivo_)
            ribalta();
    }

    // 5) Personaggi/gameplay (blocco D): fatti del frame -> stato show.
    //    Il risultato e' consultabile via showDati()/showAttivo()/livelloEscalation()/ufoDati()
    //    da renderer, HUD e moduli fx; il modulo gameplay non dipende da SDL né da orologi.
    StatoGameplayInput gi;
    gi.frame = clock_.frame;
    gi.dt = dt;            // secondi dal frame precedente (1/frequenzaHz_)
    tempoGiocoS_ += dt;    // contatore secondi di gioco (frequenza indipendente)
    gi.stage = 0;          // prototipo: stage 0 (prTab[0]); multi-stage in blocco D/K
    gi.inGara = true;      // no menu/demo in questo prototipo
    gi.inputAttivo = (input_.held != 0ull);
    gi.offRoad = player_.offroad;
    showManager_.aggiorna(gi); // risultato letto da showDati() / disegnoShow_
    disegnoShow_.aggiorna(tempoGiocoS_, showManager_); // stesso tempo (secondi)!

    // 6) Audio sintetico del motore (rpm proporzionato alla velocita').
    f32 rpm = player_.speed / player_.maxSpeed() * 8000.0f;
    mixer_.mix(audioBuf_.data(), (i32)audioBuf_.size() / 2, rpm);
    visualizzatore_.aggiorna(mixer_.info(), dt);

    // Replay: registra gli input se attivo.
    if (replay_.active()) replay_.record(input_.pressed | input_.held);
}

void Game::aggiornaMenu(f32 dt) {
    // Fase senza guida (menu/splash/risultati): show e orologio, nessun input di guida.
    tempoGiocoS_ += dt;
    StatoGameplayInput gi;
    gi.frame = clock_.frame;
    gi.dt = dt;
    gi.stage = 0;
    gi.inGara = false;
    gi.inputAttivo = (input_.held != 0ull);
    gi.offRoad = false;
    showManager_.aggiorna(gi);
    disegnoShow_.aggiorna(tempoGiocoS_, showManager_);
    // Menu e risultati: solo la radio (motore spento).
    mixer_.mix(audioBuf_.data(), (i32)audioBuf_.size() / 2, 0.0f);
}

void Game::render() {
    if (!scenaStrada()) {
        // Schermate di menu/risultati: niente strada, disegna la fase corrente.
        fb_.clear(pal_[7]);
        fasi_.disegna(ren_, fb_);
        return;
    }
    fb_.clear(pal_[7]); // cielo
    ren_.drawRoad(road_, player_.x, road_.playerZ, latoBivio_);
    // Merge traffico + cartelloni in un buffer riutilizzato (zero alloc a regime)
    // e disegna tutto in ordine di distanza (dietro prima).
    // Cartelloni e oggetti sono fissi e ordinati per worldZ: si prendono solo
    // quelli nel tratto visibile (ricerca binaria) invece di copiarli e
    // ordinarli tutti a ogni frame (migliaia di sprite, conta sul Pi 400).
    scenaSprites_ = sprites_;
    aggiungiVisibili(cartelloni_);
    aggiungiVisibili(oggettiStrada_);
    // Pubblico all'arrivo: compare quando il traguardo entra nella vista.
    // Ognuno esulta col suo ritmo, piu' forte dopo che il giocatore e' arrivato.
    const f32 vista = (f32)((Road::DRAW_DIST + 2) * Road::SEG_LEN);
    if (!folla_.empty() && (arrivo_ || road_.playerZ > (f32)road_.totalLength() - vista - FOLLA_PRIMA)) {
        const f32 t = (f32)clock_.frame / (f32)frequenzaHz_;
        for (size_t k = 0; k < folla_.size(); ++k) {
            const f32 ritmo = (arrivo_ ? 3.0f : 1.4f) + 0.5f * (f32)(k % 3);
            const bool su = std::fmod(t * ritmo + 0.37f * (f32)k, 1.0f) < 0.5f;
            folla_[k].sprite = &spriteFolla_[((k / 2 + k) % 2) * 2 + (su ? 1 : 0)];
        }
        aggiungiVisibili(folla_);
    }
    ren_.drawSprites(scenaSprites_, road_.playerZ, (f32)road_.totalLength());
    const bool steeringLeft = !arrivo_ && (input_.held & Pad::Left) != 0;
    const bool steeringRight = !arrivo_ && (input_.held & Pad::Right) != 0;
    const f32 steeringLean = steeringLeft == steeringRight
        ? 0.0f : (steeringLeft ? -0.32f : 0.32f);
    const f32 rollAngle = playerFlipping_
        ? 6.2831853f * (f32)playerFlipFrame_ / (f32)(u32)(FLIP_ANIMATION_S * (f32)frequenzaHz_)
        : steeringLean * 1.5f +
          (player_.offroad ? (player_.x < 0.0f ? -0.18f : 0.18f) : 0.0f);
    const f32 ondeggio = 0.85f + 0.15f * std::sin(sollevamentoFaseS_ * 5.0f);
    const f32 saltoPx = std::min(SALTO_MAX_PX, saltoAltezza_ * SALTO_PX_PER_UNITA);
    // Urto: sobbalzo e scodata (piu' forti col traffico), poi la scena trema.
    f32 urtoK = 0.0f, sobbalzoPx = 0.0f, scodata = 0.0f;
    if (urtoEffettoS_ > 0.0f) {
        urtoK = urtoEffettoS_ / urtoEffettoDurataS_;
        sobbalzoPx = (urtoTraffico_ ? 9.0f : 5.0f) * std::sin(3.14159f * (1.0f - urtoK));
        scodata = (urtoTraffico_ ? 0.35f : 0.18f) * urtoK * std::sin(urtoEffettoS_ * 45.0f);
    }
    // Arrivo: frenata con il muso che si abbassa e una scodata, poi un
    // rimbalzo quando l'auto si ferma.
    bool frena = false;
    if (arrivo_) {
        const f32 k = velocitaArrivo_ > 0.0f ? 1.0f - player_.speed / velocitaArrivo_ : 1.0f;
        frena = fermoS_ < 0.4f;
        if (fermoS_ < 0.0f) {
            scodata += 0.22f * std::sin(3.14159f * k);
            sobbalzoPx -= 2.0f;
        } else if (fermoS_ < 0.8f) {
            sobbalzoPx += 3.0f * std::exp(-fermoS_ * 6.0f) * std::sin(fermoS_ * 22.0f);
        }
    }
    const f32 sollevamentoPx = playerFlipping_ ? 0.0f : sollevamento_ * ondeggio * SOLLEVAMENTO_MAX_PX + saltoPx + sobbalzoPx;
    DanniAuto danni;
    danni.segni = std::min(urtiGara_ + toccateCiglio_, DanniAuto::DANNI_MAX);
    danni.dallUrtoS = dallUrtoS_;
    danni.lato = latoUrto_;
    danni.fumo = urtiGara_ + toccateCiglio_ >= DANNI_FUMO && !playerFlipping_;
    danni.tempoS = tempoGiocoS_;
    ren_.drawPlayerCar(player_.speed, player_.x - player_.centroCorsia, player_.offroad, rollAngle + scodata, sollevamentoPx,
                       frena, arrivo_ ? arrivoS_ : -1.0f, &danni);
    if (avvisoCiglioS_ > 0.0f && std::fmod(avvisoCiglioS_, 0.3f) > 0.1f) {
        // Avviso dopo il primo tocco del ciglio: il prossimo puo' essere un incidente.
        const char* testo = "CIGLIO!";
        const i32 x = 160 + (i32)((player_.x - player_.centroCorsia) * 0.035f) - Renderer::larghezzaTesto(testo, 1) / 2;
        ren_.drawTesto(x, 164 - (i32)sollevamentoPx, testo, 0xFFFFD040u, 1, true);
    }
    if (urtoK > 0.0f) {
        const i32 dx = (i32)((urtoTraffico_ ? 4.0f : 2.0f) * urtoK * std::sin(urtoEffettoS_ * 70.0f));
        const i32 dy = urtoTraffico_ ? (i32)(2.0f * urtoK * std::cos(urtoEffettoS_ * 55.0f)) : 0;
        scuotiScena(dx, dy);
    }
    // Show (UFO/personaggio/fumetto): disegno placeholder con rettangoli colorati.
    // Gli sprite veri verranno aggiunti quando @asset avra' creato gli sprite originali.
    const gameplay::ScenaShow& sc = disegnoShow_.scena();
    if (sc.ufoVisibile) {
        fb_.fillRect(sc.ufoX, sc.ufoY, sc.ufoX + 64, sc.ufoY + 24, 0xFFFF0000u);
    }
    if (sc.personaggioVisibile) {
        fb_.fillRect(sc.persX, sc.persY, sc.persX + sc.persW, sc.persY + sc.persH, 0xFF00FF00u);
    }
    if (sc.bollaVisibile) {
        fb_.fillRect(sc.bollaX, sc.bollaY, sc.bollaX + sc.bollaW, sc.bollaY + sc.bollaH, 0xFF0000FFu);
    }
    // Risultati dell'arrivo: sopra la scena solo la classifica (fasi.cpp).
    if (fasi_.attuale() == FaseId::Risultati) {
        fasi_.disegna(ren_, fb_);
        return;
    }
    // Pannello di info (tappa 1): velocita, marcia, fuoristrada.
    if (ren_.haHud()) {
        HudDati h;
        h.kmh = (i32)(player_.speed / player_.maxSpeed() * VELOCITA_MAX_KMH + 0.5f);
        h.km = km_;
        h.totaleS = (f32)frameGara_ / (f32)frequenzaHz_;
        h.tempoExtra = tempoExtraS_ > 0.0f ? tempoLimiteTappaS_ : 0;
        // Contagiri del cambio automatico a due marce: sale in LOW, riparte piu' alto in HIGH.
        const f32 v = player_.speed / player_.maxSpeed();
        constexpr f32 CAMBIO = 110.0f / 250.0f;
        h.giri = player_.gear <= 1 ? v / CAMBIO : 0.45f + 0.55f * (v - CAMBIO) / (1.0f - CAMBIO);
        h.marciaAlta = player_.gear >= 2;
        h.gara = gara_;
        h.tempoS = (i32)(tempoResiduoS_ + 0.5f);
        h.tappa = numeroTappa_;
        h.posizione = state_.posizione;
        h.concorrenti = state_.concorrenti;
        h.show = showManager_.showAttivo();
        h.fuoristrada = player_.offroad;
        h.bivio = (road_.bivioAttivo() && latoBivio_ == 0 && player_.z > road_.bivioInizioZ() - 30000.0f) ? 1 : 0;
        h.arrivo = arrivo_;
        h.tempoScaduto = tempoScaduto_;
        h.frame = clock_.frame;
        ren_.drawHudDati(h);
    } else {
        ren_.drawHud(player_.speed / player_.maxSpeed(), player_.gear, player_.offroad);
        if (gara_) ren_.drawGara(state_.posizione, state_.concorrenti, numeroTappa_, arrivo_, clock_.frame);
    }
    drawMusica();
    if (fasi_.attuale() == FaseId::FineTempo) fasi_.disegna(ren_, fb_); // blocco R2: "TEMPO SCADUTO"
}

void Game::aggiornaSalto(f32 dt) {
    // Velocita' verticale della strada sotto l'auto (unita'/s): in cima a un
    // dosso cala di colpo. A terra l'auto la segue; in volo segue la gravita'.
    const f32 quota = road_.quotaAt(road_.playerZ);
    const f32 vStrada = (quota - saltoQuotaPrec_) / dt;
    const bool primoPasso = saltoQuotaPrec_ < -1e8f;
    saltoQuotaPrec_ = quota;
    // Tappa nuova o giro del tracciato: la quota salta, non e' un dosso.
    if (primoPasso || std::abs(vStrada) > SALTO_V_DISCONTINUITA) {
        saltoVStradaPrec_ = 0.0f;
        return;
    }
    const bool puoVolare = !player_.offroad && !playerFlipping_ && !playerBump_ &&
        player_.speed >= SALTO_SOGLIA_V * player_.maxSpeed();
    if (saltoAltezza_ <= 0.0f) {
        // Cresta: la strada "cade" piu' in fretta della gravita' -> decollo.
        if (puoVolare && (vStrada - saltoVStradaPrec_) / dt < -SALTO_GRAVITA) {
            saltoVY_ = saltoVStradaPrec_;
            saltoAltezza_ = 0.001f;
        }
    } else {
        saltoVY_ -= SALTO_GRAVITA * dt;
        saltoAltezza_ += (saltoVY_ - vStrada) * dt;
        if (saltoAltezza_ <= 0.0f || playerFlipping_) saltoAltezza_ = 0.0f; // atterraggio
    }
    saltoVStradaPrec_ = vStrada;
}

void Game::caricaSpriteFolla() {
    static const char* NOMI[4] = {"pubblico_a_giu", "pubblico_a_su", "pubblico_b_giu", "pubblico_b_su"};
    for (i32 i = 0; i < 4; ++i)
        spriteFolla_[(size_t)i] = caricaSpriteRaw(percorsoDati(std::string("motore_nuovo/dati/sprite_raw/pubblico/") + NOMI[i] + ".sglraw"));
    if (spriteFolla_[0].width == 0) std::printf("arrivo: raw del pubblico mancanti (png_a_raw.py)\n");
}

void Game::preparaArrivo() {
    // Tappa d'arrivo (senza bivio): linea a scacchi dove il tracciato finisce
    // e pubblico ai due lati, prima e dopo la linea.
    folla_.clear();
    const bool arrivoQui = gara_ && !tappaConBivio(tappaId_);
    ren_.setLineaArrivo(arrivoQui);
    if (!arrivoQui) return;
    if (spriteFolla_[0].width == 0) caricaSpriteFolla();
    if (spriteFolla_[0].width == 0) return;
    const f32 giro = (f32)road_.totalLength();
    const f32 metaGruppo = 0.5f * (f32)spriteFolla_[0].width * Renderer::SPRITE_WORLD_PX * FOLLA_SCALA;
    auto fila = [&](f32 da, f32 a) {
        for (f32 z = da; z < a; z += FOLLA_PASSO) {
            const f32 bordo = Road::ROAD_HALF_WORLD * road_.larghezzaAt(z) + 150.0f + metaGruppo;
            for (f32 lato : {-1.0f, 1.0f}) {
                SpriteInstance s;
                s.worldZ = z;
                s.worldX = lato * bordo;
                s.scale = FOLLA_SCALA;
                s.sprite = &spriteFolla_[0];
                folla_.push_back(s);
            }
        }
    };
    fila(std::max(0.0f, giro - FOLLA_PRIMA), giro);
    fila(0.0f, std::min(FOLLA_DOPO, giro - FOLLA_PRIMA));
    std::sort(folla_.begin(), folla_.end(),
              [](const SpriteInstance& a, const SpriteInstance& b) { return a.worldZ < b.worldZ; });
}

void Game::frenataArrivo(f32 dt) {
    // Dopo il traguardo l'auto frena da sola fino a fermarsi (stop accesi,
    // fumo e scodata in render()); da ferma conta i secondi per i risultati.
    arrivoS_ += dt;
    player_.speed = std::max(0.0f, player_.speed - player_.maxSpeed() / FRENATA_ARRIVO_S * dt);
    if (player_.speed <= 0.0f) fermoS_ = fermoS_ < 0.0f ? 0.0f : fermoS_ + dt;
}

f32 Game::passoFonte(i32 fonte) const {
    if (fonte >= 0) return fonte < RIVALI ? rivali_[fonte].passo : 0.0f;
    const i32 k = -1 - fonte;
    return k < TRAFFICO ? traffico_[k].passo : 0.0f;
}

void Game::urto(i32 fonte, f32 dx, bool tamponamento) {
    // Due urti diversi (Andrea 29/10). Traffico (guidatori qualunque): botta
    // forte, l'auto rimbalza di lato, sobbalza e il veicolo suona il clacson.
    // Rivale (pilota): sportellata, si perde poco e l'altro viene spostato.
    const bool traffico = fonte < 0;
    const f32 xRel = player_.x - player_.centroCorsia;
    const f32 lato = dx > 1.0f ? -1.0f : (dx < -1.0f ? 1.0f : (xRel > 0.0f ? -1.0f : 1.0f));
    playerBump_ = true;
    playerBumpTimer_ = bumpDurataS_;
    playerBumpSpintaX_ = lato * (traffico ? URTO_SPINTA_TRAFFICO : URTO_SPINTA_RIVALE);
    const f32 scala = ROAD_SCROLL_SCALE * (1.0f + ROAD_SCROLL_SPEED_BOOST * player_.speed / player_.maxSpeed());
    const f32 vAuto = passoFonte(fonte) / scala; // velocita' dell'altra auto in unita' del giocatore
    if (tamponamento)
        player_.speed = std::min(player_.speed, vAuto * (traffico ? URTO_TRAFFICO_DOPO : URTO_RIVALE_DOPO));
    else
        player_.speed *= traffico ? 0.85f : bumpRallentaFattore_;
    player_.speed = std::max(player_.speed, 0.1f * player_.maxSpeed()); // mai fermi del tutto
    if (traffico) {
        Veicolo& v = traffico_[-1 - fonte];
        v.x = std::clamp(v.x - lato * 260.0f, -RIVALE_X_MAX * 2.5f, RIVALE_X_MAX * 2.5f);
    } else {
        Rivale& r = rivali_[fonte];
        r.x = std::clamp(r.x - lato * 160.0f, -RIVALE_X_MAX, RIVALE_X_MAX);
        r.xObiettivo = std::clamp(r.x - lato * 200.0f, -RIVALE_X_MAX, RIVALE_X_MAX);
        r.passo *= tamponamento ? 1.03f : 0.92f; // spinto avanti, o sbandata di fianco
    }
    ++urtiGara_;
    segnaBotta(tamponamento ? 0 : -(i32)lato); // lato e' la spinta: la botta arriva dall'altra parte
    urtoTraffico_ = traffico;
    urtoEffettoDurataS_ = traffico ? URTO_EFFETTO_TRAFFICO_S : URTO_EFFETTO_RIVALE_S;
    urtoEffettoS_ = urtoEffettoDurataS_;
    mixer_.suonaUrto(traffico);
}

void Game::segnaBotta(i32 lato) {
    // Ogni botta lascia un segno sull'auto (fino a DanniAuto::DANNI_MAX) e fa
    // partire scintille e lampo dal lato colpito.
    dallUrtoS_ = 0.0f;
    latoUrto_ = lato;
}

void Game::toccaCiglio() {
    ++toccateCiglio_;
    const i32 lato = player_.x - player_.centroCorsia > 0.0f ? 1 : -1;
    segnaBotta(lato);
    if (ciglioMemoriaS_ > 0.0f && player_.speed >= ciglioCrashVelocita_ * player_.maxSpeed()) {
        // Secondo tocco mentre si ricorda il primo, e si va ancora forte: incidente.
        ciglioMemoriaS_ = 0.0f;
        avvisoCiglioS_ = 0.0f;
        ribalta();
        return;
    }
    // Primo tocco (o abbastanza piano): botta secca, si perde velocita' e il
    // gioco avvisa che il prossimo tocco puo' costare caro.
    player_.speed *= ciglioRallenta_;
    urtoTraffico_ = false;
    urtoEffettoDurataS_ = URTO_EFFETTO_RIVALE_S;
    urtoEffettoS_ = urtoEffettoDurataS_;
    mixer_.suonaUrto(false);
    ciglioMemoriaS_ = ciglioMemoriaDurataS_;
    avvisoCiglioS_ = CIGLIO_AVVISO_S;
}

void Game::ribalta() {
    showManager_.crash();
    playerFlipping_ = true;
    playerFlipFrame_ = 0;
    playerXIncidente_ = player_.x;
    player_.speed = 0.0f;
    offroadFrames_ = 0;
}

void Game::calcolaClassifica() {
    // Tempo di ognuno sul traguardo. Il giocatore ha il suo cronometro; per i
    // rivali ancora in corsa (o gia' arrivati) si stima dal distacco e dal loro ritmo medio.
    const f32 tempo = tempoTotaleS();
    const f32 traguardo = progressoTappePrecedenti_ + (f32)road_.totalLength();
    classifica_.clear();
    classifica_.push_back({"TU", tempo, true});
    for (i32 i = 0; i < RIVALI; ++i) {
        const Rivale& r = rivali_[i];
        const f32 media = std::max(1.0f, r.passoMax * RIVALE_MEDIA_PUNTA);
        classifica_.push_back({NOMI_RIVALI[i], std::max(1.0f, tempo + (traguardo - r.progresso) / media), false});
    }
    std::stable_sort(classifica_.begin(), classifica_.end(),
                     [](const Classificato& a, const Classificato& b) { return a.tempoS < b.tempoS; });
    for (size_t i = 0; i < classifica_.size(); ++i)
        if (classifica_[i].giocatore) posizioneArrivo_ = (i32)i + 1;
}

void Game::drawMusica() {
    const InfoMusica& m = mixer_.info();
    if (!m.attiva) return;
    visualizzatore_.disegna(fb_, 8, Framebuffer::HEIGHT - 10); // spettro in basso a sinistra
    // Battito: due tacche ai lati dello schermo che si accendono sul primo tempo.
    if (m.battito > 0.05f) {
        const i32 h = (i32)(m.battito * 40.0f);
        const u32 col = 0xFF000000u | ((u32)(80 + 175 * m.battito) << 16) | ((u32)(40 + 60 * m.battito) << 8) | 0xC0u;
        fb_.fillRect(0, 112 - h, 2, 112 + h, col);
        fb_.fillRect(Framebuffer::WIDTH - 3, 112 - h, Framebuffer::WIDTH - 1, 112 + h, col);
    }
}

void Game::scuotiScena(i32 dx, i32 dy) {
    // Sposta l'immagine di qualche pixel (scossa dell'urto); l'HUD si disegna dopo, fermo.
    if (dx == 0 && dy == 0) return;
    const i32 W = Framebuffer::WIDTH, H = Framebuffer::HEIGHT;
    u32* d = fb_.data();
    std::vector<u32>& copia = scossaBuf_;
    copia.assign(d, d + W * H);
    for (i32 y = 0; y < H; ++y) {
        const i32 ys = std::clamp(y - dy, 0, H - 1);
        for (i32 x = 0; x < W; ++x) d[y * W + x] = copia[ys * W + std::clamp(x - dx, 0, W - 1)];
    }
}

void Game::ordinaSpriteFissi() {
    auto perZ = [](const SpriteInstance& a, const SpriteInstance& b) { return a.worldZ < b.worldZ; };
    std::stable_sort(cartelloni_.begin(), cartelloni_.end(), perZ);
    std::stable_sort(oggettiStrada_.begin(), oggettiStrada_.end(), perZ);
    scenaSprites_.reserve(RIVALI + cartelloni_.size() + oggettiStrada_.size()); // zero alloc per frame
}

void Game::aggiungiVisibili(const std::vector<SpriteInstance>& fissi) {
    // fissi e' ordinato per worldZ (ordinaSpriteFissi); finestra visibile
    // [playerZ, playerZ + DRAW_DIST segmenti], con il giro che ricomincia da 0.
    const f32 giro = (f32)road_.totalLength();
    if (fissi.empty() || giro <= 0.0f) return;
    const f32 vista = (f32)((Road::DRAW_DIST + 2) * Road::SEG_LEN);
    auto da = [&fissi](f32 z) {
        return std::lower_bound(fissi.begin(), fissi.end(), z,
                                [](const SpriteInstance& s, f32 v) { return s.worldZ < v; });
    };
    const f32 z0 = road_.playerZ;
    const f32 z1 = z0 + vista;
    scenaSprites_.insert(scenaSprites_.end(), da(z0), z1 <= giro ? da(z1) : fissi.end());
    if (z1 > giro) scenaSprites_.insert(scenaSprites_.end(), fissi.begin(), da(z1 - giro));
}

bool Game::collisioneOggettiVicini() const {
    const f32 giro = (f32)road_.totalLength();
    if (giro <= 0.0f) return false;

    auto collide = [this, giro](const SpriteInstance& oggetto, f32 profondita, f32 larghezza) {
        f32 dz = std::fmod(oggetto.worldZ - road_.playerZ, giro);
        if (dz < 0.0f) dz += giro;
        const f32 dx = oggetto.worldX - player_.x;
        return dz < profondita && std::abs(dx) < larghezza;
    };

    for (const SpriteInstance& oggetto : sprites_)
        if (collide(oggetto, 120.0f, 180.0f)) return true;

    for (const SpriteInstance& oggetto : cartelloni_)
        if (collide(oggetto, 90.0f, 760.0f)) return true;

    return false;
}

bool Game::collisioneRivaleVicino() const {
    const f32 giro = (f32)road_.totalLength();
    if (giro <= 0.0f) return false;
    // sprites_ contiene gli avversari e il traffico (fuori gara, il traffico sintetico).
    // Urto: un'auto dove a schermo c'e' la carrozzeria del giocatore (URTO_Z_MIN..MAX).
    for (const SpriteInstance& oggetto : sprites_) {
        f32 dz = std::fmod(oggetto.worldZ - road_.playerZ, giro);
        if (dz < 0.0f) dz += giro;
        if (dz > URTO_Z_MIN && dz < URTO_Z_MAX && std::abs(oggetto.worldX - player_.x) < URTO_LARGHEZZA) return true;
    }
    return false;
}

bool Game::collisioneCartelloneVicino() const {
    const f32 giro = (f32)road_.totalLength();
    if (giro <= 0.0f) return false;
    auto collide = [this, giro](const SpriteInstance& oggetto, f32 profondita, f32 larghezza) {
        f32 dz = std::fmod(oggetto.worldZ - road_.playerZ, giro);
        if (dz < 0.0f) dz += giro;
        const f32 dx = oggetto.worldX - player_.x;
        return dz < profondita && std::abs(dx) < larghezza;
    };
    for (const SpriteInstance& oggetto : cartelloni_)
        if (collide(oggetto, 90.0f, 760.0f)) return true;
    return false;
}

void Game::spawnTraffic() {
    // Traffico sintetico: auto senza asset, disegnate come sagome dal renderer.
    sprites_.clear();
    spritesFonte_.clear();
    const f32 giro = (f32)road_.totalLength(); // lunghezza reale del tracciato (prima 3000 fisso)
    const f32 zGiro = giro > 0.0f ? std::fmod(road_.playerZ, giro) : 0.0f;
    for (i32 i = 1; i <= 6; ++i) {
        SpriteInstance s;
        s.worldZ = (f32)(i * 500) + zGiro;
        s.worldX = (i % 2 == 0) ? -500.0f : 500.0f;
        s.scale = 1.0f;
        s.sprite = nullptr; // nessun sprite carico ancora: non disegna nulla
        s.paletteBase = 9;
        sprites_.push_back(s);
    }
}

u32 Game::frameHash() const {
    // Hash semplice del framebuffer + stato: identico su esecuzioni con stessi input.
    u32 h = 0x811c9dc5u; // FNV offset
    const u32* d = fb_.data();
    i32 n = Framebuffer::WIDTH * Framebuffer::HEIGHT;
    for (i32 i = 0; i < n; ++i) {
        h ^= d[i];
        h *= 0x01000193u;
    }
    h ^= state_.frame;
    h ^= (u32)state_.playerSpeed;
    // Stato posizionale e RNG: due esecuzioni che divergono in posizione
    // o offroad devono dare hash diverso (determinismo/replay, regola 5b).
    h ^= (u32)state_.playerX;
    h ^= (u32)state_.playerZ;
    h ^= state_.playerOffroad ? 1u : 0u;
    h ^= state_.playerFlipping ? 0x80000000u : 0u;
    h ^= state_.playerFlipFrame * 0x9e3779b9u;
    h ^= rng_.state(); // stato RNG (xorshift32): se diverge, l'hash diverge
    h *= 0x01000193u;
    return h;
}

} // namespace sgl
