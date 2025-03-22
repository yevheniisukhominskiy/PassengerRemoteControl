#ifndef RESTROOM_H
#define RESTROOM_H

#include <Arduino.h>

class Restroom {
public:
    Restroom();
    void processing();
private:
    void update_indicators();
};

#endif