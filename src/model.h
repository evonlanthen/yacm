/*
 * model.h
 *
 *  Created on: May 23, 2011
 *      Author: Ronny Stauffer (staur3@bfh.ch)
 */

#ifndef MODEL_H_
#define MODEL_H_

enum CoffeeMakerState {
	coffeeMaker_off,
	coffeeMaker_initializing,
	coffeeMaker_idle,
	coffeeMaker_producing
};

/**
 * Models the coffee ingredient.
 */
struct Coffee {
	int isAvailable;
};

struct Milk {
	int isAvailable;
};

struct Product {
	char name[256];
};

struct ProductListElement {
	struct Product *product;
	struct ProductListElement *next;
};

struct CoffeeMaker {
	enum CoffeeMakerState state;
	struct Coffee coffee;
	struct Milk milk;
	struct ProductListElement *products;
};


struct CoffeeMakerViewModel {
	enum CoffeeMakerState state;
	int isCoffeeAvailable;
	int isMilkAvailable;
	unsigned int numberOfProducts;
};

struct ProductViewModel {
	char name[256];
};


#endif /* MODEL_H_ */
