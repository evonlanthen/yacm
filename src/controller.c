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
#include "ledController.h"

/**
 * The entry point of the program.
 */
int main(int argc, char* argv[]) {
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

	initMachineController();
	initLedController();
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

	cleanUpLedController();
	cleanUpMachineController();
#endif

	while (TRUE) {
		// Catch and dispatch events

	}

	return 0;
}
