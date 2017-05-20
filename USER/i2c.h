
#ifndef __I2C_H
#define	__I2C_H

#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"

#define TRY_TIMES 1000


struct I2Cx_Conf
{
	uint32_t I2Cx_RCC;
	I2C_TypeDef* I2Cx;
	uint32_t I2C_GPIO_RCC;
	uint16_t I2C_PIN_SCL;
	uint16_t I2C_PIN_SDA;
	GPIO_TypeDef* I2C_GPIO;
	
};

void I2C1_Conf_Init(struct I2Cx_Conf *I2Cx_Conf);
void I2Cx_Init(struct I2Cx_Conf *I2Cx_Conf);

int8_t I2Cx_Wait(struct I2Cx_Conf *I2Cx_Conf);
int8_t I2Cx_Start(struct I2Cx_Conf *I2Cx_Conf);
int8_t I2Cx_Stop(struct I2Cx_Conf *I2Cx_Conf);

int8_t I2Cx_Address_Direction(struct I2Cx_Conf *I2Cx_Conf,uint8_t address, uint8_t direction);
int8_t I2Cx_Transmit(struct I2Cx_Conf *I2Cx_Conf,uint8_t byte);

int8_t I2Cx_Receive_Ack(struct I2Cx_Conf *I2Cx_Conf, uint8_t* data);
int8_t I2Cx_Receive_Nack(struct I2Cx_Conf *I2Cx_Conf, uint8_t* data);
	
int8_t I2Cx_Write(struct I2Cx_Conf *I2Cx_Conf,uint8_t address, uint8_t data);
int8_t I2Cx_Read(struct I2Cx_Conf *I2Cx_Conf,uint8_t address, uint8_t* data);


#endif 
