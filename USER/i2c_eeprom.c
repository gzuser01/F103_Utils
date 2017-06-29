
#include "i2c_eeprom.h"

int8_t i2cx_ee_error_code;

int8_t I2Cx_EE_ReadByte(struct I2Cx_Conf *i2cx_conf,uint8_t chip_address, uint16_t ee_address,uint8_t* data)
{
	
	if(!I2Cx_Wait(i2cx_conf))
	{
		i2cx_ee_error_code = 0x01;
		return 0;
	}
	
	if(!I2Cx_Start(i2cx_conf))
	{
		i2cx_ee_error_code = 0x02;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(i2cx_conf,chip_address, I2C_Direction_Transmitter))
	{
		i2cx_ee_error_code = 0x03;
		return 0;
	}
	
	if(!I2Cx_Transmit(i2cx_conf, (u8)((ee_address >>8) & 0x00ff)))
	{
		i2cx_ee_error_code = 0x04;
		return 0;
	}
	
	if(!I2Cx_Transmit(i2cx_conf, (u8)(ee_address & 0x00ff)))
	{
		i2cx_ee_error_code = 0x05;
		return 0;
	}
	
	if(!I2Cx_Start(i2cx_conf))
	{
		i2cx_ee_error_code = 0x06;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(i2cx_conf,chip_address, I2C_Direction_Receiver))
	{
		i2cx_ee_error_code = 0x07;
		return 0;
	}
	
	if(!I2Cx_Receive_Ack(i2cx_conf,data))
	{
		i2cx_ee_error_code = 0x08;
		return 0;
	}
	
	if(!I2Cx_Stop(i2cx_conf))
	{
		i2cx_ee_error_code = 0x09;
		return 0;
	}
	
	i2cx_ee_error_code = 0;
	return 1;
}

int8_t I2Cx_EE_WriteByte(struct I2Cx_Conf *i2cx_conf,uint8_t chip_address, uint16_t ee_address, uint8_t data)
{
	if(!I2Cx_Wait(i2cx_conf))
	{
		i2cx_ee_error_code = 0x11;
		return 0;
	}
	
	if(!I2Cx_Start(i2cx_conf))
	{
		i2cx_ee_error_code = 0x12;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(i2cx_conf,chip_address, I2C_Direction_Transmitter))
	{
		i2cx_ee_error_code = 0x13;
		return 0;
	}
	
	if(!I2Cx_Transmit(i2cx_conf, (u8)((ee_address >>8) & 0x00ff)))
	{
		i2cx_ee_error_code = 0x14;
		return 0;
	}
	
	if(!I2Cx_Transmit(i2cx_conf, (u8)(ee_address & 0x00ff)))
	{
		i2cx_ee_error_code = 0x15;
		return 0;
	}
	
	if(!I2Cx_Transmit(i2cx_conf, data))
	{
		i2cx_ee_error_code = 0x16;
		return 0;
	}
	
	if(!I2Cx_Stop(i2cx_conf))
	{
		i2cx_ee_error_code = 0x17;
		return 0;
	}
	
	i2cx_ee_error_code = 0;
	return 1;
}

int8_t I2Cx_EE_Error_Code(void)
{
	return i2cx_ee_error_code;
}


