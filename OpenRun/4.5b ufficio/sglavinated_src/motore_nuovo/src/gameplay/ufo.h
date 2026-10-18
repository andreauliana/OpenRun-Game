// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"

// UFO: passaggio da destra a sinistra, MAI una frase (SPEC_GAMEPLAY §1).
// Riferimento v87: routine "apps", schedule (frame & 0x7FF) < 420. Nel nuovo
// motore l'UFO non tocca la VRAM e non serve la fuga a 12/24 frame della v86:
// resta solo l'intercorsione quando uno show deve partire (l'UFO esce).
// Qui ci sono SOLO stato e geometria per il renderer: nessun disegno.

namespace sgl::gameplay {

struct UfoConfig {
    u32 periodo  = 2048; // v87: frame & 0x7FF -> 2048 frame fra un passaggio e l'altro
    u32 finestra = 420;  // v87: il passaggio parte solo se frame % periodo < 420
    u32 durata   = 340;  // frame del passaggio completo (destra -> sinistra)
    u32 fuga     = 24;   // frame per uscire quando parte uno show
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
    // passaggio, l'UFO esce in cfg_.fuga frame e non riparte finche' lo show dura.
    void aggiorna(u64 frame, bool showAttivo) {
        if (stato_ == Stato::Passaggio) {
            if (showAttivo) {
                stato_ = Stato::Fuga;
                inizio_ = frame;
            } else if (frame - inizio_ >= cfg_.durata) {
                stato_ = Stato::Off;
            }
        } else if (stato_ == Stato::Fuga) {
            if (frame - inizio_ >= cfg_.fuga) stato_ = Stato::Off;
        } else if (!showAttivo && cfg_.periodo > 0 && cfg_.finestra > 0 &&
                   (frame % cfg_.periodo) < cfg_.finestra) {
            stato_ = Stato::Passaggio;
            inizio_ = frame;
        }

        if (stato_ == Stato::Off) {
            dati_ = DatiUfo{};
            return;
        }
        const u32 dur = (stato_ == Stato::Fuga) ? cfg_.fuga : cfg_.durata;
        const u64 t = frame - inizio_;
        dati_.attivo = true;
        dati_.progresso = (dur == 0) ? 1.0f
            : clamp(static_cast<f32>(t) / static_cast<f32>(dur), 0.0f, 1.0f);
        // Ondulazione: tabella "tw" v87 (16 valori in px), 1 passo ogni 2 frame.
        static const f32 TW[16] = {
            0, 3, 5, 7, 8, 7, 5, 3, 0, -3, -5, -7, -8, -7, -5, -3
        };
        dati_.yOffset = TW[(t / 2) % 16];
    }

    bool attivo() const { return stato_ != Stato::Off; }
    const DatiUfo& dati() const { return dati_; }

private:
    enum class Stato : u8 { Off, Passaggio, Fuga };
    UfoConfig cfg_{};
    Stato stato_ = Stato::Off;
    u64 inizio_ = 0;
    DatiUfo dati_{};
};

} // namespace sgl::gameplay
