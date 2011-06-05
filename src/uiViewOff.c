/*
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

static void run(void) {
	/* Did someone turn the coffeemaker on? */
	if (getSwitchState(POWER_SWITCH) == switch_on) {
#ifdef DEBUG
		printf("Detected power switch to on\n");
#endif
		switchOn();
	}

}

static void activate(void) {
	DisplayState *displaystate = getDisplayState();
	displaystate->gContextID = GrNewGC();
	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate->gContextID, YELLOW);
	GrSetGCUseBackground(displaystate->gContextID, GR_FALSE);
	/* Select fonts */
	displaystate->font = GrCreateFont((unsigned char *) FONTNAME, 14, NULL);
	GrSetGCFont(displaystate->gContextID, displaystate->font);
	GrText(displaystate->gWinID, displaystate->gContextID, 120, 30, "Power Off!", -1, GR_TFASCII | GR_TFTOP);
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

static void deactivate(void) {
	DisplayState *displaystate = getDisplayState();
	/*Clear screen*/
	GrClearWindow(displaystate->gWinID,GR_FALSE);
}

static void update(void) {

}


CallViewActions getViewOffActions(void) {
	CallViewActions retval = { &run, &activate, &deactivate, &update };
	return retval;
}


