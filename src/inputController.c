/*
 * inputController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
 */

#include "defines.h"
#include "inputController.h"

int setUpInputDevices(void)
{
	int ret = TRUE;
	return ret;
}

int tearDownInputDevices(void)
{
	int ret = TRUE;
	return ret;
}

enum SwitchState getSwitchState(int id)
{
	enum SwitchState state = switch_off;
	return state;
}

enum ButtonState getButtonState(int id)
{
	enum ButtonState state = button_off;
	return state;
}
