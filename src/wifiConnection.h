/**
 * @file wifiConnection.h
 * @author Karl W. Berger
 * @date 2025-05-28
 * @brief Declares the interface for connecting the device to a Wi-Fi router.
 *
 * This header provides the declaration for the function responsible for establishing
 * a connection to a predefined Wi-Fi network. Ensure that the Wi-Fi credentials are
 * properly configured before invoking the connection function.
 */
#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

void logonToRouter();		// function to connect to Wi-Fi
void checkWiFiConnection(); // function to check Wi-Fi connection status

#endif // WIFI_CONNECTION_H

// End of file