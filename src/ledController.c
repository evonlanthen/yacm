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
#include "timer.h"

#ifdef CARME
 #include "carme.h"
#elif defined(ORCHID)
 #include "orchid.h"
#else
 #error "no board defined!"
#endif

typedef struct {
	int id;
	int state;
	int durationOn;
	int durationOff;
	int blinkingState;
	TimerDescriptor timer;
} LedDescriptor;

extern void *mmap_base;
static LedDescriptor leds[NUM_OF_LEDS];
static isLedControllerSetUp = FALSE;

int setUpLedController(void)
{
	// check if led controller is already set up:
	if (isLedControllerSetUp) {
		return FALSE;
	}

	// check if memory mapping is already set up, if not try to set it up:
	if (!getMmapSetUpState()) {
		if (!setUpMmap()) {
			return FALSE;
		}
	}

	int id;
	for (int i = 0; i < NUM_OF_LEDS; i++) {
		id = LED_ID(i+1);
		leds[i].id = id;
		leds[i].state = led_off;
		leds[i].durationOn = 1000;
		leds[i].durationOff = 1000;
		leds[i].blinkingState = led_off;
		leds[i].timer = setUpTimer(0);
	}
	updateAllLeds();
	isLedControllerSetUp = TRUE;
	return TRUE;
}

int tearDownLedController(void)
{
	int id;
	for (int i = 0; i < NUM_OF_LEDS; i++) {
		id = LED_ID(i+1);
		leds[i].state = led_off;
		leds[i].durationOn = 0;
		leds[i].durationOff = 0;
		leds[i].blinkingState = led_off;
		leds[i].timer = setUpTimer(0);
	}
	updateAllLeds();
	isLedControllerSetUp = FALSE;
	return TRUE;
}

int updateBlinkingState(LedDescriptor *led) {
	if (!isLedControllerSetUp) {
		return FALSE;
	}
	// check if current timer is elapsed:
	if (isTimerElapsed(led->timer)) {
		// state was led on, set new timer for duration led off:
		if (led->blinkingState == led_on) {
			led->blinkingState = led_off;
			led->timer = setUpTimer(led->durationOff);
		// state was led off, set new timer for duration led on:
		} else {
			led->blinkingState = led_on;
			led->timer = setUpTimer(led->durationOn);
		}
	}
	return TRUE;
}

int updateAllLeds() {
	if (!isLedControllerSetUp) {
		return FALSE;
	}
	int ret = TRUE, newStates = 0;
	LedDescriptor *led;

	// update structure:
	for (int i = 0; i < NUM_OF_LEDS; i++) {

		if (leds[i].state == led_on) {
			newStates |= leds[i].id;
		// update blinking states an get new states:
		} else if (leds[i].state == led_blinking) {
			led = &leds[i];
			updateBlinkingState(led);
			if (leds[i].blinkingState == led_on) {
				newStates |= leds[i].id;
			}
		}
	}

	#ifdef CARME
		*(volatile unsigned char *) (mmap_base + LED_OFFSET) = newStates;
	#elif defined(ORCHID)
		GPIO_write_led(newStates);
	#endif
	return ret;
}

int updateLed(int id, enum LedState state)
{
	if (!isLedControllerSetUp) {
		return FALSE;
	}
	// update structure:
	for (int i = 0; i < NUM_OF_LEDS; i++) {
		// update led state for specified id in structure:
		if (leds[i].id == id) {
			leds[i].state = state;
			// now update all leds:
			updateAllLeds();
			return TRUE;
		}
	}
	return FALSE;
}

int setBlinkingFreq(int id, int durationOn, int durationOff)
{
	if (!isLedControllerSetUp) {
		return FALSE;
	}
	for (int i = 0; i < NUM_OF_LEDS; i++) {
		if (leds[i].id == id) {
			leds[i].durationOn = durationOn;
			leds[i].durationOff = durationOff;
			return TRUE;
		}
	}
	return FALSE;
}
