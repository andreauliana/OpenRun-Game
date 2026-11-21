// SPDX-License-Identifier: MIT
#include "core/replay.h"

namespace sgl {

void Replay::startRecord() {
    mode = Mode::Record;
    inputs.clear();
    inputs.reserve(4096); // niente allocazioni nel ciclo dopo questa riserva
}

void Replay::startPlayback(std::vector<u64> ins) {
    mode = Mode::Playback;
    inputs = std::move(ins);
    playbackIndex = 0;
}

void Replay::record(u64 bits) {
    // Nessuna riallocazione a regime: si scartano i frame oltre la capacita' prenotata.
    if (inputs.size() < inputs.capacity()) inputs.push_back(bits);
}

u64 Replay::next() {
    if (playbackIndex >= inputs.size()) return 0; // fine della registrazione: input nullo
    return inputs[playbackIndex++];
}

} // namespace sgl
