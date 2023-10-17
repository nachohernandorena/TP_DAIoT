/*******************************************************************************
 * 
 * ttn-esp32 - The Things Network device library for ESP-IDF / SX127x
 * 
 * Licensed under MIT License
 * https://opensource.org/licenses/MIT
 *
 *******************************************************************************/

#include "freertos/FreeRTOS.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ttn.h"
#include "keys.h"

// NOTE:
// The LoRaWAN frequency and the radio chip must be configured by running 'idf.py menuconfig'.
// Go to Components / The Things Network, select the appropriate values and save.

float latitude; 
float longitude; 

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

#define TX_INTERVAL 30 // in sec

// This counter value is retained (in RTC memory) during deep sleep
RTC_DATA_ATTR int counter_in_rtc_mem;


void messageReceived(const uint8_t* message, size_t length, ttn_port_t port)
{
    printf("Message of %d bytes received on port %d:", length, port);
    for (int i = 0; i < length; i++)
        printf(" %02x", message[i]);
    printf("\n");
}

void app_main(void)
{
    esp_err_t err;
    // Initialize the GPIO ISR handler service
    err = gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    ESP_ERROR_CHECK(err);
    
    // Initialize the NVS (non-volatile storage) for saving and restoring the keys
    err = nvs_flash_init();
    ESP_ERROR_CHECK(err);

    // Initialize SPI bus
    spi_bus_config_t spi_bus_config = {
        .miso_io_num = TTN_PIN_SPI_MISO,
        .mosi_io_num = TTN_PIN_SPI_MOSI,
        .sclk_io_num = TTN_PIN_SPI_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1
    }; 
    err = spi_bus_initialize(TTN_SPI_HOST, &spi_bus_config, TTN_SPI_DMA_CHAN);
    ESP_ERROR_CHECK(err);

    // Initialize TTN
    ttn_init();

    // Configure the SX127x pins
    ttn_configure_pins(TTN_SPI_HOST, TTN_PIN_NSS, TTN_PIN_RXTX, TTN_PIN_RST, TTN_PIN_DIO0, TTN_PIN_DIO1);

    // The below line can be commented after the first run as the data is saved in NVS
    ttn_provision(devEui, appEui, appKey);

    // Register callback for received messages
    ttn_on_message(messageReceived);

    ttn_set_adr_enabled(false);
    ttn_set_data_rate(TTN_DR_US915_SF7);
    ttn_set_max_tx_pow(14);

    if (ttn_resume_after_deep_sleep()) {
        printf("Resumed from deep sleep.\n");
    
    } else {

        printf("Joining...\n");
        if (ttn_join())
        {
            printf("Joined.\n");
        }
        else
        {
            printf("Join failed. Goodbye\n");
            return;
        }
    }


    float randomFloat(float min, float max) {
        return min + ((float)rand() / RAND_MAX) * (max - min);
    }

    // Function to generate random coordinates within the limits of Buenos Aires
    void generateRandomLocation(float *latitude, float *longitude) {
        // Límites para Buenos Aires
        float minLatitude = -34.7050;
        float maxLatitude = -34.5250;
        float minLongitude = -58.5300;
        float maxLongitude = -58.3350;

        // Generate latitude and longitude randomly within specified limits
        *latitude = randomFloat(minLatitude, maxLatitude);
        *longitude = randomFloat(minLongitude, maxLongitude);
    }


    printf("Sending message...\n");
    counter_in_rtc_mem++;
    srand(time(NULL));
    generateRandomLocation(&latitude, &longitude);
    char message[8];
    int counter_in_rtc_mem = 0;

    int32_t lat = latitude*10000;
    int32_t lon = longitude*10000;

    message[0] = (lat >> 24) & 0xFF;
    message[1] = (lat >> 16) & 0xFF;
    message[2] = (lat >> 8) & 0xFF;
    message[3] = lat & 0xFF;
    message[4] = (lon >> 24) & 0xFF;
    message[5] = (lon >> 16) & 0xFF;
    message[6] = (lon >> 8) & 0xFF;
    message[7] = lon & 0xFF;

    //snprintf(message, sizeof(message), "%.4f%.4f", latitude, longitude);
    ttn_response_code_t res = ttn_transmit_message((uint8_t *)message, strlen(message), 1, false);
    // Print the generated values ​​and the message
    printf("Latitude: %.4f\n", latitude);
    printf("Longitude: %.4f\n", longitude);
    // Check the result of sending to TTN
    if (res == TTN_SUCCESSFUL_TRANSMISSION) {
        printf("Message sent to TTN.\n");
    } else {
        printf("Transmission to TTN failed.\n");
    } 


    // Wait until TTN communication is idle and save state
    ttn_wait_for_idle();
    ttn_prepare_for_deep_sleep();

    // Schedule wake up
    esp_sleep_enable_timer_wakeup(TX_INTERVAL * 1000000LL);

    printf("Going to deep sleep...\n");
    esp_deep_sleep_start();
}
