/**
 * @file indoorSensor.h
 * @brief Indoor sensor functions
 * @details This is the header file for the indoor sensor.
 * @author Karl Berger
 * @date 2025-05-15
 */

#ifndef INDOOR_SENSOR_H
#define INDOOR_SENSOR_H

typedef struct // for indoor sensor
{
	float tempC;
	float humid;
} SensorTH;

// Global sensor variables
extern bool indoorSensor; // true if sensor exists
extern SensorTH indoor;	  // Indoor sensor readings

// Function declarations
void initSensor(); // Detect & initialize indoor sensor
void readSensor(); // Read indoor sensor

#endif // INDOOR_SENSOR_H
// End of file
