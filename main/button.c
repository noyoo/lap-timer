#include "button.h"
int Button_Update(Button_t* button) {
    switch (button->_state) {
        case Released:
            if (!gpio_get_level(button->_buttonPin)) button->_state = Pressed_Debounce;
            break;
        case Pressed_Debounce:
            vTaskDelay(BUTTON_DEBOUNCE);
            button->_pressTime = esp_timer_get_time();
            button->_state = LongPress_Wait;
            break;
        case Pressed:
            if (button->_singlePressCallback != NULL) button->_singlePressCallback();
            button->_state = WaitForRelease;
            break;
        case LongPress_Wait:
            if (!gpio_get_level(button->_buttonPin)) {
                if (esp_timer_get_time() > button->_pressTime + BUTTON_LONGPRESS_THRESH) {
                    button->_state = LongPress;
                }
            } else {
                button->_state = SecondPress_Wait;
            }
            break;
        case LongPress:
            if (button->_longPressCallback != NULL) button->_longPressCallback();
            button->_state = WaitForRelease;
            break;
        case WaitForRelease:
            if (gpio_get_level(button->_buttonPin)) button->_state = Depressed_Debounce;
            break;
        case SecondPress_Wait:
            if (esp_timer_get_time() > button->_pressTime + BUTTON_NEXTPRESS_THRESH) {
                button->_state = Pressed;
            } else if (!gpio_get_level(button->_buttonPin)) {
                button->_state = SecondPress_Debounce;
            }
            break;
        case SecondPress_Debounce:
            vTaskDelay(BUTTON_DEBOUNCE);
            button->_state = SecondPress;
            break;
        case SecondPress:
            if (button->_doublePressCallback != NULL) button->_doublePressCallback();
            button->_state = WaitForRelease;
            break;
        case Depressed_Debounce:
            vTaskDelay(BUTTON_DEBOUNCE);
            button->_state = Released;
            break;
    }
    return 1;
}
