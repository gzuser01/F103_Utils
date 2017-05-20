
#include "i2c_eeprom.h"


uint8_t I2Cx_EE_ReadByte(struct I2Cx_Conf *I2Cx_Conf,u8 Chip_Address, u16 EE_Address)
{
	uint8_t data;
	
	I2Cx_Wait(I2Cx_Conf);
	I2Cx_Start(I2Cx_Conf);
	I2Cx_Address_Direction(I2Cx_Conf,Chip_Address, I2C_Direction_Transmitter);
	I2Cx_Transmit(I2Cx_Conf, (u8)((EE_Address >>8)&0x00ff));
	I2Cx_Transmit(I2Cx_Conf, (u8)(EE_Address&0x00ff));
	I2Cx_Start(I2Cx_Conf);
	I2Cx_Address_Direction(I2Cx_Conf,Chip_Address, I2C_Direction_Receiver);
	I2Cx_Receive_Ack(I2Cx_Conf,&data);
	I2Cx_Stop(I2Cx_Conf);
	return data;
}

void I2Cx_EE_WriteByte(struct I2Cx_Conf *I2Cx_Conf,u8 Chip_Address, u16 EE_Address, uint8_t data)
{
	I2Cx_Wait(I2Cx_Conf);
	I2Cx_Start(I2Cx_Conf);
	I2Cx_Address_Direction(I2Cx_Conf,Chip_Address, I2C_Direction_Transmitter);
	I2Cx_Transmit(I2Cx_Conf, (u8)((EE_Address >>8)&0x00ff));
	I2Cx_Transmit(I2Cx_Conf, (u8)(EE_Address&0x00ff));
	I2Cx_Transmit(I2Cx_Conf, data);
	I2Cx_Stop(I2Cx_Conf);
}



