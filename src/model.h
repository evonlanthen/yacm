/**
 * View model
 *
 * Defines the view model types.
 *
 * @file    model.h
 * @version 0.1
 * @author  Ronny Stauffer (staur3@bfh.ch)
 * @date    May 23, 2011
 */

#ifndef MODEL_H_
#define MODEL_H_

/**
 * Represents the coffee maker state.
 */
typedef enum {
	coffeeMaker_off,
	coffeeMaker_initializing,
	coffeeMaker_idle,
	coffeeMaker_producing
} CoffeeMakerState;

/**
 * Represents the milk preselection state.
 */
typedef enum {
  milkPreselection_off,
  milkPreselection_on
} MilkPreselectionState;

/**
 * Represents the coffee maker to a view.
 */
typedef struct {
	CoffeeMakerState state; /**< The coffee maker's state. */
	int isCoffeeAvailable; /**< Is the coffee ingredient available? */
	int isMilkAvailable; /**< Is the milk ingredient available? */
	unsigned int numberOfProducts; /**< The number of defined products. */
	int milkPreselectionState; /**< The milk preselection state. */
	int isMakingCoffee; /**< Is the coffee maker currently making coffee? */
} CoffeeMakerViewModel;

/**
 * Represents a product definition to a view.
 */
typedef struct {
	char name[256]; /**< The product's name. */
} ProductViewModel;

/**
 * Represents an activity within the coffee making process.
 */
typedef enum {
	coffeeMakingActivity_warmingUp,
	coffeeMakingActivity_withMilkGateway,
	coffeeMakingActivity_deliveringMilk,
	coffeeMakingActivity_deliveringCoffee,
	coffeeMakingActivity_finished,
	coffeeMakingActivity_error,
	coffeeMakingActivity_undefined
} CoffeeMakingActivity;

/**
 * Represents an ongoing coffee making process instance to a view.
 */
typedef struct {
	unsigned int productIndex; /**< The index of the product currently produced. The index is starting with 0. */
	int withMilk; /**< Is the product produced with milk? */
	CoffeeMakingActivity currentActivity; /**< The activity which is currently executed. */
} MakeCoffeeProcessInstanceViewModel;

#endif /* MODEL_H_ */
