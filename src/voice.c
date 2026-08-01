//
// Created by Gabriel on 30/07/2026.
//

#include <math.h>
#include <procSynth/synth.h>
#include <procSynth/voice.h>

#define INACTIVE_VS 0
#define ATTACK_VS 1
#define DECAY_VS 2
#define SUSTAIN_VS 3
#define RELEASE_VS 4

void reset_envelope(Envelope *envelope) {
	envelope->attack_overshoot = 0;
	envelope->attack_time = 0;
	envelope->decay_time = 0;
	envelope->decay_type = LINEAR_DECAY;
	envelope->release_start_multiplier = 0;
	envelope->release_time = 0;
	envelope->sustain_end_time = 0;
	envelope->sustain_level = 0;
	envelope->envelopeMultiplier = 1.0;
}

void reset_timbre(Timbre* timbre) {

}

void reset_voice(Voice* voice) {
	voice->voiceNumber = 0;
	voice->frequency = 0;

	voice->baseAmplitude = 0;
	voice->envelope.attack_overshoot = 0;
	voice->pressed = false;
	reset_envelope(&voice->envelope);

	voice->time = 0;
	voice->cumulative_phase = 0;

	voice->active = false;
}

int get_current_voice_state(Voice* voice) {
	if (!voice->active) {
		voice->active = false;
		return INACTIVE_VS;
	}
	if (voice->time < voice->envelope.attack_time) return ATTACK_VS;
	if (voice->baseAmplitude * voice->envelope.envelopeMultiplier < 1) return INACTIVE_VS;
	if (!voice->pressed && voice->baseAmplitude * voice->envelope.envelopeMultiplier > 1) return RELEASE_VS;
	if (voice->time - voice->envelope.attack_time < voice->envelope.decay_time) return DECAY_VS;
	if (voice->pressed && voice->baseAmplitude * voice->envelope.envelopeMultiplier > 1) return SUSTAIN_VS;

	return RELEASE_VS;
}

void synth_voice(Voice* v, short* output_buffer, const int buffer_size) {
	const double deltaT = 1 / (double)SAMPLE_RATE;

	const int total_frames = buffer_size / CHANNELS;
	const double current_freq = v->frequency;
	const double phase_increment = (2.0 * M_PI * current_freq) / SAMPLE_RATE;

	for (int i = 0; i < total_frames; i++) {
		switch (get_current_voice_state(v)) {
			case ATTACK_VS:
				v->envelope.envelopeMultiplier = v->time * (1 + v->envelope.attack_overshoot) / v->envelope.attack_time;
				break;
			case DECAY_VS:
				const double n_time = (v->time - v->envelope.attack_time) / v->envelope.decay_time;
				if (v->envelope.decay_type == LINEAR_DECAY)
					v->envelope.envelopeMultiplier =
							(1 + v->envelope.attack_overshoot +
							 n_time * (v->envelope.sustain_level - (1 + v->envelope.attack_overshoot)));
				else
					v->envelope.envelopeMultiplier = exp(-n_time) * (1 + v->envelope.attack_overshoot);
				v->envelope.release_start_multiplier = v->envelope.envelopeMultiplier;
				v->envelope.sustain_end_time = v->time;
				break;
			case SUSTAIN_VS:
				v->envelope.envelopeMultiplier = v->envelope.sustain_level;
				v->envelope.sustain_end_time = v->time;
				v->envelope.release_start_multiplier = v->envelope.envelopeMultiplier;
				break;
			case RELEASE_VS:
				const double norm_time = (v->time - v->envelope.sustain_end_time) / v->envelope.release_time;
				v->envelope.envelopeMultiplier =
						v->envelope.release_start_multiplier - (norm_time * v->envelope.release_start_multiplier);
				break;
			case INACTIVE_VS:
			default:
				v->envelope.envelopeMultiplier = 0;
				break;
		}
		const double current_amp = v->baseAmplitude * v->envelope.envelopeMultiplier;
		//printf("ca: %f\n", current_amp);
		const short sample = (short)(current_amp * sin(v->cumulative_phase));

		v->cumulative_phase += phase_increment;

		if (v->cumulative_phase >= 2.0 * M_PI)
			v->cumulative_phase -= 2.0 * M_PI;

		output_buffer[i * 2] = sample;     // Left channel
		output_buffer[i * 2 + 1] = sample; // Right channel

		v->time += deltaT;
	}
}