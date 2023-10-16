# TP DAIoT 📡

[![Licencia MIT](https://img.shields.io/badge/Licencia-MIT-blue.svg)](https://opensource.org/licenses/MIT)

El presente proyecto envía mensajes a través de LoRaWAN a The Things Network (TTN) utilizando el microcontrolador ESP32 y el módulo de radio SX127x. 

## Requisitos previos

Antes de usar este código, asegúrate de configurar la frecuencia LoRaWAN y los ajustes del chip de radio utilizando `idf.py menuconfig`. En el menú, navega a `Components` -> `The Things Network` para configurar los valores apropiados.

También necesitas obtener la siguiente información de activación de la consola de TTN para tu dispositivo específico:
- AppEUI (a veces llamado JoinEUI)
- DevEUI
- AppKey

## Uso

1. Asegúrate de configurar el proyecto para tu ESP32 y de establecer la configuración de LoRaWAN como se describe en "Requisitos previos".

2. El código genera coordenadas aleatorias dentro de los límites de Buenos Aires y las envía a TTN. Puedes ajustar los límites o modificar los datos según las necesidades de tu aplicación.

3. Compila y carga el código en tu ESP32.

4. Monitoriza la salida del programa para ver el resultado de la comunicación LoRaWAN.

5. El ESP32 entrará en modo de hibernación y se despertará a intervalos regulares especificados por `TX_INTERVAL` para enviar nuevos mensajes.

## Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

## Modificado y adaptado por

- [Ignacio Hernandorena](https://github.com/nachohernandorena/)

## Basado en ttn-esp32

TTN-ESP32 se basa en la biblioteca [ttn-esp32](https://github.com/manuelbl/ttn-esp32), que proporciona comunicación LoRaWAN con The Things Network. La biblioteca original incluye características como OTAA, mensajes de enlace ascendente y descendente, ahorro de EUI y clave en memoria no volátil, entre otras.

## Características Principales (Versión 4.x)

- Soporte para modo de hibernación y apagado (ver [Hibernación y Apagado](https://github.com/manuelbl/ttn-esp32/wiki/Deep-Sleep-and-Power-Off) en la Wiki).
- Compatibilidad verificada con ESP-IDF v4.3 y 5.0.
- Actualización de la biblioteca subyacente mcci-catena/arduino-lmic a v4.2.0-1.
- API en C.
- Soporte para subbandas.
- Se eliminó el soporte para compilación con *Makefile*.

## Comenzar

Sigue la guía detallada [Getting Started](https://github.com/manuelbl/ttn-esp32/wiki/Get-Started) en la Wiki para comenzar.

## Placas Soportadas

Se admiten todas las placas con chips Semtech SX127x, RFM9x y compatibles. Esto incluye placas de ttgo, Heltec y HopeRF. Para muchas de ellas, la [Configuración de Pines](https://github.com/manuelbl/ttn-esp32/wiki/Boards-and-Pins) se describe en detalle.

## Documentación de la API

Consulta la [Documentación de la API](https://codecrete.net/ttn-esp32/) para obtener información sobre la API en C y C++.

## Más información

Puedes encontrar más información en las [páginas de la Wiki](https://github.com/manuelbl/ttn-esp32/wiki).
