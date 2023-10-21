#include "lapTimer_slaves.h"

SlaveList_t slaves = {.pointer = -1};
uint32_t deviceIP;

int8_t checkIfSlaveRegistered(uint32_t IP) {
    for (int8_t i = 0; i <= slaves.pointer; i++) {
        if (slaves.slave[i]._encodedIP == IP) {
            return i;
        }
    }
    return -1;
}

void registerSlave(uint32_t IP, IR_State_t IRstate) {
    slaves.pointer++;
    slaves.slave[slaves.pointer]._encodedIP = IP;
    slaves.slave[slaves.pointer]._IRstate = IRstate;
    slaves.slave[slaves.pointer]._instruction = 0;
    slaves.slave[slaves.pointer]._lastHeartbeat = esp_timer_get_time();
    slaves.slave[slaves.pointer]._isAlive = true;
}

void updateSlave(uint32_t IP, IR_State_t IRstate) {
    for (int i = 0; i <= slaves.pointer; i++) {
        if (slaves.slave[i]._encodedIP == IP) {
            slaves.slave[i]._IRstate = IRstate;
            slaves.slave[i]._lastHeartbeat = esp_timer_get_time();
            slaves.slave[i]._isAlive = true;
            return;
        }
    }
}

void slaveCleanup(void) {
    for (int i = 0; i <= slaves.pointer; i++) {
        if (slaves.slave[i]._lastHeartbeat < esp_timer_get_time() - SLAVE_KILL_TIMEOUT_US) {
            slaves.slave[i]._isAlive = false;
        }
    }
}