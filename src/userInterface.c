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
/* Define font name and location */
#define FONTNAME	"/usr/fonts/truetype/arial.ttf"

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

/* current CoffeMaker state */
static struct CoffeeMaker coffeemaker;

/**
 * Show View Offstate
 */
int showViewOff(void) {
	int retval = TRUE;
	displaystate.gContextID = GrNewGC();
	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate.gContextID, YELLOW);
	GrSetGCUseBackground(displaystate.gContextID, GR_FALSE);
	/* Select fonts */
	displaystate.font = GrCreateFont((unsigned char *) FONTNAME, 14, NULL);
	GrSetGCFont(displaystate.gContextID, displaystate.font);
	GrText(displaystate.gWinID, displaystate.gContextID, 120, 30, "No Power!", -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate.font);
	return retval;
}
/**
 * Show View Init
 */
int showViewInit(void) {
	int retval = TRUE;
	displaystate.gContextID = GrNewGC();
		/* Back- Foreground color related stuff */
		GrSetGCForeground(displaystate.gContextID, YELLOW);
		GrSetGCUseBackground(displaystate.gContextID, GR_FALSE);
		/* Select fonts */
		displaystate.font = GrCreateFont((unsigned char *) FONTNAME, 14, NULL);
		GrSetGCFont(displaystate.gContextID, displaystate.font);
		GrText(displaystate.gWinID, displaystate.gContextID, 120, 30, "Initializing...", -1, GR_TFASCII | GR_TFTOP);
		GrDestroyFont(displaystate.font);
	return retval;
}

/**
 * Show View Idle
 */
int showViewIdle(void) {
	int retval = TRUE;
	return retval;
}

/**
 * Show View Working
 */
int showViewWorking(void) {
	int retval = TRUE;
	return retval;
}


/**
 * Update current view on display
 * parameter should be the state construct
 */
int updateView(struct CoffeeMaker newcoffeemaker) {
	int retval = TRUE;
	coffeemaker = newcoffeemaker;
	/* call the right view according to state */
	if (coffeemaker.state == coffeeMaker_off) {
		if (showViewOff() != TRUE) retval = FALSE;
	}
	if (coffeemaker.state == coffeeMaker_initializing) {
		if (showViewInit() != TRUE) retval = FALSE;
	}
	if (coffeemaker.state == coffeeMaker_idle) {
		if (showViewIdle() != TRUE) retval = FALSE;
	}
	if (coffeemaker.state == coffeeMaker_producing) {
		if (showViewWorking() != TRUE) retval = FALSE;
	}
	return retval;
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
	/* Cleanup */
	GrDestroyFont(displaystate.font);
	GrDestroyGC(displaystate.gContextID);
	GrClose();
	return TRUE;

}
