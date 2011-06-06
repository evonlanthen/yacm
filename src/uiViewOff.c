/**
 * @file   uiViewOff.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Defines the Off view for userInterface.c
 */

#include <stdio.h>
#include "defines.h"
#include "userInterface.h"
#include "inputController.h"
#include "ledController.h"
#include "logic.h"

/**
 * run action of view Off
 */
static void run(void) {
	/* Did someone turn the coffeemaker on? */
	if (getSwitchState(POWER_SWITCH) == switch_on) {
#ifdef DEBUG
		printf("Detected power switch to on\n");
#endif
		switchOn();
	}

}

/**
 * activate action of view Off
 */
static void activate(void) {
	DisplayState *displaystate = getDisplayState();
	displaystate->gContextID = GrNewGC();
	char powerOnUseText[30] = "Use ";
	strcat(powerOnUseText, POWER_SWITCH_TEXT);
	strcat(powerOnUseText, " to start!");

	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate->gContextID, YELLOW);
	GrSetGCUseBackground(displaystate->gContextID, GR_FALSE);

	/* Select fonts */
	displaystate->font = GrCreateFont((unsigned char *) FONTNAME, 14, NULL);
	GrSetGCFont(displaystate->gContextID, displaystate->font);
	GrText(displaystate->gWinID, displaystate->gContextID, 120, 30, "Power Off!", -1, GR_TFASCII | GR_TFTOP);
	GrText(displaystate->gWinID, displaystate->gContextID, 100, 50, powerOnUseText, -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate->font);

	/* turn off power led */
	updateLed(POWER_LED,led_off);

	/* turn off milk selection led */
	updateLed(MILK_LED,led_off);

	/* turn off milk sensor led */
	updateLed(MILK_SENSOR_LED, led_off);

	/* turn off coffee sensor led */
	updateLed(COFFEE_SENSOR_LED, led_off);
}

/**
 * deactivate action of view Off
 */
static void deactivate(void) {
	DisplayState *displaystate = getDisplayState();

	/*Clear screen*/
	GrClearWindow(displaystate->gWinID,GR_FALSE);
}

/**
 * update action of view Off
 */
static void update(void) {
	/* not doing much here */
}


/**
 * @copydoc getViewOffActions
 */
CallViewActions getViewOffActions(void) {
	CallViewActions retval = { &run, &activate, &deactivate, &update };
	return retval;
}


