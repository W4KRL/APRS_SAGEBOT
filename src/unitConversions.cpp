/**
 * @file unitConversions.cpp
 * @author Karl W. Berger
 * @date 2025-06-02
 * @brief Unit conversion functions.
 * @details This file contains the functions for unit conversions.
 * @note This file must reside in the same Arduino directory as the main sketch.
 */

#include "unitConversions.h" // unit conversion functions

#include <Arduino.h> // for Arduino functions

float CtoF(float tempC) // convert celsius to fahrenheit
{
	return 1.8 * tempC + 32.0;
} // CtoF()

float MStoMPH(float ms) // convert meters per second to miles per hour
{
	return 2.23694 * ms;
} // MStoMPH()

float MMtoIN(float mm) // convert millimeters to inches
{
	return mm * 0.0393701;
} // MMtoIN()

float CMtoIN(float cm)
{ // convert centimeters to inches
	return 0.39370 * cm;
} // CMtoIN()

float KMtoMILES(float km) // convert kilometers to miles or km/h to mph
{
	return km * 0.621371;
} // KMtoMILES()

float DEGtoRAD(float deg) // convert degrees to radians
{
	return 0.0174532925 * deg;
} // DEGtoRAD()

float HPAtoINHG(float hpa) // convert hectoPascal (millibar) to inches of mercury
{
	return 0.0295301 * hpa;
} // HPAtoINHG()

float MtoFT(float meters)
{ // convert meters to feet
	return 3.2808 * meters;
} // MtoFT()

float KMHtoKNOTS(float kmh)
{
	return 0.5399668 * kmh;
} // KMHtoKNOTS()

/*
*******************************************************
************** getCompassDirection ********************
*******************************************************
*/
String getCompassDirection(int degrees)
{
  // translates compass degrees to 16 points with 1 to 3 letter abbreviations
  String compassPoints[] = {
      "N", "NNE", "NE", "ENE",
      "E", "ESE", "SE", "SSE",
      "S", "SSW", "SW", "WSW",
      "W", "WNW", "NW", "NNW"};

  int index = static_cast<int>((degrees + 11.25) / 22.5); // Add 11.25 for rounding to the nearest compass point
  index = index % 16;
  return compassPoints[index];
} // getCompassDirection()

/*
*******************************************************
***************** getRainIntensity ********************
*******************************************************
*/
String getRainIntensity(float rate)
{
	// translates rainfall rate in mm/h to meteorlogical name
	// https://en.wikipedia.org/wiki/Rain
	// https://water.usgs.gov/edu/activity-howmuchrain-metric.html

	String intensity = "";
	if (rate >= 0.0 && rate < 0.0001)
	{
		intensity = "Rate Nil";
	}
	else if (rate >= 0.0001 && rate < 2.5)
	{
		intensity = "Light";
	}
	else if (rate >= 2.5 && rate < 7.6)
	{
		intensity = "Moderate";
	}
	else if (rate >= 7.6 && rate < 50)
	{
		intensity = "Heavy";
	}
	else
	{
		intensity = "Violent";
	}
	return intensity;
} // getRainIntensity()