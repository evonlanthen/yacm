/**
 * @file   userInterface.h
 * @author Toni Baumann (bauma12@bfh.ch)
 * @date   May 23, 2011
 * @brief  Contains the user interface.
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

/*
 * Define leds, buttons and switches
 * depending on target hardware
 */
#ifdef CARME
 #define POWER_SWITCH		SWITCH_5
 #define POWER_SWITCH_TEXT	"S4"
 #define MILK_SWITCH		SWITCH_6
 #define MILK_SWITCH_TEXT	"S5"
 #define POWER_LED			LED_5
 #define MILK_LED			LED_6
 #define COFFEE_SENSOR_LED	LED_7
 #define MILK_SENSOR_LED	LED_8
 #define PRODUCT_1_BUTTON	BUTTON_4
 #define PRODUCT_2_BUTTON	BUTTON_3
 #define PRODUCT_3_BUTTON	BUTTON_2
 #define PRODUCT_4_BUTTON	BUTTON_1
 #define PRODUCT_1_BUTTON_TEXT	"T3"
 #define PRODUCT_2_BUTTON_TEXT	"T2"
 #define PRODUCT_3_BUTTON_TEXT	"T1"
 #define PRODUCT_4_BUTTON_TEXT	"T0"
 #define PRODUCT_1_LED		LED_4
 #define PRODUCT_2_LED		LED_3
 #define PRODUCT_3_LED		LED_2
 #define PRODUCT_4_LED		LED_1
#elif defined(ORCHID)
 #define POWER_SWITCH		SWITCH_1
 #define POWER_SWITCH_TEXT	"S1"
 #define MILK_SWITCH		SWITCH_2
 #define MILK_SWITCH_TEXT	"S2"
 #define POWER_LED			LED_1
 #define MILK_LED			LED_2
 #define COFFEE_SENSOR_LED	LED_3
 #define MILK_SENSOR_LED	LED_4
 #define PRODUCT_1_BUTTON	BUTTON_4
 #define PRODUCT_2_BUTTON	BUTTON_3
 #define PRODUCT_3_BUTTON	BUTTON_2
 #define PRODUCT_4_BUTTON	BUTTON_1
 #define PRODUCT_1_BUTTON_TEXT	"S8"
 #define PRODUCT_2_BUTTON_TEXT	"S7"
 #define PRODUCT_3_BUTTON_TEXT	"S6"
 #define PRODUCT_4_BUTTON_TEXT	"S5"
 #define PRODUCT_1_LED		LED_8
 #define PRODUCT_2_LED		LED_7
 #define PRODUCT_3_LED		LED_6
 #define PRODUCT_4_LED		LED_5
#endif

/* Some window related constants */
#define WIN_BORDER	5

/* Define font name and location */
#define FONTNAME	"/usr/fonts/truetype/arial.ttf"

/* maximum numbers of products able to display */
#define MAX_PRODUCTS 4

/* define warning blink interval */
#define WARNING_BLINK_TIME_ON	1000
#define WARNING_BLINK_TIME_OFF	2000

#define MWINCLUDECOLORS
#include "nano-X.h"
#include "model.h"

/**
 * data type for a void function
 */
typedef void (*CallViewAction)();



/**
 * Update current view on display
 * gets called by logic.c as an observer
 * as a change occurs
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

/**
 * possible actions datatype
 */
typedef struct {
	CallViewAction	run;
	CallViewAction	activate;
	CallViewAction	deactivate;
	CallViewAction	update;
} CallViewActions;

/**
 * persistent displaystate for userinterface
 */
typedef struct {
	GR_WINDOW_ID			gWinID;
	GR_IMAGE_ID 			imageID;
	GR_IMAGE_INFO 			imageInfo;
	GR_GC_ID				gContextID, gMilkSelID, gProdID[4], gMilkSensorID, gCoffeeSensorID;
	GR_EVENT	   			event;
	GR_SCREEN_INFO  		screenInfo;
	GR_FONT_ID				font;
	int						winSizeX, winSizeY;
	CallViewActions			actions;
} DisplayState;


/**
 * get displaystate pointer, gets called by uiView modules
 */
extern DisplayState* getDisplayState(void);

/**
 * get coffeemaker state reference
 * called by uiView modules
 */
extern CoffeeMakerViewModel * getCoffeeMakerState(void);

/**
 * get updated coffeemaker state reference
 * called by uiView modules
 */
extern CoffeeMakerViewModel * getNewCoffeeMakerState(void);

/**
 * Display milk selection state
 * @param int state TRUE if milk selected, otherwise FALSE
 */
extern void showMilkSelection(int state);

/**
 * Show Product X in view
 * @param int productIndex (1-4)
 */
extern void showProduct(int productIndex);

/**
 * Heartbeat function for ongoing view tasks.
 * Gets constantly called by controller.c
 * @return TRUE if successful
 */
extern int runUserInterface(void);

/**
 * Determine which product led is active in work view
 * @return led ID for active product (PRODUCT_1_LED - PRODUCT_4_LED)
 */
extern int getActiveProductLedId(void);

/**
 * Display milk sensor state
 * @param int state TRUE if no milk detected, FALSE otherwise
 */
extern void showMilkSensor(int state);

/**
 * Display coffee sensor state
 * @param int state TRUE if no coffee detected, FALSE otherwise
 */
extern void showCoffeeSensor(int state);

#endif /* USERINTERFACE_H_ */


