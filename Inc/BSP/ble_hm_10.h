#ifndef BLE_HM_10_H_
#define BLE_HM_10_H_

/*********************************INCLUDES******************************************/
#include "stm32l0xx_hal.h"

/**********************************DEFINES******************************************/
#define BLE_HM10_STATE_PIN					GPIO_PIN_11
#define BLE_HM10_STATE_PORT					GPIOA

#define BLE_HM10_EXPECTED_RX_SIZE			10

#ifdef NOT_FOR_CLASS
#define BLE_HM10_AT_NAME					"AT+NAME"
#define BLE_HM10_AT_LED_TOGGLE_YES			"AT+PIO10"
#define BLE_HM10_AT_LED_TOGGLE_NOT			"AT+PIO11"
#endif

/* BLE msg  **********/

// Index
#define BLE_HM10_RX_INDEX_MSG_ID_1			1
#define BLE_HM10_RX_INDEX_MSG_ID_2			2
#define BLE_HM10_RX_INDEX_DATA_1			4
#define BLE_HM10_RX_INDEX_DATA_2			5
#define BLE_HM10_RX_INDEX_DATA_3			6
#define BLE_HM10_RX_INDEX_DATA_4			7
#define BLE_HM10_RX_INDEX_DATA_5			8

// Command ID
#define BLE_HM10_RX_CMD_SING				0xAF
#define BLE_HM10_RX_CMD_DUAL				0XBC

// Msg ID - LED GPIO
#define BLE_HM10_RX_MSG_LED_ONE				0x0701
#define BLE_HM10_RX_MSG_LED_TWO				0x0702
#define BLE_HM10_RX_MSG_LED_CHANGING		0x0703
#define BLE_HM10_RX_MSG_LEDS_OFF			0x0704

// Msg ID - LED PWM
#define BLE_HM10_RX_MSG_LED_PWM_ONE			0x0601
#define BLE_HM10_RX_MSG_LED_PWM_TWO			0x0602
#define BLE_HM10_RX_MSG_LED_PWM_CHANGING	0x0603

// Msg ID - Battery check
#define BLE_HM10_RX_MSG_VOLTAGE				0x0801
#define BLE_HM10_RX_MSG_VOLTAGE_TIM			0x0802


/*********************************STRUCTURES****************************************/
typedef struct {
	UART_HandleTypeDef *uart;
	uint32_t timeout_ms;
	uint16_t rx_size;
	uint8_t init_complete;
} ble_t;

typedef enum {
	BLE_NOT_CONNECTED = 0,
	BLE_CONNECTED = 1
} ble_connection_t;

#ifdef NOT_FOR_CLASS
typedef enum {
	BLE_NOT_CONN_NO_LED = 0,
	BLE_NOT_CONN_TOGGLE_LED = 1
} ble_led_option_t;
#endif

/******************************EXTERN VARIABLES*************************************/


/****************************FUNCTION PROTOTYPES************************************/
void 			  ble_init(UART_HandleTypeDef *uart, uint32_t timeout_ms, uint16_t rx_size);
HAL_StatusTypeDef ble_read_batt_voltage_and_send(uint8_t time_interval);
/*----------------------------------------------------------------*/
void ble_check_rx_buffer(uint8_t *pBatt_voltage_interval_s);
/*----------------------------------------------------------------*/
HAL_StatusTypeDef ble_send_msg(char *msg, uint8_t msg_length);
ble_connection_t  ble_is_connected(void);
void  			  change_timer_counter_period(TIM_HandleTypeDef *htim, uint8_t new_counter_value_s);

#ifdef NOT_FOR_CLASS
HAL_StatusTypeDef ble_config_set_name(char *network_name);
HAL_StatusTypeDef ble_config_set_LED_option(ble_led_option_t led_option);
void ble_config_rx_response(void);
#endif

#endif /* BLE_HM_10_H_ */
