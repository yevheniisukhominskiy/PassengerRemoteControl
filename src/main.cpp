#include <Arduino.h>
#include <configs/pins.h>

#include <modules/Lightning/Lightning.h>
#include <modules/Restroom/Restroom.h>
#include <modules/Security/Security.h>
#include <modules/Speaker/Speaker.h>
#include <data/CanManager.h>


Restroom restroom;
Security security;
Lightning lighting(&security);
Speaker speaker;
CanManager canManager;

void setup() 
{
    Serial.begin(115200);
    Wire.begin();
    speaker.begin();
    canManager.begin();
}

void loop() 
{
    canManager.processing();
    restroom.processing();
    security.processing();
    lighting.processing();
    speaker.processing();
}


