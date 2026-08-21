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

Timbre PianoTimbre_Preset = (Timbre) {
	.num_harmonics = 6,
	.harmonic_weights = {1.0, 0.6, 0.3, 0.15, 0.08, 0.03, 0.0, 0.0}
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

Timbre ViolinTimbre_Preset = (Timbre) {
	.num_harmonics = 8,
	.harmonic_weights = {1.0, 0.5, 0.33, 0.25, 0.2, 0.16, 0.14, 0.125}
};

const Envelope TromboneEnvelope_Preset = (Envelope) {
    .attack_time = 0.06,
    .attack_overshoot = 0,
    .decay_time = 0.20,
    .sustain_level = 0.85,
    .release_time = 0.12,
    .envelopeMultiplier = 1.0,
    .decay_type = LINEAR_DECAY,
    .release_start_multiplier = 0.0,
    .sustain_end_time = 0.0,
};

Timbre TromboneTimbre_Preset = (Timbre) {
    .num_harmonics = 8,
    .harmonic_weights = {1.0, 0.85, 0.75, 0.60, 0.45, 0.30, 0.20, 0.10}
};

// ==========================================
// GUITAR (Acoustic Guitar)
// ==========================================
const Envelope GuitarEnvelope_Preset = (Envelope) {
    .attack_time = 0.003,
    .attack_overshoot = 0,
    .decay_time = 2.50,
    .sustain_level = 0.0,
    .release_time = 0.20,
    .envelopeMultiplier = 1.0,
    .decay_type = LINEAR_DECAY,
    .release_start_multiplier = 0.0,
    .sustain_end_time = 0.0,
};

Timbre GuitarTimbre_Preset = (Timbre) {
    .num_harmonics = 8,
    .harmonic_weights = {1.0, 0.70, 0.45, 0.25, 0.12, 0.06, 0.03, 0.01}
};

// ==========================================
// CLARINET (Harmônicas Ímpares Predominantes)
// ==========================================
const Envelope ClarinetEnvelope_Preset = (Envelope) {
    .attack_time = 0.04,
    .attack_overshoot = 0,
    .decay_time = 0.10,
    .sustain_level = 0.85,
    .release_time = 0.08,
    .envelopeMultiplier = 1.0,
    .decay_type = LINEAR_DECAY,
    .release_start_multiplier = 0.0,
    .sustain_end_time = 0.0,
};

Timbre ClarinetTimbre_Preset = (Timbre) {
    .num_harmonics = 8,
    .harmonic_weights = {1.0, 0.05, 0.75, 0.05, 0.50, 0.05, 0.30, 0.05}
};

// ==========================================
// HARP
// ==========================================
const Envelope HarpEnvelope_Preset = (Envelope) {
    .attack_time = 0.002,
    .attack_overshoot = 0,
    .decay_time = 3.50,
    .sustain_level = 0.0,
    .release_time = 0.40,
    .envelopeMultiplier = 1.0,
    .decay_type = LINEAR_DECAY,
    .release_start_multiplier = 0.0,
    .sustain_end_time = 0.0,
};

Timbre HarpTimbre_Preset = (Timbre) {
    .num_harmonics = 8,
    .harmonic_weights = {1.0, 0.80, 0.50, 0.30, 0.15, 0.08, 0.04, 0.02}
};

// ==========================================
// ORGAN (Full Organ)
// ==========================================
const Envelope OrganEnvelope_Preset = (Envelope) {
    .attack_time = 0.001,
    .attack_overshoot = 0,
    .decay_time = 0.01,
    .sustain_level = 1.0,
    .release_time = 0.01,
    .envelopeMultiplier = 1.0,
    .decay_type = LINEAR_DECAY,
    .release_start_multiplier = 0.0,
    .sustain_end_time = 0.0,
};

Timbre OrganTimbre_Preset = (Timbre) {
    .num_harmonics = 8,
    .harmonic_weights = {1.0, 0.80, 0.90, 0.60, 0.50, 0.40, 0.30, 0.20}
};

// ==========================================
// TAMBOR / PERCUSSÃO
// ==========================================
const Envelope DrumEnvelope_Preset = (Envelope) {
    .attack_time = 0.001,
    .attack_overshoot = 0,
    .decay_time = 0.15,
    .sustain_level = 0.0,
    .release_time = 0.05,
    .envelopeMultiplier = 1.0,
    .decay_type = LINEAR_DECAY,
    .release_start_multiplier = 0.0,
    .sustain_end_time = 0.0,
};

Timbre DrumTimbre_Preset = (Timbre) {
    .num_harmonics = 8,
    .harmonic_weights = {1.0, 0.40, 0.15, 0.05, 0.02, 0.01, 0.0, 0.0}
};

// ==========================================
// BASS ELETRÔNICO / ACÚSTICO
// ==========================================
const Envelope BassEnvelope_Preset = (Envelope) {
    .attack_time = 0.005,
    .attack_overshoot = 0,
    .decay_time = 0.60,
    .sustain_level = 0.40,
    .release_time = 0.15,
    .envelopeMultiplier = 1.0,
    .decay_type = LINEAR_DECAY,
    .release_start_multiplier = 0.0,
    .sustain_end_time = 0.0,
};

Timbre BassTimbre_Preset = (Timbre) {
    .num_harmonics = 8,
    .harmonic_weights = {1.0, 0.85, 0.50, 0.25, 0.10, 0.05, 0.02, 0.01}
};

// ==========================================
// Theremin
// ==========================================
const Envelope ThereminEnvelope_Preset = (Envelope) {
    .attack_time = 0.001,
    .attack_overshoot = 0,
    .decay_time = 0.01,
    .sustain_level = 1.0,
    .release_time = 0.01,
    .envelopeMultiplier = 1.0,
    .decay_type = LINEAR_DECAY,
    .release_start_multiplier = 0.0,
    .sustain_end_time = 0.0,
};

Timbre ThereminTimbre_Preset = (Timbre) {
    .num_harmonics = 8,
    .harmonic_weights = {1.0, 0.0 ,0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
};

