# TP DAIoT 🛜

Este proyecto tiene como objetivo la implementación de un prototipo de tag o caravana para ganado que permita el seguimiento de los animales y establezca áreas permitidas para su circulación.

## Descripción del proyecto

El proyecto final se enfoca en desarrollar un sistema de seguimiento y control para el ganado mediante el uso de tecnología LoRaWAN y GPS. El objetivo principal es implementar etiquetas electrónicas (tags) que se colocarán en los animales y que permitirán realizar un seguimiento preciso de su ubicación. Además, se establecerán áreas permitidas para la circulación del ganado, lo que garantizará su seguridad y control.

Para el desarrollo de este proyecto de Desarrollo de Aplicaciones de Internet de las Cosas (DAIoT), se utilizará el microcontrolador [ESP32 LoRa Heltec v2](https://resource.heltec.cn/download/Manual%20Old/WiFi%20Lora32Manual.pdf). Las coordenadas en esta versión se generan aleatoriamente mediante el código. Estos dispositivos estarán conectados a un [Gateway RAK7243 WisGate Developer D3](https://docs.rakwireless.com/Product-Categories/WisGate/RAK7243C/Datasheet/#hardware). Este Gateway se registrará en [The Things Network](https://www.thethingsnetwork.org/), lo que permitirá la comunicación a través de la red LoRaWAN.

Luego, mediante la vinculación con la plataforma [All Thing Talk](https://www.allthingstalk.com/), se mostrará la ubicación de los animales en un mapa, y el usuario final podrá configurar las áreas permitidas para la circulación del ganado. El sistema generará alertas en caso de que un animal se desplace fuera de las zonas establecidas.


![diagram](/images/diagram.png)

## Requisitos previos

1. Este código debe ejecutarse desde  `/esp/src`
  
2. Antes de utilizar este código, asegúrate de configurar la frecuencia LoRaWAN y los ajustes del chip de radio utilizando `idf.py menuconfig`. En el menú, navega a `Components` -> `The Things Network` para configurar los valores apropiados.

![sdk](/images/sdk.png)

3. Levantar Gateway LoraWAN: Al basarse en un dispositivo Raspberry Pi 3, funciona con el sistema operativo Raspbian, que a su vez se basa en la distribución de Linux Debian y permite ejecutar la configuración del dispositivo por consola a través de SSH (Secure Shell), esto es una característica común de los dispositivos que funcionan con sistema operativo Linux. El procedimiento de configuración se encuentra en el [sitio del fabricante](https://docs.rakwireless.com/Product-Categories/WisGate/RAK7243/Quickstart/).
Los puntos principales de la configuración son:
- Conexión Wi-Fi a la red disponible.
- Configuración del concentrador LoRa apuntando al servidor TTS.
- Plan de frecuencia, para este caso AU915.

4. Deberás configurar los pines dentro de `main.c` dependiendo del dispositivo que tengas.
 
```cpp
// Pins and other resources
#define TTN_SPI_HOST      SPI2_HOST
#define TTN_SPI_DMA_CHAN  SPI_DMA_DISABLED
#define TTN_PIN_SPI_SCLK  5
#define TTN_PIN_SPI_MOSI  27
#define TTN_PIN_SPI_MISO  19
#define TTN_PIN_NSS       18
#define TTN_PIN_RXTX      TTN_NOT_CONNECTED
#define TTN_PIN_RST       14
#define TTN_PIN_DIO0      26
#define TTN_PIN_DIO1      35
```

5. Además, necesitas obtener la siguiente información de activación de la consola de TTN para tu dispositivo específico:
- AppEUI (a veces llamado JoinEUI)
- DevEUI
- AppKey

En [The Things Network](https://www.thethingsnetwork.org/) deberás crear una nueva aplicación `Aplications` -> `New Aplication`. [Mas info](https://www.thethingsindustries.com/docs/integrations/adding-applications/).

![app](/images/app.png)

Dentro de la carpeta `include` del proyecto tendrás crear un archivo `keys.h` para insertar la información anterior. 

**Nota:** Recuerda no subirlo a tu repositorio, para esto deberas agregarlo al `.gitignore`.

Ejemplo código: 

```cpp
extern const char *appEui;
extern const char *devEui;
extern const char *appKey;

// AppEUI (sometimes called JoinEUI)
const char *appEui = "????????????????";
// DevEUI
const char *devEui = "????????????????";
// AppKey
const char *appKey = "????????????????????????????????"; 
```
**Nota:** https://www.thethingsnetwork.org/docs/lorawan/security/


## Uso

1. Asegúrate de configurar el proyecto para tu ESP32 y de establecer la configuración de LoRaWAN como se describe en "Requisitos previos".

2. El código genera coordenadas aleatorias en la zona de la ciudad de Buenos Aires y Gran Buenos Aires y las envía a [The Things Network](https://www.thethingsnetwork.org/). Puedes ajustar los límites o modificar los datos según las necesidades de tu aplicación. En la version final de este sistema se enviarán los datos obtenidos de un sensor GPS.

3. Compila y carga el código en tu ESP32.

4. Monitoriza la salida del programa para ver el resultado de la comunicación LoRaWAN.

5. El ESP32 entrará en modo de hibernación y se despertará a intervalos regulares especificados por `TX_INTERVAL` para enviar nuevos mensajes. En este caso esta seteado para que envie mensajes cada 30s.

6. En [The Things Network](https://www.thethingsnetwork.org/) ve a `Integrations` -> `Webhooks`  y  vincula con [All Thing Talk](https://www.allthingstalk.com/). 

7. En [All Thing Talk](https://www.allthingstalk.com/) configura un nuevo proyecto, y luego en `Devices` -> `New Devices` se selecciona el tipo de dispositivo a utilizar, y se configura un `Asset` como se muestra en la imagen:

![att_newdevices](/images/att_newdevices.png)

8. En `Settings` -> `Payload Formats`, deberás parsear el contenido del [payload recibido](https://docs.allthingstalk.com/developers/data/custom-payload-conversion/). Para el caso, se usa la opción de ABCL y se arma el siguiente código:

```json
{
  "sense": [
    {
      "asset": "test-01-gps",
      "value": {
        "latitude": {
          "byte": 0,
          "bytelength": 4,
          "type": "integer",
          "calculation": "val/10000"
        },
        "longitude": {
          "byte": 4,
          "bytelength": 4,
          "type": "integer",
          "calculation": "val/10000"
        }
      }
    }
  ]
}
```
![payload_format](/images/payload_format.png)

Mas info de los puntos 6, 7 y 8 en [All Things Docs](https://docs.allthingstalk.com/networks/use-the-things-network-v3/)

![device](/images/device.png)

9. Dentro de [All Thing Talk](https://www.allthingstalk.com/) utiliza Geofence que te permite definir el límite geográfico para detectar cuándo los objetos entran, salen o permanecen dentro de él. Puedes crear múltiples límites y hacer seguimiento de varios dispositivos. En el proyecto se define el límite de la ciudad de Buenos Aires, y se establecen alarmas para cuando el dispositivo salga fuera del área, mediante push en la aplicación mobile, mediante la web y por medio de emails.

![geofence](/images/geofence.png)

10. Para la realización del dashboard donde se puede visualizar el `Tracking` del dispositivo utiliza [Pinboard](https://www.allthingstalk.com/faq/pinboard-management-in-also-iot-platform) dentro de [All Thing Talk](https://www.allthingstalk.com/). 

![pinboards](/images/pinboards.png)

![tracking](/images/tracking.png)

12. Puedes ver las `Alertas` en tu celular si utilizas la app de [All Thing Talk](https://www.allthingstalk.com/) y configuras el/los dispositivo/s a visualizar. 

![notifications](/images/notifications.jpg)

11. En [The Things Network](https://www.thethingsnetwork.org/) cuentas con `Storage` de la información generada.

![storage](/images/storage.png)

Con un codigo de `Python` como el del ejemplo que dejo a continuación se puede consultar el `JSON` de la base de datos:

```python

import requests
import json


# Definir la URL base de la API de TTN
base_url = 'https://nam1.cloud.thethings.network/api/v3/as/applications/muquitta01/packages/storage'

# Reemplaza '<API_KEY>' con tu token de autenticación
headers = {'Authorization': 'Bearer <TOKEN>'}

# Ejemplo de una solicitud GET para obtener información de un dispositivo
response = requests.get(f'{base_url}/uplink_message', headers=headers)

# Verificar si la solicitud fue exitosa
if response.status_code == 200:
    try:
        data = response.json()
        print(data)
    except json.JSONDecodeError as e:
        print(f'Error al decodificar JSON: {e}')
else:
    print(f'Error {response.status_code}: {response.text}')
```
Ejemplo de comando ejecutado desde la consola de Linux:

```shell
/src$ python ttn_api.py
```

Extracto del archivo `.JSON` en la base de datos de `TTN` para un mensaje de uplink:

```json
        "decoded_payload": {
          "latitude": -34.6165,
          "longitude": -58.4686
        },
        "rx_metadata": [
          {
            "gateway_ids": {
              "gateway_id": "rak7243gw",
              "eui": "XXXXXXXXXXXXXXX"
            },
            "timestamp": 1191794099,
            "rssi": -24,
            "channel_rssi": -24,
            "snr": 9.2,
            "location": {
              "latitude": -34.625527954139,
              "longitude": -58.4864767150376,
              "source": "SOURCE_REGISTRY"
            },
            "uplink_token": "XXXXXXXXXXXXXXXXXXXXXXXXXXXX",
            "channel_index": 3,
            "received_at": "2023-10-18T01:52:54.067528982Z"
```


El período de retención de datos es de 24 horas. Se puede cambiar de plan para tener otros servicios, mayor tiempo de retención y SLA.

Consulta [Storage Integration API](https://www.thethingsindustries.com/docs/reference/api/storage_integration/) para mas info.

## Modificado y adaptado por

- [Ignacio Hernandorena](https://github.com/nachohernandorena/)

## Placas Soportadas

Se admiten todas las placas con chips Semtech SX127x, RFM9x y compatibles. Esto incluye placas de ttgo, Heltec y HopeRF. Para muchas de ellas, la [Configuración de Pines](https://github.com/manuelbl/ttn-esp32/wiki/Boards-and-Pins) se describe en detalle.

### Documentación de la API

Consulta la [Documentación de la API](https://codecrete.net/ttn-esp32/) para obtener información sobre la API en C y C++.

### Licencia para el proyecto de conexión del ESP con TTN

Este proyecto está bajo la Licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

### La conexión a ttn esta basado en el proyecto: ttn-esp32

TTN-ESP32 se basa en la biblioteca [ttn-esp32](https://github.com/manuelbl/ttn-esp32), que proporciona comunicación LoRaWAN con The Things Network. La biblioteca original incluye características como OTAA, mensajes de enlace ascendente y descendente, ahorro de EUI y clave en memoria no volátil, entre otras.

### Más información sobre la conexion del ESP a TTN

Puedes encontrar más información en las [páginas de la Wiki](https://github.com/manuelbl/ttn-esp32/wiki).
