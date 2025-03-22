#ifndef LIGHTNING_H
#define LIGHTNING_H

#include <Arduino.h>
#include <EncButton.h>
#include <modules/Security/Security.h>

enum LightingMode {
    OFF_MODE,  
    HIGH_MODE,
    MEDIUM_MODE,
    LOW_MODE
};

class Lightning {
public:
    Lightning(Security* sec);
    void processing();
private:
    LightingMode current_mode;
    Security* security;
    bool dim_forced;
    bool dim_was_activated_by_alarm = false; // Запоминаем, включилось ли "Тьмяно" из-за тревоги

    Button button_high;
    Button button_medium;
    Button button_low;
    Button button_off;

    void button_managment();
    void set_lighting(uint8_t high, uint8_t medium, uint8_t low);
    void set_dimforced(bool state);
    void check_securitymode();
};

#endif