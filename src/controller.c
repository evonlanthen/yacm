/**
 * @mainpage yacm - yet another coffee maker
 * @section intro Introduction
 *
 * This software implements the logic for a coffee maker.
 *
 * @section usage Usage
 * @subsection step1 Step 1: Compile project
 * @code
 * <user>@<host> $ make
 * @endcode
 * @subsection step2 Step 2: Install files
 * @code
 * <user>@<host> $ make install
 * @endcode
 * @subsection step3 Step 3: Start program
 * @code
 * root@<target> # /usr/local/bin/yacm
 * @endcode
 *
 * @section authors Authors
 * @arg Toni Baumann (bauma12@bfh.ch)
 * @arg Ronny Stauffer (staur3@bfh.ch)
 * @arg Elmar Vonlanthen (vonle1@bfh.ch)
 *
 * @file   controller.c
 * @author Ronny Stauffer (staur3@bfh.ch)
 * @date   May 23, 2011
 * @brief  Controls the application.
 *
 * Initializes and controls the application. Contains the entry point.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "defines.h"
#include "logic.h"
#include "userInterface.h"
#include "hardwareController.h"
#include "machineController.h"
#include "inputController.h"
#include "ledController.h"
#include "sensorController.h"
#include "timer.h"

//#define ELMITESTS

static volatile int ctrlCPressed = FALSE;

static void sigCtrlC(int sig)
{
	ctrlCPressed = TRUE;
	(void) signal(SIGINT, SIG_DFL);
}

static void setUpSubsystems();
static void tearDownSubsystems();

#ifndef TEST
/**
 * The entry point of the application.
 */
int main(int argc, char* argv[]) {
	setUpSubsystems();

#ifdef ELMITESTS
	#ifdef CARME
		printf("Initializing for board CARME\n");
	#elif defined(ORCHID)
		printf("Initializing for board ORCHID\n");
	#endif

	/* button test: */
	printf("Checking button 1...\n");
	enum ButtonState buttonState = getButtonState(BUTTON_1);
	if (buttonState == button_on) {
		printf("Button 1 is on\n");
	} else {
		printf("Button 1 is off\n");
	}

	printf("Setting led1 on...\n");
	updateLed(LED_1, led_on);
	sleep(1);
	printf("Setting led8 on...\n");
	updateLed(LED_8, led_on);
	sleep(1);
	printf("Setting led1 off...\n");
	updateLed(LED_1, led_off);
	sleep(1);

	// check switches:
	enum SwitchState switchState = getSwitchState(SWITCH_2);
	if (switchState == switch_on) {
		printf("Switch 2 is on, setting LED_2 on\n");
		updateLed(LED_2, led_on);
	} else if (switchState == switch_off){
		printf("Switch 2 is off\n");
	} else {
		printf("Switch 2 state is unknown\n");
	}

	// check buttons:
	buttonState = getButtonState(BUTTON_4);
	if (buttonState == button_on) {
		printf("Button 4 is on\n");
	} else if (buttonState == button_off) {
		printf("Button 4 is off\n");
	} else {
		printf("Button 1 state is unknown\n");
	}

	// check sensors:
	enum SensorState sensorState = getSensorState(SENSOR_1);
	if (sensorState == sensor_alert) {
		printf("Sensor 1 state: alert\n");
	} else if(sensorState == sensor_normal) {
		printf("Sensor 1 state: normal\n");
	} else {
		printf("Sensor 1 state: unknown\n");
	}

	// blink for 10 seconds:
	printf("Setting led4 blinking...\n");
	setBlinkingFreq(LED_4, 3000, 1000);
	updateLed(LED_4, led_blinking);
	printf("Setting led3 blinking...\n");
	setBlinkingFreq(LED_3, 500, 500);
	updateLed(LED_3, led_blinking);
	TIMER timer = setUpTimer(10000);
	while (!isTimerElapsed(timer)) {
		updateAllLeds();
	}
	// blink for 5 seconds:
	setBlinkingFreq(LED_3, 100, 100);
	timer = setUpTimer(5000);
	while (!isTimerElapsed(timer)) {
		updateAllLeds();
	}
	printf("Blinking disabled\n");

	// start coffee maker:
	printf("Starting coffee machine...\n");
	startMachine(ingredient_coffee, 5000);
	setBlinkingFreq(LED_1, 500, 500);
	updateLed(LED_1, led_blinking);
	while (machineRunning()) {
		updateAllLeds();
	}
	printf("Coffee is ready!\n");
	updateLed(LED_1, led_off);
#endif

	(void) signal(SIGINT, sigCtrlC);

	while (!ctrlCPressed) {
		// Propagate "heartbeat"
		runUserInterface();
		runBusinessLogic();
	}

	printf("\nShutting down system...\n");
	tearDownSubsystems();
#ifdef DEBUG
	printf("Shutdown successful, terminating!\n");
#endif
	exit(0);
}

void setUpSubsystems() {
	setUpHardwareController();
	setUpMachineController();
	setUpInputController();
	setUpLedController();
	setUpSensorController();
	setUpBusinessLogic();
	setUpDisplay();
}

void tearDownSubsystems() {
	tearDownDisplay();
	tearDownBusinessLogic();
	tearDownSensorController();
	tearDownLedController();
	tearDownInputController();
	tearDownMachineController();
	tearDownHardwareController();
}

#endif
