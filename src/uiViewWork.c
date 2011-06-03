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

#define BUTTON_DELAY	800
#define PRODUCT_BLINK_TIME_ON	250
#define PRODUCT_BLINK_TIME_OFF	250

static TIMER delayTimer;

static void run(void) {
	MakeCoffeeProcessInstanceViewModel makingCoffee = getCoffeeMakingProcessInstanceViewModel();
	int activeButton = PRODUCT_1_BUTTON;
	if ((delayTimer == NULL) || (isTimerElapsed(delayTimer))) {
		/*make sure we don't use timer again */
		delayTimer = NULL;
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
		/* user tries to stop making coffee? */
		if (getButtonState(activeButton) == button_on) {
			abortMakingCoffee();
		}
	}
	/* Did someone turn the coffeemaker off? */
	if (getSwitchState(POWER_SWITCH) == switch_off) {
#ifdef DEBUG
		printf("Detected power switch to off\n");
	#endif
		switchOff();
	}

	/* update blinking Leds */
	updateAllLeds();
}

static void activate(void) {
	DisplayState *displaystate = getDisplayState();
	displaystate->gContextID = GrNewGC();
	MakeCoffeeProcessInstanceViewModel activeProduct = getCoffeeMakingProcessInstanceViewModel();
	/*start Timer for button release delay*/
	delayTimer = setUpTimer(BUTTON_DELAY);
	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate->gContextID, YELLOW);
	GrSetGCUseBackground(displaystate->gContextID, GR_FALSE);
	/* Select fonts */
	displaystate->font = GrCreateFont((unsigned char *) FONTNAME, 14, NULL);
	GrSetGCFont(displaystate->gContextID, displaystate->font);
	GrText(displaystate->gWinID, displaystate->gContextID, 120, 30, "Making coffee...", -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate->font);
	/* display active product */
#ifdef DEBUG
	printf("uiViewWork.c: Calling showProduct(%d)\n",activeProduct.productIndex);
#endif
	showProduct(activeProduct.productIndex + 1);

	/* display chosen milk selection for active Product*/
#ifdef DEBUG
	printf("uiViewWork.c: Calling showMilkSelection(%d)\n",activeProduct.withMilk);
#endif
	showMilkSelection(activeProduct.withMilk);

	/* start blinking led for product */
	setBlinkingFreq(getActiveProductLedId(), PRODUCT_BLINK_TIME_ON, PRODUCT_BLINK_TIME_OFF);
	updateLed(getActiveProductLedId(), led_blinking);
}

static void deactivate(void) {
	DisplayState *displaystate = getDisplayState();

	/* Turn off all product Leds */
	updateLed(PRODUCT_1_LED, led_off);
	updateLed(PRODUCT_2_LED, led_off);
	updateLed(PRODUCT_3_LED, led_off);
	updateLed(PRODUCT_4_LED, led_off);

	/*Clear screen*/
	GrClearWindow(displaystate->gWinID,GR_FALSE);
}

static void update(void) {

}


CallViewActions getViewWorkActions(void) {
	CallViewActions retval = { &run, &activate, &deactivate, &update };
	return retval;
}
