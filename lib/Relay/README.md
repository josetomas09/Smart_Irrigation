# Relay Library

Librería orientada a objetos diseñada para abstraer y gestionar los módulos de relé conectados al ESP32. 

Permite inicializar el pin, definir su lógica de estado activo y cambiar el estado del actuador (bombas de agua, electroválvulas) manteniendo limpia la capa de lógica de negocio.

## Ejemplo de Integración

```cpp
#include "Relay.h"

// El segundo parámetro (opcional) indica si es activo en ALTO (true) o BAJO (false).
Relay miBomba(23, false); 

void setup() {
    miBomba.begin(); 
}

void loop() {
    miBomba.turnOn();
    delay(5000);
    
    miBomba.turnOff();
    delay(5000);
}
```

## ⚙️ Métodos Principales
* `begin()`: Inicializa el hardware.
* `turnOn()`: Enciende el relé aplicando la lógica configurada.
* `turnOff()`: Apaga el relé.
* `getState()`: Retorna el estado actual del relé para retroalimentación.
