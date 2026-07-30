//
// Created by Gabriel on 30/07/2026.
//

#include<procSynth/voice.h>

struct Voice {
	int voiceNumber;

	double frequency;
	double baseAmplitude;

	double attack;
	double release;
	double sustain;
	double decay;

	double time;
};

typedef struct Voice Voice;

void synthetize_voice(Voice v, short* output_buffer, int buffer_size) {



}