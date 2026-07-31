#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <alsa/asoundlib.h>
#include <procSynth/synth.h>
#include <procSynth/voice.h>

#define DURATION_SEC (40.0/1000.0)
#define FREQUENCY 440.0
#define VOLUME 30000.0 // Max for 16-bit signed is 32767

#define VOICES_NUM 100

static Voice voices[VOICES_NUM] = {};

void write_buffer(snd_pcm_t* playback_handle, short* buffer, const long length) {
	snd_pcm_sframes_t frames_written = snd_pcm_writei(playback_handle, buffer, length);
	if (frames_written < 0) {
		frames_written = snd_pcm_recover(playback_handle, (int)frames_written, 0);
		if (frames_written < 0) {
			printf("Error: %s\n", snd_strerror((int)frames_written));
		}
	}
}

void close_and_drain(snd_pcm_t* playback_handle) {
	snd_pcm_drain(playback_handle);
	snd_pcm_close(playback_handle);
}

int open_and_configure_interface(snd_pcm_t** playback_handle) {
	int err;

	if ((err = snd_pcm_open(playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		fprintf(stderr, "Cannot open audio device: %s\n", snd_strerror(err));
		return -1;
	}

	err = snd_pcm_set_params(*playback_handle,
							 SND_PCM_FORMAT_S16_LE,
							 SND_PCM_ACCESS_RW_INTERLEAVED,
							 CHANNELS,
							 SAMPLE_RATE,
							 1,			// Allow software resampling
							 20000); // 20ms latency for real time
	if (err < 0) {
		fprintf(stderr, "Playback configuration error: %s\n", snd_strerror(err));
		snd_pcm_close(*playback_handle);
		return -1;
	}

	return 0;
}

void synth() {
	snd_pcm_t *playback_handle;

	if (open_and_configure_interface(&playback_handle) != 0)
		return;

	for (int i = 1; i < VOICES_NUM; i++) {
		reset_voice(&voices[i]);
	}

	voices[0] = (Voice) {
		.voiceNumber = 0,
		.frequency = 440,
		.active = true,

		.baseAmplitude = 7500,
		.attack_time = 0.05,
		.attack_overshoot = .5,
		.pressed = true,
		.decay_time = 12,
		.sustain_level = 0,
		.release_time = .1,
		.envelopeMultiplier = 1.0,
		.decay_type = LINEAR_DECAY
	};

	voices[1] = (Voice) {
		.voiceNumber = 1,
		.frequency = 440 * exp2(4.0/12.0),
		.active = false,

		.baseAmplitude = 4500,
		.attack_time = 0.05,
		.attack_overshoot = .5,
		.pressed = false,
		.decay_time = 12,
		.sustain_level = 0,
		.release_time = .1,
		.envelopeMultiplier = 1.0,
		.decay_type = LINEAR_DECAY
	};

	voices[2] = (Voice) {
		.voiceNumber = 2,
		.frequency = 440 * exp2(7.0/12.0),
		.active = false,

		.baseAmplitude = 6000,
		.attack_time = 0.05,
		.attack_overshoot = .5,
		.pressed = false,
		.decay_time = 12,
		.sustain_level = 0,
		.release_time = .1,
		.envelopeMultiplier = 1.0,
		.decay_type = LINEAR_DECAY
	};

	voices[3] = (Voice) {
		.voiceNumber = 3,
		.frequency = 440 * exp2(12.0/12.0),
		.active = false,

		.baseAmplitude = 2000,
		.attack_time = 0.05,
		.attack_overshoot = .5,
		.pressed = false,
		.decay_time = 12,
		.sustain_level = 0,
		.release_time = .1,
		.envelopeMultiplier = 1.0,
		.decay_type = LINEAR_DECAY
	};

	const int total_frames = SAMPLE_RATE * 0.1;
	short *buffer = malloc(total_frames * CHANNELS * sizeof(short));
	short *loc_buffer = malloc(total_frames * CHANNELS * sizeof(short));

	for (int i = 0; i < total_frames * CHANNELS; i++) {
		buffer[i] = 0;
		loc_buffer[i] = 0;
	}

	int i = 0;
	while (true){
		i++;
		//if (i++ > 50) voices[0].pressed = false;

		if (i % 15 == 0) {
			voices[i / 15].active = true;
			voices[i / 15].pressed = true;
		}

		for (int j = 0; j < VOICES_NUM; j++) {
			if (!voices[j].active) continue;

			synth_voice(&voices[j], loc_buffer, total_frames * CHANNELS);

			for (int k = 0; k < total_frames * CHANNELS; k++) {
				if (buffer[k] + loc_buffer[k] > MAX_VOLUME) buffer[k] = MAX_VOLUME;
				else buffer[k] = (short)(buffer[k] + loc_buffer[k]);
			}
		}

		write_buffer(playback_handle, buffer, total_frames);

		for (int j = 0; j < total_frames * CHANNELS; j++) {
			buffer[j] = 0;
		}
	}

	free(buffer);
	free(loc_buffer);
	close_and_drain(playback_handle);
}
