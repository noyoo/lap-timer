#include "lapTimer_status.h"

uint16_t _statusBits;

void setStatusBit(Status_t statusBit) {
    switch (statusBit) {
        case System_ready:
            _statusBits = _statusBits | STATUS_SYSTEM_START_READY_SET;
            break;
        case System_running:
            _statusBits = _statusBits | STATUS_SYSTEM_START_RUNNING_SET;
            break;
        case Gate1_calibrated:
            _statusBits = _statusBits | STATUS_GATE1_CALIBRATION_STATUS_SET;
            break;
        case Gate1_calibrating:
            _statusBits = _statusBits | STATUS_GATE1_CALIBRATION_INPROGRESS_SET;
            break;
        case Gate1_active:
            _statusBits = _statusBits | STATUS_GATE1_ACTIVATION_SET;
            break;
    }
}

void resetStatusBit(Status_t statusBit) {
    switch (statusBit) {
        case System_ready:
            _statusBits = _statusBits & STATUS_SYSTEM_START_READY_RESET;
            break;
        case System_running:
            _statusBits = _statusBits & STATUS_SYSTEM_START_RUNNING_RESET;
            break;
        case Gate1_calibrated:
            _statusBits = _statusBits & STATUS_GATE1_CALIBRATION_STATUS_RESET;
            break;
        case Gate1_calibrating:
            _statusBits = _statusBits & STATUS_GATE1_CALIBRATION_INPROGRESS_RESET;
            break;
        case Gate1_active:
            _statusBits = _statusBits & STATUS_GATE1_ACTIVATION_RESET;
            break;
    }
}

bool getStatusBit(Status_t statusBit) {
    switch (statusBit) {
        case System_ready:
            return (_statusBits & STATUS_SYSTEM_START_READY_SET) == STATUS_SYSTEM_START_READY_SET ? true : false;
            break;
        case System_running:
            return (_statusBits & STATUS_SYSTEM_START_RUNNING_SET) == STATUS_SYSTEM_START_RUNNING_SET ? true : false;
            break;
        case Gate1_calibrated:
            return (_statusBits & STATUS_GATE1_CALIBRATION_STATUS_SET) == STATUS_GATE1_CALIBRATION_STATUS_SET ? true : false;
            break;
        case Gate1_calibrating:
            return (_statusBits & STATUS_GATE1_CALIBRATION_INPROGRESS_SET) == STATUS_GATE1_CALIBRATION_INPROGRESS_SET ? true : false;
            break;
        case Gate1_active:
            return (_statusBits & STATUS_GATE1_ACTIVATION_SET) == STATUS_GATE1_ACTIVATION_SET ? true : false;
            break;
    }
    return -1;
}