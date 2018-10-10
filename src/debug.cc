// debug.c
// the actual debugging functions

#include <stdio.h>

#include "debug.h"

long long int count = 0;
long long int pmiss = 0;

void miss_split(long long int miss) {

	#define SPLIT	100000
	if (! (++count % SPLIT)) {
		printf("[INFO] misses=%lld,t_off_start=%lld,t_off_end=%lld\n",
			   miss - pmiss, count - SPLIT, count - 1);

		pmiss = miss;
	}
}
