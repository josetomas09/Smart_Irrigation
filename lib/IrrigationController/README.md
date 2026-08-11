# IrrigationController Library

Clase central que encapsula la lógica del sistema de riego. Evalúa las condiciones temporales (horarios programados) o peticiones asíncronas (activación manual vía Blynk) y decide el accionamiento de los actuadores.

## Características
*   **Gestión de Tiempos:** Control de la duración del riego sin usar funciones bloqueantes.
*   **Modos de Operación:** Soporta modo Automático y modo Manual.
*   **Inyección de Dependencias:** Integración desacoplada con la capa de hardware utilizando instancias de la librería `Relay`.

## Ejemplo de Integración

```cpp
#include "IrrigationController.h"
#include "Relay.h"

Relay valvulaZona1(22, false);

IrrigationController controlador(&valvulaZona1);

void setup() {
    valvulaZona1.begin();
}

void loop() {
    controlador.update(); 
    controlador.startManualIrrigation(10); // Inicia riego por 10 minutos
}
```
