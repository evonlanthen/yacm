/**
 * @file   controller.c
 * @author Ronny Stauffer (staur3@bfh.ch)
 * @date   May 23, 2011
 * @brief  Controls the program.
 *
 * Initializes and controls the program. Contains the entry point.
 */
#define TONITESTS

#include <stdio.h>

#include "defines.h"
#include "model.h"
#include "userInterface.h"

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
	updateView(coffeemaker);
#endif

	while (TRUE) {
		// Catch and dispatch events

	}

	return 0;
}
