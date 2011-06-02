/*
 * @file   uiViewWork.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Defines the Work view for userInterface.c
 */
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "userInterface.h"
#include "inputController.h"
#include "ledController.h"
#include "logic.h"
#include "timer.h"

static void run(void) {
	MakeCoffeeProcessInstanceViewModel makingCoffee = getCoffeeMakingProcessInstanceViewModel();
	int activeButton = PRODUCT_1_BUTTON;
	/* let's get the right button to query for stopping */
	switch ( makingCoffee.productIndex ) {
		case 1: activeButton = PRODUCT_1_BUTTON;
		break;
		case 2: activeButton = PRODUCT_2_BUTTON;
		break;
		case 3: activeButton = PRODUCT_3_BUTTON;
		break;
		case 4: activeButton = PRODUCT_4_BUTTON;
		break;
		default: activeButton = PRODUCT_1_BUTTON;
		break;
	}
	/* Did someone turn the coffeemaker off? */
	if (getSwitchState(POWER_SWITCH) == switch_off) {
#ifdef DEBUG
		printf("Detected power switch to off\n");
	#endif
		switchOff();
	}

	/* user tries to stop making coffee? */
	/* TODO do we have to wait some time to avoid same
	 * button press on/off problems?
	 */
	if (getButtonState(activeButton) == button_on) {
		abortMakingCoffee();
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
	GrText(displaystate->gWinID, displaystate->gContextID, 120, 30, "Making coffee...", -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate->font);

}

static void deactivate(void) {
	DisplayState *displaystate = getDisplayState();
	/*Clear screen*/
	GrClearWindow(displaystate->gWinID,GR_FALSE);
}

static void update(void) {

}


CallViewActions getViewWorkActions(void) {
	CallViewActions retval = { &run, &activate, &deactivate, &update };
	return retval;
}
