//
// Created by GABRIEL on 03/08/2026.
//

#include <procSynth/io.h>
#include <alsa/asoundlib.h>
#include <pthread.h>
#include <math.h>
#include <procSynth/synth.h>

#include "procSynth/presets.h"
#include "procSynth/utils.h"



void * midi_thread_func(void* _) {
	print_formated("Initializing MIDI Input thread\n");
	snd_seq_t *seq_handle;

	if (snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
		print_formated("Error opening ALSA Sequencer\n");
		return NULL;
	}

	snd_seq_set_client_name(seq_handle, "ProcSynth MIDI Input");

	snd_seq_create_simple_port(seq_handle, "ProcSynth Port",
										  SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
										  SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);


	print_formated("Ready to receive MIDI input. Connect device via terminal using 'aconnect -l' and 'aconnect XX YY'.\n");
	int res = system("aconnect -l");
	if (res < 0) {
		print_formated("aconnect -l returned error\n");
	}
	snd_seq_event_t *ev = NULL;

	int noteToVoice[256];
	int internalVolume = 50;

	memset(noteToVoice, 0, sizeof(noteToVoice));

	while (snd_seq_event_input(seq_handle, &ev) >= 0) {
		print_formated("Note: %d - Vel: %d - Tag: %c - Type: %d\n", ev->data.note.note, ev->data.note.velocity, ev->tag, ev->type);
		if (ev->type == SND_SEQ_EVENT_NOTEON) {
			int note = ev->data.note.note;
			int velocity = ev->data.note.velocity;

			double freq = 440.0 * pow(2.0, (note - 69.0) / 12.0);

			short amplitude = (short)((velocity/127.0) * (internalVolume/127.0) * MAX_VOLUME);

			if (velocity > 0) {
				noteToVoice[ev->data.note.note] = press_key(freq, amplitude, PianoEnvelope_Preset, PianoTimbre_Preset);
			} else {
				release_key(noteToVoice[ev->data.note.note]);
			}
		}
		else if (ev->type == SND_SEQ_EVENT_NOTEOFF) {
			release_key(noteToVoice[ev->data.note.note]);
		} else if (ev->type == SND_SEQ_EVENT_PGMCHANGE) {
			print_formated("Value: %d", ev->data.control.value);
			internalVolume =  ev->data.control.value;
		}

		snd_seq_free_event(ev);
	}

	return NULL;
}
