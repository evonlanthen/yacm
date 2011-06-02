/*
 * @file   uiViewIdle.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Defines the Idle view for userInterface.c
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
	CoffeeMakerViewModel *coffeemaker = getCoffeeMakerState();
	/* Did someone turn the coffeemaker off? */
	if (getSwitchState(POWER_SWITCH) == switch_off) {
#ifdef DEBUG
		printf("Detected power switch to off\n");
	#endif
		switchOff();
	}
	if (getButtonState(PRODUCT_1_BUTTON) == button_on) {
		startMakingCoffee(0);
	}
	if (getButtonState(PRODUCT_2_BUTTON) == button_on) {
		startMakingCoffee(1);
	}
	if (getButtonState(PRODUCT_3_BUTTON) == button_on) {
		startMakingCoffee(2);
	}
	if (getButtonState(PRODUCT_4_BUTTON) == button_on) {
		startMakingCoffee(3);
	}
	/* Did someone use the milk selector? */
	if (getSwitchState(MILK_SWITCH) == switch_off) {
		if (coffeemaker->milkPreselectionState == milkPreselection_on) {
			setMilkPreselection(milkPreselection_off);
		}
		switchOff();
	}
	else {
		if (coffeemaker->milkPreselectionState == milkPreselection_off) {
			setMilkPreselection(milkPreselection_on);
		}
	}
}

static void activate(void) {
	CoffeeMakerViewModel *coffeemaker = getCoffeeMakerState();
	DisplayState *displaystate = getDisplayState();
	displaystate->gContextID = GrNewGC();
	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate->gContextID, YELLOW);
	GrSetGCUseBackground(displaystate->gContextID, GR_FALSE);
	/* Select fonts */
	displaystate->font = GrCreateFont((unsigned char *) FONTNAME, 14, NULL);
	GrSetGCFont(displaystate->gContextID, displaystate->font);
	GrText(displaystate->gWinID, displaystate->gContextID, 120, 30, "Product Selection:", -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate->font);
	if (coffeemaker->milkPreselectionState == milkPreselection_on) {
		/* indicate milk selection on display*/
		showMilkSelection(TRUE);
	}
}

static void deactivate(void) {
	DisplayState *displaystate = getDisplayState();
	/*Clear screen*/
	GrClearWindow(displaystate->gWinID,GR_FALSE);
}

static void update(void) {
	//TODO React on milk selection change

}

CallViewActions getViewIdleActions(void) {
	CallViewActions retval = { &run, &activate, &deactivate, &update };
	return retval;
}
