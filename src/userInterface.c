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

/* Some window related constants */
#define WIN_BORDER	5

struct DisplayState {
	GR_WINDOW_ID	gWinID;
	GR_IMAGE_ID 	imageID;
	GR_IMAGE_INFO 	imageInfo;
	GR_GC_ID		gContextID, gTestID, gRectID, gElliID, gElli2ID;
	GR_EVENT	   	event;
	GR_SCREEN_INFO  screenInfo;
	GR_FONT_ID		font;
	int				winSizeX, winSizeY;
};

/* Current state of display with handles and elements */
static struct DisplayState displaystate;

/**
 * Update current view on display
 * parameter should be the state construct
 */
int updateView(struct CoffeeMaker coffeemaker) {
	return TRUE;
}

/**
 * Initialize display
 */
int setUpDisplay(void) {
	int retval = TRUE;
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
	GrClose();
	return TRUE;

}
