#!/usr/bin/env python3
"""Taglia il suono del motore da un WAV lungo (Andrea 13/11: "Old Sport Car Realistic").

Uso: python3 strumenti_py/taglia_motore.py <file.wav>
Scrive in dati/audio/motore/ tre WAV mono 22050 Hz 16 bit:
  accensione.wav  (avviamento, una volta sola)
  minimo.wav      (giro chiuso, motore al minimo)
  pieno.wav       (giro, motore spinto)
Il gioco cambia tono e mescola minimo/pieno secondo i giri (src/audio/mixer.cpp).
"""
import os, sys, wave
import numpy as np

SR = 22050
# Tratti del file originale (secondi), scelti ascoltando l'andamento del volume e del tono.
ACCENSIONE = (3.3, 4.9)
MINIMO = (25.4, 28.4)
PIENO = (14.0, 17.6)
INCROCIO = 0.25  # secondi di dissolvenza tra fine e inizio del giro

def leggi(path):
    w = wave.open(path)
    sr, ch, sw = w.getframerate(), w.getnchannels(), w.getsampwidth()
    d = np.frombuffer(w.readframes(w.getnframes()), dtype={2: np.int16, 4: np.int32}[sw]).astype(np.float64)
    d = d.reshape(-1, ch).mean(1) / float(2 ** (8 * sw - 1))
    # ricampiona a SR (lineare, basta per un motore)
    t = np.arange(int(len(d) * SR / sr)) * sr / SR
    return np.interp(t, np.arange(len(d)), d)

def taglio(d, a, b):
    return d[int(a * SR):int(b * SR)].copy()

def giro(d, a, b):
    seg = taglio(d, a, b)
    c = int(INCROCIO * SR)
    out = seg[c:].copy()
    f = np.linspace(0.0, 1.0, c)
    out[-c:] = seg[-c:] * (1.0 - f) + seg[:c] * f  # la fine sfuma nell'inizio: giro senza scatti
    return out

def scrivi(path, d, rms):
    d = d * (rms / max(1e-9, np.sqrt((d ** 2).mean())))
    d = np.clip(d, -0.98, 0.98)
    w = wave.open(path, "wb")
    w.setnchannels(1); w.setsampwidth(2); w.setframerate(SR)
    w.writeframes((d * 32767).astype("<i2").tobytes())
    w.close()
    print(path, f"{len(d) / SR:.2f} s")

def main():
    d = leggi(sys.argv[1])
    out = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "dati", "audio", "motore")
    os.makedirs(out, exist_ok=True)
    acc = taglio(d, *ACCENSIONE)
    n = int(0.3 * SR)
    acc[-n:] *= np.linspace(1.0, 0.0, n)
    scrivi(os.path.join(out, "accensione.wav"), acc, 0.20)
    scrivi(os.path.join(out, "minimo.wav"), giro(d, *MINIMO), 0.20)
    scrivi(os.path.join(out, "pieno.wav"), giro(d, *PIENO), 0.20)

if __name__ == "__main__":
    main()
