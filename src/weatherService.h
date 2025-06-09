/**
 * @file weatherService.h
 * @author Karl Berger
 * @date 2025-05-28
 * @brief Weather service interface for fetching and storing weather data.
 *
 * This header defines the `weather` struct, which holds both forecast and observation data,
 * and declares functions for retrieving and updating weather information from external APIs.
 *
 * Dependencies:
 * - Arduino.h: For Arduino types and compatibility.
 * - ArduinoJson.h: For JSON parsing and HTTP data handling.
 *
 * Structures:
 * - weather: Holds forecast phrases, temperature extremes, cloud cover, sunrise/sunset times,
 *            observation station location, and a variety of observed meteorological parameters.
 *
 * Globals:
 * - wx: Externally declared instance of the `weather` struct for use across translation units.
 *
 * Functions:
 * - getWXforecast(): Retrieve and update forecasted weather data.
 * - getWXcurrent(): Retrieve and update current weather conditions.
 * - fetchDataAndParse(getQuery, filter, doc): Perform HTTP GET request to Weather Underground API,
 *      filter and parse the resulting JSON into the provided document.
 * - updateWXcurrent(): Update current weather conditions and post data to ThingSpeak.
 */
#ifndef WEATHER_SERVICE_H
#define WEATHER_SERVICE_H

#include <Arduino.h>     // for struct
#include <ArduinoJson.h> // for fetchData prototype v7.2 Benoit Blanchon https://arduinojson.org/

struct weather
{
  String forPhraseLong;     ///< long weather forecast 32 characters max
  String forPhraseShort;    ///< short weather forecast 12 characters max
  int forTempMax;           ///< forcasted high (°C)
  int forTempMin;           ///< forecasted low (°C)
  int forCloud;             ///< forecasted average cloud coverage (%)
  unsigned long forSunRise; ///< sunrise (unix time UTC) from forecast
  unsigned long forSunSet;  ///< sunset (unix time UTC) from forecast
  float obsLat;             ///< station latitude in decimal degrees
  float obsLon;             ///< station longitude in decimal degrees
  String obsNeighborhood;   ///< station neighborhood assigned by Weather Underground
  float obsSolarRadiation;  ///< luminosity (W/m^2)
  float obsUV;              ///< UV index
  float obsHumidity;        ///< relative humidity (%)
  float obsDewPt;           ///< dewpoint (°C)
  float obsTemp;            ///< temperature (°C)
  float obsHeatIndex;       ///< temperature feel in Celsius valid for greater than 65°F (18°C)
  float obsWindChill;       ///< temperature feel valid below 65°F (18°C)
  float obsWindDir;         ///< wind direction degrees clockwise from north
  float obsWindSpeed;       ///< km/h
  float obsWindGust;        ///< km/h
  float obsPressure;        ///< sea level pressure millibars (hPa)
  float obsPrecipRate;      ///< instantaneous rate for an hour (mm/h)
  float obsPrecipTotal;     ///< total precipitation midnight to present (mm)
};

extern weather wx; // Declaration for use in other files

void getWXforecast();   ///< get forecasted weather
void getWXcurrent();    ///< get current conditions

#endif // WEATHER_SERVICE_H
// End of file
