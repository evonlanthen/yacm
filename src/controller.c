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

#define TONITESTS

static void setUpSubsystems();
static void tearDownSubsystems();

#ifndef TEST
/**
 * The entry point of the application.
 */
int main(int argc, char* argv[]) {
	setUpSubsystems();

#ifdef TONITESTS
	struct CoffeeMaker coffeeMaker = {
		.state = coffeeMaker_off,
		.coffee.isAvailable = TRUE,
		.milk.isAvailable = TRUE//,
		//.products = &coffeeProductListElement
	};

	setUpDisplay();

	updateView(coffeeMaker);
#endif

	while (TRUE) {
		// Catch and dispatch events
	}

	tearDownSubsystems();

	exit(0);
}

void setUpSubsystems() {
	setUpBusinessLogic();
}

void tearDownSubsystems() {
	tearDownBusinessLogic();
}

#endif
