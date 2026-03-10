#include "IrrigationController.h"

// ============================================================
//                    Struct Schedule
// ============================================================

Schedule::Schedule() {
    reset();
}

void Schedule::reset() {
    startTimeInSecs = -1;
    stopTimeInSecs = -1;
    startAtSunrise = false;
    startAtSunset = false;
    stopAtSunrise = false;
    stopAtSunset = false;
    isScheduled = false;
    
    for (int i = 0; i < 7; i++) {
        activeDays[i] = false;
    }
}

// ============================================================
//              Clase IrrigationController
// ============================================================

IrrigationController::IrrigationController()
    : irrigationRelayCount(0), manualMode(false), isWatering(false) {
    
    // Inicializar array de relays de riego
    for(int i = 0; i < MAX_IRRIGATION_RELAYS; i++) {
        irrigationRelays[i] = nullptr;
    }
    
    // Valores por defecto para sunrise/sunset
    sunriseSecs = 7 * 3600 + 30 * 60;  // 07:30
    sunsetSecs = 19 * 3600 + 30 * 60;  // 19:30
}

bool IrrigationController::addIrrigationRelay(Relay* relay) {
    if(irrigationRelayCount >= MAX_IRRIGATION_RELAYS) {
        Serial.println("Máximo de relays de riego alcanzado");
        return false;
    }
    
    irrigationRelays[irrigationRelayCount] = relay;
    irrigationRelayCount++;
    
    Serial.print("Relay de riego añadido: ");
    Serial.print(relay->getName());
    Serial.print(" (");
    Serial.print(irrigationRelayCount);
    Serial.print("/");
    Serial.print(MAX_IRRIGATION_RELAYS);
    Serial.println(")");
    
    return true;
}

void IrrigationController::begin() {
    Serial.println("=== IrrigationController ===");
    
    if(irrigationRelayCount == 0) {
        Serial.println("ADVERTENCIA: No hay relays de riego configurados");
    }else {
        Serial.print("Relays de riego: ");
        Serial.println(irrigationRelayCount);
    }
    
    struct tm timeinfo;
    if(getLocalTime(&timeinfo)){
        Serial.print("RTC Interno OK - ");
        Serial.print(timeinfo.tm_mday);
        Serial.print("/");
        Serial.print(timeinfo.tm_mon + 1);
        Serial.print("/");
        Serial.print(timeinfo.tm_year + 1900);
        Serial.print(" ");
        Serial.print(timeinfo.tm_hour);
        Serial.print(":");
        Serial.println(timeinfo.tm_min);
    } else {
        Serial.println("RTC Interno: Esperando sincronización NTP...");
    }
    
    Serial.println("============================");
}

void IrrigationController::startWatering() {
    for(uint8_t i = 0; i < irrigationRelayCount; i++) {
        if(irrigationRelays[i] != nullptr) {
            irrigationRelays[i]->on();
        }
    }
    isWatering = true;
    Serial.println("Riego Iniciado");
}

void IrrigationController::stopWatering() {
    for(uint8_t i = 0; i < irrigationRelayCount; i++) {
        if(irrigationRelays[i] != nullptr) {
            irrigationRelays[i]->off();
        }
    }
    isWatering = false;
    Serial.println("Riego Detenido");
}

long IrrigationController::getCurrentTimeInSecs() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return 0; // Si no hay hora válida, retornar 0
    }
    return timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60 + timeinfo.tm_sec;
}

bool IrrigationController::shouldWaterNow() {
    if(!schedule.isScheduled) return false;
    
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return false; // Si no hay hora válida, no regar
    }
    
    // Verificar día activo (tm_wday: 0=Dom, 1=Lun)
    int espDay = timeinfo.tm_wday;  // 0=Dom, 1=Lun
    int blynkDay = (espDay == 0) ? 7 : espDay;
    
    if(!schedule.activeDays[blynkDay]) {
        return false;
    }
    
    // Determinar horarios objetivo
    long startTarget = schedule.startTimeInSecs;
    long stopTarget = schedule.stopTimeInSecs;
    
    if(schedule.startAtSunrise) startTarget = sunriseSecs;
    if(schedule.startAtSunset) startTarget = sunsetSecs;
    if(schedule.stopAtSunrise) stopTarget = sunriseSecs;
    if(schedule.stopAtSunset) stopTarget = sunsetSecs;
    
    // Si inicio y fin son iguales, no hacer nada
    if(startTarget == stopTarget){return false;} 
    
    long currentSecs = getCurrentTimeInSecs();
    
    if(startTarget < stopTarget) {
        // Caso normal: 08:00 a 18:00
        return (currentSecs >= startTarget && currentSecs < stopTarget);
    }else {
        // Cruza medianoche: 23:00 a 01:00
        return (currentSecs >= startTarget || currentSecs < stopTarget);
    }
}

void IrrigationController::update() {
    if(manualMode) return;
    
    bool shouldBeOn = shouldWaterNow();
    
    if(shouldBeOn && !isWatering) {
        startWatering();
        Serial.println("Activado por Horario");
    }else if(!shouldBeOn && isWatering) {
        stopWatering();
        Serial.println("Desactivado por Horario");
    }
}

void IrrigationController::enableManualMode() {
    manualMode = true;
    startWatering();
    Serial.println("Modo Manual: ON");
}

void IrrigationController::disableManualMode() {
    manualMode = false;
    stopWatering();
    Serial.println("Modo Manual: OFF");
}

void IrrigationController::setSchedule(long startSecs, long stopSecs) {
    schedule.startTimeInSecs = startSecs;
    schedule.stopTimeInSecs = stopSecs;
    schedule.startAtSunrise = false;
    schedule.startAtSunset = false;
    schedule.stopAtSunrise = false;
    schedule.stopAtSunset = false;
    schedule.isScheduled = true;
    Serial.println("Horario configurado");
}

void IrrigationController::setStartAtSunrise(bool value) {
    schedule.startAtSunrise = value;
    if(value) schedule.isScheduled = true;
}

void IrrigationController::setStartAtSunset(bool value) {
    schedule.startAtSunset = value;
    if(value) schedule.isScheduled = true;
}

void IrrigationController::setStopAtSunrise(bool value) {
    schedule.stopAtSunrise = value;
}

void IrrigationController::setStopAtSunset(bool value) {
    schedule.stopAtSunset = value;
}

void IrrigationController::setDayActive(int day, bool active) {
    if(day >= 1 && day <= 7) {
        schedule.activeDays[day] = active;
    }
}

void IrrigationController::clearSchedule() {
    schedule.reset();
    Serial.println("Programación eliminada");
}

void IrrigationController::setSunriseSunset(long sunrise, long sunset) {
    sunriseSecs = sunrise;
    sunsetSecs = sunset;
}

bool IrrigationController::getIsWatering() const {
    return isWatering;
}

bool IrrigationController::getManualMode() const {
    return manualMode;
}
