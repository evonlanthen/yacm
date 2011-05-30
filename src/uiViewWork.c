/*
 * @file   uiViewWork.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Defines the Work view for userInterface.c
 */

#include "userInterface.h"

static void run(void) {

}

static void activate(void) {

}

static void deactivate(void) {

}

static void update(void) {

}


struct callViewActions getViewWorkActions(void) {
	struct callViewActions retval = { &run, &activate, &deactivate, &update };
	return retval;
}
