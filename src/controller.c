/**
 * @file   controller.c
 * @author Ronny Stauffer (staur3@bfh.ch)
 * @date   May 23, 2011
 * @brief  Controls the program.
 *
 * Initializes and controls the program. Contains the entry point.
 */
#define TONITESTS
#undef ELMITESTS

#include <stdio.h>

#include "defines.h"
#include "model.h"
#include "userInterface.h"
#include "machineController.h"
#include "inputController.h"
#include "ledController.h"
#include "sensorController.h"
#include "timer.h"
#include "mmap.h"

/**
 * The entry point of the program.
 */
int main(int argc, char* argv[]) {
#ifdef ELMITESTS
	TimerDescriptor timer;
#endif
	//struct CoffeeMaker coffeeMaker;
#ifdef TONITESTS
	struct CoffeeMaker coffeeMaker = {
			.state = coffeeMaker_off,
			.coffee.isAvailable = TRUE,
			.milk.isAvailable = TRUE//,
			//.products = &coffeeProductListElement
	};
	setUpDisplay();
	printf("Hello world!\n");
	updateView(coffeeMaker);
#endif

#ifdef ELMITESTS
	//struct CoffeeMaker coffeeMaker;

	#ifdef CARME
		printf("Initializing for board CARME\n");
	#elif defined(ORCHID)
		printf("Initializing for board ORCHID\n");
	#endif

	setUpMmap();
	setUpMachineController();
	setUpInputController();
	setUpLedController();
	setUpSensorController();

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

	timer = setUpTimer(10000);
	while (!isTimerElapsed(timer)) {
		updateAllLeds();
	}
	setBlinkingFreq(LED_3, 100, 100);
	timer = setUpTimer(5000);
	while (!isTimerElapsed(timer)) {
		updateAllLeds();
	}
	printf("Timer is elapsed\n");

	tearDownSensorController();
	tearDownLedController();
	tearDownInputController();
	tearDownMachineController();
	tearDownMmap();
#endif

	while (TRUE) {
		// Catch and dispatch events

	}

	return 0;
}
