/*
 * sensorController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
 */

#include <unistd.h>

#include "defines.h"
#include "types.h"
#include "hardwareController.h"
#include "sensorController.h"

#ifdef CARME
 #include "carme.h"
#elif defined(ORCHID)
 #include "orchid.h"
#else
 #error "no board defined!"
#endif

static int isSensorControllerSetUp = FALSE;

int setUpSensorController(void)
{
	// check if sensor controller is already set up:
	if (isSensorControllerSetUp) {
		return FALSE;
	}

	// check if hardware is already initialized:
	if (!getHardwareSetUpState()) {
		if (!setUpHardwareController()) {
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
	UINT8 sensors;
	if (!isSensorControllerSetUp) {
		return sensor_unknown;
	}

#ifdef CARME
	sensors = *(volatile unsigned char *) (mmap_base + SWITCH_OFFSET);
#elif defined(ORCHID)
	// it is important to read twice, else the result would not be reliable
	// (usleep for some microseconds would work as well):
	GPIO_read_switch();
    sensors = GPIO_read_switch();
#endif

    if (sensors & id) {
    	return sensor_alert;
    } else {
    	return sensor_normal;
    }
}
