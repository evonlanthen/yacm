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
extern int setUpDisplay(void);

/**
 * turn off display and clean up
 */
extern int tearDownDisplay(void);


#endif /* USERINTERFACE_H_ */


