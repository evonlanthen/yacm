/*
 * logic.h
 *
 *  Created on: May 25, 2011
 *      Author: Ronny Stauffer (staur3@bfh.ch)
 */

#ifndef LOGIC_H_
#define LOGIC_H_

#include "model.h"

/**
 * Initialize business logic.
 */
// TODO Rename to setUp()?
extern int initBusinessLogic();

/**
 * Shut down business logic.
 */
// TODO Rename to tearDown()?
extern int shutdownBusinessLogic();

typedef void (*NotifyModelChanged)();

extern void registerModelObserver(NotifyModelChanged);

/**
 * Gets the view model of the coffee maker.
 * @return The view model.
 */
extern struct CoffeeMakerViewModel getCoffeeMakerViewModel();

/**
 * Gets the view model of the specified product.
 * @param productIndex The index of the product.
 * @return The view model.
 */
extern struct ProductViewModel getProductViewModel(unsigned int productIndex);

/**
 * Sets the coffee maker's state.
 *
 * ...
 *
 * @param state The coffee maker's state.
 */
extern void setCoffeeMakerState(enum CoffeeMakerState state);

#endif /* LOGIC_H_ */
