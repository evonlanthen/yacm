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
#include "timer.h"
#include "sensorController.h"
#include "machineController.h"

// Durations
#define INITIALIZING_DURATION 2000
#define WARMING_UP_DURATION 1000
#define DELIVERING_MILK_DURATION 2000
#define DELIVERING_COFFEE_DURATION 5000

static int isBusinessLogicSetUp = FALSE;

// Business model
/**
 * Represents the coffee ingredient.
 */
typedef struct {
	int isAvailable;
	int emptyTankSensorId;
} Coffee;

/**
 * Represents the milk ingredient.
 */
typedef struct {
	int isAvailable;
	int emptyTankSensorId;
} Milk;

/**
 * Represents a product definition.
 */
typedef struct {
	char name[256];
} Product;

typedef struct ProductListElement {
	Product *product;
	struct ProductListElement *next;
} ProductListElement;

/**
 * Represents an ongoing coffee making process instance.
 *
 */
typedef struct {
	Product *product;
	int withMilk;
	CoffeeMakingActivity currentActivity;
} MakeCoffeeProcessInstance;

/**
 * Represents the coffee maker.
 */
typedef struct {
	CoffeeMakerState state;
	Coffee coffee;
	Milk milk;
	ProductListElement *products;
	MilkPreselectionState milkPreselectionState;
	MakeCoffeeProcessInstance *ongoingCoffeeMaking;
} CoffeeMaker;

// Model observers
static NotifyModelChanged observer;
static void notifyObservers();

// Memory management
static void * newObject(void *initializer, size_t size);
static void deleteObject(void *object);

// Product list helpers
static unsigned int getNumberOfProducts();
static Product * getProduct(unsigned int productIndex);

// Model initialization
static void setUpProducts();

// State machine
// Events
typedef enum {
	event_switchedOn,
	event_switchedOff,
	event_isInitialized,
	event_productSelected,
	event_productionProcessAborted,
	event_productionProcessIsFinished,
	event_ingredientTankIsEmpty,
	event_none
} Event;

typedef enum {
	coffeeMakingEvent_isWarmedUp,
	coffeeMakingEvent_deliverMilk,
	coffeeMakingEvent_milkDelivered,
	coffeeMakingEvent_deliverCoffee,
	coffeeMakingEvent_coffeeDelivered,
	coffeeMakingEvent_none
} CoffeeMakingEvent;

static unsigned int selectedProductIndex;

static void processEvent(Event event);

static void checkIngredientTankSensors();

// Timers
static TIMER initTimer;
static TIMER warmingUpTimer;

/**
 * Notify the model observers of a model change.
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

static enum SensorState lastEmptyCoffeeTankSensorState = sensor_unknown;
static enum SensorState lastEmptyMilkTankSensorState = sensor_unknown;

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









// State machine engine

typedef int (*StatePrecondition)();
typedef void (*StateAction)();
typedef Event (*DoStateAction)();

typedef struct {
	int stateIndex;
	StatePrecondition precondition;
	StateAction entryAction;
	DoStateAction doAction;
	StateAction exitAction;
} State;

typedef struct {
	int isInitialized;
	unsigned int numberOfEvents;
	State *initialState;
	State *activeState;
	State *transitions[];
} StateMachine;

static void setUpStateMachine(StateMachine *stateMachine);
static void runStateMachine(StateMachine *stateMachine);
static void abortStateMachine(StateMachine *stateMachine);
static Event runState(State *state);
static void processEventInt(StateMachine *stateMachine, Event event);
static void activateState(StateMachine *stateMachine, State *nextState);

static void setUpStateMachine(StateMachine *stateMachine) {
	if (stateMachine->isInitialized) {
		return;
	}

	activateState(stateMachine, stateMachine->initialState);

	stateMachine->isInitialized = TRUE;
}

static void runStateMachine(StateMachine *stateMachine) {
	if (!stateMachine->isInitialized) {
		return;
	}

	// Run active state and process events
	Event event = runState(stateMachine->activeState);
	if (event != event_none) {
		processEventInt(stateMachine, event);
	}
}

static void abortStateMachine(StateMachine *stateMachine) {
	if (!stateMachine->isInitialized) {
		return;
	}

	if (stateMachine->activeState->exitAction) {
		stateMachine->activeState->exitAction();
	}

	stateMachine->isInitialized = FALSE;
}

static Event runState(State *state) {
	Event event = event_none;

	// If the state has a 'do' action, then run it
	if (state->doAction) {
		event = state->doAction();
	}

	return event;
}

static void processEventInt(StateMachine *stateMachine, Event event) {
	if (!stateMachine->isInitialized) {
		return;
	}

	// Processing an event means looking up the state machine's next state in the transition table
	State *nextState = stateMachine->transitions[stateMachine->activeState->stateIndex * stateMachine->numberOfEvents + event];
	if (nextState) {
		// If next state either has no precondition
		// or the precondition is true...
		if (!nextState->precondition
			|| nextState->precondition()) {
			// Activate next state
			activateState(stateMachine, nextState);
		}
	}
}

static void activateState(StateMachine *stateMachine, State *nextState) {
	// If a state is currently active and the state has an exit action,
	// then run the state's exit action
	if (stateMachine->activeState) {
		if (stateMachine->activeState->exitAction) {
			stateMachine->activeState->exitAction();
		}
	}
	// Make the next state the currently active state
	stateMachine->activeState = nextState;
	// If the (now currently active) state has an entry action,
	// run the state's entry action
	if (stateMachine->activeState->entryAction) {
		stateMachine->activeState->entryAction();
	}
}



















void registerModelObserver(NotifyModelChanged pObserver) {
	observer = pObserver;
}



// View Model

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



// Presentation interface

void switchOn() {
	processEvent(event_switchedOn);
}

void switchOff() {
	processEvent(event_switchedOff);
}

void setMilkPreselection(MilkPreselectionState state) {
	coffeeMaker.milkPreselectionState = state;

	notifyObservers();
}

void startMakingCoffee(unsigned int productIndex) {
	selectedProductIndex = productIndex;

	processEvent(event_productSelected);
}

void abortMakingCoffee() {
	processEvent(event_productionProcessAborted);
}















// State machines

// Main state machine
static StateMachine coffeeMakingProcessMachine;

// Off state
static void offStateEntryAction() {
	coffeeMaker.state = coffeeMaker_off;

	notifyObservers();
}

static State offState = {
	.stateIndex = coffeeMaker_off,
	.entryAction = offStateEntryAction
};

// Initializing state
static void initializingStateEntryAction() {
	coffeeMaker.state = coffeeMaker_initializing;

	initTimer = setUpTimer(INITIALIZING_DURATION);

	notifyObservers();
}

static Event initializingStateDoAction() {
	if (isTimerElapsed(initTimer)) {
		return event_isInitialized;
	}

	return event_none;
}

static State initializingState = {
	.stateIndex = coffeeMaker_initializing,
	.entryAction = initializingStateEntryAction,
	.doAction = initializingStateDoAction
};

// Idle state
static void idleStateEntryAction() {
	coffeeMaker.state = coffeeMaker_idle;

	notifyObservers();
}

static State idleState = {
	.stateIndex = coffeeMaker_idle,
	.entryAction = idleStateEntryAction
};

// Producing state
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
	if (coffeeMaker.ongoingCoffeeMaking->currentActivity == coffeeMakingActivity_finished) {
		return event_productionProcessIsFinished;
	}

	return event_none;
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

static StateMachine stateMachine = {
	.numberOfEvents = 7,
	.initialState = &offState,
//static State * stateTransitions[][event_none] = {
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

// Make coffee process

// Warming Up activity
static void warmingUpActivityEntryAction() {
	coffeeMaker.ongoingCoffeeMaking->currentActivity = coffeeMakingActivity_warmingUp;

	warmingUpTimer = setUpTimer(WARMING_UP_DURATION);

	notifyObservers();
}

static Event warmingUpActivityDoAction() {
	if (isTimerElapsed(warmingUpTimer)) {
		return coffeeMakingEvent_isWarmedUp;
	}

	return coffeeMakingEvent_none;
}

static State warmingUpActivity = {
	.stateIndex = coffeeMakingActivity_warmingUp,
	.entryAction = warmingUpActivityEntryAction,
	.doAction = warmingUpActivityDoAction
};

// With Milk gateway
static Event withMilkGatewayDoAction() {
	if (coffeeMaker.ongoingCoffeeMaking->withMilk) {
		return coffeeMakingEvent_deliverMilk;
	} else {
		return coffeeMakingEvent_deliverCoffee;
	}

	return coffeeMakingEvent_none;
}

static State withMilkGateway = {
	.stateIndex = coffeeMakingActivity_withMilkGateway,
	.doAction = withMilkGatewayDoAction
};

// Delivering Milk activity
static void deliveringMilkActivityEntryAction() {
	coffeeMaker.ongoingCoffeeMaking->currentActivity = coffeeMakingActivity_deliveringMilk;

	startMachine(ingredient_milk, DELIVERING_MILK_DURATION);

	notifyObservers();
}

static Event deliveringMilkActivityDoAction() {
	if (!machineRunning()) {
		return coffeeMakingEvent_milkDelivered;
	}

	return coffeeMakingEvent_none;
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

// Delivering Coffe activity
static void deliveringCoffeeActivityEntryAction() {
	coffeeMaker.ongoingCoffeeMaking->currentActivity = coffeeMakingActivity_deliveringCoffee;

	startMachine(ingredient_coffee, DELIVERING_COFFEE_DURATION);

	notifyObservers();
}

static Event deliveringCoffeeActivityDoAction() {
	if (!machineRunning()) {
		return coffeeMakingEvent_coffeeDelivered;
	}

	return coffeeMakingEvent_none;
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

// Finishing activity
static void finishingActivityEntryAction() {
	coffeeMaker.ongoingCoffeeMaking->currentActivity = coffeeMakingActivity_finished;

	notifyObservers();
}

static State finishingActivity = {
	.stateIndex = coffeeMakingActivity_finished,
	.entryAction = finishingActivityEntryAction
};

static StateMachine coffeeMakingProcessMachine = {
	.numberOfEvents = 5,
	.initialState = &warmingUpActivity,
//static State * coffeeMakingActivityTransitions[][coffeeMakingEvent_none] = {
	.transitions = {
		/* coffeeMakingActivity_warmingUp: */
			/* coffeeMakingEvent_isWarmedUp: */ &withMilkGateway,
			/* coffeeMakingEvent_deliverMilk: */ NULL,
			/* coffeeMakingEvent_milkDelivered: */ NULL,
			/* coffeeMakingEvent_deliverCoffee: */ NULL,
			/* coffeeMakingEvent_coffeeDelivered: */ NULL,
		/* coffeeMakingActivity_withMilkGateway: */
			/* coffeeMakingEvent_isWarmedUp: */ NULL,
			/* coffeeMakingEvent_deliverMilk: */ &deliveringMilkActivity,
			/* coffeeMakingEvent_milkDelivered: */ NULL,
			/* coffeeMakingEvent_deliverCoffee: */ &deliveringCoffeeActivity,
			/* coffeeMakingEvent_coffeeDelivered: */ NULL,
		/* coffeeMakingActivity_deliveringMilk: */
			/* coffeeMakingEvent_isWarmedUp: */ NULL,
			/* coffeeMakingEvent_deliverMilk: */ NULL,
			/* coffeeMakingEvent_milkDelivered: */ &deliveringCoffeeActivity,
			/* coffeeMakingEvent_deliverCoffee: */ NULL,
			/* coffeeMakingEvent_coffeeDelivered: */ NULL,
		/* coffeeMakingActivity_deliveringCoffee: */
			/* coffeeMakingEvent_isWarmedUp: */ NULL,
			/* coffeeMakingEvent_deliverMilk: */ NULL,
			/* coffeeMakingEvent_milkDelivered: */ NULL,
			/* coffeeMakingEvent_deliverCoffee: */ NULL,
			/* coffeeMakingEvent_coffeeDelivered: */ &finishingActivity
		}
};


// Logic


int setUpBusinessLogic() {
	// Check if business logic is already set up
	if (isBusinessLogicSetUp) {
		return FALSE;
	}

#ifdef DEBUG

#endif

	// Load product definitions
	setUpProducts();

	setUpStateMachine(&stateMachine);

	isBusinessLogicSetUp = TRUE;

	return TRUE;
}

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

void runBusinessLogic() {
	//if (!stateMachine.isInitialized) {
	//	setUpStateMachine(&stateMachine);
	//}

	// Check ingredient tank sensors
	checkIngredientTankSensors();

	// Run state machine
	runStateMachine(&stateMachine);
}

static void processEvent(Event event) {
	processEventInt(&stateMachine, event);
}

static void checkIngredientTankSensors() {
	// Coffee sensor
	enum SensorState emptyCoffeeTankSensorState = getSensorState(coffeeMaker.coffee.emptyTankSensorId);
	// If sensor state has changed...
	if (emptyCoffeeTankSensorState != lastEmptyCoffeeTankSensorState) {
		// Update model
		coffeeMaker.coffee.isAvailable = !(emptyCoffeeTankSensorState == sensor_alert);

		notifyObservers();

		// Fire event
		if (emptyCoffeeTankSensorState == sensor_alert) {
			processEvent(event_ingredientTankIsEmpty);
		}

		lastEmptyCoffeeTankSensorState = emptyCoffeeTankSensorState;
	}

	// Milk sensor
	enum SensorState emptyMilkTankSensorState = getSensorState(coffeeMaker.milk.emptyTankSensorId);
	// If sensor state has changed...
	if (emptyMilkTankSensorState != lastEmptyMilkTankSensorState) {
		// Update model
		coffeeMaker.milk.isAvailable = !(emptyMilkTankSensorState == sensor_alert);

		notifyObservers();

		// Fire event
		if (emptyMilkTankSensorState == sensor_alert) {
			processEvent(event_ingredientTankIsEmpty);
		}

		lastEmptyMilkTankSensorState = emptyMilkTankSensorState;
	}
}
