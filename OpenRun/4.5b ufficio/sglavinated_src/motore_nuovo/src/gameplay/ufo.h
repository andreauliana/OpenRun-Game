// SPDX-License-Identifier: MIT
#pragma once
#include <cmath>
#include "core/types.h"

// UFO: passaggio da destra a sinistra, MAI una frase (SPEC_GAMEPLAY §1).
// Riferimento v87: routine "apps", schedule (frame & 0x7FF) < 420. Nel nuovo
// motore l'UFO non tocca la VRAM e non serve la fuga a 12/24 frame della v86:
// resta solo l'intercorsione quando uno show deve partire (l'UFO esce).
// Qui ci sono SOLO stato e geometria per il renderer: nessun disegno.
// I tempi sono in SECONDI (indipendenti da 50/60 Hz).
//
namespace sgl::gameplay {

struct UfoConfig {
    f32 periodo  = 40.96f; // v87: 2048 frame a 50 Hz -> intervallo fra passaggi
    f32 finestra = 8.4f;   // v87: 420 frame a 50 Hz -> passaggio solo se dentro finestra
    f32 durata   = 6.8f;   // v87: 340 frame a 50 Hz -> durata passaggio (destra -> sinistra)
    f32 fuga     = 0.48f;  // secondi per uscire quando parte uno show (24 frame a 50 Hz)
};

struct DatiUfo {
    bool attivo = false;
    f32 progresso = 0.0f; // 0 = entra a destra, 1 = uscito a sinistra
    f32 yOffset  = 0.0f;  // ondulazione verticale (px), tabella tw della v87
};

class Ufo {
public:
    void setConfig(const UfoConfig& c) { cfg_ = c; }
    const UfoConfig& config() const { return cfg_; }

    void reset() {
        stato_ = Stato::Off;
        inizio_ = 0;
        dati_ = DatiUfo{};
    }

    // Chiamare una volta per frame. Se showAttivo diventa true durante un
    // passaggio, l'UFO esce in cfg_.fuga secondi e non riparte finche' lo show dura.
    void aggiorna(f32 tempoS, bool showAttivo) {
        if (stato_ == Stato::Passaggio) {
            if (showAttivo) {
                stato_ = Stato::Fuga;
                inizio_ = tempoS;
            } else if (tempoS - inizio_ >= cfg_.durata) {
                stato_ = Stato::Off;
            }
        } else if (stato_ == Stato::Fuga) {
            if (tempoS - inizio_ >= cfg_.fuga) stato_ = Stato::Off;
        } else if (!showAttivo && cfg_.periodo > 0 && cfg_.finestra > 0 &&
                   std::fmod(tempoS, cfg_.periodo) < cfg_.finestra) {
            stato_ = Stato::Passaggio;
            inizio_ = tempoS;
        }

        if (stato_ == Stato::Off) {
            dati_ = DatiUfo{};
            return;
        }
        const f32 dur = (stato_ == Stato::Fuga) ? cfg_.fuga : cfg_.durata;
        const f32 t = tempoS - inizio_;
        dati_.attivo = true;
        dati_.progresso = (dur == 0) ? 1.0f
            : clamp(static_cast<f32>(t) / static_cast<f32>(dur), 0.0f, 1.0f);
        // Ondulazione: tabella "tw" v87 (16 valori in px), 1 passo ogni 2 frame
        // (0.04 s a 50 Hz di riferimento).
        static const f32 TW[16] = {
            0, 3, 5, 7, 8, 7, 5, 3, 0, -3, -5, -7, -8, -7, -5, -3
        };
        dati_.yOffset = TW[(i32)(t / 0.04f) % 16];
    }

    bool attivo() const { return stato_ != Stato::Off; }
    const DatiUfo& dati() const { return dati_; }

private:
    enum class Stato : u8 { Off, Passaggio, Fuga };
    UfoConfig cfg_{};
    Stato stato_ = Stato::Off;
    f32 inizio_ = 0;
    DatiUfo dati_{};
};

} // namespace sgl::gameplay
