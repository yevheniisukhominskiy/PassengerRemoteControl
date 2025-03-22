#include <modules/Restroom/Restroom.h>
#include <configs/pins.h>

#include <Arduino.h>
#include <data/CanManager.h>

Restroom::Restroom() {
    pinMode(LED_RESTROOM1_RED, OUTPUT);
    pinMode(LED_RESTROOM1_GREEN, OUTPUT);
    pinMode(LED_RESTROOM2_GREEN, OUTPUT);
    pinMode(LED_RESTROOM2_RED, OUTPUT);

    pinMode(SENSOR_RESTROOM1, INPUT_PULLUP);
    pinMode(SENSOR_RESTROOM2, INPUT_PULLUP);
}

void Restroom::processing() {
    update_indicators();
}

void Restroom::update_indicators() {
    bool sensor1 = !digitalRead(SENSOR_RESTROOM1);
    bool sensor2 = !digitalRead(SENSOR_RESTROOM2);

    if (sensor1) {
        digitalWrite(LED_RESTROOM1_RED, HIGH);
        digitalWrite(LED_RESTROOM1_GREEN, LOW);
    } else {
        digitalWrite(LED_RESTROOM1_RED, LOW);
        digitalWrite(LED_RESTROOM1_GREEN, HIGH);
    }

    if (sensor2) {
        digitalWrite(LED_RESTROOM2_RED, HIGH);
        digitalWrite(LED_RESTROOM2_GREEN, LOW);
    } else {
        digitalWrite(LED_RESTROOM2_RED, LOW);
        digitalWrite(LED_RESTROOM2_GREEN, HIGH);
    }
}
