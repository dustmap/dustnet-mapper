/* Copyright 2018 Dustmap.org. All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

// Device EUI, LSB
// Must be unique per application
#define DEVEUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

// Application EUI, LSB
// Should end with 0xD5, 0xB3, 0x70 for TTN
#define APPEUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0xD5, 0xB3, 0x70 }

// App Key, MSB
#define APPKEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

#define MAX_JOINING_DURATION 25

// HopeRF RFM95W pin mapping
// NSS & DIO0, DIO1, DIO2
// See https://ictoblog.nl/2018/01/13/my-chinese-esp32-sx1276-board-dht11-connected-to-the-things-network-1
#define LMIC_PIN_NSS 18
#define LMIC_PIN_DIO { 26, 33, 32 }

#define DEVICE_NAME "dustnet-mapper-000000"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define GPS_CHARACTERISTIC_UUID "911938e8-9bb8-11e8-98d0-529269fb1459"
