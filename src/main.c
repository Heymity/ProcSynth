/*
 * Build platform: Raspberry pi 3, 64-bit-quad-core ARM Cortex-A53
 */

#include <procSynth/synth.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "procSynth/presets.h"
#include "procSynth/utils.h"
#include <procSynth/io.h>


static char hello_msg[] =	"\n	░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n"
							"	░░█▀█░█▀▄░█▀█░█▀▀░█▀▀░█░█░█▀█░▀█▀░█░█░░\n"
							"	░░█▀▀░█▀▄░█░█░█░░░▀▀█░░█░░█░█░░█░░█▀█░░\n"
							"	░░▀░░░▀░▀░▀▀▀░▀▀▀░▀▀▀░░▀░░▀░▀░░▀░░▀░▀░░\n"
							"	░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n";

int main() {
	//system("clear");
	printf("%s\n\n", hello_msg);

	ThreadsId[0] = pthread_self();
	ThreadsNames[0] = "Main Thread";

	/* PTHREAD PRIORITY CONFIG - REQUIRES SUDO
	pthread_attr_t attr;
	struct sched_param param;

	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

	param.sched_priority = 90;
	pthread_attr_setschedparam(&attr, &param);

	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	*/
	ThreadsNames[1] = "Synth Thread";
	if (pthread_create(&ThreadsId[1], NULL, start_synth_thread, NULL) != 0) {
		print_formated("Error creating synth thread. Aborting... \n");
		return 1;
	}
	print_formated("Synth thread created \n");

	//pthread_attr_destroy(&attr);

	ThreadsNames[2] = "MIDI input Thread";
	if (pthread_create(&ThreadsId[2], NULL, midi_thread_func, NULL) != 0) {
		print_formated("Error creating MIDI input thread. Aborting... \n");
		return 1;
	}
	print_formated("MIDI input thread created \n");

	sleep(1);

	print_formated("Pressing C major chord for 1s as Piano for test \n");
	int voiceC = press_key(NOTE_C4, 10000, PianoEnvelope_Preset, PianoTimbre_Preset);
	int voiceE = press_key(NOTE_E4, 10000, PianoEnvelope_Preset, PianoTimbre_Preset);
	int voiceG = press_key(NOTE_G4, 10000, PianoEnvelope_Preset, PianoTimbre_Preset);
	sleep(1);
	release_key(voiceC);
	release_key(voiceE);
	release_key(voiceG);
	print_formated("Key released \n");

	for (int i = 0; i < N_THREADS; i++) {
		pthread_join(ThreadsId[i], NULL);
	}
}
