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
enum CoffeeMakerState {
	coffeeMaker_off,
	coffeeMaker_initializing,
	coffeeMaker_idle,
	coffeeMaker_producing
};

/**
 * Represents the milk preselection state.
 */
enum MilkPreselectionState {
  milkPreselection_off,
  milkPreselection_on
};

/*
 * Represents the coffee maker to a view.
 */
struct CoffeeMakerViewModel {
	/**
	 * The coffee maker's state.
	 */
	enum CoffeeMakerState state;
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
};

/*
 * Represents a product definition to a view.
 */
struct ProductViewModel {
	/*
	 * The product's name.
	 */
	char name[256];
};

/**
 * Represents an activity within the coffee making process.
 */
enum CoffeeMakingActivity {
	coffeeMakingActivity_warmingUp,
	coffeeMakingActivity_withMilkGateway,
	coffeeMakingActivity_deliveringMilk,
	coffeeMakingActivity_deliveringCoffee,
	coffeeMakingActivity_finished,
	coffeeMakingActivity_undefined
};

/*
 * Represents an ongoing coffee making process instance to a view.
 */
struct MakeCoffeeProcessInstanceViewModel {
	unsigned int productIndex;
	int withMilk;
	enum CoffeeMakingActivity currentActivity;
};

#endif /* MODEL_H_ */
