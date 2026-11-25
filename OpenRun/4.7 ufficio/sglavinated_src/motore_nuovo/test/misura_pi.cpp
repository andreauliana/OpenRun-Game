// SPDX-License-Identifier: MIT
// misura_pi.cpp — misura dei tempi per frame del nucleo, pensata per il Pi 400.
// Solo lettura: non cambia nulla del gioco, guida da sola e cronometra.
//
// Build : cmake --build build --target misura_pi
// Uso   : ./motore_nuovo/build/misura_pi [cartella_out=out/misura_pi] [secondi_per_pista=60] [solo_livello]
//         Lanciare dalla cartella sglavinated_src (dati con percorsi relativi).
//
// Per ogni pista (6 livelli in tondo) e per la gara a tappe guida da sola
// (gas sempre, sterzo verso il centro quando l'auto si allarga) e misura,
// frame per frame:
//   update  = logica (Game::update)
//   render  = disegno del framebuffer 320x224 (Game::render)
//   crt     = sbavatura del filtro CRT (crtSbava), come fa il runner col filtro acceso
// Scrive <out>/misura.txt (riassunto) e <out>/frame_ms.csv (tutti i frame).
// Non apre finestre e non usa SDL: misura la CPU del gioco, non la GPU ne' lo
// schermo. Per quelli c'e' il runner con --misura (vedi pi/LEGGIMI.md).
//
// Budget: 1/frequenza s (16.67 ms a 60 Hz). Il runner deve ancora copiare il
// framebuffer nella texture e presentarlo, quindi un margine sotto ~10 ms
// (update+render+crt) e' quello che serve sul Pi.
// Exit code: 0 sempre se la misura e' completa (e' una misura, non un gate) | 4 cwd errata
#include "game/game.h"
#include "render/crt.h"
#include "render/framebuffer.h"
#include "render/sprite_raw.h"
#include "runner/sponsor_loader.h"
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace sgl;

// Frequenza logica da dati/config.json: definita in src/game/game.cpp nello
// spazio globale (come in prova_gate.cpp).
sgl::i32 leggiFrequenzaDaConfig();

namespace {

using Orologio = std::chrono::steady_clock;

double msDa(Orologio::time_point a, Orologio::time_point b) {
    return std::chrono::duration<double, std::milli>(b - a).count();
}

// Primi frame esclusi dalle statistiche: caricamenti pigri e cache fredde.
constexpr int FRAME_RISCALDAMENTO = 60;

struct Serie {
    std::vector<double> v;
    double media() const {
        if (v.empty()) return 0.0;
        double s = 0.0;
        for (double x : v) s += x;
        return s / (double)v.size();
    }
    double percentile(double p) const {
        if (v.empty()) return 0.0;
        std::vector<double> c = v;
        const size_t k = std::min(c.size() - 1, (size_t)(p / 100.0 * (double)(c.size() - 1) + 0.5));
        std::nth_element(c.begin(), c.begin() + (std::ptrdiff_t)k, c.end());
        return c[k];
    }
    double massimo() const { return v.empty() ? 0.0 : *std::max_element(v.begin(), v.end()); }
    size_t sopra(double soglia) const {
        return (size_t)std::count_if(v.begin(), v.end(), [&](double x) { return x > soglia; });
    }
};

struct Risultato {
    std::string nome;
    Serie update, render, crt, totale; // totale = update + render + crt
    double tempC0 = -1.0, tempC1 = -1.0;
    long mhz0 = -1, mhz1 = -1;
    double velocitaMedia = 0.0; // controllo: l'auto deve correre davvero
};

// Letture di sistema (Linux / Pi OS). Su Mac non ci sono: -1.
double leggiTemperaturaC() {
    std::ifstream f("/sys/class/thermal/thermal_zone0/temp");
    long milli = 0;
    if (f >> milli) return (double)milli / 1000.0;
    return -1.0;
}
long leggiMHz() {
    std::ifstream f("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    long khz = 0;
    if (f >> khz) return khz / 1000;
    return -1;
}
std::string modelloMacchina() {
    {
        std::ifstream f("/proc/device-tree/model");
        std::string s;
        if (std::getline(f, s, '\0') && !s.empty()) return s;
    }
    std::ifstream f("/proc/cpuinfo");
    std::string riga;
    while (std::getline(f, riga))
        if (riga.rfind("model name", 0) == 0) {
            const size_t p = riga.find(':');
            if (p != std::string::npos) return riga.substr(p + 2);
        }
    return "sconosciuto";
}

// Stessi sprite del runner (runner/main.cpp), caricati prima di init()/avviaGara():
// senza, il disegno costerebbe meno del gioco vero.
void caricaSprite(Game& game) {
    game.setSponsorSprites(caricaSponsorSprite("GRAFICA/sponsor"));
    std::vector<Sprite> rivali;
    static const char* kRivale[7] = { "rosso", "azzurro", "giallo", "verde", "bianco", "arancio", "viola" };
    for (const char* c : kRivale) {
        Sprite s = caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/rivali/auto_rivale_") + c + "_retro.sglraw");
        if (s.width > 0) rivali.push_back(std::move(s));
    }
    game.setRivaliSprites(std::move(rivali));
    std::vector<Sprite> traffico;
    static const char* kTraffico[4] = { "utilitaria_bianca", "furgone_grigio", "berlina_blu", "camioncino_beige" };
    for (const char* n : kTraffico) {
        Sprite s = caricaSpriteRaw(std::string("motore_nuovo/dati/sprite_raw/traffico/traffico_") + n + "_retro.sglraw");
        if (s.width > 0) traffico.push_back(std::move(s));
    }
    game.setTrafficoSprites(std::move(traffico));
}

// Guida automatica: gas sempre, sterzo verso il centro quando l'auto si allarga
// (stessa regola di prova_gate, modo avvolgimento).
Pad pilota(const Game& game) {
    Pad pad{};
    pad.held = Pad::A;
    const f32 x = game.state().playerX;
    if (x > 120.0f) pad.held |= Pad::Left;
    else if (x < -120.0f) pad.held |= Pad::Right;
    return pad;
}

Risultato misura(const std::string& nome, const std::string& livello, int frames, std::FILE* csv) {
    Risultato r;
    r.nome = nome;
    auto game = std::make_unique<Game>(20261015u); // stesso seme del runner
    caricaSprite(*game);
    game->abilitaPartenza(true); // come il runner: griglia e 3-2-1 fanno parte della gara
    if (!livello.empty()) game->init(livello);
    else if (!game->avviaGara()) game->init();

    std::vector<u32> sbavato((size_t)Framebuffer::WIDTH * Framebuffer::HEIGHT);
    r.tempC0 = leggiTemperaturaC();
    r.mhz0 = leggiMHz();
    u64 heldPrev = 0;
    for (int f = 0; f < frames; ++f) {
        Pad pad = pilota(*game);
        pad.pressed = pad.held & ~heldPrev;
        pad.released = heldPrev & ~pad.held;
        heldPrev = pad.held;
        game->setInput(pad);

        const auto t0 = Orologio::now();
        game->update();
        const auto t1 = Orologio::now();
        game->render();
        const auto t2 = Orologio::now();
        crtSbava(game->fb().data(), sbavato.data(), Framebuffer::WIDTH, Framebuffer::HEIGHT);
        const auto t3 = Orologio::now();

        const double u = msDa(t0, t1), d = msDa(t1, t2), c = msDa(t2, t3);
        if (csv) std::fprintf(csv, "%s,%d,%.3f,%.3f,%.3f\n", nome.c_str(), f, u, d, c);
        if (f < FRAME_RISCALDAMENTO) continue;
        r.update.v.push_back(u);
        r.render.v.push_back(d);
        r.crt.v.push_back(c);
        r.totale.v.push_back(u + d + c);
        r.velocitaMedia += game->state().playerSpeed;
    }
    if (frames > FRAME_RISCALDAMENTO) r.velocitaMedia /= (double)(frames - FRAME_RISCALDAMENTO);
    r.tempC1 = leggiTemperaturaC();
    r.mhz1 = leggiMHz();
    return r;
}

void riga(std::ostringstream& o, const Risultato& r, double budget) {
    char buf[320];
    const double pctSopra = r.totale.v.empty() ? 0.0
        : 100.0 * (double)r.totale.sopra(budget) / (double)r.totale.v.size();
    std::snprintf(buf, sizeof buf,
                  "%-16s %6.2f %6.2f %6.2f | %6.2f %6.2f %6.2f %7.2f | %5.1f%% | %5.0f | ",
                  r.nome.c_str(), r.update.media(), r.render.media(), r.crt.media(),
                  r.totale.media(), r.totale.percentile(95), r.totale.percentile(99),
                  r.totale.massimo(), pctSopra, r.velocitaMedia);
    o << buf;
    if (r.tempC0 >= 0) {
        std::snprintf(buf, sizeof buf, "%4.1f->%4.1f C", r.tempC0, r.tempC1);
        o << buf;
    } else o << "     -     ";
    if (r.mhz0 >= 0) {
        std::snprintf(buf, sizeof buf, " %4ld->%4ld MHz", r.mhz0, r.mhz1);
        o << buf;
    }
    o << "\n";
}

} // namespace

int main(int argc, char** argv) {
    if (argc > 1 && argv[1][0] == '-') {
        std::printf("uso: misura_pi [cartella_out=out/misura_pi] [secondi_per_pista=60] [solo_livello]\n"
                    "Lanciare dalla cartella sglavinated_src.\n");
        return 0;
    }
    const std::string outDir = argc > 1 ? argv[1] : "out/misura_pi";
    const int secondi = argc > 2 ? std::max(2, std::atoi(argv[2])) : 60;
    const std::string solo = argc > 3 ? argv[3] : "";
    if (!std::filesystem::exists("motore_nuovo/dati/livelli/lv_01_neon/livello.json")) {
        std::printf("FALLITO: dati non trovati. Lanciare dalla cartella sglavinated_src.\n");
        return 4;
    }
    std::filesystem::create_directories(outDir);

    const int hz = std::max(1, (int)::leggiFrequenzaDaConfig());
    const double budget = 1000.0 / (double)hz;
    const int frames = secondi * hz;

    std::FILE* csv = std::fopen((outDir + "/frame_ms.csv").c_str(), "w");
    if (csv) std::fprintf(csv, "pista,frame,update_ms,render_ms,crt_ms\n");

    std::vector<std::pair<std::string, std::string>> prove; // (nome, livello; vuoto = gara)
    static const char* kLivelli[6] = { "lv_01_neon", "lv_02_atomico", "lv_03_foglie",
                                       "lv_04_cromo", "lv_05_spray", "lv_06_desktop" };
    for (const char* lv : kLivelli)
        if (solo.empty() || solo == lv) prove.push_back({lv, lv});
    if (solo.empty() || solo == "gara") prove.push_back({"gara_a_tappe", ""});

    std::printf("misura_pi: %s, %d Hz (budget %.2f ms), %d s per pista\n",
                modelloMacchina().c_str(), hz, budget, secondi);
    std::vector<Risultato> ris;
    for (const auto& p : prove) {
        std::printf("  %s ...\n", p.first.c_str());
        std::fflush(stdout);
        ris.push_back(misura(p.first, p.second, frames, csv));
    }
    if (csv) std::fclose(csv);

    std::ostringstream o;
    o << "misura_pi — tempi per frame del nucleo (ms), primi " << FRAME_RISCALDAMENTO << " frame esclusi\n";
    o << "macchina: " << modelloMacchina() << "\n";
    char buf[200];
    std::snprintf(buf, sizeof buf, "frequenza: %d Hz, budget %.2f ms per frame; %d s per pista\n\n", hz, budget, secondi);
    o << buf;
    o << "pista            update render    crt |  media    p95    p99     max | >budget | vel.  | temperatura / clock\n";
    o << "---------------- ------ ------ ------ | ------ ------ ------ ------- | ------- | ----- | ----------------\n";
    Risultato peggiore;
    for (const auto& r : ris) {
        riga(o, r, budget);
        if (r.totale.percentile(99) > peggiore.totale.percentile(99)) peggiore = r;
    }
    std::snprintf(buf, sizeof buf,
                  "\npista piu' pesante (p99): %s, %.2f ms = %.0f%% del budget\n",
                  peggiore.nome.c_str(), peggiore.totale.percentile(99),
                  100.0 * peggiore.totale.percentile(99) / budget);
    o << buf;
    o << "Lettura: media e p99 sotto ~10 ms lasciano margine al runner (copia texture, CRT, vsync).\n"
         "p99 oltre il budget = scatti visibili su quella pista.\n";

    std::fputs(o.str().c_str(), stdout);
    std::ofstream((outDir + "/misura.txt").c_str()) << o.str();
    std::printf("scritti: %s/misura.txt, %s/frame_ms.csv\n", outDir.c_str(), outDir.c_str());
    return 0;
}
