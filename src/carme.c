/**
 * <short description>
 *
 * <long description>
 *
 * @file    gpioCarme.c
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    Jun 2, 2011
 */

#include <stdio.h>
#include <fcntl.h>

#include "defines.h"
#include "inputController.h"
#include "carme.h"

static int exportGPIO(int id) {
	int exportFD;

	exportFD = open("/sys/class/gpio/export", O_WRONLY);
	if (exportFD < 0) {
		printf("Cannot open GPIO to export %d\n", id);
		return FALSE;
	}
	write(exportFD, id, 4);
	close(exportFD);
	return TRUE;
}

static int setGPIODirection(int id)
{
	int directionFD;
	char *directionFileName;

	/* Set direction to 'in' for reading its values */
	directionFileName = sprintf("/sys/class/gpio/gpio%d/direction", id);
	directionFD = open(directionFileName, O_WRONLY);
	if (directionFD < 0) {
		printf("Cannot open GPIO direction for %d\n", id);
		return FALSE;
	}
	write(directionFD, "in", 4);
	close(directionFD);
	return TRUE;
}
int initGPIO(void) {
	/* Export button GPIOs */
	exportGPIO(BUTTON_1);
	exportGPIO(BUTTON_2);
	exportGPIO(BUTTON_3);
	exportGPIO(BUTTON_4);

	/* Set button GPIO directions to in */
	setGPIODirection(BUTTON_1);
	setGPIODirection(BUTTON_2);
	setGPIODirection(BUTTON_3);
	setGPIODirection(BUTTON_4);
}

int readGPIOValue(int id) {
	int valueFD;
	char *valueFileName;
	volatile int value;

	valueFileName = sprintf("/sys/class/gpio/gpio%d/value", id);
	valueFD = open(valueFileName, O_RDONLY);
	if (valueFD < 0) {
		printf("Cannot open GPIO value for %d\n", id);
		return FALSE;
	}
	read(valueFD, &value, 2);
	close(valueFD);

	return value;
}
