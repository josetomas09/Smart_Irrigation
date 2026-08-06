#include "Relay.h"

Relay::Relay(uint8_t p, String relayName) 
    : pin(p), state(false), name(relayName) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void Relay::on() {
    digitalWrite(pin, LOW);
    state = true;
}

void Relay::off() {
    digitalWrite(pin, HIGH);
    state = false;
}

void Relay::toggle() {
    state ? off() : on();
}

bool Relay::isActive() const {
    return state;
}