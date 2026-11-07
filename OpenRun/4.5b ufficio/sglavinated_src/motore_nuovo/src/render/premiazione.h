// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include <string>

namespace sgl {

class Framebuffer;
class Renderer;

// Premiazione all'arrivo (Andrea 07/11): quando l'auto si ferma sul traguardo
// parte una piccola festa intorno all'auto, sulla strada (niente scena a
// parte, Andrea 29/10). Cambia col tema della tappa d'arrivo e con la
// posizione: 1 = tutto (trofeo, effetti pieni), 2-3 = medaglia, effetti
// medi, 4+ = bandiera a scacchi, effetti leggeri.
// Pixel art: pochi colori per tema, aloni a trama Bayer (niente sfumature),
// tutto calcolato dal tempo: deterministico, nessuna allocazione.
enum class TemaPremio : i32 { Neon, Atomico, Foglie, Cromo, Spray, Desktop };

// Tema dall'id della tappa ("lv_04_cromo" -> Cromo). Sconosciuto -> Neon.
TemaPremio temaPremioDaLivello(const std::string& livelloId);

struct DatiPremiazione {
    TemaPremio tema = TemaPremio::Neon;
    i32 posizione = 1;   // posizione d'arrivo (1 = primo)
    f32 tempoS = -1.0f;  // secondi da quando l'auto e' ferma (< 0 = niente festa)
    i32 autoX = 160;     // centro dell'auto a schermo
    bool soloAmbiente = false; // sotto la classifica: niente scritta ne' trofeo
};

// Durata della festa prima della classifica (la festa continua sotto).
constexpr f32 PREMIAZIONE_S = 4.2f;

// Due strati: dietro l'auto (aloni, raggi, anelli) e davanti (oggetti
// che girano, trofeo, scritta). Il Game chiama il primo prima di
// drawPlayerCar e il secondo subito dopo.
void disegnaPremiazioneDietro(Framebuffer& fb, const DatiPremiazione& d);
void disegnaPremiazioneDavanti(Framebuffer& fb, Renderer& ren, const DatiPremiazione& d);

} // namespace sgl
