#include <modules/Speaker/Speaker.h>
#include <configs/pins.h>

#include <Arduino.h>
#include <configs/settings.h>
#include <EncButton.h>

Speaker::Speaker() 
    :   button_minus(BUTTON_SPEAKER_MINUS),
        button_plus(BUTTON_SPEAKER_PLUS),
        amp(),
        eeprom(volume_mode, SPEAKER_SAVER_TIME)
{  
    pinMode(INPUT_SPEAKER_ALERT, INPUT_PULLUP);
    for (uint8_t i = 0; i < 5; i++) {
        pinMode(LEDS[i], OUTPUT);
        digitalWrite(LEDS[i], LOW);
    }
}

void Speaker::begin() {
    amp.begin();

    amp.disableLimiter();
    amp.disableNoiseGate();
    amp.writeRelease(1);
    amp.writeAttack(1);

    eeprom.begin(0, 'v');
    volume(volume_mode);
}

void Speaker::processing() {
    manegment();
}

void Speaker::leds(byte level) {
    for (uint8_t i = 0; i < 5; i++) {
        digitalWrite(LEDS[i], (i < level) ? HIGH : LOW);
    }
}

void Speaker::volume(byte index) {
    if (index == 0) {
        amp.enableShutdown();
        leds(0);
    } else {
        amp.disableShutdown();
        amp.writeFixedGain(volumeLevels[index - 1]);
        leds(index);
    }
}

void Speaker::manegment() {
    button_minus.tick();
    button_plus.tick();
    eeprom.tick();

    if (digitalRead(INPUT_SPEAKER_ALERT) == LOW) {
        alert = true;
        volume(5);
    } else {
        alert = false;
        volume(volume_mode);
    }

    if (!alert) {
        if (button_minus.press() && volume_mode > 0) {
            volume_mode--;
            volume(volume_mode);
            eeprom.update();
        }

        if (button_plus.press() && volume_mode < 5) {
            volume_mode++;
            volume(volume_mode);
            eeprom.update();
        }
    }
}

