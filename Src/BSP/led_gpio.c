

///*********************************INCLUDES******************************************/
#include "BSP/led_gpio.h"

///**********************GLOBAL VARIABLES AND CONSTANTS*******************************/


///****************************FORWARD DECLARATIONS***********************************/


///*********************************FUNCTIONS*****************************************/
void led_gpio_set(led_t led, led_output_t led_out)
{
	if (led == LED_RED) {
		HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, led_out);
	}
	else if (led == LED_GREEN) {
		HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, led_out);
	}
	else if (led == LED_BLUE) {
		HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, led_out);
	}
	else if (led == LED_WHITE) {
		HAL_GPIO_WritePin(LED_WHITE_PORT, LED_WHITE_PIN, led_out);
	}
}


void led_gpio_all_off(void)
{
	HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_WHITE_PORT, LED_WHITE_PIN, GPIO_PIN_RESET);
}
