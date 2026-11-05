#include "Communication.h"
#include <Arduino.h>

char Communication::receivedChar = 0;

void Communication::begin() {

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);


    Serial.print("Iniciando WiFi ..");
    Serial.print("Iniciando WiFi ..");
    const unsigned long WIFI_TIMEOUT_MS = 15000;
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < WIFI_TIMEOUT_MS) {
        Serial.print('.');
        delay(500);
    }

    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi conectado exitosamente");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nSin WiFi: se continua sin conexion y se reintenta en segundo plano");
    }

    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi conectado exitosamente");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    }else {
        Serial.println("\nNo se pudo conectar a WiFi");
        Serial.print("Estado de WiFi: ");
        Serial.println(WiFi.status());
    }


    Serial.println("Inicializando ESP-NOW...");
    if(esp_now_init() != ESP_OK) {
        Serial.println("Error al iniciar ESP-NOW");
        return;
    }
    Serial.println("ESP-NOW iniciado exitosamente");
    esp_now_register_recv_cb(onDataReceived);

}

bool Communication::isWifiConnected() {
    return WiFi.status() == WL_CONNECTED;
}



void Communication::onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    
    if(data_len != sizeof(receivedChar) || data_len <= 0){ 
        Serial.println("Error: Tamaño de dato recibido incorrecto");
        return;
    }else {
        memcpy(&receivedChar, data, sizeof(receivedChar));
        Serial.print("Dato recibido via ESP-NOW: ");
        Serial.println(receivedChar);
    }
    
}