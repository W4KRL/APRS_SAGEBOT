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

// Weather Underground credentials
extern const String WX_STATION_ID; // your Weather Underground Personal Weather Station (PWS) id
extern const String WX_KEY;        // your Weather Underground API key
extern const String MY_TIMEZONE;   // Olson timezone https://en.wikipedia.org/wiki/List_of_tz_database_time_zones

// APRS credentials
extern const String CALLSIGN;      // call-SSID
extern const String APRS_PASSCODE; // https://aprs.do3sww.de/
extern const String APHORISM_FILE;

// ThingSpeak Credentials
extern const String TS_WRITE_KEY;
extern const String TS_CHANNEL;

// Weather update intervals (note minutes)
extern const unsigned int WX_CURRENT_INTERVAL;  // minutes between current weather requests (Should be >= 1)
extern const unsigned int WX_FORECAST_INTERVAL; // minutes between forecast requests
extern const unsigned int TS_POST_INTERVAL; // minutes between posting to ThingSpeak
extern const unsigned int WX_APRS_INTERVAL;     // minutes between posting weather data to APRS (Must be >= 5)
extern const unsigned int SCREEN_DURATION;      // display frame interval in !!!seconds!!!

// Display selections
extern const bool DIGITAL_CLOCK;  // display digital clock (true/false)
extern const bool ANALOG_CLOCK;   // display analog clock (true/false)
extern const bool METRIC_DISPLAY; // weather display units

#endif // CREDENTIALS_H
// End of file
