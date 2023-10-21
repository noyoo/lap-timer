#ifndef MAIN_LAPTIMER_TYPES_H_
#define MAIN_LAPTIMER_TYPES_H_
#include <inttypes.h>
#include <stdbool.h>

typedef enum {
    Init,
    Master_Init,
    Master,
    Slave_Init,
    Slave,
    Error,
} State_t;

typedef enum {
    Intercept,
    Armed,
    Active,
    WaitForClear,
} IR_State_t;

typedef struct {
    int64_t _start_time;
    int64_t _split[100];
    int8_t _position;
} SplitGroup_t;

typedef struct {
    IR_State_t _state;
    uint8_t _sensorPin;
    bool _isActive;
} IR_Sensor_t;

typedef struct {
    uint32_t _encodedIP;
    IR_State_t _IRstate;
    uint8_t _instruction;  // 0 - do nothing, 1-activate, 2 - calibrate
    int64_t _lastHeartbeat;
    bool _isAlive;
} SlaveData_t;

typedef struct {
    SlaveData_t slave[10];
    int8_t pointer;
} SlaveList_t;



#endif /* MAIN_LAPTIMER_TYPES_H_ */
