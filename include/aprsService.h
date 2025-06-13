/**
 * @file aprsService.h
 * @brief APRS service header for posting weather data and bulletins to APRS-IS.
 * @author Karl Berger
 * @date 2025-05-14
 */

#include <Arduino.h> // for String

// Bulletin tracking flags
extern bool amBulletinSent;
extern bool pmBulletinSent;

void logonToAPRS();
bool readAPRSPacket(String &packet);
void postToAPRS(String message);
void APRSsendBulletin(String msg, String ID);
void processBulletins();
void handleAPRSData(const String &packet);
void pollAPRS();
