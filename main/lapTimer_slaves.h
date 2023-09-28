#ifndef LAPTIMER_STATUS_H_
#define LAPTIMER_STATUS_H_

#include <inttypes.h>
#include <stdbool.h>

#include "lapTimer_types.h"

extern SlaveList_t slaves;
extern uint32_t deviceIP;

int8_t checkIfSlaveRegistered(uint32_t IP);
void registerSlave(uint32_t IP, IR_State_t IRstate);
void updateSlave(uint32_t IP, IR_State_t IRstate);

#endif