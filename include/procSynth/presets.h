//
// Created by Gabriel on 31/07/2026.
//

#ifndef PRESETS_H
#define PRESETS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <procSynth/voice.h>

#define NOTE_A4 440
#define NOTE_B4 493.88
#define NOTE_C4 261.63
#define NOTE_D4 293.66
#define NOTE_E4 329.63
#define NOTE_F4 349.23
#define NOTE_G4 392

extern const Envelope PianoEnvelope_Preset;
extern const Envelope ViolinEnvelope_Preset;

extern Timbre PianoTimbre_Preset;
extern Timbre ViolinTimbre_Preset;

#ifdef __cplusplus
  }
#endif

#endif //PRESETS_H
