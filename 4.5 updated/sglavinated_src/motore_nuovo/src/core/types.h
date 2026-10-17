// SPDX-License-Identifier: MIT
#pragma once

// sgl — nucleo del motore "Sglavinated", C++17, indipendente dalla piattaforma.
// Tecniche di riferimento (solo come idee, nessun codice copiato):
//   L. Gorenfeld, "Lou's Pseudo 3d Page", https://www.extentofthejam.com/pseudo/
//     (c) 2013, nessuna licenza dichiarata: letta come riferimento tecnico,
//     nessun testo o immagine riusato.
//   Jake Gordon, "JavaScript Racer", https://jakesgordon.com/writing/javascript-racer/
//     tutorial pubblico; licenza del codice non dichiarata: solo ispirazione.

#include <cstdint>

namespace sgl {

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using f32 = float;
using f64 = double;

inline constexpr f32 PI = 3.14159265358979323846f;

template <typename T>
inline constexpr T clamp(T v, T lo, T hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

} // namespace sgl
