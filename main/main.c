#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "lapTimer.h"

State_t _state = INIT;

void app_main(void) {
    for (;;) {
        state_machine();
        vTaskDelay(1);
    }
}
