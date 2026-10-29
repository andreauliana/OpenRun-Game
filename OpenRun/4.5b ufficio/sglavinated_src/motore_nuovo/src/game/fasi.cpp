// SPDX-License-Identifier: MIT
#include "game/fasi.h"
#include "game/game.h"
#include "core/json_mini.h"
#include "render/renderer.h"
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <array>
#include <cmath>

namespace sgl {

namespace {
// Durate in secondi (frequenza indipendente, decisione Andrea 19 ott).
constexpr f32 SPLASH_DURATA_S = 2.0f;   // logo, poi menu da solo o con un tasto
constexpr f32 FINE_TEMPO_DURATA_S = 4.0f; // "TEMPO SCADUTO" visibile prima dei risultati
constexpr f32 FINALE_DURATA_S = 8.0f;   // scena di arrivo prima dei risultati (blocco FIN); Start la salta dopo 1,5 s

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

u32 rgb(f32 r, f32 g, f32 b) {
    auto c = [](f32 v) { return (u32)std::clamp(v, 0.0f, 255.0f); };
    return 0xFF000000u | (c(r) << 16) | (c(g) << 8) | c(b);
}

void linea(Framebuffer& fb, i32 x0, i32 y0, i32 x1, i32 y1, u32 col) {
    const i32 dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    const i32 dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    i32 err = dx + dy;
    for (;;) {
        fb.put(x0, y0, col);
        if (x0 == x1 && y0 == y1) break;
        const i32 e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// Tempo M'SS"CC (come il TOTALE dell'HUD).
void formattaTempo(char* buf, size_t n, f32 secondi) {
    const i32 cs = (i32)(secondi * 100.0f + 0.5f);
    std::snprintf(buf, n, "%d'%02d\"%02d", cs / 6000, (cs / 100) % 60, cs % 100);
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
        // Arrivo: classifica della gara sul tempo di ognuno (Andrea 29/10).
        arrivo_.clear();
        if (gioco.arrivato())
            for (const Game::Classificato& c : gioco.classificaArrivo()) arrivo_.push_back(c);
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
    void disegna(Renderer& ren, Framebuffer& fb) const override {
        char buf[96];
        if (!arrivo_.empty()) { disegnaArrivo(ren, fb); return; }
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
    // Classifica d'arrivo: posizione, nome e tempo; il giocatore evidenziato,
    // podio in oro, argento e bronzo, distacco dal primo.
    void disegnaArrivo(Renderer& ren, Framebuffer& fb) const {
        const char* titolo = "CLASSIFICA";
        ren.drawTesto(160 - ren.larghezzaTesto(titolo, 2) / 2, 22, titolo, 0xFFFFFFFFu, 2, true);
        ren.drawTesto(64, 48, "POS", 0xFFA0A0A0u, 0, true);
        ren.drawTesto(96, 48, "PILOTA", 0xFFA0A0A0u, 0, true);
        ren.drawTesto(170, 48, "TEMPO", 0xFFA0A0A0u, 0, true);
        ren.drawTesto(226, 48, "DISTACCO", 0xFFA0A0A0u, 0, true);
        static constexpr u32 PODIO[3] = {0xFFFFD040u, 0xFFD8E0F0u, 0xFFE09050u};
        char buf[32], tempo[24];
        for (size_t i = 0; i < arrivo_.size(); ++i) {
            const Game::Classificato& c = arrivo_[i];
            const i32 y = 58 + (i32)i * 14;
            if (c.giocatore) fb.fillRect(56, y - 3, 266, y + 10, 0xFF303060u);
            const u32 col = c.giocatore ? 0xFFFFFFFFu : 0xFFB0B0B0u;
            std::snprintf(buf, sizeof(buf), "%d", (i32)i + 1);
            ren.drawTesto(66, y, buf, i < 3 ? PODIO[i] : col, 1, true);
            ren.drawTesto(96, y, c.nome, col, 1, true);
            formattaTempo(tempo, sizeof(tempo), c.tempoS);
            ren.drawTesto(170, y, tempo, col, 1, true);
            if (i > 0) {
                std::snprintf(buf, sizeof(buf), "+%.2f", c.tempoS - arrivo_[0].tempoS);
                ren.drawTesto(226, y, buf, col, 1, true);
            }
        }
        std::snprintf(buf, sizeof(buf), "PUNTI %d", punteggio_);
        ren.drawTesto(160 - ren.larghezzaTesto(buf, 1) / 2, 178, buf, 0xFFFFE080u, 1, true);
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
    std::vector<Game::Classificato> arrivo_; // classifica sul tempo, solo dopo l'arrivo
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


// Scena d'arrivo (Andrea 29/10): tramonto con sole a righe e griglia al
// neon, l'auto che frena dopo il traguardo, fuochi d'artificio sul battito
// della musica, coriandoli per chi vince. Il titolo cambia con la posizione.
// Tutto disegnato dal motore; scena_ (dati/finali.json) resta l'id per la
// scena grafica del @grafico quando ci sara'.
class FinaleFase : public Fase {
public:
    FinaleFase() : timerS_(0.0f), durataS_(FINALE_DURATA_S), finita_(false), posizione_(0) {}
    void entra(Game& gioco) override {
        timerS_ = 0.0f; finita_ = false;
        pista_ = gioco.tappaCorrente();
        posizione_ = gioco.posizioneArrivo();
        concorrenti_ = gioco.state().concorrenti;
        tempoS_ = gioco.tempoTotaleS();
        scena_ = scenaFinale(pista_, posizione_);
        for (Particella& p : fuochi_) p.vita = 0.0f;
        for (i32 i = 0; i < CORIANDOLI; ++i) {
            coriandoli_[i].x = caso() * 320.0f;
            coriandoli_[i].y = -caso() * 224.0f;
            coriandoli_[i].vy = 25.0f + 30.0f * caso();
            coriandoli_[i].vita = caso() * 6.28f; // fase dell'ondeggio
            coriandoli_[i].col = COLORI[i % 6];
        }
    }
    void aggiorna(Game& gioco, f32 dt) override {
        timerS_ += dt;
        const InfoMusica& m = gioco.musica();
        // Un fuoco sul primo tempo della battuta (o ogni 0,8 s se la musica tace).
        const bool colpo = m.attiva ? m.battito > battitoPrec_ + 0.5f : timerS_ >= prossimoFuocoS_;
        battitoPrec_ = m.battito;
        if (timerS_ > 0.6f && (colpo || timerS_ >= prossimoFuocoS_)) {
            fuoco(40.0f + 240.0f * caso(), 22.0f + 60.0f * caso());
            if (posizione_ == 1) fuoco(40.0f + 240.0f * caso(), 22.0f + 60.0f * caso());
            prossimoFuocoS_ = timerS_ + 0.8f;
        }
        for (Particella& p : fuochi_) {
            if (p.vita <= 0.0f) continue;
            p.vy += 45.0f * dt;
            p.x += p.vx * dt; p.y += p.vy * dt;
            p.vx *= 1.0f - 1.2f * dt;
            p.vita -= dt;
        }
        if (posizione_ == 1)
            for (i32 i = 0; i < CORIANDOLI; ++i) {
                Particella& c = coriandoli_[i];
                c.vita += dt * 3.0f;
                c.y += c.vy * dt;
                c.x += std::sin(c.vita) * 20.0f * dt;
                if (c.y > 224.0f) c.y -= 240.0f;
            }
        const Pad& p = gioco.input();
        if ((timerS_ > 1.5f && (p.pressed & (Pad::Start | Pad::A))) || timerS_ >= durataS_) finita_ = true;
    }
    void disegna(Renderer& ren, Framebuffer& fb) const override {
        constexpr i32 ORIZZONTE = 120;
        // Cielo: dal viola al rosa, arancio vicino all'orizzonte.
        for (i32 y = 0; y < ORIZZONTE; ++y) {
            const f32 t = (f32)y / (f32)ORIZZONTE;
            fb.hline(0, 319, y, rgb(20 + 200 * t * t, 8 + 60 * t * t * t, 40 + 80 * t));
        }
        // Sole a righe: le fessure scendono piano.
        const i32 cx = 160, cy = 108, r = 40;
        for (i32 y = cy - r; y < ORIZZONTE; ++y) {
            const i32 dy = y - cy;
            if (dy * dy > r * r) continue;
            const f32 t = (f32)(y - (cy - r)) / (f32)(2 * r);
            if (t > 0.35f && ((y + (i32)(timerS_ * 12.0f)) % 7) >= 5) continue;
            const i32 w = (i32)std::sqrt((f32)(r * r - dy * dy));
            fb.hline(cx - w, cx + w, y, rgb(255, 230 - 150 * t, 90 + 60 * t));
        }
        // Terreno con griglia al neon che scorre e si ferma con l'auto.
        fb.fillRect(0, ORIZZONTE, 319, 223, rgb(28, 10, 48));
        const f32 frenata = std::clamp(1.0f - timerS_ / 3.0f, 0.0f, 1.0f);
        const f32 scorri = timerS_ * 3.0f - timerS_ * timerS_ * 0.5f * frenata; // rallenta fino a fermarsi
        const f32 fase = std::fmod(std::max(0.0f, timerS_ < 3.0f ? scorri : 4.5f), 1.0f);
        for (i32 k = 0; k < 14; ++k) {
            const f32 z = (f32)k + 1.0f - fase;
            const i32 y = ORIZZONTE + (i32)(110.0f / z);
            if (y >= 224) continue;
            const f32 luce = std::clamp(1.4f / z, 0.15f, 1.0f);
            fb.hline(0, 319, y, rgb(255 * luce, 40 * luce, 200 * luce));
        }
        for (i32 i = -9; i <= 9; ++i)
            linea(fb, cx + i * 6, ORIZZONTE, cx + i * 60, 223, rgb(150, 30, 140));
        fb.hline(0, 319, ORIZZONTE, rgb(255, 120, 200));
        // Linea del traguardo a scacchi che passa sotto l'auto nel primo secondo.
        if (timerS_ < 1.0f) {
            const i32 yT = 150 + (i32)(timerS_ * 90.0f);
            for (i32 x = 0; x < 320; x += 8)
                for (i32 riga = 0; riga < 2; ++riga)
                    fb.fillRect(x, yT + riga * 4, x + 7, yT + riga * 4 + 3,
                                ((x / 8 + riga) % 2) ? 0xFFFFFFFFu : 0xFF000000u);
        }
        ren.drawPlayerCar(250.0f * frenata, // 250 = punta del giocatore
                           0.0f, false, 0.0f, 0.0f);
        // Fuochi e coriandoli.
        for (const Particella& p : fuochi_) {
            if (p.vita <= 0.0f) continue;
            const u32 col = p.vita > 0.35f ? p.col : 0xFF602050u;
            if (p.vita > 0.8f) fb.fillRect((i32)p.x, (i32)p.y, (i32)p.x + 1, (i32)p.y + 1, 0xFFFFFFFFu);
            else fb.put((i32)p.x, (i32)p.y, col);
        }
        if (posizione_ == 1)
            for (i32 i = 0; i < CORIANDOLI; ++i) {
                const Particella& c = coriandoli_[i];
                const bool largo = std::sin(c.vita * 2.0f) > 0.0f; // gira su se stesso
                fb.fillRect((i32)c.x, (i32)c.y, (i32)c.x + (largo ? 2 : 0), (i32)c.y + 1, c.col);
            }
        // Titolo che cade dall'alto e rimbalza.
        const char* titolo = posizione_ == 1 ? "VITTORIA!" : (posizione_ <= 3 ? "PODIO!" : "TRAGUARDO!");
        const f32 t = std::min(1.0f, timerS_ / 0.6f);
        const i32 yTitolo = (i32)(-30.0f + 54.0f * t) + (t < 1.0f ? 0 : (i32)(3.0f * std::exp(-(timerS_ - 0.6f) * 4.0f) * std::sin((timerS_ - 0.6f) * 18.0f)));
        const f32 bagliore = 0.5f + 0.5f * std::sin(timerS_ * 6.0f);
        const u32 coloreTitolo = posizione_ == 1 ? rgb(255, 210 + 45 * bagliore, 60 + 120 * bagliore)
                               : (posizione_ <= 3 ? rgb(200 + 55 * bagliore, 220, 255) : 0xFFFFFFFFu);
        ren.drawTesto(160 - ren.larghezzaTesto(titolo, 3) / 2, yTitolo, titolo, coloreTitolo, 3, true);
        if (timerS_ > 0.7f) {
            char buf[48], tempo[24];
            std::snprintf(buf, sizeof(buf), "POSIZIONE %d/%d", posizione_, concorrenti_);
            ren.drawTesto(160 - ren.larghezzaTesto(buf, 1) / 2, 58, buf, 0xFFFFFFFFu, 1, true);
            formattaTempo(tempo, sizeof(tempo), tempoS_);
            std::snprintf(buf, sizeof(buf), "TEMPO %s", tempo);
            ren.drawTesto(160 - ren.larghezzaTesto(buf, 1) / 2, 70, buf, 0xFFFFE080u, 1, true);
        }
        if (timerS_ > 1.5f && ((i32)(timerS_ * 2.0f) % 2) == 0)
            ren.drawTesto(160 - ren.larghezzaTesto("START", 0) / 2, 90, "START", 0xFFFFFFFFu, 0, true);
    }
    void esci(Game&) override {}
    FaseId prossima() const override { return FaseId::Risultati; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "Finale"; }
private:
    struct Particella { f32 x = 0, y = 0, vx = 0, vy = 0, vita = 0; u32 col = 0xFFFFFFFFu; };
    static constexpr i32 CORIANDOLI = 48;
    static constexpr u32 COLORI[6] = {0xFFFF4060u, 0xFFFFD040u, 0xFF40E0FFu, 0xFF80FF60u, 0xFFFF60E0u, 0xFFFFFFFFu};
    f32 caso() { seme_ = seme_ * 1664525u + 1013904223u; return (f32)(seme_ >> 8) / (f32)(1u << 24); }
    void fuoco(f32 x, f32 y) {
        // Scoppio: 24 scintille a raggiera dello stesso colore.
        const u32 col = COLORI[(i32)(caso() * 5.99f)];
        i32 messe = 0;
        for (Particella& p : fuochi_) {
            if (p.vita > 0.0f) continue;
            const f32 ang = 6.2831853f * (f32)messe / 24.0f + caso() * 0.2f;
            const f32 v = 35.0f + 30.0f * caso();
            p = {x, y, std::cos(ang) * v, std::sin(ang) * v, 1.1f + 0.3f * caso(), col};
            if (++messe == 24) break;
        }
    }
    f32 timerS_;
    f32 durataS_;
    bool finita_;
    std::string pista_;
    std::string scena_;
    i32 posizione_;
    i32 concorrenti_ = 8;
    f32 tempoS_ = 0.0f;
    f32 battitoPrec_ = 0.0f, prossimoFuocoS_ = 0.0f;
    u32 seme_ = 0x5EED1234u;
    std::array<Particella, 120> fuochi_{};
    Particella coriandoli_[CORIANDOLI];
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
