/**
 * Initializes and controls the LEDs.
 *
 * @file   ledController.c
 * @author Elmar Vonlanthen (vonle1@bfh.ch)
 * @date   May 20, 2011
 */

#include <stdio.h>

#include "defines.h"
#include "types.h"
#include "timer.h"
#include "hardwareController.h"
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
	int state;
	int durationOn;
	int durationOff;
	int blinkingState;
	TIMER timer;
} LedDescriptor;

static LedDescriptor leds[NUM_OF_LEDS];
static int isLedControllerSetUp = FALSE;

/**
 * @copydoc setUpLedController
 */
int setUpLedController(void)
{
	// check if led controller is already set up:
	if (isLedControllerSetUp) {
		return FALSE;
	}

	// check if hardware is already initialized:
	if (!getHardwareSetUpState()) {
		if (!setUpHardwareController()) {
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
		leds[i].timer = NULL;
	}
	updateAllLeds();
	isLedControllerSetUp = TRUE;
	return TRUE;
}

/**
 * @copydoc tearDownLedController
 */
int tearDownLedController(void)
{
	int id;
	for (int i = 0; i < NUM_OF_LEDS; i++) {
		id = LED_ID(i+1);
		leds[i].state = led_off;
		leds[i].durationOn = 0;
		leds[i].durationOff = 0;
		leds[i].blinkingState = led_off;
		abortTimer(leds[i].timer);
		leds[i].timer = NULL;
	}
	updateAllLeds();
	isLedControllerSetUp = FALSE;
	return TRUE;
}

/**
 * @copydoc updateBlinkingState
 */
static int updateBlinkingState(LedDescriptor *led) {
	if (!isLedControllerSetUp) {
		return FALSE;
	}
	// check if current timer is elapsed:
	if (led->timer == NULL || isTimerElapsed(led->timer)) {
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

/**
 * @copydoc updateAllLeds
 */
int updateAllLeds() {
	if (!isLedControllerSetUp) {
		return FALSE;
	}
	UINT8 newStates = 0;
	int ret = TRUE;
	LedDescriptor *led;

	// Read all values from leds structure and set the leds according to the
	// configured state. Link the state of each value with the variable
	// newStates ('OR operation') and update all leds at once
	for (int i = 0; i < NUM_OF_LEDS; i++) {

		if (leds[i].state == led_on) {
			newStates |= leds[i].id;
		// update blinking states an get new states:
		} else if (leds[i].state == led_blinking) {
			led = &leds[i];
			// if the state is 'blinking' we let the function updateBlinkingState
			// decide if the led has to be on or off. The state is stored in the
			// structure value 'blinkingState'
			updateBlinkingState(led);
			if (leds[i].blinkingState == led_on) {
				newStates |= leds[i].id;
			}
		}
	}

	// Now update the LEDs:
	#ifdef CARME
		*(volatile unsigned char *) (mmap_base + LED_OFFSET) = newStates;
	#elif defined(ORCHID)
		GPIO_write_led(newStates);
	#endif
	return ret;
}

/**
 * @copydoc updateLed
 */
int updateLed(int id, enum LedState state)
{
	if (!isLedControllerSetUp) {
		return FALSE;
	}
	// we change only the value in the led structure and use the function
	// updateAllLeds() to really change the LED states:
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

/**
 * @copydoc setBlinkingFreq
 */
int setBlinkingFreq(int id, int durationOn, int durationOff)
{
	if (!isLedControllerSetUp) {
		return FALSE;
	}
	// change the blinking frequence for one led:
	for (int i = 0; i < NUM_OF_LEDS; i++) {
		if (leds[i].id == id) {
			// duration in milliseconds while the led is on:
			leds[i].durationOn = durationOn;
			// duration in milliseconds while the led is off:
			leds[i].durationOff = durationOff;
			return TRUE;
		}
	}
	return FALSE;
}
