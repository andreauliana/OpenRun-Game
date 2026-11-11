// SPDX-License-Identifier: MIT
#include "game/fasi.h"
#include "game/game.h"
#include "game/salvataggi.h" // blocco ARC1: nome del giocatore salvato in salvataggi/giocatore.json
#include "core/json_mini.h"
#include "render/renderer.h"
#include "render/premiazione.h" // PREMIAZIONE_S: festa a tema prima della classifica
#include "render/framebuffer.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <vector>
#include <utility>

namespace sgl {

namespace {
// Durate in secondi (frequenza indipendente, decisione Andrea 19 ott).
constexpr f32 SPLASH_DURATA_S = 2.0f;   // logo, poi menu da solo o con un tasto
constexpr f32 FINE_TEMPO_DURATA_S = 4.0f; // "TEMPO SCADUTO" visibile prima dei risultati
// Arrivo (Andrea 29/10): l'auto frena e si ferma sulla strada, poi la
// classifica compare sopra la scena. Tetto di sicurezza alla frenata.
// Andrea 07/11: da ferma parte la premiazione a tema (render/premiazione.*);
// la classifica arriva alla fine, o prima con START/A dopo un secondo.
constexpr f32 FERMO_PRIMA_DEI_RISULTATI_S = PREMIAZIONE_S;
constexpr f32 PREMIAZIONE_SALTA_DOPO_S = 1.0f;
constexpr f32 ARRIVO_MAX_S = 10.0f;
constexpr f32 RIGA_CLASSIFICA_S = 0.06f; // le righe della classifica entrano una alla volta (09/11: piu' svelte)
// Consulto finale: i tempi girano prima del verdetto (Andrea 08/11). Andrea
// 09/11: breve, la premiazione non deve aspettare (prima 1.6 s).
constexpr f32 CONSULTO_S = 0.5f;

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

// Vero se il nome (fino a 6 caratteri, spazi = posto libero) e' vuoto.
bool nomeVuoto(const char nome[7]) {
    for (i32 i = 0; i < 6; ++i)
        if (nome[i] != ' ') return false;
    return true;
}

// Nome da visualizzare: caratteri fino al primo spazio.
std::string nomeVisibile(const char nome[7]) {
    std::string out;
    for (i32 i = 0; i < 6; ++i) {
        if (nome[i] == ' ') break;
        out.push_back(nome[i]);
    }
    return out;
}

// Scurisce un rettangolo del framebuffer (pannello sopra la scena di gioco).
void scurisci(Framebuffer& fb, i32 x0, i32 y0, i32 x1, i32 y1, u32 percento) {
    u32* d = fb.data();
    for (i32 y = std::max(0, y0); y <= std::min(Framebuffer::HEIGHT - 1, y1); ++y)
        for (i32 x = std::max(0, x0); x <= std::min(Framebuffer::WIDTH - 1, x1); ++x) {
            const u32 c = d[y * Framebuffer::WIDTH + x];
            d[y * Framebuffer::WIDTH + x] = 0xFF000000u | ((((c >> 16) & 0xFFu) * percento / 100) << 16) |
                                            ((((c >> 8) & 0xFFu) * percento / 100) << 8) | ((c & 0xFFu) * percento / 100);
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
        const char* titolo = "OPEN RUN";
        const i32 w = ren.larghezzaTitolo(titolo, 2);
        fb.fillRect(160 - w / 2 - 8, 76, 160 + w / 2 + 8, 116, 0xFF6060A0u);
        ren.drawTitolo(160 - w / 2, 83, titolo, 0xFFFFFFFFu, 2, true); // font grassetto (Andrea 09/11)
        const char* premi = "PRESS START";
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

namespace {
// Livello della corsa singola scelto nel menu (Andrea 11/11): resta fino
// alla chiusura del gioco, cosi' "SINGLE RACE" a fine gara rifa' lo stesso.
// Vuoto = la partenza solita dell'albero (il tragitto iniziale, per pratica).
std::string g_tappaSingola;

// Le due partenze del menu, usate anche dalla scelta a fine gara.
void avviaSerie(Game& gioco) {
    if (!gioco.avviaGara()) gioco.init("lv_01_neon"); // senza percorso: giro libero
}
void avviaSingola(Game& gioco) {
    if (!gioco.avviaGara("motore_nuovo/dati/percorso_gara.json", true, g_tappaSingola)) gioco.init("lv_01_neon");
}
} // namespace

class MenuFase : public Fase {
public:
    MenuFase() : sel_(0), finita_(false), prossima_(FaseId::Gara) {}
    void entra(Game& gioco) override {
        sel_ = 0; finita_ = false; prossima_ = FaseId::Gara;
        livelli_ = gioco.elencoTappe();
        liv_ = 0;
        for (size_t i = 0; i < livelli_.size(); ++i) if (livelli_[i] == g_tappaSingola) liv_ = (i32)i;
    }
    void aggiorna(Game& gioco, f32 dt) override {
        (void)dt;
        const Pad& p = gioco.input();
        if (p.pressed & Pad::Up)   sel_ = (sel_ + 2) % 3;
        if (p.pressed & Pad::Down) sel_ = (sel_ + 1) % 3;
        // Corsa singola: SINISTRA/DESTRA sceglie il livello.
        const i32 n = (i32)livelli_.size();
        if (sel_ == 1 && n > 0) {
            if (p.pressed & Pad::Left)  liv_ = (liv_ + n - 1) % n;
            if (p.pressed & Pad::Right) liv_ = (liv_ + 1) % n;
        }
        if (p.pressed & (Pad::Start | Pad::A)) {
            switch (sel_) {
                case 0: avviaSerie(gioco); break; // gara a tappe con bivi (Andrea 11/11: voce principale)
                case 1: // corsa singola: una tappa a scelta, stessi rivali e tempo, poi arrivo
                    g_tappaSingola = n > 0 ? livelli_[(size_t)liv_] : std::string();
                    avviaSingola(gioco);
                    break;
                case 2: gioco.richiediUscita(); prossima_ = FaseId::Menu; break; // il runner chiude
            }
            finita_ = true;
        }
    }
    void disegna(Renderer& ren, Framebuffer& fb) const override {
        // Logo segnaposto OPEN RUN (Andrea 08/11): solo la scritta, nient'altro.
        // Il PNG e' in Libreria/assets_originali/ui/logo/logo_openrun.png.
        if (ren.haLogo()) ren.drawLogo(160, 50);
        else {
            const char* titolo = "OPEN RUN";
            const i32 w = ren.larghezzaTitolo(titolo, 2);
            ren.drawTitolo(160 - w / 2, 52, titolo, 0xFFFFFFFFu, 2, true);
        }
        static const char* voci[3] = { "RACE SERIES", "SINGLE RACE", "EXIT" };
        // La riga del livello sta sotto SINGLE RACE: EXIT scende di 10 px.
        static constexpr i32 Y[3] = { 98, 114, 140 };
        for (i32 i = 0; i < 3; ++i) {
            // Voci col font grassetto (Andrea 09/11), il testo d'aiuto resta piccolo.
            i32 wi = ren.larghezzaTitolo(voci[i], 1);
            u32 colore = (i == sel_) ? 0xFFFFFFFFu : 0xFFA0A0A0u;
            ren.drawTitolo(160 - wi / 2, Y[i], voci[i], colore, 1, true);
            if (i == sel_) ren.drawTitolo(160 - wi / 2 - 12, Y[i], ">", 0xFFFFFFFFu, 1, true);
        }
        if (!livelli_.empty()) {
            char buf[40];
            // Solo "ROUTE n" (Andrea 11/11 sera): i nomi interni dei livelli
            // restano segreti per chi gioca.
            if (sel_ == 1) std::snprintf(buf, sizeof(buf), "< ROUTE %d >", liv_ + 1);
            else std::snprintf(buf, sizeof(buf), "ROUTE %d", liv_ + 1);
            ren.drawTesto(160 - ren.larghezzaTesto(buf, 1) / 2, 129, buf, sel_ == 1 ? 0xFFB4F02Cu /* = ACCENTO */ : 0xFF808080u, 1, true);
        }
        (void)fb;
        const char* aiuto = sel_ == 1 ? "LEFT/RIGHT: ROUTE  START: OK" : "UP/DOWN: CHOOSE  START: OK";
        ren.drawTesto(160 - ren.larghezzaTesto(aiuto, 1) / 2, 166, aiuto, 0xFFA0A0A0u, 1, true);
    }
    void esci(Game&) override {}
    FaseId prossima() const override { return prossima_; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "Menu"; }
private:
    i32 sel_;
    bool finita_;
    FaseId prossima_;
    std::vector<std::string> livelli_;
    i32 liv_ = 0;
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
    void aggiorna(Game& gioco, f32 dt) override {
        timerS_ += dt;
        // Arrivo: si aspetta che l'auto si fermi in mezzo al pubblico.
        if (arrivo_) {
            const f32 fermo = gioco.fermoDopoArrivoS();
            const bool salta = fermo >= PREMIAZIONE_SALTA_DOPO_S && (gioco.input().pressed & (Pad::Start | Pad::A));
            finita_ = fermo >= FERMO_PRIMA_DEI_RISULTATI_S || salta || timerS_ >= ARRIVO_MAX_S;
        }
        else if (timerS_ >= FINE_TEMPO_DURATA_S) finita_ = true;
    }
    void disegna(Renderer& ren, Framebuffer&) const override {
        if (arrivo_) return; // la scena d'arrivo e' la strada (scacchi, pubblico, frenata)
        const char* testo = "TIME UP";
        const i32 w = ren.larghezzaTitolo(testo, 2);
        ren.drawTitolo(160 - w / 2, 96, testo, 0xFFFF0000u, 2, true);
    }
    void esci(Game&) override {}
    FaseId prossima() const override { return FaseId::Risultati; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "FineTempo"; }
private:
    f32 timerS_;
    bool finita_;
    bool arrivo_ = false;
};

// ---------------------------------------------------------------------------
// Fase: Risultati (fine gara uguale per tutti, Andrea 11/11)
// ---------------------------------------------------------------------------
// Stessi tre passi dopo l'arrivo e dopo il tempo scaduto:
//   1. RANKING (solo all'arrivo): la classifica del 09/11, invariata; passa
//      da sola dopo il verdetto, o con START/A;
//   2. NOME: fino a sei lettere da sala giochi (A-Z e '.'), una alla volta;
//   3. HIGH SCORES + scelta: rigiocare la stessa corsa, l'altra, o il menu.
// Il nome si salva in salvataggi/giocatore.json con la classifica Top-10.
// Se il file manca o e' vuoto la tabella si riempie con dati/classifica_finta.json.

struct PunteggioEntry {
    std::string nome;
    i32 punti;
    bool nostro = false; // la riga appena fatta (evidenziata)
};

namespace {
constexpr i32 NOME_LETTERE = 6;             // come i nomi dei rivali (Andrea 11/11 sera: 3 erano poche)
constexpr const char* LETTERE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ. "; // poche lettere, tutte nel font; spazio = casella vuota
constexpr i32 LETTERE_N = 28;
constexpr f32 RANKING_DOPO_VERDETTO_S = 3.0f; // poi il nome arriva da solo
constexpr f32 ATTESA_MAX_S = 30.0f;           // fermo senza tasti: conferma / menu
constexpr i32 RIGHE_TABELLA = 10;

i32 indiceLettera(char c) {
    for (i32 i = 0; i < LETTERE_N; ++i) if (LETTERE[i] == c) return i;
    return 0;
}

// Pannello scuro con bordo di 1 px (stesso stile della classifica del 09/11).
void pannello(Framebuffer& fb, i32 x0, i32 y0, i32 x1, i32 y1) {
    scurisci(fb, x0, y0, x1, y1, 30);
    fb.fillRect(x0, y0, x1, y0, 0xFF909090u);
    fb.fillRect(x0, y1, x1, y1, 0xFF909090u);
    fb.fillRect(x0, y0, x0, y1, 0xFF909090u);
    fb.fillRect(x1, y0, x1, y1, 0xFF909090u);
}

// Velo bianco trasparente (Andrea 11/11 sera: evidenza in bianco e nero,
// niente colore): schiarisce il fondo di un quarto. Solo interi.
void velo(Framebuffer& fb, i32 x0, i32 y0, i32 x1, i32 y1) {
    u32* d = fb.data();
    for (i32 y = std::max(0, y0); y <= std::min(Framebuffer::HEIGHT - 1, y1); ++y)
        for (i32 x = std::max(0, x0); x <= std::min(Framebuffer::WIDTH - 1, x1); ++x) {
            const u32 c = d[y * Framebuffer::WIDTH + x];
            const u32 r = (c >> 16) & 0xFFu, g = (c >> 8) & 0xFFu, b = c & 0xFFu;
            d[y * Framebuffer::WIDTH + x] = 0xFF000000u | ((r + (255 - r) / 4) << 16) |
                                            ((g + (255 - g) / 4) << 8) | (b + (255 - b) / 4);
        }
}

// Colore d'accento della UI (Andrea 11/11 sera): un solo colore
// riconoscibile, verde lime al posto del giallo; il resto bianco e grigi.
constexpr u32 ACCENTO = 0xFFB4F02Cu;

// Titolo col grassetto e le lettere un po' piu' larghe (aria fra una e l'altra).
void titoloSpaziato(Renderer& ren, i32 cx, i32 y, const char* t, u32 col, i32 scala, i32 aria) {
    i32 w = 0, n = 0;
    for (const char* c = t; *c; ++c, ++n) { const char l[2] = {*c, 0}; w += Renderer::larghezzaTitolo(l, scala) + scala; }
    w += aria * (n - 1) - scala;
    i32 x = cx - w / 2;
    for (const char* c = t; *c; ++c) {
        const char l[2] = {*c, 0};
        ren.drawTitolo(x, y, l, col, scala, true);
        x += Renderer::larghezzaTitolo(l, scala) + scala + aria;
    }
}

// Freccina piena di 5 px di base, punta in su o in giu'.
void freccia(Framebuffer& fb, i32 cx, i32 y, bool su, u32 c) {
    for (i32 r = 0; r < 3; ++r) {
        const i32 yy = su ? y + r : y + 2 - r;
        fb.fillRect(cx - r, yy, cx + r, yy, c);
    }
}
} // namespace

class RisultatiFase : public Fase {
public:
    enum class Passo { Ranking, Nome, Scelta };

    RisultatiFase() { std::strcpy(nome_, "PLAYER"); }
    void entra(Game& gioco) override {
        timerS_ = 0.0f; passoS_ = 0.0f; fermoS_ = 0.0f; finita_ = false;
        nomePos_ = 0;
        singola_ = gioco.corsaSingola();
        sel_ = singola_ ? 1 : 0; // la corsa appena fatta gia' scelta
        // Nome di partenza: quello salvato l'ultima volta; al primo giro
        // "PLAYER" (Andrea 11/11 sera: chi non vuole scriverlo preme START
        // e prosegue col nome generico).
        GiocatoreDati salvato = SalvataggiGiocatore::carica();
        std::strcpy(nome_, "PLAYER");
        if (!nomeVuoto(salvato.nome)) {
            for (i32 i = 0; i < NOME_LETTERE; ++i) {
                const char c = salvato.nome[i];
                nome_[i] = (c >= 'A' && c <= 'Z') || c == '.' || c == ' ' ? c : '.';
            }
            nome_[NOME_LETTERE] = '\0';
        }
        nomeVis_ = nomeScritto();
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
        // Arrivo: classifica della gara sul tempo di ognuno (Andrea 29/10).
        arrivo_.clear();
        if (gioco.arrivato())
            for (const Game::Classificato& c : gioco.classificaArrivo()) arrivo_.push_back(c);
        passo_ = arrivo_.empty() ? Passo::Nome : Passo::Ranking;
    }
    void aggiorna(Game& gioco, f32 dt) override {
        timerS_ += dt;
        passoS_ += dt;
        const Pad& p = gioco.input();
        fermoS_ = p.pressed ? 0.0f : fermoS_ + dt;
        switch (passo_) {
            case Passo::Ranking: {
                // Il verdetto compare dopo il consulto e le righe; poi si va
                // avanti da soli (niente scritta "PRESS START", Andrea 09/11).
                const f32 verdetto = CONSULTO_S + (f32)arrivo_.size() * RIGA_CLASSIFICA_S;
                const bool tasto = (p.pressed & (Pad::Start | Pad::A)) && timerS_ >= verdetto;
                if (tasto || timerS_ >= verdetto + RANKING_DOPO_VERDETTO_S) vaiA(Passo::Nome);
                break;
            }
            case Passo::Nome: {
                // Sala giochi: SU/GIU' cambia la lettera, A (o DESTRA) la
                // conferma e passa alla prossima, B (o SINISTRA) torna
                // indietro. Dopo la terza lettera, o con START, il nome e' fatto.
                char& c = nome_[nomePos_];
                if (p.pressed & Pad::Up)   c = LETTERE[(indiceLettera(c) + 1) % LETTERE_N];
                if (p.pressed & Pad::Down) c = LETTERE[(indiceLettera(c) + LETTERE_N - 1) % LETTERE_N];
                bool fatto = (p.pressed & Pad::Start) != 0;
                if (p.pressed & (Pad::A | Pad::Right)) {
                    if (nomePos_ + 1 < NOME_LETTERE) ++nomePos_;
                    else if (p.pressed & Pad::A) fatto = true;
                }
                if ((p.pressed & (Pad::B | Pad::Left)) && nomePos_ > 0) --nomePos_;
                nomeVis_ = nomeScritto();
                if (fatto || fermoS_ >= ATTESA_MAX_S) { salva(); vaiA(Passo::Scelta); }
                break;
            }
            case Passo::Scelta: {
                if (passoS_ < 0.3f) break; // il tasto del nome non sceglie anche qui
                if (p.pressed & Pad::Up)   sel_ = (sel_ + 2) % 3;
                if (p.pressed & Pad::Down) sel_ = (sel_ + 1) % 3;
                if (p.pressed & (Pad::Start | Pad::A)) {
                    // Stesse voci e stesse partenze del menu principale.
                    if (sel_ == 0) avviaSerie(gioco);
                    else if (sel_ == 1) avviaSingola(gioco);
                    finita_ = true; // sel 2: menu (prossima()); 0/1: la gara ha gia' chiesto il cambio
                }
                if (fermoS_ >= ATTESA_MAX_S) { sel_ = 2; finita_ = true; }
                break;
            }
        }
    }
    void disegna(Renderer& ren, Framebuffer& fb) const override {
        switch (passo_) {
            case Passo::Ranking: disegnaArrivo(ren, fb); break;
            case Passo::Nome:    disegnaNome(ren, fb); break;
            case Passo::Scelta:  disegnaScelta(ren, fb); break;
        }
    }
    Passo passo() const { return passo_; }

private:
    void vaiA(Passo p) { passo_ = p; passoS_ = 0.0f; fermoS_ = 0.0f; }

    // Il nome come si legge e si salva: senza caselle vuote; tutto vuoto = "PLAYER".
    std::string nomeScritto() const {
        std::string n;
        for (i32 i = 0; i < NOME_LETTERE; ++i) if (nome_[i] != ' ') n.push_back(nome_[i]);
        return n.empty() ? std::string("PLAYER") : n;
    }

    // Salva le iniziali e il punteggio (temp + rename, salvataggi.cpp) e
    // prepara la tabella: i record salvati; se non ce ne sono abbastanza,
    // quelli di dati/classifica_finta.json riempiono i posti vuoti.
    void salva() {
        GiocatoreDati g = SalvataggiGiocatore::carica();
        for (i32 i = 0; i < 6; ++i) g.nome[i] = i < (i32)nomeVis_.size() ? nomeVis_[(size_t)i] : ' ';
        g.nome[6] = '\0';
        ClassificaEntry e;
        e.nome = nomeVis_; e.punti = punteggio_; e.data = "0000-00-00"; // core senza orologio di sistema
        g.classifica.push_back(e);
        std::stable_sort(g.classifica.begin(), g.classifica.end(),
                         [](const ClassificaEntry& a, const ClassificaEntry& b) { return a.punti > b.punti; });
        if (g.classifica.size() > 10) g.classifica.resize(10);
        SalvataggiGiocatore::salva(g); // se non si puo' scrivere (sola lettura) si gioca lo stesso

        tabella_.clear();
        bool messo = false;
        for (const ClassificaEntry& r : g.classifica) {
            std::string n = r.nome;
            while (!n.empty() && n.back() == ' ') n.pop_back();
            const bool nostro = !messo && n == nomeVis_ && r.punti == punteggio_;
            messo = messo || nostro;
            tabella_.push_back({n, r.punti, nostro});
        }
        if (tabella_.size() < (size_t)RIGHE_TABELLA) {
            JsonReader finta(leggiFileTesto("motore_nuovo/dati/classifica_finta.json"));
            if (finta.parse() && jsonIsObject(finta.valore())) {
                JsonValue elencoV;
                if (jsonGet(jsonAsObject(finta.valore()), "giocatori", elencoV) && jsonIsArray(elencoV))
                    for (const JsonValue& v : jsonAsArray(elencoV)) {
                        if (!jsonIsObject(v)) continue;
                        const JsonObject& o = jsonAsObject(v);
                        JsonValue nv;
                        if (!jsonGet(o, "nome", nv) || !jsonIsString(nv)) continue;
                        tabella_.push_back({jsonAsString(nv), jsonNumero(o, "punti", 0), false});
                    }
            }
        }
        // Il nostro punteggio c'e' sempre, anche se non entra nei dieci.
        if (!messo) tabella_.push_back({nomeVis_, punteggio_, true});
        std::stable_sort(tabella_.begin(), tabella_.end(),
                         [](const PunteggioEntry& a, const PunteggioEntry& b) {
                             return a.punti != b.punti ? a.punti > b.punti : a.nostro && !b.nostro; });
        posizione_ = 1;
        for (size_t i = 0; i < tabella_.size(); ++i) if (tabella_[i].nostro) posizione_ = (i32)i + 1;
        if (tabella_.size() > (size_t)RIGHE_TABELLA) {
            tabella_.resize(RIGHE_TABELLA);
            if (posizione_ > RIGHE_TABELLA) tabella_.back() = {nomeVis_, punteggio_, true};
        }
    }

    // Passo 2: sei caselle col font dei titoli; quella in uso
    // lampeggia, con le freccine su/giu' (la lettera cambia con SU/GIU').
    void disegnaNome(Renderer& ren, Framebuffer& fb) const {
        constexpr i32 SU = 4, GIU = 156;
        pannello(fb, 48, SU, 271, GIU);
        const char* titolo = "Enter your name"; // frase: minuscole col grassetto (Andrea 11/11 sera)
        titoloSpaziato(ren, 160, 16, titolo, 0xFFFFFFFFu, 1, 1);
        constexpr i32 CASELLA = 26, PASSO = 30, Y = 66;
        const i32 x0 = 160 - (PASSO * (NOME_LETTERE - 1) + CASELLA) / 2;
        const bool acceso = std::fmod(passoS_, 0.5f) < 0.3f;
        for (i32 i = 0; i < NOME_LETTERE; ++i) {
            const i32 x = x0 + i * PASSO;
            const bool qui = i == nomePos_;
            // Bianco e nero: velo chiaro sulla casella in uso, scuro sulle altre.
            if (qui) velo(fb, x, Y, x + CASELLA - 1, Y + CASELLA - 1);
            else scurisci(fb, x, Y, x + CASELLA - 1, Y + CASELLA - 1, 55);
            fb.fillRect(x, Y + CASELLA, x + CASELLA - 1, Y + CASELLA, qui ? 0xFFFFFFFFu : 0xFF707070u);
            const char l[2] = { nome_[i], '\0' };
            const u32 col = qui ? (acceso ? ACCENTO : 0xFFFFFFFFu) : 0xFFFFFFFFu;
            ren.drawTitolo(x + CASELLA / 2 - ren.larghezzaTitolo(l, 2) / 2, Y, l, col, 2, true);
            if (qui) {
                freccia(fb, x + CASELLA / 2, Y - 7, true, 0xFFFFFFFFu);
                freccia(fb, x + CASELLA / 2, Y + CASELLA + 5, false, 0xFFFFFFFFu);
            }
        }
        ren.drawTesto(160 - ren.larghezzaTesto("UP/DOWN  LETTER", 1) / 2, 122, "UP/DOWN  LETTER", 0xFFA0A0A0u, 1, true);
        ren.drawTesto(160 - ren.larghezzaTesto("A  NEXT    B  BACK", 1) / 2, 133, "A  NEXT    B  BACK", 0xFFA0A0A0u, 1, true);
        ren.drawTesto(160 - ren.larghezzaTesto("START  OK", 1) / 2, 144, "START  OK", 0xFFA0A0A0u, 1, true);
    }

    // Passo 3: tabella HIGH SCORES (la nostra riga evidenziata) e sotto le
    // stesse voci del menu principale, la corsa appena fatta gia' scelta.
    void disegnaScelta(Renderer& ren, Framebuffer& fb) const {
        pannello(fb, 48, 4, 271, 214);
        const char* titolo = "HIGH SCORES";
        titoloSpaziato(ren, 160, 9, titolo, 0xFFFFFFFFu, 1, 2);
        char buf[32];
        for (size_t i = 0; i < tabella_.size(); ++i) {
            const PunteggioEntry& e = tabella_[i];
            const i32 y = 28 + (i32)i * 11;
            const i32 pos = e.nostro ? posizione_ : (i32)i + 1;
            if (e.nostro) velo(fb, 56, y - 2, 263, y + 8);
            const u32 col = e.nostro ? (std::fmod(passoS_, 0.5f) < 0.3f ? ACCENTO : 0xFFFFFFFFu) : 0xFFB0B0B0u;
            std::snprintf(buf, sizeof(buf), "%d", pos);
            ren.drawTesto(80 - ren.larghezzaTesto(buf, 1), y, buf, pos == 1 ? ACCENTO : col, 1, true);
            ren.drawTesto(104, y, e.nome.c_str(), col, 1, true);
            std::snprintf(buf, sizeof(buf), "%d", e.punti);
            ren.drawTesto(240 - ren.larghezzaTesto(buf, 1), y, buf, col, 1, true);
        }
        fb.fillRect(56, 145, 263, 145, 0xFF9898B0u);
        static const char* voci[3] = { "RACE SERIES", "SINGLE RACE", "MAIN MENU" };
        for (i32 i = 0; i < 3; ++i) {
            const i32 wi = ren.larghezzaTitolo(voci[i], 1);
            const u32 colore = (i == sel_) ? 0xFFFFFFFFu : 0xFFA0A0A0u;
            ren.drawTitolo(160 - wi / 2, 152 + i * 20, voci[i], colore, 1, true);
            if (i == sel_) ren.drawTitolo(160 - wi / 2 - 12, 152 + i * 20, ">", 0xFFFFFFFFu, 1, true);
        }
    }
public:
    // Classifica d'arrivo sopra la scena della strada (auto ferma, pubblico):
    // posizione, nome, tempo e distacco; il giocatore
    // evidenziato, podio in oro, argento e bronzo.
    // Consulto finale (Andrea 08/11): prima i tempi "girano" mentre si
    // confrontano (chi ha preso altre strade arriva da altre parti), poi le
    // righe si fermano una alla volta e sotto compare il verdetto: trofeo al
    // vincitore, altrimenti la posizione.
    void disegnaArrivo(Renderer& ren, Framebuffer& fb) const {
        // Pannello basso (Andrea 09/11): finisce a y 156, cosi' sotto si vede
        // la premiazione intorno all'auto.
        constexpr i32 SU = 4, GIU = 156;
        scurisci(fb, 48, SU, 271, GIU, 30);
        fb.fillRect(48, SU, 271, SU, 0xFF909090u);
        fb.fillRect(48, GIU, 271, GIU, 0xFF909090u);
        fb.fillRect(48, SU, 48, GIU, 0xFF909090u);
        fb.fillRect(271, SU, 271, GIU, 0xFF909090u);
        const bool consulto = timerS_ < CONSULTO_S;
        // Andrea 11/11 sera: "TOURNAMENT RESULTS" (corsa singola: "RACE RESULTS").
        const char* titolo = consulto ? "FINAL CHECK" : (singola_ ? "RACE RESULTS" : "TOURNAMENT RESULTS");
        if (!consulto || std::fmod(timerS_, 0.5f) < 0.35f)
            titoloSpaziato(ren, 160, 10, titolo, 0xFFFFFFFFu, 1, 1); // 11/11 sera: piu' piccolo, con aria
        ren.drawTesto(64, 29, "POS", 0xFFA0A0A0u, 0, true);
        ren.drawTesto(96, 29, "DRIVER", 0xFFA0A0A0u, 0, true);
        ren.drawTesto(170, 29, "TIME", 0xFFA0A0A0u, 0, true);
        ren.drawTesto(226, 29, "GAP", 0xFFA0A0A0u, 0, true);
        char buf[32], tempo[24];
        // Durante il consulto tutte le righe ci sono, coi tempi che girano;
        // poi si fermano una alla volta dall'alto.
        const size_t ferme = consulto ? 0 : std::min(arrivo_.size(), (size_t)((timerS_ - CONSULTO_S) / RIGA_CLASSIFICA_S) + 1);
        for (size_t i = 0; i < arrivo_.size(); ++i) {
            const Game::Classificato& c = arrivo_[i];
            const bool ferma = i < ferme;
            const i32 y = 38 + (i32)i * 11;
            if (c.giocatore && ferma) velo(fb, 56, y - 2, 266, y + 8);
            const u32 col = c.giocatore ? 0xFFFFFFFFu : 0xFFB0B0B0u;
            if (ferma) {
                std::snprintf(buf, sizeof(buf), "%d", (i32)i + 1);
                ren.drawTesto(66, y, buf, i == 0 ? ACCENTO : col, 1, true);
            } else {
                ren.drawTesto(66, y, "-", 0xFF707070u, 1, true);
            }
            // Il nostro nome in grande, gli altri piu' piccoli (Andrea 09/11):
            // il nostro risultato si trova subito.
            // Andrea 11/11: le nostre iniziali al posto di "YOU" (le ultime salvate).
            // Nome e tempo nostri col font principale: il grassetto, che non
            // ha una misura piu' piccola, qui era troppo grande (Andrea 11/11 sera).
            if (c.giocatore) ren.drawTesto(96, y, nomeVis_.c_str(), ferma ? col : 0xFF808080u, 1, true);
            else ren.drawTesto(96, y + 2, c.nome, ferma ? col : 0xFF808080u, 0, true);
            // Linea chiara di 1 px fra un pilota e l'altro.
            if (i + 1 < arrivo_.size()) fb.fillRect(56, y + 9, 263, y + 9, 0xFF9898B0u);
            if (ferma) {
                formattaTempo(tempo, sizeof(tempo), c.tempoS);
                ren.drawTesto(170, y, tempo, col, 1, true);
                if (i > 0) {
                    std::snprintf(buf, sizeof(buf), "+%.2f", c.tempoS - arrivo_[0].tempoS);
                    ren.drawTesto(226, y, buf, col, 1, true);
                }
            } else {
                // Cronometri che girano: cifre a caso dal tempo e dalla riga.
                const u32 h = ((u32)(timerS_ * 30.0f) * 2654435761u) ^ (u32)(i * 40503u);
                std::snprintf(tempo, sizeof(tempo), "%d'%02d\"%02d", (i32)(h % 3) + 1, (i32)((h >> 4) % 60), (i32)((h >> 10) % 100));
                ren.drawTesto(170, y, tempo, 0xFF808080u, 1, true);
            }
        }
        if (consulto) {
            const char* attesa = "HOLD ON..."; // Andrea 09/11
            ren.drawTesto(160 - ren.larghezzaTesto(attesa, 1) / 2, 132, attesa, 0xFFA0A0A0u, 1, true);
            return;
        }
        if (ferme < arrivo_.size()) return;
        // Verdetto: il vincitore ha il trofeo (la scritta lampeggia oro/bianco),
        // gli altri la loro posizione.
        i32 pos = 0;
        for (size_t i = 0; i < arrivo_.size(); ++i) if (arrivo_[i].giocatore) pos = (i32)i + 1;
        const f32 dalVerdetto = timerS_ - CONSULTO_S - (f32)arrivo_.size() * RIGA_CLASSIFICA_S;
        if (pos == 1) {
            // Born2bSporty anche qui (Andrea 11/11 sera); lampeggia lime/bianco.
            const char* v = "WINNER!";
            const i32 w = ren.larghezzaTitolo(v, 2);
            const u32 c = std::fmod(dalVerdetto, 0.4f) < 0.2f ? ACCENTO : 0xFFFFFFFFu;
            ren.drawTitolo(160 - w / 2, 126, v, c, 2, true);
            trofeo(fb, 160 - w / 2 - 18, 132);
            trofeo(fb, 160 + w / 2 + 8, 132);
        } else if (pos > 0) {
            static const char* SUFF[4] = {"th", "st", "nd", "rd"};
            const char* suff = (pos % 100 >= 11 && pos % 100 <= 13) || pos % 10 > 3 ? SUFF[0] : SUFF[pos % 10];
            std::snprintf(buf, sizeof(buf), "You finished %d%s", pos, suff);
            ren.drawTitolo(160 - ren.larghezzaTitolo(buf, 1) / 2, 132, buf, pos <= 3 ? ACCENTO : 0xFFFFFFFFu, 1, true);
        }
        // Niente POINTS (Andrea 11/11 sera: non si riferiscono a niente).
        // URTI / CIGLIO tolti dalla classifica (Andrea 06/11).
    }
    // Trofeo pixel 10x12: coppa oro con manici, gambo e base scura.
    static void trofeo(Framebuffer& fb, i32 x, i32 y) {
        const u32 oro = ACCENTO, ombra = 0xFF6E9A18u, base = 0xFF404040u; // coppa nel colore d'accento
        fb.fillRect(x + 1, y, x + 8, y + 5, oro);
        fb.fillRect(x + 2, y + 6, x + 7, y + 6, oro);
        fb.fillRect(x + 6, y + 1, x + 7, y + 5, ombra);
        fb.fillRect(x, y + 1, x, y + 3, oro);         // manico sinistro
        fb.fillRect(x + 9, y + 1, x + 9, y + 3, oro); // manico destro
        fb.fillRect(x + 4, y + 7, x + 5, y + 9, ombra);
        fb.fillRect(x + 2, y + 10, x + 7, y + 11, base);
        fb.fillRect(x + 3, y + 1, x + 3, y + 3, 0xFFFFFFFFu); // riflesso
    }
    void esci(Game&) override {}
    FaseId prossima() const override { return FaseId::Menu; }
    bool finita() const override { return finita_; }
    const char* nome() const override { return "Risultati"; }
private:
    f32 timerS_ = 0.0f;  // dall'entrata (consulto e righe della classifica)
    f32 passoS_ = 0.0f;  // dall'inizio del passo corrente (lampeggi)
    f32 fermoS_ = 0.0f;  // senza tasti premuti
    i32 punteggio_ = 0;
    i32 posizione_ = 1;  // posizione nella tabella HIGH SCORES
    bool finita_ = false;
    Passo passo_ = Passo::Nome;
    char nome_[NOME_LETTERE + 1]; // le sei caselle (spazio = vuota)
    std::string nomeVis_ = "PLAYER"; // il nome scritto (salvato in salvataggi/giocatore.json)
    i32 nomePos_ = 0;    // lettera in uso 0..5
    bool singola_ = false; // corsa singola (titolo della classifica)
    i32 sel_ = 0;        // scelta finale: serie, singola, menu
    std::vector<PunteggioEntry> tabella_;
    std::vector<Game::Classificato> arrivo_; // classifica sul tempo, solo dopo l'arrivo
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
    // V47 V4: dissolvenza di 0.5 s fra risultati, menu e partenza. Nella
    // prima meta' la fase vecchia resta (e attuale() non cambia, cosi' Game
    // disegna ancora la sua scena) e si scurisce; a meta' si cambia fase e
    // la nuova schiarisce.
    if (fadeSt_ == FadeState::None) {
        const bool daRisultatiAMenu = (attuale_ == FaseId::Risultati && id == FaseId::Menu);
        const bool daMenuAGara = (attuale_ == FaseId::Menu && id == FaseId::Gara);
        const bool daRisultatiAGara = (attuale_ == FaseId::Risultati && id == FaseId::Gara); // rigioca (11/11)
        if (daRisultatiAMenu || daMenuAGara || daRisultatiAGara) {
            fadeSt_ = FadeState::Out;
            fadeT_ = 0.0f;
            faseProssimaFade_ = id;
            return;
        }
    } else {
        faseProssimaFade_ = id; // cambio chiesto a dissolvenza in corso: vince l'ultimo
        if (fadeSt_ == FadeState::Out) return;
        fadeSt_ = FadeState::None; // gia' in schiarita: cambio secco
    }
    if (fase_) fase_->esci(gioco);
    fase_ = crea(id);
    attuale_ = id;
    if (fase_) fase_->entra(gioco);
}

void GestoreFasi::aggiorna(Game& gioco, f32 dt) {
    if (fadeSt_ == FadeState::Out) {
        fadeT_ += dt;
        if (fadeT_ >= fadeDur_ * 0.5f) {
            fadeSt_ = FadeState::In;
            fadeT_ = 0.0f;
            if (fase_) fase_->esci(gioco);
            fase_ = crea(faseProssimaFade_);
            attuale_ = faseProssimaFade_;
            if (fase_) fase_->entra(gioco);
        }
        return;
    }
    if (fadeSt_ == FadeState::In) {
        fadeT_ += dt;
        if (fadeT_ >= fadeDur_ * 0.5f) {
            fadeSt_ = FadeState::None;
            fadeT_ = 0.0f;
        }
        // la fase nuova gira gia' mentre schiarisce
    }
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

void GestoreFasi::dissolvenza(Framebuffer& fb) const {
    if (fadeSt_ == FadeState::None) return;
    const f32 meta = fadeDur_ * 0.5f;
    f32 buio = std::clamp(fadeT_ / meta, 0.0f, 1.0f); // 0 = pieno, 1 = nero
    if (fadeSt_ == FadeState::In) buio = 1.0f - buio;
    const u32 p = (u32)(buio * 100.0f);
    u32* d = fb.data();
    for (i32 i = 0; i < Framebuffer::WIDTH * Framebuffer::HEIGHT; ++i) {
        const u32 c = d[i];
        d[i] = 0xFF000000u | ((((c >> 16) & 0xFFu) * (100 - p) / 100) << 16) |
               ((((c >> 8) & 0xFFu) * (100 - p) / 100) << 8) | ((c & 0xFFu) * (100 - p) / 100);
    }
}

void GestoreFasi::disegna(Renderer& ren, Framebuffer& fb) const {
    if (fase_) fase_->disegna(ren, fb);
    dissolvenza(fb);
}


} // namespace sgl
