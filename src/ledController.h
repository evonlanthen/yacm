/**
 * LED controlling
 *
 * Initialize and control the LEDs.
 *
 * @file    ledController.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 20, 2011
 */

#ifndef LEDCONTROLLER_H_
#define LEDCONTROLLER_H_

/**
 * Predefined LED states
 */
enum LedStates {
  led_off = 0,
  led_on,
  led_blinking
};

/**
 * Initialize LEDs
 *
 * @return Returns TRUE if initializing was successful
 */
extern int initLeds(void);

/**
 * Update LED state
 *
 * Update LED according to the overall (on/off) or blinking state.
 *
 * @param id LED identifier
 * @param state LED state (on, off or blinking)
 * @return Returns TRUE if LED updating was successful
 */
extern int updateLed(int id, int state);

/**
 * Set LED blinking interval
 *
 * Set the LED blinking interval values in milliseconds
 *
 * @param id LED indentifier
 * @param intervalOn Duration while LED is on
 * @param intervalOff Duration while LED is off
 */
extern int setBlinkingFreq(int id, int intervalOn, int intervalOff);

#endif /* LEDCONTROLLER_H_ */
