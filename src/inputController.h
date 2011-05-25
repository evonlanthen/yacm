/**
 * Input controlling
 *
 * Initialize and control buttons and switches.
 *
 * @file    inputController.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 20, 2011
 */

#ifndef INPUTCONTROLLER_H_
#define INPUTCONTROLLER_H_

#ifdef CARME
 /* Define swiches */
 #define SWITCH_1		(1 << 1)
 #define SWITCH_2		(1 << 2)
 #define SWITCH_3		(1 << 3)
 #define SWITCH_4		(1 << 4)
 /* Define buttons */
 #define BUTTON_1		(1 << 1)
 #define BUTTON_2		(1 << 2)
 #define BUTTON_3		(1 << 3)
 #define BUTTON_4		(1 << 4)
#elif defined(ORCHID)
 #define SWITCH_1		(1 << 1)
 #define SWITCH_2		(1 << 2)
 #define SWITCH_3		(1 << 3)
 #define SWITCH_4		(1 << 4)
 /* Define buttons */
 #define BUTTON_1		(1 << 1)
 #define BUTTON_2		(1 << 2)
 #define BUTTON_3		(1 << 3)
 #define BUTTON_4		(1 << 4)
#endif

/**
 * Predefined switch states
 */
enum SwitchStates {
  switch_off = 0,
  switch_on
};

/**
 * Predefined button states
 */
enum ButtonStates {
  button_off = 0,
  button_on
};

extern int initInputController(void);
extern int cleanUpInputController(void);
extern int getSwitchState(int num);
extern int getButtonState(int num);

#endif /* INPUTCONTROLLER_H_ */
