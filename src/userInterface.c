/*
 * @file   userInterface.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Contains the user interface.
 */

#define MWINCLUDECOLORS

#include "defines.h"
#include "nano-X.h"
#include "model.h"
#include "userInterface.h"
#include "uiViewIdle.h"
#include "uiViewInit.h"
#include "uiViewOff.h"
#include "uiViewWork.h"
#include "logic.h"


/* Current state of display with handles and elements */
static struct DisplayState displaystate;

/* current CoffeMaker state */
static struct CoffeeMakerViewModel coffeemaker;

/**
 * Set correct Action pointers for active view in displaystate
 */
void setCallViewActions(void) {
	if (coffeemaker.state == coffeeMaker_off) {
		displaystate.run = getViewOffActionRun();
		displaystate.activate = getViewOffActionActivate();
		displaystate.deactivate = getViewOffActionDeactivate();
		displaystate.update = getViewOffActionUpdate();
	}
	if (coffeemaker.state == coffeeMaker_idle) {
			displaystate.run = getViewIdleActionRun();
			displaystate.activate = getViewIdleActionActivate();
			displaystate.deactivate = getViewIdleActionDeactivate();
			displaystate.update = getViewIdleActionUpdate();
	}
	if (coffeemaker.state == coffeeMaker_producing) {
		displaystate.run = getViewWorkActionRun();
		displaystate.activate = getViewWorkActionActivate();
		displaystate.deactivate = getViewWorkActionDeactivate();
		displaystate.update = getViewWorkActionUpdate();
	}
	if (coffeemaker.state == coffeeMaker_initializing) {
		displaystate.run = getViewInitActionRun();
		displaystate.activate = getViewInitActionActivate();
		displaystate.deactivate = getViewInitActionDeactivate();
		displaystate.update = getViewInitActionUpdate();
	}

}


/**
 * Update current view on display
 * This function gets registered in logic.c as an observer
 */
void updateView() {
	struct CoffeeMakerViewModel newCoffeeMaker;
	/* get the current state and update*/
	newCoffeeMaker = getCoffeeMakerViewModel();
	/*Did we change state?*/
	if (newCoffeeMaker.state != coffeemaker.state) {
		/*Deactivate old view*/
		if (displaystate.deactivate) {
			(*displaystate.deactivate)();
		}
		/*this changes about everything, let's set the new model*/
		coffeemaker = newCoffeeMaker;
		setCallViewActions();
		/*let's activate the new view*/
		if (displaystate.activate) {
			(*displaystate.activate)();
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
	return retval;

}

/**
 * turn off display and clean up
 */
int tearDownDisplay(void) {
	/* Cleanup */
	GrDestroyFont(displaystate.font);
	GrDestroyGC(displaystate.gContextID);
	GrClose();
	return TRUE;

}

/**
 * Heartbeat function for ongoing view tasks.
 * Gets constantly called by controller.c
 */
int runUserInterface(void) {
	if (displaystate.run) {
			(*displaystate.run)();
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
struct DisplayState* getDisplayState(void) {
	return &displaystate;

}

