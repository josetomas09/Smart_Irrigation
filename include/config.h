#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// Blynk Configuration (DEBE estar ANTES de incluir BlynkSimpleEsp32.h)
#define BLYNK_TEMPLATE_ID "TMPL2_1d50D2s"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation Garaje"
// #define BLYNK_AUTH_TOKEN "FFpWXZbVi_lb8A2TbVkcSf-ie1Dv1SWi"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

// WiFi Credentials
#define WIFI_SSID "Casa"                   // "TP-Link_CF0E"
#define WIFI_PASSWORD "20joseluis23"       // "garaje2023"

// NTP Config
#define ntpServer "ar.pool.ntp.org"
const long gmtOffset_SEC = -3 * 3600;

// ESPNOW Config
const uint8_t MAC_SENDER_1[] = { 0x08, 0x92, 0x72, 0x84, 0x35, 0x48 };

#endif