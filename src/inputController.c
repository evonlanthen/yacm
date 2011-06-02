/*
 * inputController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
 */

#include <stdio.h>
#include "defines.h"
#include "types.h"
#include "mmap.h"
#include "inputController.h"

#ifdef CARME
 #include "carme.h"
#elif defined(ORCHID)
 #include "orchid.h"
#else
 #error "no board defined!"
#endif

static int isInputControllerSetUp = FALSE;
extern void *mmap_base;
extern void *mmap_base2;

int setUpInputController(void)
{
	// check if input controller is already set up:
	if (isInputControllerSetUp) {
		return FALSE;
	}

	// check if memory mapping is already set up, if not try to set it up:
	if (!getMmapSetUpState()) {
		if (!setUpMmap()) {
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
	UINT8 buttons;

	if (!isInputControllerSetUp) {
		return button_unknown;
	}

	/* We use the GPIO functions for Orchid and CARME board.
	 * It is important to read twice, else the result would not be reliable
	 * (usleep for some microseconds would work as well) */
	GPIO_read_button();
	buttons = GPIO_read_button();
	printf("Button value = 0x%02x\n", buttons);

    if (buttons & id) {
    	return button_on;
    } else {
    	return button_off;
    }
}
