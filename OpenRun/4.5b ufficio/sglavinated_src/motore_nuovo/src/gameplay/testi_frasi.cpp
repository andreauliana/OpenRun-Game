// SPDX-License-Identifier: MIT
#include "gameplay/testi_frasi.h"

#include "core/json_mini.h"   // stesso parser di livello.json (header-only, MIT)
#include "gameplay/frasi.h"   // eGianni()/eGesu() per verifica()
#include <cstdio>             // std::fopen/fread/fclose
#include <string>

// Caricamento dei testi da motore_nuovo/dati/frasi.json (CC-BY-4.0).
// Struttura usata del file: { "frasi": [ { "id": N, "personaggio": "gianni"|
// "gesu", "linea1": "...", "linea2": "..." }, ... ] } (id 0..155, in ordine;
// "linea2" puo' mancare). Nessun codice 68000, nessun asset Sega.

namespace sgl::gameplay {
namespace {

void copiaRiga(char* dst, u32 dim, const std::string& s) {
    u32 i = 0;
    const u32 n = static_cast<u32>(s.size());
    for (; i + 1 < dim && i < n; ++i) dst[i] = s[i];
    dst[i] = '\0';
}

Personaggio daNome(const std::string& s) {
    if (s == "gianni") return Personaggio::Gianni;
    if (s == "gesu" || s == "gesu'" || s == "gesù") return Personaggio::Gesu;
    return Personaggio::Nessuno;
}

} // namespace

bool TestiFrasi::carica(const char* percorso) {
    f_.clear();
    if (!percorso) return false;
    std::FILE* fp = std::fopen(percorso, "rb");
    if (!fp) return false;
    std::string testo;
    char buf[4096];
    size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), fp)) > 0) testo.append(buf, n);
    std::fclose(fp);
    if (testo.empty()) return false;

    JsonReader reader(testo);
    if (!reader.parse()) return false;
    const JsonValue& root = reader.valore();
    JsonValue campo;
    if (!jsonIsObject(root) ||
        !jsonGet(jsonAsObject(root), "frasi", campo) ||
        !jsonIsArray(campo)) {
        return false;
    }
    const JsonArray& arr = jsonAsArray(campo);

    // Prima passata: id massimo (gli id sono 0..N-1, ma non si assume ordine).
    u32 maxId = 0;
    bool primo = true;
    for (const JsonValue& v : arr) {
        if (!jsonIsObject(v)) continue;
        const JsonObject& o = jsonAsObject(v);
        JsonValue idV;
        if (!jsonGet(o, "id", idV) || !jsonIsNumber(idV)) continue;
        const u32 id = static_cast<u32>(jsonAsNumber(idV));
        if (primo || id > maxId) { maxId = id; primo = false; }
    }
    if (primo) return false; // nessuna voce valida

    f_.assign(static_cast<size_t>(maxId) + 1, FraseTesto{}); // alloc qui

    // Seconda passata: testi per id.
    for (const JsonValue& v : arr) {
        if (!jsonIsObject(v)) continue;
        const JsonObject& o = jsonAsObject(v);
        JsonValue idV;
        if (!jsonGet(o, "id", idV) || !jsonIsNumber(idV)) continue;
        const u32 id = static_cast<u32>(jsonAsNumber(idV));
        if (static_cast<size_t>(id) >= f_.size()) continue;
        FraseTesto& f = f_[id];

        JsonValue t1;
        if (jsonGet(o, "linea1", t1) && jsonIsString(t1))
            copiaRiga(f.linea1, FraseTesto::MAX_RIGA, jsonAsString(t1));
        JsonValue t2;
        if (jsonGet(o, "linea2", t2) && jsonIsString(t2))
            copiaRiga(f.linea2, FraseTesto::MAX_RIGA, jsonAsString(t2));
        JsonValue pV;
        if (jsonGet(o, "personaggio", pV) && jsonIsString(pV))
            f.personaggio = daNome(jsonAsString(pV));

        f.righe = (f.linea2[0] != '\0') ? 2
                 : (f.linea1[0] != '\0' ? 1 : 0);
    }
    return true;
}

bool TestiFrasi::verifica() const {
    for (size_t i = 0; i < f_.size(); ++i) {
        const FraseTesto& f = f_[i];
        if (f.righe == 0) continue; // voce vuota: nessun testo da disegnare
        const u32 id = static_cast<u32>(i);
        switch (f.personaggio) {
            case Personaggio::Gianni: if (!eGianni(id)) return false; break;
            case Personaggio::Gesu:   if (!eGesu(id))   return false; break;
            default: return false; // personaggio mancante o sconosciuto
        }
    }
    return true;
}

} // namespace sgl::gameplay
