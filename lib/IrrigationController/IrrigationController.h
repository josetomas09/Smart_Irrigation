#ifndef IRRIGATION_CONTROLLER_H
#define IRRIGATION_CONTROLLER_H

#include <Arduino.h>
#include <Relay.h>
#include <time.h>
#include "../../include/config.h"

#define MAX_IRRIGATION_RELAYS 2  // Máximo número de relays para riego


/**
 * @brief Estructura para almacenar la programación de riego
 */
struct Schedule {
    long startTimeInSecs;
    long stopTimeInSecs;
    bool activeDays[7];  // Índices 0-6 para Lun-Dom
    bool isScheduled;

    bool startAtSunrise;
    bool startAtSunset;
    bool stopAtSunrise;
    bool stopAtSunset;
    
    Schedule();
    void reset();
};

/**
 * @brief Controlador principal del sistema de riego automático
 * 
 * Maneja los relays específicos para riego.
 * 
 */
class IrrigationController {
private:
    Relay* irrigationRelays[MAX_IRRIGATION_RELAYS];  // Relays del sistema de riego
    uint8_t irrigationRelayCount;                    // Cantidad de relays de riego activos
    
    Schedule schedule;
    bool manualMode;
    bool isWatering;
    
    // Sunrise/Sunset en segundos desde medianoche
    long sunriseSecs;
    long sunsetSecs;
    
    void startWatering();
    void stopWatering();
    bool shouldWaterNow();
    long getCurrentTimeInSecs();

public:
    /**
     * @brief Constructor
     */
    IrrigationController();
    
    /**
     * @brief Añade un relay al sistema de riego
     * @param relay Puntero al relay a añadir (bomba, válvula, etc.)
     * @return true si se añadió correctamente, false si se alcanzó el máximo
     */
    bool addIrrigationRelay(Relay* relay);
    
    /**
     * @brief Inicializa el controlador
     */
    void begin();
    
    /**
     * @brief Actualiza el estado del riego (llamar periódicamente)
     */
    void update();
    
    /**
     * @brief Activa el riego manual
     */
    void enableManualMode();
    
    /**
     * @brief Desactiva el riego manual
     */
    void disableManualMode();
    
    /**
     * @brief Configura un horario de riego
     * @param startSecs Hora de inicio en segundos desde medianoche
     * @param stopSecs Hora de fin en segundos desde medianoche
     */
    void setSchedule(long startSecs, long stopSecs);
    
    /**
     * @brief Configura inicio/fin en sunrise/sunset
     */
    void setStartAtSunrise(bool value);
    void setStartAtSunset(bool value);
    void setStopAtSunrise(bool value);
    void setStopAtSunset(bool value);
    
    /**
     * @brief Activa/desactiva un día de la semana
     * @param day Día (1=Lunes, 7=Domingo)
     * @param active true para activar, false para desactivar
     */
    void setDayActive(int day, bool active);
    
    /**
     * @brief Limpia toda la programación
     */
    void clearSchedule();
    
    /**
     * @brief Configura sunrise/sunset manualmente
     */
    void setSunriseSunset(long sunrise, long sunset);
    
    /**
     * @brief Obtiene el estado actual del riego
     */
    bool getIsWatering() const;
    
    /**
     * @brief Obtiene el modo manual
     */
    bool getManualMode() const;
};

#endif // IRRIGATION_CONTROLLER_H
