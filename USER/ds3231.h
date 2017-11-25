#ifndef __DS3231_H
#define	__DS3231_H

#include "i2c.h"


//T(sec)(min)(hour)(dayOfWeek)(dayOfMonth)(month)(year)
//T(00-59)(00-59)(00-23)(1-7)(01-31)(01-12)(00-99)

uint8_t _m_DS3231_Chip_Address = 0xD0;
uint8_t _m_DS3231_Sub_Address = 0x00;

int8_t _m_DS3231_I2C_Error_Code;

struct I2Cx_Conf DS3231_I2C1_Conf;

u8 bcd_to_dec(u8 bcdByte) {
  return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

u8 dec_to_bcd(u8 decByte) {
  return (((decByte / 10) << 4) | (decByte % 10));
}

uint16_t dayOfWeek(uint16_t year, uint16_t month, uint16_t day) {
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  year -= month < 3;
  return ((year + year/4 - year/100 + year/400 + t[month-1] + day) % 7); 
}


uint8_t DS3231_Init(uint8_t chip_address)
{
	uint16_t i;
	
	_m_DS3231_I2C_Error_Code = 0x00;
	
	while (I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)); // Wait until I2C free
	
	I2C_GenerateSTART(I2C1,ENABLE);
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT);i++) // Wait for EV5
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA0;
			return 0;
		}
	}
	
	I2C_Send7bitAddress(I2C1,chip_address,I2C_Direction_Transmitter); // Send DS3231 slave address
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);i++) // Wait for EV6
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA1;
			return 0;
		}
	}
	
	I2C_SendData(I2C1,0x0E); // Send DS3231 control register address
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++) // Wait for EV8
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA2;
			return 0;
		}
	}
	
	I2C_SendData(I2C1,0x00); // DS3231 EOSC enabled, INTCN enabled, SQW set to 1Hz
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++) // Wait for EV8
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA3;
			return 0;
		}
	}
	
	
	I2C_SendData(I2C1,0x00); // DS3231 clear alarm flags
	for (i=0;!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++) // Wait for EV8
	{
		if(i> TRY_TIMES)
		{
			_m_DS3231_I2C_Error_Code = 0xA4;
			return 0;
		}
	}
		
	I2C_GenerateSTOP(I2C1,ENABLE);
	return 1;
	
}



uint8_t DS3231_Read(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint8_t sub_address,uint8_t* data)
{
	_m_DS3231_I2C_Error_Code = 0x00;
	
	I2C1_Conf_Init(I2Cx_Conf_Struct);

	I2Cx_Ack_Enable(I2Cx_Conf_Struct);
	
	if(!I2Cx_Wait(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x01;
		//return 0;
	}
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x02;
		//return 0;
	}
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Transmitter))
	{
		_m_DS3231_I2C_Error_Code = 0x03;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, sub_address))
	{
		_m_DS3231_I2C_Error_Code = 0x04;
		return 0;
	}
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x05;
		return 0;
	}
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Receiver))
	{
		_m_DS3231_I2C_Error_Code = 0x06;
		return 0;
	}
	if(!I2Cx_Receives(I2Cx_Conf_Struct,data,7))
	{
		_m_DS3231_I2C_Error_Code = 0x07;
		return 0;
	}
	
	I2Cx_Ack_Disable(I2Cx_Conf_Struct);
		
	if(!I2Cx_Stop(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x09;
		return 0;
	}

	return 1;
	
}


uint8_t DS3231_Write(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t chip_address, uint8_t sub_address, uint8_t *data)
{
	uint8_t i;
	
	_m_DS3231_I2C_Error_Code = 0x00;
	
	I2C1_Conf_Init(I2Cx_Conf_Struct);

	I2Cx_Ack_Enable(I2Cx_Conf_Struct);
	
	if(!I2Cx_Wait(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x11;
		return 0;
	}
	if(!I2Cx_Start(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x12;
		return 0;
	}
	
	if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,chip_address, I2C_Direction_Transmitter))
	{
		_m_DS3231_I2C_Error_Code = 0x13;
		return 0;
	}
	
	if(!I2Cx_Transmit(I2Cx_Conf_Struct, sub_address))
	{
		_m_DS3231_I2C_Error_Code = 0x14;
		return 0;
	}
	
	for (i = 0; i < 7; i++) 
	{
		if(!I2Cx_Transmit(I2Cx_Conf_Struct, data[i]))
		{
			_m_DS3231_I2C_Error_Code = 0x15;
			return 0;
		}
	}
	
	I2Cx_Ack_Disable(I2Cx_Conf_Struct);
	
	if(!I2Cx_Stop(I2Cx_Conf_Struct))
	{
		_m_DS3231_I2C_Error_Code = 0x16;
		return 0;
	}
	return 1;
}


int8_t DS3231_I2C_Error_Code(void)
{
	return _m_DS3231_I2C_Error_Code;
}






#endif 
