//
// Created by Gabriel on 30/07/2026.
//

#ifndef SYNTH_H
#define SYNTH_H

#ifdef __cplusplus
extern "C" {
#endif

#define SAMPLE_RATE 48000
#define MAX_VOLUME 32767
#define CHANNELS 2
#include "voice.h"

void * start_synth_thread(void*);
void synth();

int press_key(double frequency, short amplitude, Envelope envelope, Timbre timbre);
void update_voice(int voice, double frequency, short amplitude);
void release_key(int voice_number);


#ifdef __cplusplus
}
#endif

#endif //SYNTH_H
