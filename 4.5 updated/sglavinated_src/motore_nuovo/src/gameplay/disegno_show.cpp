// SPDX-License-Identifier: MIT
#include "gameplay/disegno_show.h"

#include "gameplay/testi_frasi.h" // testi di frasi.json per il fumetto
#include <cstdio>                 // std::snprintf (chiave asset della pose)

// Calcolo della scena di uno show, un frame alla volta.
// Riferimenti comportamentali (non codice): hack v87 (tavole ez/xo/bz/sw/bob,
// pose a 16 frame, "Gesu' cade dall'alto e risale"), HANDOVER.md §0/§2/§4,
// SPEC_personaggi_v74.md §4-§7, dati/show_logica.md. I numeri qui sono dati
// in ShowLayout: nessun vincolo 88 px / 156 fumetti / VRAM della v87.
// Nessun codice 68000, nessun asset/audio Sega, nessun libm (tabelle intere:
// identico su ogni piattaforma, serve per il determinismo dei test replay).

namespace sgl::gameplay {
namespace {

// Tavole di movimento (valori in passi, moltiplicati per le ampiezze in dati):
// deriva orizzontale (sway) e ondulazione verticale (bob), come le tavole
// sw_tab/bob della v87 ma valori nostri.
const i8 SWAY[4] = { 0, 1, 0, -1 };
const i8 BOB[16] = { 0, 1, 2, 3, 4, 3, 2, 1, 0, -1, -2, -3, -4, -3, -2, -1 };

u32 numCaratteri(const char* s) {
    u32 n = 0;
    if (!s) return 0;
    while (s[n] != '\0') ++n;
    return n;
}

// Scostamento fuori schermo lungo una direzione: k = 1 (appena fuori) .. 0
// (a riposo). La posa a riposo e' il box alto-sx (x - larghezza/2, y - altezza).
void scivola(const PersonaggioLayout& L, DirezioneShow d, f32 k,
             i32 schermoW, i32 schermoH, i32& dx, i32& dy) {
    dx = 0;
    dy = 0;
    const i32 bx = L.x - L.larghezza / 2;
    const i32 by = L.y - L.altezza;
    switch (d) {
        case DirezioneShow::Sinistra:
            dx = static_cast<i32>(k * static_cast<f32>(-L.larghezza - bx));
            break;
        case DirezioneShow::Destra:
            dx = static_cast<i32>(k * static_cast<f32>(schermoW - bx));
            break;
        case DirezioneShow::Alto:
            dy = static_cast<i32>(k * static_cast<f32>(-L.altezza - by));
            break;
        case DirezioneShow::Basso:
            dy = static_cast<i32>(k * static_cast<f32>(schermoH - by));
            break;
    }
}

} // namespace

void DisegnoShow::calcolaPersonaggio(const DatiShow& s, u64 frame) {
    const PersonaggioLayout& L = lay_.per(s.personaggio);
    const u32 durata = s.durata;
    const u64 t = (frame > s.frameInizio) ? (frame - s.frameInizio) : 0;

    sc_.personaggioVisibile = true;
    sc_.personaggio = s.personaggio;
    sc_.origine = s.origine;
    sc_.idFrase = s.idFrase;
    sc_.frameShow = (t > 0xFFFFFFFFull) ? 0xFFFFFFFFu : static_cast<u32>(t);
    sc_.frameRimasti = s.frameRimasti(frame);

    const u32 posePasso = L.posePasso ? L.posePasso : 1u;
    const u32 poseNum = L.poseNumero ? L.poseNumero : 1u;
    sc_.pose = (static_cast<u32>(t) / posePasso) % poseNum;

    // Fasi: entra -> tenuta -> esce (durate in dati; show brevi: dimezzate).
    u32 entra = L.entraFrame;
    u32 esce = L.esceFrame;
    if (entra + esce > durata) {
        entra = durata / 2;
        esce = durata - entra;
    }
    f32 kEntra = 0.0f; // 1 = fuori schermo, 0 = a riposo
    f32 kEsce = 0.0f;
    if (entra > 0 && t < entra) {
        kEntra = 1.0f - static_cast<f32>(t) / static_cast<f32>(entra);
    } else if (esce > 0 && durata > esce && t >= durata - esce) {
        kEsce = static_cast<f32>(t - (durata - esce)) / static_cast<f32>(esce);
    }

    i32 dx = 0, dy = 0;
    if (kEntra > 0.0f) {
        scivola(L, L.entraDa, clamp(kEntra, 0.0f, 1.0f),
                lay_.schermoW, lay_.schermoH, dx, dy);
    } else if (kEsce > 0.0f) {
        scivola(L, L.esceVerso, clamp(kEsce, 0.0f, 1.0f),
                lay_.schermoW, lay_.schermoH, dx, dy);
    } else {
        // Tenuta: deriva da tabelle (sway/bob), nessun seno.
        const u32 swayPasso = L.swayPasso ? L.swayPasso : 1u;
        const u32 bobPasso = L.bobPasso ? L.bobPasso : 1u;
        dx = static_cast<i32>(SWAY[(static_cast<u32>(t) / swayPasso) & 3u]) * L.swayAmp;
        dy = static_cast<i32>(BOB[(static_cast<u32>(t) / bobPasso) & 15u]) * L.bobAmp;
    }

    sc_.persX = L.x - L.larghezza / 2 + dx;
    sc_.persY = L.y - L.altezza + dy;
    sc_.persW = L.larghezza;
    sc_.persH = L.altezza;
}

void DisegnoShow::calcolaBolla(const DatiShow& s, const ShowManager& sm) {
    // I tempi (appare al ~30, sparisce prima dell'uscita) sono gia' decisi da
    // ShowManager::bollaVisibile(): qui non si ricalcola nulla.
    if (!sm.bollaVisibile()) return;
    sc_.bollaVisibile = true;

    const FraseTesto* f = testi_ ? testi_->get(s.idFrase) : nullptr;
    if (f && f->righe > 0) {
        sc_.righe = f->righe;
        sc_.linea1 = (f->righe >= 1) ? f->linea1 : nullptr;
        sc_.linea2 = (f->righe >= 2) ? f->linea2 : nullptr;
    }

    // Scatola calcolata sul testo (nessun limite 88 px della v87).
    const BollaLayout& B = lay_.bolla;
    const u32 nRighe = (sc_.righe > 0) ? static_cast<u32>(sc_.righe) : 1u;
    u32 maxChar = numCaratteri(sc_.linea1);
    const u32 c2 = numCaratteri(sc_.linea2);
    if (c2 > maxChar) maxChar = c2;

    i32 w = static_cast<i32>(maxChar) * B.carattereLarghezza + 2 * B.paddingX;
    if (w < B.larghezzaMin) w = B.larghezzaMin;
    i32 wMax = lay_.schermoW - 2 * B.margineSchermo;
    if (B.larghezzaMax < wMax) wMax = B.larghezzaMax;
    if (w > wMax) w = wMax;
    const i32 h = static_cast<i32>(nRighe) * B.rigaAltezza + 2 * B.paddingY;

    // Ancoraggio: centrata sopra la testa del personaggio, dentro lo schermo.
    const i32 cx = sc_.persX + sc_.persW / 2;
    i32 x = cx - w / 2;
    x = clamp(x, B.margineSchermo, lay_.schermoW - B.margineSchermo - w);
    i32 y = (sc_.persY - sc_.persH) - h - B.sopra;
    y = clamp(y, B.margineSchermo, lay_.schermoH - B.margineSchermo - h);

    sc_.bollaX = x;
    sc_.bollaY = y;
    sc_.bollaW = w;
    sc_.bollaH = h;
    sc_.testoX = x + B.paddingX;
    sc_.rigaY[0] = y + B.paddingY;
    sc_.rigaY[1] = y + B.paddingY + B.rigaAltezza;
    sc_.codaX = cx;
    sc_.codaY = (sc_.persY - sc_.persH) + 2; // subito sotto la testa
}

void DisegnoShow::calcolaUfo(const ShowManager& sm) {
    // Con uno show a schermo l'UFO e' gia' in fuga (ufo.h): non si disegna,
    // evita il salto di posa della fuga a 24 frame.
    if (sm.showAttivo()) return;
    const DatiUfo& u = sm.ufo().dati();
    if (!u.attivo) return;
    const UfoLayout& L = lay_.ufo;
    const f32 p = clamp(u.progresso, 0.0f, 1.0f);
    const i32 x0 = lay_.schermoW; // appena fuori a destra (progresso 0)
    const i32 x1 = -L.larghezza;  // appena fuori a sinistra (progresso 1)
    sc_.ufoVisibile = true;
    sc_.ufoX = x0 + static_cast<i32>(static_cast<f32>(x1 - x0) * p);
    sc_.ufoY = L.y + static_cast<i32>(u.yOffset); // ondulazione y (tabella tw)
}

const ScenaShow& DisegnoShow::aggiorna(u64 frame, const ShowManager& sm) {
    sc_ = ScenaShow{}; // default: tutto spento (nessun residuo del frame prima)
    const DatiShow& s = sm.datiShow();
    if (s.attivo) {
        calcolaPersonaggio(s, frame);
        calcolaBolla(s, sm);
    }
    calcolaUfo(sm);
    return sc_;
}

void DisegnoShow::chiaveSprite(Personaggio p, u32 pose, char* out, u32 dim) {
    if (!out || dim == 0) return;
    const char* nomeP = (p == Personaggio::Gianni) ? "gianni"
                      : (p == Personaggio::Gesu)   ? "gesu"
                                                  : "nessuno";
    std::snprintf(out, dim, "%s/pose_%02u", nomeP,
                  static_cast<unsigned>(pose));
}

} // namespace sgl::gameplay
