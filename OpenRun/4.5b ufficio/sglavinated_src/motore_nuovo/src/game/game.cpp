// SPDX-License-Identifier: MIT
#include "game/game.h"
#include "core/json_mini.h" // lettura livello.json (campo cartelloni_sponsor)
#include <cmath> // std::fmod
#include <cstdio> // std::fopen/fread/fclose
#include <utility> // std::move

namespace sgl {
namespace {
constexpr f32 ROAD_SCROLL_SCALE = 72.0f;
constexpr f32 ROAD_SCROLL_SPEED_BOOST = 0.65f;
constexpr f32 CURVA_SPINTA = 0.015f; // forza centrifuga in curva (vedi update)

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

void Game::init(const std::string& livelloId) {
    // Tracciato da dati: legge il campo "segmenti" (array di {curva, collina,
    // lunghezza}) da motore_nuovo/dati/livelli/<livelloId>/livello.json.
    // Se il campo manca o il file e' illeggibile, usa il fallback deterministico
    // hardcoded sotto. Stesso seme = stesso tracciato (determinismo).
    // livello.json letto e analizzato una volta sola: ne usano i dati
    // sia i segmenti sia i cartelloni (prima il file veniva letto due volte).
    JsonReader livello(leggiFileTesto(percorsoDati("motore_nuovo/dati/livelli/" + livelloId + "/livello.json")));
    const JsonValue* radiceLivello = livello.parse() ? &livello.valore() : nullptr;

    std::vector<Road::SegmentDesc> descs;
    const bool daFile = leggiSegmentiDaFile(radiceLivello, descs) && !descs.empty();
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
        if (!leggiCartelloniDaFile(radiceLivello)) {
            // Campo assente o non valido -> fallback casualita' deterministica.
            Rng rndCartelloni(seed_);
            const f32 latoSx = -1600.0f; // bordo strada sinistro (unita' mondo)
            const f32 latoDx = 1600.0f;  // bordo strada destro
            i32 k = 0;
            const i32 nSeg = (i32)road_.segments().size();
            for (i32 i = 0; i < nSeg; ++i) {
                if (rndCartelloni.next(4) != 0) continue; // ~1 segmento su 4 con cartellone
                SpriteInstance b;
                b.worldZ = (f32)(i * Road::SEG_LEN);
                b.worldX = (k & 1) ? latoDx : latoSx; // lato alternato: distribuzione bilanciata
                b.scale = 1.0f;
                b.sprite = ren_.sponsorSprite(sponsorSpriteIdx_[rndCartelloni.next((u32)sponsorSpriteIdx_.size())]);
                b.paletteBase = 0;
                cartelloni_.push_back(b);
                ++k;
            }
        }
    }
    scenaSprites_.reserve(sprites_.capacity() + cartelloni_.size()); // zero alloc per frame a regime

    road_.playerZ = 0.0f;
    road_.playerX = 0.0f;
    player_.z = 0.0f;
    player_.x = 0.0f;
    player_.speed = 0.0f;
    playerFlipping_ = false;
    playerFlipFrame_ = 0;
    offroadFrames_ = 0;
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
}

void Game::setInput(const Pad& pad) {
    input_ = pad;
}

i32 Game::risolviSponsorSprite(const std::string& sponsorId) const {
    if (sponsorId.empty()) return -1;
    for (size_t i = 0; i < sponsorId_.size(); ++i) {
        if (sponsorId_[i] == sponsorId) return (i32)i;
    }
    return -1; // sconosciuto: il chiamante sceglie a caso (seeded RNG)
}

void Game::setSponsorSprites(std::vector<std::pair<std::string, Sprite>> sprites) {
    sponsorSpriteIdx_.clear();
    sponsorId_.clear();
    sponsorSpriteIdx_.reserve(sprites.size());
    sponsorId_.reserve(sprites.size());
    for (auto& p : sprites) {
        sponsorId_.push_back(std::move(p.first));
        sponsorSpriteIdx_.push_back(ren_.aggiungiSponsorSprite(std::move(p.second)));
    }
    // Possesso esclusivo del Renderer; puntatori stabili dopo questa chiamata.
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

bool Game::leggiCartelloniDaFile(const JsonValue* radice) {
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
    f32 dt = 1.0f / 50.0f; // un frame di gioco a 50 Hz

    if (playerFlipping_) {
        ++playerFlipFrame_;
        if (playerFlipFrame_ >= FLIP_ANIMATION_FRAMES) {
            playerFlipping_ = false;
            playerFlipFrame_ = 0;
            offroadFrames_ = 0;
            player_.x = 0.0f;
            player_.speed = 0.0f;
            player_.offroad = false;
            road_.playerX = 0.0f;
        }
    } else {
        player_.input(input_, dt);
        player_.update(dt);

        road_.playerX = player_.x;
        const f32 roadScrollScale = ROAD_SCROLL_SCALE *
            (1.0f + ROAD_SCROLL_SPEED_BOOST * player_.speed / player_.maxSpeed());
        road_.advance(player_.speed * dt * roadScrollScale);
        player_.z = road_.playerZ;

        // Le curve spingono l'auto verso l'esterno, con intensita' crescente alla velocita'.
        // Le curve ora durano centinaia di metri: la spinta resta sotto lo sterzo
        // (a curva 0.8 e velocita' massima circa 7 unita'/frame contro 11 di sterzo),
        // cosi' la curva si tiene sterzando.
        const f32 curvaQui = road_.curvaAt(player_.z);
        player_.x += curvaQui * player_.speed * CURVA_SPINTA * dt * roadScrollScale;
        if (player_.x > 900.0f) player_.x = 900.0f;
        if (player_.x < -900.0f) player_.x = -900.0f;
        player_.offroad = player_.x < -700.0f || player_.x > 700.0f;

        if (player_.offroad) ++offroadFrames_;
        else offroadFrames_ = 0;

        if (trafficoAttivo_) spawnTraffic();
        const bool collisione = collisioneOggettiVicini();
        if (collisione && !inCollision_) {
            showManager_.crash();
            inCollision_ = true;
        } else if (!collisione) {
            inCollision_ = false;
        }

        if (collisione || offroadFrames_ >= OFFROAD_CRASH_FRAMES) {
            if (!collisione) showManager_.crash();
            playerFlipping_ = true;
            playerFlipFrame_ = 0;
            player_.speed = 0.0f;
            offroadFrames_ = 0;
        }
    }

    // 5) Personaggi/gameplay (blocco D): fatti del frame -> stato show.
    //    Il risultato e' consultabile via showDati()/showAttivo()/livelloEscalation()/ufoDati()
    //    da renderer, HUD e moduli fx; il modulo gameplay non dipende da SDL né da orologi.
    StatoGameplayInput gi;
    gi.frame = clock_.frame;
    gi.stage = 0;          // prototipo: stage 0 (prTab[0]); multi-stage in blocco D/K
    gi.inGara = true;      // no menu/demo in questo prototipo
    gi.inputAttivo = (input_.held != 0ull);
    gi.offRoad = player_.offroad;
    showManager_.aggiorna(gi); // risultato letto da showDati() / disegnoShow_
    disegnoShow_.aggiorna(clock_.frame, showManager_); // stesso frame!

    // 6) Audio sintetico del motore (rpm proporzionato alla velocita').
    f32 rpm = player_.speed / player_.maxSpeed() * 8000.0f;
    mixer_.mix(audioBuf_.data(), AUDIO_FRAME_SAMPLES, rpm);

    // Replay: registra gli input se attivo.
    if (replay_.active()) replay_.record(input_.pressed | input_.held);

    // Aggiorna stato esportato.
    state_.frame = clock_.frame;
    state_.playerSpeed = player_.speed;
    state_.playerX = player_.x;
    state_.playerZ = player_.z;
    state_.playerOffroad = player_.offroad;
    state_.playerFlipping = playerFlipping_;
    state_.playerFlipFrame = playerFlipFrame_;

    clock_.tick();
}

void Game::render() {
    fb_.clear(pal_[7]); // cielo
    ren_.drawRoad(road_, player_.x, road_.playerZ);
    // Merge traffico + cartelloni in un buffer riutilizzato (zero alloc a regime)
    // e disegna tutto in ordine di distanza (dietro prima).
    scenaSprites_ = sprites_;
    scenaSprites_.insert(scenaSprites_.end(), cartelloni_.begin(), cartelloni_.end());
    ren_.drawSprites(scenaSprites_, road_.playerZ, (f32)road_.totalLength());
    const bool steeringLeft = (input_.held & Pad::Left) != 0;
    const bool steeringRight = (input_.held & Pad::Right) != 0;
    const f32 steeringLean = steeringLeft == steeringRight
        ? 0.0f : (steeringLeft ? -0.32f : 0.32f);
    const f32 rollAngle = playerFlipping_
        ? 6.2831853f * (f32)playerFlipFrame_ / (f32)FLIP_ANIMATION_FRAMES
        : steeringLean * 1.5f +
          (player_.offroad ? (player_.x < 0.0f ? -0.18f : 0.18f) : 0.0f);
    ren_.drawPlayerCar(player_.speed, player_.x, player_.offroad, rollAngle);
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
    ren_.drawHud(player_.speed / player_.maxSpeed(), player_.gear, player_.offroad);
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
