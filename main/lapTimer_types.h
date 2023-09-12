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

typedef struct {
    int64_t _start_time;
    int64_t _split[100];
    uint8_t _position;
} SplitGroup_t;

extern bool measurmentStarted;

#endif /* MAIN_LAPTIMER_TYPES_H_ */
