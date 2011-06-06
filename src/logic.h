/**
 * Business logic
 *
 * Contains the business logic.
 *
 * @file    logic.h
 * @version 0.1
 * @author  Ronny Stauffer (staur3@bfh.ch)
 * @date    May 23, 2011
 */

#ifndef LOGIC_H_
#define LOGIC_H_

#include "model.h"

/**
 * Sets up the business logic.
 * @return Returns TRUE if successful, otherwise FALSE.
 */
extern int setUpBusinessLogic();

/**
 * Tears down the business logic.
 * @return Returns TRUE if successful, otherwise FALSE.
 */
extern int tearDownBusinessLogic();

/**
 * Heartbeat function for ongoing business logic tasks.
 * Gets constantly called by the main controller.
 */
extern void runBusinessLogic();

/**
 * A handler which will be called upon a model change.
 */
typedef void (*NotifyModelChanged)();

/**
 * Registers a model observer.
 * A model observer is notified if the model changes.
 * @param pObserver The handler which will be called if the model changes.
 */
extern void registerModelObserver(NotifyModelChanged pObserver);

/**
 * Gets the view model of the coffee maker.
 * @return The coffee maker view model.
 */
extern CoffeeMakerViewModel getCoffeeMakerViewModel();

/**
 * Gets the view model of the specified product definition.
 * @param productIndex The index of the product definition.
 *   The index is starting with 0.
 * @return The product view model.
 */
extern ProductViewModel getProductViewModel(unsigned int productIndex);

/**
 * Gets the view model of an ongoing coffee making process instance.
 * @return The coffee making process instance view model.
 */
extern MakeCoffeeProcessInstanceViewModel getCoffeeMakingProcessInstanceViewModel();

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
extern void setMilkPreselection(MilkPreselectionState state);

/**
 * Requests the coffee maker to start making coffee.
 * This triggers the start of the coffee making process.
 * @param The index of the product to produce.
 */
extern void startMakingCoffee(unsigned int productIndex);

/**
 * Aborts an ongoing coffee making process instance.
 */
extern void abortMakingCoffee();

#endif /* LOGIC_H_ */
