// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include <vector>
#include <string>

namespace sgl {

// Quello che la musica sta facendo adesso, letto dal player tracker a ogni
// frame: alimenta l'equalizzatore del menu e i tocchi a ritmo in gara
// (primo passo verso il visualizzatore interattivo).
struct InfoMusica {
    static constexpr i32 MAX_CANALI = 32;
    bool attiva = false;          // un brano .xm sta suonando
    const char* titolo = "";      // nome del brano (playlist.json o titolo del modulo)
    i32 canali = 0;               // canali del modulo
    i32 pattern = 0, riga = 0;    // posizione nel brano
    i32 bpm = 0;
    f32 battito = 0.0f;           // 1 sul primo tempo della battuta, poi cala
    f32 energia = 0.0f;           // volume medio della musica (0..1, smorzato)
    f32 livello[MAX_CANALI] = {}; // 1 quando un canale suona una nota, poi cala
    i32 nota[MAX_CANALI] = {};    // nota che il canale sta suonando (0..119), -1 = muto
};

// Mixer audio int16 stereo, deterministico.
// Suono motore sintetico + musica tracker (.xm di Andrea) suonata da
// libxmp-lite, inclusa nel progetto (terzeparti/): niente da installare.
class Mixer {
public:
    Mixer(u32 sampleRate = 44100);
    ~Mixer();
    Mixer(const Mixer&) = delete;
    Mixer& operator=(const Mixer&) = delete;

    // Aggiunge un brano tracker (.xm, .mod, .s3m) alla radio. Il file resta in
    // memoria; titolo vuoto = nome scritto nel modulo. false se manca o non si legge.
    bool aggiungiBrano(const std::string& path, const std::string& titolo);

    // Seleziona il brano corrente (indice 0-based) e lo fa ripartire. Fuori range: niente.
    void selezionaBrano(i32 indice);
    void nextBrano();

    // Effetto di un urto: botta secca; con clacson = il traffico suona (Andrea 29/10).
    void suonaUrto(bool clacson) { urtoN_ = 0; clacson_ = clacson; }

    // Riempie `out` (interleaved L/R, int16) per `frames` campioni stereo.
    void mix(i16* out, i32 frames, f32 engineRpm);

    void setEnabled(bool e) { enabled_ = e; }
    void setMusicVolume(f32 v) { musicVol_ = v; } // 0.0 - 1.0
    void setEngineVolume(f32 v) { engineVol_ = v; }

    i32 currentTrack() const { return curTrack_; }
    i32 numTracks() const { return (i32)brani_.size(); }
    const char* titoloBrano(i32 i) const;
    const InfoMusica& info() const { return info_; }

private:
    struct Brano {
        std::vector<u8> dati; // file .xm intero
        std::string titolo;
    };

    void avviaBrano();
    void aggiornaInfo(const i16* musica, i32 frames);

    u32 sampleRate_;        // output: 44100 Hz
    f32 phase_ = 0.0f;      // fase accumulata del suono motore
    bool enabled_ = true;
    f32 musicVol_ = 0.65f;
    f32 engineVol_ = 0.35f;

    void* xmp_ = nullptr;   // xmp_context (opaco: xmp.h resta nel .cpp)
    bool suona_ = false;
    std::vector<Brano> brani_;
    i32 curTrack_ = 0;
    std::vector<i16> bufMusica_;
    i32 urtoN_ = -1;        // campioni dall'ultimo urto (-1 = nessuno)
    bool clacson_ = false;
    u32 rumore_ = 0x1234567u;
    f32 sfxUrto();
    i32 rigaPrec_ = -1;
    InfoMusica info_;
};

} // namespace sgl
