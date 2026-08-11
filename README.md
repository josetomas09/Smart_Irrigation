# Smart Irrigation System

![ESP32](https://img.shields.io/badge/ESP--32-blue?style=flat-square&logo=espressif)
![PlatformIO](https://img.shields.io/badge/PlatformIO-white?style=flat-square&logo=platformio)
![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
![Blynk](https://img.shields.io/badge/Blynk-IoT-11C281?style=flat-square)

Sistema de riego inteligente automatizado basado en el microcontrolador **ESP32** utilizando el framework de Arduino bajo el entorno **PlatformIO**. El sistema se conecta a la plataforma **Blynk** para permitir el monitoreo y control remoto via app movil o dashboard (PC).

### Funcionalidades Actuales
*   **Control de Riego:** Activación del riego mediante horarios preestablecidos o de forma manual a demanda.
*   **Monitoreo Ambiental:** Lectura y visualización en tiempo real de los niveles de temperatura y humedad.

### Próximamente (Roadmap)
*   **Automatización Dinámica:** Activación inteligente del riego basada en valores o umbrales específicos de temperatura y/o humedad.
*   **Iluminación (Opcional):** Manejo de luces (220V) a través de módulos de relé.
*   **Monitoreo de porton/puerta Garaje:** Lectura del estado de un portón de garaje, comunicándose con un segundo ESP32 a través del protocolo ESP-NOW.

## Características Principales
*   **Conectividad IoT:** Integración completa con Blynk (Wi-Fi) para control desde un dispositivo móvil o panel web.
*   **Control Modular:** Librerías propias orientadas a objetos para la abstracción de componentes (`Relay`, `IrrigationController`, `Comunication`).
*   **Actualizaciones Inalámbricas:** Estructura preparada para integrar actualizaciones OTA (Over-The-Air).

## Estructura del Proyecto

El proyecto está organizado de manera modular para facilitar el mantenimiento, la lectura y la escalabilidad del código:

```text
├── include/                    # Configuraciones globales y utilidades de Blynk (BlynkEdgent, SysUtils, etc.)
├── lib/                        # Librerías desarrolladas a medida
│   ├── Comunication/           # Gestión de conexión Wi-Fi, OTA y Blynk
│   ├── IrrigationController/   # Lógica central, temporizadores y reglas del sistema de riego
│   └── Relay/                  # Abstracción segura para el manejo de actuadores (bombas/electroválvulas)
├── src/                        # Código fuente principal (main.cpp)
└── platformio.ini              # Configuración del entorno, dependencias y placa objetivo
```

## Requisitos de Hardware
*   Placa de desarrollo ESP32 (probado con ESP32 NodeMCU DevKit V1).
*   Módulo de Relés (ej. 5V, optoacoplado).
*   Bomba de agua o electroválvula (12V/24V/220V según la necesidad).
*   Sensores de humedad (opcional pero recomendado).

## Instalación y Configuración

1.  **Clonar el repositorio:**
    ```bash
    git clone https://github.com/josetomas09/Smart_Irrigation.git
    ```
2.  **Abrir con PlatformIO:**
    Abre la carpeta del proyecto en VS Code con la extensión PlatformIO instalada. El archivo `platformio.ini` descargará las dependencias automáticamente.
3.  **Configurar credenciales:**
    Renombra el archivo `include/config.h(example)` a `include/config.h`. Edita el archivo para incluir tus credenciales de red Wi-Fi y el *Auth Token* provisto por tu proyecto de Blynk.
4.  **Compilar y Subir:**
    Conecta el ESP32 por USB y ejecuta las tareas *Build* y *Upload* desde PlatformIO.

## Documentación de Librerías Internas
Para fomentar la reutilización de código, cada módulo principal cuenta con su propia documentación detallada:
*   [`Comunication`](lib/Comunication/README.md) - Gestiona la conexión inicial vía Wi-Fi para enlazar con Blynk.
*   [`IrrigationController`](lib/IrrigationController/README.md) - Define la lógica de control del riego.
*   [`Relay`](lib/Relay/README.md) - Abstracción de bajo nivel para el manejo seguro de los pines GPIO y estados de relé.

## 📄 License
This project is licensed under the [MIT](LICENSE) License.
