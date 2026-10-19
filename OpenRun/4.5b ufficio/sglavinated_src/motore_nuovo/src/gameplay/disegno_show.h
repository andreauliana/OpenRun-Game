// SPDX-License-Identifier: MIT
#pragma once
#include "core/types.h"
#include "gameplay/personaggi.h"
#include "gameplay/show.h"

// Interfaccia di disegno degli show: da DatiShow (+ testi di frasi.json) a
// "cosa disegnare in questo frame" (personaggio, posa, posizione, fumetto,
// UFO). Specifica: motore_nuovo/dati/SPEC_GAMEPLAY.md §5/§7 e questo
// collegamento: motore_nuovo/src/gameplay/DISEGNO_SHOW.md (come collegarlo a
// src/game/game.cpp, che oggi scarta lo show con "(void)show;").
//
// Il modulo NON disegna e non include render/ o SDL: restituisce solo valori
// in px sul framebuffer 320x224 (origine in alto a sinistra) che il renderer
// consuma. Le misure della hack v87 (bolla 96x24, 88 px di riga, 8 pose x 4
// pezzi) sono qui solo riferimento diventato dato configurabile, non vincolo.
//
// Uso per frame (dopo ShowManager::aggiorna, con lo stesso contatore di frame):
//   disegnoShow_.aggiorna(frame, showManager_);
//   const ScenaShow& sc = disegnoShow_.scena();
//   -> disegna in ordine: UFO, personaggio, fumetto (scatola + testo).
// Deterministico: tempo = contatore di frame, tabelle fisse, nessun RNG
// nell'interfaccia di disegno, nessun orologio di sistema.

namespace sgl::gameplay {

class TestiFrasi;

// Direzione di entrata/uscita del personaggio.
enum class DirezioneShow : u8 { Sinistra = 0, Destra = 1, Alto = 2, Basso = 3 };

// Layout del personaggio (dati, come ShowConfig: si cambia qui, non nel codice).
struct PersonaggioLayout {
    i32 x = 240;           // centro orizzontale dell'ingombro (px)
    i32 y = 212;           // piedi (px)
    i32 larghezza = 72;    // ingombro di riferimento: il renderer scala lo
    i32 altezza = 88;      // sprite su questo box (persW x persH)
    u32 poseNumero = 8;    // quante pose ha il personaggio (Gianni 8, Gesu' 4)
    u32 posePasso = 16;    // frame fra una pose e l'altra (v87: pose a 16 frame)
    u32 entraFrame = 24;   // durata entrata (v87 ez_tab: 24)
    u32 esceFrame = 24;    // durata uscita  (v87 xo_tab: 24)
    DirezioneShow entraDa = DirezioneShow::Sinistra;
    DirezioneShow esceVerso = DirezioneShow::Sinistra;
    // Deriva "viva" durante la tenuta: sway orizzontale e bob verticale,
    // passi della tavola (nessun seno: tabelle intere = identico ovunque).
    u32 swayPasso = 2;
    i32 swayAmp = 2;       // px
    u32 bobPasso = 2;
    i32 bobAmp = 4;        // px
};

// Layout del fumetto: la scatola e' calcolata sul testo (max 2 righe di
// frasi.json). Metrica del font allineabile a dati/ui/font (dove il renderer
// avra' il font di @grafico).
struct BollaLayout {
    i32 sopra = 6;                // distanza dalla testa del personaggio
    i32 paddingX = 6;
    i32 paddingY = 5;
    i32 rigaAltezza = 12;
    i32 carattereLarghezza = 8;   // metrica per stimare la larghezza
    i32 larghezzaMin = 96;        // come la bolla v87, ma solo minimo estetico
    i32 larghezzaMax = 264;       // cap di sicurezza (NON il limite 88 px v87)
    i32 margineSchermo = 4;
};

// UFO: passaggio da destra a sinistra, mai una frase.
struct UfoLayout {
    i32 y = 40;        // quota del bordo superiore (px); l'ondulazione y e' in DatiUfo
    i32 larghezza = 64;
    i32 altezza = 24;
};

// Layout completo: default pensati per il framebuffer 320x224 con l'HUD in
// alto a sinistra (dati/ui/hud.json) — i personaggi stanno a destra, il
// fumetto centrato sopra la loro testa.
struct ShowLayout {
    i32 schermoW = 320; // allinearli a Framebuffer::WIDTH/HEIGHT (opzionale)
    i32 schermoH = 224;
    PersonaggioLayout gianni{};
    // Gesu' "cade dall'alto e risale" (dati/personaggi/gesu/manifest.json):
    // entrata e uscita dal alto, 4 pose piu' lente, niente sway/bob propri.
    PersonaggioLayout gesu{
        240, 212, 72, 96, 4, 32, 24, 24,
        DirezioneShow::Alto, DirezioneShow::Alto,
        2, 0, 2, 0
    };
    BollaLayout bolla{};
    UfoLayout ufo{};

    PersonaggioLayout& per(Personaggio p) {
        return (p == Personaggio::Gesu) ? gesu : gianni;
    }
    const PersonaggioLayout& per(Personaggio p) const {
        return (p == Personaggio::Gesu) ? gesu : gianni;
    }
};

// Cosa disegnare in questo frame: valori gia' pronti in px.
// Ordine di disegno (il renderer rispetta l'ordine): 1) UFO, 2) personaggio,
// 3) fumetto (scatola poi testo). Il personaggio va sopra la strada e sotto
// il fumetto; l'UFO vola in alto e non si sovrappone agli show (show.h/ufo.h).
struct ScenaShow {
    // -- Personaggio --
    bool personaggioVisibile = false;
    Personaggio personaggio = Personaggio::Nessuno;
    OrigineShow origine = OrigineShow::Nessuna; // per fx post-crash, HUD, test
    u32 pose = 0;                // indice pose (chiave asset: chiaveSprite)
    i32 persX = 0, persY = 0;    // angolo alto-sx dell'ingombro
    i32 persW = 0, persH = 0;    // box da coprire (scala dello sprite)
    u32 frameShow = 0;           // frame vissuti dello show
    u32 frameRimasti = 0;

    // -- Fumetto --
    bool bollaVisibile = false;  // gia' calcolato da ShowManager::bollaVisibile()
    i32 bollaX = 0, bollaY = 0;  // scatola
    i32 bollaW = 0, bollaH = 0;
    i32 testoX = 0;              // inizio testo (bollaX + paddingX)
    i32 rigaY[2] = {0, 0};       // y della riga 1 e della riga 2
    i32 codaX = 0, codaY = 0;    // punto della coda (sulla testa del personaggio)
    u32 idFrase = 0;             // indice in dati/frasi.json
    u8 righe = 0;                // 1 o 2 (0 = testo non disponibile)
    const char* linea1 = nullptr; // da TestiFrasi (nullptr se non caricato)
    const char* linea2 = nullptr;

    // -- UFO (passaggio senza frase) --
    bool ufoVisibile = false;
    i32 ufoX = 0, ufoY = 0;      // angolo alto-sx; y gia' comprensiva di ondulazione
};

class DisegnoShow {
public:
    void setLayout(const ShowLayout& l) { lay_ = l; }
    const ShowLayout& layout() const { return lay_; }

    // Testi di frasi.json (opzionale): senza, il fumetto mostra solo la
    // scatola. Il puntatore deve restare valido (membro di Game/runner).
    void setTesti(const TestiFrasi* t) { testi_ = t; }

    void reset() { sc_ = ScenaShow{}; }

    // Calcola la scena del frame. Chiamare DOPO ShowManager::aggiorna() con lo
    // stesso frame, altrimenti bolla/posa arrivano di un frame indietro.
    const ScenaShow& aggiorna(u64 frame, const ShowManager& sm);

    const ScenaShow& scena() const { return sc_; }

    // Chiave asset della pose (convenzione con CATALOGO_PERSONAGGI.md e @asset):
    // "gianni/pose_00".."gianni/pose_07", "gesu/pose_00".."gesu/pose_03",
    // "nessuno/pose_00" per il personaggio assente.
    static void chiaveSprite(Personaggio p, u32 pose, char* out, u32 dim);

private:
    ShowLayout lay_{};
    const TestiFrasi* testi_ = nullptr;
    ScenaShow sc_{};

    void calcolaPersonaggio(const DatiShow& s, u64 frame);
    void calcolaBolla(const DatiShow& s, const ShowManager& sm);
    void calcolaUfo(const ShowManager& sm);
};

} // namespace sgl::gameplay
