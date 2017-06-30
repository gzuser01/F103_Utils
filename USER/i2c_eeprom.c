
#include "i2c_eeprom.h"

int8_t _m_I2C_EE_Error_Code;

int8_t I2Cx_EE_ReadByte(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint16_t ee_address,uint8_t* data)
{
	
	if(!I2Cx_Wait(I2Cx_Conf_Struct))
	{
		_m_I2C_EE_Error_Code = 0x01;
		return 0;
	}
	
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_I2C_EE_Error_Code = 0x02;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Transmitter))
	{
		_m_I2C_EE_Error_Code = 0x03;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, (u8)((ee_address >>8) & 0x00ff)))
	{
		_m_I2C_EE_Error_Code = 0x04;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, (u8)(ee_address & 0x00ff)))
	{
		_m_I2C_EE_Error_Code = 0x05;
		return 0;
	}
	
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_I2C_EE_Error_Code = 0x06;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Receiver))
	{
		_m_I2C_EE_Error_Code = 0x07;
		return 0;
	}
	
	if(!I2Cx_Receive_Ack(I2Cx_Conf_Struct,data))
	{
		_m_I2C_EE_Error_Code = 0x08;
		return 0;
	}
	
	if(!I2Cx_Stop(I2Cx_Conf_Struct))
	{
		_m_I2C_EE_Error_Code = 0x09;
		return 0;
	}
	
	_m_I2C_EE_Error_Code = 0;
	return 1;
}

int8_t I2Cx_EE_WriteByte(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint16_t ee_address, uint8_t data)
{
	if(!I2Cx_Wait(I2Cx_Conf_Struct))
	{
		_m_I2C_EE_Error_Code = 0x11;
		return 0;
	}
	
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_I2C_EE_Error_Code = 0x12;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Transmitter))
	{
		_m_I2C_EE_Error_Code = 0x13;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, (u8)((ee_address >>8) & 0x00ff)))
	{
		_m_I2C_EE_Error_Code = 0x14;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, (u8)(ee_address & 0x00ff)))
	{
		_m_I2C_EE_Error_Code = 0x15;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, data))
	{
		_m_I2C_EE_Error_Code = 0x16;
		return 0;
	}
	
	if(!I2Cx_Stop(I2Cx_Conf_Struct))
	{
		_m_I2C_EE_Error_Code = 0x17;
		return 0;
	}
	
	_m_I2C_EE_Error_Code = 0;
	return 1;
}

int8_t I2Cx_EE_Error_Code(void)
{
	return _m_I2C_EE_Error_Code;
}


