/**
 * @file credentials.cpp
 * @author Karl Berger
 * @date 2025-06-04
 * @brief Wi-Fi, Weather Underground, APRS, and ThingSpeak credentials
 */

#include "credentials.h"

#include <Arduino.h> // for String

const String FW_VERSION = "2500610"; // Firmware version

// Wi-Fi Credentials
//! Place values in quotes " "
const String WIFI_SSID = "DCMNET";                   // your Wi-Fi SSID
const String WIFI_PASSWORD = "0F1A2D3E4D5G6L7O8R9Y"; // your Wi-Fi password

const String MY_TIMEZONE = "America/New_York"; // Olson timezone https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
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
const String CALLSIGN = "W4KRL-2";   // call-SSID
const String APRS_PASSCODE = "9092"; // https://aprs.do3sww.de/
const String APHORISM_FILE = "/aphorisms.txt";

// End of file
