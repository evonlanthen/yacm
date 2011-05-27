/*
 * inputController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
 */

#include "defines.h"
#include "mmap.h"
#include "inputController.h"

static int isInputControllerSetUp = FALSE;

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
	if (!isInputControllerSetUp) {
		return switch_unknown;
	}
	enum SwitchState state = switch_off;
	return state;
}

enum ButtonState getButtonState(int id)
{
	if (!isInputControllerSetUp) {
		return button_unknown;
	}
	enum ButtonState state = button_off;
	return state;
}
