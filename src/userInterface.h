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
extern int updateView(int view);

/**
 * Initialize display
 */
extern int initDisplay(void);

/**
 * turn off display and clean up
 */
extern int shutdownDisplay(void);

#endif /* USERINTERFACE_H_ */


