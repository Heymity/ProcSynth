//
// Created by GABRIEL on 01/08/2026.
//

#include <procSynth/utils.h>

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

char* ThreadsNames[N_THREADS];
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
