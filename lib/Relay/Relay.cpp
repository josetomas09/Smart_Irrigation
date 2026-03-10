#include "Relay.h"

Relay::Relay(uint8_t p, String relayName) 
    : pin(p), state(false), name(relayName) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Relay::on() {
    digitalWrite(pin, LOW); // HIGH
    state = true;
    if (name != "") {
        Serial.print("Relay '");
        Serial.print(name);
        Serial.println("' ON");
    }
}

void Relay::off() {
    digitalWrite(pin, HIGH); // LOW
    state = false;
    if (name != "") {
        Serial.print("Relay '");
        Serial.print(name);
        Serial.println("' OFF");
    }
}

void Relay::toggle() {
    if (state) {
        off();
    } else {
        on();
    }
}

bool Relay::isActive() const {
    return state;
}