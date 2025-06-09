/**
 * @file weatherService.cpp
 * @author Karl Berger
 * @date 2025-06-02
 * @brief Weather service implementation
 * @details This file contains the functions for fetching weather data from Weather Underground
 *          and posting it to the display, ThingSpeak, and APRS
 */

#include "weatherService.h"

#include <Arduino.h>           // Arduino functions
#include "credentials.h"       // Wi-Fi and weather station credentials
#include <ESP8266HTTPClient.h> // [builtin] for http and https
#include <WiFiClientSecure.h>  // [builtin] for https
#include <ArduinoJson.h>       // [manager] v7.2 Benoit Blanchon https://arduinojson.org/
#include "thingSpeakService.h" // ThingSpeak service header
#include "wug_debug.h"         // debug print

weather wx; // global weather object

//! ***** Weather Underground Personal Weather Station (PWS) *****
// !!! DO NOT CHANGE !!!
// documentation: https://docs.google.com/document/d/1eKCnKXI9xnoMGRRzOL1xPCBihNV2rOet08qpE_gArAY/edit?tab=t.0
// WX_KEY is in credentials.h
const String WX_HOST = "https://api.weather.com";        ///< Weather Underground API host
const String WX_CURRENT = "v2/pws/observations/current"; ///< Current weather observations endpoint
const String WX_FORECAST = "v3/wx/forecast/daily/5day";  ///< Forecast weather endpoint
const String WX_LANGUAGE = "en-US";                      ///< Language for the API response
const String WX_UNITS = "m";                             ///< MUST USE METRIC!!!
const String WX_FORMAT = "json";                         ///< Format of the API response
const String WX_PRECISION = "decimal";                   ///< Precision of the API response

/*
******************************************************
************* fetch Data and Parse *******************
******************************************************
*/

void fetchDataAndParse(String getQuery, JsonDocument &filter, JsonDocument &doc)
{
  // HTTP request and parsing logic
  // by Copilot 12/15/2024
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;

  if (https.begin(client, getQuery))
  {
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      if (httpCode == HTTP_CODE_OK)
      {
        DeserializationError error = deserializeJson(doc, client, DeserializationOption::Filter(filter));
        if (error)
        {
          DEBUG_PRINT("deserialization failed: ");
          DEBUG_PRINTLN(error.c_str());
        }
      }
    }
    else
    {
      DEBUG_PRINT("GET error: ");
      DEBUG_PRINTLN(https.errorToString(httpCode).c_str());
    }
    https.end();   // Ensure connection is closed after each request
    client.stop(); // Explicitly close the client connection
  }
  else
  {
    DEBUG_PRINTLN("https: can't connect");
  }
} // fetchDataAndParse()

/*
******************************************************
************** Get Current Weather *******************
******************************************************
*/
void getWXcurrent()
{
  // Documentation:
  // https://api.weather.com/v2/pws/observations/current?stationId=yourStationID&format=json&units=m&numericPrecision=decimal&apiKey=yourApiKey
  // Uses API stream

  String getQuery = WX_HOST + "/" + WX_CURRENT +
                    "?stationId=" + WX_STATION_ID +
                    "&format=" + WX_FORMAT +
                    "&units=" + WX_UNITS +
                    "&numericPrecision=" + WX_PRECISION +
                    "&apiKey=" + WX_KEY;

  JsonDocument filter; // filter to reduce size of JsonDocument
  filter["observations"][0]["lat"] = true;
  filter["observations"][0]["lon"] = true;
  filter["observations"][0]["neighborhood"] = true;
  filter["observations"][0]["solarRadiation"] = true;
  filter["observations"][0]["uv"] = true;
  filter["observations"][0]["winddir"] = true;
  filter["observations"][0]["humidity"] = true;
  filter["observations"][0]["metric"] = true; // extends to all items under "metric"
  // includes temp, heatIndex, dewpt, windChill, windSpeed, windGust, pressure, precip rates

  JsonDocument doc; // holds filtered json stream

  fetchDataAndParse(getQuery, filter, doc);

  JsonObject observations_0 = doc["observations"][0];
  if (observations_0["lat"] != 0)
  {
    wx.obsLat = observations_0["lat"];                           // decimal latitude
    wx.obsLon = observations_0["lon"];                           // decimal longitude
    wx.obsNeighborhood = (String)observations_0["neighborhood"]; // WU area
    wx.obsSolarRadiation = observations_0["solarRadiation"];     // W/m^2
    wx.obsUV = observations_0["uv"];                             // UV index
    wx.obsWindDir = observations_0["winddir"];                   // degrees clockwise from North
    wx.obsHumidity = observations_0["humidity"];                 // relative humidity 0 - 100%
    JsonObject observations_0_metric = observations_0["metric"]; //! Program expects all metric values
    wx.obsTemp = observations_0_metric["temp"];                  // Celsius
    wx.obsHeatIndex = observations_0_metric["heatIndex"];        // Celsius valid when temp >+ 18C
    wx.obsDewPt = observations_0_metric["dewpt"];                // Celsius
    wx.obsWindChill = observations_0_metric["windChill"];        // Celsius valid when temp < 18C
    wx.obsWindSpeed = observations_0_metric["windSpeed"];        // km/h
    wx.obsWindGust = observations_0_metric["windGust"];          // km/h
    wx.obsPressure = observations_0_metric["pressure"];          // millibars of mercury
    wx.obsPrecipRate = observations_0_metric["precipRate"];      // mm for rain, cm for snow
    wx.obsPrecipTotal = observations_0_metric["precipTotal"];    // mm/h for rain, {cm/h for snow???} from midnight
  }
  else
  {
    DEBUG_PRINTLN("No data from WU");
  }
} // getWXcurrent()

/*
******************************************************
************** Get Forecast Weather ******************
******************************************************
*/
void getWXforecast()
{
  // The PWS Google doc appears to be out of date. Use the IBM document for 5-day.
  // Obsolete Documentation: https://docs.google.com/document/d/1_Zte7-SdOjnzBttb1-Y9e0Wgl0_3tah9dSwXUyEA3-c/edit?tab=t.0
  // Use this Documentation: https://www.ibm.com/docs/en/environmental-intel-suite?topic=fa-daily-forecast-3-day-5-day-7-day-10-day
  // Query: https://api.weather.com/v3/wx/forecast/daily/5day?geocode=38.89,-77.03&format=json&units=m&language=en-US&apiKey=yourApiKey
  // Uses API stream
  // By Copilot 12/15/2024
  // solves String capacity problem

  String getQuery = WX_HOST + "/" + WX_FORECAST +
                    "?geocode=" + String(wx.obsLat) + "," + String(wx.obsLon) +
                    "&format=" + WX_FORMAT +
                    "&units=" + WX_UNITS +
                    "&language=" + WX_LANGUAGE +
                    "&apiKey=" + WX_KEY;

  JsonDocument filter;
  filter["calendarDayTemperatureMax"] = true;   // The midnight to midnight daily maximum temperature.
  filter["calendarDayTemperatureMin"] = true;   // The midnight to midnight daily minimum temperature.
  filter["sunriseTimeUtc"] = true;              // Sunrise time in UNIX epoch value.
  filter["sunsetTimeUtc"] = true;               // Sunset time in UNIX epoch value.
  filter["daypart"][0]["cloudCover"] = true;    // Daytime average cloud cover expressed as a percentage.
  filter["daypart"][0]["wxPhraseLong"] = true;  // Hourly sensible weather phrase up to 32 characters.
  filter["daypart"][0]["wxPhraseShort"] = true; // Hourly sensible weather phrase up to 12 characters.

  // parse the filtered JsonDocument
  JsonDocument doc;
  fetchDataAndParse(getQuery, filter, doc);

  JsonArray calendarDayTemperatureMax = doc["calendarDayTemperatureMax"];
  wx.forTempMax = (calendarDayTemperatureMax[0]) ? calendarDayTemperatureMax[0] : calendarDayTemperatureMax[1];

  JsonArray calendarDayTemperatureMin = doc["calendarDayTemperatureMin"];
  wx.forTempMin = (calendarDayTemperatureMin[0]) ? calendarDayTemperatureMin[0] : calendarDayTemperatureMin[1];

  JsonArray sunriseTimeUtc = doc["sunriseTimeUtc"];
  wx.forSunRise = (sunriseTimeUtc[0]) ? sunriseTimeUtc[0] : sunriseTimeUtc[1]; // 1731412186

  JsonArray sunsetTimeUtc = doc["sunsetTimeUtc"];
  wx.forSunSet = (sunsetTimeUtc[0]) ? sunsetTimeUtc[0] : sunsetTimeUtc[1]; // 1731448679

  JsonObject daypart_0 = doc["daypart"][0];
  JsonArray daypart_0_cloudCover = daypart_0["cloudCover"];
  wx.forCloud = (daypart_0_cloudCover[0]) ? daypart_0_cloudCover[0] : daypart_0_cloudCover[1];

  JsonArray daypart_0_wxPhraseLong = daypart_0["wxPhraseLong"];
  wx.forPhraseLong = (daypart_0_wxPhraseLong[0]) ? (String)daypart_0_wxPhraseLong[0] : (String)daypart_0_wxPhraseLong[1];

  JsonArray daypart_0_wxPhraseShort = daypart_0["wxPhraseShort"];
  wx.forPhraseShort = (daypart_0_wxPhraseShort[0]) ? (String)daypart_0_wxPhraseShort[0] : (String)daypart_0_wxPhraseShort[1];

  // prettified print
  DEBUG_PRINTLN("Forecast filtered:");
  DEBUG_PRINT("\tTemp Max:\t");
  DEBUG_PRINTLN(wx.forTempMax);
  DEBUG_PRINT("\tTemp Min:\t");
  DEBUG_PRINTLN(wx.forTempMin);
  DEBUG_PRINT("\tCloud cover:\t");
  DEBUG_PRINTLN(wx.forCloud);
  DEBUG_PRINT("\tPhrase Long:\t");
  DEBUG_PRINTLN(wx.forPhraseLong);
  DEBUG_PRINT("\tPhrase Short:\t");
  DEBUG_PRINTLN(wx.forPhraseShort);
} // getWXforecast()
