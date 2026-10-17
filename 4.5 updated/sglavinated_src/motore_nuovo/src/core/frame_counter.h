// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

namespace sgl {

// Contatore di frame: l'unico "orologio" del motore.
// Nessun orologio di sistema entra nel core: determinismo e replay
// (stesso input = stesso risultato) ne dipendono.
class FrameClock {
public:
    u64 frame = 0; // frame corrente, parte da 0
    void tick() { ++frame; }
};

} // namespace sgl
