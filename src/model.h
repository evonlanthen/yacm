/*
 * model.h
 *
 *  Created on: May 23, 2011
 *      Author: Ronny Stauffer (staur3@bfh.ch)
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

/**
 * Represents the coffee ingredient.
 */
struct Coffee {
	int isAvailable;
};

/**
 * Represents the milk ingredient.
 */
struct Milk {
	int isAvailable;
};

/**
 * Represents a product definition.
 */
struct Product {
	char name[256];
};

struct ProductListElement {
	struct Product *product;
	struct ProductListElement *next;
};

enum CoffeeMakingActivity {
	coffeeMakingActivity_undefined,
	coffeeMakingActivity_warmingUp,
	coffeeMakingActivity_deliveringMilk,
	coffeeMakingActivity_deliveringCoffee,
	coffeeMakingActivity_finished
};

/**
 * Represents an ongoing coffee making process instance.
 *
 */
struct MakeCoffeeProcessInstance {
	enum CoffeeMakingActivity currentActivity;
};

/**
 * Represents the coffee maker.
 */
struct CoffeeMaker {
	enum CoffeeMakerState state;
	struct Coffee coffee;
	struct Milk milk;
	struct ProductListElement *products;
	enum MilkPreselectionState milkPreselectionState;
	struct MakeCoffeeProcessInstance *ongoingCoffeeMaking;
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

/*
 * Represents an ongoing coffee making process instance to a view.
 */
struct MakeCoffeeProcessInstanceViewModel {
	enum CoffeeMakingActivity currentActivity;
};

#endif /* MODEL_H_ */
