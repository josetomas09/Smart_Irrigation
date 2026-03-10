#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../../include/config.h"
#include <WiFi.h>
#include <esp_now.h>

class Communication {
    
public:
    static char recivedChar;

    static void begin();
    static bool isWifiConnected();
    static void onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int data_len);
};

#endif // COMMUNICATION_H