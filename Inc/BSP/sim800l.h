#ifndef SIM800L_H_
#define SIM800L_H_

/*********************************INCLUDES******************************************/
#include "stm32l0xx_hal.h"

/**********************************DEFINES******************************************/
#define SIM800L_POWER_PIN				GPIO_PIN_0
#define SIM800L_POWER_PORT				GPIOC

/*********************************STRUCTURES****************************************/
typedef struct {
	UART_HandleTypeDef *uart;
	uint32_t default_txrx_timeout_ms;
	uint8_t init_complete;
	uint8_t power_status;
} sim800l_t;

typedef struct {
	char tx_msg[70];
	uint32_t rx_timeout;
} at_tx_msg_t;

typedef struct {
	at_tx_msg_t set_sms_mode;
	at_tx_msg_t	set_full_func;
	at_tx_msg_t	set_one_IP_comm;
	at_tx_msg_t	set_apn;
	at_tx_msg_t	connect_gprs;
	at_tx_msg_t get_IP;
} at_all_conf_msg_t;

typedef enum {
	ECHO_ENABLE,
	ECHO_DISABLE
}echo_t;

/******************************EXTERN VARIABLES*************************************/


/****************************FUNCTION PROTOTYPES************************************/
void 			  sim800l_init(UART_HandleTypeDef *uart, uint32_t txrx_timeout_ms);
HAL_StatusTypeDef sim800l_configure_module(void);
HAL_StatusTypeDef sim800l_send_sms(char *phone_numb, char *sms_text);
HAL_StatusTypeDef sim800l_send_web_data(float value);

HAL_StatusTypeDef sim800l_send_at_cmd(char *msg, uint32_t msg_size, uint32_t rx_timeout);
void 			  sim800l_rx_read(uint32_t timeout);

void 			  sim800l_powerOn(void);
void 			  sim800l_powerOff(void);

#endif /* SIM800L_H_ */
