// SPDX-License-Identifier: MIT
#include "audio/mixer.h"
#include "xmp.h"
#include <cmath>
#include <cstdio>
#include <algorithm>

namespace sgl {

namespace {
xmp_context ctx(void* p) { return static_cast<xmp_context>(p); }

// Secondi per far scendere a ~1/e livelli e battito: i canali a lungo, il battito corto.
constexpr f32 DECADIMENTO_CANALE_S = 0.18f;
constexpr f32 DECADIMENTO_BATTITO_S = 0.12f;
constexpr i32 RIGHE_PER_BATTITO = 4; // convenzione tracker: un quarto ogni 4 righe

// Musica che reagisce (Andrea 07/11). Tutto dal vivo sul .xm: il canale si
// spegne col volume di canale di libxmp, tono e filtro sono interpolazioni
// del segnale gia' mixato. Costo: poche moltiplicazioni a campione.
constexpr f32 MUTE_SPEGNI_S = 0.03f;       // dissolvenza in uscita del canale
constexpr f32 MUTE_RIENTRO_S = 0.10f;      // dissolvenza in entrata
constexpr f32 MUTE_LIVELLO_SUONA = 0.15f;  // InfoMusica::livello sopra cui un canale "sta suonando"
constexpr f32 OVATTATO_FC_PIENA = 18000.0f; // Hz: filtro aperto
constexpr f32 OVATTATO_FC_CHIUSA = 650.0f;  // Hz: tutto ovattato
constexpr f32 OVATTATO_TEMPO_S = 0.12f;     // quanto ci mette ad aprirsi/chiudersi
constexpr f32 STRIDIO_VOLUME = 3600.0f;     // stridio pieno: si sente sopra il motore, sotto la musica
} // namespace

Mixer::Mixer(u32 sampleRate) : sampleRate_(sampleRate) {}

Mixer::~Mixer() {
    if (!xmp_) return;
    if (suona_) { xmp_end_player(ctx(xmp_)); xmp_release_module(ctx(xmp_)); }
    xmp_free_context(ctx(xmp_));
}

bool Mixer::aggiungiBrano(const std::string& path, const std::string& titolo) {
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return false;
    Brano b;
    std::fseek(f, 0, SEEK_END);
    const long sz = std::ftell(f);
    std::fseek(f, 0, SEEK_SET);
    if (sz > 0) {
        b.dati.resize((size_t)sz);
        if (std::fread(b.dati.data(), 1, (size_t)sz, f) != (size_t)sz) b.dati.clear();
    }
    std::fclose(f);
    if (b.dati.empty()) return false;

    if (!xmp_) xmp_ = xmp_create_context();
    if (!xmp_) return false;
    // Prova a leggerlo subito: un file rotto non entra nella radio.
    struct xmp_test_info ti;
    if (xmp_test_module_from_memory(b.dati.data(), (long)b.dati.size(), &ti) != 0) return false;
    b.titolo = titolo.empty() ? std::string(ti.name) : titolo;
    for (char& c : b.titolo) if (c >= 'a' && c <= 'z') c = (char)(c - 'a' + 'A'); // font HUD maiuscolo
    brani_.push_back(std::move(b));
    if (brani_.size() == 1) { curTrack_ = 0; avviaBrano(); }
    return true;
}

const char* Mixer::titoloBrano(i32 i) const {
    return (i >= 0 && i < numTracks()) ? brani_[(size_t)i].titolo.c_str() : "";
}

void Mixer::avviaBrano() {
    if (!xmp_ || brani_.empty()) return;
    if (suona_) { xmp_end_player(ctx(xmp_)); xmp_release_module(ctx(xmp_)); suona_ = false; }
    info_ = InfoMusica{};
    rigaPrec_ = -1;
    sorgente_.clear();
    letturaPos_ = 0.0;
    muteCanale_ = -1;
    muteVol_ = 100.0f;
    muteRientro_ = false;
    const Brano& b = brani_[(size_t)curTrack_];
    if (xmp_load_module_from_memory(ctx(xmp_), b.dati.data(), (long)b.dati.size()) != 0) return;
    if (xmp_start_player(ctx(xmp_), (int)sampleRate_, 0) != 0) { xmp_release_module(ctx(xmp_)); return; }
    xmp_set_player(ctx(xmp_), XMP_PLAYER_INTERP, XMP_INTERP_SPLINE);
    xmp_set_player(ctx(xmp_), XMP_PLAYER_AMP, 2); // default 1: troppo bassa sotto il motore
    suona_ = true;
    struct xmp_module_info mi;
    xmp_get_module_info(ctx(xmp_), &mi);
    info_.attiva = true;
    info_.titolo = b.titolo.c_str();
    info_.canali = std::min(mi.mod->chn, InfoMusica::MAX_CANALI);
}

void Mixer::selezionaBrano(i32 indice) {
    if (indice < 0 || indice >= numTracks()) return;
    curTrack_ = indice;
    avviaBrano();
}

void Mixer::aggiornaInfo(const i16* musica, i32 frames) {
    struct xmp_frame_info fi;
    xmp_get_frame_info(ctx(xmp_), &fi);
    const f32 dt = (f32)frames / (f32)sampleRate_;
    const f32 kCanale = std::exp(-dt / DECADIMENTO_CANALE_S);
    const f32 kBattito = std::exp(-dt / DECADIMENTO_BATTITO_S);

    info_.pattern = fi.pattern;
    info_.riga = fi.row;
    info_.bpm = fi.bpm;
    info_.battito *= kBattito;
    // Una nota tenuta resta accesa a meta' (in proporzione al volume del canale).
    for (i32 c = 0; c < info_.canali; ++c) {
        const struct xmp_channel_info& ci = fi.channel_info[c];
        info_.livello[c] = std::max(info_.livello[c] * kCanale, 0.45f * (f32)ci.volume / 64.0f);
        info_.nota[c] = (ci.volume > 0 && ci.note < XMP_MAX_KEYS) ? (i32)ci.note : -1;
    }

    // Riga nuova: ogni canale che attacca una nota si accende al suo volume.
    if (fi.row != rigaPrec_) {
        rigaPrec_ = fi.row;
        if (fi.row % RIGHE_PER_BATTITO == 0) info_.battito = 1.0f;
        for (i32 c = 0; c < info_.canali; ++c) {
            const struct xmp_channel_info& ci = fi.channel_info[c];
            if (ci.event.note > 0 && ci.event.note <= XMP_MAX_KEYS)
                info_.livello[c] = std::max(info_.livello[c], 0.35f + 0.65f * (f32)ci.volume / 64.0f);
        }
    }

    // Energia: valore medio assoluto del blocco, smorzato.
    f32 somma = 0.0f;
    for (i32 i = 0; i < frames * 2; ++i) somma += std::fabs((f32)musica[i]);
    const f32 e = std::min(1.0f, somma / (f32)(frames * 2) / 8000.0f);
    info_.energia = e > info_.energia ? e : info_.energia * kCanale + e * (1.0f - kCanale);
}

void Mixer::scuotiMusica(f32 forza) {
    if (!suona_) return;
    forza = std::clamp(forza, 0.0f, 1.0f);

    // 1) Calo di tono: un urto piu' forte di quello in corso lo fa ripartire
    //    dalla velocita' attuale del nastro (niente scatti).
    if (tonoT_ < 0.0f || forza >= tonoForza_) {
        tonoR0_ = tonoT_ < 0.0f ? 1.0f : rapportoTono(tonoT_);
        tonoT_ = 0.0f;
        tonoForza_ = forza;
        const f32 f2 = forza * forza;
        tonoDurata_ = (0.05f + 0.40f * f2 * f2) + (0.10f + 0.25f * f2 * f2) + (0.45f + 0.45f * f2);
    }

    // 2) Un canale a caso si spegne. Solo tra quelli che stanno suonando (se
    //    no non si sente) e mai il piu' forte, cosi' il ritmo resta. Se uno e'
    //    gia' spento, resta lui e dura di piu'.
    const f32 durata = 0.9f + 0.9f * forza;
    if (muteCanale_ >= 0 && !muteRientro_) { muteS_ = std::max(muteS_, durata); return; }
    i32 forte = -1;
    for (i32 c = 0; c < info_.canali; ++c)
        if (forte < 0 || info_.livello[c] > info_.livello[forte]) forte = c;
    i32 candidati[InfoMusica::MAX_CANALI];
    i32 n = 0;
    for (i32 c = 0; c < info_.canali; ++c)
        if (c != forte && info_.livello[c] > MUTE_LIVELLO_SUONA) candidati[n++] = c;
    if (n == 0) return;
    caso_ = caso_ * 1664525u + 1013904223u;
    const i32 scelto = candidati[(caso_ >> 16) % (u32)n];
    if (muteCanale_ >= 0 && muteCanale_ != scelto) xmp_channel_vol(ctx(xmp_), muteCanale_, 100);
    if (muteCanale_ != scelto) muteVol_ = 100.0f;
    muteCanale_ = scelto;
    muteS_ = durata;
    muteRientro_ = false;
}

f32 Mixer::rapportoTono(f32 t) const {
    // Nastro schiacciato: rallenta (tono e tempo scendono insieme), resta giu'
    // un attimo e torna con un tremolio. A forza 1 (incidente) e' quasi un
    // "tape stop". Tempi in secondi.
    const f32 f = tonoForza_;
    const f32 f2 = f * f, f4 = f2 * f2;
    const f32 fondo = 1.0f - (0.15f + 0.70f * f2 * f); // 0.45 -> 0.79, 0.7 -> 0.61, 1 -> 0.15
    const f32 attacco = 0.05f + 0.40f * f4;
    const f32 tenuta = 0.10f + 0.25f * f4;
    const f32 rilascio = 0.45f + 0.45f * f2;
    auto morbido = [](f32 x) { return x * x * (3.0f - 2.0f * x); };
    if (t < attacco) return tonoR0_ + (fondo - tonoR0_) * morbido(t / attacco);
    t -= attacco;
    if (t < tenuta) return fondo;
    t -= tenuta;
    if (t < rilascio) {
        const f32 x = t / rilascio;
        const f32 tremolio = 0.025f * f * std::sin(2.0f * PI * 7.0f * t) * (1.0f - x);
        return fondo + (1.0f - fondo) * morbido(x) + tremolio;
    }
    return 1.0f;
}

void Mixer::aggiornaMute(f32 dt) {
    if (muteCanale_ < 0) return;
    if (!muteRientro_) {
        muteVol_ = std::max(0.0f, muteVol_ - 100.0f * dt / MUTE_SPEGNI_S);
        muteS_ -= dt;
        if (muteS_ <= 0.0f && muteRiga_ < 0) muteRiga_ = info_.riga;
        // Torna su a riga nuova del brano, cosi' rientra a tempo.
        if (muteRiga_ >= 0 && info_.riga != muteRiga_) { muteRientro_ = true; muteRiga_ = -1; }
    } else {
        muteVol_ += 100.0f * dt / MUTE_RIENTRO_S;
        if (muteVol_ >= 100.0f) {
            xmp_channel_vol(ctx(xmp_), muteCanale_, 100);
            muteCanale_ = -1;
            muteVol_ = 100.0f;
            muteRientro_ = false;
            return;
        }
    }
    xmp_channel_vol(ctx(xmp_), muteCanale_, (int)muteVol_);
}

void Mixer::renderMusica(i32 frames) {
    bufMusica_.resize((size_t)frames * 2);
    // loop 0: il brano ricomincia da capo all'infinito, come la radio di OutRun.
    if (xmp_play_buffer(ctx(xmp_), bufMusica_.data(), frames * 4, 0) != 0)
        std::fill(bufMusica_.begin(), bufMusica_.end(), (i16)0);
    else
        aggiornaInfo(bufMusica_.data(), frames);
    sorgente_.insert(sorgente_.end(), bufMusica_.begin(), bufMusica_.end());
}

f32 Mixer::sfxUrto() {
    // Botta: rumore e tonfo grave che si spengono in un attimo. Col traffico
    // segue il clacson (due note in onda quadra); coi rivali un colpo di lamiera.
    const f32 t = (f32)urtoN_ / (f32)sampleRate_;
    rumore_ = rumore_ * 1664525u + 1013904223u;
    const f32 n = (f32)((rumore_ >> 16) & 0xFFFFu) / 32768.0f - 1.0f;
    f32 s = (n * 9000.0f + 7000.0f * std::sin(2.0f * PI * 70.0f * t)) * std::exp(-t / 0.08f);
    if (clacson_) {
        if (t > 0.08f && t < 0.6f) {
            const f32 inv = std::min(1.0f, (t - 0.08f) / 0.01f) * std::min(1.0f, (0.6f - t) / 0.04f);
            const f32 a = std::fmod(t * 392.0f, 1.0f) < 0.5f ? 1.0f : -1.0f;
            const f32 b = std::fmod(t * 494.0f, 1.0f) < 0.5f ? 1.0f : -1.0f;
            s += inv * 2600.0f * (a + b);
        }
    } else {
        s += 3500.0f * std::sin(2.0f * PI * 880.0f * t) * std::exp(-t / 0.05f);
    }
    if (++urtoN_ > (i32)(0.6f * (f32)sampleRate_)) urtoN_ = -1;
    return s;
}

// Bip della partenza: onda quadra con un attacco breve (niente click) e una
// coda che cala. 3, 2, 1: La (880 Hz) corto; START: ottava sopra, piu' lungo.
f32 Mixer::sfxBip() {
    const f32 t = (f32)bipN_ / (f32)sampleRate_;
    const f32 durata = bipVia_ ? 0.55f : 0.16f;
    const f32 f = bipVia_ ? 1760.0f : 880.0f;
    const f32 onda = std::fmod(t * f, 1.0f) < 0.5f ? 1.0f : -1.0f;
    const f32 inv = std::min(1.0f, t / 0.004f) * std::min(1.0f, (durata - t) / 0.03f) *
                    (bipVia_ ? std::exp(-t / 0.35f) : 1.0f);
    if (++bipN_ > (i32)(durata * (f32)sampleRate_)) bipN_ = -1;
    return 4200.0f * onda * std::max(0.0f, inv);
}

// V47 V1: sorpasso al pelo, blip breve e leggero (effetto nuovo).
f32 Mixer::sfxSorpasso() {
    const f32 t = (f32)sorpassoN_ / (f32)sampleRate_;
    const f32 durata = 0.09f;
    const f32 f = 1320.0f + 660.0f * (t / durata); // sweep breve verso l'alto
    const f32 onda = std::sin(2.0f * PI * f * t);
    const f32 inv = std::min(1.0f, t / 0.003f) * std::min(1.0f, (durata - t) / 0.02f);
    if (++sorpassoN_ > (i32)(durata * (f32)sampleRate_)) sorpassoN_ = -1;
    return 1800.0f * onda * std::max(0.0f, inv);
}

// MIG1 D5: paletto abbattuto che vola (effetto nuovo, in aggiunta).
f32 Mixer::sfxPaletto() {
    const f32 t = (f32)palettoN_ / (f32)sampleRate_;
    const f32 durata = 0.18f;
    rumore_ = rumore_ * 1664525u + 1013904223u;
    const f32 n = (f32)((rumore_ >> 16) & 0xFFFFu) / 32768.0f - 1.0f;
    const f32 inv = std::min(1.0f, t / 0.004f) * std::min(1.0f, (durata - t) / 0.06f);
    const f32 f = 520.0f - 220.0f * (t / durata); // colpo che scende di tono
    const f32 s = (2200.0f * std::sin(2.0f * PI * f * t) + 1200.0f * n) * std::exp(-t / 0.07f);
    if (++palettoN_ > (i32)(durata * (f32)sampleRate_)) palettoN_ = -1;
    return s * std::max(0.0f, inv);
}

// Stridio delle gomme: due toni acuti (circa 1.2 e 1.75 kHz) che salgono e
// scendono un po' a caso, con un tremolo veloce e un filo di fruscio. Volume
// sotto il motore pieno; deterministico (generatore interno, niente orologi).
f32 Mixer::sfxStridio() {
    const f32 sr = (f32)sampleRate_;
    rumore_ = rumore_ * 1664525u + 1013904223u;
    const f32 n = (f32)(i32)(rumore_ >> 8 & 0xFFFF) / 32768.0f - 1.0f;
    stridioDeriva_ += (n * 0.5f - stridioDeriva_) * 0.0004f;
    stridioRumore_ += (n - stridioRumore_) * 0.35f;
    const f32 f0 = 1180.0f * (1.0f + 0.35f * stridioDeriva_);
    stridioFase_[0] += f0 / sr;
    stridioFase_[1] += f0 * 1.47f / sr;
    stridioFase_[2] += 23.0f / sr;
    for (f32& f : stridioFase_) if (f > 1.0f) f -= 1.0f;
    const f32 trem = 0.7f + 0.3f * std::sin(2.0f * PI * stridioFase_[2]);
    const f32 tono = 0.6f * std::sin(2.0f * PI * stridioFase_[0]) + 0.4f * std::sin(2.0f * PI * stridioFase_[1]);
    return STRIDIO_VOLUME * stridio_ * (tono * trem + 0.3f * stridioRumore_);
}

void Mixer::mix(i16* out, i32 frames, f32 engineRpm) {
    if (!enabled_) {
        for (i32 i = 0; i < frames * 2; ++i) out[i] = 0;
        return;
    }

    const f32 dt = (f32)frames / (f32)sampleRate_;
    const f32 dtCampione = 1.0f / (f32)sampleRate_;
    const bool musica = suona_ && !musicaFerma_;
    if (musica) {
        aggiornaMute(dt);
        // Il nastro corre al massimo a ~1.03 (tremolio): basta avere davanti
        // frames * 1.05 campioni di musica, piu' due per l'interpolazione.
        const i32 servono = (i32)std::ceil(letturaPos_ + (f64)frames * 1.05) + 2;
        const i32 ci = (i32)(sorgente_.size() / 2);
        if (servono > ci) renderMusica(servono - ci);
    }
    // Filtro "ovattato": si apre e si chiude in modo graduale.
    ovattato_ += (ovattatoVoluto_ - ovattato_) * (1.0f - std::exp(-dt / OVATTATO_TEMPO_S));
    const bool filtra = ovattato_ > 0.002f;
    const f32 fc = OVATTATO_FC_PIENA * std::pow(OVATTATO_FC_CHIUSA / OVATTATO_FC_PIENA, ovattato_);
    const f32 aFiltro = 1.0f - std::exp(-2.0f * PI * fc / (f32)sampleRate_);

    // Suono motore: due oscillatori con fase accumulata (niente click tra frame).
    f32 base = 40.0f + engineRpm * 0.6f; // Hz fondamentale
    f32 amp = 3000.0f * (engineRpm / 8000.0f);
    if (amp > 12000.0f) amp = 12000.0f;

    f32 phaseInc = base / (f32)sampleRate_;
    const f32 aStridio = 1.0f - std::exp(-1.0f / (0.06f * (f32)sampleRate_)); // ~60 ms per salire o scendere

    for (i32 i = 0; i < frames; ++i) {
        f32 eng = amp * (std::sin(2.0f * PI * phase_) +
                         0.4f * std::sin(2.0f * PI * phase_ * 2.0f));
        f32 l = eng * engineVol_, r = l;
        if (urtoN_ >= 0) { const f32 u = sfxUrto(); l += u; r += u; }
        stridio_ += (stridioVoluto_ - stridio_) * aStridio;
        if (stridio_ > 0.002f) { const f32 st = sfxStridio(); l += st; r += st; }
        if (bipN_ >= 0) { const f32 b = sfxBip(); l += b; r += b; }
        if (sorpassoN_ >= 0) { const f32 sp = sfxSorpasso(); l += sp; r += sp; }
        if (palettoN_ >= 0) { const f32 pl = sfxPaletto(); l += pl; r += pl; }
        if (musica) {
            // Lettura del nastro con interpolazione lineare tra due campioni.
            const f32 passo = tonoT_ >= 0.0f ? rapportoTono(tonoT_) : 1.0f;
            const size_t i0 = (size_t)letturaPos_;
            const f32 fr = (f32)(letturaPos_ - (f64)i0);
            f32 ml = 0.0f, mr = 0.0f;
            if ((i0 + 1) * 2 + 1 < sorgente_.size()) {
                ml = (f32)sorgente_[i0 * 2] + ((f32)sorgente_[i0 * 2 + 2] - (f32)sorgente_[i0 * 2]) * fr;
                mr = (f32)sorgente_[i0 * 2 + 1] + ((f32)sorgente_[i0 * 2 + 3] - (f32)sorgente_[i0 * 2 + 1]) * fr;
            }
            letturaPos_ += (f64)passo;
            if (tonoT_ >= 0.0f) {
                tonoT_ += dtCampione;
                if (tonoT_ >= tonoDurata_) tonoT_ = -1.0f;
            }
            if (filtra) {
                filtro_[0] += aFiltro * (ml - filtro_[0]);
                filtro_[1] += aFiltro * (mr - filtro_[1]);
                filtro_[2] += aFiltro * (filtro_[0] - filtro_[2]);
                filtro_[3] += aFiltro * (filtro_[1] - filtro_[3]);
                ml = filtro_[2]; mr = filtro_[3];
            } else {
                filtro_[0] = filtro_[2] = ml;
                filtro_[1] = filtro_[3] = mr;
            }
            l += ml * musicVol_;
            r += mr * musicVol_;
        }
        out[i * 2]     = (i16)std::max(-32000.0f, std::min(32000.0f, l));
        out[i * 2 + 1] = (i16)std::max(-32000.0f, std::min(32000.0f, r));

        phase_ += phaseInc;
        if (phase_ > 1.0f) phase_ -= 1.0f;
    }

    // Butta la musica gia' letta (tiene il campione corrente per l'interpolazione).
    if (musica) {
        const size_t letti = std::min((size_t)letturaPos_, sorgente_.size() / 2);
        sorgente_.erase(sorgente_.begin(), sorgente_.begin() + (std::ptrdiff_t)(letti * 2));
        letturaPos_ -= (f64)letti;
    }
}

} // namespace sgl
