#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include "../../include/config.h"

/**
 * @brief Clase para controlar un relay individual
 * 
 * Permite encender/apagar relays y consultar su estado.
 * Cada relay es independiente y puede usarse para cualquier propósito.
 */
class Relay {
private:
    uint8_t pin;
    bool state;
    String name;

public:
    /**
     * @brief Constructor
     * @param p Pin GPIO del relay
     * @param relayName Nombre opcional del relay (ej: "Bomba", "Válvula Zona 1")
     */
    Relay(uint8_t p, String relayName = "");
    
    void on();
    void off();
    void toggle();
    bool isActive() const;
    String getName() const { return name; }
    
};

#endif // RELAY_H
