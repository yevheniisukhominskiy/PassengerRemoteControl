#include <modules/Security/Security.h>
#include <configs/pins.h>

#include <Arduino.h>
#include <configs/settings.h>
#include <data/CanManager.h>
#include <EncButton.h>

Security::Security()
    :   button_call(BUTTON_CALL),
        button_alarm(BUTTON_SECURITY)  
    {
    pinMode(INPUT_CALL_RESET, INPUT);
    pinMode(OUTPUT_CALL, OUTPUT);

    pinMode(INPUT_SECURITY, INPUT_PULLUP);
    pinMode(LED_SECURITY, OUTPUT);
    pinMode(BUZZER_SECURITY, OUTPUT);
    pinMode(OUTPUT_SECURITY, OUTPUT);
}

void Security::processing() {
    call();
    alarm();
}

void Security::call() {
    static bool call_active = false;  
    static bool reset_handled = false;

    button_call.tick();

    // Переключаем состояние вызова кнопкой
    if (button_call.press()) { 
        call_active = !call_active;
    }

    // Проверяем сигнал сброса только один раз при переднем фронте
    if ((digitalRead(INPUT_CALL_RESET) == HIGH /*|| CanManager::isResetCall*/) && !reset_handled) {
        call_active = false;
        reset_handled = true;  // Запоминаем, что сброс обработан
    } 
    
    if (digitalRead(INPUT_CALL_RESET) == LOW) {
        reset_handled = false;  // Сброс готов к новой обработке
    }

    digitalWrite(OUTPUT_CALL, call_active ? HIGH : LOW);
}

void Security::alarm() {
    button_alarm.tick();

    static bool alarm_state = false;
    
    if (button_alarm.press()) {  
        if (!alarm_state) {  
            // Если сигнализация выключена и НЕТ питания (INPUT_SECURITY == LOW), то НЕ включаем
            if (digitalRead(INPUT_SECURITY) == LOW) return;
        }
        
        alarm_state = !alarm_state;
        
        if (!alarm_state) {  
            // Выключение сигнализации
            digitalWrite(BUZZER_SECURITY, LOW);
            digitalWrite(OUTPUT_SECURITY, LOW);
            danger = false;
        }
        
        digitalWrite(LED_SECURITY, alarm_state ? HIGH : LOW);
    }

    // Если сигнализация включена, но двери открыты, включаем тревогу
    if (alarm_state && digitalRead(INPUT_SECURITY) == LOW) {
        danger = true;
        digitalWrite(OUTPUT_SECURITY, HIGH);
    }

    // Обновляем зуммер
    activate_boozer(danger);
}

void Security::activate_boozer(bool state) {
    static unsigned long previousMillis = 0;
    static bool buzzerState = false;

    if (!state) {
        // Немедленно выключаем зуммер
        buzzerState = false;
        digitalWrite(BUZZER_SECURITY, LOW);
        return;
    }

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= SIREN_INTERVAL) {
        previousMillis = currentMillis;
        buzzerState = !buzzerState;
        digitalWrite(BUZZER_SECURITY, buzzerState ? HIGH : LOW);
    }
}

