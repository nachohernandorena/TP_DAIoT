# TP DAIoT 🛜

[![Licencia MIT](https://img.shields.io/badge/Licencia-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Este proyecto tiene como objetivo la implementación de un prototipo de tag o caravana para ganado que permita el seguimiento de los animales y establezca áreas permitidas para su circulación.

## Descripción del proyecto

El proyecto final se enfoca en desarrollar un sistema de seguimiento y control para el ganado mediante el uso de tecnología LoRaWAN y GPS. El objetivo principal es implementar etiquetas electrónicas (tags) que se colocarán en los animales y que permitirán realizar un seguimiento preciso de su ubicación. Además, se establecerán áreas permitidas para la circulación del ganado, lo que garantizará su seguridad y control.

Para el desarrollo de este proyecto de Dispositivos y Aplicaciones de Internet de las Cosas (DAIoT), se utilizará el microcontrolador [ESP32 LoRa Heltec v2](https://resource.heltec.cn/download/Manual%20Old/WiFi%20Lora32Manual.pdf), las coordenadas, en esta versión, se generan aleatoriamente mendiante el codigo. Estos dispositivos estarán conectados a un [Gateway RAK7243 WisGate Developer D3](https://docs.rakwireless.com/Product-Categories/WisGate/RAK7243C/Datasheet/#hardware). Este Gateway se registrará en [The Things Network](https://www.thethingsnetwork.org/), lo que permitirá la comunicación a través de la red LoRaWAN.

Luego, mediante la vinculación con la plataforma [All Thing Talk](https://www.allthingstalk.com/), se mostrará la ubicación de los animales en un mapa, y el usuario final podrá configurar las áreas permitidas para la circulación del ganado. El sistema generará alertas en caso de que un animal se desplace fuera de las zonas establecidas.



## Requisitos previos

Antes de utilizar este código, asegúrate de configurar la frecuencia LoRaWAN y los ajustes del chip de radio utilizando `idf.py menuconfig`. En el menú, navega a `Components` -> `The Things Network` para configurar los valores apropiados.

Además, necesitas obtener la siguiente información de activación de la consola de TTN para tu dispositivo específico:
- AppEUI (a veces llamado JoinEUI)
- DevEUI
- AppKey

Deberas crear un archivo `keys.h` para insertar la información anterior. Recuerda no subirlo a tu repositorio, para esto deberas agregarlo al `.gitignore`.
```cpp
// AppEUI (sometimes called JoinEUI)
const char *appEui = "????????????????";
// DevEUI
const char *devEui = "????????????????";
// AppKey
const char *appKey = "????????????????????????????????";
```

## Uso

1. Asegúrate de configurar el proyecto para tu ESP32 y de establecer la configuración de LoRaWAN como se describe en "Requisitos previos".

2. El código genera coordenadas aleatorias dentro de los límites de Buenos Aires y las envía a TTN. Puedes ajustar los límites o modificar los datos según las necesidades de tu aplicación.

3. Compila y carga el código en tu ESP32.

4. Monitoriza la salida del programa para ver el resultado de la comunicación LoRaWAN.

5. El ESP32 entrará en modo de hibernación y se despertará a intervalos regulares especificados por `TX_INTERVAL` para enviar nuevos mensajes. En este caso esta seteado para que envie mensajes cada 30s.

6. En [All Thing Talk](https://www.allthingstalk.com/) se configura un nuevo proyecto, y luego en `Devices` -> `The Things Network`.

2. El código genera coordenadas aleatorias dentro de los límites de Buenos Aires y las envía a TTN. Puedes ajustar los límites o modificar los datos según las necesidades de tu aplicación.

3. Compila y carga el código en tu ESP32.

4. Monitoriza la salida del programa para ver el resultado de la comunicación LoRaWAN.

5. El ESP32 entrará en modo de hibernación y se despertará a intervalos regulares especificados por `TX_INTERVAL` para enviar nuevos mensajes.




## Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

## Modificado y adaptado por

- [Ignacio Hernandorena](https://github.com/nachohernandorena/)

## Comenzar

Sigue la guía detallada [Getting Started](https://github.com/manuelbl/ttn-esp32/wiki/Get-Started) en la Wiki para comenzar.

## Placas Soportadas

Se admiten todas las placas con chips Semtech SX127x, RFM9x y compatibles. Esto incluye placas de ttgo, Heltec y HopeRF. Para muchas de ellas, la [Configuración de Pines](https://github.com/manuelbl/ttn-esp32/wiki/Boards-and-Pins) se describe en detalle.

## Documentación de la API

Consulta la [Documentación de la API](https://codecrete.net/ttn-esp32/) para obtener información sobre la API en C y C++.


### La conexión a ttn esta basado en el proyecto: ttn-esp32

TTN-ESP32 se basa en la biblioteca [ttn-esp32](https://github.com/manuelbl/ttn-esp32), que proporciona comunicación LoRaWAN con The Things Network. La biblioteca original incluye características como OTAA, mensajes de enlace ascendente y descendente, ahorro de EUI y clave en memoria no volátil, entre otras.

### Más información sobre la conexion del ESP a TTN

Puedes encontrar más información en las [páginas de la Wiki](https://github.com/manuelbl/ttn-esp32/wiki).
