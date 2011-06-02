/**
 * Domain model
 *
 * Defines the domain model types.
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

/*
 * Represents the coffee maker to a view.
 */
typedef struct {
	/**
	 * The coffee maker's state.
	 */
	CoffeeMakerState state;
	/*
	 * Is the coffee ingredient available?
	 */
	int isCoffeeAvailable;
	/*
	 * Is the milk ingredient available?
	 */
	int isMilkAvailable;
	/*
	 * The number of defined products.
	 */
	unsigned int numberOfProducts;
	/*
	 * The milk preselection state.
	 */
	int milkPreselectionState;
	/*
	 * Is the coffee maker currently making coffee?
	 */
	int isMakingCoffee;
} CoffeeMakerViewModel;

/*
 * Represents a product definition to a view.
 */
typedef struct {
	/*
	 * The product's name.
	 */
	char name[256];
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
	coffeeMakingActivity_undefined
} CoffeeMakingActivity;

/*
 * Represents an ongoing coffee making process instance to a view.
 */
typedef struct {
	unsigned int productIndex;
	int withMilk;
	CoffeeMakingActivity currentActivity;
} MakeCoffeeProcessInstanceViewModel;

#endif /* MODEL_H_ */
