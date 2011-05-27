/**
 * Sensor controlling
 *
 * Initialize and control the sensors.
 *
 * @file    sensorController.h
 * @version 0.1
 * @author  Elmar Vonlanthen (vonle1@bfh.ch)
 * @date    May 20, 2011
 */

#ifndef SENSORCONTROLLER_H_
#define SENSORCONTROLLER_H_

#define SENSOR_1	1
#define SENSOR_2	2

/**
 * Predefined sensor states
 */
enum SensorState {
  sensor_normal = 0,
  sensor_alert,
  sensor_unknown
};

extern int setUpSensorController(void);
extern int tearDownSensorController(void);
extern enum SensorState getSensorState(int id);

#endif /* SENSORCONTROLLER_H_ */
