/**
 * @file credentials.h
 * @author Karl Berger
 * @date 2025-05-29
 * @brief Wi-Fi, Weather Underground, APRS, and ThingSpeak credentials
 */

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <Arduino.h> // for String

extern const String FW_VERSION; // Firmware version

// Wi-Fi Credentials
extern const String WIFI_SSID;     // your Wi-Fi SSID
extern const String WIFI_PASSWORD; // your Wi-Fi password

extern const String MY_TIMEZONE;   // Olson timezone https://en.wikipedia.org/wiki/List_of_tz_database_time_zones

// APRS credentials
extern const String CALLSIGN;      // call-SSID
extern const String APRS_PASSCODE; // https://aprs.do3sww.de/
extern const String APHORISM_FILE;
extern const String APRS_SOFTWARE_NAME;
extern const String APRS_FILTER; // default value - Change to "b-your call-*"

#endif // CREDENTIALS_H
// End of file
