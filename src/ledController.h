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
 * Define LED symbols
 */
#ifdef CARME
 #define	LED_1			(1 << 0)
 #define	LED_2			(1 << 1)
 #define	LED_3			(1 << 2)
 #define	LED_4			(1 << 3)
 #define	LED_5			(1 << 4)
 #define	LED_6			(1 << 5)
 #define	LED_7			(1 << 6)
 #define	LED_8			(1 << 7)
 #define    NUM_OF_LEDS     8
#elif defined(ORCHID)
 #define	LED_1			(1 << 0)
 #define	LED_2			(1 << 1)
 #define	LED_3			(1 << 2)
 #define	LED_4			(1 << 3)
 #define	LED_5			(1 << 4)
 #define	LED_6			(1 << 5)
 #define	LED_7			(1 << 6)
 #define	LED_8			(1 << 7)
 #define    NUM_OF_LEDS     8
#endif
/* Get LED ID (e.g.: LED_ID(4) == LED_4) */
#define		LED_ID(x) (1 << ((x)-1))


/**
 * Predefined LED states
 */
enum LedState {
  led_off = 0, /**< led_off      */
  led_on,      /**< led_on       */
  led_blinking /**< led_blinking */
};

/**
 * Initialize LED controller
 *
 * @return Returns TRUE if initializing was successful
 */
extern int setUpLedController(void);

/**
 * Clean up LED controller
 *
 * @return Returns TRUE if cleaning up was successful
 */
extern int tearDownLedController(void);

/**
 * Update all LEDs according to the state in the array 'leds'.
 *
 * @return Returns TRUE if LED updating was successful
 */
extern int updateAllLeds();

/**
 * Update LED state
 *
 * Update LED state for one LED in array 'leds' and update all leds with
 * function updateAllLeds().
 *
 * @param id LED identifier
 * @param state LED state (on, off or blinking)
 * @return Returns TRUE if LED updating was successful
 */
extern int updateLed(int id, enum LedState state);

/**
 * Set LED blinking interval
 *
 * Set the LED blinking interval values in milliseconds
 *
 * @param id LED indentifier
 * @param durationOn Duration while LED is on
 * @param durationOff Duration while LED is off
 */
extern int setBlinkingFreq(int id, int durationOn, int durationOff);

#endif /* LEDCONTROLLER_H_ */
