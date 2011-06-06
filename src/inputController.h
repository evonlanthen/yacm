/**
 * @brief   Initialize and read buttons and switches
 * @file    inputController.h
 * @version 1.0
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
  #define BUTTON_1		 99
  #define BUTTON_2		101
  #define BUTTON_3		102
  #define BUTTON_4		103
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
  switch_off = 0, /**< switch_off     */
  switch_on,      /**< switch_on      */
  switch_unknown  /**< switch_unknown */
};

/**
 * Predefined button states
 */
enum ButtonState {
  button_off = 0, /**< button_off     */
  button_on,      /**< button_on      */
  button_unknown  /**< button_unknown */
};

/**
 * Initializes the input controller
 *
 * @return Returns TRUE if initialization was successful
 */
extern int setUpInputController(void);

/**
 * Tears down input controller
 *
 * @return Returns TRUE if tearing down was successful
 */
extern int tearDownInputController(void);

/**
 * Checks a switch of its state
 *
 * @param id Id of switch to check
 * @return State of switch (on, off or unknown)
 */
extern enum SwitchState getSwitchState(int id);

/**
 * Checks a button of its state
 *
 * @param id Id of button to check
 * @return State of button (on, off or unknown)
 */
extern enum ButtonState getButtonState(int id);

#endif /* INPUTCONTROLLER_H_ */
