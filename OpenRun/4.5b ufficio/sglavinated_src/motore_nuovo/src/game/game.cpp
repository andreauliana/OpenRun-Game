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
constexpr f32 RIVALE_CURVA_SCALE = 0.5f;     // rallentamento in curva: scala |curva| del segmento
constexpr f32 RIVALE_CURVA_SLOW_MAX = 0.50f; // max rallentamento in curva estrema (50%)
constexpr f32 RIVALE_DISTANZA_MIN = 1500.0f; // distanza minima di seguito tra due rivali
constexpr f32 RIVALE_LATERALE_MIN = 250.0f;  // soglia laterale per intervenire sull'altro rivale
constexpr f32 RIVALE_SPOSTAMENTO = 200.0f;   // spostamento laterale per sorpassare di lato
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
constexpr f32 ASSIST_RICHIAMO_MAX = 250.0f; // unita'/s, sotto lo sterzo (>= 510 in alto)
constexpr f32 VELOCITA_MAX_KMH = 293.0f; // velocita' massima mostrata (come l'originale)
// Unita' di strada in un km: a 293 km/h l'auto percorre ~590 unita' al secondo.
// speed e' gia' in unita'/secondo, quindi questa costante e' indipendente dalla frequenza.
constexpr f32 UNITA_PER_KM = 590.0f * 50.0f * 3600.0f / 293.0f;

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
        JsonValue cv, hv, lv, ev, uv;
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
        if (o) {
            JsonValue cv, hv, lv, ev, uv;
            if (jsonGet(*o, "curva", cv) && jsonIsNumber(cv)) d.curva = (f32)jsonAsNumber(cv);
            if (jsonGet(*o, "collina", hv) && jsonIsNumber(hv)) d.collina = (f32)jsonAsNumber(hv);
            if (jsonGet(*o, "lunghezza", lv) && jsonIsNumber(lv)) {
                i32 l = (i32)jsonAsNumber(lv);
                if (l > 0) d.lunghezza = l;
            }
            if (jsonGet(*o, "entrata", ev) && jsonIsNumber(ev)) {
                i32 e = (i32)jsonAsNumber(ev);
                if (e >= 0) d.entrata = e;
            }
            if (jsonGet(*o, "uscita", uv) && jsonIsNumber(uv)) {
                i32 u = (i32)jsonAsNumber(uv);
                if (u >= 0) d.uscita = u;
            }
        }
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

        // 2) DECO1 2d / D2b: densita' totale cartelloni ~20-30% dei segmenti
        //    (target ~25%, valore scelto: 1 segmento su 4). I segmenti senza
        //    cartellone esplicito vengono riempiti in modo deterministico
        //    (seed di tappa), senza sovrapporre le posizioni esplicite del file.
        //    Tutte le allocazioni avvengono qui (init): zero alloc per frame.
        i32 k = (i32)cartelloni_.size();
        for (i32 i = 0; i < nSeg; ++i) {
            if (segmentoOccupato[(size_t)i]) continue;
            if (rndCartelloni.next(4) != 0) continue; // ~1 segmento su 4 con cartellone
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
    // Una istanza ogni passoZ unita', lato alternato, bordo strada (+/-1600).
    // Nessun oggetto negli ultimi BIVIO_SEG segmenti: quando il bivio e' attivo
    // i due rami occupano la carreggiata e un oggetto a +/-1600 finirebbe in
    // mezzo alla strada (DECO1 2f punto 1).
    const f32 passoZ = 2000.0f; // DECO1 2f punto 3: una istanza ogni 2000 unita' (prima 4000)
    const f32 zBivioInizio = road_.bivioInizioZ(); // inizio ultimi BIVIO_SEG segmenti
    // Scala degli oggetti: campo altezza_mondo/scala per oggetto nel manifest
    // della variante; senza campo si usa 4.0 (lampione ~2x l'auto, visibile).
    const f32 scalaOggetto = 4.0f; // DECO1 2f punto 2
    i32 k = 0;
    for (f32 z = passoZ; z < giro - passoZ; z += passoZ) {
        if (zBivioInizio > 0.0f && z >= zBivioInizio) break; // nessun oggetto nell'area del bivio
        const i32 j = k % nOggetti;
        const i32 idx = idxPerId[j];
        SpriteInstance b;
        b.worldZ = z;
        b.worldX = (k & 1) ? 1600.0f : -1600.0f;
        b.scale = scalaOggetto;
        b.sprite = (idx >= 0) ? ren_.oggettoSprite(idx) : nullptr;
        b.paletteBase = 0;
        oggettiStrada_.push_back(b);
        ++k;
    }
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
    assistenzaBivio_ = 0.0f;
    playerFlipFrame_ = 0;
    offroadFrames_ = 0;
    playerBump_ = false;
    playerBumpTimer_ = 0.0f;
    playerBumpSpintaX_ = 0.0f;
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

    // Radio: nessun path hardcodato su shots/ (cartella del filone Mega Drive
    // v87: regola 1; e fuori dal flusso audio guidato
    // da dati della regola 8, motore_nuovo/dati/audio/playlist.json).
    // Il runner carica il brano da playlist.json e lo passa al mixer; qui si
    // lascia radioM senza path -> loadRadio() restituisce 0 (silenzio, nessun crash).
    RadioDef radioM;
    radioM.banksPerTrack = 5;
    radioM.sampleRate = 12922;
    i32 n = mixer_.loadRadio(radioM);
    if (n > 0) mixer_.selezionaBrano(0);
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
        rgb(o, "colore", e.colore);
        rgb(o, "colore_label", e.coloreLabel);
        rgb(o, "colore_bordo", e.coloreBordo);
        rgb(o, "colore_riempimento", e.coloreRiempimento);
        if (e.tipo == "cornice" || e.tipo == "testo" || e.tipo == "barra") elementi.push_back(std::move(e));
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

bool Game::avviaGara(const std::string& fileTappe) {
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

    init(partenza);
    gara_ = true;
    tappaId_ = partenza;
    numeroTappa_ = 1;
    progressoTappePrecedenti_ = 0.0f;
    frameGara_ = 0;
    arrivo_ = false;
    tempoResiduoS_ = (f32)tempoLimiteTappaS_; // tempo della prima tappa (secondi)
    preparaBivio();
    inizializzaRivali();
    aggiornaStatoGara();
    fasi_.cambia(FaseId::Gara, *this); // blocco R2: avviaGara() parte guidando (gate, runner --subito)
    return true;
}

const Game::Tappa* Game::trovaTappa(const std::string& id) const {
    for (const Tappa& t : tappe_)
        if (t.id == id) return &t;
    return nullptr;
}

void Game::preparaBivio() {
    // Bivio solo se la tappa ha due rami; il cartello sta nello spartitraffico
    // poco prima del punto in cui la scelta si blocca.
    const Tappa* t = trovaTappa(tappaId_);
    const bool rami = t && !t->sinistra.empty() && !t->destra.empty();
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
    scenaSprites_.reserve(RIVALI + cartelloni_.size());
}

void Game::inizializzaRivali() {
    // Griglia di partenza: gli avversari partono davanti, in due file, e hanno
    // ognuno una velocita' di punta un po' sotto quella del giocatore.
    Rng rnd(seed_ ^ 0x52495641u);
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
        r.spriteIdx = nSpriteRivali_ > 0 ? i % nSpriteRivali_ : -1; // sprite rivale (RIV punto b)
    }
    rndRivali_ = Rng(seed_ ^ 0x4C414E45u);
    sprites_.clear();
    sprites_.reserve(RIVALI);
}

void Game::aggiornaRivali() {
    const f32 giro = (f32)road_.totalLength();
    const f32 dt = 1.0f / (f32)frequenzaHz_;
    const f32 avvicina = 1.0f - std::exp(-dt / RIVALE_ACCEL_TAU_S);
    const f32 velocitaGiocatore = passoGiocatore_ / dt; // unita' al secondo
    sprites_.clear();
    for (i32 i = 0; i < RIVALI; ++i) {
        Rivale& r = rivali_[i];
        const f32 zLocale = r.progresso - progressoTappePrecedenti_;
        // Rallenta in curva in base alla curvatura del segmento (piu' curva = piu' lento),
        // attenuato dalla bravura: i rivali piu' bravi soffrono meno la curva.
        const f32 curvaQui = std::abs(road_.curvaAt(zLocale));
        const f32 lentaCurva = 1.0f - std::clamp(curvaQui * RIVALE_CURVA_SCALE * r.bravura,
                                                  0.0f, RIVALE_CURVA_SLOW_MAX);
        // Elastico come nei giochi arcade: chi e' avanti rallenta, chi e' dietro
        // spinge, in proporzione al distacco. Cosi' anche dopo un incidente la gara resta aperta.
        const f32 distacco = r.progresso - (progressoTappePrecedenti_ + player_.z);
        const f32 elastico = std::clamp(1.0f - 0.25f * distacco / RIVALE_ELASTICO_DISTANZA,
                                        RIVALE_ELASTICO_MIN, RIVALE_ELASTICO_MAX);
        r.passo += (r.passoMax * elastico * lentaCurva - r.passo) * avvicina;
        // Chi arriva da dietro non tampona il giocatore: cambia corsia e, finche'
        // e' allineato, resta alla sua velocita' (sorpasso/evita, mai cambi casuali di corsia).
        const f32 dxGiocatore = r.x - (player_.x - player_.centroCorsia);
        if (distacco < 0.0f && distacco > -3000.0f && std::abs(dxGiocatore) < 400.0f) {
            r.xObiettivo = (player_.x - player_.centroCorsia) > 0.0f ? -450.0f : 450.0f;
            if (distacco > -600.0f) r.passo = std::min(r.passo, velocitaGiocatore);
        }
        // Distanza minima tra rivali: chi raggiunge un altro rivale rallenta e,
        // se molto vicino, si sposta di lato per superarlo (corsia libera).
        for (i32 j = 0; j < RIVALI; ++j) {
            if (j == i) continue;
            const Rivale& q = rivali_[j];
            const f32 dz = r.progresso - q.progresso;
            if (dz > 0.0f && dz < RIVALE_DISTANZA_MIN && std::abs(r.x - q.x) < RIVALE_LATERALE_MIN) {
                r.passo = std::min(r.passo, q.passo * 0.95f);
                if (dz < RIVALE_DISTANZA_MIN * 0.5f) {
                    r.xObiettivo = r.x > q.x
                        ? std::min(r.xObiettivo + RIVALE_SPOSTAMENTO, 650.0f)
                        : std::max(r.xObiettivo - RIVALE_SPOSTAMENTO, -650.0f);
                }
            }
        }
        r.progresso += r.passo * dt;
        const f32 laterale = RIVALE_LATERALE_PER_S * dt;
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
        s.sprite = r.spriteIdx >= 0 ? ren_.rivaleSprite(r.spriteIdx) : nullptr; // RIV punto b: sprite vero se disponibile
        s.paletteBase = r.colore;
        sprites_.push_back(s);
    }
}

void Game::fineTappa(f32 avanzo) {
    // Fine del tracciato in gara: il ramo scelto porta alla tappa seguente;
    // una tappa senza rami e' l'arrivo.
    const Tappa* t = trovaTappa(tappaId_);
    const std::string prossima = !t ? std::string() : (latoBivio_ < 0 ? t->sinistra : t->destra);
    if (prossima.empty() || !trovaTappa(prossima)) {
        if (!arrivo_) {
            arrivo_ = true;
            posizioneArrivo_ = posizioneGiocatore();
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
    progressoTappePrecedenti_ += (f32)road_.totalLength();
    caricaLivello(prossima); // imposta subito il tema e lo skyline della tappa nuova (impostaTema)
    // Transizione graduale verso il tema di arrivo (FORK1 punto 6): i colori di
    // ambiente e lo skyline si interpolano in transizioneTemaS_ secondi, niente scatto visivo.
    ren_.iniziaTransizioneTema(temaPrima, skylinePrima, transizioneTemaS_);
    tappaId_ = prossima;
    ++numeroTappa_;
    if (!tempoScaduto_) tempoResiduoS_ += (f32)tempoLimiteTappaS_; // checkpoint: tempo extra (secondi)
    preparaBivio();
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
    if (fasi_.attuale() == FaseId::Gara || fasi_.attuale() == FaseId::FineTempo) {
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
        if (gara_) aggiornaRivali(); // gli avversari non aspettano
    } else {
        // Corsia dell'auto: al bivio e' il ramo piu' vicino.
        const Road::Carreggiate carr = road_.carreggiate(player_.z, latoBivio_);
        if (latoBivio_ == 0 && !carr.doppia) {
            player_.centroCorsia = 0.0f;
            player_.larghezzaCorsia = 1.0f;
        } else if (latoBivio_ == 0) {
            // Scelta ancora aperta: corsia = ramo piu' vicino, cosi' l'auto
            // segue il ramo mentre si apre fino a BIVIO_DIST. Con il centro
            // fisso a 0 il clamp X_LIMIT (900) la teneva sullo spartitraffico,
            // fuori strada, e al blocco scattava di 100 unita' (gate
            // bivio_uscita, 25/10). Il clamp agisce ogni frame mentre il ramo
            // si apre, quindi la spinta verso il proprio lato resta continua.
            const i32 vicino = std::abs(player_.x - carr.centro[0]) <= std::abs(player_.x - carr.centro[1]) ? 0 : 1;
            player_.centroCorsia = carr.centro[vicino];
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

        road_.playerX = player_.x;
        const f32 roadScrollScale = ROAD_SCROLL_SCALE *
            (1.0f + ROAD_SCROLL_SPEED_BOOST * player_.speed / player_.maxSpeed());
        const f32 zPrima = road_.playerZ;
        passoGiocatore_ = player_.speed * dt * roadScrollScale;
        km_ += passoGiocatore_ / UNITA_PER_KM;
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
            if (c.doppia && latoBivio_ == 0) centro = player_.x < 0.0f ? c.centro[0] : c.centro[1];
            f32 richiamo = (centro - player_.x) * (1.0f - std::exp(-dt / ASSIST_RICHIAMO_TAU_S));
            const f32 maxPasso = ASSIST_RICHIAMO_MAX * dt;
            richiamo = std::clamp(richiamo, -maxPasso, maxPasso);
            player_.x += richiamo * assistenzaBivio_;
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

        if (player_.offroad && assistenzaBivio_ <= 0.0f) ++offroadFrames_;
        else offroadFrames_ = 0;

        if (gara_) aggiornaRivali();
        else if (trafficoAttivo_) spawnTraffic();
        // RIV (Andrea 27/10): separare il contatto con gli avversari (sprites_) da
        // quello con i cartelloni. Urto con un avversario = bump: spinta laterale
        // leggera + rallentamento (~ -15%), nessun ribaltamento, controlli sempre
        // attivi. Contatto con un cartellone = nessun ribaltamento (ostacolo
        // decorativo a bordo strada). Il ribaltamento resta solo per fuori strada
        // prolungato.
        const bool toccaRivale = assistenzaBivio_ <= 0.0f && collisioneRivaleVicino();
        const bool toccaCartellone = assistenzaBivio_ <= 0.0f && collisioneCartelloneVicino();
        const bool toccaQualcosa = toccaRivale || toccaCartellone;

        if (toccaRivale && !inCollision_ && !playerFlipping_ && !playerBump_) {
            // Avvia il bump: spinta laterale che allontana dal rivale urtato +
            // rallentamento leggero. I controlli restano attivi (auto non bloccata).
            playerBump_ = true;
            playerBumpTimer_ = bumpDurataS_;
            playerBumpSpintaX_ = 0.0f;
            const f32 giro = (f32)road_.totalLength();
            f32 dxMin = 1e9f;
            i32 rivaleUrtato = -1;
            // Trova l'avversario piu' vicino in collisione (stessa soglia di
            // collisioneRivaleVicino): e' l'urttato e subisce il rallentamento.
            for (i32 k = 0; k < RIVALI; ++k) {
                const Rivale& rk = rivali_[k];
                const f32 zLocale = rk.progresso - progressoTappePrecedenti_;
                if (zLocale <= 0.0f || zLocale > 60000.0f || zLocale >= giro + 60000.0f) continue;
                f32 dz = std::fmod(zLocale - road_.playerZ, giro);
                if (dz < 0.0f) dz += giro;
                if (dz >= 120.0f) continue;
                const Road::Carreggiate c = road_.carreggiate(zLocale, latoBivio_);
                const i32 indice = latoBivio_ == 0 ? (rk.ramo < 0 ? 0 : 1) : (rk.ramo == latoBivio_ ? 0 : 1);
                const f32 dx = c.centro[indice] + rk.x * c.fattore - player_.x;
                if (std::abs(dx) >= 180.0f) continue;
                if (std::abs(dx) < std::abs(dxMin)) { dxMin = dx; rivaleUrtato = k; }
            }
            if (std::abs(dxMin) < 1e8f) {
                playerBumpSpintaX_ = (dxMin >= 0.0f ? -1.0f : 1.0f) * bumpSpintaLaterale_;
                if (rivaleUrtato >= 0) rivali_[rivaleUrtato].passo *= 0.85f; // urtato perde velocita'
            }
            player_.speed *= bumpRallentaFattore_;
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

        // Ribaltamento solo per fuori strada prolungato (mai per contatto con
        // rivali o cartelloni).
        if (offroadFrames_ >= (u32)(OFFROAD_CRASH_S * (f32)frequenzaHz_) && !playerFlipping_ && !playerBump_) {
            showManager_.crash();
            playerFlipping_ = true;
            playerFlipFrame_ = 0;
            playerXIncidente_ = player_.x;
            player_.speed = 0.0f;
            offroadFrames_ = 0;
        }
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
    mixer_.mix(audioBuf_.data(), AUDIO_FRAME_SAMPLES, rpm);

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
    // Silenzio in menu: velocita' zero.
    std::fill(audioBuf_.begin(), audioBuf_.end(), (i16)0);
}

void Game::render() {
    if (fasi_.attuale() != FaseId::Gara && fasi_.attuale() != FaseId::FineTempo) {
        // Schermate di menu/risultati: niente strada, disegna la fase corrente.
        fb_.clear(pal_[7]);
        fasi_.disegna(ren_, fb_);
        return;
    }
    fb_.clear(pal_[7]); // cielo
    ren_.drawRoad(road_, player_.x, road_.playerZ, latoBivio_);
    // Merge traffico + cartelloni in un buffer riutilizzato (zero alloc a regime)
    // e disegna tutto in ordine di distanza (dietro prima).
    scenaSprites_ = sprites_;
    scenaSprites_.insert(scenaSprites_.end(), cartelloni_.begin(), cartelloni_.end());
    scenaSprites_.insert(scenaSprites_.end(), oggettiStrada_.begin(), oggettiStrada_.end());
    ren_.drawSprites(scenaSprites_, road_.playerZ, (f32)road_.totalLength());
    const bool steeringLeft = (input_.held & Pad::Left) != 0;
    const bool steeringRight = (input_.held & Pad::Right) != 0;
    const f32 steeringLean = steeringLeft == steeringRight
        ? 0.0f : (steeringLeft ? -0.32f : 0.32f);
    const f32 rollAngle = playerFlipping_
        ? 6.2831853f * (f32)playerFlipFrame_ / (f32)(u32)(FLIP_ANIMATION_S * (f32)frequenzaHz_)
        : steeringLean * 1.5f +
          (player_.offroad ? (player_.x < 0.0f ? -0.18f : 0.18f) : 0.0f);
    const f32 ondeggio = 0.85f + 0.15f * std::sin(sollevamentoFaseS_ * 5.0f);
    const f32 sollevamentoPx = playerFlipping_ ? 0.0f : sollevamento_ * ondeggio * SOLLEVAMENTO_MAX_PX;
    ren_.drawPlayerCar(player_.speed, player_.x - player_.centroCorsia, player_.offroad, rollAngle, sollevamentoPx);
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
    // Pannello di info (tappa 1): velocita, marcia, fuoristrada.
    if (ren_.haHud()) {
        HudDati h;
        h.kmh = (i32)(player_.speed / player_.maxSpeed() * VELOCITA_MAX_KMH + 0.5f);
        h.km = km_;
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
    if (fasi_.attuale() == FaseId::FineTempo) fasi_.disegna(ren_, fb_); // blocco R2: "TEMPO SCADUTO"
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
    auto collide = [this, giro](const SpriteInstance& oggetto, f32 profondita, f32 larghezza) {
        f32 dz = std::fmod(oggetto.worldZ - road_.playerZ, giro);
        if (dz < 0.0f) dz += giro;
        const f32 dx = oggetto.worldX - player_.x;
        return dz < profondita && std::abs(dx) < larghezza;
    };
    // sprites_ contiene gli avversari (e, fuori gara, il traffico sintetico).
    // Un contatto ravvicinato (dentro 120 unita' e 180 di larghezza) e' un urto.
    for (const SpriteInstance& oggetto : sprites_)
        if (collide(oggetto, 120.0f, 180.0f)) return true;
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
