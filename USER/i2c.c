
#include "i2c.h"



	
void I2C1_Conf_Init(struct I2Cx_Conf *i2cx_conf)
{
	
	i2cx_conf->i2cx_rcc = RCC_APB1Periph_I2C1;
	i2cx_conf->i2cx = I2C1;
	i2cx_conf->i2c_gpio_rcc = RCC_APB2Periph_GPIOB;
	i2cx_conf->i2c_pin_scl = GPIO_Pin_6;
	i2cx_conf->i2c_pin_sda = GPIO_Pin_7;
	i2cx_conf->i2c_gpio = GPIOB;
	
	I2Cx_Init(i2cx_conf);
}


void I2Cx_Init(struct I2Cx_Conf *i2cx_conf)
{
    // Initialization struct
    I2C_InitTypeDef i2c_init_struct;
    GPIO_InitTypeDef gpio_init_struct;
	
    //  Initialize GPIO 
    RCC_APB2PeriphClockCmd(i2cx_conf->i2c_gpio_rcc, ENABLE);
    gpio_init_struct.GPIO_Pin = i2cx_conf->i2c_pin_scl | i2cx_conf->i2c_pin_sda;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_AF_OD;
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(i2cx_conf->i2c_gpio, &gpio_init_struct);

    // Initialize I2C
    RCC_APB1PeriphClockCmd(i2cx_conf->i2cx_rcc, ENABLE);
		I2C_DeInit(i2cx_conf->i2cx);
    i2c_init_struct.I2C_ClockSpeed = 100000;
    i2c_init_struct.I2C_Mode = I2C_Mode_I2C;
    i2c_init_struct.I2C_DutyCycle = I2C_DutyCycle_2;
    i2c_init_struct.I2C_OwnAddress1 = 0x00;
    i2c_init_struct.I2C_Ack = I2C_Ack_Enable;
    i2c_init_struct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    
    I2C_Cmd(i2cx_conf->i2cx, ENABLE);
		I2C_Init(i2cx_conf->i2cx, &i2c_init_struct);


}

uint8_t I2Cx_Wait(struct I2Cx_Conf *i2cx_conf)
{
		uint16_t i;
	  // 
    for (i=0;I2C_GetFlagStatus(i2cx_conf->i2cx, I2C_FLAG_BUSY);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}
		return 1;
}

uint8_t I2Cx_Start(struct I2Cx_Conf *i2cx_conf)
{
		uint16_t i;
    // 
    I2C_GenerateSTART(i2cx_conf->i2cx, ENABLE);

    // 
    for (i=0;!I2C_CheckEvent(i2cx_conf->i2cx, I2C_EVENT_MASTER_MODE_SELECT);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}
		return 1;
}

uint8_t I2Cx_Stop(struct I2Cx_Conf *i2cx_conf)
{
		uint16_t i;
    // 
    I2C_GenerateSTOP(i2cx_conf->i2cx, ENABLE);
    // 
    for(i=0;I2C_GetFlagStatus(i2cx_conf->i2cx, I2C_FLAG_STOPF);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}
		return 1;
}

uint8_t I2Cx_Address_Direction(struct I2Cx_Conf *i2cx_conf,uint8_t address, uint8_t direction)
{
		uint16_t i;
    // 
    I2C_Send7bitAddress(i2cx_conf->i2cx, address, direction);

    // 
    if (direction == I2C_Direction_Transmitter)
    {
        for(i=0;!I2C_CheckEvent(i2cx_conf->i2cx,
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
        for(i=0;!I2C_CheckEvent(i2cx_conf->i2cx,
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

uint8_t I2Cx_Transmit(struct I2Cx_Conf *i2cx_conf,uint8_t byte)
{
		uint16_t i;
    // 
    I2C_SendData(i2cx_conf->i2cx, byte);
    // 
    for(i=0;!I2C_CheckEvent(i2cx_conf->i2cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}
		return 1;
}

void I2Cx_Ack_Enable(struct I2Cx_Conf *i2cx_conf)
{
    I2C_AcknowledgeConfig(i2cx_conf->i2cx, ENABLE);
}

void I2Cx_Ack_Disable(struct I2Cx_Conf *i2cx_conf)
{
    I2C_AcknowledgeConfig(i2cx_conf->i2cx, DISABLE);
}



uint8_t I2Cx_Receive_Ack(struct I2Cx_Conf *i2cx_conf,uint8_t* data)
{
		uint16_t i;
    // 
    I2C_AcknowledgeConfig(i2cx_conf->i2cx, ENABLE);
    // 
    for(i=0;!I2C_CheckEvent(i2cx_conf->i2cx, I2C_EVENT_MASTER_BYTE_RECEIVED);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}

    // 
    *data = I2C_ReceiveData(i2cx_conf->i2cx);
		return 1;
}


uint8_t I2Cx_Receives(struct I2Cx_Conf *i2cx_conf,uint8_t* data,uint8_t len)
{
		uint16_t i;
		uint8_t j;

		for(j=0;j<len;j++)
		{
			// 
			for(i=0;!I2C_CheckEvent(i2cx_conf->i2cx, I2C_EVENT_MASTER_BYTE_RECEIVED);i++)
			{
				if(i> TRY_TIMES)
				{
					return 0;
				}
			}

			// 
			data[j] = I2C_ReceiveData(i2cx_conf->i2cx);
		}
		return 1;
}


uint8_t I2Cx_Receive_Nack(struct I2Cx_Conf *i2cx_conf,uint8_t* data)
{
		uint16_t i;
    // 
    I2C_AcknowledgeConfig(i2cx_conf->i2cx, DISABLE);
    // 
    for(i=0;!I2C_CheckEvent(i2cx_conf->i2cx, I2C_EVENT_MASTER_BYTE_RECEIVED);i++)
		{
			if(i> TRY_TIMES)
			{
				return 0;
			}
		}

    // 
    *data = I2C_ReceiveData(i2cx_conf->i2cx);
		
		return 1;
}

uint8_t I2Cx_Write(struct I2Cx_Conf *i2cx_conf,uint8_t address, uint8_t data)
{
    if(!I2Cx_Start(i2cx_conf))
		{
			return 0;
		}
    if(!I2Cx_Address_Direction(i2cx_conf,address, I2C_Direction_Transmitter))
		{
			return 0;
		}
    if(!I2Cx_Transmit(i2cx_conf,data))
		{
			return 0;
		}
    if(!I2Cx_Stop(i2cx_conf))
		{
			return 0;
		}
		return 1;
}

uint8_t I2Cx_Read(struct I2Cx_Conf *i2cx_conf,uint8_t address, uint8_t* data)
{
    if(!I2Cx_Start(i2cx_conf))
		{
			return 0;
		}
    if(!I2Cx_Address_Direction(i2cx_conf,address, I2C_Direction_Receiver))
		{
			return 0;
		}
    if(!I2Cx_Receive_Nack(i2cx_conf,data))
		{
			return 0;
		}
    if(!I2Cx_Stop(i2cx_conf))
		{
			return 0;
		}
		return 1;
}



