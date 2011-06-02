/*
 * inputController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
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

int tearDownInputController(void)
{
	// check if input controller was already torn down:
	if (!isInputControllerSetUp) {
		return FALSE;
	}
	isInputControllerSetUp = FALSE;
	return TRUE;
}

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

    if (switches & id) {
    	return switch_on;
    } else {
    	return switch_off;
    }
}

enum ButtonState getButtonState(int id)
{
	UINT8 button;

	if (!isInputControllerSetUp) {
		return button_unknown;
	}

#ifdef CARME
	button = (UINT8) readGPIOButton(id);
	if (button == 1) {
		return button_on;
	} else {
		return button_off;
	}
#elif defined(ORCHID)
	/* It is important to read twice, else the result would not be reliable
	 * (usleep for some microseconds would work as well) */
	GPIO_read_button();
	button = GPIO_read_button();

    if (button & id) {
    	return button_on;
    } else {
    	return button_off;
    }
#endif
}
