/*
 * userInterface.h
 *
 *  Created on: May 23, 2011
 *      Author: Toni Baumann
 */

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

/**
 * Update current view on display
 * parameter should be the state construct
 */
extern int updateView(struct CoffeeMaker coffeemaker);

/**
 * Initialize display
 */
extern int initDisplay(void);

/**
 * turn off display and clean up
 */
extern int shutdownDisplay(void);

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

#endif /* USERINTERFACE_H_ */


