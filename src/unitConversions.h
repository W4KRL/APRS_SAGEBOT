/**
 * @file unitConversions.h
 * @author Karl W. Berger
 * @date 2025-06-02
 * @brief Unit conversion header for functions
 * @details This is the header file for unit conversion functions.
 */

#ifndef UNIT_CONVERSION_H
#define UNIT_CONVERSION_H

#include <Arduino.h> // [builtin] PlatformIO

float CtoF(float tempC);     ///< convert Celsius to Fahrenheit
float MStoMPH(float ms);     ///< convert meters per second to miles per hour
float MMtoIN(float mm);      ///< convert millimeters to inches
float CMtoIN(float cm);      ///< convert centimeters to inches
float KMtoMILES(float km);   ///< convert kilometers to miles or km/h to mph
float DEGtoRAD(float deg);   ///< convert degrees to radians
float HPAtoINHG(float hpa);  ///< convert hectoPascal (millibar) to inches of mercury
float MtoFT(float meters);   ///< convert meters to feet
float KMHtoKNOTS(float kmh); ///< convert kilometers per hour to knots
String getCompassDirection(int degrees) ; ///< convert degrees to compass direction
String getRainIntensity(float rate); ///< convert rain rate to intensity description

#endif // UNIT_CONVERSION_H
// End of file
