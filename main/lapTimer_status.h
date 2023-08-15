#ifndef LAPTIMER_STATUS_H_
#define LAPTIMER_STATUS_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

// 0x00000000
#define STATUS_SYSTEM_START_READY_SET 0b00000001
#define STATUS_SYSTEM_START_RUNNING_SET 0b00000010
#define STATUS_GATE1_CALIBRATION_STATUS_SET 0b00000100
#define STATUS_GATE1_CALIBRATION_INPROGRESS_SET 0b00001000
#define STATUS_GATE1_ACTIVATION_SET 0b00010000

#define STATUS_SYSTEM_START_READY_RESET (0b00000001 ^ 0b11111111)
#define STATUS_SYSTEM_START_RUNNING_RESET (0b00000010 ^ 0b11111111)
#define STATUS_GATE1_CALIBRATION_STATUS_RESET (0b00000100 ^ 0b11111111)
#define STATUS_GATE1_CALIBRATION_INPROGRESS_RESET (0b00001000 ^ 0b11111111)
#define STATUS_GATE1_ACTIVATION_RESET (0b00010000 ^ 0b11111111)

typedef enum{
    System_ready,
    System_running,
    Gate1_calibrated,
    Gate1_calibrating,
    Gate1_active,
} Status_t;

extern uint16_t _statusBits;

void setStatusBit(Status_t statusBit);
void resetStatusBit(Status_t statusBit);
bool getStatusBit (Status_t statusBit);

#endif