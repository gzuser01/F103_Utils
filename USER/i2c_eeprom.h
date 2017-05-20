
#ifndef __I2C_EEPROM_H
#define	__I2C_EEPROM_H


#include "i2c.h"


uint8_t I2Cx_EE_ReadByte(struct I2Cx_Conf *I2Cx_Conf,u8 Chip_Address, u16 EE_Address);

void I2Cx_EE_WriteByte(struct I2Cx_Conf *I2Cx_Conf,u8 Chip_Address, u16 EE_Address, uint8_t data);



#endif 
