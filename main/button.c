/*
 * button.c
 *
 *  Created on: 4 lip 2023
 *      Author: dvagh
 */
#include "button.h"
int Button_Update(Button_t* button){
	button->_lastState = button->_state;
	switch(button->_state)
	{
	case Depressed:
		if(!gpio_get_level(button->_buttonPin))button->_state = Pressed_Debounce;
		break;
	case Pressed_Debounce:
		if(button->_callback != NULL)button->_callback(); // @suppress("Symbol is not resolved")
		vTaskDelay(BUTTON_DEBOUNCE);
		button->_state = Pressed;
		break;
	case Pressed:
		if(gpio_get_level(button->_buttonPin))button->_state = Depressed_Debounce;
		break;
	case Depressed_Debounce:
		vTaskDelay(BUTTON_DEBOUNCE);
		button->_state = Depressed;
		break;
	}
	return 1;
}
