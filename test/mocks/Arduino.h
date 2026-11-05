// Simulacion minima del entorno Arduino/ESP32 para ejecutar las pruebas
// unitarias en la PC (entorno "native" de PlatformIO), sin la placa.
#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <ctime>

#define HIGH 0x1
#define LOW  0x0
#define INPUT  0x01
#define OUTPUT 0x03

// ---------------- GPIO simulado ----------------
inline uint8_t fakePinMode[40]  = {0};
inline uint8_t fakePinLevel[40] = {0};

inline void pinMode(uint8_t pin, uint8_t mode)   { fakePinMode[pin] = mode; }
inline void digitalWrite(uint8_t pin, uint8_t v) { fakePinLevel[pin] = v ? HIGH : LOW; }
inline int  digitalRead(uint8_t pin)             { return fakePinLevel[pin]; }

// ---------------- Tiempo simulado ----------------
inline unsigned long fakeMillis = 0;
inline unsigned long millis() { return fakeMillis; }
inline void delay(unsigned long ms) { fakeMillis += ms; }

// Reloj del sistema simulado: reemplaza a getLocalTime() del core ESP32
inline struct tm fakeClock = {};
inline bool fakeClockValid = false;

inline bool getLocalTime(struct tm *info, uint32_t ms = 5000) {
    (void)ms;
    if (!fakeClockValid) return false;
    *info = fakeClock;
    return true;
}

// ---------------- String ----------------
class String : public std::string {
public:
    String(const char *s = "") : std::string(s) {}
    String(const std::string &s) : std::string(s) {}
};

// ---------------- Serial (descarta la salida) ----------------
class FakeSerial {
public:
    void begin(unsigned long) {}
    template <typename T> void print(const T &) {}
    template <typename T> void println(const T &) {}
    void println() {}
};
inline FakeSerial Serial;