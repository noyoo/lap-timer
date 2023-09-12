#include "lapTimer_status.h"

uint16_t _statusBits;
SlaveList_t slaves = {.pointer = -1};
uint32_t deviceIP;

void setStatusBit(Status_t statusBit) {
    switch (statusBit) {
        case Gate_calibrated:
            _statusBits = _statusBits | STATUS_CALIBRATION_MASK;
            break;
        case Gate_calibrating:
            _statusBits = _statusBits | STATUS_CALIBRATING_MASK;
            break;
        case Gate_active:
            _statusBits = _statusBits | STATUS_ACTIVATION_MASK;
            break;
        case Gate_Error:
            _statusBits = _statusBits | STATUS_ERROR_MASK;
            break;
    }
}

void resetStatusBit(Status_t statusBit) {
    switch (statusBit) {
        case Gate_calibrated:
            _statusBits = _statusBits & ~STATUS_CALIBRATION_MASK;
            break;
        case Gate_calibrating:
            _statusBits = _statusBits & ~STATUS_CALIBRATING_MASK;
            break;
        case Gate_active:
            _statusBits = _statusBits & ~STATUS_ACTIVATION_MASK;
            break;
        case Gate_Error:
            _statusBits = _statusBits & ~STATUS_ERROR_MASK;
            break;
    }
}

bool getStatusBit(Status_t statusBit) {
    switch (statusBit) {
        case Gate_calibrated:
            return (_statusBits & STATUS_CALIBRATION_MASK) == STATUS_CALIBRATION_MASK ? true : false;
            break;
        case Gate_calibrating:
            return (_statusBits & STATUS_CALIBRATING_MASK) == STATUS_CALIBRATING_MASK ? true : false;
            break;
        case Gate_active:
            return (_statusBits & STATUS_ACTIVATION_MASK) == STATUS_ACTIVATION_MASK ? true : false;
            break;
        case Gate_Error:
            return (_statusBits & STATUS_ERROR_MASK) == STATUS_ERROR_MASK ? true : false;
            break;
    }
    return -1;
}

bool checkIfSlaveRegistered(uint32_t IP) {
    for (int i = 0; i < slaves.pointer; i++) {
        if (slaves.slave[i].encodedIP == IP) {
            return true;
        }
    }
    return false;
}

void registerSlave(uint32_t IP, uint8_t status) {
    slaves.pointer++;
    slaves.slave[slaves.pointer].encodedIP = IP;
    slaves.slave[slaves.pointer].status = status;
}

uint8_t getSlaveStatus(uint32_t IP) {
    for (int i = 0; i < slaves.pointer; i++) {
        if (slaves.slave[i].encodedIP == IP) {
            return slaves.slave[i].status;
        }
    }
    return 0;
}

void updateSlaveStatus(uint32_t IP, uint8_t status) {
    for (int i = 0; i < slaves.pointer; i++) {
        if (slaves.slave[i].encodedIP == IP) {
            slaves.slave[i].status = status;
            return;
        }
    }
}