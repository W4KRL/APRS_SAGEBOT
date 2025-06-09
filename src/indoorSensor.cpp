/**
 * @file indoorSensor.cpp
 * @brief Indoor sensor functions
 * @details This file contains the functions to initialize and read the indoor sensor.
 * @author Karl Berger
 * @date 2025-05-15
 */

#include "indoorSensor.h"

#include <Arduino.h>        // PlatformIO
#include <Wire.h>           // I2C library
#include <Adafruit_AHTX0.h> // Adafruit sensor library

Adafruit_AHTX0 aht; // Create sensor object

// Define global variables
bool indoorSensor = false;    // Default to false = no sesnor found
SensorTH indoor = {0.0, 0.0}; // initialize global sensor variable

void initSensor()
{
  Wire.begin(SDA, SCL); // Define I2C pins
  // Detect sensor and initialize
  indoorSensor = aht.begin(); // True if AHT10 sensor is found
}

void readSensor()
{
  if (indoorSensor)
  {
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    indoor.tempC = temp.temperature;
    indoor.humid = humidity.relative_humidity;
  }
}