# Dustnet Mapper

This is the firmware for the dustnet mapper. It has been build especially for the [Dustmap](https://dustmap.org) project. It uses [LoRaWAN](https://lora-alliance.org/about-lorawan) and [The Things Network](https://www.thethingsnetwork.org) (short: TTN) and [TTNMapper](http://ttnmapper.org).
A more detailed description can be found on this [Netidee blog post](https://www.netidee.at/dustmap/neues-gateway-und-lorawan-mapper)(German).

<p align="center">
  <a href="https://www.dustmap.org/">
    <img alt="Dustnet LoRa Mapper" title="Dustnet LoRa Mapper" src="https://www.netidee.at/sites/default/files/styles/back/public/2018-09/ttnmapper.jpg" width="600">
  </a>
</p>

*This documentation is a work-in-progress.*

This open source project has been generously supported by [![netidee](https://www.netidee.at/themes/Netidee/images/netidee-logo-color.svg)](https://www.netidee.at).

### Configuration

The configuration can be found in the `config.h` file. Please be advised that we do not recommend to change anything in here. Except you know exactly, what you are doing. Than you will properly be fine.

| Key | Description |
| --- | --- |
| `APPEUI` | The Things Network Application EUI. See `The Things Network Integration` for more information. |
| `DEVEUI` | The Things Network Device EUI. See `The Things Network Integration` for more information. |
| `APPKEY` | The Things Network Application Key. See `The Things Network Integration` for more information. |
| `MAX_JOINING_DURATION` |  |
| `DEVICE_NAME` | Name of the BLE device. |
| `SERVICE_UUID` |  |
| `CHARACTERISTIC_UUID` |  |
| `GPS_CHARACTERISTIC_UUID` |  |
| `LMIC_PIN_NSS` |  |
| `LMIC_PIN_DIO` |  |

To compile this code, you need to have the [Arduino-LMIC library](https://github.com/matthijskooijman/arduino-lmic) by [Matthijs Kooijman](https://github.com/matthijskooijman) installed. We recommend to check it out directly from Github as using "Manage Libraries..." in the Arduino IDE will most likely fail.

## Licence

Copyright 2018 Dustmap.org. All rights reserved.

This work is licensed under the terms of the MIT license.
For a copy, see <https://opensource.org/licenses/MIT>.
