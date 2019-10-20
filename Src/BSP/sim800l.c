

/*********************************INCLUDES******************************************/
#include "BSP/sim800l.h"
#include <string.h>

/**********************GLOBAL VARIABLES AND CONSTANTS*******************************/
/* static structure, initilized with sim800l_init() 
	gives information for GPRS/SMS functions
 */
static sim800l_t sim800l;

/****************************FORWARD DECLARATIONS***********************************/


/*********************************FUNCTIONS*****************************************/
void sim800l_init(UART_HandleTypeDef *uart, uint32_t txrx_timeout_ms)
{
	sim800l.uart = uart;
	sim800l.default_txrx_timeout_ms = txrx_timeout_ms;
	sim800l.init_complete = 1;
}


HAL_StatusTypeDef sim800l_configure_module(void)
{
	if ((sim800l.power_status == 0) || (sim800l.init_complete == 0)) {
		return HAL_ERROR;
	}

	at_all_conf_msg_t at_msg;

	// SMS configuration
	strcpy(at_msg.set_sms_mode.tx_msg, "AT+CMGF=1\r");
	at_msg.set_sms_mode.rx_timeout = sim800l.default_txrx_timeout_ms;


	// Internet configuration
	strcpy(at_msg.set_full_func.tx_msg, "AT+CFUN=1\r");
	at_msg.set_full_func.rx_timeout = sim800l.default_txrx_timeout_ms;

	strcpy(at_msg.set_one_IP_comm.tx_msg, "AT+CIPMUX=0\r");
	at_msg.set_one_IP_comm.rx_timeout = sim800l.default_txrx_timeout_ms;

	strcpy(at_msg.set_apn.tx_msg, "AT+CSTT=\"web.htgprs\",\"\",\"\"\r");
	at_msg.set_apn.rx_timeout = sim800l.default_txrx_timeout_ms;

	strcpy(at_msg.connect_gprs.tx_msg, "AT+CIICR\r");
	at_msg.connect_gprs.rx_timeout = 3000;

	strcpy(at_msg.get_IP.tx_msg, "AT+CIFSR\r");
	at_msg.get_IP.rx_timeout = sim800l.default_txrx_timeout_ms;


	//Data sending
	at_tx_msg_t *poka_struct = &at_msg.set_sms_mode;
	uint8_t numb_of_msg = sizeof(at_all_conf_msg_t) / sizeof(at_tx_msg_t);

	for (uint8_t i = 0; i < numb_of_msg; i++) {
		if (sim800l_send_at_cmd(poka_struct->tx_msg, strlen(poka_struct->tx_msg), poka_struct->rx_timeout) == HAL_ERROR) {
			return HAL_ERROR;
		}

		poka_struct++;
	}
	return HAL_OK;
}


// Function for sending SMS massege
HAL_StatusTypeDef sim800l_send_sms(char *phone_numb, char *sms_text)
{
	if ((sim800l.power_status == 0) || (sim800l.init_complete == 0)) {
		return HAL_ERROR;
	}

	char at_numb_cmd[] 	= "AT+CMGS=";
	char at_phone_numb[50];
	char msg[50];

	sprintf (at_phone_numb, "%s\"%s\"\r", at_numb_cmd, phone_numb);

	// <ctrl+z> = decimal number 26
	sprintf (msg, "%s%c", sms_text, 26);

	// Setting phone mumber
	if (sim800l_send_at_cmd(&at_phone_numb[0], strlen(at_phone_numb), sim800l.default_txrx_timeout_ms) == HAL_ERROR) {
		return HAL_ERROR;
	}

	// Sending message data
	if (sim800l_send_at_cmd(&msg[0], strlen(msg), 5000) == HAL_ERROR) {
		return HAL_ERROR;
	}
	return HAL_OK;
}


HAL_StatusTypeDef sim800l_send_web_data(float value)
{
	if ((sim800l.power_status == 0) || (sim800l.init_complete == 0)) {
		return HAL_ERROR;
	}

	char at_open_thingspeak_tcp[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r";
	char at_close_tcp[] = "AT+CIPSHUT\r";

	char at_send_length[] = "AT+CIPSEND=";
	char at_send_data[] = "GET https://api.thingspeak.com/update.json?api_key=";

	char thingspeak_write_key[] = "J2084V1YICURG754";


	char at_send_len[40] = {0};
	char at_data[100] = {0};
	sprintf (at_data, "%s%s&field1=%2f%c\n", at_send_data, thingspeak_write_key, value, 26);
	sprintf(at_send_len, "%s%d\r", at_send_length, strlen(at_data));


	// Sending message data
	if (sim800l_send_at_cmd(&at_open_thingspeak_tcp[0], strlen(at_open_thingspeak_tcp), 3000) == HAL_ERROR) {
		return HAL_ERROR;
	}

	if (sim800l_send_at_cmd(&at_send_len[0], strlen(at_send_len), sim800l.default_txrx_timeout_ms) == HAL_ERROR) {
		return HAL_ERROR;
	}

	if (sim800l_send_at_cmd(&at_data[0], strlen(at_data), 5000) == HAL_ERROR) {
		return HAL_ERROR;
	}

	if (sim800l_send_at_cmd(&at_close_tcp[0], strlen(at_close_tcp), 5000) == HAL_ERROR) {
		return HAL_ERROR;
	}
	return HAL_OK;
}


HAL_StatusTypeDef sim800l_send_at_cmd(char *msg, uint32_t msg_size, uint32_t rx_timeout)
{
	if (HAL_UART_Transmit(sim800l.uart, (uint8_t*)msg, msg_size, sim800l.default_txrx_timeout_ms) == HAL_OK) {
		sim800l_rx_read(rx_timeout);
		return HAL_OK;
	}
	return HAL_ERROR;
}


void sim800l_rx_read(uint32_t timeout)
{
	// array large enough to receive a response from SIM800L 
	// experimentally defined array size 
	uint8_t read_data[350] = {0};
	uint16_t i = 0;
	
	// unnecessary variable
	// serves as a breakpoint to check array read_data
	uint8_t debug_temp = 0;


	while(1) {
		/* If it reads 1 byte during timeout, try reading the second byte etc. 
			 If it cannot read more bytes -> we have received all the data
		 */
		if (HAL_UART_Receive(sim800l.uart, &read_data[i], 1, timeout) == HAL_OK) {
			i++;
		}
		else {
			// exit the while loop
			break;
		}
	}

	//serves as a breakpoint for debuging
	debug_temp = debug_temp + 8;
}


void sim800l_powerOn(void)
{
	HAL_Delay(1000);
	HAL_GPIO_WritePin(SIM800L_POWER_PORT, SIM800L_POWER_PIN, GPIO_PIN_SET);
	sim800l.power_status = 1;
	HAL_Delay(20000);
}


void sim800l_powerOff(void)
{
	HAL_GPIO_WritePin(SIM800L_POWER_PORT, SIM800L_POWER_PIN, GPIO_PIN_RESET);
	sim800l.power_status = 0;
}
