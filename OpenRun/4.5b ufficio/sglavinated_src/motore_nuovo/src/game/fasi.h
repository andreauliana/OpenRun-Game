// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "input/pad.h"
#include <memory>

namespace sgl {

class Game;
class Renderer;
class Framebuffer;

// Fase di gioco (blocco R2). Ogni fase espone entra/aggiorna/disegna/esce.
// Deterministica: niente orologi di sistema, durate in secondi e input solo via Pad.
enum class FaseId { Splash, Menu, Gara, FineTempo, Finale, Risultati };

class Fase {
public:
    virtual ~Fase() = default;
    virtual void entra(Game& gioco) = 0;
    virtual void aggiorna(Game& gioco, f32 dt) = 0;
    virtual void disegna(Renderer& ren, Framebuffer& fb) const = 0;
    virtual void esci(Game& gioco) = 0;
    virtual FaseId prossima() const = 0;
    virtual bool finita() const = 0;
    virtual const char* nome() const = 0;
};

// Gestore della macchina a stati: esegue la fase corrente e passa alla
// prossima quando quella corrente segnala finita(). Il runner non conosce le
// fasi: chiama solo Game::update()/Game::render() (blocco R2).
class GestoreFasi {
public:
    GestoreFasi();
    ~GestoreFasi();
    // Passa alla fase id. Se chiamata da dentro la fase corrente (es. il menu
    // che avvia la gara) il cambio e' rimandato alla fine di aggiorna(), cosi'
    // la fase che sta girando non viene distrutta a meta'.
    void cambia(FaseId id, Game& gioco);
    FaseId attuale() const { return attuale_; }
    void aggiorna(Game& gioco, f32 dt);
    void disegna(Renderer& ren, Framebuffer& fb) const;
private:
    std::unique_ptr<Fase> crea(FaseId id);
    std::unique_ptr<Fase> fase_;
    FaseId attuale_ = FaseId::Splash;
    bool inAggiorna_ = false;        // true mentre gira fase_->aggiorna()
    bool cambioRimandato_ = false;   // cambia() chiamata durante aggiorna()
    FaseId fasePendente_ = FaseId::Splash;
};

} // namespace sgl
