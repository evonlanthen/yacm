/*
 * machineController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
 */

#include <stdio.h>

#include "defines.h"
#include "mmap.h"
#include "machineController.h"
#include "timer.h"

static TIMER timer;
static int isMachineControllerSetUp = FALSE;

int setUpMachineController(void)
{
	// check if machine controller is already set up:
	if (isMachineControllerSetUp) {
		return FALSE;
	}

	// check if memory mapping is already set up, if not try to set it up:
	if (!getMmapSetUpState()) {
		if (!setUpMmap()) {
			return FALSE;
		}
	}

	isMachineControllerSetUp = TRUE;
	return TRUE;
}

int tearDownMachineController(void) {
	// check if machine controller was already torn down:
	if (!isMachineControllerSetUp) {
		return FALSE;
	}

	isMachineControllerSetUp = FALSE;
	return TRUE;
}

int startMachine(void)
{
	if (!isMachineControllerSetUp) {
		return FALSE;
	}
	timer = setUpTimer(COFFEE_MAKING_TIME);
	return TRUE;
}

int stopMachine(void)
{
	if (!isMachineControllerSetUp) {
		return FALSE;
	}
	timer = NULL;
	return TRUE;
}

int machineRunning(void)
{
	if (!isMachineControllerSetUp) {
		return FALSE;
	}
	if (isTimerElapsed(timer)) {
		return FALSE;
	} else {
		return TRUE;
	}
}
