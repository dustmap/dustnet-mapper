/* Copyright 2018 Dustmap.org. All rights reserved.
 *
 * This work is licensed under the terms of the MIT license.
 * For a copy, see <https://opensource.org/licenses/MIT>.
 */

#include <lmic.h>
#include <hal/hal.h>
#include <SSD1306.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include "config_000002.h"

#define STATUS_DISCONNECTED 0
#define STATUS_JOINING 1
#define STATUS_JOIN_TIMEOUT 2
#define STATUS_JOIN_FAILED 3
#define STATUS_CONNECTED 4
#define STATUS_PACKAGE_SENT 5

SSD1306 display(0x3c, 4, 15);

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

uint8_t status = STATUS_DISCONNECTED;
int sentCount = 0;
int lastContact = 0;
bool bleConnected = false;
uint16_t joining_duration = 0;

BLECharacteristic *pCharacteristic;
BLECharacteristic *gpsCharacteristic;


static osjob_t sendjob;

static u1_t devEUI[8] = DEVEUI;
void os_getDevEui (u1_t* buf) {
	memcpy(buf, devEUI, 8);
}

static const u1_t PROGMEM appEUI[8] = APPEUI;
void os_getArtEui (u1_t* buf) {
	memcpy_P(buf, appEUI, 8);
}

static const u1_t PROGMEM appKey[16] = APPKEY;
void os_getDevKey (u1_t* buf) {
	memcpy_P(buf, appKey, 16);
}

const lmic_pinmap lmic_pins = {
    .nss = LMIC_PIN_NSS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LMIC_UNUSED_PIN,
    .dio = LMIC_PIN_DIO,
};

void onEvent (ev_t ev) {
    switch(ev) {
        case EV_JOINING:
			status = STATUS_JOINING;
			joining_duration = 0;
            break;
        case EV_JOINED:
			status = STATUS_CONNECTED;
			LMIC_setDrTxpow(DR_SF7, 14);
            LMIC_setLinkCheckMode(0);
            break;
        case EV_JOIN_FAILED:
			status = STATUS_JOIN_FAILED;
            break;
        case EV_REJOIN_FAILED:
			status = STATUS_JOIN_FAILED;
            break;
        case EV_TXCOMPLETE:
			status = STATUS_PACKAGE_SENT;
			sentCount++;
			digitalWrite(25, LOW);
            break;
		case EV_LINK_DEAD:
			status = STATUS_DISCONNECTED;
			digitalWrite(25, LOW);
			break;
    }
}


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      bleConnected = true;
      sentCount = 0;
    };

    void onDisconnect(BLEServer* pServer) {
		status = STATUS_DISCONNECTED;
      bleConnected = false;

	  digitalWrite(25, LOW);
	  LMIC_reset();
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) {

	}
};

class gpsCallbacks: public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) {
		std::string rxValue = pCharacteristic->getValue();

		if (rxValue.length() > 0) {
			byte payload[rxValue.length()];

			Serial.print("gps_Callbacks Value: ");
			for (int i = 0; i < rxValue.length(); i++) {
				Serial.print(rxValue[i]);
				payload[i] = rxValue[i];
			}
			Serial.println();

			lastContact = 0;

			if (LMIC.opmode & OP_TXRXPEND) {
	        } else {
				digitalWrite(25, HIGH);
				LMIC_reset();
				LMIC_setTxData2(1, payload, sizeof(payload), 0);
	        }
		}
	}
};

void IRAM_ATTR onTimer() {
	portENTER_CRITICAL_ISR(&timerMux);
	lastContact++;

	if (status == STATUS_JOINING) {
		joining_duration++;
	}

	portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
	 Serial.begin(115200);

	pinMode(16,OUTPUT);
    pinMode(25,OUTPUT);

    digitalWrite(16, LOW);
    delay(50);
    digitalWrite(16, HIGH);

    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);

	delay(1000);

	timer = timerBegin(0, 80, true);
	timerAttachInterrupt(timer, &onTimer, true);
	timerAlarmWrite(timer, 1000000, true);
	timerAlarmEnable(timer);

	BLEDevice::init(DEVICE_NAME);
	BLEServer *pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());

	BLEService *pService = pServer->createService(SERVICE_UUID);
	pCharacteristic = pService->createCharacteristic(
		CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_NOTIFY
    );
	pCharacteristic->setValue("Disconnected");
	pCharacteristic->setCallbacks(new MyCallbacks());

	gpsCharacteristic = pService->createCharacteristic(
		GPS_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
		BLECharacteristic::PROPERTY_WRITE
    );
	gpsCharacteristic->setCallbacks(new gpsCallbacks());
	pService->start();

	BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->start();

	os_init();
	LMIC_reset();

	pCharacteristic->notify();

	LMIC.dn2Dr = DR_SF9;
}

void loop() {
    os_runloop_once();

	display.clear();
	display.setTextAlignment(TEXT_ALIGN_LEFT);
	display.drawString(0, 0, (String)DEVICE_NAME);
	switch (status) {
		case STATUS_DISCONNECTED:
			display.drawString(0, 10, "Status: Disconnected");
			pCharacteristic->setValue("Disconnected");
			break;
		case STATUS_JOINING:
			display.drawString(0, 10, "Status: Joining");
			pCharacteristic->setValue("Joining");
			break;
		case STATUS_JOIN_TIMEOUT:
			display.drawString(0, 10, "Status: Join Timeout");
			pCharacteristic->setValue("Join Timeout");
			break;
		case STATUS_JOIN_FAILED:
			display.drawString(0, 10, "Status: Join Failed");
			pCharacteristic->setValue("Join Failed");
			break;
		case STATUS_CONNECTED:
			display.drawString(0, 10, "Status: Connected");
			pCharacteristic->setValue("Connected");
			break;
		case STATUS_PACKAGE_SENT:
			display.drawString(0, 10, "Status: Package sent");
			pCharacteristic->setValue("Package sent");
			break;
	}

	if (sentCount > 0) {
		uint8_t seconds = lastContact % 60;
		uint8_t minutes = (lastContact / 60) % 60;
		uint8_t hours = lastContact / 3600;
		char str[12];

		sprintf( str, "%02u:%02u:%02u", hours, minutes, seconds);
		display.drawString(0, 30, "Last contact:  " + (String)str);
		display.drawString(0, 40, "Packages sent: " + (String)sentCount);
	}

	if (bleConnected) {
		display.setTextAlignment(TEXT_ALIGN_RIGHT);
		display.drawString(128, 0, "B");
	}

	display.display();

	if (joining_duration > MAX_JOINING_DURATION) {
		status = STATUS_JOIN_TIMEOUT;
		joining_duration = 0;
	}

	if (status == STATUS_PACKAGE_SENT) {
		status = STATUS_DISCONNECTED;
	}
}
