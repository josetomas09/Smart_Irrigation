#include "Communication.h"
#include <Arduino.h>

char Communication::recivedChar = 0;

void Communication::begin() {

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);


    Serial.print("Iniciando WiFi ..");
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
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
    
    if(data_len != sizeof(recivedChar) || data_len <= 0){ 
        Serial.println("Error: Tamaño de dato recibido incorrecto");
        return;
    }else {
        memcpy(&recivedChar, data, sizeof(recivedChar));
        Serial.print("Dato recibido via ESP-NOW: ");
        Serial.println(recivedChar);
    }
    
}