#ifndef MAX17048_H_
#define MAX17048_H_

/*********************************INCLUDES******************************************/
#include "stm32l0xx_hal.h"

/**********************************DEFINES******************************************/
//#define MAX17048_I2C_ADDR				0x36

#define MAX17048_I2C_REG_VOLTAGE		0x02
#define MAX17048_VOLTAGE_MULTIPLIER		0.000078125

#ifdef NOT_FOR_CLASS
#define MAX17048_I2C_REG_SOC			0x04
#define MAX17048_I2C_REG_MODE			0x06
#define MAX17048_I2C_REG_CRATE			0x16
#define MAX17048_I2C_REG_POR			0xFE

#define MAX17048_I2C_SET_QUICK_START	0x4000
#define MAX17048_I2C_SET_POR_CMD		0x5400

#define MAX17048_SOC_DIVIDER			256
#endif

/*********************************STRUCTURES****************************************/
typedef struct {
	I2C_HandleTypeDef *i2c;
	uint16_t i2c_addr;
	uint32_t timeout_ms;
	uint8_t init_complete;
} battery_t;

#ifdef NOT_FOR_CLASS
typedef enum {
	BATTERY_IS_DISCHARGING = 0,
	BATTERY_IS_CHARGING = 1,
	NO_INFO = 2
} batt_charge_t;
#endif

/******************************EXTERN VARIABLES*************************************/


/****************************FUNCTION PROTOTYPES************************************/
void 				batt_init(I2C_HandleTypeDef *i2c, uint16_t i2c_addr, uint32_t timeout_ms);
HAL_StatusTypeDef 	batt_get_voltage_value(float *voltage_value);
/*----------------------------------------------------------------*/
HAL_StatusTypeDef 	batt_read_value(uint8_t *send_cmd, uint8_t *received_data, uint16_t rx_expected_length);

#ifdef NOT_FOR_CLASS
HAL_StatusTypeDef 	batt_get_SoC_value(uint8_t *soc_value);
batt_charge_t 		batt_get_battery_charge_status(void);
HAL_StatusTypeDef	batt_set_quick_start(void);
HAL_StatusTypeDef	batt_set_POR(void);
#endif

#endif /* MAX17048_H_ */
