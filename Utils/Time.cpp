#include "Time.h"
#include <time.h>

static clock_t timer;

Time::Time() {
	timer = clock();
}

double Time::getCurrentTime() {
	return clock() - timer;
}


