/**
 * Create timer objects
 *
 * @file    timer.c
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 26, 2011
 */

#include <sys/time.h>
#include "defines.h"
#include "timer.h"

TimerDescriptor setUpTimer(unsigned int time) {
	TimerDescriptor timerDescriptor;
	struct timeval tv;

	if (time == 0) {
		timerDescriptor.startTime = 0;
		timerDescriptor.endTime = 0;
	} else {
		// get current time as timeval structure:
		gettimeofday(&tv, NULL);

		// get time in milliseconds and save it as start time:
		timerDescriptor.startTime = (tv.tv_sec*1000) + (tv.tv_usec/1000);
		// set end time:
		timerDescriptor.endTime = timerDescriptor.startTime + time;
	}
	// return structure:
	return timerDescriptor;
}

int isTimerElapsed(TimerDescriptor timerDescriptor) {
	struct timeval tv;
	unsigned long curTime;

	// get current time as timeval structure:
	gettimeofday(&tv, NULL);

	// get time in milliseconds and save it as start time:
	curTime = (tv.tv_sec*1000) + (tv.tv_usec/1000);
	// check if timer is elapsed:
	if (curTime >= timerDescriptor.endTime) {
		return TRUE;
	}
	return FALSE;
}
