#include "../include/config.h"

#include <Arduino.h>
#include "BlynkEdgent.h"
#include <communication.h>
#include <DHT.h>
#include <Relay.h>
#include <IrrigationController.h>
#include <Preferences.h>

#define BLYNK_PRINT Serial

/* 
    ========================================================
                2. Global Variables and Objects
    ========================================================
*/

uint8_t DHTPIN = 5;
uint8_t RELAY1_PIN = 12;
uint8_t RELAY2_PIN = 13;

DHT dht(5, DHT11); 
BlynkTimer timer;
Preferences preferences;

// Relays de riego
Relay electrovalvula_1(RELAY1_PIN, "Valvula Zona 1");
Relay electrovalvula_2(RELAY2_PIN, "Válvula Zona 2");
WidgetTerminal terminal(V5);

IrrigationController riego;

bool ntpSync();
void sendTempAndHum();

/* 
    ========================================================
                3. Setup and Loop
    ========================================================
*/

void setup(){
  Serial.begin(115200);
  delay(100);

  terminal.clear();
  terminal.println("Iniciando...");
  
  Communication::begin();
  dht.begin();

  preferences.begin("Irrigation", false);

  riego.addIrrigationRelay(&electrovalvula_1);
  riego.addIrrigationRelay(&electrovalvula_2);
  riego.begin();

  if(Communication::isWifiConnected()){
    BlynkEdgent.begin();
    delay(1000);
    ntpSync();
  }

  timer.setInterval(30000L, sendTempAndHum); // Enviar temperatura y humedad cada 30 segundos
  timer.setInterval(60000L, [](){ riego.update(); }); // Actualizar estado del riego cada minuto

}

void loop() {
  if(Communication::isWifiConnected()){
     BlynkEdgent.run();
  }
  timer.run();
}

/* 
    ========================================================
                3. Helper Functions
    ========================================================
*/

bool ntpSync(){
    
    configTime(gmtOffset_SEC, 0, ntpServer);
    delay(2000);

    struct tm timeInfo;
    uint8_t retries = 0;
    while(!getLocalTime(&timeInfo) && retries < 20) {
        terminal.println("Sincronizando NTP...");
        delay(1000);
        retries++;
    };

    if(retries >= 20) {
        terminal.println("Error: No se pudo sincronizar fecha y hora luego de varios intentos.");
        return false;
    }

    uint16_t ye = timeInfo.tm_year + 1900;
    uint8_t mo = timeInfo.tm_mon + 1;
    uint8_t da = timeInfo.tm_mday;
    uint8_t ho = timeInfo.tm_hour;
    uint8_t mi = timeInfo.tm_min;
    uint8_t se = timeInfo.tm_sec;

    terminal.println("Fecha y hora obtenida de NTP:");
    terminal.print(da); terminal.print("/");
    terminal.print(mo); terminal.print("/"); terminal.print(ye); terminal.print(" ");
    terminal.print(ho); terminal.print(":"); terminal.print(mi); terminal.print(":"); terminal.print(se); terminal.println();

    // Guardar timestamp en Preferences
    time_t now = mktime(&timeInfo);
    preferences.putUInt("lastSync", now);

    return true;
}

/* 
    ========================================================
                4. Blynk Functions
    ========================================================
*/

void sendTempAndHum(){
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if(isnan(temp) || isnan(humidity)) {
        terminal.println("Error leyendo sensor DHT11");
        return;
    }


    /*
    terminal.print("Temp: "); terminal.print(temp);
    terminal.print(" Hum: "); terminal.println(humidity);
    */
   
    Blynk.virtualWrite(V0, temp);
    Blynk.virtualWrite(V1, humidity);
}

// Riego manual
BLYNK_WRITE(V2) {
    int v = param.asInt();
    if(v == 1) {
        riego.enableManualMode();
    } else {
        riego.disableManualMode();
    }
}


BLYNK_WRITE(V3) {
    TimeInputParam t(param);
    
    riego.clearSchedule();

    // 1. Configurar horario de inicio
    if(t.hasStartTime()) {
        long startSecs = t.getStartHour() * 3600 + t.getStartMinute() * 60 + t.getStartSecond();
        long stopSecs = -1;
        
        // 2. Configurar horario de fin
        if(t.hasStopTime()) {
            stopSecs = t.getStopHour() * 3600 + t.getStopMinute() * 60 + t.getStopSecond();
        }
        
        if(stopSecs != -1) {
            riego.setSchedule(startSecs, stopSecs);
        }
    } else if(t.isStartSunrise()) {
        riego.setStartAtSunrise(true);
    } else if(t.isStartSunset()) {
        riego.setStartAtSunset(true);
    }

    // Configurar fin en sunrise/sunset si aplica
    if(t.isStopSunrise()) {
        riego.setStopAtSunrise(true);
    } else if(t.isStopSunset()) {
        riego.setStopAtSunset(true);
    }

    // 3. Configurar días activos
    for(int i = 1; i <= 7; i++) {
        riego.setDayActive(i, t.isWeekdaySelected(i));
    }
    
    terminal.println("Nueva programación recibida");
}

BLYNK_READ(V4) {
    Blynk.virtualWrite(V4, Communication::recivedChar);
}