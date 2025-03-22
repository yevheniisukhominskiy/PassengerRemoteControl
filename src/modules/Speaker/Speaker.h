#ifndef SPEAKER_H
#define SPEAKER_H

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_TPA2016D2_Arduino_Library.h>
#include <EncButton.h>
#include <EEManager.h>

class Speaker
{
private:
    Button button_minus;
    Button button_plus;
    TPA2016D2 amp;
    EEManager eeprom;

    const byte volumeLevels[6] = {0, 6, 12, 18, 24, 30};
    int volume_mode = 3;
    bool alert;
    
    void leds(byte level);
    void volume(byte index);
    void manegment();
public:
    Speaker();
    void begin();
    void processing();
};



#endif