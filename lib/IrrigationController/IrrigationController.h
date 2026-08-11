#ifndef IRRIGATION_CONTROLLER_H
#define IRRIGATION_CONTROLLER_H

#include <Arduino.h>
#include <Relay.h>
#include <time.h>
#include "../../include/Config.h"


#define MAX_IRRIGATION_RELAYS 2  // Maximum number of irrigation relays


/**
 * @brief Structure to store irrigation schedule
 */
struct Schedule {
    long startTimeInSecs;
    long stopTimeInSecs;
    bool activeDays[7];  // Indices 0-6 for Mon-Sun
    bool isScheduled;

    bool startAtSunrise;
    bool startAtSunset;
    bool stopAtSunrise;
    bool stopAtSunset;
    
    Schedule();
    void reset();
};

/**
 * @brief Main controller for the automatic irrigation system
 *
 * Manages the relays dedicated to irrigation.
 *
 */
class IrrigationController {
private:
    Relay* irrigationRelays[MAX_IRRIGATION_RELAYS];  // Irrigation system relays
    uint8_t irrigationRelayCount;                    // Number of active irrigation relays
    
    Schedule schedule;
    bool manualMode;
    bool isWatering;
    
    // Sunrise/Sunset in seconds from midnight
    long sunriseSecs;
    long sunsetSecs;
    
    void startWatering();
    void stopWatering();
    bool shouldWaterNow();
    long getCurrentTimeInSecs();
    int normalizeDayIndex(int day) const;

public:
    /**
     * @brief Constructor
     */
    IrrigationController();
    
    /**
     * @brief Adds a relay to the irrigation system
     * @param relay Pointer to the relay to add (pump, valve, etc.)
     * @return true if added successfully, false if the maximum was reached
     */
    bool addIrrigationRelay(Relay* relay);
    
    /**
     * @brief Initializes the controller
     */
    void begin();
    
    /**
     * @brief Updates the irrigation state (call periodically)
     */
    void update();
    
    /**
     * @brief Enable manual irrigation
     */
    void enableManualMode();
    
    /**
     * @brief Disable manual irrigation
     */
    void disableManualMode();
    
    /**
     * @brief Configure an irrigation schedule
     * @param startSecs Start time in seconds since midnight
     * @param stopSecs Stop time in seconds since midnight
     */
    void setSchedule(long startSecs, long stopSecs);
    
    /**
     * @brief Configure start/stop at sunrise/sunset
     */
    void setStartAtSunrise(bool value);
    void setStartAtSunset(bool value);
    void setStopAtSunrise(bool value);
    void setStopAtSunset(bool value);
    
    /**
     * @brief Enable/disable a weekday
     * @param day Day (1=Monday, 7=Sunday)
     * @param active true to enable, false to disable
     */
    void setDayActive(int day, bool active);
    
    /**
     * @brief Clear the entire schedule
     */
    void clearSchedule();
    
    /**
     * @brief Set sunrise/sunset values manually
     */
    void setSunriseSunset(long sunrise, long sunset);
    
    /**
     * @brief Get the current watering state
     */
    bool getIsWatering() const;
    
    /**
     * @brief Get the manual mode state
     */
    bool getManualMode() const;
};

#endif // IRRIGATION_CONTROLLER_H
