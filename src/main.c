/*
 * Build platform: Raspberry pi 3, 64-bit-quad-core ARM Cortex-A53
 */

#include<stdio.h>
#include<stdlib.h>
#include<procSynth/synth.h>

char hello_msg[] =	"\n	░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n"
					"	░█▀█░█▀▄░█▀█░█▀▀░█▀▀░█░█░█▀█░▀█▀░█░█░\n"
					"	░█▀▀░█▀▄░█░█░█░░░▀▀█░░█░░█░█░░█░░█▀█░\n"
					"	░▀░░░▀░▀░▀▀▀░▀▀▀░▀▀▀░░▀░░▀░▀░░▀░░▀░▀░\n"
					"	░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░\n";

int main() {
	//system("clear");
	printf("%s\n\n", hello_msg);

	synth();
}
