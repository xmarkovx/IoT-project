

/*********************************INCLUDES******************************************/
#include "BSP/led_pwm.h"

/**********************GLOBAL VARIABLES AND CONSTANTS*******************************/
//static structure, initilized with led_pwm_init() 
	static led_pwm_t led_pwm;

/****************************FORWARD DECLARATIONS***********************************/


/*********************************FUNCTIONS*****************************************/
void led_pwm_init(TIM_HandleTypeDef *tim, uint16_t max_counter_value)
{
	led_pwm.tim = tim;
	led_pwm.max_counter_value = max_counter_value;
	led_pwm.init_complete = 1;

	HAL_TIM_PWM_Start(led_pwm.tim, LED_RED_CHANNEL);
	HAL_TIM_PWM_Start(led_pwm.tim, LED_GREEN_CHANNEL);
	HAL_TIM_PWM_Start(led_pwm.tim, LED_BLUE_CHANNEL);
	HAL_TIM_PWM_Start(led_pwm.tim, LED_WHITE_CHANNEL);
}


void led_pwm_set(led_t led, uint8_t percentage_on)
{
	if (led_pwm.init_complete == 0) {
		return;
	}

	uint16_t compare_value = (percentage_on * led_pwm.max_counter_value) / 100;

	if (led == LED_RED) {
		__HAL_TIM_SET_COMPARE(led_pwm.tim, LED_RED_CHANNEL, compare_value);
	}
	else if (led == LED_GREEN) {
		__HAL_TIM_SET_COMPARE(led_pwm.tim, LED_GREEN_CHANNEL, compare_value);
	}
	else if (led == LED_BLUE) {
		__HAL_TIM_SET_COMPARE(led_pwm.tim, LED_BLUE_CHANNEL, compare_value);
	}
	else if (led == LED_WHITE) {
		__HAL_TIM_SET_COMPARE(led_pwm.tim, LED_WHITE_CHANNEL, compare_value);
	}
}


void led_pwm_all_off(void)
{
	if (led_pwm.init_complete == 0) {
		return;
	}

	__HAL_TIM_SET_COMPARE(led_pwm.tim, LED_RED_CHANNEL, 0);
	__HAL_TIM_SET_COMPARE(led_pwm.tim, LED_GREEN_CHANNEL, 0);
	__HAL_TIM_SET_COMPARE(led_pwm.tim, LED_BLUE_CHANNEL, 0);
	__HAL_TIM_SET_COMPARE(led_pwm.tim, LED_WHITE_CHANNEL, 0);
}
