/**
 * @file   controller.c
 * @author Ronny Stauffer (staur3@bfh.ch)
 * @date   May 23, 2011
 * @brief  Controls the application.
 *
 * Initializes and controls the application. Contains the entry point.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "defines.h"
#include "logic.h"
#include "userInterface.h"
#include "machineController.h"
#include "inputController.h"
#include "ledController.h"
#include "sensorController.h"
#include "timer.h"
#include "mmap.h"

#define ELMITESTS
//#define TONITESTS

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
		printf("Switch 2 is on\n");
		updateLed(LED_2, led_on);
	} else if (switchState == switch_off){
		printf("Switch 2 is off\n");
	} else {
		printf("Switch 2 state is unknown\n");
	}

	// check buttons:
	enum ButtonState buttonState = getButtonState(BUTTON_1);
	if (buttonState == button_on) {
		printf("Button 1 is on\n");
		updateLed(LED_5, led_on);
	} else if (buttonState == button_off) {
		printf("Button 1 is off\n");
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
	TimerDescriptor timer = setUpTimer(10000);
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
	startMachine();
	setBlinkingFreq(LED_1, 500, 500);
	updateLed(LED_1, led_blinking);
	while (machineRunning()) {
		updateAllLeds();
	}
	printf("Coffee is ready!\n");
	updateLed(LED_1, led_off);
#endif

#ifdef TONITESTS
	struct CoffeeMaker coffeeMaker = {
		.state = coffeeMaker_off,
		.coffee.isAvailable = TRUE,
		.milk.isAvailable = TRUE//,
		//.products = &coffeeProductListElement
	};

	updateView(coffeeMaker);
#endif

	while (TRUE) {
		// Propagate "heartbeat"
		runBusinessLogic();
	}

	tearDownSubsystems();

	exit(0);
}

void setUpSubsystems() {
	setUpMmap();
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
	tearDownMmap();
}

#endif
