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

#include "defines.h"
#include "logic.h"
#include "userInterface.h"
#include "machineController.h"
#include "inputController.h"
#include "ledController.h"
#include "sensorController.h"
#include "timer.h"
#include "mmap.h"

//#define ELMITESTS
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
	setBlinkingFreq(LED_3, 100, 100);
	timer = setUpTimer(5000);
	while (!isTimerElapsed(timer)) {
		updateAllLeds();
	}
	printf("Timer is elapsed\n");

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
