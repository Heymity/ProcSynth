//
// Created by Gabriel on 31/07/2026.
//

#ifndef PRESETS_H
#define PRESETS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <procSynth/voice.h>

#define NOTE_A4 440.0
#define NOTE_B4 493.88
#define NOTE_C4 261.63
#define NOTE_D4 293.66
#define NOTE_E4 329.63
#define NOTE_F4 349.23
#define NOTE_G4 392.0

extern const Envelope PianoEnvelope_Preset;
extern const Envelope ViolinEnvelope_Preset;
extern const Envelope GuitarEnvelope_Preset;
extern const Envelope TromboneEnvelope_Preset;
extern const Envelope ClarinetEnvelope_Preset;
extern const Envelope HarpEnvelope_Preset;
extern const Envelope OrganEnvelope_Preset;
extern const Envelope DrumEnvelope_Preset;
extern const Envelope BassEnvelope_Preset;

extern Timbre PianoTimbre_Preset;
extern Timbre ViolinTimbre_Preset;
extern Timbre GuitarTimbre_Preset;
extern Timbre TromboneTimbre_Preset;
extern Timbre ClarinetTimbre_Preset;
extern Timbre HarpTimbre_Preset;
extern Timbre OrganTimbre_Preset;
extern Timbre DrumTimbre_Preset;
extern Timbre BassTimbre_Preset;

#ifdef __cplusplus
  }
#endif

#endif //PRESETS_H
