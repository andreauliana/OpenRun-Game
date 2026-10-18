// SPDX-License-Identifier: MIT
#include "gameplay/show.h"

// Motore degli show — vedi SPEC_GAMEPLAY.md §1 (trigger e priorita'):
//   crash > off-road in attesa > periodico > inattivita'; UFO senza frase.
// Nessun vincolo hardware della v87 (156 fumetti, VRAM, 88 px): qui servono
// solo regole di comportamento e dati deterministici.

namespace sgl::gameplay {

void ShowManager::reset(u32 seed) {
    cfg_ = ShowConfig{}; // default; setConfig() va richiamato dopo il reset
    esc_.reset();
    sel_.reset(seed);
    ufo_.reset();
    show_ = DatiShow{};
    frame_ = 0;
    frameGaraInizio_ = 0;
    frameStage_ = 0;
    ultimoShow_ = 0;
    offRoadFrames_ = 0;
    idleFrames_ = 0;
    offRoadPending_ = false;
    frameOffRoadAttesa_ = 0;
    crashPending_ = false;
    inattivitaArmata_ = false;
    garaVista_ = false;
    rotRampa_ = 0;
}

void ShowManager::cambiaStage() {
    esc_.reset();
    chiudiShow();
    offRoadFrames_ = 0;
    idleFrames_ = 0;
    offRoadPending_ = false;
    frameOffRoadAttesa_ = 0;
    crashPending_ = false;
    inattivitaArmata_ = false;
    rotRampa_ = 0;
    frameStage_ = frame_;
    ultimoShow_ = frame_;
}

void ShowManager::crash() { crashPending_ = true; }

void ShowManager::avviaShow(u64 frame, Personaggio p, OrigineShow o, u32 idFrase) {
    show_.attivo = true;
    show_.personaggio = p;
    show_.origine = o;
    show_.idFrase = idFrase;
    show_.frameInizio = frame;
    show_.durata = cfg_.durataShow;
    ultimoShow_ = frame;
}

void ShowManager::chiudiShow() { show_ = DatiShow{}; }

bool ShowManager::bollaVisibile() const {
    if (!show_.attivo) return false;
    if (frame_ - show_.frameInizio < cfg_.bollaAppare) return false;
    const u32 rimasti = show_.frameRimasti(frame_);
    const u32 nascondi = (show_.personaggio == Personaggio::Gesu)
                             ? cfg_.bollaNascondiGesu
                             : cfg_.bollaNascondiGianni;
    return rimasti >= nascondi;
}

u32 ShowManager::fraseCrash() {
    const i32 liv = esc_.livello();
    if (liv <= 0) return sel_.scegli(TabId::Xtab, tabelle::xtab, 32);
    switch (liv) {
        case 1:  return sel_.scegli(TabId::J1, tabelle::j1t, 6);
        case 2:  return sel_.scegli(TabId::J2, tabelle::j2t, 6);
        default: return sel_.scegli(TabId::J3, tabelle::j3t, 6);
    }
}

// Rampa del personaggio agli show periodici (SPEC_GAMEPLAY §3): contatori a
// rotazione, probabilita' esatte a ciclo chiuso (come VAR184 nella v87).
bool ShowManager::parlaGesuPeriodico(i32 livello) {
    switch (livello) {
        case 0:  return false;                             // Gianni sempre
        case 1:  return (rotRampa_++ & 1u) != 0u;          // Gesu' 1 su 2
        case 2:  return (rotRampa_++ & 3u) != 1u;          // Gesu' 3 su 4
        default: return true;                              // Gesu' sempre
    }
}

void ShowManager::frasePeriodica(i32 livello, Personaggio& p, TabId& tab,
                                 const u32*& ids, u32& n) {
    if (parlaGesuPeriodico(livello)) {
        p = Personaggio::Gesu;
        switch (livello) {
            case 1:  tab = TabId::J1; ids = tabelle::j1t; n = 6; break;
            case 2:  tab = TabId::J2; ids = tabelle::j2t; n = 6; break;
            case 3:  tab = TabId::J3; ids = tabelle::j3t; n = 6; break;
            default: tab = TabId::J;  ids = tabelle::jtab; n = 32; break;
        }
    } else {
        p = Personaggio::Gianni;
        switch (livello) {
            case 1:  tab = TabId::G1; ids = tabelle::g1t; n = 6; break;
            case 2:  tab = TabId::G2; ids = tabelle::g2t; n = 6; break;
            case 3:  tab = TabId::G3; ids = tabelle::g3t; n = 6; break;
            default: tab = TabId::Ph; ids = tabelle::ptab; n = 32; break;
        }
    }
}

const DatiShow& ShowManager::aggiorna(const StatoGameplayInput& in) {
    frame_ = in.frame;

    // 1) Decadimento escalation (20 s puliti -> -1 errore).
    esc_.aggiorna(in.frame, cfg_.decadimentoErrori);

    // 2) Inizio gara: ci serve per la guardia minGaraFrame.
    if (in.inGara && !garaVista_) {
        frameGaraInizio_ = in.frame;
        garaVista_ = true;
    } else if (!in.inGara) {
        garaVista_ = false;
    }

    // 3) Fine show per durata.
    if (show_.attivo && in.frame - show_.frameInizio >= show_.durata) chiudiShow();

    // 4) SBORDATA (v85c): off-road in attesa fa chiudere uno show periodico
    //    che ha gia' fatto leggere il fumetto; gli show reattivi non si tagliano.
    if (offRoadPending_ && show_.attivo &&
        show_.origine == OrigineShow::Periodico &&
        in.frame - show_.frameInizio >= cfg_.offRoadTagliaPeriodico) {
        chiudiShow();
    }

    // 5) Rilevamento off-road: >= soglia frame di fila scatta una volta l'evento.
    //    Ignorato se un crash e' pendente (v87). Non conta come errore subito:
    //    lo decide Escalation (una uscita su due).
    if (in.inGara && in.offRoad && !crashPending_) {
        if (offRoadFrames_ < 0xFFFFFFFFu) offRoadFrames_++;
        if (offRoadFrames_ == cfg_.offRoadSoglia) {
            offRoadPending_ = true;
            frameOffRoadAttesa_ = in.frame; // attesa 250 frame prima di servire (v85c SBORDATA)
            esc_.notificaUscitaStrada(in.frame);
            idleFrames_ = 0; // "nessun off-road" vale anche per l'inattivita'
        }
    } else if (!in.offRoad) {
        offRoadFrames_ = 0;
    }

    // 6) Inattivita': 10 s senza tasti in gara; una volta per pausa (si riarma
    //    solo con un input). Il fuoco avviene sotto, con tutte le guardie.
    if (!in.inGara) {
        idleFrames_ = 0;
    } else if (in.inputAttivo) {
        idleFrames_ = 0;
        inattivitaArmata_ = true;
    } else if (idleFrames_ < 0xFFFFFFFFu) {
        idleFrames_++;
    }

    // 7) Crash: priorita' massima, sostituisce qualsiasi show (Gesu' interrompe
    //    chiunque). Fuori gara viene ignorato (menu/demo intatti, regola v87).
    if (crashPending_) {
        crashPending_ = false;
        if (in.inGara) {
            offRoadPending_ = false;
            offRoadFrames_ = 0;
            avviaShow(in.frame, Personaggio::Gesu, OrigineShow::Crash, fraseCrash());
        }
        ufo_.aggiorna(in.frame, show_.attivo);
        return show_;
    }

    // 8) Nessun show a schermo, gara in corso, guardie di partenza OK.
    if (!show_.attivo && in.inGara && prontoPerShow(in.frame) && !ufo_.attivo()) {
        // Con un evento off-road in attesa il cooldown di 24 frame non vale (v87).
        const bool cooldownOk = (in.frame - ultimoShow_) >= cfg_.cooldownShow;
        if (offRoadPending_ &&
            in.frame - frameOffRoadAttesa_ >= cfg_.offRoadAttesa) {
            offRoadPending_ = false;
            frameOffRoadAttesa_ = 0;
            avviaShow(in.frame, Personaggio::Gianni, OrigineShow::UscitaStrada,
                      fraseOffRoad());
        } else if (cooldownOk &&
                   in.frame - ultimoShow_ >= prTab(in.stage)) {
            Personaggio p = Personaggio::Nessuno;
            TabId tab = TabId::Ph;
            const u32* ids = tabelle::ptab;
            u32 n = 32;
            frasePeriodica(esc_.livello(), p, tab, ids, n);
            avviaShow(in.frame, p, OrigineShow::Periodico, sel_.scegli(tab, ids, n));
        } else if (cooldownOk && inattivitaArmata_ &&
                   idleFrames_ >= cfg_.inattivitaSoglia) {
            inattivitaArmata_ = false;
            idleFrames_ = 0;
            avviaShow(in.frame, Personaggio::Gianni, OrigineShow::Inattivita,
                      fraseInattivita());
        }
    }

    // 9) UFO: passaggio senza frase; se parte uno show esce in pochi frame.
    ufo_.aggiorna(in.frame, show_.attivo);
    return show_;
}

} // namespace sgl::gameplay
