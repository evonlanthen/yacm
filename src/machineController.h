/**
 * Machine controlling
 *
 * Initialize and control the coffee machine.
 *
 * @file    machineController.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 20, 2011
 */

#ifndef MACHINECONTROLLER_H_
#define MACHINECONTROLLER_H_


enum Ingredient {
	ingredient_coffee = 0, /**< ingredient_coffee */
	ingredient_milk        /**< ingredient_milk   */
};

/**
 * Initializes the machine controller
 *
 * @return Returns TRUE if initialization was successful
 */
extern int setUpMachineController(void);

/**
 * Tears down machine controller
 *
 * @return Returns TRUE if tearing down was successful
 */
extern int tearDownMachineController(void);

/**
 * Starting process of putting out ingredients
 *
 * @param ing Ingretient type (milk or coffee)
 * @param time Output time in milliseconds
 * @return Returns TRUE if start was successful
 */
extern int startMachine(enum Ingredient ing, unsigned int time);

/**
 * Stopping output process immediately
 *
 * @return Returns TRUE if stop was successful
 */
extern int stopMachine(void);

/**
 * Check if machine is still running
 *
 * @return Returns TRUE if machine is running
 */
extern int machineRunning(void);

#endif /* MACHINECONTROLLER_H_ */
