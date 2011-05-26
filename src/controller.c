/**
 * @file   controller.c
 * @author Ronny Stauffer (staur3@bfh.ch)
 * @date   May 23, 2011
 * @brief  Controls the application.
 *
 * Initializes and controls the application. Contains the entry point.
 */
#define TONITESTS
#undef ELMITESTS

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "logic.h"
#include "userInterface.h"
#include "machineController.h"
#include "ledController.h"
#include "timer.h"

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
	sleep(3);
	printf("Setting led5 on...\n");
	updateLed(LED_5, led_on);
	sleep(3);
	printf("Setting led1 off...\n");
	updateLed(LED_1, led_off);
	sleep(3);
	printf("hello\n");

	TimerDescriptor timer = setUpTimer(3000);
	while (!isTimerElapsed(timer)) {
		printf("Timer is not elapsed\n");
		sleep(1);
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
	setUpMachineController();
	setUpLedController();
	setUpBusinessLogic();
	setUpDisplay();
}

void tearDownSubsystems() {
	tearDownDisplay();
	tearDownBusinessLogic();
	tearDownLedController();
	tearDownMachineController();
}

#endif
