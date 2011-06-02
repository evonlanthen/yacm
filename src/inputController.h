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
 /* Define switches */
 #define SWITCH_1		(1 << 0)
 #define SWITCH_2		(1 << 1)
 #define SWITCH_3		(1 << 2)
 #define SWITCH_4		(1 << 3)
 #define SWITCH_5		(1 << 4)
 #define SWITCH_6		(1 << 5)
 #define SWITCH_7		(1 << 6)
 #define SWITCH_8		(1 << 7)
 /* Define buttons */
 #define BUTTON_1		(1 << 0)
 #define BUTTON_2		(1 << 1)
 #define BUTTON_3		(1 << 2)
 #define BUTTON_4		(1 << 3)
#elif defined(ORCHID)
 /* Define switches */
 #define SWITCH_1		(1 << 0)
 #define SWITCH_2		(1 << 1)
 #define SWITCH_3		(1 << 2)
 #define SWITCH_4		(1 << 3)
 /* Define buttons */
 #define BUTTON_1		(1 << 0)
 #define BUTTON_2		(1 << 1)
 #define BUTTON_3		(1 << 2)
 #define BUTTON_4		(1 << 3)
#endif

/**
 * Predefined switch states
 */
enum SwitchState {
  switch_off = 0,
  switch_on,
  switch_unknown
};

/**
 * Predefined button states
 */
enum ButtonState {
  button_off = 0,
  button_on,
  button_unknown
};

extern int setUpInputController(void);
extern int tearDownInputController(void);
extern enum SwitchState getSwitchState(int id);
extern enum ButtonState getButtonState(int id);

#endif /* INPUTCONTROLLER_H_ */
