#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "lapTimer.h"

State _state = INIT;

void app_main(void) {
  for(;;){
//
      state_machine();
      vTaskDelay(1);
  }
}
