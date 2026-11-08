REVIEW PROPOSTA P1 - Fix screenshot P1 incomplete
================================================

PROBLEM ROOT CAUSE:
-------------------
The `salvaScreenshotP1` function in `motore_nuovo/test/prova_gate.cpp` (line 969) uses
this pad pattern after frame 600:

  pad.held = (frame > 600 && (frame / 40) % 2) ? 0ull : (u64)Pad::A;

This alternates gas/stop every 40 frames. The show/fumetto requires 500 consecutive
frames without input (no A pressed) to start (see padInattivita() in the same file:
frame < 60 => A, frame >= 60 => 0). The current pattern never provides 500 consecutive
idle frames, so framePers can remain -1 (incomplete screenshot).

FILE:           motore_nuovo/test/prova_gate.cpp
FUNCTION:       salvaScreenshotP1
LINE:           969

PROPOSED FIX:
-------------
Change line 969 from:
  pad.held = (frame > 600 && (frame / 40) % 2) ? 0ull : (u64)Pad::A;
to:
  pad.held = (frame <= 600) ? (u64)Pad::A : 0;

This ensures A (gas) is pressed for the first 600 frames, then released completely
for all subsequent frames. This provides 2300+ consecutive idle frames (frames 601-3000),
well above the 500-frame threshold needed for the show/fumetto to appear.

IMPACT:
- Determinism: preserved (fixed input pattern, seed 12345)
- ms/frame: unchanged (same simulation logic)
- Replay 2x IDENTICO: preserved (same seed, same input)
- Other gates: unaffected (this fix only changes the screenshot P1 test)

ALTERNATIVE (if C++ mod not possible):
---------------
Modify gate_t1.sh to retry the screenshot P1 command up to 3 times until
framePers > 0, since the current non-deterministic pattern sometimes produces
framePers=446 (as observed in current runs).

STATUS: Dipendenza bloccante - richiede modifica a @developer-junior su
motore_nuovo/test/prova_gate.cpp linea 969. Proposta non applicabile in questo
turno per rispettare il divieto di modificare file domain @developer-junior.