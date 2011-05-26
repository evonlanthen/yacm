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
#include "model.h"

#ifndef TEST
/**
 * The entry point of the application.
 */
int main(int argc, char* argv[]) {
	printf("Hello world!\n");

	while (TRUE) {
		// Catch and dispatch events

	}

	exit(0);
}
#endif
