/**
 * @file aprsService.h
 * @brief APRS service header for posting weather data and bulletins to APRS-IS.
 * @author Karl Berger
 * @date 2025-05-14
 */

#include <Arduino.h> // for String

// Bulletin tracking flags
extern bool amBulletinSent; ///< Indicates if the morning bulletin was sent
extern bool pmBulletinSent; ///< Indicates if the evening bulletin was sent

/**
 * @brief Posts a message to APRS-IS.
 * @param message Message to be posted.
 */
void postToAPRS(String message);

/**
 * @brief Formats and sends weather data to APRS-IS.
 * @return Formatted weather string.
 */
String APRSformatWeather();

void postWXtoAPRS();


/**
 * @brief Formats a bulletin or announcement message according to the APRS specification.
 *
 * This function constructs an APRS bulletin or announcement string using the provided message and ID.
 * The format follows the APRS101.pdf specification (page 83), where:
 * - Bulletin ID is a single digit from 0 to 9.
 * - Announcement ID is a single uppercase letter from A to Z.
 * - The message may not contain the characters '|', '~', or '`'.
 * - The resulting string is formatted as: "<CALLSIGN>APRS,TCPIP*::BLN<ID>     :<message>"
 *
 * @param msg The message content to be included in the bulletin (0 to 67 characters).
 * @param ID The bulletin or announcement identifier (single digit or uppercase letter).
 * @return Formatted bulletin string.
 */
String APRSformatBulletin(String msg, String ID);

/**
 * @brief Sends an APRS bulletin message with a specified ID.
 *
 * This function formats the given message and ID as an APRS bulletin and posts it to the APRS-IS network.
 *
 * @param msg The bulletin message to be sent.
 * @param ID  The identifier for the bulletin message.
 */
void APRSsendBulletin(String msg, String ID);

/**
 * @brief Pads values for APRS formatting.
 * @param value Numeric value to pad.
 * @param width Desired width.
 * @return Padded string.
 */
String APRSpadder(float value, int width);

/**
 * @brief Pads a callsign for APRS formatting.
 * @param callSign Callsign to pad.
 * @return Padded callsign string.
 */
String APRSpadCall(String callSign);

/**
 * @brief Formats latitude and longitude for APRS location.
 * @param lat Latitude coordinate.
 * @param lon Longitude coordinate.
 * @return Formatted location string.
 */
String APRSlocation(float lat, float lon);

void processBulletins();