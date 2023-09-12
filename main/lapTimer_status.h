#ifndef LAPTIMER_STATUS_H_
#define LAPTIMER_STATUS_H_

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>


#define STATUS_CALIBRATION_MASK 0b00000001
#define STATUS_CALIBRATING_MASK 0b00000010
#define STATUS_ACTIVATION_MASK 0b00000100
#define STATUS_ERROR_MASK 0b10000000

typedef enum{
    Gate_calibrated,
    Gate_calibrating,
    Gate_active,
    Gate_Error,
} Status_t;

typedef struct {
    uint32_t encodedIP;
    uint8_t status;
} SlaveData_t;

typedef struct {
    SlaveData_t slave[10];
    int8_t pointer;
}SlaveList_t;

extern uint16_t _statusBits;
extern SlaveList_t slaves;
extern uint32_t deviceIP;

void setStatusBit(Status_t statusBit);
void resetStatusBit(Status_t statusBit);
bool getStatusBit (Status_t statusBit);

bool checkIfSlaveRegistered(uint32_t IP);
void registerSlave(uint32_t IP, uint8_t status);
uint8_t getSlaveStatus(uint32_t IP);
void updateSlaveStatus(uint32_t IP, uint8_t status);


#endif