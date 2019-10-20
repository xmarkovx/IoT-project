#ifndef LED_GPIO_H_
#define LED_GPIO_H_

/*********************************INCLUDES******************************************/
#include "stm32l0xx_hal.h"

/**********************************DEFINES******************************************/
#define LED_RED_PIN				GPIO_PIN_6
#define LED_RED_PORT			GPIOA

#define LED_GREEN_PIN			GPIO_PIN_7
#define LED_GREEN_PORT			GPIOA

#define LED_BLUE_PIN			GPIO_PIN_0
#define LED_BLUE_PORT			GPIOB

#define LED_WHITE_PIN			GPIO_PIN_1
#define LED_WHITE_PORT			GPIOB

#define LED_NUMBER				4

/*********************************STRUCTURES****************************************/
typedef enum {
	LED_RED = 1,
	LED_GREEN = 2,
	LED_BLUE = 3,
	LED_WHITE = 4
} led_t;

typedef enum {
	LED_OFF = 0,
	LED_ON = 1
} led_output_t;

/******************************EXTERN VARIABLES*************************************/


/****************************FUNCTION PROTOTYPES************************************/
void led_gpio_set(led_t led, led_output_t led_out);
void led_gpio_all_off(void);


#endif /* LED_GPIO_H_ */
