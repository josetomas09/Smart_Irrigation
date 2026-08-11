#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../../include/Config.h"
#include <WiFi.h>
#include <esp_now.h>

class Communication {
    
public:
    /**
     * @brief Stores the last character received through ESP-NOW.
     */
    static char receivedChar;

    /**
     * @brief Initializes Wi-Fi and ESP-NOW communication.
     */
    static void begin();

    /**
     * @brief Checks whether the device is currently connected to Wi-Fi.
     * @return true if Wi-Fi is connected, false otherwise.
     */
    static bool isWifiConnected();

    /**
     * @brief Handles data received through ESP-NOW.
     * @param mac_addr Source MAC address of the sender.
     * @param data Pointer to the received data.
     * @param data_len Length of the received data.
     */
    static void onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int data_len);
};

#endif // COMMUNICATION_H