#ifndef SECURITY_H
#define SECURITY_H

#include <Arduino.h>
#include <EncButton.h>

class Security {
public:
    Security();
    void processing();
    bool isDanger() const { return danger; }
private:
    Button button_call;
    Button button_alarm;

    bool danger;

    void call();
    void alarm();

    void activate_boozer(bool state);
};

#endif