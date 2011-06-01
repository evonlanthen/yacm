/*
 * userInterface.h
 *
 *  Created on: May 23, 2011
 *      Author: Toni Baumann
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#ifdef CARME
 #define POWER_SWITCH		SWITCH_5
 #define POWER_SWITCH_TEXT	S4
 #define POWER_LED			LED_5
#elif defined(ORCHID)
 #define POWER_SWITCH		SWITCH_1
 #define POWER_SWITCH_TEXT	S1
 #define POWER_LED			LED_1
#endif

/* Some window related constants */
#define WIN_BORDER	5
/* Define font name and location */
#define FONTNAME	"/usr/fonts/truetype/arial.ttf"

#define MWINCLUDECOLORS
#include "nano-X.h"

/**
 * data type for a void function
 */
typedef void (*callViewAction)();



/**
 * Update current view on display
 * gets called by logic.c as an observer
 */
extern void updateView(void);

/**
 * Initialize display
 */
extern int setUpDisplay(void);

/**
 * turn off display and clean up
 */
extern int tearDownDisplay(void);

struct callViewActions {
	callViewAction	run;
	callViewAction	activate;
	callViewAction	deactivate;
	callViewAction	update;
};

struct DisplayState {
	GR_WINDOW_ID			gWinID;
	GR_IMAGE_ID 			imageID;
	GR_IMAGE_INFO 			imageInfo;
	GR_GC_ID				gContextID, gTestID, gRectID, gElliID, gElli2ID;
	GR_EVENT	   			event;
	GR_SCREEN_INFO  		screenInfo;
	GR_FONT_ID				font;
	int						winSizeX, winSizeY;
	struct callViewActions	actions;

};


/**
 * get displaystate pointer, gets called by uiView modules
 */
extern struct DisplayState* getDisplayState(void);

/**
 * Heartbeat function for ongoing view tasks.
 * Gets constantly called by controller.c
 */
extern int runUserInterface(void);


#endif /* USERINTERFACE_H_ */


