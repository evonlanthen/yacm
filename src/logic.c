/**
 * @file   logic.c
 * @author Ronny Stauffer (staur3@bfh.ch)
 * @date   May 23, 2011
 * @brief  Contains the business logic.
 *
 * Contains the business logic.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defines.h"
#include "logic.h"

static void setUpProducts();

static NotifyModelChanged observer;

static struct CoffeeMaker coffeeMaker = {
		.state = coffeeMaker_off,
		.coffee.isAvailable = TRUE,
		.milk.isAvailable = TRUE//,
		//.products = &coffeeProductListElement
};

static struct ProductViewModel undefinedProduct = {
		.name = "<undefined>"
};

int initBusinessLogic() {
	setUpProducts();

	return 0;
}

/**
 * Shut down business logic.
 */
// TODO Rename to tearDown()?
int shutdownBusinessLogic() {
	return 0;
}

void registerModelObserver(NotifyModelChanged pObserver) {
	observer = pObserver;
}

static void notifyObservers() {
	if (observer) {
		(*observer)();
	}
}




static void * newObject(void *initializer, size_t size) {
	void *object = malloc(size);
	memcpy(object, initializer, size);

	return object;
}

/**
 * Sets up product definitions (either hardcoded or by reading from a file?).
 */
static void setUpProducts() {
	int i = 2;
	//struct Product coffeeProduct = {
	//		.name = "Coffee"
	//};
	struct Product *coffeeProduct = newObject(&(struct Product) {
		.name = "Coffee"
	}, sizeof(struct Product));
	//struct Product espressoProduct = {
	//		.name = "Espresso"
	//};
	struct Product *espressoProduct = newObject(&(struct Product) {
		.name = "Espresso"
	}, sizeof(struct Product));
	//struct Product ristrettoProduct = {
	//		.name = "Ristretto"
	//};
	struct Product *ristrettoProduct = newObject(&(struct Product) {
		.name = "Ristretto"
	}, sizeof(struct Product));
	//struct ProductListElement ristrettoProductListElement = {
	//		.product = &ristrettoProduct
	//};
	//struct ProductListElement espressoProductListElement = {
	//		.product = &espressoProduct,
	//		.next = &ristrettoProductListElement
	//};
	//struct ProductListElement coffeeProductListElement = {
	//		.product = &coffeeProduct,
	//		.next = &espressoProductListElement
	//};
	struct Product *products[] = {
			coffeeProduct,
			espressoProduct,
			ristrettoProduct
	};
	struct ProductListElement *nextProductListElement = NULL;
	for (i = 2; i >= 0; i--) {
		struct ProductListElement *productListElement = newObject(&(struct ProductListElement) {
			.product = products[i],
			.next = nextProductListElement
		}, sizeof(struct ProductListElement));
		nextProductListElement = productListElement;
	}
	coffeeMaker.products = nextProductListElement;
}

/* Many errors
struct CoffeeMakerViewModel getCoffeeMakerViewModel() {
	// Count number of products
	unsigned int numberOfProducts = 0;
	struct ProductListElement *productListElement = coffeeMaker.products;
	while (productListElement) {
		numberOfProducts++;

		productListElement = productListElement->next;
	}

	// Map to view model
	struct CoffeeMakerViewModel coffeeMakerViewModel = {
			.state = coffeeMaker.state,
			.isCoffeeAvailable = coffeeMaker.coffee.isAvailable,
			.isMilkAvailable = coffeeMaker.milk.isAvailable,
			.numberOfProducts = numberOfProducts
	};

	return coffeeMakerViewModel;
}
*/
static struct ProductListElement * getProductListElement(unsigned int productIndex) {
	unsigned int i = 0;
	struct ProductListElement *productListElement = coffeeMaker.products;
	while (productListElement) {
		if (i == productIndex) {
			return productListElement;
		}

		i++;

		productListElement = productListElement->next;
	}

	return NULL;
}

struct ProductViewModel getProductViewModel(unsigned int productIndex) {
	struct ProductListElement *productListElement = getProductListElement(productIndex);
	if (productListElement) {
		// Map to view model
		struct ProductViewModel productViewModel = {
			//.name = productListElement->product->name
		};
		strcpy(productViewModel.name, productListElement->product->name);

		return productViewModel;
	}

	return undefinedProduct;
}

void setCoffeeMakerState(enum CoffeeMakerState state) {
	printf("Setting coffee maker state to %d...\n", state);

	coffeeMaker.state = state;

	notifyObservers();

	printf("...done.\n");
}
