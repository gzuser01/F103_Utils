
#ifndef __I2C_EEPROM_H
#define	__I2C_EEPROM_H


#include "i2c.h"


int8_t I2Cx_EE_ReadByte(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint16_t ee_address,uint8_t* data);

int8_t I2Cx_EE_WriteByte(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint16_t ee_address, uint8_t data);

int8_t I2Cx_EE_Error_Code(void);

#endif 
