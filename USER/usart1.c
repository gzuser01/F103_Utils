/***************************************
 * ------------------------
 * | PA9  - USART1(Tx)      |
 * | PA10 - USART1(Rx)      |
 * ------------------------
 * 
**********************************************************************************/


#include "usart1.h"
#include "gpio.h" 
#include "misc.h" 


/**
 * 串口中断回调函数的指针
 */
void (*_m_USART1_IRQHandler_Ptr[])(unsigned char) = {0,0,0,0,0,0,0,0,0,0};



void USART1_Config(GPIO_TypeDef *Tx_GPIOx,uint16_t Tx_GPIO_Pin,GPIO_TypeDef *Rx_GPIOx,uint16_t Rx_GPIO_Pin)
{

	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/* 使能 USART1 中断 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//选择分组方式2
  
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; 
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
	
	
	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	
	/* USART1 使用IO端口配置 */    
	GPIO_Config(Tx_GPIOx,Tx_GPIO_Pin,GPIO_Mode_AF_PP,GPIO_Speed_50MHz);//Tx 复用推挽输出
	GPIO_Config(Rx_GPIOx,Rx_GPIO_Pin,GPIO_Mode_IN_FLOATING,GPIO_Speed_50MHz);//Rx 浮空输入
	
	  
	/* USART1 工作模式配置 */
	USART_InitStruct.USART_BaudRate = 115200;	//波特率设置：115200
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStruct.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStruct.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART1, &USART_InitStruct);  //初始化USART1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能接受中断，在接受移位寄存器中有数据是产生
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);//使能发送中断，在发送完数据后产生。
	
	USART_Cmd(USART1, ENABLE);// USART1使能
	
	

}

 /*发送一个字节数据*/
void USART1_Send_Byte(unsigned char Send_Data)
{	   
	USART_SendData(USART1,Send_Data);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}  

/*接收一个字节数据*/
unsigned char USART1_Get_Byte(unsigned char* Get_Data)
{   	   
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{  
		
		return 0;//没有收到数据 
	}
	
	*Get_Data = USART_ReceiveData(USART1); 
	
	return 1;//收到数据
}

/*串口中断*/
void USART1_IRQHandler(void)
{
	int i;
	unsigned char c = 0;
		
	while(USART1_Get_Byte(&c))
	{
		
		
		for(i = 0;i<10;i++)
		{
			if(_m_USART1_IRQHandler_Ptr[i] == 0)
			{
				break;
			}

			(* _m_USART1_IRQHandler_Ptr[i])(c);
		}
	}
}


/**
 * 注册中断回调函数
 */
void Register_USART1_Callback(void (* ptr)(unsigned char))
{
	int i;
	for(i = 0;i<10;i++)
	{
		//已经注册
		if(_m_USART1_IRQHandler_Ptr[i] == ptr)
		{
			return;
		}
		//序号移动到未设置或未占用
		if(_m_USART1_IRQHandler_Ptr[i] == 0)
		{
			_m_USART1_IRQHandler_Ptr[i] = ptr;
			return;
		}
	}
}



