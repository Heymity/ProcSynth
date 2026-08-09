//
// Created by GABRIEL on 01/08/2026.
//

#include <procSynth/utils.h>

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wiringPiI2C.h>

char * ThreadsNames[N_THREADS];
pthread_t ThreadsId[N_THREADS];

void print_formated(const char *format, ...) {
	pthread_t current_id = pthread_self();
	time_t raw_time = time(NULL);
	struct tm *local_time = localtime(&raw_time);

	char buffer[10];
	strftime(buffer, sizeof(buffer), "%H:%M:%S", local_time);

	char* name_buffer = "Unknown";
	for (int i = 0; i < N_THREADS; i++) {
		if (ThreadsId[i] == current_id)
			name_buffer = ThreadsNames[i];
	}

	va_list args;
	va_start(args, format);
	printf("<%s> [%s] ", buffer, name_buffer);
	vprintf(format, args);
	va_end(args);
}

int detectI2C(int addr){
	int _fd = wiringPiI2CSetup(addr);
	if (_fd < 0){
		print_formated("I2C Address error: 0x%x \n",addr);
		return 0 ;
	}

	if (wiringPiI2CWrite(_fd, 0) < 0) {
		print_formated("No device found at address 0x%x \n", addr);
		return 0;
	}

	print_formated("Found device in address 0x%x \n", addr);
	return _fd;
}

int read_analog(int _fd, int chn){
	wiringPiI2CWrite(_fd, 0x84|(((chn<<2 | chn>>1)&0x07)<<4));
	return wiringPiI2CRead(_fd);
}