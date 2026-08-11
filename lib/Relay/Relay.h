#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include "../../include/config.h"

/**
 * @brief Class to control an individual relay
 *
 * Allows turning relays on/off and querying their state.
 * Each relay is independent and can be used for any purpose.
 */
class Relay {
private:
    uint8_t pin;
    bool state;
    String name;

public:
    /**
     * @brief Constructor
     * @param p GPIO pin of the relay
     * @param relayName Optional name for the relay
     */
    
    Relay(uint8_t p, String relayName = "");
    
    void on();
    void off();
    void toggle();
    bool isActive() const;
    String getName() const { return name; }
    
};

#endif // RELAY_H
