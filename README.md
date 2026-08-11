# Smart Irrigation

Proyecto de riego automático para ESP32 con integración Blynk, sensor DHT11, módulo RTC DS3231, y relés para controlar electroválvulas.

**Características principales**
- Control de hasta 3 relés para riego (válvulas/LED estado).
- Lectura periódica de temperatura y humedad desde un DHT11 y envío a Blynk.
- Programación horaria de riego (inicio/fin) mediante widget `TimeInput` en Blynk.
- Modo manual de riego desde Blynk.
- Sincronización de hora por NTP y persistencia en RTC DS3231.
- Guardado de últimas sincronizaciones y opciones en `Preferences` de ESP32.
- Comunicación local mediante ESPNOW (configurable en `include/config.h`).

**Estructura del repositorio**
- `platformio.ini` — Configuración de PlatformIO (env: `esp32`, LittleFS, dependencias).
- `include/` — Cabeceras y configuración del proyecto (`config.h`, `Settings.h`, etc.).
- `lib/` — Librerías locales modularizadas:
  - `Comunication/` — Abstracción de WiFi/ESPNOW (`Communication.h`/`.cpp`).
  - `IrrigationController/` — Lógica del riego y programación.
  - `Relay/` — Clase para manejar relés.
- `src/` — Código fuente principal (`main.cpp`).
- `boards/partitions/` — Tablas de particiones para ESP32.
- `test/` — Información de tests (README).

Hardware y pines (por defecto)
- MCU: ESP32 (env `esp32dev` en `platformio.ini`).
- Sensor DHT11: GPIO 5 (`DHTPIN`).
- Relé Válvula Zona 1: GPIO 14 (`RELAY1_PIN`).
- Relé Válvula Zona 2: GPIO 13 (`RELAY2_PIN`).
- Relé LED Estado Sistema: GPIO 12 (`RELAY3_PIN`).
- RTC DS3231: SDA GPIO 21, SCL GPIO 22.

Estos pines están definidos en `src/main.cpp`.

Configuración
- Credenciales WiFi y valores de Blynk/NTP se encuentran en `include/config.h`.
  - Cambia `WIFI_SSID` y `WIFI_PASSWORD` por tus datos.
  - Actualiza `BLYNK_TEMPLATE_ID` / `BLYNK_TEMPLATE_NAME` según tu plantilla Blynk.
- Recomendación: NO subir credenciales sensibles a un repositorio público. Considera usar un archivo de configuración local ignorado por git.

Virtual Pins de Blynk (uso)
- `V0` — Temperatura (float) enviada periódicamente.
- `V1` — Humedad (float) enviada periódicamente.
- `V2` — Toggle modo manual (1 = manual ON, 0 = manual OFF).
- `V3` — Widget `TimeInput` para programaciones: el código procesa start/stop, sunrise/sunset y días activos.
- `V4` — Terminal / estado / recepción de caracteres desde `Communication`.
- Terminal virtual: Widget Terminal asignado a `V5` (usado para mensajes de log)

Compilación y flasheo (PlatformIO)
- Requisitos: PlatformIO (VS Code extension) y toolchain para ESP32.
- Comandos:

```bash
# Compilar
platformio run

# Subir al dispositivo (asegúrate de seleccionar el puerto correcto)
platformio run --target upload

# Monitor serie
platformio device monitor --baud 115200
```

Sistema de archivos
- `board_build.filesystem = littlefs` en `platformio.ini` — el proyecto está configurado para usar LittleFS.
- Si necesitas cargar archivos a LittleFS, usa las herramientas de PlatformIO para subir el sistema de archivos.

Detalles de software relevantes
- Dependencias (declaradas en `platformio.ini`):
  - `Blynk` (para control remoto y dashboard)
  - `Adafruit DHT sensor library` (DHT11)
  - `RTClib` (DS3231)
  - `Adafruit BusIO` y `Adafruit Unified Sensor`
- El código central está en `src/main.cpp`: inicializa periféricos, gestiona timers (BlynkTimer) y registra callbacks Blynk.
- `IrrigationController` encapsula la lógica de programación y ejecución del riego. Revisa `lib/IrrigationController/IrrigationController.h` y `.cpp` para detalles sobre horarios, sunrise/sunset y control de relés.

Cómo contribuir / extender
- Añadir más relés: `IrrigationController` soporta hasta `MAX_IRRIGATION_RELAYS` (ver `IrrigationController.h`). Si necesitas más, aumenta el valor y revisa la lógica de almacenamiento.
- Integrar otros sensores: crea nuevas clases en `lib/` y regístalas en `src/main.cpp`.
- Mejorar seguridad: mover credenciales fuera de `config.h` e implementar un método más seguro de gestión de secretos.

Mantenimiento y pruebas
- Prueba localmente con `platformio run` y `platformio device monitor`.
- Para depurar, usa el `WidgetTerminal` de Blynk (V5) y la salida serie (`Serial`) a 115200.

Archivos clave para revisar
- [platformio.ini](platformio.ini)
- [src/main.cpp](src/main.cpp)
- [include/config.h](include/config.h)
- [lib/IrrigationController/IrrigationController.h](lib/IrrigationController/IrrigationController.h)
- [lib/Relay/Relay.h](lib/Relay/Relay.h)
- [lib/Comunication/Communication.h](lib/Comunication/Communication.h)