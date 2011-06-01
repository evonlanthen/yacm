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
 * Sets up the business logic.
 */
extern int setUpBusinessLogic();

/**
 * Tears down the business logic.
 */
extern int tearDownBusinessLogic();

extern void runBusinessLogic();

/**
 * A handler which will be called upon a model change.
 */
typedef void (*NotifyModelChanged)();

/**
 * Registers a model observer.
 * A model observer is notified if the model changes.
 * @param observer The handler which will be called if the model changes.
 */
extern void registerModelObserver(NotifyModelChanged observer);

/**
 * Gets the view model of the coffee maker.
 * @return The view model.
 */
extern struct CoffeeMakerViewModel getCoffeeMakerViewModel();

/**
 * Gets the view model of the specified product definition.
 * @param productIndex The index of the product definition.
 * @return The view model.
 */
extern struct ProductViewModel getProductViewModel(unsigned int productIndex);

/**
 * Gets the view model of an ongoing coffee making process instance.
 * @return The view model.
 */
extern struct MakeCoffeeProcessInstanceViewModel getCoffeeMakingProcessInstanceViewModel();

/**
 * Switches the coffee maker on.
 */
extern void switchOn();

/**
 * Switches the coffee maker off.
 */
extern void switchOff();

/**
 * Sets the milk preselection state.
 * @param @state The milk preselection state.
 */
extern void setMilkPreselection(enum MilkPreselectionState state);

/**
 * Requests the coffee maker to start making coffee.
 * This triggers the start of the coffee making process.
 * @param The index of the product to produce.
 */
extern void startMakingCoffee(unsigned int productIndex);

/**
 * Aborts an ongoing coffee making process.
 */
extern void abortMakingCoffee();

#endif /* LOGIC_H_ */
