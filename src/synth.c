#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <procSynth/synth.h>
#include <procSynth/voice.h>
#include <procSynth/presets.h>

#include "procSynth/utils.h"

#define VOICES_NUM 100

static Voice voices[VOICES_NUM] = {};
static snd_pcm_uframes_t buffer_size, period_size;

static void write_buffer(snd_pcm_t* playback_handle, const short * buffer, const long length) {
	snd_pcm_sframes_t frames_written = snd_pcm_writei(playback_handle, buffer, length);
	if (frames_written < 0) {
		frames_written = snd_pcm_recover(playback_handle, (int)frames_written, 0);
		if (frames_written < 0) {
			printf("Error: %s\n", snd_strerror((int)frames_written));
		}
	}
}

static void close_and_drain(snd_pcm_t* playback_handle) {
	snd_pcm_drain(playback_handle);
	snd_pcm_close(playback_handle);
}

static int open_and_configure_interface(snd_pcm_t** playback_handle) {
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
							 60000); // 20ms latency for real time
	if (err < 0) {
		fprintf(stderr, "Playback configuration error: %s\n", snd_strerror(err));
		snd_pcm_close(*playback_handle);
		return -1;
	}

	return 0;
}

int find_free_voice() {
	for (int i = 0; i < VOICES_NUM; i++) {
		if (!voices[i].active)
			return i;
	}
	return -1;
}

int press_key(double frequency, short amplitude, Envelope envelope, Timbre timbre) {
	int v_index = find_free_voice();
	if (v_index < 0) return v_index;

	reset_voice(&voices[v_index]);

	voices[v_index] = (Voice) {
		.frequency = frequency,
		.active = true,
		.pressed = true,
		.baseAmplitude = amplitude,
		.envelope = envelope,
		.timbre = timbre
	};

	return v_index;
}

void release_key(int voice_number) {
	if (voice_number < 0 || voice_number >= VOICES_NUM) return;
	voices[voice_number].pressed = false;
}

void* start_synth_thread(void* _) {
	(void)_; // Suppress unused param warning
	print_formated("Initializing synth thread\n");
	synth();
	print_formated("Synth thread ended abruptly for no apparent reason\n");
	return NULL;
}

void synth() {
	snd_pcm_t *playback_handle;

	if (open_and_configure_interface(&playback_handle) != 0)
		return;

	snd_pcm_get_params(playback_handle, &buffer_size, &period_size);

	build_wavetable(&PianoTimbre_Preset);
	build_wavetable(&ViolinTimbre_Preset);
	build_wavetable(&TromboneTimbre_Preset);
	build_wavetable(&GuitarTimbre_Preset);
	build_wavetable(&ClarinetTimbre_Preset);
	build_wavetable(&HarpTimbre_Preset);
	build_wavetable(&OrganTimbre_Preset);
	build_wavetable(&DrumTimbre_Preset);
	build_wavetable(&BassTimbre_Preset);

	print_formated("Wavetables built; Buffer Size = %lu; Period Size = %lu\n", buffer_size, period_size);

	for (int i = 1; i < VOICES_NUM; i++) {
		reset_voice(&voices[i]);
		voices[i].voiceNumber = i;
	}

	const int total_frames = period_size;
	short *buffer = malloc(total_frames * CHANNELS * sizeof(short));
	short *loc_buffer = malloc(total_frames * CHANNELS * sizeof(short));

	for (int i = 0; i < total_frames * CHANNELS; i++) {
		buffer[i] = 0;
		loc_buffer[i] = 0;
	}

	print_formated("Synth Running\n");

	while (true){
		for (int i = 0; i < VOICES_NUM; i++) {
			if (!voices[i].active) continue;

			synth_voice(&voices[i], buffer, total_frames * CHANNELS);
		}

		write_buffer(playback_handle, buffer, total_frames);

		memset(buffer, 0, total_frames * CHANNELS * sizeof(short));
	}

	free(buffer);
	free(loc_buffer);
	close_and_drain(playback_handle);
}
