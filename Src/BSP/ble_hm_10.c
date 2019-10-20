

/*********************************INCLUDES******************************************/
#include "BSP/ble_hm_10.h"
#include "BSP/led_pwm.h"
#include "BSP/max17048.h"
#include "stm32l0xx_it.h"
#include "tim.h"
#include <string.h>

/**********************GLOBAL VARIABLES AND CONSTANTS*******************************/
//static structure, initilized with ble_init()
static ble_t ble_hm10;

/****************************FORWARD DECLARATIONS***********************************/


/*********************************FUNCTIONS*****************************************/
void ble_init(UART_HandleTypeDef *uart, uint32_t timeout_ms, uint16_t rx_size)
{
	ble_hm10.uart = uart;
	ble_hm10.timeout_ms = timeout_ms;
	ble_hm10.rx_size = rx_size;
	ble_hm10.init_complete = 1;
}


HAL_StatusTypeDef ble_read_batt_voltage_and_send(uint8_t time_interval)
{
	char ble_data_to_send[60] = {0};
	float batt_voltage = 0;
	HAL_StatusTypeDef send_status;

	if (batt_get_voltage_value(&batt_voltage) == HAL_OK) {
		if (time_interval == 0) {
			sprintf(&ble_data_to_send[0], "Battery voltage: %.2f volti\r\n", batt_voltage);
		}
		else {
			sprintf(&ble_data_to_send[0], "Battery voltage: %.2f volti (%d sekundi)\r\n", batt_voltage, time_interval);
		}
		send_status = ble_send_msg(&ble_data_to_send[0], strlen(ble_data_to_send));
		return send_status;
	}

	return HAL_ERROR;
}


void ble_check_rx_buffer(uint8_t *pBatt_voltage_interval_s)
{
	if ((ble_hm10.init_complete == 0) || (ble_is_connected() == BLE_NOT_CONNECTED)) {
		return;
	}

	uint8_t rx_data[BLE_HM10_EXPECTED_RX_SIZE] = {0};

	// Reading BLE-a data
	if (HAL_UART_Receive(ble_hm10.uart, &rx_data[0], ble_hm10.rx_size, ble_hm10.timeout_ms) == HAL_OK) {

		uint16_t msg_id = (rx_data[BLE_HM10_RX_INDEX_MSG_ID_1] << 8) | rx_data[BLE_HM10_RX_INDEX_MSG_ID_2];

		// if msg_id...
		switch(msg_id) {
			case BLE_HM10_RX_MSG_LED_ONE: {
				uint8_t led_id = rx_data[BLE_HM10_RX_INDEX_DATA_1];
				uint8_t led_output_status = rx_data[BLE_HM10_RX_INDEX_DATA_2] * 100;  // 1 -> 100% for PWM

				HAL_TIM_Base_Stop_IT(&htim6);
				led_pwm_set(led_id, led_output_status);
				break;
			}
			case BLE_HM10_RX_MSG_LED_TWO: {
				uint8_t led_id_1 = rx_data[BLE_HM10_RX_INDEX_DATA_1];
				uint8_t led_output_status_1 = rx_data[BLE_HM10_RX_INDEX_DATA_2] * 100; // 1 -> 100% for PWM
				uint8_t led_id_2 = rx_data[BLE_HM10_RX_INDEX_DATA_3];
				uint8_t led_output_status_2 = rx_data[BLE_HM10_RX_INDEX_DATA_4] * 100; // 1 -> 100% for PWM

				HAL_TIM_Base_Stop_IT(&htim6);
				led_pwm_set(led_id_1, led_output_status_1);
				led_pwm_set(led_id_2, led_output_status_2);
				break;
			}
			case BLE_HM10_RX_MSG_LED_CHANGING: {
				__disable_irq();
				g_led_id[0] = rx_data[BLE_HM10_RX_INDEX_DATA_1];
				g_led_id[1] = rx_data[BLE_HM10_RX_INDEX_DATA_2];
				g_led_id[2] = rx_data[BLE_HM10_RX_INDEX_DATA_3];
				g_led_id[3] = rx_data[BLE_HM10_RX_INDEX_DATA_4];
				for (uint8_t i = 0; i < LED_NUMBER; i++) {
					g_led_output_percentage[i] = 100;	// percentage value for PWM output
				}
				g_led_change_type = FOUR_LED_GPIO_CHANGE;
				__enable_irq();

				change_timer_counter_period(&htim6, rx_data[BLE_HM10_RX_INDEX_DATA_5]);
				HAL_TIM_Base_Start_IT(&htim6);
				break;
			}
			case BLE_HM10_RX_MSG_LEDS_OFF: {
				if (rx_data[BLE_HM10_RX_INDEX_DATA_1] == 0xAA) {
					HAL_TIM_Base_Stop_IT(&htim6);
					led_pwm_all_off();
				}
				break;
			}
			case BLE_HM10_RX_MSG_LED_PWM_ONE: {
				uint8_t led_id = rx_data[BLE_HM10_RX_INDEX_DATA_1];
				uint8_t led_output_status = rx_data[BLE_HM10_RX_INDEX_DATA_2];

				HAL_TIM_Base_Stop_IT(&htim6);
				led_pwm_set(led_id, led_output_status);
				break;
			}
			case BLE_HM10_RX_MSG_LED_PWM_TWO: {
				uint8_t led_id_1 = rx_data[BLE_HM10_RX_INDEX_DATA_1];
				uint8_t led_output_status_1 = rx_data[BLE_HM10_RX_INDEX_DATA_2];
				uint8_t led_id_2 = rx_data[BLE_HM10_RX_INDEX_DATA_3];
				uint8_t led_output_status_2 = rx_data[BLE_HM10_RX_INDEX_DATA_4];

				HAL_TIM_Base_Stop_IT(&htim6);
				led_pwm_set(led_id_1, led_output_status_1);
				led_pwm_set(led_id_2, led_output_status_2);
				break;
			}
			case BLE_HM10_RX_MSG_LED_PWM_CHANGING: {
				__disable_irq();
				g_led_id[0] = rx_data[BLE_HM10_RX_INDEX_DATA_1];
				g_led_id[1] = rx_data[BLE_HM10_RX_INDEX_DATA_3];
				g_led_output_percentage[0] = rx_data[BLE_HM10_RX_INDEX_DATA_2];
				g_led_output_percentage[1] = rx_data[BLE_HM10_RX_INDEX_DATA_4];
				g_led_change_type = TWO_LED_PWM_CHANGE;
				__enable_irq();

				change_timer_counter_period(&htim6, rx_data[BLE_HM10_RX_INDEX_DATA_5]);
				HAL_TIM_Base_Start_IT(&htim6);
				break;
			}
			case BLE_HM10_RX_MSG_VOLTAGE: {
				if (rx_data[BLE_HM10_RX_INDEX_DATA_1] == 0xAA) {
					ble_read_batt_voltage_and_send(0);
				}
				break;
			}
			case BLE_HM10_RX_MSG_VOLTAGE_TIM: {
				if (rx_data[BLE_HM10_RX_INDEX_DATA_1] == 0) {
					HAL_TIM_Base_Stop_IT(&htim7);
				}
				else {
					change_timer_counter_period(&htim7, rx_data[BLE_HM10_RX_INDEX_DATA_1]);
					*pBatt_voltage_interval_s = rx_data[BLE_HM10_RX_INDEX_DATA_1];
					HAL_TIM_Base_Start_IT(&htim7);
				}
				break;
			}
			default: {
				break;
			}
		}
	}
}


HAL_StatusTypeDef ble_send_msg(char *msg, uint8_t msg_length)
{
	// if BLE is not initilazed with ble_init() or phone is not connected
	if ((ble_hm10.init_complete == 0) || (ble_is_connected() == BLE_NOT_CONNECTED)) {
		return HAL_ERROR;
	}
	return (HAL_UART_Transmit(ble_hm10.uart, (uint8_t*)&msg[0], msg_length, ble_hm10.timeout_ms));
}


ble_connection_t ble_is_connected(void)
{
	if (HAL_GPIO_ReadPin(BLE_HM10_STATE_PORT, BLE_HM10_STATE_PIN) == GPIO_PIN_SET) {
		return BLE_CONNECTED;
	}
	return BLE_NOT_CONNECTED;
}


void change_timer_counter_period(TIM_HandleTypeDef *htim, uint8_t new_counter_value_s)
{
	__HAL_TIM_CLEAR_IT(htim, TIM_IT_UPDATE);
	__HAL_TIM_SET_COUNTER(htim, new_counter_value_s*1000);
	__HAL_TIM_SET_AUTORELOAD(htim, new_counter_value_s*1000);
}

#ifdef NOT_FOR_CLASS
HAL_StatusTypeDef ble_config_set_name(char *network_name)
{
	if ((ble_hm10.init_complete == 0) || (ble_is_connected() == BLE_CONNECTED)) {
		return HAL_ERROR;
	}

	char new_name[30] = {0};
	sprintf (new_name, "%s%s", BLE_HM10_AT_NAME, network_name);

	if (HAL_UART_Transmit(ble_hm10.uart, (uint8_t*)&new_name[0], strlen(new_name), ble_hm10.timeout_ms) == HAL_OK) {
		ble_config_rx_response();
		return HAL_OK;
	}
	return HAL_ERROR;
}


HAL_StatusTypeDef ble_config_set_LED_option(ble_led_option_t led_option)
{
	if ((ble_hm10.init_complete == 0) || (ble_is_connected() == BLE_CONNECTED)) {
	    return HAL_ERROR;
	}

	char pio_cmd[15] = {0};
	if (led_option == BLE_NOT_CONN_NO_LED) {
		strcpy(pio_cmd, BLE_HM10_AT_LED_TOGGLE_NOT);
	}
	else if (led_option == BLE_NOT_CONN_TOGGLE_LED) {
		strcpy(pio_cmd, BLE_HM10_AT_LED_TOGGLE_YES);
	}

	if (HAL_UART_Transmit(ble_hm10.uart, (uint8_t*)&pio_cmd[0], strlen(pio_cmd), ble_hm10.timeout_ms) == HAL_OK) {
		ble_config_rx_response();
		return HAL_OK;
	}
	return HAL_ERROR;
}


void ble_config_rx_response(void)
{
	uint8_t i = 0;
	uint8_t received_data[30] = {0};
	uint8_t debug_var = 0;
	while(1) {
	  if (HAL_UART_Receive(ble_hm10.uart, (uint8_t*)&received_data[i], 1, ble_hm10.timeout_ms) != HAL_OK) {
		break;
	  }
	  i++;
	}
	debug_var = i + debug_var;
	
}
#endif
