/**
 * State machine engine
 *
 * Contains the state machine engine.
 *
 * @file    stateMachineEngine.h
 * @version 0.1
 * @author  Ronny Stauffer (staur3@bfh.ch)
 * @date    Jun 4, 2011
 */

#ifndef STATEMACHINEENGINE_H_
#define STATEMACHINEENGINE_H_

#define NO_EVENT 999

typedef int Event;

/**
 * Defines the signature of a state precondition predicate.
 */
typedef int (*StatePrecondition)();
/**
 * Defines the signature of a state action.
 */
typedef void (*StateAction)();
/**
 * Defines the signature of a 'do' state action.
 */
typedef Event (*DoStateAction)();

/**
 * Represents a state.
 */
typedef struct {
	/**
	 * The state's index.
	 */
	int stateIndex;
	/**
	 * The state's precondition predicate determines if a state can be activated or not.
	 * If all preconditions to activate the state are met the predicate should return TRUE, otherwise FALSE.
	 */
	StatePrecondition precondition;
	/**
	 * The state's 'entry' action is called once after the state was activated.
	 */
	StateAction entryAction;
	/**
	 * The state's 'do' action is constantly called while the state is active.
	 */
	DoStateAction doAction;
	/**
	 * The state's 'exit' action is called once before the state will be deactivated.
	 */
	StateAction exitAction;
} State;

/**
 * Represents a state machine definition.
 */
typedef struct {
	int isInitialized;
	unsigned int numberOfEvents;
	State *initialState;
	State *activeState;
	State *transitions[];
} StateMachine;

/**
 * Sets up and starts a new state machine.
 *
 * @param stateMachine A state machine definition.
 */
extern void setUpStateMachine(StateMachine *stateMachine);

/**
 * Heartbeat function for ongoing tasks.
 * Should be constantly called by the client.
 *
 * @param stateMachine A state machine definition.
 */
extern void runStateMachine(StateMachine *stateMachine);

/**
 * Aborts a running state machine.
 *
 * @param stateMachine A state machine definition.
 */
extern void abortStateMachine(StateMachine *stateMachine);

/**
 * Signals an event to a state machine.
 *
 * @param stateMachine A state machine definition.
 * @param event An event.
 */
extern void processStateMachineEvent(StateMachine *stateMachine, Event event);

#endif /* STATEMACHINEENGINE_H_ */
