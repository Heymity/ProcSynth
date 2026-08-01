//
// Created by Gabriel on 31/07/2026.
//

#include <procSynth/presets.h>
#include <procSynth/voice.h>

const Envelope PianoEnvelope_Preset = (Envelope) {
	.attack_time = 0.005,
	.attack_overshoot = .1,
	.decay_time = 5,
	.sustain_level = 0.05,
	.release_time = .1,
	.envelopeMultiplier = 1.0,
	.decay_type = LINEAR_DECAY,
	.release_start_multiplier = 0.0,
	.sustain_end_time = 0.0,
};

const Envelope ViolinEnvelope_Preset = (Envelope) {
	.attack_time = 0.12,
	.attack_overshoot = 0,
	.decay_time = 0.05,
	.sustain_level = 0.95,
	.release_time = .2,
	.envelopeMultiplier = 1.0,
	.decay_type = LINEAR_DECAY,
	.release_start_multiplier = 0.0,
	.sustain_end_time = 0.0,
};