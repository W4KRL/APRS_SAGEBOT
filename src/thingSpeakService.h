/**
 * @file thingSpeakService.h
 * @author Karl W. Berger
 * @date 2025-05-28
 * @brief Declarations for ThingSpeak service integration.
 *
 * This header provides the interface for posting data to ThingSpeak and managing
 * the status of the ThingSpeak connection.
 *
 * Globals:
 *   - unitStatus: A String representing the current status of the ThingSpeak service.
 *
 * Functions:
 *   - postToThingSpeak(): Posts data to the ThingSpeak service.
 */
#ifndef THINGSPEAK_SERVICE_H
#define THINGSPEAK_SERVICE_H

#include <Arduino.h> // for String

extern String unitStatus; // ThingSpeak status

void postWXtoThingspeak();

#endif // THINGSPEAK_SERVICE_H
// End of file
