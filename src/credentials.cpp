/**
 * @file credentials.cpp
 * @author Karl Berger
 * @date 2025-06-04
 * @brief Wi-Fi, Weather Underground, APRS, and ThingSpeak credentials
 */

#include "credentials.h"

#include <Arduino.h> // for String

const String FW_VERSION = "250529"; // Firmware version

// Wi-Fi Credentials
//! Place values in quotes " "
const String WIFI_SSID = "DCMNET";                   // your Wi-Fi SSID
const String WIFI_PASSWORD = "0F1A2D3E4D5G6L7O8R9Y"; // your Wi-Fi password

// Weather Underground credentials
//! Place all values in quotes " "
const String WX_STATION_ID = "KVACENTR126";               // your Weather Underground Personal Weather Station (PWS) id
const String WX_KEY = "c41eb27afef64b6b9eb27afef62b6bed"; // your Weather Underground API key
const String MY_TIMEZONE = "America/New_York";            // Olson timezone https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
/*
Common Olson Timezones:
EST  America/New_York
CST  America/Chicago
MST  America/Denver
PST  America/Los_Angeles
AKST America/Juneau
HST  Pacific/Honolulu
NST  America/St_Johns
AST  America/Halifax
EST  America/Toronto
CST  America/Winnipeg
MST  America/Edmonton
PST  America/Vancouver
GMT  Europe/London
CET  Europe/Berlin
WAT  Africa/Lagos
JST  Asia/Tokyo
KST  Asia/Seoul
CST  Asia/Shanghai
IST  Asia/Kolkata
*/

// APRS credentials
//! Place all values in quotes " "
const String CALLSIGN = "W4KRL-13";  // call-SSID
const String APRS_PASSCODE = "9092"; // https://aprs.do3sww.de/
const String APHORISM_FILE = "/aphorisms.txt";

// ThingSpeak Credentials
//! Place all values in quotes " "
const String TS_WRITE_KEY = "KKJB0YPV19V9KMX0";
const String TS_CHANNEL = "88666";
/* ThingSpeak Field Reference:
   Read Key  CRYD7ZD9AY8HHOXP
   Field 1 TempC
   Field 2 Humid
   Field 3 Pressure
   Filed 4 WindSpeed
   Field 5 WindDir
   Field 6 WindGust
   Field 7 RailToday
   Field 8 RainRate
   Status
*/

// Weather update intervals (note minutes)
//! Use unsigned integer values. No quote marks
const unsigned int WX_CURRENT_INTERVAL = 7;   // minutes between current weather requests (Should be >= 1)
const unsigned int TS_POST_INTERVAL = 7;      // minutes between posting to ThingSpeak same as WX_CURRENT_INTERVAL
const unsigned int WX_APRS_INTERVAL = 10;     // minutes between posting weather data to APRS (Must be >= 5)
const unsigned int WX_FORECAST_INTERVAL = 13; // minutes between forecast requests
const unsigned int SCREEN_DURATION = 5;       // display frame interval in !!!seconds!!!

// Display selections
//! Use true/false values. No quote marks
// If both clocks are false, no clock is displayed
// If both clocks are true, digital clock is displayed
const bool DIGITAL_CLOCK = false;  // display digital clock (true/false)
const bool ANALOG_CLOCK = true;    // display analog clock (true/false)
const bool METRIC_DISPLAY = false; // weather display units

// End of file
