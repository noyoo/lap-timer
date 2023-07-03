#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "esp_task_wdt.h"
#include "lapTimer.h"

State _state = INIT;

void app_main(void) {
  for(;;){
      ESP_ERROR_CHECK(esp_task_wdt_reset());
      state_machine();
  }
}
