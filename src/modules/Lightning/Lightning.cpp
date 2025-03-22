#include <modules/Lightning/Lightning.h>
#include <configs/pins.h>

#include <Arduino.h>
#include <EncButton.h>
#include <modules/Security/Security.h>

Lightning::Lightning(Security* sec)
    :   security(sec), dim_forced(false),
        button_high(BUTTON_LIGHTING_HIGH), 
        button_medium(BUTTON_LIGHTING_MEDIUM), 
        button_low(BUTTON_LIGHTING_LOW), 
        button_off(BUTTON_LIGHTING_OFF),
        current_mode(OFF_MODE) {

    pinMode(LED_LIGHTING_HIGH, OUTPUT);
    pinMode(LED_LIGHTING_MEDIUM, OUTPUT);
    pinMode(LED_LIGHTING_LOW, OUTPUT);
}

void Lightning::processing() {
    check_securitymode();
    button_managment();
}

void Lightning::check_securitymode() {
    if (security->isDanger()) {  
        // Если до тревоги свет НЕ горел, включаем "Тьмяно" и запоминаем это
        if (!digitalRead(LED_LIGHTING_HIGH) && 
            !digitalRead(LED_LIGHTING_MEDIUM) && 
            !digitalRead(LED_LIGHTING_LOW)) {
            
            set_dimforced(true);
            dim_was_activated_by_alarm = true; // "Тьмяно" включилось именно из-за тревоги
        }
    } else {
        // Если "Тьмяно" включилось из-за тревоги, оставляем его гореть
        if (!dim_was_activated_by_alarm) {
            set_dimforced(false); // Восстанавливаем обычный режим
        }
    }
}


void Lightning::button_managment() {
    button_high.tick();
    button_medium.tick();
    button_low.tick();
    button_off.tick();
    
    if (button_high.press()) {
        set_lighting(HIGH, LOW, LOW);
        current_mode = HIGH_MODE;
    } else if (button_medium.press()) {
        set_lighting(LOW, HIGH, LOW);
        current_mode = MEDIUM_MODE;
    } else if (button_low.press()) {
        set_lighting(LOW, LOW, HIGH);
        current_mode = LOW_MODE;
    } else if (button_off.press() && !security->isDanger()) {
        set_lighting(LOW, LOW, LOW);
        current_mode = OFF_MODE;
    }
}

void Lightning::set_lighting(uint8_t high, uint8_t medium, uint8_t low) {
    digitalWrite(LED_LIGHTING_HIGH, high);
    digitalWrite(LED_LIGHTING_MEDIUM, medium);
    digitalWrite(LED_LIGHTING_LOW, low);
}

void Lightning::set_dimforced(bool state) {
    dim_forced = state;
    if (dim_forced) {
        set_lighting(LOW, LOW, HIGH);  // Включаем «Тьмяно»
    } else {
        // После тревоги восстанавливаем предыдущий режим
        if (current_mode == HIGH_MODE) {
            set_lighting(HIGH, LOW, LOW);
        } else if (current_mode == MEDIUM_MODE) {
            set_lighting(LOW, HIGH, LOW);
        } else if (current_mode == LOW_MODE) {
            set_lighting(LOW, LOW, HIGH);
        } else {
            set_lighting(LOW, LOW, LOW);  // Если режим был OFF, оставляем выключенным
        }
    }
}
