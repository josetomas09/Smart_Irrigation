# Comunication Library

Módulo encargado de gestionar la capa de conectividad del ESP32. Función principal es establecer, monitorear y mantener la conexión Wi-Fi, así como el enlace con los servidores de **Blynk** de forma estable.

##  Roadmap y Futuras Implementaciones
*   **OTA (Over-The-Air):** Soporte para actualización remota del firmware.
*   **ESP-NOW:** Implementación del protocolo de Espressif para recibir eventos rápidos desde otros nodos de la red local.

## Ejemplo de Integración

```cpp
#include "Communication.h"

Communication redIoT;

void setup() {
    redIoT.begin(); 
}

void loop() {
    redIoT.run(); 
}
```
