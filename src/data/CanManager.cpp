#include "CanManager.h"
#include <configs/pins.h>
#include <configs/settings.h>

// Response
bool CanManager::left_restroom = false;
bool CanManager::right_restroom = false;
bool CanManager::reset_call = false;

// Request
bool CanManager::lighting_high = false;
bool CanManager::lighting_medium = false;
bool CanManager::lighting_low = false;
bool CanManager::guard_notok = false;
bool CanManager::door_state = false;
bool CanManager::call = false;
bool CanManager::guard_activated = false;

CanManager::CanManager() :
    can(CAN_CS) {
    
}

void CanManager::processing() {
    static unsigned long lastTime = 0;
    if (millis() - lastTime >= CAN_SURVEY) {
        lastTime = millis();
        request();
        response();
        heartbeat(COMPARTMENT_ROOM);
    }
}

void CanManager::begin() {
    if (can.reset() != MCP2515::ERROR_OK) {
        Serial.println("Ошибка инициализации MCP2515");
        return;
    }
    can.setBitrate(CAN_250KBPS, MCP_8MHZ);
    can.setNormalMode();
    Serial.println("CAN начат.");
}

void CanManager::request() {
    struct can_frame message;

    if (can.readMessage(&message) == MCP2515::ERROR_OK) {
        Serial.print("Получено сообщение с ID: ");
        Serial.println(message.can_id, HEX);

        if (message.can_id == 0x180) { // RPDO_1
            left_restroom = message.data[0] & 0x01;
            right_restroom = message.data[0] & 0x02;
            reset_call = message.data[0] & 0x04;
        }
    }
}

void CanManager::response() {
    struct can_frame message;
    message.can_id = 0x200; // TPDO_1
    message.can_dlc = 8;

    message.data[0] = lighting_low;
    message.data[1] = lighting_medium;
    message.data[2] = lighting_high;
    message.data[3] = guard_notok;
    message.data[4] = door_state;
    message.data[5] = call;
    message.data[6] = guard_activated;
    message.data[7] = 0x00;

    can.sendMessage(&message);
}

void CanManager::heartbeat(uint8_t nodeID) {
    struct can_frame message;
    message.can_id = 0x700 + nodeID;
    message.can_dlc = 1;

    // Статус узла (Operational = 0x05, Pre-Operational = 0x7F, Stopped = 0x04)
    message.data[0] = 0x05;
    can.sendMessage(&message);
    Serial.println("Отправлен Heartbeat");
}

