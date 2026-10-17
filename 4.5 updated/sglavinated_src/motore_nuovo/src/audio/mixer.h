// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include <vector>
#include <string>

namespace sgl {

// Descrizione di una radio: percorso al .bin, numero di banchi per brano.
struct RadioDef {
    std::string path;       // es. "shots/pcmr_M.bin"
    i32 banksPerTrack;      // 5 per radio M, 7 per radio A
    i32 sampleRate;         // ~12922 Hz (FSP di mkradio2.py)
};

// Mixer audio int16 stereo, deterministico.
// Genera suono motore sintetico + musica da banchi PCM 8-bit (.bin della hack).
// Nessun file Sega: i .bin contengono brani tracker di Andrea convertiti.
class Mixer {
public:
    static constexpr i32 BANK_SIZE = 32768; // byte per banco (mkradio2.py)

    Mixer(u32 sampleRate = 44100);

    // Carica una radio da file .bin (PCM 8-bit unsigned, mono, centrato su 128).
    // Ritorna il numero di brani trovati, o 0 se il file non esiste.
    i32 loadRadio(const RadioDef& def);

    // Seleziona il brano corrente (indice 0-based). Se fuori range, non fa nulla.
    void selezionaBrano(i32 indice);

    // Passa al brano successivo (wrap-around).
    void nextBrano();

    // Riempie `out` (interleaved L/R, int16) per `frames` campioni stereo.
    // Mixa motore sintetico + brano radio (se caricato).
    void mix(i16* out, i32 frames, f32 engineRpm);

    void setEnabled(bool e) { enabled_ = e; }
    void setMusicVolume(f32 v) { musicVol_ = v; } // 0.0 - 1.0
    void setEngineVolume(f32 v) { engineVol_ = v; }

    i32 currentTrack() const { return curTrack_; }
    i32 numTracks() const { return numTracks_; }

private:
    u32 sampleRate_;        // output: 44100 Hz
    f32 phase_ = 0.0f;     // fase accumulata del suono motore
    bool enabled_ = true;
    f32 musicVol_ = 0.65f;
    f32 engineVol_ = 0.35f;

    // Radio PCM
    std::vector<u8> radioData_;     // dati grezzi del .bin
    i32 radioRate_ = 12922;         // sample rate sorgente
    i32 banksPerTrack_ = 5;
    i32 numTracks_ = 0;
    i32 curTrack_ = 0;
    f32 playPos_ = 0.0f;           // posizione di lettura frazionaria (per upsample)
    bool radioLoaded_ = false;

    // Legge un campione dal brano corrente con interpolazione lineare (upsample).
    f32 readSample();
};

} // namespace sgl
