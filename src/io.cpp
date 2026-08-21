//
// Created by GABRIEL on 03/08/2026.
//

#include <Freenove/MatrixKeypad/include/Keypad.hpp>
#include <alsa/asoundlib.h>
#include <cmath>
#include <procSynth/io.h>
#include <procSynth/synth.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "procSynth/presets.h"
#include "procSynth/utils.h"
#include "procSynth/voice.h"
#include <pcf8574.h>
#include <lcd.h>

#define TRIG_PIN 14
#define ECHO_PIN 15
#define MAX_DISTANCE 220
#define TIMEOUT (MAX_DISTANCE * 60)
#define THEREMIN_CUTOFF_DISTANCE 40.0

int pcf8574_address = 0x27;
#define BASE 64
#define RS      (BASE+0)
#define RW      (BASE+1)
#define EN      (BASE+2)
#define LED     (BASE+3)
#define D4      (BASE+4)
#define D5      (BASE+5)
#define D6      (BASE+6)
#define D7      (BASE+7)

static int lcdHd;

static void handle_keypad_matrix (char key);

/*
 *  MIDI CODE
 */

static Envelope midi_envelope;
static Timbre midi_timbre;

void* midi_thread_func(void *_) {
	(void) _; // Suppress unused param warning
	print_formated("Initializing MIDI Input thread\n");
	snd_seq_t *seq_handle;

	midi_envelope = PianoEnvelope_Preset;
	midi_timbre = PianoTimbre_Preset;

	if (snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
		print_formated("Error opening ALSA Sequencer\n");
		return nullptr;
	}

	snd_seq_set_client_name(seq_handle, "ProcSynth MIDI Input");

	snd_seq_create_simple_port(seq_handle, "ProcSynth Port", SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
							   SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_APPLICATION);


	print_formated(
			"Ready to receive MIDI input. Connect device via terminal using 'aconnect -l' and 'aconnect XX YY'.\n");
	if (const int res = system("aconnect -l"); res < 0) {
		print_formated("aconnect -l returned error\n");
	}
	snd_seq_event_t *ev = nullptr;

	int noteToVoice[256];
	int internalVolume = 50;

	memset(noteToVoice, 0, sizeof(noteToVoice));

	while (snd_seq_event_input(seq_handle, &ev) >= 0) {
		if (ev->type == SND_SEQ_EVENT_NOTEON) {
			// print_formated("Note: %d - Vel: %d - Tag: %c - Type: %d\n", ev->data.note.note, ev->data.note.velocity,
			// ev->tag, ev->type);
			const int note = ev->data.note.note;
			const int velocity = ev->data.note.velocity;

			const double freq = 440.0 * pow(2.0, (note - 69.0) / 12.0);

			const auto amplitude = static_cast<short>((velocity / 127.0) * (internalVolume / 127.0) * MAX_VOLUME);

			if (velocity > 0) {
				noteToVoice[ev->data.note.note] = press_key(freq, amplitude, midi_envelope, midi_timbre);
			} else {
				release_key(noteToVoice[ev->data.note.note]);
			}
		} else if (ev->type == SND_SEQ_EVENT_NOTEOFF) {
			// print_formated("Note: %d - Vel: %d - Tag: %c - Type: %d\n", ev->data.note.note, ev->data.note.velocity,
			// ev->tag, ev->type);
			release_key(noteToVoice[ev->data.note.note]);
		} else if (ev->type == SND_SEQ_EVENT_PGMCHANGE) {
			// print_formated("Value: %d\n", ev->data.control.value);
			internalVolume = ev->data.control.value;
		}

		snd_seq_free_event(ev);
	}

	return nullptr;
}

/*
 *	GENERAL IO CODE
 */

constexpr unsigned char KEYPADROWS = 4;
constexpr unsigned char KEYPADCOLS = 4;
static char keypadkeys[KEYPADROWS][KEYPADCOLS] = {
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};
static unsigned char KeypadRowPins[KEYPADROWS] = {16, 20, 21, 26};
static unsigned char KeypadColPins[KEYPADCOLS] = {19, 13, 6, 5};
static auto keypad = Keypad( makeKeymap(keypadkeys), KeypadRowPins, KeypadColPins, KEYPADROWS, KEYPADCOLS );

static float theremin_low_freq = 0.0;
static float theremin_high_freq = 0.0;
static short theremin_volume = 0;

__attribute__((noreturn)) void* io_thread(void * _) {
	(void) _;
	
	print_formated("Initializing IO thread\n");
	keypad.setDebounceTime(50);

	int _fd_ADC = detectI2C(0x48);
	if (_fd_ADC < 0)
		print_formated("Error initializing ADC\n");

	if(detectI2C(0x27)) pcf8574_address = 0x27;
	else if(detectI2C(0x3F)) pcf8574_address = 0x3F;
	else print_formated("No correct I2C address found, please use command 'i2cdetect -y 1' to check the I2C address! \n");

	pcf8574Setup(BASE,pcf8574_address);
	for(int i = 0; i < 8; i++){
		pinMode(BASE+i,OUTPUT);
	}

	digitalWrite(LED,HIGH);     //turn on LCD backlight
	digitalWrite(RW,LOW);       //allow writing to LCD
	lcdHd = lcdInit(2,16,4,RS,EN,D4,D5,D6,D7,0,0,0,0);// initialize LCD and return “handle” used to handle LCD
	if(lcdHd == -1)
		print_formated("lcdInit failed.\n");

	lcdPosition(lcdHd,0,0);
	lcdPrintf(lcdHd,"Hello!");
	lcdPosition(lcdHd,0,1);
	lcdPrintf(lcdHd,"ProcSynth");

	sleep(1);

	while (true) {
		if (const char key = keypad.getKey()) {
			handle_keypad_matrix(key);
		}

		if (_fd_ADC > 0) {
			theremin_volume = MAX_VOLUME * static_cast<float>(read_analog(_fd_ADC, 4)) / 255.0f;
			theremin_low_freq = static_cast<float>(NOTE_A4/4.0 * pow(2, 2.0*static_cast<float>(read_analog(_fd_ADC, 3)) / 255.0f));
			theremin_high_freq = static_cast<float>(NOTE_A4/1.0 * pow(2, 2.0*static_cast<float>(read_analog(_fd_ADC, 2)) / 255.0f));
			//print_formated("V %d CF %f UF %f\n", theremin_volume, theremin_low_freq, theremin_high_freq);
		}
	}
}

void handle_keypad_matrix (const char key) {
	lcdPosition(lcdHd,0,0);
	lcdPrintf(lcdHd,"Instrument:");
	lcdPosition(lcdHd,0,1);
	switch(key){
		case '1':
			midi_envelope = PianoEnvelope_Preset;
			midi_timbre = PianoTimbre_Preset;
			lcdPrintf(lcdHd,"Piano           ");
			break;
		case '2':
			midi_envelope = ViolinEnvelope_Preset;
			midi_timbre = ViolinTimbre_Preset;
			lcdPrintf(lcdHd,"Violin          ");
			break;
		case '3':
			midi_envelope = GuitarEnvelope_Preset;
			midi_timbre = GuitarTimbre_Preset;
			lcdPrintf(lcdHd,"Guitar          ");
			break;
		case '4':
			midi_envelope = TromboneEnvelope_Preset;
			midi_timbre = TromboneTimbre_Preset;
			lcdPrintf(lcdHd,"Trombone        ");
			break;
		case '5':
			midi_envelope = ClarinetEnvelope_Preset;
			midi_timbre = ClarinetTimbre_Preset;
			lcdPrintf(lcdHd,"Clarinet       ");
			break;
		case '6':
			midi_envelope = HarpEnvelope_Preset;
			midi_timbre = HarpTimbre_Preset;
			lcdPrintf(lcdHd,"Harp          ");
			break;
		case '7':
			midi_envelope = OrganEnvelope_Preset;
			midi_timbre = OrganTimbre_Preset;
			lcdPrintf(lcdHd,"Organ          ");
			break;
		case '8':
			midi_envelope = DrumEnvelope_Preset;
			midi_timbre = DrumTimbre_Preset;
			lcdPrintf(lcdHd,"Drum          ");
			break;
		case '9':
			midi_envelope = BassEnvelope_Preset;
			midi_timbre = BassTimbre_Preset;
			lcdPrintf(lcdHd,"Bass          ");
			break;
		default:
			midi_envelope = PianoEnvelope_Preset;
			midi_timbre = PianoTimbre_Preset;
			lcdPrintf(lcdHd,"Piano          ");
			break;
	}
}

/*
 *	ULTRASONIC SENSOR
 */

static int pulseIn(int pin, int level, int timeout)
{
	timeval tn{}, t0{}, t1{};
	gettimeofday(&t0, nullptr);
	long micros = 0;
	while (digitalRead(pin) != level)
	{
		gettimeofday(&tn, nullptr);
		if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
		micros += (tn.tv_usec - t0.tv_usec);
		if (micros > timeout) return 0;
	}
	gettimeofday(&t1, nullptr);
	while (digitalRead(pin) == level)
	{
		gettimeofday(&tn, nullptr);
		if (tn.tv_sec > t0.tv_sec) micros = 1000000L; else micros = 0;
		micros = micros + (tn.tv_usec - t0.tv_usec);
		if (micros > timeout) return 0;
	}
	if (tn.tv_sec > t1.tv_sec) micros = 1000000L; else micros = 0;
	micros = micros + (tn.tv_usec - t1.tv_usec);
	return static_cast<int>(micros);
}
static float getSonar() {
	digitalWrite(TRIG_PIN,HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIG_PIN,LOW);
	long pingTime = pulseIn(ECHO_PIN, HIGH, TIMEOUT);
	float distance = static_cast<float>(pingTime) * 340.0f / 2.0f / 10000.0f;
	return distance;
}

__attribute__((noreturn)) void* ultrasonic_thread(void* _) {
	(void)_;
	sleep(3);
	print_formated("Initializing Ultrasonic sensor thread\n");

	float distance_filtered = 0;
	int theremin_voice = -1;
	while (true) {
		float d = getSonar();
		if (d <= 0 || d > MAX_DISTANCE) continue;

		float alpha = 0.3f;
		distance_filtered = (1 - alpha) * distance_filtered + alpha * d;	// Basic Low Pass IIR filter

		
		if (theremin_voice != -1 && distance_filtered > THEREMIN_CUTOFF_DISTANCE) {
			release_key(theremin_voice);
			theremin_voice = -1;
			continue;
		}

		double freq = theremin_low_freq + (distance_filtered / THEREMIN_CUTOFF_DISTANCE) * (theremin_high_freq - theremin_low_freq);

		
		//print_formated("Got sonar, %f, %f, voice %d, freq %f\n",d, distance_filtered, theremin_voice, freq);

		if (theremin_voice == -1 && d < THEREMIN_CUTOFF_DISTANCE - 20) {
			theremin_voice = press_key(freq, theremin_volume, ThereminEnvelope_Preset, ThereminTimbre_Preset);
		}
		update_voice(theremin_voice, freq, theremin_volume);
	}
}

