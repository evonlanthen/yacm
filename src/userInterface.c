/*
 * userInterface.c
 *
 *  Created on: May 23, 2011
 *      Author: Toni Bauman
 */

#include "defines.h"
#include "nano-X.h"

/**
 * Update current view on display
 * parameter should be the state construct
 */
int updateView(int view) {
	return TRUE;
}

/**
 * Initialize display
 */
int initDisplay(void) {
	int retval = TRUE;
	/* Try to open the graphic device */
	if (GrOpen() < 0) {
		retval = FALSE;
	}
	return retval;

}

/**
 * turn off display and clean up
 */
int shutdownDisplay(void) {
	GrClose();
	return TRUE;

}
