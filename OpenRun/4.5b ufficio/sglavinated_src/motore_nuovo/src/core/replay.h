// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include <vector>

namespace sgl {

// Registrazione / riproduzione degli input per i test di replay deterministico.
// Un frame = un bit-pack degli 8 tasti logici (tipo RetroPad).
class Replay {
public:
    enum class Mode : u8 { Off, Record, Playback };
    Mode mode = Mode::Off;
    std::vector<u64> inputs;    // destinatario (record) o sorgente (playback)
    u64 playbackIndex = 0;      // cursore in riproduzione

    void startRecord();
    void startPlayback(std::vector<u64> ins);
    void stop() { mode = Mode::Off; inputs.clear(); playbackIndex = 0; }
    void record(u64 bits);
    u64 next(); // playback: input del frame corrente
    bool active() const { return mode != Mode::Off; }
};

} // namespace sgl
