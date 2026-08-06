//
// Created by Gabriel on 30/07/2026.
//

#ifndef VOICE_H
#define VOICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define LINEAR_DECAY 0
#define EXPONENTIAL_DECAY 1

#define WAVETABLE_SIZE 1024
#define MAX_HARMONICS 8

struct Envelope {
  double envelopeMultiplier;
  double attack_time;
  double attack_overshoot;
  double decay_time;
  double sustain_level;
  double sustain_end_time;
  double release_time;
  double release_start_multiplier;
  int decay_type;
};
typedef struct Envelope Envelope;

struct Timbre {
    int num_harmonics;
    double harmonic_weights[MAX_HARMONICS];
    double wavetable[WAVETABLE_SIZE]; // Armazenará a onda processada pelo PFFFT
};
typedef struct Timbre Timbre;

struct Voice {
  int voiceNumber;

  double frequency;

  short baseAmplitude;
  Envelope envelope;

  Timbre timbre;

  double time;
  double cumulative_phase;

  bool pressed;
  bool active;
};
typedef struct Voice Voice;

void synth_voice(Voice* v, short* output_buffer, int buffer_size);
void reset_voice(Voice* voice);
void reset_envelope(Envelope *envelope);
void reset_timbre(Timbre* timbre);
void build_wavetable(Timbre* timbre);


#ifdef __cplusplus
}
#endif

#endif //VOICE_H
