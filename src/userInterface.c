/**
 * @file   userInterface.c
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Contains the user interface.
 */

#define MWINCLUDECOLORS

#include <string.h>
#include <stdio.h>
#include "defines.h"
#include "nano-X.h"
#include "model.h"
#include "userInterface.h"
#include "uiViewIdle.h"
#include "uiViewInit.h"
#include "uiViewOff.h"
#include "uiViewWork.h"
#include "logic.h"
#include "ledController.h"


/* Current state of display with handles and elements */
static DisplayState displaystate;

/* current CoffeeMaker state */
static CoffeeMakerViewModel coffeemaker;

/* CoffeMaker state after a change */
static CoffeeMakerViewModel newCoffeeMaker;

/* Text labels for the product buttons according to hardware */
static char *buttonLabels[4] = {
		PRODUCT_1_BUTTON_TEXT,
		PRODUCT_2_BUTTON_TEXT,
		PRODUCT_3_BUTTON_TEXT,
		PRODUCT_4_BUTTON_TEXT
};

/**
 * Set correct Action pointers for active view in displaystate
 */
void setCallViewActions(void) {
	if (coffeemaker.state == coffeeMaker_off) {
		displaystate.actions = getViewOffActions();
	}
	if (coffeemaker.state == coffeeMaker_idle) {
			displaystate.actions = getViewIdleActions();
	}
	if (coffeemaker.state == coffeeMaker_producing) {
		displaystate.actions = getViewWorkActions();
	}
	if (coffeemaker.state == coffeeMaker_initializing) {
		displaystate.actions = getViewInitActions();
	}

}


/**
 * @copydoc updateView
 */
void updateView() {

	/* get the current state and update*/
	newCoffeeMaker = getCoffeeMakerViewModel();

	/* Did we change state? */
	if (newCoffeeMaker.state != coffeemaker.state) {
#ifdef DEBUG
		printf("Got new state: %d\n", newCoffeeMaker.state);
#endif

		/* Deactivate old view */
		if (displaystate.actions.deactivate) {
			(*displaystate.actions.deactivate)();
		}

		/* this changes about everything, let's set the new model */
		coffeemaker = newCoffeeMaker;
		setCallViewActions();

		/* let's activate the new view */
		if (displaystate.actions.activate) {
			(*displaystate.actions.activate)();
		}

	}
	else {
		/* seems there is only a small update, let the view handle this */
		if (displaystate.actions.update) {
			(*displaystate.actions.update)();
		}
	}

	/* Finally clean old values with updated model*/
	coffeemaker = newCoffeeMaker;
}

/**
 * @copydoc setUpDisplay
 */
int setUpDisplay(void) {
	int retval = TRUE;

	/* get initial coffeemakerview model */
	coffeemaker = getCoffeeMakerViewModel();

	/* Initialize view actions in displaystate */
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

	/* Init graphic contexts*/
	displaystate.gMilkSelID = GrNewGC();
	displaystate.gContextID = GrNewGC();
	displaystate.gMilkSensorID = GrNewGC();
	displaystate.gCoffeeSensorID = GrNewGC();

	/* Move cursor out of view */
	GrMoveCursor(displaystate.screenInfo.cols,displaystate.screenInfo.rows);

	/* Show the window */
	GrMapWindow(displaystate.gWinID);

	/* Show our initial state */
	if (displaystate.actions.activate) {
		(*displaystate.actions.activate)();
	}

	/* Register with logic.c as observer */
	registerModelObserver(&updateView);
	return retval;

}

/**
 * @copydoc tearDownDisplay
 */
int tearDownDisplay(void) {
	/* Cleanup */
	GrDestroyFont(displaystate.font);
	GrDestroyGC(displaystate.gContextID);

	/* Clear screen */
	GrClearWindow(displaystate.gWinID,GR_FALSE);

	/* this function terminates the application, reason unknown */
	//GrClose();

	return TRUE;
}

/**
 * @copydoc runUserInterface
 */
int runUserInterface(void) {
	/* trigger view run action */
	if (displaystate.actions.run) {
			(*displaystate.actions.run)();
		}
	else {
		return FALSE;
	}
	return TRUE;
}

/**
 * @copydoc getDisplayState
 */
DisplayState * getDisplayState(void) {
	return &displaystate;
}

/**
 * @copydoc getCoffeeMakerState
 */
	CoffeeMakerViewModel* getCoffeeMakerState(void) {
	return &coffeemaker;
}

/**
 * @copydoc getNewCoffeeMakerState
 */
	CoffeeMakerViewModel* getNewCoffeeMakerState(void) {
	return &newCoffeeMaker;
}

/**
 * @copydoc showMilkSelection
 */
void showMilkSelection(int state) {
	char milkStateText[30] = "No";
	char milkStateUseText[30] = MILK_SWITCH_TEXT;
	if (state) {
		strcpy(milkStateText,"Yes");
	}

	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate.gMilkSelID, YELLOW);
	GrSetGCUseBackground(displaystate.gMilkSelID, GR_FALSE);

	/* Select fonts */
	displaystate.font = GrCreateFont((unsigned char *) FONTNAME, 12, NULL);
	GrSetGCFont(displaystate.gMilkSelID, displaystate.font);
	GrText(displaystate.gWinID, displaystate.gMilkSelID, 20, 60, "With milk:", -1, GR_TFASCII | GR_TFTOP);
	GrText(displaystate.gWinID, displaystate.gMilkSelID, 20, 80, milkStateText, -1, GR_TFASCII | GR_TFTOP);
	displaystate.font = GrCreateFont((unsigned char *) FONTNAME, 10, NULL);
	GrSetGCFont(displaystate.gMilkSelID, displaystate.font);
	GrText(displaystate.gWinID, displaystate.gMilkSelID, 20, 100, strcat(milkStateUseText, " to change!"), -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate.font);

	/* set the milk led according to state */
	if (state) {
		updateLed(MILK_LED, led_on);
	}
	else {
		updateLed(MILK_LED, led_off);
	}
}

/**
 * @copydoc showProduct
 */
void showProduct(int productIndex) {
	char productUseText[20] = "Press ";
	int xPos = 40;

	/* valid product ? */
	if ((productIndex > 4) || (productIndex < 1)) return;

	/* productIndex starts with 0 for product 1 */
	productIndex--;

	/* set correct label text for button */
	strcat(productUseText, buttonLabels[productIndex]);

	/* Check if product can be selected */
	/* Coffee available? */
	if (newCoffeeMaker.isCoffeeAvailable == FALSE) {
		strcpy(productUseText, "No coffee!");
	}
	else {
		/* or milk selected and none available? */
		if ((newCoffeeMaker.milkPreselectionState == milkPreselection_on) && (newCoffeeMaker.isMilkAvailable == FALSE)) {
			strcpy(productUseText, "No milk!");
		}
	}

	ProductViewModel product = getProductViewModel(productIndex);
	xPos = xPos + (productIndex * 70);
	displaystate.gProdID[productIndex] = GrNewGC();

	/* Back- Foreground color related stuff */
	GrSetGCForeground(displaystate.gProdID[productIndex], YELLOW);
	GrSetGCUseBackground(displaystate.gProdID[productIndex], GR_FALSE);

	/* Select fonts */
	displaystate.font = GrCreateFont((unsigned char *) FONTNAME, 12, NULL);
	GrSetGCFont(displaystate.gProdID[productIndex], displaystate.font);

	/* show product name and helper text */
	GrText(displaystate.gWinID, displaystate.gProdID[productIndex], xPos, 140, product.name, -1, GR_TFASCII | GR_TFTOP);
	displaystate.font = GrCreateFont((unsigned char *) FONTNAME, 10, NULL);
	GrSetGCFont(displaystate.gProdID[productIndex], displaystate.font);
	GrText(displaystate.gWinID, displaystate.gProdID[productIndex], xPos, 160, productUseText, -1, GR_TFASCII | GR_TFTOP);
	GrDestroyFont(displaystate.font);
}

/**
 * @copydoc getActiveProductLedId
 */
int getActiveProductLedId(void) {
	MakeCoffeeProcessInstanceViewModel activeProduct = getCoffeeMakingProcessInstanceViewModel();
	int activeLed = PRODUCT_1_LED;

	/* let's get the right button to query for stopping */
	switch ( activeProduct.productIndex ) {
		case 0: activeLed = PRODUCT_1_LED;
		break;
		case 1: activeLed = PRODUCT_2_LED;
		break;
		case 2: activeLed = PRODUCT_3_LED;
		break;
		case 3: activeLed = PRODUCT_4_LED;
		break;
		default: activeLed = PRODUCT_1_LED;
		break;
	}
	return activeLed;
}

/**
 * @copydoc showMilkSensor
 */
void showMilkSensor(int state) {
	/* check if we ran out of milk */
	if (state) {
		/* Back- Foreground color related stuff */
		GrSetGCForeground(displaystate.gMilkSensorID, RED);
		GrSetGCUseBackground(displaystate.gMilkSensorID, GR_FALSE);

		/* Select fonts */
		displaystate.font = GrCreateFont((unsigned char *) FONTNAME, 12, NULL);
		GrSetGCFont(displaystate.gMilkSensorID, displaystate.font);
		GrText(displaystate.gWinID, displaystate.gMilkSensorID, 230, 60, "Milk empty!", -1, GR_TFASCII | GR_TFTOP);
		GrDestroyFont(displaystate.font);

		/* start blinking led for product */
		setBlinkingFreq(MILK_SENSOR_LED, WARNING_BLINK_TIME_ON, WARNING_BLINK_TIME_OFF);

		updateLed(MILK_SENSOR_LED, led_blinking);
	}
	else {
		updateLed(MILK_SENSOR_LED, led_off);
	}
}

/**
 * @copydoc showCoffeeSensor
 */
void showCoffeeSensor(int state) {
	/* check if we ran out of coffee */
	if (state) {
		/* Back- Foreground color related stuff */
		GrSetGCForeground(displaystate.gCoffeeSensorID, RED);
		GrSetGCUseBackground(displaystate.gCoffeeSensorID, GR_FALSE);

		/* Select fonts */
		displaystate.font = GrCreateFont((unsigned char *) FONTNAME, 12, NULL);
		GrSetGCFont(displaystate.gCoffeeSensorID, displaystate.font);
		GrText(displaystate.gWinID, displaystate.gCoffeeSensorID, 230, 80, "Coffee empty!", -1, GR_TFASCII | GR_TFTOP);
		GrDestroyFont(displaystate.font);

		/* start blinking led for product */
		setBlinkingFreq(COFFEE_SENSOR_LED, WARNING_BLINK_TIME_ON, WARNING_BLINK_TIME_OFF);
		updateLed(COFFEE_SENSOR_LED, led_blinking);
	}
	else {
		updateLed(COFFEE_SENSOR_LED, led_off);
	}
}

