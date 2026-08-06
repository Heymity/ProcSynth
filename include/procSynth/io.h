//
// Created by GABRIEL on 03/08/2026.
//

#ifndef PROCSYNTH_IO_H
#define PROCSYNTH_IO_H

#ifdef __cplusplus
extern "C" {
#endif

void * midi_thread_func(void* _);

void * io_thread(void * _);

#ifdef __cplusplus
  }
#endif

#endif // PROCSYNTH_IO_H
