/*
 * sensorController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
 */

#include "defines.h"
#include "mmap.h"
#include "sensorController.h"

static int isSensorControllerSetUp = FALSE;

int setUpSensorController(void)
{
	// check if sensor controller is already set up:
	if (isSensorControllerSetUp) {
		return FALSE;
	}

	// check if memory mapping is already set up, if not try to set it up:
	if (!getMmapSetUpState()) {
		if (!setUpMmap()) {
			return FALSE;
		}
	}
	isSensorControllerSetUp = TRUE;
	return TRUE;
}

int tearDownSensorController(void)
{
	// check if sensor controller was already torn down:
	if (!isSensorControllerSetUp) {
		return FALSE;
	}
	isSensorControllerSetUp = FALSE;
	return TRUE;
}

enum SensorState getSensorState(int id)
{
	if (!isSensorControllerSetUp) {
		return sensor_unknown;
	}
	enum SensorState state = sensor_normal;
	return state;
}
