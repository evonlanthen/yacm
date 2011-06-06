/**
 * @file   logic.c
 * @author Ronny Stauffer (staur3@bfh.ch)
 * @date   May 23, 2011
 * @brief  Contains the business logic.
 *
 * Contains the business logic.
 *
 * Module Contents:
 * - Constants
 * - (Internal) Memory management interface
 * - (Public) Model observer registration and (internal) notification interface
 * - (Internal) Domain model types and instances
 *   - Product collection helpers
 *   - Model initializers
 *   - State machine definitions
 *   - Ongoing tasks
 * - (Public) View Model presentation interface
 * - (Public) Operations presentation interface
 * - (Public) Initialization & Heartbeat interface
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "defines.h"
#include "logic.h"
#include "stateMachineEngine.h"
#include "sensorController.h"
#include "machineController.h"
#include "timer.h"

// =============================================================================
// Constants
// =============================================================================

// -----------------------------------------------------------------------------
// Duration constants
// -----------------------------------------------------------------------------

/**
 * The initialization duration.
 */
#define INITIALIZING_DURATION 2000
/**
 * The warming up duration.
 */
#define WARMING_UP_DURATION 1000
/**
 * The milk delivery duration.
 */
#define DELIVERING_MILK_DURATION 3000
/**
 * The coffee delivery duration.
 */
#define DELIVERING_COFFEE_DURATION 5000

// =============================================================================
// Memory management interface
// =============================================================================

static void * newObject(void *initializer, size_t size);
static void deleteObject(void *object);

/**
 * Instanciates and initializes a new object.
 */
static void * newObject(void *initializer, size_t size) {
	void *object = malloc(size);
	memcpy(object, initializer, size);

	return object;
}

/**
 * Deletes an object
 */
static void deleteObject(void *object) {
	free(object);
}

// =============================================================================
// Model observer registration and notification interface
// =============================================================================

static void notifyObservers();

/**
 * A registered model observer.
 */
static NotifyModelChanged observer;

/**
 * @copydoc registerModelObserver(NotifyModelChanged pObserver)
 */
void registerModelObserver(NotifyModelChanged pObserver) {
	observer = pObserver;
}

/**
 * Notifies the model observers of a model change.
 */
static void notifyObservers() {
#ifdef DEBUG
	printf("Notifying model observers...\n");
#endif

	if (observer) {
		(*observer)();
	}

#ifdef DEBUG
	printf("Model observers notified.\n");
#endif
}

// =============================================================================
// Domain model types and instances
// =============================================================================

/**
 * Represents the coffee ingredient.
 */
typedef struct {
	int isAvailable; /**< Is coffee available? */
	int emptyTankSensorId; /**< Defines the coffee tank empty sensor. */
} Coffee;

/**
 * Represents the milk ingredient.
 */
typedef struct {
	int isAvailable; /**< Is milk available? */
	int emptyTankSensorId; /**< Defines the milk tank empty sensor. */
} Milk;

/**
 * Represents a product definition.
 */
typedef struct {
	char name[256]; /**< The product's name. */
} Product;

/**
 * A wrapper for a 'Product' instance, allowing it to add the product to the product definition collection.
 */
typedef struct ProductListElement {
	Product *product; /**< The wrapped product. */
	struct ProductListElement *next; /**< The next element in the collection. */
} ProductListElement;

/**
 * Represents an ongoing coffee making process instance.
 */
typedef struct {
	Product *product; /**< The product currently produced. */
	int withMilk; /**< Is the product produced with milk? */
	CoffeeMakingActivity currentActivity; /**< The activity which is currently executed. */
} MakeCoffeeProcessInstance;

/**
 * Represents the coffee maker.
 */
typedef struct {
	CoffeeMakerState state; /**< The coffee maker's state. */
	Coffee coffee; /**< The coffee ingredient. */
	Milk milk; /**< The milk ingredient. */
	ProductListElement *products; /**< The product definition collection. */
	MilkPreselectionState milkPreselectionState; /**< The milk preselection state. */
	MakeCoffeeProcessInstance *ongoingCoffeeMaking; /**< A possibly ongoing coffee making process instance. */
} CoffeeMaker;

/**
 * The coffee maker model instance.
 */
static CoffeeMaker coffeeMaker = {
		.state = coffeeMaker_off,
		.coffee.isAvailable = TRUE,
		.coffee.emptyTankSensorId = SENSOR_1,
		.milk.isAvailable = TRUE,
		.milk.emptyTankSensorId = SENSOR_2,
		.milkPreselectionState = milkPreselection_off
};

/**
 * Special case value for an undefined product index.
 */
#define UNDEFINED_PRODUCT_INDEX 999

/**
 * Special case value for an undefined product definition.
 */
#define UNDEFINED_PRODUCT (ProductViewModel){ \
		.name = "<undefined>" \
}

/**
 * Special case value for an inexistent coffee making process instance.
 */
#define INEXISTENT_COFFEE_MAKING_PROCESS_INSTANCE (MakeCoffeeProcessInstanceViewModel){ \
		.currentActivity = coffeeMakingActivity_undefined \
}

// =============================================================================
// Product collection helpers
// =============================================================================

static unsigned int getNumberOfProducts();
static Product * getProduct(unsigned int productIndex);
static unsigned int getProductIndex(Product *product);

/**
 * Gets the number of product definitions.
 */
static unsigned int getNumberOfProducts() {
	unsigned int numberOfProducts = 0;

	ProductListElement *productListElement = coffeeMaker.products;
	while (productListElement) {
		numberOfProducts++;

		productListElement = productListElement->next;
	}

	return numberOfProducts;
}

/**
 * Returns the index of the given product.
 */
static unsigned int getProductIndex(Product *product) {
	unsigned int productIndex = 0;

	ProductListElement *productListElement = coffeeMaker.products;
	while (productListElement) {
		if (productListElement->product == product) {
			return productIndex;
		}

		productIndex++;

		productListElement = productListElement->next;
	}

	return UNDEFINED_PRODUCT_INDEX;
}

/**
 * Returns the product with the specified index.
 */
static Product * getProduct(unsigned int productIndex) {
	unsigned int i = 0;

	ProductListElement *productListElement = coffeeMaker.products;
	while (productListElement) {
		if (i == productIndex) {
			return productListElement->product;
		}

		i++;

		productListElement = productListElement->next;
	}

	return NULL;
}

// =============================================================================
// Model initializers
// =============================================================================

/**
 * Sets up product definitions.
 * Preliminary the setup is done hardcoded.
 * In the future definitions could possibly read from a file?
 */
static void setUpProducts() {
	Product *coffeeProduct = newObject(&(Product) {
		.name = "Coffee"
	}, sizeof(Product));
	Product *espressoProduct = newObject(&(Product) {
		.name = "Espresso"
	}, sizeof(Product));
	Product *ristrettoProduct = newObject(&(Product) {
		.name = "Ristretto"
	}, sizeof(Product));
	Product *products[] = {
			coffeeProduct,
			espressoProduct,
			ristrettoProduct
	};
	ProductListElement *nextProductListElement = NULL;
	int i;
	for (i = 2; i >= 0; i--) {
		ProductListElement *productListElement = newObject(&(ProductListElement) {
			.product = products[i],
			.next = nextProductListElement
		}, sizeof(ProductListElement));
		nextProductListElement = productListElement;
	}
	coffeeMaker.products = nextProductListElement;
}

// =============================================================================
// State machine definitions
// =============================================================================

// =============================================================================
// Main state machine
// =============================================================================

static StateMachine coffeeMakingProcessMachine;

// -----------------------------------------------------------------------------
// Events
// -----------------------------------------------------------------------------

/**
 * Represents a coffee maker event.
 */
typedef enum {
	event_switchedOn,
	event_switchedOff,
	event_isInitialized,
	event_productSelected,
	event_productionProcessAborted,
	event_productionProcessIsFinished,
	event_ingredientTankIsEmpty
} CoffeeMakerEvent;

// -----------------------------------------------------------------------------
// Off state
// -----------------------------------------------------------------------------

static void offStateEntryAction() {
	coffeeMaker.state = coffeeMaker_off;

	notifyObservers();
}

static State offState = {
	.stateIndex = coffeeMaker_off,
	.entryAction = offStateEntryAction
};

// -----------------------------------------------------------------------------
// Initializing state
// -----------------------------------------------------------------------------

// Timers
static TIMER initTimer;

static void initializingStateEntryAction() {
	coffeeMaker.state = coffeeMaker_initializing;

	initTimer = setUpTimer(INITIALIZING_DURATION);

	notifyObservers();
}

static Event initializingStateDoAction() {
	if (isTimerElapsed(initTimer)) {
		return event_isInitialized;
	}

	return NO_EVENT;
}

static State initializingState = {
	.stateIndex = coffeeMaker_initializing,
	.entryAction = initializingStateEntryAction,
	.doAction = initializingStateDoAction
};

// -----------------------------------------------------------------------------
// Idle state
// -----------------------------------------------------------------------------

static void idleStateEntryAction() {
	coffeeMaker.state = coffeeMaker_idle;

	notifyObservers();
}

static State idleState = {
	.stateIndex = coffeeMaker_idle,
	.entryAction = idleStateEntryAction
};

// -----------------------------------------------------------------------------
// Producing state
// -----------------------------------------------------------------------------

static unsigned int selectedProductIndex;

static int producingStatePrecondition() {
	// Only start production if...
	// - no coffee making process is already running
	// - coffee is available (coffee tank is not empty)
	// - milk preselection is off or
	//   milk is available (milk tank is not emtpy)
	// - selected product is defined
	return !coffeeMaker.ongoingCoffeeMaking
		&& coffeeMaker.coffee.isAvailable
		&& (coffeeMaker.milkPreselectionState != milkPreselection_on || coffeeMaker.milk.isAvailable)
		&& selectedProductIndex < getNumberOfProducts();
}

static void startMakeCoffeeProcess(unsigned int productIndex) {
	coffeeMaker.ongoingCoffeeMaking = newObject(&(MakeCoffeeProcessInstance) {
		.product = getProduct(productIndex),
		.withMilk = coffeeMaker.milkPreselectionState == milkPreselection_on ? TRUE : FALSE
	}, sizeof(MakeCoffeeProcessInstance));

	coffeeMaker.state = coffeeMaker_producing;

	setUpStateMachine(&coffeeMakingProcessMachine);
}

static void producingStateEntryAction() {
	startMakeCoffeeProcess(selectedProductIndex);

	notifyObservers();
}

static Event producingStateDoAction() {
	runStateMachine(&coffeeMakingProcessMachine);

	// Check coffee making process instance progress
	if (coffeeMaker.ongoingCoffeeMaking->currentActivity == coffeeMakingActivity_finished
		|| coffeeMaker.ongoingCoffeeMaking->currentActivity == coffeeMakingActivity_error) {
		return event_productionProcessIsFinished;
	}

	return NO_EVENT;
}

static void abortMakeCoffeeProcessInstance() {
	if (coffeeMaker.ongoingCoffeeMaking) {
		abortStateMachine(&coffeeMakingProcessMachine);

		deleteObject(coffeeMaker.ongoingCoffeeMaking);
		coffeeMaker.ongoingCoffeeMaking = NULL;
	}
}

static void producingStateExitAction() {
	abortMakeCoffeeProcessInstance();

	notifyObservers();
}

static State producingState = {
	.stateIndex = coffeeMaker_producing,
	.precondition = producingStatePrecondition,
	.entryAction = producingStateEntryAction,
	.doAction = producingStateDoAction,
	.exitAction = producingStateExitAction
};

// -----------------------------------------------------------------------------
// State transitions
// -----------------------------------------------------------------------------

static StateMachine stateMachine = {
	.numberOfEvents = 7,
	.initialState = &offState,
	.transitions = {
		/* coffeeMaker_off: */
			/* event_switchedOn: */ &initializingState,
			/* event_switchedOff: */ NULL,
			/* event_isInitialized: */ NULL,
			/* event_productSelected: */ NULL,
			/* event_productionProcessAborted: */ NULL,
			/* event_productionProcessIsFinished: */ NULL,
			/* event_ingredientTankIsEmpty: */ NULL,
		/* coffeeMaker_initializing: */
			/* event_switchedOn: */ NULL,
			/* event_switchedOff: */ NULL,
			/* event_isInitialized: */ &idleState,
			/* event_productSelected: */ NULL,
			/* event_productionProcessAborted: */ NULL,
			/* event_productionProcessIsFinished: */ NULL,
			/* event_ingredientTankIsEmpty: */ NULL,
		/* coffeeMaker_idle: */
			/* event_switchedOn: */ NULL,
			/* event_switchedOff: */ &offState,
			/* event_isInitialized: */ NULL,
			/* event_productSelected: */ &producingState,
			/* event_productionProcessAborted: */ NULL,
			/* event_productionProcessIsFinished: */ NULL,
			/* event_ingredientTankIsEmpty: */ NULL,
		/* coffeeMaker_producing: */
			/* event_switchedOn: */ NULL,
			/* event_switchedOff: */ &offState,
			/* event_isInitialized: */ NULL,
			/* event_productSelected: */ NULL,
			/* event_productionProcessAborted: */ &idleState,
			/* event_productionProcessIsFinished: */ &idleState,
			/* event_ingredientTankIsEmpty: */ &idleState
		}
};

// =============================================================================
// 'Make coffee' process
// =============================================================================

// -----------------------------------------------------------------------------
// Events
// -----------------------------------------------------------------------------

/**
 * Represents a coffee making event.
 */
typedef enum {
	coffeeMakingEvent_isWarmedUp,
	coffeeMakingEvent_deliverMilk,
	coffeeMakingEvent_milkDelivered,
	coffeeMakingEvent_deliverCoffee,
	coffeeMakingEvent_coffeeDelivered,
	coffeeMakingEvent_ingredientTankIsEmpty
} CoffeeMakingEvent;

// -----------------------------------------------------------------------------
// Warming Up activity
// -----------------------------------------------------------------------------

// Timer
static TIMER warmingUpTimer;

static void warmingUpActivityEntryAction() {
	coffeeMaker.ongoingCoffeeMaking->currentActivity = coffeeMakingActivity_warmingUp;

	warmingUpTimer = setUpTimer(WARMING_UP_DURATION);

	notifyObservers();
}

static Event warmingUpActivityDoAction() {
	if (isTimerElapsed(warmingUpTimer)) {
		return coffeeMakingEvent_isWarmedUp;
	}

	return NO_EVENT;
}

static State warmingUpActivity = {
	.stateIndex = coffeeMakingActivity_warmingUp,
	.entryAction = warmingUpActivityEntryAction,
	.doAction = warmingUpActivityDoAction
};

// -----------------------------------------------------------------------------
// With Milk gateway
// -----------------------------------------------------------------------------

static Event withMilkGatewayDoAction() {
	if (coffeeMaker.ongoingCoffeeMaking->withMilk) {
		return coffeeMakingEvent_deliverMilk;
	} else {
		return coffeeMakingEvent_deliverCoffee;
	}

	return NO_EVENT;
}

static State withMilkGateway = {
	.stateIndex = coffeeMakingActivity_withMilkGateway,
	.doAction = withMilkGatewayDoAction
};

// -----------------------------------------------------------------------------
// Delivering Milk activity
// -----------------------------------------------------------------------------

static void deliveringMilkActivityEntryAction() {
	coffeeMaker.ongoingCoffeeMaking->currentActivity = coffeeMakingActivity_deliveringMilk;

	startMachine(ingredient_milk, DELIVERING_MILK_DURATION);

	notifyObservers();
}

static Event deliveringMilkActivityDoAction() {
	if (!machineRunning()) {
		return coffeeMakingEvent_milkDelivered;
	}

	if (!coffeeMaker.milk.isAvailable) {
		return coffeeMakingEvent_ingredientTankIsEmpty;
	}

	return NO_EVENT;
}

static void deliveringMilkActivityExitAction() {
	stopMachine();
}

static State deliveringMilkActivity = {
	.stateIndex = coffeeMakingActivity_deliveringMilk,
	.entryAction = deliveringMilkActivityEntryAction,
	.doAction = deliveringMilkActivityDoAction,
	.exitAction = deliveringMilkActivityExitAction
};

// -----------------------------------------------------------------------------
// Delivering Coffe activity
// -----------------------------------------------------------------------------

static void deliveringCoffeeActivityEntryAction() {
	coffeeMaker.ongoingCoffeeMaking->currentActivity = coffeeMakingActivity_deliveringCoffee;

	startMachine(ingredient_coffee, DELIVERING_COFFEE_DURATION);

	notifyObservers();
}

static Event deliveringCoffeeActivityDoAction() {
	if (!machineRunning()) {
		return coffeeMakingEvent_coffeeDelivered;
	}

	if (!coffeeMaker.coffee.isAvailable) {
		return coffeeMakingEvent_ingredientTankIsEmpty;
	}

	return NO_EVENT;
}

static void deliveringCoffeeActivityExitAction() {
	stopMachine();
}

static State deliveringCoffeeActivity = {
	.stateIndex = coffeeMakingActivity_deliveringCoffee,
	.entryAction = deliveringCoffeeActivityEntryAction,
	.doAction = deliveringCoffeeActivityDoAction,
	.exitAction = deliveringCoffeeActivityExitAction
};

// -----------------------------------------------------------------------------
// Finished state
// -----------------------------------------------------------------------------

static void finishedStateEntryAction() {
	coffeeMaker.ongoingCoffeeMaking->currentActivity = coffeeMakingActivity_finished;

	notifyObservers();
}

static State finishedState = {
	.stateIndex = coffeeMakingActivity_finished,
	.entryAction = finishedStateEntryAction
};

// -----------------------------------------------------------------------------
// Error state
// -----------------------------------------------------------------------------

static void errorStateEntryAction() {
	coffeeMaker.ongoingCoffeeMaking->currentActivity = coffeeMakingActivity_error;

	notifyObservers();
}

static State errorState = {
	.stateIndex = coffeeMakingActivity_error,
	.entryAction = errorStateEntryAction
};

// -----------------------------------------------------------------------------
// Activity/state transitions
// -----------------------------------------------------------------------------
static StateMachine coffeeMakingProcessMachine = {
	.numberOfEvents = 6,
	.initialState = &warmingUpActivity,
	.transitions = {
		/* coffeeMakingActivity_warmingUp: */
			/* coffeeMakingEvent_isWarmedUp: */ &withMilkGateway,
			/* coffeeMakingEvent_deliverMilk: */ NULL,
			/* coffeeMakingEvent_milkDelivered: */ NULL,
			/* coffeeMakingEvent_deliverCoffee: */ NULL,
			/* coffeeMakingEvent_coffeeDelivered: */ NULL,
			/* coffeeMakingEvent_ingredientTankIsEmpty: */ NULL,
		/* coffeeMakingActivity_withMilkGateway: */
			/* coffeeMakingEvent_isWarmedUp: */ NULL,
			/* coffeeMakingEvent_deliverMilk: */ &deliveringMilkActivity,
			/* coffeeMakingEvent_milkDelivered: */ NULL,
			/* coffeeMakingEvent_deliverCoffee: */ &deliveringCoffeeActivity,
			/* coffeeMakingEvent_coffeeDelivered: */ NULL,
			/* coffeeMakingEvent_ingredientTankIsEmpty: */ NULL,
		/* coffeeMakingActivity_deliveringMilk: */
			/* coffeeMakingEvent_isWarmedUp: */ NULL,
			/* coffeeMakingEvent_deliverMilk: */ NULL,
			/* coffeeMakingEvent_milkDelivered: */ &deliveringCoffeeActivity,
			/* coffeeMakingEvent_deliverCoffee: */ NULL,
			/* coffeeMakingEvent_coffeeDelivered: */ NULL,
			/* coffeeMakingEvent_ingredientTankIsEmpty: */ &errorState,
		/* coffeeMakingActivity_deliveringCoffee: */
			/* coffeeMakingEvent_isWarmedUp: */ NULL,
			/* coffeeMakingEvent_deliverMilk: */ NULL,
			/* coffeeMakingEvent_milkDelivered: */ NULL,
			/* coffeeMakingEvent_deliverCoffee: */ NULL,
			/* coffeeMakingEvent_coffeeDelivered: */ &finishedState,
			/* coffeeMakingEvent_ingredientTankIsEmpty: */ &errorState
		}
};

// =============================================================================
// Ongoing tasks
// =============================================================================

// Sensor states
static enum SensorState lastEmptyCoffeeTankSensorState = sensor_unknown;
static enum SensorState lastEmptyMilkTankSensorState = sensor_unknown;

/**
 * Checks the ingredient tank sensors.
 */
static void checkIngredientTankSensors() {
	// Coffee sensor
	enum SensorState emptyCoffeeTankSensorState = getSensorState(coffeeMaker.coffee.emptyTankSensorId);
	// If sensor state has changed...
	if (emptyCoffeeTankSensorState != lastEmptyCoffeeTankSensorState) {
		// Update model
		coffeeMaker.coffee.isAvailable = !(emptyCoffeeTankSensorState == sensor_alert);

		notifyObservers();

		lastEmptyCoffeeTankSensorState = emptyCoffeeTankSensorState;
	}

	// Milk sensor
	enum SensorState emptyMilkTankSensorState = getSensorState(coffeeMaker.milk.emptyTankSensorId);
	// If sensor state has changed...
	if (emptyMilkTankSensorState != lastEmptyMilkTankSensorState) {
		// Update model
		coffeeMaker.milk.isAvailable = !(emptyMilkTankSensorState == sensor_alert);

		notifyObservers();

		lastEmptyMilkTankSensorState = emptyMilkTankSensorState;
	}
}

// =============================================================================
// Initialization & Heartbeat interface
// =============================================================================

static int isBusinessLogicSetUp = FALSE;

/**
 * @copydoc setUpBusinessLogic
 */
int setUpBusinessLogic() {
	// Check if business logic is already set up
	if (isBusinessLogicSetUp) {
		return FALSE;
	}

	// Load product definitions
	setUpProducts();

	setUpStateMachine(&stateMachine);

	isBusinessLogicSetUp = TRUE;

	return TRUE;
}

/**
 * @copydoc tearDownBusinessLogic
 */
int tearDownBusinessLogic() {
	if (!isBusinessLogicSetUp) {
		return FALSE;
	}

	abortStateMachine(&stateMachine);

	// Delete product definitions
	ProductListElement *productListElement = coffeeMaker.products;
	coffeeMaker.products = NULL;
	while (productListElement) {
		ProductListElement *next = productListElement->next;

		deleteObject(productListElement->product);
		deleteObject(productListElement);

		productListElement = next;
	}

	isBusinessLogicSetUp = FALSE;

	return TRUE;
}

/**
 * @copydoc runBusinessLogic
 */
void runBusinessLogic() {
	// Check ingredient tank sensors
	checkIngredientTankSensors();

	// Run state machine
	runStateMachine(&stateMachine);
}

// =============================================================================
// View Model presentation interface
// =============================================================================

/**
 * @copydoc getCoffeeMakerViewModel
 */
CoffeeMakerViewModel getCoffeeMakerViewModel() {
	// Map to view model
	CoffeeMakerViewModel coffeeMakerViewModel = {
		.state = coffeeMaker.state,
		.isCoffeeAvailable = coffeeMaker.coffee.isAvailable,
		.isMilkAvailable = coffeeMaker.milk.isAvailable,
		.numberOfProducts = getNumberOfProducts(),
		.milkPreselectionState = coffeeMaker.milkPreselectionState,
		.isMakingCoffee = coffeeMaker.ongoingCoffeeMaking ? TRUE : FALSE
	};

	return coffeeMakerViewModel;
}

/**
 * @copydoc getProductViewModel
 */
ProductViewModel getProductViewModel(unsigned int productIndex) {
	 Product *product = getProduct(productIndex);
	if (product) {
		// Map to view model
		ProductViewModel productViewModel = {
			//.name = product->name
		};
		strcpy(productViewModel.name, product->name);

		return productViewModel;
	}

	return UNDEFINED_PRODUCT;
}

/**
 * @copydoc getCoffeeMakingProcessInstanceViewModel
 */
MakeCoffeeProcessInstanceViewModel getCoffeeMakingProcessInstanceViewModel() {
	if (coffeeMaker.ongoingCoffeeMaking) {
		MakeCoffeeProcessInstanceViewModel coffeeMakingViewModel = {
			.productIndex =  getProductIndex(coffeeMaker.ongoingCoffeeMaking->product),
			.withMilk = coffeeMaker.ongoingCoffeeMaking->withMilk,
			.currentActivity = coffeeMaker.ongoingCoffeeMaking->currentActivity
		};

		return coffeeMakingViewModel;
	}

	return INEXISTENT_COFFEE_MAKING_PROCESS_INSTANCE;
}

// =============================================================================
// Operations presentation interface
// =============================================================================

static void processEvent(CoffeeMakerEvent event);

/**
 * @copydoc switchOn
 */
void switchOn() {
	processEvent(event_switchedOn);
}

/**
 * @copydoc switchOff
 */
void switchOff() {
	processEvent(event_switchedOff);
}

/**
 * @copydoc setMilkPreselection
 */
void setMilkPreselection(MilkPreselectionState state) {
	coffeeMaker.milkPreselectionState = state;

	notifyObservers();
}

/**
 * @copydoc startMakingCoffee
 */
void startMakingCoffee(unsigned int productIndex) {
	selectedProductIndex = productIndex;

	processEvent(event_productSelected);
}

/**
 * @copydoc abortMakingCoffee
 */
void abortMakingCoffee() {
	processEvent(event_productionProcessAborted);
}

/**
 * Processes an event by delegating it to the main state machine.
 */
static void processEvent(CoffeeMakerEvent event) {
	processStateMachineEvent(&stateMachine, event);
}
