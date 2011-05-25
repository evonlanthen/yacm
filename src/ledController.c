/**
 * @file   ledController.c
 * @author Elmar Vonlanthen (vonle1@bfh.ch)
 * @date   May 20, 2011
 * @brief  Controls the LEDs.
 *
 * Initializes and controls the LEDs.
 */

#include <stdio.h>

#include "defines.h"
#include "types.h"
#include "ledController.h"

#ifdef CARME
 #include "carme.h"
#elif defined(ORCHID)
 #include "orchid.h"
#else
 #error "no board defined!"
#endif

typedef struct {
	int id;
	int on;
	int off;
} LedInterval;

int ledStates;
LedInterval ledIntervals[NUM_OF_LEDS];

int initLedController(void)
{
	int i, id;
	for (i = 0; i < NUM_OF_LEDS; i++) {
		id = LED_ID(id+1);
		ledIntervals[i].id = id;
		ledIntervals[i].on = 0;
		ledIntervals[i].off = 0;
		updateLed(id, led_off);
	}
	ledStates = led_off;
	return TRUE;
}

int cleanUpLedController(void)
{
	int i, id;
	for (i = 0; i < NUM_OF_LEDS; i++) {
		id = LED_ID(i+1);
		updateLed(id, led_off);
	}
	ledStates = led_off;
	return TRUE;
}

int updateLed(int id, enum LedState state)
{
	int curStates;

	curStates = ledStates & id;
	switch(state) {
	case led_blinking:
		// not every instruction, use intervals!!!
		ledStates &= ~curStates;
		break;
	case led_on:
		ledStates |= id;
		break;
	case led_off:
		ledStates &= ~id;
		break;
	default:
		return FALSE;
	}

	#ifdef CARME
		*(volatile unsigned char *) (mmap_base + LED_OFFSET) = ledStates;
	#elif defined(ORCHID)
		GPIO_write_led(ledStates);
	#endif
	return TRUE;
}

int setBlinkingFreq(int id, int intervalOn, int intervalOff)
{
	if (id > NUM_OF_LEDS) {
		return FALSE;
	}
	ledIntervals[id].on = intervalOn;
	ledIntervals[id].off = intervalOff;
	return TRUE;
}
