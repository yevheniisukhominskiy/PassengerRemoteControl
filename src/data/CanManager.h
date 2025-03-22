#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <Arduino.h>
#include <mcp2515.h>

class CanManager {
public:
    CanManager();
    void begin();
    void processing();

    static bool isLeftRestroom() {return left_restroom;};
    static bool isRightRestroom() {return right_restroom;};
    static bool isResetCall() {return reset_call;};

    static bool isLightingHigh() {return lighting_high;};
    static bool isLightingMedium() {return lighting_medium;};
    static bool isLightingLow() {return lighting_low;};
    static bool isGuardNotOk() {return guard_notok;};
    static bool isDoorState() {return door_state;};
    static bool isCall() {return call;};
    static bool isGuardActivated() {return guard_activated;};

private:
    MCP2515 can;

    static bool left_restroom;
    static bool right_restroom;
    static bool reset_call;

    static bool lighting_high;
    static bool lighting_medium;
    static bool lighting_low;
    static bool guard_notok;
    static bool door_state;
    static bool call;
    static bool guard_activated;

    void request();
    void response();
    void heartbeat(uint8_t nodeID);
};

#endif