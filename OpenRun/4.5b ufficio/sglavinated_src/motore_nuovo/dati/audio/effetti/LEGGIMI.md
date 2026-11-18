# Effetti campionati

- `voce_sterzo.wav`: jingle vocale dello sterzo (Andrea 18/11, "steer-wicked").
  Suona insieme allo stridio delle gomme quando si sterza forte in curva,
  una volta per curva (vedi `VOCE_STERZO_*` in `src/game/game.cpp`).
  Formato leggero per il Pi: mono, 8 bit, 11025 Hz (~25 KB). Fade in/out
  sono quelli di Andrea nel file: la conversione toglie solo il silenzio.
  Monofonico: se la voce sta gia' suonando non riparte e non si sovrappone.

Per sostituirlo con un'altra voce, stesso nome, convertita cosi':

    ffmpeg -i originale.wav -af "areverse,silenceremove=start_periods=1:start_threshold=-60dB,areverse,silenceremove=start_periods=1:start_threshold=-60dB,lowpass=f=5000" -ar 11025 -ac 1 -c:a pcm_u8 voce_sterzo.wav

Il gioco legge anche WAV a 16 bit, quindi va bene anche senza convertire.
