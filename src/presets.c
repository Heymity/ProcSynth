//
// Created by Gabriel on 31/07/2026.
//

#include <procSynth/presets.h>
#include <procSynth/voice.h>

const Envelope PianoEnvelope_Preset = (Envelope) {
	.attack_time = 0.005,
	.attack_overshoot = 0,
	.decay_time = 3,
	.sustain_level = 0.05,
	.release_time = .25,
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
	.release_time = .8,
	.envelopeMultiplier = 1.0,
	.decay_type = LINEAR_DECAY,
	.release_start_multiplier = 0.0,
	.sustain_end_time = 0.0,
};

const Envelope TromboneEnvelope_Preset = (Envelope) {
	.attack_time = 0.12,
	.attack_overshoot = 0,
	.decay_time = 0.05,
	.sustain_level = 0.95,
	.release_time = .8,
	.envelopeMultiplier = 1.0,
	.decay_type = LINEAR_DECAY,
	.release_start_multiplier = 0.0,
	.sustain_end_time = 0.0,
};

const Envelope ClarinetEnvelope_Preset = (Envelope) {
	.attack_time = 0.12,
	.attack_overshoot = 0,
	.decay_time = 0.05,
	.sustain_level = 0.95,
	.release_time = .8,
	.envelopeMultiplier = 1.0,
	.decay_type = LINEAR_DECAY,
	.release_start_multiplier = 0.0,
	.sustain_end_time = 0.0,
};

const Envelope GuitarEnvelope_Preset = (Envelope) {
	.attack_time = 0.12,
	.attack_overshoot = 0,
	.decay_time = 0.05,
	.sustain_level = 0.95,
	.release_time = .8,
	.envelopeMultiplier = 1.0,
	.decay_type = LINEAR_DECAY,
	.release_start_multiplier = 0.0,
	.sustain_end_time = 0.0,
};

const Envelope DrumEnvelope_Preset = (Envelope) {
	.attack_time = 0.12,
	.attack_overshoot = 0,
	.decay_time = 0.05,
	.sustain_level = 0.95,
	.release_time = .8,
	.envelopeMultiplier = 1.0,
	.decay_type = LINEAR_DECAY,
	.release_start_multiplier = 0.0,
	.sustain_end_time = 0.0,
};

const Envelope HarpEnvelope_Preset = (Envelope) {
	.attack_time = 0.12,
	.attack_overshoot = 0,
	.decay_time = 0.05,
	.sustain_level = 0.95,
	.release_time = .8,
	.envelopeMultiplier = 1.0,
	.decay_type = LINEAR_DECAY,
	.release_start_multiplier = 0.0,
	.sustain_end_time = 0.0,
};

const Envelope OrganEnvelope_Preset = (Envelope) {
	.attack_time = 0.12,
	.attack_overshoot = 0,
	.decay_time = 0.05,
	.sustain_level = 0.95,
	.release_time = .8,
	.envelopeMultiplier = 1.0,
	.decay_type = LINEAR_DECAY,
	.release_start_multiplier = 0.0,
	.sustain_end_time = 0.0,
};


Timbre PianoTimbre_Preset = (Timbre) {
    .num_harmonics = 6,
    .harmonic_weights = {1.0, 0.6, 0.3, 0.15, 0.08, 0.03, 0.0, 0.0}
};

Timbre ViolinTimbre_Preset = (Timbre) {
	.num_harmonics = 8,
	.harmonic_weights = {1.0, 0.5, 0.33, 0.25, 0.2, 0.16, 0.14, 0.125}
};
