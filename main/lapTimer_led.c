#include "lapTimer_led.h"

void updateLED(LED_t* led) {
    switch (led->_state) {
        case Wait:
            gpio_set_level(led->_pin, 0);
            break;

        case BlinkNTimes:
            if (led->_blinkCounter >= led->_blinkTarget) {
                led->_state = Wait;
                break;
            }
            if (esp_timer_get_time() > (led->_captureTime + 1000000 / (led->_blinkFrequency / 2))) {
                switch (led->_blinkTracker) {
                    case false:
                        gpio_set_level(led->_pin, 1);
                        led->_blinkTracker = 1;
                        break;
                    case true:
                        gpio_set_level(led->_pin, 0);
                        led->_blinkTracker = 0;
                        led->_blinkCounter++;
                        break;
                }
            }
            break;

        case ConstantBlink:
            if (esp_timer_get_time() > (led->_captureTime + 1000000 / (led->_blinkFrequency * 2))) {
                switch (led->_blinkTracker) {
                    case false:
                        gpio_set_level(led->_pin, 1);
                        led->_blinkTracker = 1;
                        break;
                    case true:
                        gpio_set_level(led->_pin, 0);
                        led->_blinkTracker = 0;
                        break;
                }
                led->_captureTime = esp_timer_get_time();
            }
            break;

        case Constant:
            gpio_set_level(led->_pin, 1);
            break;
    }
}

void LED_blinkNtimes(LED_t* led, uint8_t N) {
    led->_blinkCounter = 0;
    led->_blinkTarget = N;
    led->_state = BlinkNTimes;
}
void LED_blinkConstanty(LED_t* led) {
    led->_state = ConstantBlink;
}
void LED_setBlinkFrequency(LED_t* led, uint8_t blinkFrequency) {
    led->_blinkFrequency = blinkFrequency;
}
void LED_turnOn(LED_t* led) {
    led->_state = Constant;
}
void LED_turnOff(LED_t* led) {
    led->_state = Wait;
}

void LED_setup(LED_t* led) {
    gpio_reset_pin(led->_pin);
    ESP_ERROR_CHECK(gpio_set_direction(led->_pin, GPIO_MODE_OUTPUT));
}