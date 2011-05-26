/**
 * @file   controller.c
 * @author Ronny Stauffer (staur3@bfh.ch)
 * @date   May 23, 2011
 * @brief  Controls the application.
 *
 * Initializes and controls the application. Contains the entry point.
 */
#define TONITESTS

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"
#include "model.h"
#include "userInterface.h"

#ifndef TEST
/**
 * The entry point of the application.
 */
int main(int argc, char* argv[]) {
<<<<<<< HEAD
=======
	//struct CoffeeMaker coffeeMaker;
#ifdef TONITESTS
	struct CoffeeMaker coffeeMaker = {
			.state = coffeeMaker_off,
			.coffee.isAvailable = TRUE,
			.milk.isAvailable = TRUE//,
			//.products = &coffeeProductListElement
	};
	setUpDisplay();
>>>>>>> 97cd09daae41d1081b0f55b3f9880cc258715423
	printf("Hello world!\n");
	updateView(coffeeMaker);
#endif

	while (TRUE) {
		// Catch and dispatch events

	}

	exit(0);
}
#endif
