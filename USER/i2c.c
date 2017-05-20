
#include "i2c.h"
#include "usart1_util.h"

	
void I2C1_Conf_Init(struct I2Cx_Conf *I2Cx_Conf)
{
	
	I2Cx_Conf->I2Cx_RCC = RCC_APB1Periph_I2C1;
	I2Cx_Conf->I2Cx = I2C1;
	I2Cx_Conf->I2C_GPIO_RCC = RCC_APB2Periph_GPIOB;
	I2Cx_Conf->I2C_PIN_SCL = GPIO_Pin_6;
	I2Cx_Conf->I2C_PIN_SDA = GPIO_Pin_7;
	I2Cx_Conf->I2C_GPIO = GPIOB;
	
	I2Cx_Init(I2Cx_Conf);
}


void I2Cx_Init(struct I2Cx_Conf *I2Cx_Conf)
{
    // Initialization struct
    I2C_InitTypeDef I2C_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    // Initialize I2C
    RCC_APB1PeriphClockCmd(I2Cx_Conf->I2Cx_RCC, ENABLE);
    I2C_InitStruct.I2C_ClockSpeed = 400000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2Cx_Conf->I2Cx, &I2C_InitStruct);
    I2C_Cmd(I2Cx_Conf->I2Cx, ENABLE);
 
    //  Initialize GPIO 
    RCC_APB2PeriphClockCmd(I2Cx_Conf->I2C_GPIO_RCC, ENABLE);
    GPIO_InitStruct.GPIO_Pin = I2Cx_Conf->I2C_PIN_SCL | I2Cx_Conf->I2C_PIN_SDA;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2Cx_Conf->I2C_GPIO, &GPIO_InitStruct);
}

int8_t I2Cx_Wait(struct I2Cx_Conf *I2Cx_Conf)
{
		u16 i;
	  // 
    for (i=0;I2C_GetFlagStatus(I2Cx_Conf->I2Cx, I2C_FLAG_BUSY);i++)
		{
			if(i> TRY_TIMES)
			{
				return -1;
			}
		}
		return 0;
}

int8_t I2Cx_Start(struct I2Cx_Conf *I2Cx_Conf)
{
		u16 i;
    // 
    I2C_GenerateSTART(I2Cx_Conf->I2Cx, ENABLE);

    // 
    for (i=0;!I2C_CheckEvent(I2Cx_Conf->I2Cx, I2C_EVENT_MASTER_MODE_SELECT);i++)
		{
			if(i> TRY_TIMES)
			{
				return -1;
			}
		}
		return 0;
}

int8_t I2Cx_Stop(struct I2Cx_Conf *I2Cx_Conf)
{
		u16 i;
    // 
    I2C_GenerateSTOP(I2Cx_Conf->I2Cx, ENABLE);
    // 
    for(i=0;I2C_GetFlagStatus(I2Cx_Conf->I2Cx, I2C_FLAG_STOPF);i++)
		{
			if(i> TRY_TIMES)
			{
				return -1;
			}
		}
		return 0;
}

int8_t I2Cx_Address_Direction(struct I2Cx_Conf *I2Cx_Conf,uint8_t address, uint8_t direction)
{
		u16 i;
    // 
    I2C_Send7bitAddress(I2Cx_Conf->I2Cx, address, direction);

    // 
    if (direction == I2C_Direction_Transmitter)
    {
        for(i=0;!I2C_CheckEvent(I2Cx_Conf->I2Cx,
            I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);i++)
				{
					if(i> TRY_TIMES)
					{
						return -1;
					}
				}
    }
    else if (direction == I2C_Direction_Receiver)
    { 
        for(i=0;!I2C_CheckEvent(I2Cx_Conf->I2Cx,
            I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);i++)
				{
					if(i> TRY_TIMES)
					{
						return -1;
					}
				}
    }
		return 0;
}

int8_t I2Cx_Transmit(struct I2Cx_Conf *I2Cx_Conf,uint8_t byte)
{
		u16 i;
    // 
    I2C_SendData(I2Cx_Conf->I2Cx, byte);
    // 
    for(i=0;!I2C_CheckEvent(I2Cx_Conf->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++)
		{
			if(i> TRY_TIMES)
			{
				return -1;
			}
		}
		return 0;
}

int8_t I2Cx_Receive_Ack(struct I2Cx_Conf *I2Cx_Conf,uint8_t* data)
{
		u16 i;
    // 
    I2C_AcknowledgeConfig(I2Cx_Conf->I2Cx, ENABLE);
    // 
    for(i=0;!I2C_CheckEvent(I2Cx_Conf->I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED);i++)
		{
			if(i> TRY_TIMES)
			{
				return -1;
			}
		}

    // 
    *data = I2C_ReceiveData(I2Cx_Conf->I2Cx);
		return 0;
}

int8_t I2Cx_Receive_Nack(struct I2Cx_Conf *I2Cx_Conf,uint8_t* data)
{
		u16 i;
    // 
    I2C_AcknowledgeConfig(I2Cx_Conf->I2Cx, DISABLE);
    // 
    for(i=0;!I2C_CheckEvent(I2Cx_Conf->I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED);i++)
		{
			if(i> TRY_TIMES)
			{
				return -1;
			}
		}

    // 
    *data = I2C_ReceiveData(I2Cx_Conf->I2Cx);
		
		return 0;
}

int8_t I2Cx_Write(struct I2Cx_Conf *I2Cx_Conf,uint8_t address, uint8_t data)
{
    if(I2Cx_Start(I2Cx_Conf) == -1)
		{
			return -1;
		}
    if(I2Cx_Address_Direction(I2Cx_Conf,address, I2C_Direction_Transmitter))
		{
			return -1;
		}
    if(I2Cx_Transmit(I2Cx_Conf,data))
		{
			return -1;
		}
    if(I2Cx_Stop(I2Cx_Conf))
		{
			return -1;
		}
		return 0;
}

int8_t I2Cx_Read(struct I2Cx_Conf *I2Cx_Conf,uint8_t address, uint8_t* data)
{
    if(I2Cx_Start(I2Cx_Conf))
		{
			return -1;
		}
    if(I2Cx_Address_Direction(I2Cx_Conf,address, I2C_Direction_Receiver))
		{
			return -1;
		}
    if(I2Cx_Receive_Nack(I2Cx_Conf,data))
		{
			return -1;
		}
    if(I2Cx_Stop(I2Cx_Conf))
		{
			return -1;
		}
		return 0;
}

