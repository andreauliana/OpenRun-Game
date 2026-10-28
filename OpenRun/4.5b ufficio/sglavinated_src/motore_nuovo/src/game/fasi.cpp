// SPDX-License-Identifier: MIT
#include "game/fasi.h"
#include "game/game.h"
#include "core/json_mini.h"
#include "render/renderer.h"
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>

namespace sgl {

namespace {
// Durate in secondi (frequenza indipendente, decisione Andrea 19 ott).
constexpr f32 SPLASH_DURATA_S = 2.0f;   // logo, poi menu da solo o con un tasto
constexpr f32 FINE_TEMPO_DURATA_S = 4.0f; // "TEMPO SCADUTO" visibile prima dei risultati
constexpr f32 FINALE_DURATA_S = 5.0f;   // scena di arrivo prima dei risultati (blocco FIN)

// Score: distanza + bonus tempo residuo + bonus checkpoint (dati/punteggio.json).
constexpr i32 PUNTI_PER_KM_DEFAULT = 10;
constexpr i32 BONUS_TEMPO_RESIDUO_DEFAULT = 2;
constexpr i32 BONUS_CHECKPOINT_DEFAULT = 300;

// Legge un file dati (vuoto se manca): solo all'entrata di una fase, mai nel ciclo di gioco.
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

i32 jsonNumero(const JsonObject& o, const char* chiave, i32 def) {
    JsonValue v;
    return jsonGet(o, chiave, v) && jsonIsNumber(v) ? (i32)jsonAsNumber(v) : def;
}
} // namespace

// ---------------------------------------------------------------------------
// Fase: Splash (logo e caricamento, poi menu)
// ---------------------------------------------------------------------------

class SplashFase : public Fase {
public:
    SplashFase() : timerS_(0.0f), finita_(false) {}
    void entra(Game&) override { timerS_ = 0.0f; finita_ = false; }
    void aggiorna(Game& gioco, f32 dt) override {
        timerS_ += dt;
        const Pad& p = gioco.input();
        if (p.pressed != 0ull || timerS_ >= SPLASH_DURATA_S) finita_ = true;
    }
    void disegna(Renderer& ren, Framebuffer& fb) const override {
        // Placeholder del titolo: @grafico consegna lo schermo di splash (D15/D16).
        const char* titolo = "SGLAVINATED EDITION";
        const i32 w = ren.larghezzaTesto(titolo, 2);
        fb.fillRect(160 - w / 2 - 8, 76, 160 + w / 2 + 8, 116, 0xFF6060A0u);
        ren.drawTesto(160 - w / 2, 90, titolo, 0xFFFFFFFFu, 2, true);
        const char* premi = "PREMI START";
        ren.drawTesto(160 - ren.larghezzaTesto(premi, 1) / 2, 136, premi, 0xFFA0A0A0u, 1, true);
    }
    void esci(Game&) override {}
    FaseId prossima() const override { return FaseId::Menu; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "Splash"; }
private:
    f32 timerS_;
    bool finita_;
};

// ---------------------------------------------------------------------------
// Fase: Menu (scelta corsa singola / gara a tappe, conferma Start)
// ---------------------------------------------------------------------------

class MenuFase : public Fase {
public:
    MenuFase() : sel_(0), finita_(false), prossima_(FaseId::Gara) {}
    void entra(Game&) override { sel_ = 0; finita_ = false; prossima_ = FaseId::Gara; }
    void aggiorna(Game& gioco, f32 dt) override {
        (void)dt;
        const Pad& p = gioco.input();
        if (p.pressed & Pad::Up)   sel_ = (sel_ + 2) % 3;
        if (p.pressed & Pad::Down) sel_ = (sel_ + 1) % 3;
        if (p.pressed & (Pad::Start | Pad::A)) {
            switch (sel_) {
                case 0: // corsa singola: prima tappa della gara (stessi tracciati, rivali e tempo), poi arrivo
                    if (!gioco.avviaGara("motore_nuovo/dati/percorso_gara.json", true)) gioco.init("lv_01_neon");
                    break;
                case 1: // gara a tappe con bivi (avviaGara -> Gara); senza percorso: corsa singola
                    if (!gioco.avviaGara()) gioco.init("lv_01_neon");
                    break;
                case 2: gioco.richiediUscita(); prossima_ = FaseId::Menu; break; // il runner chiude
            }
            finita_ = true;
        }
    }
    void disegna(Renderer& ren, Framebuffer&) const override {
        const char* titolo = "MENU";
        const i32 w = ren.larghezzaTesto(titolo, 2);
        ren.drawTesto(160 - w / 2, 56, titolo, 0xFFFFFFFFu, 2, true);
        static const char* voci[3] = { "CORSA SINGOLA", "GARA A TAPPE", "ESCI" };
        for (i32 i = 0; i < 3; ++i) {
            i32 wi = ren.larghezzaTesto(voci[i], 1);
            u32 colore = (i == sel_) ? 0xFFFFFFFFu : 0xFFA0A0A0u;
            ren.drawTesto(160 - wi / 2, 104 + i * 14, voci[i], colore, 1, true);
            if (i == sel_) ren.drawTesto(160 - wi / 2 - 12, 104 + i * 14, ">", 0xFFFFFFFFu, 1, true);
        }
        const char* aiuto = "SU/GIU SCEGLI   START CONFERMA";
        ren.drawTesto(160 - ren.larghezzaTesto(aiuto, 1) / 2, 160, aiuto, 0xFFA0A0A0u, 1, true);
    }
    void esci(Game&) override {}
    FaseId prossima() const override { return prossima_; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "Menu"; }
private:
    i32 sel_;
    bool finita_;
    FaseId prossima_;
};

// ---------------------------------------------------------------------------
// Fase: Gara (guida delegata a Game; FineTempo gestito da Game su tempo scaduto/arrivo)
// ---------------------------------------------------------------------------

class GaraFase : public Fase {
public:
    GaraFase() : finita_(false) {}
    void entra(Game&) override { finita_ = false; } // Game ha gi avviato la corsa
    void aggiorna(Game&, f32) override {}            // la guida e' in Game::aggiornaGuida
    void disegna(Renderer&, Framebuffer&) const override {} // Game disegna la strada
    void esci(Game&) override {}
    FaseId prossima() const override { return FaseId::FineTempo; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "Gara"; }
private:
    bool finita_;
};

// ---------------------------------------------------------------------------
// Fase: FineTempo ("TEMPO SCADUTO", poi risultati)
// ---------------------------------------------------------------------------

class FineTempoFase : public Fase {
public:
    FineTempoFase() : timerS_(0.0f), finita_(false) {}
    void entra(Game& gioco) override { timerS_ = 0.0f; finita_ = false; arrivo_ = gioco.arrivato(); }
    void aggiorna(Game&, f32 dt) override {
        timerS_ += dt;
        if (timerS_ >= FINE_TEMPO_DURATA_S) finita_ = true;
    }
    void disegna(Renderer& ren, Framebuffer&) const override {
        const char* testo = arrivo_ ? "ARRIVO" : "TEMPO SCADUTO";
        const i32 w = ren.larghezzaTesto(testo, 2);
        ren.drawTesto(160 - w / 2, 100, testo, 0xFFFF0000u, 2, true);
    }
    void esci(Game&) override {}
    // Arrivo: Finale poi Risultati. Tempo scaduto: salta il finale, va diretto
    // a Risultati (compatibile con il gate ciclo, che finisce sempre in tempo scaduto).
    FaseId prossima() const override { return arrivo_ ? FaseId::Finale : FaseId::Risultati; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "FineTempo"; }
private:
    f32 timerS_;
    bool finita_;
    bool arrivo_ = false;
};

// ---------------------------------------------------------------------------
// Fase: Risultati (punteggio + classifica finta)
// ---------------------------------------------------------------------------

struct PunteggioEntry {
    std::string nome;
    i32 punti;
};

class RisultatiFase : public Fase {
public:
    RisultatiFase() : timerS_(0.0f), punteggio_(0), posizione_(0), finita_(false),
                      nomePos_(0) {
        nome_[0] = 'A'; nome_[1] = 'A'; nome_[2] = 'A'; nome_[3] = '\0';
    }
    void entra(Game& gioco) override {
        timerS_ = 0.0f; finita_ = false;
        nomePos_ = 0;
        nome_[0] = 'A'; nome_[1] = 'A'; nome_[2] = 'A'; nome_[3] = '\0';
        // Formula punteggio (dati/punteggio.json, valori default se manca il file).
        i32 puntiPerKm = PUNTI_PER_KM_DEFAULT;
        i32 bonusTempo = BONUS_TEMPO_RESIDUO_DEFAULT;
        i32 bonusCheckpoint = BONUS_CHECKPOINT_DEFAULT;
        JsonReader punteggio(leggiFileTesto("motore_nuovo/dati/punteggio.json"));
        if (punteggio.parse() && jsonIsObject(punteggio.valore())) {
            const JsonObject& o = jsonAsObject(punteggio.valore());
            puntiPerKm = jsonNumero(o, "punti_per_km", PUNTI_PER_KM_DEFAULT);
            bonusTempo = jsonNumero(o, "bonus_tempo_residuo_per_secondo", BONUS_TEMPO_RESIDUO_DEFAULT);
            bonusCheckpoint = jsonNumero(o, "bonus_checkpoint", BONUS_CHECKPOINT_DEFAULT);
        }
        const f32 km = gioco.distanzaKm();
        const i32 tappeCompletate = gioco.numeroTappa() - 1;
        punteggio_ = (i32)(km * puntiPerKm) + (i32)(gioco.tempoResiduoS() * bonusTempo)
            + tappeCompletate * bonusCheckpoint;
        if (punteggio_ < 0) punteggio_ = 0;

        // Classifica: 9 giocatori fittizi + il punteggio del giocatore
        // (dati/classifica_finta.json; default fitti se il file manca).
        classifica_.clear();
        classifica_.push_back({std::string(nome_), punteggio_});
        JsonReader classifica(leggiFileTesto("motore_nuovo/dati/classifica_finta.json"));
        if (classifica.parse() && jsonIsObject(classifica.valore())) {
            JsonValue elencoV;
            if (jsonGet(jsonAsObject(classifica.valore()), "giocatori", elencoV) && jsonIsArray(elencoV)) {
                for (const JsonValue& g : jsonAsArray(elencoV)) {
                    if (!jsonIsObject(g)) continue;
                    const JsonObject& o = jsonAsObject(g);
                    JsonValue nv, pv;
                    std::string nome;
                    i32 punti = 0;
                    if (jsonGet(o, "nome", nv) && jsonIsString(nv)) nome = jsonAsString(nv);
                    if (jsonGet(o, "punti", pv) && jsonIsNumber(pv)) punti = (i32)jsonAsNumber(pv);
                    if (!nome.empty()) classifica_.push_back({std::move(nome), punti});
                }
            }
        }
        if (classifica_.size() == 1) {
            static const char* nomi[9] = { "ALE", "BEN", "SIM", "LUC", "MAR", "GIA", "FIO", "LEO", "ANN" };
            i32 punti[9] = { 9800, 9100, 8500, 7900, 7400, 6900, 6300, 5800, 5200 };
            for (i32 i = 0; i < 9; ++i) classifica_.push_back({nomi[i], punti[i]});
        }
        std::sort(classifica_.begin(), classifica_.end(),
                  [](const PunteggioEntry& a, const PunteggioEntry& b) { return a.punti > b.punti; });
        if (classifica_.size() > 10) classifica_.resize(10);
        posizione_ = 1;
        for (const PunteggioEntry& e : classifica_) {
            if (e.nome == nome_) break;
            if (e.punti >= punteggio_) ++posizione_;
        }
    }
    void aggiorna(Game& gioco, f32 dt) override {
        (void)dt;
        timerS_ += dt;
        const Pad& p = gioco.input();
        // Inserimento nome a 3 lettere: SU/GIU cambia lettera, Left/Right sposta il cursore.
        if (p.pressed & Pad::Up)   { nome_[nomePos_] = (nome_[nomePos_] == 'Z') ? 'A' : char(nome_[nomePos_] + 1); }
        if (p.pressed & Pad::Down) { nome_[nomePos_] = (nome_[nomePos_] == 'A') ? 'Z' : char(nome_[nomePos_] - 1); }
        if (p.pressed & Pad::Right) nomePos_ = (nomePos_ + 1) % 3;
        if (p.pressed & Pad::Left)  nomePos_ = (nomePos_ + 2) % 3;
        // Start conferma il nome e torna al menu; timeout di sicurezza a 30 s.
        if ((p.pressed & Pad::Start) || timerS_ > 30.0f) finita_ = true;
    }
    void disegna(Renderer& ren, Framebuffer&) const override {
        char buf[96];
        const char* titolo = "RISULTATI";
        const i32 w = ren.larghezzaTesto(titolo, 2);
        ren.drawTesto(160 - w / 2, 40, titolo, 0xFFFFFFFFu, 2, true);
        char nomeVis[8];
        std::snprintf(nomeVis, sizeof(nomeVis), "%c%c%c", nome_[0], nome_[1], nome_[2]);
        // Riga nome: cursore visibile sulla lettera in editing.
        char nomeCursore[8];
        std::snprintf(nomeCursore, sizeof(nomeCursore), "%c%c%c", nome_[0], nome_[1], nome_[2]);
        nomeCursore[nomePos_] = '_';
        std::snprintf(buf, sizeof(buf), "NOME: %s   SU/GIU CAMBIA  START OK", nomeCursore);
        ren.drawTesto(64, 72, buf, 0xFFFFFFFFu, 1, true);
        std::snprintf(buf, sizeof(buf), "%s %d PUNTI   POSIZIONE %d", nomeVis, punteggio_, posizione_);
        ren.drawTesto(64, 88, buf, 0xFFA0A0A0u, 1, true);
        i32 y = 110;
        for (size_t i = 0; i < classifica_.size(); ++i) {
            const PunteggioEntry& e = classifica_[i];
            std::snprintf(buf, sizeof(buf), "%c %-3s %6d", (e.nome == nomeVis ? '>' : ' '), e.nome.c_str(), e.punti);
            ren.drawTesto(64, y + (i32)i * 11, buf, e.nome == nomeVis ? 0xFFFFFFFFu : 0xFFA0A0A0u, 1, true);
        }
        const char* aiuto = "START TORNA AL MENU";
        ren.drawTesto(160 - ren.larghezzaTesto(aiuto, 1) / 2, 206, aiuto, 0xFFA0A0A0u, 1, true);
    }
    void esci(Game&) override {}
    FaseId prossima() const override { return FaseId::Menu; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "Risultati"; }
private:
    f32 timerS_;
    i32 punteggio_;
    i32 posizione_;
    bool finita_;
    char nome_[4];       // nome giocatore a 3 lettere (inserito con SU/GIU)
    i32 nomePos_;        // cursore 0..2 sulla lettera in editing
    std::vector<PunteggioEntry> classifica_;
};

namespace {
// Scena del finale scelta da dati/finali.json in base alla pista d'arrivo e alla
// posizione (primo / podio / altro). Voci mancanti o file assente -> segnaposto motore.
std::string scenaFinale(const std::string& pista, i32 posizione) {
    std::string path = "motore_nuovo/dati/finali.json";
    JsonReader r(leggiFileTesto(path));
    if (!r.parse() || !jsonIsObject(r.valore())) return {};
    const JsonObject& o = jsonAsObject(r.valore());
    JsonValue pv;
    if (!jsonGet(o, pista.c_str(), pv) || !jsonIsObject(pv)) return {};
    const JsonObject& po = jsonAsObject(pv);
    const char* chiave = (posizione == 1) ? "primo" : (posizione <= 3 ? "podio" : "altro");
    JsonValue sv;
    if (jsonGet(po, chiave, sv) && jsonIsString(sv)) return jsonAsString(sv);
    return {};
}
} // namespace

// ---------------------------------------------------------------------------
// Fase: Finale (scena di arrivo, poi Risultati) — blocco FIN
// ---------------------------------------------------------------------------

class FinaleFase : public Fase {
public:
    FinaleFase() : timerS_(0.0f), durataS_(FINALE_DURATA_S), finita_(false), posizione_(0) {}
    void entra(Game& gioco) override {
        timerS_ = 0.0f; finita_ = false;
        pista_ = gioco.tappaCorrente();
        posizione_ = gioco.posizioneArrivo();
        scena_ = scenaFinale(pista_, posizione_);
    }
    void aggiorna(Game&, f32 dt) override {
        timerS_ += dt;
        if (timerS_ >= durataS_) finita_ = true;
    }
    void disegna(Renderer& ren, Framebuffer&) const override {
        const char* testo = scena_.empty() ? "FINALE" : scena_.c_str();
        const i32 w = ren.larghezzaTesto(testo, 2);
        ren.drawTesto(160 - w / 2, 100, testo, 0xFFFF0000u, 2, true);
        char buf[96];
        std::snprintf(buf, sizeof(buf), "PISTA: %s   POSIZIONE: %d", pista_.c_str(), posizione_);
        ren.drawTesto(64, 120, buf, 0xFFFFFFFFu, 1, true);
    }
    void esci(Game&) override {}
    FaseId prossima() const override { return FaseId::Risultati; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "Finale"; }
private:
    f32 timerS_;
    f32 durataS_;
    bool finita_;
    std::string pista_;
    std::string scena_;
    i32 posizione_;
};

// ---------------------------------------------------------------------------
// GestoreFasi
// ---------------------------------------------------------------------------

GestoreFasi::GestoreFasi() {
    // L'entrata vera avviene in cambia() con il Game già costruito
    // (chiamato dal costruttore di Game).
    fase_ = crea(attuale_);
}

GestoreFasi::~GestoreFasi() = default;

std::unique_ptr<Fase> GestoreFasi::crea(FaseId id) {
    switch (id) {
        case FaseId::Splash:    return std::make_unique<SplashFase>();
        case FaseId::Menu:      return std::make_unique<MenuFase>();
        case FaseId::Gara:      return std::make_unique<GaraFase>();
        case FaseId::FineTempo: return std::make_unique<FineTempoFase>();
        case FaseId::Finale:    return std::make_unique<FinaleFase>();
        case FaseId::Risultati: return std::make_unique<RisultatiFase>();
    }
    return std::make_unique<MenuFase>();
}

void GestoreFasi::cambia(FaseId id, Game& gioco) {
    if (inAggiorna_) {
        // Il cambio avviene alla fine di aggiorna(): la fase corrente sta ancora girando.
        cambioRimandato_ = true;
        fasePendente_ = id;
        return;
    }
    if (fase_) fase_->esci(gioco);
    fase_ = crea(id);
    attuale_ = id;
    if (fase_) fase_->entra(gioco);
}

void GestoreFasi::aggiorna(Game& gioco, f32 dt) {
    if (!fase_) return;
    inAggiorna_ = true;
    cambioRimandato_ = false;
    fase_->aggiorna(gioco, dt);
    inAggiorna_ = false;
    // Un cambio chiesto durante aggiorna() (es. menu che avvia la gara) vince
    // su prossima(): una sola transizione per tick.
    if (cambioRimandato_) {
        cambioRimandato_ = false;
        cambia(fasePendente_, gioco);
    } else if (fase_->finita()) {
        cambia(fase_->prossima(), gioco);
    }
}

void GestoreFasi::disegna(Renderer& ren, Framebuffer& fb) const {
    if (fase_) fase_->disegna(ren, fb);
}

} // namespace sgl
