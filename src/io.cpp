//
// Created by GABRIEL on 03/08/2026.
//

#include <procSynth/io.h>
#include <alsa/asoundlib.h>
#include <pthread.h>
#include <cmath>
#include <procSynth/synth.h>

#include "procSynth/presets.h"
#include "procSynth/utils.h"
#include "procSynth/voice.h"
#include <wiringPi.h>

#include <Freenove/MatrixKeypad/include/Keypad.hpp>

const byte KEYPADROWS = 4;
const byte KEYPADCOLS = 4;
char keypadkeys[KEYPADROWS][KEYPADCOLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
static byte KeypadRowPins[KEYPADROWS] = {16, 20, 21, 26};
static byte KeypadColPins[KEYPADCOLS] = {19, 13, 6, 5};
static Keypad keypad = Keypad( makeKeymap(keypadkeys), KeypadRowPins, KeypadColPins, KEYPADROWS, KEYPADCOLS );

static Envelope midi_envelope;
static Timbre midi_timbre;

void handle_keypad_matrix (char key);

void * midi_thread_func(void * _) {
	(void)_; // Suppress unused param warning
	print_formated("Initializing MIDI Input thread\n");
	snd_seq_t *seq_handle;
	
	midi_envelope = PianoEnvelope_Preset;
	midi_timbre = PianoTimbre_Preset;

	if (snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
		print_formated("Error opening ALSA Sequencer\n");
		return nullptr;
	}

	snd_seq_set_client_name(seq_handle, "ProcSynth MIDI Input");

	snd_seq_create_simple_port(seq_handle, "ProcSynth Port",
										  SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
										  SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);


	print_formated("Ready to receive MIDI input. Connect device via terminal using 'aconnect -l' and 'aconnect XX YY'.\n");
	if (const int res = system("aconnect -l"); res < 0) {
		print_formated("aconnect -l returned error\n");
	}
	snd_seq_event_t *ev = nullptr;

	int noteToVoice[256];
	int internalVolume = 50;

	memset(noteToVoice, 0, sizeof(noteToVoice));

	while (snd_seq_event_input(seq_handle, &ev) >= 0) {
		if (ev->type == SND_SEQ_EVENT_NOTEON) {
			//print_formated("Note: %d - Vel: %d - Tag: %c - Type: %d\n", ev->data.note.note, ev->data.note.velocity, ev->tag, ev->type);
			const int note = ev->data.note.note;
			const int velocity = ev->data.note.velocity;

			const double freq = 440.0 * pow(2.0, (note - 69.0) / 12.0);

			const auto amplitude = static_cast<short>((velocity / 127.0) * (internalVolume / 127.0) * MAX_VOLUME);

			if (velocity > 0) {
				noteToVoice[ev->data.note.note] = press_key(freq, amplitude, midi_envelope, midi_timbre);
			} else {
				release_key(noteToVoice[ev->data.note.note]);
			}
		}
		else if (ev->type == SND_SEQ_EVENT_NOTEOFF) {
			//print_formated("Note: %d - Vel: %d - Tag: %c - Type: %d\n", ev->data.note.note, ev->data.note.velocity, ev->tag, ev->type);
			release_key(noteToVoice[ev->data.note.note]);
		} else if (ev->type == SND_SEQ_EVENT_PGMCHANGE) {
			//print_formated("Value: %d\n", ev->data.control.value);
			internalVolume =  ev->data.control.value;
		}

		snd_seq_free_event(ev);
	}

	return nullptr;
}

void * io_thread(void * _) {
	(void) _;
	
	print_formated("Initializing MIDI Input thread\n");
    wiringPiSetupGpio();
	keypad.setDebounceTime(50);
	
	while (1) {
		char key = keypad.getKey();
		if (key) {
			handle_keypad_matrix(key);
		}
				
	}
	
	return NULL;
}

void handle_keypad_matrix (char key) {
	switch(key){
		case '1':
			midi_envelope = PianoEnvelope_Preset;
			midi_timbre = PianoTimbre_Preset;
			break;
		case '2':
			midi_envelope = ViolinEnvelope_Preset;
			midi_timbre = ViolinTimbre_Preset;
			break;
		case '3':
			midi_envelope = GuitarEnvelope_Preset;
			midi_timbre = GuitarTimbre_Preset;
			break;
		case '4':
			midi_envelope = TromboneEnvelope_Preset;
			midi_timbre = TromboneTimbre_Preset;
			break;
		case '5':
			midi_envelope = ClarinetEnvelope_Preset;
			midi_timbre = ClarinetTimbre_Preset;
			break;
		case '6':
			midi_envelope = HarpEnvelope_Preset;
			midi_timbre = HarpTimbre_Preset;
			break;
		case '7':
			midi_envelope = OrganEnvelope_Preset;
			midi_timbre = OrganTimbre_Preset;
			break;
		case '8':
			midi_envelope = DrumEnvelope_Preset;
			midi_timbre = DrumTimbre_Preset;
			break;
		case '9':
			midi_envelope = BassEnvelope_Preset;
			midi_timbre = BassTimbre_Preset;
		break;
	}
	
		
}

