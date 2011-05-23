/*
 * model.h
 *
 *  Created on: May 23, 2011
 *      Author: staufferr
 */

#ifndef MODEL_H_
#define MODEL_H_

enum CoffeeMakerState {
	coffeeMaker_off,
	coffeeMaker_initializing,
	coffeeMaker_idle,
	coffeeMaker_producing
};

struct Coffee {
	int isAvailable;
};

struct Milk {
	int isAvailable;
};

struct Product {
	char *name;
};

struct ProductListElement {
	struct Product *product;
	struct ListElement *next;
};

struct CoffeeMaker {
	enum CoffeeMakerState state;
	struct Coffee coffee;
	struct Milk milk;
	struct ProductListElement *products;
};

struct ProductViewModel {
	char *name;
};

struct ViewModel {
	enum CoffeeMakerState coffeeMakerState;
	int isCoffeeAvailable;
	int isMilkAvailable;
	struct ProductViewModel products[];
};

#endif /* MODEL_H_ */
