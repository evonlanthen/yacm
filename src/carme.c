/**
 * <short description>
 *
 * <long description>
 *
 * @file    carme.c
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    Jun 2, 2011
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "defines.h"
#include "inputController.h"
#include "carme.h"

/**
 * GPIO export states
 */
enum GPIOExportValue {
	gpio_unexport = 0, /**< gpio_unexport */
	gpio_export        /**< gpio_export   */
};

/**
 * @copydoc exportGPIO
 */
static int exportGPIO(int id, enum GPIOExportValue val) {
	int exportFD;
	char exportFileName[50];
	char idStr[4];

	/* open gpio file handle according to the specific action
	 * (export or unexport) */
	if (val == gpio_export) {
		strcpy(exportFileName, "/sys/class/gpio/export");
	} else {
		strcpy(exportFileName, "/sys/class/gpio/unexport");
	}
	exportFD = open(exportFileName, O_WRONLY);
	if (exportFD < 0) {
		printf("Cannot open file %s\n", exportFileName);
		return FALSE;
	}
	// write gpio number to the file
	sprintf(idStr, "%d", id);
	write(exportFD, idStr, 4);
	close(exportFD);
	return TRUE;
}

/**
 * @copydoc setGPIODirection
 */
static int setGPIODirection(int id)
{
	int directionFD;
	char directionFileName[50];

	/* Set direction to 'in' for reading its values */
	sprintf(directionFileName, "/sys/class/gpio/gpio%d/direction", id);
	directionFD = open(directionFileName, O_WRONLY);
	if (directionFD < 0) {
		printf("Cannot open GPIO direction for %d\n", id);
		return FALSE;
	}
	write(directionFD, "in", 4);
	close(directionFD);
	return TRUE;
}

/**
 * @copydoc setUpButton
 */
static int setUpButton(int id) {
	/* Export button GPIO */
	if (!exportGPIO(id, gpio_export)) {
		return FALSE;
	}
	/* Set button GPIO directions to in */
	if (!setGPIODirection(id)) {
		return FALSE;
	}
	return TRUE;
}

/**
 * @copydoc readGPIOValue
 */
static int readGPIOValue(int id) {
	int valueFD;
	char valueFileName[50];
	char value[3];

	/* read value from gpio file in sysfs */
	sprintf(valueFileName, "/sys/class/gpio/gpio%d/value", id);
	valueFD = open(valueFileName, O_RDONLY);
	if (valueFD < 0) {
		printf("Cannot open GPIO value for %d\n", id);
		return -1;
	}
	read(valueFD, value, 2);
	close(valueFD);

	/* return value as integer */
	return atoi(value);
}

/**
 * @copydoc setUpCarmeGPIO
 */
int setUpCarmeGPIO(void) {
	int ret = TRUE;

	/* Initialize all buttons */
	if (!setUpButton(BUTTON_1)) {
		ret = FALSE;
	}
	if (!setUpButton(BUTTON_2)) {
		ret = FALSE;
	}
	if (!setUpButton(BUTTON_3)) {
		ret = FALSE;
	}
	if (!setUpButton(BUTTON_4)) {
		ret = FALSE;
	}
	return ret;
}

/**
 * @copydoc tearDownCarmeGPIO
 */
int tearDownCarmeGPIO(void) {
	int ret = TRUE;

	/* unexport all GPIO buttons */
	if (!exportGPIO(BUTTON_1, gpio_unexport)) {
		ret = FALSE;
	}
	if (!exportGPIO(BUTTON_2, gpio_unexport)) {
		ret = FALSE;
	}
	if (!exportGPIO(BUTTON_3, gpio_unexport)) {
		ret = FALSE;
	}
	if (!exportGPIO(BUTTON_4, gpio_unexport)) {
		ret = FALSE;
	}
	return ret;
}

/**
 * @copydoc readGPIOButton
 */
int readGPIOButton(int id) {
	return readGPIOValue(id);
}
