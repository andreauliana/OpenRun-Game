// SPDX-License-Identifier: MIT
#include "game/salvataggi.h"
#include "core/json_mini.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define SGL_MKDIR(d) _mkdir(d)
#else
#include <sys/stat.h>
#define SGL_MKDIR(d) mkdir(d, 0777)
#endif

namespace sgl {

const char* SalvataggiGiocatore::PERCORSO = "salvataggi/giocatore.json";

namespace {

const char* CHARSET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789. "; // 37 caratteri

bool carattereValido(char c) {
    for (const char* p = CHARSET; *p; ++p)
        if (*p == c) return true;
    return false;
}

void normalizzaNome(char nome[7]) {
    for (i32 i = 0; i < 6; ++i)
        if (!carattereValido(nome[i])) nome[i] = ' ';
    // spazi sempre in fondo: compatta verso sinistra
    char compatto[7] = {' ', ' ', ' ', ' ', ' ', ' ', '\0'};
    i32 j = 0;
    for (i32 i = 0; i < 6 && j < 6; ++i)
        if (nome[i] != ' ') compatto[j++] = nome[i];
    for (i32 i = 0; i < 6; ++i) nome[i] = compatto[i];
    nome[6] = '\0';
}

bool nomeNonVuoto(const char nome[7]) {
    for (i32 i = 0; i < 6; ++i)
        if (nome[i] != ' ') return true;
    return false;
}

std::string leggiFile(const char* percorso) {
    std::string testo;
    std::FILE* f = std::fopen(percorso, "rb");
    if (!f) return testo;
    char buf[4096];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0) testo.append(buf, n);
    std::fclose(f);
    return testo;
}

std::string jsonEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 2);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:   out.push_back(c); break;
        }
    }
    return out;
}

} // namespace

bool SalvataggiGiocatore::creaDirectorySeManca() {
    std::FILE* probe = std::fopen("salvataggi", "rb"); // esiste già come file: non si puo' creare directory omonima
    if (probe) { std::fclose(probe); return false; }
    if (SGL_MKDIR("salvataggi") != 0) return false; // errno=EEXIST se gia' presente: accettato
    return true;
}

namespace {
void leggiClassifica(const JsonObject& root, const char* chiave, std::vector<ClassificaEntry>& out) {
    JsonValue cv;
    if (!jsonGet(root, chiave, cv) || !jsonIsArray(cv)) return;
    for (const JsonValue& e : jsonAsArray(cv)) {
        if (!jsonIsObject(e)) continue;
        const JsonObject& o = jsonAsObject(e);
        ClassificaEntry entry;
        JsonValue nv2, pv, dv;
        if (!jsonGet(o, "nome", nv2) || !jsonIsString(nv2)) continue;
        entry.nome = jsonAsString(nv2);
        if (entry.nome.size() > 6) entry.nome.resize(6);
        while (entry.nome.size() < 6) entry.nome.push_back(' ');
        entry.punti = jsonGet(o, "punti", pv) && jsonIsNumber(pv) ? (i32)jsonAsNumber(pv) : 0;
        entry.data = jsonGet(o, "data", dv) && jsonIsString(dv) ? jsonAsString(dv) : std::string("0000-00-00");
        out.push_back(std::move(entry));
    }
}

void scriviClassifica(std::FILE* f, const char* chiave, const std::vector<ClassificaEntry>& c, bool ultima) {
    std::fprintf(f, "  \"%s\": [\n", chiave);
    i32 n = 0; // massimo 10, ordinati per punti decrescenti
    for (const ClassificaEntry& e : c) {
        if (n > 0) std::fprintf(f, ",\n");
        std::fprintf(f, "    {\"nome\": \"%s\", \"punti\": %d, \"data\": \"%s\"}",
                     jsonEscape(e.nome).c_str(), e.punti, jsonEscape(e.data).c_str());
        if (++n >= 10) break;
    }
    std::fprintf(f, "\n  ]%s\n", ultima ? "" : ",");
}
} // namespace

GiocatoreDati SalvataggiGiocatore::carica() {
    GiocatoreDati g;
    std::string testo = leggiFile(PERCORSO);
    if (testo.empty()) return g;

    JsonReader reader(testo);
    if (!reader.parse()) return g; // file danneggiato: default, si ricrea al salvataggio
    const JsonValue& val = reader.valore();
    if (!jsonIsObject(val)) return g;
    const JsonObject& root = jsonAsObject(val);

    // Nome.
    JsonValue nv;
    if (jsonGet(root, "nome", nv) && jsonIsString(nv)) {
        std::string nome = jsonAsString(nv);
        if (nome.size() > 6) nome.resize(6);
        for (i32 i = 0; i < 6; ++i) g.nome[i] = i < (i32)nome.size() ? nome[i] : ' ';
        normalizzaNome(g.nome);
    }

    // Classifiche (massimo 10 ciascuna, come salvate): gara a tappe e corsa singola.
    leggiClassifica(root, "classifica", g.classifica);
    leggiClassifica(root, "classifica_singola", g.classificaSingola);
    return g;
}

bool SalvataggiGiocatore::salva(const GiocatoreDati& g) {
    GiocatoreDati normale = g;
    normalizzaNome(normale.nome);
    if (!creaDirectorySeManca()) {
        // directory gia' esistente o errore: prova lo stesso a scrivere
    }
    std::string tmp = std::string(PERCORSO) + ".tmp";
    std::FILE* f = std::fopen(tmp.c_str(), "wb");
    if (!f) return false;

    std::string nomeVis;
    for (i32 i = 0; i < 6; ++i)
        if (normale.nome[i] != ' ') nomeVis.push_back(normale.nome[i]);
    if (nomeVis.empty()) nomeVis = "PLAYER";

    std::fprintf(f, "{\n");
    std::fprintf(f, "  \"nome\": \"%s\",\n", jsonEscape(nomeVis).c_str());
    scriviClassifica(f, "classifica", g.classifica, false);
    scriviClassifica(f, "classifica_singola", g.classificaSingola, true);
    std::fprintf(f, "}\n");
    if (std::fflush(f) != 0) { std::fclose(f); return false; }
    std::fclose(f);
    if (std::rename(tmp.c_str(), PERCORSO) != 0) return false;
    return true;
}

} // namespace sgl
