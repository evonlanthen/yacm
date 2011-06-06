/**
 * Input controlling
 *
 * Initialize and read buttons and switches.
 *
 * @file    inputController.c
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 27, 2011
 */

#include <stdio.h>
#include "defines.h"
#include "types.h"
#include "hardwareController.h"
#include "inputController.h"

#ifdef CARME
 #include "carme.h"
#elif defined(ORCHID)
 #include "orchid.h"
#else
 #error "no board defined!"
#endif

static int isInputControllerSetUp = FALSE;

/**
 * @copydoc setUpInputController
 */
int setUpInputController(void)
{
	// check if input controller is already set up:
	if (isInputControllerSetUp) {
		return FALSE;
	}

	// check if hardware is already initialized:
	if (!getHardwareSetUpState()) {
		if (!setUpHardwareController()) {
			return FALSE;
		}
	}
	isInputControllerSetUp = TRUE;
	return TRUE;
}

/**
 * @copydoc tearDownInputController
 */
int tearDownInputController(void)
{
	// check if input controller was already torn down:
	if (!isInputControllerSetUp) {
		return FALSE;
	}
	isInputControllerSetUp = FALSE;
	return TRUE;
}

/**
 * @copydoc getSwitchState
 */
enum SwitchState getSwitchState(int id)
{
	UINT8 switches;

	if (!isInputControllerSetUp) {
		return switch_unknown;
	}

#ifdef CARME
	switches = *(volatile unsigned char *) (mmap_base + SWITCH_OFFSET);
#elif defined(ORCHID)
	// it is important to read twice, else the result would not be reliable
	// (usleep for some microseconds would work as well):
	GPIO_read_switch();
    switches = GPIO_read_switch();
#endif

    // mask value of all switches with switch id to get only the status of the
    // selected switch:
    if (switches & id) {
    	return switch_on;
    } else {
    	return switch_off;
    }
}

/**
 * @copydoc getButtonState
 */
enum ButtonState getButtonState(int id)
{
	UINT8 button;

	if (!isInputControllerSetUp) {
		return button_unknown;
	}

#ifdef CARME
	// on CARME board we read each single button state directly over GPIO
	button = (UINT8) readGPIOButton(id);
	if (button == 1) {
		return button_on;
	} else {
		return button_off;
	}
#elif defined(ORCHID)
	// on ORCHID board we get the states of all buttons.
	// It is important to read twice, else the result would not be reliable
	// (usleep for some microseconds would work as well)
	GPIO_read_button();
	button = GPIO_read_button();

    // mask value of all buttons with button id to get only the status of the
	// selected button
    if (button & id) {
    	return button_on;
    } else {
    	return button_off;
    }
#endif
}
