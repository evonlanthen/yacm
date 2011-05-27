/**
 * Create timer objects
 *
 * @file    timer.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 26, 2011
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef struct {
	unsigned long startTime;
	unsigned long endTime;
} TimerDescriptor;

/**
 * Set up timer
 *
 * Starts the timer and returns timer description structure
 *
 * @param time Time in milliseconds
 * @return Returns timer description structure
 */
TimerDescriptor setUpTimer(unsigned int time);

/**
 * Check if timer is elapsed.
 *
 * @param TimerDescriptor Timer descriptor structure
 * @return Returns TRUE if time is elapsed and FALSE if not
 */
int isTimerElapsed(TimerDescriptor timerDescriptor);

/**
 * Check if timer is elapsed.
 *
 * @param TimerDescriptor Timer descriptor structure
 * @return Returns TRUE if time is elapsed and FALSE if not
 */
int isTimerElapsed(TimerDescriptor timerDescriptor);

#endif /* TIMER_H_ */
