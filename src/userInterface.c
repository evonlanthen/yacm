/*
 * @file   userInterface.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Contains the user interface.
 */

#define MWINCLUDECOLORS

#include <string.h>
#include <stdio.h>
#include "defines.h"
#include "nano-X.h"
#include "model.h"
#include "userInterface.h"
#include "uiViewIdle.h"
#include "uiViewInit.h"
#include "uiViewOff.h"
#include "uiViewWork.h"
#include "logic.h"
#include "ledController.h"


/* Current state of display with handles and elements */
static DisplayState displaystate;

/* current CoffeMaker state */
static CoffeeMakerViewModel coffeemaker;

/* CoffeMaker state after a change */
static CoffeeMakerViewModel newCoffeeMaker;

/**
 * Set correct Action pointers for active view in displaystate
 */
void setCallViewActions(void) {
	if (coffeemaker.state == coffeeMaker_off) {
		displaystate.actions = getViewOffActions();
	}
	if (coffeemaker.state == coffeeMaker_idle) {
			displaystate.actions = getViewIdleActions();
	}
	if (coffeemaker.state == coffeeMaker_producing) {
		displaystate.actions = getViewWorkActions();
	}
	if (coffeemaker.state == coffeeMaker_initializing) {
		displaystate.actions = getViewInitActions();
	}

}


/**
 * Update current view on display
 * This function is registered in logic.c as an observer
 * and gets triggered as a change occurs.
 */
void updateView() {
	/* get the current state and update*/
	newCoffeeMaker = getCoffeeMakerViewModel();
	/*Did we change state?*/
	if (newCoffeeMaker.state != coffeemaker.state) {
#ifdef DEBUG
		printf("Got new state: %d\n", newCoffeeMaker.state);
#endif
		/*Deactivate old view*/
		if (displaystate.actions.deactivate) {
			(*displaystate.actions.deactivate)();
		}
		/*this changes about everything, let's set the new model*/
		coffeemaker = newCoffeeMaker;
		setCallViewActions();
		/*let's activate the new view*/
		if (displaystate.actions.activate) {
			(*displaystate.actions.activate)();
		}

	}
	else {
		if (displaystate.actions.update) {
			(*displaystate.actions.update)();
		}
	}

	/* Finally clean old values with updated model*/
	coffeemaker = newCoffeeMaker;
}

/**
 * Initialize display
 */
int setUpDisplay(void) {
	int retval = TRUE;
	/* get initial coffeemakerview model*/
	coffeemaker = getCoffeeMakerViewModel();
	/* Initialize view actions in displaystate*/
	setCallViewActions();
	/* Try to open the graphic device */
	if (GrOpen() < 0) {
		retval = FALSE;
	}
	/* Get the screen info */
	GrGetScreenInfo(&displaystate.screenInfo);
	displaystate.winSizeX = displaystate.screenInfo.cols - 2 * WIN_BORDER;
	displaystate.winSizeY = displaystate.screenInfo.rows - 2 * WIN_BORDER;

	/* Get a window handle */
	displaystate.gWinID = GrNewWindow(GR_ROOT_WINDOW_ID, WIN_BORDER, WIN_BORDER, displaystate.winSizeX, displaystate.winSizeY, WIN_BORDER, BLACK, YELLOW);

	/* Show the window */
	GrMapWindow(displaystate.gWinID);
	/* Show our initial state */
	if (displaystate.actions.activate) {
		(*displaystate.actions.activate)();
	}

	/*Register with logic.c as observer */
	registerModelObserver(&updateView);
	return retval;

}

/**
 * turn off display and clean up
 */
int tearDownDisplay(void) {
	/* Cleanup */
	GrDestroyFont(displaystate.font);
	GrDestroyGC(displaystate.gContextID);
	/*Clear screen*/
	GrClearWindow(displaystate.gWinID,GR_FALSE);
	/*this function terminates the application, reason unknown*/
	//GrClose();
	return TRUE;

}

/**
 * Heartbeat function for ongoing view tasks.
 * Gets constantly called by controller.c
 */
int runUserInterface(void) {
	if (displaystate.actions.run) {
			(*displaystate.actions.run)();
		}
	else {
		return FALSE;
	}
	return TRUE;
}

/**
 * get displaystate reference
 * called by uiView modules
 */
DisplayState * getDisplayState(void) {
	return &displaystate;
}

/**
 * get coffeemaker state reference
 * called by uiView modules
 */
	CoffeeMakerViewModel* getCoffeeMakerState(void) {
	return &coffeemaker;
}

/**
 * Display milk selection state
 */
extern void showMilkSelection(int state) {
	char milkStateText[30] = "no Milk";
	displaystate.gMilkSelID = GrNewGC();
	if (state) {
		strcpy(milkStateText,"Milk");
	}
	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate.gMilkSelID, YELLOW);
	GrSetGCUseBackground(displaystate.gMilkSelID, GR_FALSE);
	/* Select fonts */
	displaystate.font = GrCreateFont((unsigned char *) FONTNAME, 12, NULL);
	GrSetGCFont(displaystate.gMilkSelID, displaystate.font);
	GrText(displaystate.gWinID, displaystate.gMilkSelID, 20, 60, milkStateText, -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate.font);
	if (state) {
		updateLed(MILK_LED, led_on);
	}
	else {
		updateLed(MILK_LED, led_off);
	}
}

