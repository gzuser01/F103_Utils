
#include "i2c.h"



	
void I2C1_Conf_Init(struct I2Cx_Conf *I2Cx_Conf_Struct)
{
	
	I2Cx_Conf_Struct->I2C_RCC = RCC_APB1Periph_I2C1;
	I2Cx_Conf_Struct->I2Cx = I2C1;
	I2Cx_Conf_Struct->I2C_GPIO_RCC = RCC_APB2Periph_GPIOB;
	I2Cx_Conf_Struct->I2C_SCL_Pin = GPIO_Pin_6;
	I2Cx_Conf_Struct->I2C_SDA_Pin = GPIO_Pin_7;
	I2Cx_Conf_Struct->I2C_GPIO = GPIOB;
	
	I2Cx_Init(I2Cx_Conf_Struct);
}


void I2Cx_Init(struct I2Cx_Conf *I2Cx_Conf_Struct)
{
    // Initialization struct
    I2C_InitTypeDef i2c_init_struct;
    GPIO_InitTypeDef gpio_init_struct;
	
    //  Initialize GPIO 
    RCC_APB2PeriphClockCmd(I2Cx_Conf_Struct->I2C_GPIO_RCC, ENABLE);
    gpio_init_struct.GPIO_Pin = I2Cx_Conf_Struct->I2C_SCL_Pin | I2Cx_Conf_Struct->I2C_SDA_Pin;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_OD;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(I2Cx_Conf_Struct->I2C_GPIO, &gpio_init_struct);

    // Initialize I2C
    RCC_APB1PeriphClockCmd(I2Cx_Conf_Struct->I2C_RCC, ENABLE);
		I2C_DeInit(I2Cx_Conf_Struct->I2Cx);
    i2c_init_struct.I2C_ClockSpeed = 100000;
    i2c_init_struct.I2C_Mode = I2C_Mode_I2C;
    i2c_init_struct.I2C_DutyCycle = I2C_DutyCycle_2;
    i2c_init_struct.I2C_OwnAddress1 = 0x00;
    i2c_init_struct.I2C_Ack = I2C_Ack_Enable;
    i2c_init_struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    
    I2C_Cmd(I2Cx_Conf_Struct->I2Cx, ENABLE);
		I2C_Init(I2Cx_Conf_Struct->I2Cx, &i2c_init_struct);


}

uint8_t I2Cx_Wait(struct I2Cx_Conf *I2Cx_Conf_Struct)
{
		uint16_t i;
	  // 
    for (i=0;I2C_GetFlagStatus(I2Cx_Conf_Struct->I2Cx, I2C_FLAG_BUSY);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}
		return 1;
}

uint8_t I2Cx_Start(struct I2Cx_Conf *I2Cx_Conf_Struct)
{
		uint16_t i;
    // 
    I2C_GenerateSTART(I2Cx_Conf_Struct->I2Cx, ENABLE);

    // 
    for (i=0;!I2C_CheckEvent(I2Cx_Conf_Struct->I2Cx, I2C_EVENT_MASTER_MODE_SELECT);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}
		return 1;
}

uint8_t I2Cx_Stop(struct I2Cx_Conf *I2Cx_Conf_Struct)
{
		uint16_t i;
    // 
    I2C_GenerateSTOP(I2Cx_Conf_Struct->I2Cx, ENABLE);
    // 
    for(i=0;I2C_GetFlagStatus(I2Cx_Conf_Struct->I2Cx, I2C_FLAG_STOPF);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}
		return 1;
}

uint8_t I2Cx_Address_Direction(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t address, uint8_t direction)
{
		uint16_t i;
    // 
    I2C_Send7bitAddress(I2Cx_Conf_Struct->I2Cx, address, direction);

    // 
    if (direction == I2C_Direction_Transmitter)
    {
        for(i=0;!I2C_CheckEvent(I2Cx_Conf_Struct->I2Cx,
            I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);i++)
				{
					if(i> TRY_TIMES)
					{
						return 0;
					}
				}
    }
    else if (direction == I2C_Direction_Receiver)
    { 
        for(i=0;!I2C_CheckEvent(I2Cx_Conf_Struct->I2Cx,
            I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);i++)
				{
					if(i> TRY_TIMES)
					{
						return 0;
					}
				}
    }
		return 1;
}

uint8_t I2Cx_Transmit(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t byte)
{
		uint16_t i;
    // 
    I2C_SendData(I2Cx_Conf_Struct->I2Cx, byte);
    // 
    for(i=0;!I2C_CheckEvent(I2Cx_Conf_Struct->I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}
		return 1;
}

void I2Cx_Ack_Enable(struct I2Cx_Conf *I2Cx_Conf_Struct)
{
    I2C_AcknowledgeConfig(I2Cx_Conf_Struct->I2Cx, ENABLE);
}

void I2Cx_Ack_Disable(struct I2Cx_Conf *I2Cx_Conf_Struct)
{
    I2C_AcknowledgeConfig(I2Cx_Conf_Struct->I2Cx, DISABLE);
}



uint8_t I2Cx_Receive_Ack(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t* data)
{
		uint16_t i;
    // 
    I2C_AcknowledgeConfig(I2Cx_Conf_Struct->I2Cx, ENABLE);
    // 
    for(i=0;!I2C_CheckEvent(I2Cx_Conf_Struct->I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}

    // 
    *data = I2C_ReceiveData(I2Cx_Conf_Struct->I2Cx);
		return 1;
}


uint8_t I2Cx_Receives(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t* data,uint8_t len)
{
		uint16_t i;
		uint8_t j;

		for(j=0;j<len;j++)
		{
			// 
			for(i=0;!I2C_CheckEvent(I2Cx_Conf_Struct->I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED);i++)
			{
				if(i> TRY_TIMES)
				{
					return 0;
				}
			}

			// 
			data[j] = I2C_ReceiveData(I2Cx_Conf_Struct->I2Cx);
		}
		return 1;
}


uint8_t I2Cx_Receive_Nack(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t* data)
{
		uint16_t i;
    // 
    I2C_AcknowledgeConfig(I2Cx_Conf_Struct->I2Cx, DISABLE);
    // 
    for(i=0;!I2C_CheckEvent(I2Cx_Conf_Struct->I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}

    // 
    *data = I2C_ReceiveData(I2Cx_Conf_Struct->I2Cx);
		
		return 1;
}

uint8_t I2Cx_Write(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t address, uint8_t data)
{
    if(!I2Cx_Start(I2Cx_Conf_Struct))
		{
			return 0;
		}
    if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,address, I2C_Direction_Transmitter))
		{
			return 0;
		}
    if(!I2Cx_Transmit(I2Cx_Conf_Struct,data))
		{
			return 0;
		}
    if(!I2Cx_Stop(I2Cx_Conf_Struct))
		{
			return 0;
		}
		return 1;
}

uint8_t I2Cx_Read(struct I2Cx_Conf *I2Cx_Conf_Struct,uint8_t address, uint8_t* data)
{
    if(!I2Cx_Start(I2Cx_Conf_Struct))
		{
			return 0;
		}
    if(!I2Cx_Address_Direction(I2Cx_Conf_Struct,address, I2C_Direction_Receiver))
		{
			return 0;
		}
    if(!I2Cx_Receive_Nack(I2Cx_Conf_Struct,data))
		{
			return 0;
		}
    if(!I2Cx_Stop(I2Cx_Conf_Struct))
		{
			return 0;
		}
		return 1;
}



