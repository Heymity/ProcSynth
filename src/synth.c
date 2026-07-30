#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <alsa/asoundlib.h>
#include <procSynth/synth.h>

#define DURATION_SEC 40.0/1000.0
#define FREQUENCY 440.0
#define VOLUME 30000.0 // Max for 16-bit signed is 32767


void write_buffer(snd_pcm_t* playback_handle, short* buffer, const long length) {
	int frames_written = snd_pcm_writei(playback_handle, buffer, length);
	if (frames_written < 0) {
		frames_written = snd_pcm_recover(playback_handle, frames_written, 0);
	}

	free(buffer);
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
							 1,      // Allow software resampling
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

	if (open_and_configure_interface(&playback_handle) != 0) {
		return;
	}

	double phase = 0.0;

	for (int j = 0; j < 1000; j++) {
		const int total_frames = (int)(SAMPLE_RATE * DURATION_SEC);
		short *buffer = malloc(total_frames * CHANNELS * sizeof(short));
		const double current_freq = FREQUENCY + j;
		for (int i = 0; i < total_frames; i++) {
			const double current_amp = VOLUME;
			const short sample = (short)(current_amp * sin(phase));

			// Incrementa a fase continuamente
			double phase_increment = (2.0 * M_PI * current_freq) / SAMPLE_RATE;
			phase += phase_increment;

			// Evita que a variável phase cresça infinitamente
			if (phase >= 2.0 * M_PI) {
				phase -= 2.0 * M_PI;
			}

			buffer[i * 2] = sample;     // Left channel
			buffer[i * 2 + 1] = sample; // Right channel
		}

		write_buffer(playback_handle, buffer, total_frames);
	}

	close_and_drain(playback_handle);
}
