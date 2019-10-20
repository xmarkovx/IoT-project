

/*********************************INCLUDES******************************************/
#include "BSP/max17048.h"

/**********************GLOBAL VARIABLES AND CONSTANTS*******************************/
/* static structure, initilized with batt_init () 
gives information for I2C comunication with MAX17048 
*/	
	static battery_t battery;

/****************************FORWARD DECLARATIONS***********************************/


/*********************************FUNCTIONS*****************************************/
void batt_init(I2C_HandleTypeDef *i2c, uint16_t i2c_addr, uint32_t timeout_ms)
{
	battery.i2c = i2c;
	battery.i2c_addr = i2c_addr;
	battery.timeout_ms = timeout_ms;
	battery.init_complete = 1;
}


HAL_StatusTypeDef batt_get_voltage_value(float *voltage_value)
{
	if (battery.init_complete != 1) {
	  return HAL_ERROR;
	}

	uint8_t voltage_register = MAX17048_I2C_REG_VOLTAGE;
	uint8_t voltage_received[2] = {0};
	uint16_t temp_voltage_value = 0;

	if (batt_read_value(&voltage_register, &voltage_received[0], sizeof(voltage_received)) == HAL_OK) {
		temp_voltage_value = voltage_received[0] << 8;
		temp_voltage_value |= voltage_received[1];

		*voltage_value = temp_voltage_value * MAX17048_VOLTAGE_MULTIPLIER;
		return HAL_OK;
	}

	*voltage_value = 0;
	return HAL_ERROR;
}


HAL_StatusTypeDef batt_read_value(uint8_t *send_cmd, uint8_t *received_data, uint16_t rx_expected_length)
{
	if (HAL_I2C_Master_Transmit(battery.i2c, battery.i2c_addr << 1, send_cmd, 1, battery.timeout_ms) == HAL_OK) {
		if (HAL_I2C_Master_Receive(battery.i2c, battery.i2c_addr << 1, received_data, rx_expected_length, battery.timeout_ms) == HAL_OK) {
			return HAL_OK;
		}
	}
	return HAL_ERROR;
}


#ifdef NOT_FOR_CLASS
HAL_StatusTypeDef batt_get_SoC_value(uint8_t *soc_value)
{
	if (battery.init_complete != 1) {
	  return HAL_ERROR;
	}

	uint8_t soc_register = MAX17048_I2C_REG_SOC;
	uint8_t soc_received[2] = {0};
	uint16_t temp_soc_value = 0;
	
	if (batt_read_value(&soc_register, &soc_received[0], sizeof(soc_received)) == HAL_OK) {
		temp_soc_value = soc_received[0] << 8;
		temp_soc_value |= soc_received[1];

		*soc_value = temp_soc_value / MAX17048_SOC_DIVIDER;
		return HAL_OK;
	}

	*soc_value = 0;
	return HAL_ERROR;
}


batt_charge_t batt_get_battery_charge_status(void)
{
	if (battery.init_complete != 1) {
	  return HAL_ERROR;
	}

	uint8_t crate_register = MAX17048_I2C_REG_CRATE;
	uint8_t rate_received[2] = {0};
	int16_t crate_value = 0;
	
	if (batt_read_value(&crate_register, &rate_received[0], sizeof(rate_received)) == HAL_OK) {
		crate_value = rate_received[0] << 8;
		crate_value |= rate_received[1];

		if (crate_value < 0) {
			return BATTERY_IS_DISCHARGING;
		}
		else {
			return BATTERY_IS_CHARGING;
		}
	}

	return NO_INFO;
}

HAL_StatusTypeDef batt_set_quick_start(void)
{
	if (battery.init_complete != 1) {
	  return HAL_ERROR;
	}

	uint8_t mode_register = MAX17048_I2C_REG_MODE;

	uint8_t quick_start_cmd[2] = {0};
	quick_start_cmd[0] = (uint8_t)((MAX17048_I2C_SET_QUICK_START & 0xFF00) >> 8);
	quick_start_cmd[1] = (uint8_t)(MAX17048_I2C_SET_QUICK_START & 0x00FF);

	if (HAL_I2C_Mem_Write(battery.i2c, battery.i2c_addr << 1, mode_register, sizeof(mode_register), &quick_start_cmd[0], sizeof(quick_start_cmd), battery.timeout_ms) == HAL_OK) {
		return HAL_OK;
	}
	return HAL_ERROR;
}


HAL_StatusTypeDef batt_set_POR(void)
{
	if (battery.init_complete != 1) {
	  return HAL_ERROR;
	}

	uint8_t por_register = MAX17048_I2C_REG_POR;

	uint8_t por_cmd[2] = {0};
	por_cmd[0] = (uint8_t)((MAX17048_I2C_SET_POR_CMD & 0xFF00) >> 8);
	por_cmd[1] = (uint8_t)(MAX17048_I2C_SET_POR_CMD & 0x00FF);

	if (HAL_I2C_Mem_Write(battery.i2c, battery.i2c_addr << 1, por_register, sizeof(por_register), &por_cmd[0], sizeof(por_cmd), battery.timeout_ms) == HAL_OK) {
		return HAL_OK;
	}
	return HAL_ERROR;
}
#endif
