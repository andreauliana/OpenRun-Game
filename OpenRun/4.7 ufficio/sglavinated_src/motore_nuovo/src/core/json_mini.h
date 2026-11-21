// SPDX-License-Identifier: MIT
// Lettura JSON minima (sottoinsieme usato da livello.json): oggetti {}, array [],
// stringhe "..." con escape, numeri, true/false/null.
// Header-only, C++17, zero dipendenze esterne (solo libreria standard).
// Tecnica: parsing ricorsivo-discendente scritto da zero (nessun codice copiato).
// Usato da Game::init() per leggere motore_nuovo/dati/livelli/*/livello.json.
#pragma once

#include <cstdlib>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "core/types.h" // u32, i32

namespace sgl {

struct JsonNull {};

// std::variant non supporta tipi ricorsivi diretti in C++17.
// Usiamo un wrapper struct con unique_ptr per array e oggetti.
struct JsonValue;
using JsonObject = std::map<std::string, JsonValue>;
using JsonArray  = std::vector<JsonValue>;

struct JsonValue {
    using Var = std::variant<JsonNull, bool, double, std::string,
                             std::unique_ptr<JsonArray>,
                             std::unique_ptr<JsonObject>>;
    Var v;
    JsonValue() : v(JsonNull{}) {}
    JsonValue(JsonNull n)        : v(n) {}
    JsonValue(bool b)            : v(b) {}
    JsonValue(double d)          : v(d) {}
    JsonValue(std::string s)     : v(std::move(s)) {}
    JsonValue(JsonArray a)       : v(std::make_unique<JsonArray>(std::move(a))) {}
    JsonValue(JsonObject o)      : v(std::make_unique<JsonObject>(std::move(o))) {}
    JsonValue(const JsonValue& o) { copyFrom(o); }
    JsonValue(JsonValue&&) = default;
    JsonValue& operator=(const JsonValue& o) { if (this != &o) { copyFrom(o); } return *this; }
    JsonValue& operator=(JsonValue&&) = default;
private:
    void copyFrom(const JsonValue& o) {
        if (auto* p = std::get_if<std::unique_ptr<JsonArray>>(&o.v))
            v = std::make_unique<JsonArray>(**p);
        else if (auto* p = std::get_if<std::unique_ptr<JsonObject>>(&o.v))
            v = std::make_unique<JsonObject>(**p);
        else if (auto* p = std::get_if<JsonNull>(&o.v)) v = *p;
        else if (auto* p = std::get_if<bool>(&o.v)) v = *p;
        else if (auto* p = std::get_if<double>(&o.v)) v = *p;
        else if (auto* p = std::get_if<std::string>(&o.v)) v = *p;
    }
};

// Legge un documento JSON da una stringa. In caso di errore ok() e' falso.
class JsonReader {
public:
    explicit JsonReader(std::string testo) : testo_(std::move(testo)), pos_(0), ok_(false) {}

    bool parse() {
        skipWs();
        if (!leggiValore()) return false;
        skipWs();
        ok_ = (pos_ == testo_.size());
        return ok_;
    }

    bool ok() const { return ok_; }
    const JsonValue& valore() const { return valore_; }

private:
    bool leggiValore() {
        skipWs();
        char c = peek();
        if (c == '{') return leggiOggetto();
        if (c == '[') return leggiArray();
        if (c == '"') {
            std::string s;
            if (!leggiStringa(s)) return false;
            valore_ = std::move(s);
            return true;
        }
        if (c == 't' || c == 'f' || c == 'n') return leggiLiteral();
        if (c == '-' || (c >= '0' && c <= '9')) {
            double d;
            if (!leggiNumero(d)) return false;
            valore_ = d;
            return true;
        }
        return false;
    }

    bool leggiOggetto() {
        if (next() != '{') return false;
        JsonObject o;
        skipWs();
        if (peek() == '}') { next(); valore_ = std::move(o); return true; }
        for (;;) {
            skipWs();
            std::string chiave;
            if (peek() != '"' || !leggiStringa(chiave)) return false;
            skipWs();
            if (next() != ':') return false;
            if (!leggiValore()) return false;
            o.emplace(std::move(chiave), std::move(valore_));
            skipWs();
            char c = next();
            if (c == ',') continue;
            if (c == '}') { valore_ = std::move(o); return true; }
            return false;
        }
    }

    bool leggiArray() {
        if (next() != '[') return false;
        JsonArray a;
        skipWs();
        if (peek() == ']') { next(); valore_ = std::move(a); return true; }
        for (;;) {
            if (!leggiValore()) return false;
            a.push_back(std::move(valore_));
            skipWs();
            char c = next();
            if (c == ',') continue;
            if (c == ']') { valore_ = std::move(a); return true; }
            return false;
        }
    }

    bool leggiStringa(std::string& out) {
        if (next() != '"') return false;
        out.clear();
        for (;;) {
            if (pos_ >= testo_.size()) return false;
            char c = testo_[pos_++];
            if (c == '"') return true;
            if (c == '\\') {
                if (pos_ >= testo_.size()) return false;
                char e = testo_[pos_++];
                switch (e) {
                    case '"': out.push_back('"'); break;
                    case '\\': out.push_back('\\'); break;
                    case '/': out.push_back('/'); break;
                    case 'b': out.push_back('\b'); break;
                    case 'f': out.push_back('\f'); break;
                    case 'n': out.push_back('\n'); break;
                    case 'r': out.push_back('\r'); break;
                    case 't': out.push_back('\t'); break;
                    case 'u': {
                        if (pos_ + 4 > testo_.size()) return false;
                        u32 cp = 0;
                        for (i32 i = 0; i < 4; ++i) {
                            char h = testo_[pos_++];
                            cp <<= 4;
                            if (h >= '0' && h <= '9') cp |= (u32)(h - '0');
                            else if (h >= 'a' && h <= 'f') cp |= (u32)(h - 'a' + 10);
                            else if (h >= 'A' && h <= 'F') cp |= (u32)(h - 'A' + 10);
                            else return false;
                        }
                        if (cp < 0x80) out.push_back((char)cp);
                        else if (cp < 0x800) {
                            out.push_back((char)(0xC0 | (cp >> 6)));
                            out.push_back((char)(0x80 | (cp & 0x3F)));
                        } else if (cp < 0xD800 || cp > 0xDFFF) {
                            out.push_back((char)(0xE0 | (cp >> 12)));
                            out.push_back((char)(0x80 | ((cp >> 6) & 0x3F)));
                            out.push_back((char)(0x80 | (cp & 0x3F)));
                        } else {
                            out.push_back('?');
                        }
                        break;
                    }
                    default: return false;
                }
            } else if ((unsigned char)c < 0x20) {
                return false; // caratteri di controllo non escapati: vietati
            } else {
                out.push_back(c);
            }
        }
    }

    bool leggiNumero(double& out) {
        size_t start = pos_;
        if (peek() == '-') next();
        bool cifre = false;
        while (peek() >= '0' && peek() <= '9') { next(); cifre = true; }
        if (peek() == '.') {
            next();
            while (peek() >= '0' && peek() <= '9') { next(); cifre = true; }
        }
        if (peek() == 'e' || peek() == 'E') {
            next();
            if (peek() == '+' || peek() == '-') next();
            while (peek() >= '0' && peek() <= '9') { next(); cifre = true; }
        }
        if (!cifre) return false;
        char* end = nullptr;
        out = std::strtod(testo_.c_str() + start, &end);
        pos_ = (size_t)(end - testo_.c_str());
        return true;
    }

    bool leggiLiteral() {
        if (testo_.compare(pos_, 4, "true") == 0) { pos_ += 4; valore_ = true; return true; }
        if (testo_.compare(pos_, 5, "false") == 0) { pos_ += 5; valore_ = false; return true; }
        if (testo_.compare(pos_, 4, "null") == 0) { pos_ += 4; valore_ = JsonNull{}; return true; }
        return false;
    }

    void skipWs() {
        while (pos_ < testo_.size()) {
            char c = testo_[pos_];
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r') ++pos_;
            else break;
        }
    }

    char peek() const { return pos_ < testo_.size() ? testo_[pos_] : '\0'; }
    char next() { return pos_ < testo_.size() ? testo_[pos_++] : '\0'; }

    std::string testo_;
    size_t pos_;
    JsonValue valore_;
    bool ok_;
};

inline bool jsonGet(const JsonObject& o, const std::string& chiave, JsonValue& out) {
    auto it = o.find(chiave);
    if (it == o.end()) return false;
    out = it->second;
    return true;
}
inline bool jsonIsNumber(const JsonValue& jv) { return std::holds_alternative<double>(jv.v); }
inline bool jsonIsString(const JsonValue& jv) { return std::holds_alternative<std::string>(jv.v); }
inline bool jsonIsArray(const JsonValue& jv)  { return std::holds_alternative<std::unique_ptr<JsonArray>>(jv.v); }
inline bool jsonIsObject(const JsonValue& jv) { return std::holds_alternative<std::unique_ptr<JsonObject>>(jv.v); }
inline double jsonAsNumber(const JsonValue& jv) { return std::get<double>(jv.v); }
inline const std::string& jsonAsString(const JsonValue& jv) { return std::get<std::string>(jv.v); }
inline const JsonArray& jsonAsArray(const JsonValue& jv) { return *std::get<std::unique_ptr<JsonArray>>(jv.v); }
inline const JsonObject& jsonAsObject(const JsonValue& jv) { return *std::get<std::unique_ptr<JsonObject>>(jv.v); }

} // namespace sgl
