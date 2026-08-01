//
// Created by Gabriel on 30/07/2026.
//

#ifndef VOICE_H
#define VOICE_H
#include <stdbool.h>

#define LINEAR_DECAY 0
#define EXPONENTIAL_DECAY 1

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

#endif //VOICE_H
