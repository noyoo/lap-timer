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
}

void updateSlave(uint32_t IP, IR_State_t IRstate) {
    for (int i = 0; i <= slaves.pointer; i++) {
        if (slaves.slave[i]._encodedIP == IP) {
            slaves.slave[slaves.pointer]._IRstate = IRstate;
            return;
        }
    }
}