#ifndef LED_PWM_H_
#define LED_PWM_H_

/*********************************INCLUDES******************************************/
#include "stm32l0xx_hal.h"

/**********************************DEFINES******************************************/
#define LED_RED_CHANNEL			TIM_CHANNEL_1
#define LED_GREEN_CHANNEL		TIM_CHANNEL_2
#define LED_BLUE_CHANNEL		TIM_CHANNEL_3
#define LED_WHITE_CHANNEL		TIM_CHANNEL_4

#define LED_NUMBER				4

/*********************************STRUCTURES****************************************/
typedef struct {
	TIM_HandleTypeDef *tim;
	uint16_t max_counter_value;
	uint8_t init_complete;
} led_pwm_t;

typedef enum {
	LED_RED = 1,
	LED_GREEN = 2,
	LED_BLUE = 3,
	LED_WHITE = 4
} led_t;

/******************************EXTERN VARIABLES*************************************/


/****************************FUNCTION PROTOTYPES************************************/
void led_pwm_init(TIM_HandleTypeDef *tim, uint16_t max_counter_value);
void led_pwm_set(led_t led, uint8_t percentage_on);
void led_pwm_all_off(void);


#endif /* LED_GPIO_H_ */
