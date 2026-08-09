//
// Created by GABRIEL on 01/08/2026.
//

#ifndef PROCSYNTH_UTILS_H
#define PROCSYNTH_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#define N_THREADS 5
#include <pthread.h>



extern char* ThreadsNames[N_THREADS];
extern pthread_t ThreadsId[N_THREADS];

void print_formated(const char *format, ...) __attribute__((format(printf, 1, 2)));
int read_analog(int _fd, int chn);
int detectI2C(int addr);

#ifdef __cplusplus
}
#endif

#endif // PROCSYNTH_UTILS_H
