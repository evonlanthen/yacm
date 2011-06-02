/*
 * @file   uiViewInit.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Defines the Init view for userInterface.c
 */

#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "userInterface.h"
#include "inputController.h"
#include "ledController.h"
#include "logic.h"
#include "timer.h"

#define RUN_INTERVAL	200

static TIMER initTimer;
static int intervals = 0;

static void run(void) {
	char initMessage[40] = "Initializing";
	if (isTimerElapsed(initTimer)) {
		/*set new timer*/
		initTimer = setUpTimer(RUN_INTERVAL);
		if (intervals < 25) {
			intervals++;
		}
		for (int i = 1; i < intervals; i++) {
			strcat(initMessage,".");
		}
		DisplayState *displaystate = getDisplayState();
		/* Select fonts */
		displaystate->font = GrCreateFont((unsigned char *) FONTNAME, 14, NULL);
		GrSetGCFont(displaystate->gContextID, displaystate->font);
		GrText(displaystate->gWinID, displaystate->gContextID, 120, 30, initMessage, -1, GR_TFASCII | GR_TFTOP);
		GrDestroyFont(displaystate->font);
	}
	/* Did someone turn the coffeemaker off? */
	if (getSwitchState(POWER_SWITCH) == switch_off) {
#ifdef DEBUG
		printf("Detected power switch to off\n");
#endif
		switchOff();
	}
}

static void activate(void) {
	/* turn of power led */
	updateLed(POWER_LED,led_on);
	/* reset init wait intervals*/
	intervals = 0;
	/*start Timer*/
	initTimer = setUpTimer(RUN_INTERVAL);
	DisplayState *displaystate = getDisplayState();
	displaystate->gContextID = GrNewGC();
	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate->gContextID, YELLOW);
	GrSetGCUseBackground(displaystate->gContextID, GR_FALSE);
	/* Select fonts */
	displaystate->font = GrCreateFont((unsigned char *) FONTNAME, 14, NULL);
	GrSetGCFont(displaystate->gContextID, displaystate->font);
	GrText(displaystate->gWinID, displaystate->gContextID, 120, 30, "Initializing", -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate->font);
}

static void deactivate(void) {
	DisplayState *displaystate = getDisplayState();
	/*Clear screen*/
	GrClearWindow(displaystate->gWinID,GR_FALSE);
}

static void update(void) {

}


CallViewActions getViewInitActions(void) {
	CallViewActions retval = { &run, &activate, &deactivate, &update };
	return retval;
}
