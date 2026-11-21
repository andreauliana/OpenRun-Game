// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

namespace sgl {

class Framebuffer;

// Segnale dei personaggi (Andrea 11/11): se il giocatore resta fermo in gara
// per qualche secondo, a destra (dove entreranno Gianni e Gesu') compaiono
// due cubetti che girano veloci, uno verde (Gianni) e uno blu (Gesu'), e
// sopra di loro un fumetto piccolo coi puntini che si scrivono "...".
// E' l'annuncio dei personaggi: versione piccola, da espandere quando
// arriveranno gli sprite veri di Andrea. Ricorda i rettangoli verde/blu
// segnaposto della prima versione (personaggio e fumetto), tolti il 06/11.
//
// Sovrimpressione a parte (non HUD, non auto): pixel art, palette corta,
// comparsa e uscita a trama Bayer. Deterministico, nessuna allocazione.
constexpr f32 SEGNALE_ATTESA_S = 3.0f;   // secondi da fermo prima del segnale
constexpr f32 SEGNALE_ENTRA_S = 0.6f;    // comparsa a trama
constexpr f32 SEGNALE_ESCE_S = 0.25f;    // ripartendo sparisce subito
constexpr f32 SEGNALE_FERMO_V = 0.5f;    // sotto questa velocita' l'auto e' ferma

// t = secondi da quando il segnale e' partito, k = 0..1 (comparsa/uscita).
void disegnaSegnalePersonaggi(Framebuffer& fb, f32 t, f32 k);

} // namespace sgl
