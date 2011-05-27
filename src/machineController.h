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

extern int setUpMachineController(void);
extern int tearDownMachineController(void);
extern int startMachine(void);
extern int stopMachine(void);
extern int machineRunning(void);

// Set coffee making time to 5 seconds:
#define COFFEE_MAKING_TIME 5000

#endif /* MACHINECONTROLLER_H_ */
