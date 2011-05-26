/*
 * sensorController.c
 *
 *  Created on: May 20, 2011
 *      Author: elmar
 */

#include "defines.h"
#include "sensorController.h"

int setUpSensorController(void)
{
	int ret = TRUE;
	return ret;
}

int tearDownSensorController(void)
{
	int ret = TRUE;
	return ret;
}

enum SensorState getSensorState(int id)
{
	enum SensorState state = sensor_normal;
	return state;
}
